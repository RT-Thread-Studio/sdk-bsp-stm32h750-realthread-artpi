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
*************************************************/

#include <rthw.h>

#ifdef PKG_USING_AGILE_TELNET

#ifndef PKG_USING_AGILE_CONSOLE
#error "Please enable agile_console package"
#endif

#include <agile_telnet.h>
#include <agile_console.h>

#include <dfs_posix.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>


/* 线程堆栈大小 */
#ifndef PKG_AGILE_TELNET_THREAD_STACK_SIZE
#define PKG_AGILE_TELNET_THREAD_STACK_SIZE          2048
#endif

/* 线程优先级 */
#ifndef PKG_AGILE_TELNET_THREAD_PRIORITY
#define PKG_AGILE_TELNET_THREAD_PRIORITY            RT_THREAD_PRIORITY_MAX - 6
#endif

/* 监听端口 */
#ifndef PKG_AGILE_TELNET_PORT
#define PKG_AGILE_TELNET_PORT                       23
#endif

/* 接收缓冲区大小 */
#ifndef PKG_AGILE_TELNET_RX_BUFFER_SIZE
#define PKG_AGILE_TELNET_RX_BUFFER_SIZE             256
#endif

/* 发送缓冲区大小 */
#ifndef PKG_AGILE_TELNET_TX_BUFFER_SIZE
#define PKG_AGILE_TELNET_TX_BUFFER_SIZE             2048
#endif

/* 客户端空闲超时时间(单位：min) */
#ifndef PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT
#define PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT     3
#endif

static struct agile_telnet telnet = {0};
static struct agile_console_backend telnet_backend = {0};

static int process_tx(void)
{
    rt_size_t length = 0;
    rt_uint8_t tx_buffer[100];
    rt_size_t tx_len = 0;

    while(1)
    {
        rt_base_t level = rt_hw_interrupt_disable();
        length = rt_ringbuffer_get(telnet.tx_rb, tx_buffer, sizeof(tx_buffer));
        rt_hw_interrupt_enable(level);

        if(length > 0)
        {
            tx_len += length;
            send(telnet.client_fd, tx_buffer, length, 0);
        }
        else
        {
            break;
        }
    }

    return tx_len;
}

static void process_rx(rt_uint8_t *data, rt_size_t length)
{
    rt_base_t level = rt_hw_interrupt_disable();
    while(length)
    {
        if(*data != '\r') /* ignore '\r' */
        {
            rt_ringbuffer_putchar(telnet.rx_rb, *data);
        }

        data++;
        length--;
    }
    rt_hw_interrupt_enable(level);
}

