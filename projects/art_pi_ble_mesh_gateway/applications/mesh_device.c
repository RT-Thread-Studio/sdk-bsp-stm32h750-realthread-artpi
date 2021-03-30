#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"
#include "mesh_command.h"
#include "mesh_node.h"

#define LOG_TAG "mesh_device" // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL LOG_LVL_DBG   // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>             // 必须在 LOG_TAG 与 LOG_LVL 下面

#define THREAD_STACK_SIZE 1000 //线程栈大小（字节）
#define THREAD_TIMESLICE 40    //占用的滴答时钟数
#define UART_NAME "uart5"      /* 串口设备名称 */

/* 消息队列控制块 */
struct rt_messagequeue mesh_mq;
static rt_thread_t mesh_command_tx_thread = RT_NULL;
static rt_thread_t mesh_command_rx_thread = RT_NULL;
static rt_uint8_t thread_priority = 20;
static rt_device_t serial;                                        /* 串口设备句柄 */
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */
static struct rt_semaphore rx_sem;

/* 消息队列中用到的放置消息的内存池 */
static rt_uint8_t msg_pool[2048];

void mesh_send_command(command_opcode_t opc, uint16_t addr, uint8_t *data)
{
    uint8_t command[11];

    if (opc != COMMAND_OPCODE_FIND && opc != COMMAND_OPCODE_GET && opc != COMMAND_OPCODE_SET)
        return;

    command[0] = opc;
    memcpy(&command[1], &addr, 2);

    if (data != NULL)
        memcpy(&command[3], data, COMMAND_DATA_LENGTH);

    rt_mq_send(&mesh_mq, command, sizeof(command));
}

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void mesh_command_tx_thread_entry(void *params)
{
    serial = rt_device_find(UART_NAME);
    if (serial != RT_NULL)
    {
        /* step2：修改串口配置参数 */
        config.baud_rate = BAUD_RATE_115200; //修改波特率为 115200
        config.data_bits = DATA_BITS_8;      //数据位 8
        config.stop_bits = STOP_BITS_1;      //停止位 1
        config.bufsz = 128;                  //修改缓冲区 buff size 为 128
        config.parity = PARITY_NONE;         //无奇偶校验位

        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

        rt_device_set_rx_indicate(serial, uart_input);

        while (1)
        {

            char command[11];

            /* 从消息队列中接收消息 */
            if (rt_mq_recv(&mesh_mq, command, sizeof(command), RT_WAITING_FOREVER) == RT_EOK)
            {
                rt_size_t res = rt_device_write(serial, 0, command, sizeof(command));

                if (res < 0)
                {
                    // TODO: print error
                }
            }
        }
    }
}

static void command_handler(uint8_t *command)
{
    uint8_t opcode = command[0];

    switch (opcode)
    {
    case COMMAND_OPCODE_REGISTER:
    {

        mesh_node_t node;
        memcpy(&node.addr, &command[1], 2);
        node.type = command[3];
        (void)mesh_node_add(node); // 不关心结果
        break;
    }
    case COMMAND_OPCODE_DATA:
    case COMMAND_OPCODE_STATUS:
    {
        mesh_node_t node;
        memcpy(&node.addr, &command[1], 2);
        memcpy(&node.data, &command[3], COMMAND_DATA_LENGTH);

        uint16_t index = mesh_node_find(node.addr);
        if (index != -1)
        {
            mesh_node_update_data(index, node.data);
        }
    }
    }
}

static void mesh_command_rx_thread_entry(void *parameter)
{
    rt_err_t result;
    rt_uint32_t rx_length = 0;
    char rx_buffer[11];

    while (1)
    {

        uint8_t ch;
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }

        rx_buffer[rx_length++] = ch;

        if (rx_length == sizeof(rx_buffer))
        {

            LOG_HEX("All data", 11, rx_buffer, sizeof(rx_buffer));
            rx_length = 0;
            command_handler(rx_buffer);
        }
    }
}

//创建串口线程
int create_mesh_command_thread(void)
{

    rt_err_t result;
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    /* 初始化消息队列 */
    result = rt_mq_init(&mesh_mq,
                        "mesh_mq",
                        &msg_pool[0],      /* 内存池指向 msg_pool */
                        11,                /* 每个消息的大小是 11 字节 */
                        sizeof(msg_pool),  /* 内存池的大小是 msg_pool 的大小 */
                        RT_IPC_FLAG_FIFO); /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    mesh_command_tx_thread = rt_thread_create("mesh_command_tx_thread", mesh_command_tx_thread_entry,
                                              RT_NULL, THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (mesh_command_tx_thread != RT_NULL)
    {
        rt_thread_startup(mesh_command_tx_thread);
    }

    mesh_command_rx_thread = rt_thread_create("mesh_command_rx_thread", mesh_command_rx_thread_entry,
                                              RT_NULL, THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (mesh_command_rx_thread != RT_NULL)
    {
        rt_thread_startup(mesh_command_rx_thread);
    }
    return 0;
}

INIT_APP_EXPORT(create_mesh_command_thread);