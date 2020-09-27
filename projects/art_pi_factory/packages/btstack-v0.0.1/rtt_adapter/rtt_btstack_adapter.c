/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-14     super_mcu    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <dfs_posix.h>


/* defined the LED2 pin: PB7 */

#define FS_PARTITION_NAME              "param"    

int bt_stack_init(void)
{

    return RT_EOK;
}
INIT_APP_EXPORT(bt_stack_init);


#define SAMPLE_UART_NAME       "uart3"

/* ?????????? */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* ???????? */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ????????????,???????,????????? */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* ????????????,????????????? */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* ?????????,???????????? */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* ?????????????? */
        //ch = ch + 1;
        //rt_device_write(serial, 0, &ch, 1);
        rt_kprintf("\r\n === recv:0x%x\r\n",ch);
    }
}

static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
  //  uint8_t  tx_str[] = {0x01, 0x01, 0x10, 0x00};
    uint8_t  tx_str[] = {0x01, 0x03, 0x0c, 0x00};
    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }
#define BT_AP6212_PIN    GET_PIN(E, 1)
     rt_pin_mode(BT_AP6212_PIN, PIN_MODE_OUTPUT);

     rt_pin_write(BT_AP6212_PIN, PIN_LOW);
     rt_thread_mdelay(1000);
     rt_pin_write(BT_AP6212_PIN, PIN_HIGH);
     rt_thread_mdelay(1000);
    /* ?????????? */
    if(serial == NULL)
    {
        serial = rt_device_find(uart_name);
        if (!serial)
        {
            rt_kprintf("find %s failed!\n", uart_name);
            return RT_ERROR;
        }

        /* ?????? */
        rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
        /* ?????????????????? */
        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

        /* ???????? */
        rt_device_set_rx_indicate(serial, uart_input);
        /* ????? */
    }
    rt_device_write(serial, 0, tx_str, 4);

    /* ?? serial ?? */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* ????????? */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}
MSH_CMD_EXPORT(uart_sample, uart device sample);
