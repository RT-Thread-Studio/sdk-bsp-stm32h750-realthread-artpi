/*************************************************
 All rights reserved.
 File name:     agile_telnet.c
 Description:   agile_telnet源码
 History:
 1. Version:      v1.0.0
    Date:         2020-02-27
    Author:       Longwei Ma
    Modification: 新建版本

 2. Version:      v2.0.0
    Date:         2020-08-02
    Author:       Longwei Ma
    Modification: 将agile_telnet作为agile_console的一个插件

 3. Version:      v2.0.1
    Date:         2021-04-07
    Author:       Longwei Ma
    Modification: 将telnet的tx_rb做成devfs，使用select监听是否有数据

 4. Version:      v2.0.2
    Date:         2021-04-10
    Author:       Longwei Ma
    Modification: 使用 agile_console_wakeup
                  增加登录验证用户功能

 5. Version:      v2.0.3
    Date:         2022-01-09
    Author:       Longwei Ma
    Modification: 兼容 RT-Thread 4.1.0
*************************************************/

#include <rthw.h>

#ifdef PKG_USING_AGILE_TELNET

#if (RT_VER_NUM < 0x40003) || (RT_VER_NUM == 0x40004)
#error "Not support."
#endif

#ifndef PKG_USING_AGILE_CONSOLE
#error "Please enable agile_console package"
#endif

#include <agile_telnet.h>
#include <agile_console.h>

#if RT_VER_NUM < 0x40100
#include <dfs_posix.h>
#include <dfs_poll.h>
#else
#include <dfs_file.h>
#include <poll.h>
#include <unistd.h>
#endif
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>

/* 线程堆栈大小 */
#ifndef PKG_AGILE_TELNET_THREAD_STACK_SIZE
#define PKG_AGILE_TELNET_THREAD_STACK_SIZE 2048
#endif

/* 线程优先级 */
#ifndef PKG_AGILE_TELNET_THREAD_PRIORITY
#define PKG_AGILE_TELNET_THREAD_PRIORITY RT_THREAD_PRIORITY_MAX - 6
#endif

/* 监听端口 */
#ifndef PKG_AGILE_TELNET_PORT
#define PKG_AGILE_TELNET_PORT 23
#endif

/* 接收缓冲区大小 */
#ifndef PKG_AGILE_TELNET_RX_BUFFER_SIZE
#define PKG_AGILE_TELNET_RX_BUFFER_SIZE 256
#endif

/* 发送缓冲区大小 */
#ifndef PKG_AGILE_TELNET_TX_BUFFER_SIZE
#define PKG_AGILE_TELNET_TX_BUFFER_SIZE 2048
#endif

/* 客户端空闲超时时间(单位：min) */
#ifndef PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT
#define PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT 3
#endif

/* 登录验证 */
#ifdef PKG_AGILE_TELNET_USING_AUTH
#ifndef PKG_AGILE_TELNET_USERNAME
#define PKG_AGILE_TELNET_USERNAME "loogg"
#endif
#ifndef PKG_AGILE_TELNET_PASSWORD
#define PKG_AGILE_TELNET_PASSWORD "loogg"
#endif
#endif

static struct agile_telnet telnet = {0};
static struct agile_console_backend telnet_backend = {0};

RT_WEAK rt_size_t rt_ringbuffer_peak(struct rt_ringbuffer *rb, rt_uint8_t **ptr)
{
    RT_ASSERT(rb != RT_NULL);

    *ptr = RT_NULL;

    /* whether has enough data  */
    rt_size_t size = rt_ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    *ptr = &rb->buffer_ptr[rb->read_index];

    if (rb->buffer_size - rb->read_index > size) {
        rb->read_index += size;
        return size;
    }

    size = rb->buffer_size - rb->read_index;

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = 0;

    return size;
}

