#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"

#define THREAD_STACK_SIZE 1000   //线程栈大小（字节）
#define THREAD_TIMESLICE 40      //占用的滴答时钟数
#define SAMPLE_UART_NAME "uart1" /* 串口设备名称 */

#define PLAY_BUSY_PIN GET_PIN(H, 14)
#define LED_G GET_PIN(H, 15)

/* 消息队列控制块 */
struct rt_messagequeue player_mq;
rt_mutex_t player_use_mtx = RT_NULL;

static rt_thread_t jr_thread = RT_NULL;
static rt_uint8_t thread_priority = 20;
static rt_device_t serial;                                        /* 串口设备句柄 */
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */

/* 消息队列中用到的放置消息的内存池 */
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

// MSH命令
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

        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

        jr_send_command("AF:20");
        char command[20];

        while (1)
        {
            // 等待播放结束
            while (rt_pin_read(PLAY_BUSY_PIN))
            {
                // rt_kprintf("jr60001: %d\n", rt_pin_read(PLAY_BUSY_PIN));
                rt_thread_mdelay(10);
            }

            // delay100ms之后释放互斥锁
            rt_thread_mdelay(100);
            rt_mutex_release(player_use_mtx);
            rt_pin_write(LED_G, 1);

            char buf = 0;

            /* 从消息队列中接收消息 */
            if (rt_mq_recv(&player_mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
            {

                rt_mutex_take(player_use_mtx, RT_WAITING_FOREVER);
                rt_pin_write(LED_G, 0);

                memset(command, 0, 20);
                // sprintf(command, "A7:0000%c", buf);
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

//创建串口线程
int create_jr6001_thread(void)
{

    rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
    rt_pin_write(LED_G, 1);

    rt_pin_mode(PLAY_BUSY_PIN, PIN_MODE_INPUT);
    // rt_pin_attach_irq(PLAY_BUSY_PIN, PIN_IRQ_MODE_FALLING, jr6001_free, RT_NULL);
    // rt_pin_irq_enable(PLAY_BUSY_PIN, PIN_IRQ_ENABLE);

    rt_err_t result;

    /* 初始化消息队列 */
    result = rt_mq_init(&player_mq,
                        "player_mq",
                        &msg_pool[0],      /* 内存池指向 msg_pool */
                        1,                 /* 每个消息的大小是 1 字节 */
                        sizeof(msg_pool),  /* 内存池的大小是 msg_pool 的大小 */
                        RT_IPC_FLAG_FIFO); /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    // 初始化信号量
    player_use_mtx = rt_mutex_create("player_use_mtx", RT_IPC_FLAG_FIFO);

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    jr_thread = rt_thread_create("serial_test", jr_thread_entry,
                                 RT_NULL, THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (jr_thread != RT_NULL)
    {
        rt_thread_startup(jr_thread);
    }
    return 0;
}

INIT_APP_EXPORT(create_jr6001_thread);
