/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date            Author           Notes
 * 2020-12-10      liuda123.club
 */

#include <connect_mqtt.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

#define LED_PIN GET_PIN(I, 8)

#define SAMPLE_UART_NAME       "uart6"      //开启的串口名称

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

uint16_t temp = 0;  //定义温度变量
uint16_t humi = 0;  //定义湿度变量

char tem[10]={0};   //存放温度数据的数组
char hum[10]={0};   //存放湿度数据的数组

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);
    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch=0;
    char sensor[20]={0};
    uint8_t i = 0;
    char sensor_T_H[] = {0x01,0x04,0x00,0x00,0x00,0x02,0x71,0xCB};  //根据你的485传感器问询数据的指令修改
    while (1)
    {

        rt_device_write(serial, 0, sensor_T_H, sizeof(sensor_T_H));  //发送采集数据指令
        rt_thread_mdelay(1000);  //1s 采集一次
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);   //如果没有收到数据就阻塞等待
        }
        /* 读取到的数据通过串口错位输出 */

        sensor[i] = ch;
         i++;

        if(i==8)
        {
          i=0;     //清零
//          for(uint8_t a=0;a<8; a++)
//          {
//           rt_kprintf("sensor[%d] = %.2x \r\n",a,sensor[a]);
//          }

          //计算数据
          temp = (sensor[2]<<8)  + sensor[3];
          humi = (sensor[4]<<8)  + sensor[5];
          sprintf((char*)tem,"%.2f",(float)temp/100);  //拼接到温度数组里
          sprintf((char*)hum,"%.2f",(float)humi/100);  //拼接到湿度数组里
        }
    }
}

static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
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


int main(void)
{
    rt_uint32_t count = 1;

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    uart_sample(1,"uart6");             //调用串口
    mqtt_emqx();                        //开启mqtt线程

    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
    return RT_EOK;
}


#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


