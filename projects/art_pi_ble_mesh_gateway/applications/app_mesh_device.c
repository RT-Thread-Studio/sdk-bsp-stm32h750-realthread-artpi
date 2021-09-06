#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"
#include "mesh_command.h"
#include "mesh_node.h"

#define LOG_TAG "mesh_device"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

#define RX_THREAD_STACK_SIZE 512
#define TX_THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 40
#define UART_NAME "uart5"

static struct rt_messagequeue mesh_mq;
static rt_thread_t mesh_command_tx_thread = RT_NULL;
static rt_thread_t mesh_command_rx_thread = RT_NULL;
static rt_uint8_t thread_priority = 20;
static rt_device_t serial;
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
static struct rt_semaphore rx_sem;

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

    LOG_HEX("Uart send data", 11, command, sizeof(command));
}

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void mesh_command_tx_thread_entry(void *params)
{
    serial = rt_device_find(UART_NAME);
    if (serial != RT_NULL)
    {
        config.baud_rate = BAUD_RATE_115200;
        config.data_bits = DATA_BITS_8;
        config.stop_bits = STOP_BITS_1;
        config.bufsz = 128;
        config.parity = PARITY_NONE;

        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

        rt_device_set_rx_indicate(serial, uart_input);

        while (1)
        {

            char command[11];

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

        int16_t index = mesh_node_find(node.addr);
        if (index != -1)
        {
            mesh_node_update_data(index, node.data);
        }
    }
    }
}

static void mesh_command_rx_thread_entry(void *parameter)
{
    rt_uint32_t rx_length = 0;
    char rx_buffer[11];

    while (1)
    {

        uint8_t ch;
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }

        rx_buffer[rx_length++] = ch;

        if (rx_length == sizeof(rx_buffer))
        {

            LOG_HEX("Uart receive data", 11, rx_buffer, sizeof(rx_buffer));
            rx_length = 0;
            command_handler(rx_buffer);
        }
    }
}

int create_mesh_command_thread(void)
{

    rt_err_t result;
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    result = rt_mq_init(&mesh_mq,
                        "mesh_mq",
                        &msg_pool[0],
                        11,
                        sizeof(msg_pool),
                        RT_IPC_FLAG_FIFO);

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    mesh_command_tx_thread = rt_thread_create("mesh_tx", mesh_command_tx_thread_entry,
                                              RT_NULL, TX_THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (mesh_command_tx_thread != RT_NULL)
    {
        rt_thread_startup(mesh_command_tx_thread);
    }

    mesh_command_rx_thread = rt_thread_create("mesh_rx", mesh_command_rx_thread_entry,
                                              RT_NULL, RX_THREAD_STACK_SIZE, thread_priority, THREAD_TIMESLICE);
    if (mesh_command_rx_thread != RT_NULL)
    {
        rt_thread_startup(mesh_command_rx_thread);
    }
    return 0;
}

INIT_APP_EXPORT(create_mesh_command_thread);
