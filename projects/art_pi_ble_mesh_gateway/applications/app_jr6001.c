#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"

#define THREAD_STACK_SIZE 1024
#define THREAD_TIMESLICE 40
#define SAMPLE_UART_NAME "uart1"

#define PLAY_BUSY_PIN GET_PIN(H, 14)
#define LED_G GET_PIN(H, 15)

struct rt_messagequeue player_mq;
rt_mutex_t player_use_mtx = RT_NULL;

static rt_thread_t jr_thread = RT_NULL;
static rt_uint8_t thread_priority = 20;
static rt_device_t serial;
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

static rt_uint8_t msg_pool[2048];

static void jr_send_command(const char *str)
{
    rt_kprintf("command: %s, len: %d\n", str, strlen(str));
    rt_size_t res = rt_device_write(serial, 0, str, strlen(str));
    if (res < 0)
    {
        // TODO: print error
    }
}

void jr_command(int argc, char **argv)
{
    jr_send_command(argv[1]);
}
MSH_CMD_EXPORT(jr_command, jr send command);

void jr_thread_entry(void *params)
{
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (serial != RT_NULL)
    {
        config.baud_rate = BAUD_RATE_9600;
        config.data_bits = DATA_BITS_8;
        config.stop_bits = STOP_BITS_1;
        config.bufsz = 128;
        config.parity = PARITY_NONE;

        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

        jr_send_command("AF:20");
        char command[20];

        while (1)
        {
            // 等待播放结束
            while (rt_pin_read(PLAY_BUSY_PIN))
            {
                rt_thread_mdelay(10);
            }

            // delay100ms之后释放互斥锁
            rt_thread_mdelay(100);
            rt_mutex_release(player_use_mtx);
            rt_pin_write(LED_G, 1);

            char buf = 0;

            if (rt_mq_recv(&player_mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
            {
                rt_mutex_take(player_use_mtx, RT_WAITING_FOREVER);
                rt_pin_write(LED_G, 0);

                memset(command, 0, 20); // 设置音量
                sprintf(command, "A8:02/0000%c*MP3", buf);
                jr_send_command(command);

                // 等待开始播放
                while (!rt_pin_read(PLAY_BUSY_PIN))
                {
                    rt_thread_mdelay(10);
                }
            }
        }
    }
}

int create_jr6001_thread(void)
{

    rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
    rt_pin_write(LED_G, 1);

    rt_pin_mode(PLAY_BUSY_PIN, PIN_MODE_INPUT);

    rt_err_t result;

    result = rt_mq_init(&player_mq,
                        "player_mq",
                        &msg_pool[0],
                        1,
                        sizeof(msg_pool),
                        RT_IPC_FLAG_FIFO);

    player_use_mtx = rt_mutex_create("player_use_mtx", RT_IPC_FLAG_FIFO);

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    jr_thread = rt_thread_create("jr6001", jr_thread_entry,
                                 RT_NULL, THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (jr_thread != RT_NULL)
    {
        rt_thread_startup(jr_thread);
    }
    return 0;
}

INIT_APP_EXPORT(create_jr6001_thread);