/* telnet server thread entry */
static void telnet_thread(void* parameter)
{
#define RECV_BUF_LEN 64

    struct sockaddr_in addr;
    int enable = 1;
    socklen_t addr_size;
    rt_uint8_t recv_buf[RECV_BUF_LEN];
    int max_fd = -1;
    fd_set readset, writeset, exceptset;
    rt_tick_t client_tick_timeout = rt_tick_get();
    int rc;
    // select超时时间
    struct timeval select_timeout;
    select_timeout.tv_sec = 10;
    select_timeout.tv_usec = 0;

    telnet.server_fd = -1;
    telnet.client_fd = -1;
    telnet.isconnected = 0;
    telnet.client_timeout = PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT;

    rt_thread_mdelay(5000);
_telnet_start:
    if ((telnet.server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        goto _telnet_restart;

    if(setsockopt(telnet.server_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&enable, sizeof(enable)) < 0)
        goto _telnet_restart;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PKG_AGILE_TELNET_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    rt_memset(&(addr.sin_zero), 0, sizeof(addr.sin_zero));
    if (bind(telnet.server_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) < 0)
        goto _telnet_restart;

    if (listen(telnet.server_fd, 1) < 0)
        goto _telnet_restart;

    while (1)
    {
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exceptset);

        FD_SET(telnet.server_fd, &readset);
        FD_SET(telnet.server_fd, &exceptset);

        max_fd = telnet.server_fd;
        if(telnet.client_fd >= 0)
        {
            FD_SET(telnet.client_fd, &readset);
            FD_SET(telnet.client_fd, &writeset);
            FD_SET(telnet.client_fd, &exceptset);
            if(max_fd < telnet.client_fd)
                max_fd = telnet.client_fd;
        }

        rc = select(max_fd + 1, &readset, &writeset, &exceptset, &select_timeout);
        if(rc < 0)
            goto _telnet_restart;
        else if(rc > 0)
        {
            //服务器事件
            if(FD_ISSET(telnet.server_fd, &exceptset))
                goto _telnet_restart;

            if(FD_ISSET(telnet.server_fd, &readset))
            {
                int client_sock_fd = accept(telnet.server_fd, (struct sockaddr *)&addr, &addr_size);
                if(client_sock_fd < 0)
                    goto _telnet_restart;
                else
                {
                    if(telnet.client_fd >= 0)
                    {
                        telnet.isconnected = 0;
                        close(telnet.client_fd);
                        telnet.client_fd = -1;
                    }

                    struct timeval tv;
                    tv.tv_sec = 20;
                    tv.tv_usec = 0;
                    setsockopt(client_sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
                    telnet.client_fd = client_sock_fd;
                    telnet.client_timeout = PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT;
                    client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);

                    rt_base_t level = rt_hw_interrupt_disable();
                    rt_ringbuffer_reset(telnet.tx_rb);
                    rt_hw_interrupt_enable(level);

                    telnet.isconnected = 1;

                    const char *format = "Login Successful.\r\n";
                    send(telnet.client_fd, format, strlen(format), 0);
                }
            }

            // 客户端事件
            if(telnet.client_fd >= 0)
            {
                if(FD_ISSET(telnet.client_fd, &exceptset))
                {
                    telnet.isconnected = 0;
                    close(telnet.client_fd);
                    telnet.client_fd = -1;
                }
                else if(FD_ISSET(telnet.client_fd, &readset))
                {
                    int recv_len = recv(telnet.client_fd, recv_buf, RECV_BUF_LEN, MSG_DONTWAIT);
                    if(recv_len <= 0)
                    {
                        telnet.isconnected = 0;
                        close(telnet.client_fd);
                        telnet.client_fd = -1;
                    }
                    else
                    {
                        process_rx(recv_buf, recv_len);

                        client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);
                    }
                }
                else if(FD_ISSET(telnet.client_fd, &writeset))
                {
                    int send_len = process_tx();
                    if(send_len > 0)
                    {
                        client_tick_timeout = rt_tick_get() + rt_tick_from_millisecond(telnet.client_timeout * 60000);
                    }
                    else  
                        rt_thread_mdelay(50);
                }
            }
        }

        if(telnet.client_fd >= 0)
        {
            if((rt_tick_get() - client_tick_timeout) < (RT_TICK_MAX / 2))
            {
                telnet.isconnected = 0;
                close(telnet.client_fd);
                telnet.client_fd = -1;
            }
        }
    }

_telnet_restart:
    telnet.isconnected = 0;
    if(telnet.server_fd >= 0)
    {
        close(telnet.server_fd);
        telnet.server_fd = -1;
    }
    if(telnet.client_fd >= 0)
    {
        close(telnet.client_fd);
        telnet.client_fd = -1;
    }

    rt_thread_mdelay(10000);
    goto _telnet_start;

}



static void telnet_backend_output(const uint8_t *buf, int len)
{
    if(telnet.isconnected == 0)
        return;

    rt_base_t level = rt_hw_interrupt_disable();

    while(len > 0)
    {
        if(*buf == '\n')
            rt_ringbuffer_putchar(telnet.tx_rb, '\r');

        rt_ringbuffer_putchar(telnet.tx_rb, *buf);

        ++buf;
        --len;
    }

    rt_hw_interrupt_enable(level);
}

static int telnet_backend_read(uint8_t *buf, int len)
{
    if(telnet.isconnected == 0)
        return 0;

    rt_size_t result = 0;

    rt_base_t level = rt_hw_interrupt_disable();
    result = rt_ringbuffer_get(telnet.rx_rb, buf, len);
    rt_hw_interrupt_enable(level);

    return result;
}

static int agile_telnet_init(void)
{
    rt_memset(&telnet, 0, sizeof(struct agile_telnet));

    telnet.isconnected = 0;
    telnet.server_fd = -1;
    telnet.client_fd = -1;
    telnet.client_timeout = PKG_AGILE_TELNET_CLIENT_DEFAULT_TIMEOUT;

    telnet.rx_rb = rt_ringbuffer_create(PKG_AGILE_TELNET_RX_BUFFER_SIZE);
    RT_ASSERT(telnet.rx_rb != RT_NULL);

    telnet.tx_rb = rt_ringbuffer_create(PKG_AGILE_TELNET_TX_BUFFER_SIZE);
    RT_ASSERT(telnet.tx_rb != RT_NULL);

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
    if(argc == 1)
    {
        rt_kprintf("telnet client timeout:%d min\r\n", telnet.client_timeout);
    }
    else if(argc == 2)
    {
        int timeout = atoi(argv[1]);
        if(timeout <= 0)
        {
            rt_kprintf("telnet client timeout must be greater than 0.");
        }
        else
        {
            telnet.client_timeout = timeout;
            rt_kprintf("set telnet client timeout success.\r\n");
        }
    }
    else
    {
        rt_kprintf("Usage:\r\n");
        rt_kprintf("telnet_client_timeout           - get telnet client timeout\r\n");
        rt_kprintf("telnet_client_timeout timeout   - set telnet client timeout\r\n");
    }
  
    return RT_EOK;
}
MSH_CMD_EXPORT_ALIAS(telnet_client_timeout, telnet_ctm, telnet client teimeout);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */

#endif /* PKG_USING_AGILE_TELNET */