#ifdef PKG_AGILE_TELNET_USING_AUTH
static void telnet_client_process(rt_uint8_t *recv_buf, int recv_len)
{
    static rt_uint16_t index = 0;

    switch (telnet.state) {
    case AGILE_TELNET_STATE_USER: {
        if (telnet.username[0] == '\0')
            index = 0;

        rt_uint8_t change = 0;
        for (int i = 0; i < recv_len; i++) {
            if (recv_buf[i] == '\r') {
                change = 1;
                break;
            }

            if (index == sizeof(telnet.username) - 1)
                break;
            telnet.username[index++] = recv_buf[i];
        }

        if (change) {
            index = 0;
            telnet.state = AGILE_TELNET_STATE_PASSWORD;
            const char *format = "\r\npassword:";
            send(telnet.client_fd, format, strlen(format), 0);
        }
    } break;

    case AGILE_TELNET_STATE_PASSWORD: {
        rt_uint8_t change = 0;
        for (int i = 0; i < recv_len; i++) {
            if (recv_buf[i] == '\r') {
                change = 1;
                break;
            }

            if (index == sizeof(telnet.password) - 1)
                break;
            telnet.password[index++] = recv_buf[i];
        }

        if (change) {
            int result = -RT_ERROR;
            do {
                if (rt_strcmp(telnet.username, PKG_AGILE_TELNET_USERNAME))
                    break;
                if (rt_strcmp(telnet.password, PKG_AGILE_TELNET_PASSWORD))
                    break;

                result = RT_EOK;
            } while (0);

            if (result != RT_EOK) {
                telnet.state = AGILE_TELNET_STATE_USER;
                rt_memset(telnet.username, 0, sizeof(telnet.username));
                rt_memset(telnet.password, 0, sizeof(telnet.password));

                const char *format = "\r\nAuth failed!\r\n\r\nusername:";
                send(telnet.client_fd, format, strlen(format), 0);
            } else {
                telnet.state = AGILE_TELNET_STATE_PROCESS;
                rt_base_t level = rt_hw_interrupt_disable();
                rt_ringbuffer_reset(telnet.tx_rb);
                rt_hw_interrupt_enable(level);

                telnet.isconnected = 1;

                const char *format = "\r\nLogin Successful.\r\n";
                send(telnet.client_fd, format, strlen(format), 0);
            }
        }
    } break;

    default:
        break;
    }
}
#endif

/* telnet server thread entry */
static void telnet_thread(void *parameter)
{
#define RECV_BUF_LEN 64

    int enable = 1;
    int flags;
    struct sockaddr_in addr;
    rt_uint8_t recv_buf[RECV_BUF_LEN];
    rt_tick_t client_tick_timeout = rt_tick_get();

    // select使用
    fd_set readset, exceptset;
    // select超时时间
    struct timeval select_timeout;
    select_timeout.tv_sec = 10;
    select_timeout.tv_usec = 0;

    rt_thread_mdelay(5000);
_telnet_start:
    telnet.server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (telnet.server_fd < 0)
        goto _telnet_restart;

    if (setsockopt(telnet.server_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&enable, sizeof(enable)) < 0)
        goto _telnet_restart;

    rt_memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PKG_AGILE_TELNET_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(telnet.server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        goto _telnet_restart;

    if (listen(telnet.server_fd, 1) < 0)
        goto _telnet_restart;

    flags = fcntl(telnet.server_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(telnet.server_fd, F_SETFL, flags);

    while (1) {
        FD_ZERO(&readset);
        FD_ZERO(&exceptset);

        FD_SET(telnet.server_fd, &readset);
        FD_SET(telnet.server_fd, &exceptset);

        int max_fd = telnet.server_fd;
        if (telnet.client_fd >= 0) {
            FD_SET(telnet.client_fd, &readset);
            FD_SET(telnet.client_fd, &exceptset);

            if (max_fd < telnet.client_fd)
                max_fd = telnet.client_fd;
        }
        if (telnet.isconnected) {
            FD_SET(telnet.tx_fd, &readset);

            if (max_fd < telnet.tx_fd)
                max_fd = telnet.tx_fd;
        }

        int rc = select(max_fd + 1, &readset, RT_NULL, &exceptset, &select_timeout);
        if (rc < 0)
            break;
        if (rc > 0) {
            //服务器事件
            if (FD_ISSET(telnet.server_fd, &exceptset))
                break;

            if (FD_ISSET(telnet.server_fd, &readset)) {
                socklen_t addrlen = sizeof(struct sockaddr_in);
                int new_s = accept(telnet.server_fd, (struct sockaddr *)&addr, &addrlen);
                if (new_s < 0)
                    break;

                if (telnet.client_fd >= 0) {
                    telnet.isconnected = 0;
                    close(telnet.client_fd);
                    telnet.client_fd = -1;
                }

                int option = 1;
                setsockopt(new_s, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int));
                struct timeval tv;
                tv.tv_sec = 5;
                tv.tv_usec = 0;
                setsockopt(new_s, SOL_SOCKET, SO_SNDTIMEO, (const void *)&tv, sizeof(struct timeval));

                telnet.client_fd = new_s;
                telnet.client_timeout = PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT;
                client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);

#ifndef PKG_AGILE_TELNET_USING_AUTH
                rt_base_t level = rt_hw_interrupt_disable();
                rt_ringbuffer_reset(telnet.tx_rb);
                rt_hw_interrupt_enable(level);

                telnet.isconnected = 1;

                const char *format = "Login Successful.\r\n";
                send(telnet.client_fd, format, strlen(format), 0);
#else
                telnet.state = AGILE_TELNET_STATE_USER;
                rt_memset(telnet.username, 0, sizeof(telnet.username));
                rt_memset(telnet.password, 0, sizeof(telnet.password));

                const char *format = "\r\nusername:";
                send(telnet.client_fd, format, strlen(format), 0);
#endif

                continue;
            }

            // 客户端事件
            if (telnet.client_fd >= 0) {
                if (FD_ISSET(telnet.client_fd, &exceptset)) {
                    telnet.isconnected = 0;
                    close(telnet.client_fd);
                    telnet.client_fd = -1;
                } else if (FD_ISSET(telnet.client_fd, &readset)) {
                    int recv_len = recv(telnet.client_fd, recv_buf, RECV_BUF_LEN, MSG_DONTWAIT);
                    if (recv_len <= 0) {
                        telnet.isconnected = 0;
                        close(telnet.client_fd);
                        telnet.client_fd = -1;
                    } else {
#ifdef PKG_AGILE_TELNET_USING_AUTH
                        telnet_client_process(recv_buf, recv_len);
                        if (telnet.state == AGILE_TELNET_STATE_PROCESS) {
#endif
                            rt_base_t level = rt_hw_interrupt_disable();
                            rt_ringbuffer_put(telnet.rx_rb, recv_buf, recv_len);
                            rt_hw_interrupt_enable(level);

                            agile_console_wakeup();
#ifdef PKG_AGILE_TELNET_USING_AUTH
                        }
#endif

                        client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);
                    }
                }
            }

            if (telnet.isconnected && FD_ISSET(telnet.tx_fd, &readset)) {
                rt_uint8_t *send_ptr = RT_NULL;
                rt_base_t level = rt_hw_interrupt_disable();
                int send_len = rt_ringbuffer_peak(telnet.tx_rb, &send_ptr);
                rt_hw_interrupt_enable(level);

                if (send_len > 0) {
                    send(telnet.client_fd, send_ptr, send_len, 0);
                    client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);
                }
            }
        }

        if (telnet.client_fd >= 0) {
            if ((rt_tick_get() - client_tick_timeout) < (RT_TICK_MAX / 2)) {
                telnet.isconnected = 0;
                close(telnet.client_fd);
                telnet.client_fd = -1;
            }
        }
    }

_telnet_restart:
    telnet.isconnected = 0;
    if (telnet.server_fd >= 0) {
        close(telnet.server_fd);
        telnet.server_fd = -1;
    }
    if (telnet.client_fd >= 0) {
        close(telnet.client_fd);
        telnet.client_fd = -1;
    }

    rt_thread_mdelay(10000);
    goto _telnet_start;
}

static void telnet_backend_output(rt_device_t dev, const uint8_t *buf, int len)
{
    if (telnet.isconnected == 0)
        return;

    int put_len = 0;

    rt_base_t level = rt_hw_interrupt_disable();

    if (dev->open_flag & RT_DEVICE_FLAG_STREAM) {
        while (len > 0) {
            if (*buf == '\n') {
                if (rt_ringbuffer_putchar(telnet.tx_rb, '\r') == 0)
                    break;
            }

            if (rt_ringbuffer_putchar(telnet.tx_rb, *buf) == 0)
                break;

            put_len++;
            ++buf;
            --len;
        }
    } else
        put_len = rt_ringbuffer_put(telnet.tx_rb, buf, len);

    rt_hw_interrupt_enable(level);

    if (put_len > 0)
        rt_wqueue_wakeup(&(telnet.tlnt_dev->wait_queue), (void *)POLLIN);
}

static int telnet_backend_read(rt_device_t dev, uint8_t *buf, int len)
{
    if (telnet.isconnected == 0)
        return 0;

    rt_size_t result = 0;

    rt_base_t level = rt_hw_interrupt_disable();
    result = rt_ringbuffer_get(telnet.rx_rb, buf, len);
    rt_hw_interrupt_enable(level);

    return result;
}

static int tlnt_fops_open(struct dfs_fd *fd)
{
    rt_device_t device = (rt_device_t)fd->data;
    RT_ASSERT(device != RT_NULL);

    device->ref_count++;

    return 0;
}

static int tlnt_fops_poll(struct dfs_fd *fd, rt_pollreq_t *req)
{
    int mask = 0;
    rt_device_t device = (rt_device_t)fd->data;
    RT_ASSERT(device != RT_NULL);

    rt_poll_add(&(device->wait_queue), req);

    if (rt_ringbuffer_data_len(telnet.tx_rb) != 0)
        mask |= POLLIN;

    return mask;
}

static const struct dfs_file_ops tlnt_fops =
    {
        tlnt_fops_open,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        tlnt_fops_poll,
};

static int agile_telnet_init(void)
{
    telnet.isconnected = 0;
    telnet.server_fd = -1;
    telnet.client_fd = -1;
    telnet.client_timeout = PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT;
    telnet.tx_fd = -1;

    telnet.rx_rb = rt_ringbuffer_create(PKG_AGILE_TELNET_RX_BUFFER_SIZE);
    RT_ASSERT(telnet.rx_rb != RT_NULL);

    telnet.tx_rb = rt_ringbuffer_create(PKG_AGILE_TELNET_TX_BUFFER_SIZE);
    RT_ASSERT(telnet.tx_rb != RT_NULL);

    telnet.tlnt_dev = rt_malloc(sizeof(struct rt_device));
    rt_memset(telnet.tlnt_dev, 0, sizeof(struct rt_device));
    telnet.tlnt_dev->type = RT_Device_Class_Miscellaneous;
    rt_device_register(telnet.tlnt_dev, "tlnt", RT_DEVICE_FLAG_RDWR);
    telnet.tlnt_dev->fops = &tlnt_fops;

    telnet.tx_fd = open("/dev/tlnt", O_RDWR, 0);
    RT_ASSERT(telnet.tx_fd >= 0);

    rt_thread_t tid = rt_thread_create("telnet", telnet_thread, RT_NULL, PKG_AGILE_TELNET_THREAD_STACK_SIZE,
                                       PKG_AGILE_TELNET_THREAD_PRIORITY, 100);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    telnet_backend.output = telnet_backend_output;
    telnet_backend.read = telnet_backend_read;

    agile_console_backend_register(&telnet_backend);

    return RT_EOK;
}
INIT_ENV_EXPORT(agile_telnet_init);

#ifdef RT_USING_FINSH
#ifdef FINSH_USING_MSH
static int telnet_client_timeout(int argc, char **argv)
{
    if (argc == 1) {
        rt_kprintf("telnet client timeout:%d min\r\n", telnet.client_timeout);
    } else if (argc == 2) {
        int timeout = atoi(argv[1]);
        if (timeout <= 0) {
            rt_kprintf("telnet client timeout must be greater than 0.");
        } else {
            telnet.client_timeout = timeout;
            rt_kprintf("set telnet client timeout success.\r\n");
        }
    } else {
        rt_kprintf("Usage:\r\n");
        rt_kprintf("telnet_client_timeout           - get telnet client timeout\r\n");
        rt_kprintf("telnet_client_timeout timeout   - set telnet client timeout\r\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT_ALIAS(telnet_client_timeout, telnet_ctm, telnet client timeout);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */

#endif /* PKG_USING_AGILE_TELNET */
