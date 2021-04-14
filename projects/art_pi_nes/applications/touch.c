/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author                 Notes
 * 2020-12-03     liuduanfei/Ghazigq    the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "ft6236.h"
#include "touch.h"
#include "drv_common.h"
#include <lcd_spi_port.h>
#include "stdbool.h"

#ifdef MEDIA_IO_USING_TOUCH

#define DBG_TAG     "ft6236"
#define DBG_LVL     DBG_WARNING
#include <rtdbg.h>

static rt_thread_t ft6236_thread = NULL;
static rt_device_t touch;
static rt_bool_t touch_nes_mode = false;

extern void littlevgl2rtt_send_input_event(rt_int16_t x, rt_int16_t y, rt_uint8_t state);
extern void nes_control_send_input_event(uint8_t num, int16_t x, int16_t y, uint8_t state);

void ft6236_thread_entry(void *parameter)
{
    struct rt_touch_data *read_data;
    rt_bool_t last_touch_nes_mode = false;

    read_data = (struct rt_touch_data *)rt_calloc(2, sizeof(struct rt_touch_data));

    while(1)
    {
        if(touch_nes_mode && !last_touch_nes_mode)
#ifdef LCD_HOR_SCREEN
            littlevgl2rtt_send_input_event(read_data[0].y_coordinate, read_data[0].x_coordinate?(LCD_HEIGHT - read_data[0].x_coordinate):0, RT_TOUCH_EVENT_NONE);
#else
            littlevgl2rtt_send_input_event(read_data[0].x_coordinate, read_data[0].y_coordinate, RT_TOUCH_EVENT_NONE);
#endif
        last_touch_nes_mode = touch_nes_mode;
        rt_device_read(touch, 0, read_data, 2);
        if(touch_nes_mode)
        {
#ifdef LCD_HOR_SCREEN
            nes_control_send_input_event(0, read_data[0].y_coordinate, read_data[0].x_coordinate?(LCD_HEIGHT - read_data[0].x_coordinate):0, read_data[0].event);
            nes_control_send_input_event(1, read_data[1].y_coordinate, read_data[1].x_coordinate?(LCD_HEIGHT - read_data[1].x_coordinate):0, read_data[1].event);
#else
            nes_control_send_input_event(0, read_data[0].x_coordinate, read_data[0].y_coordinate, read_data[0].event);
            nes_control_send_input_event(1, read_data[1].x_coordinate, read_data[1].y_coordinate, read_data[1].event);
#endif
        }
        else
#ifdef LCD_HOR_SCREEN
            littlevgl2rtt_send_input_event(read_data[0].y_coordinate, read_data[0].x_coordinate?(LCD_HEIGHT - read_data[0].x_coordinate):0, read_data[0].event);
#else
            littlevgl2rtt_send_input_event(read_data[0].x_coordinate, read_data[0].y_coordinate, read_data[0].event);
#endif
        rt_thread_mdelay(10);
    }

}

void touch_set_nes_mode(rt_bool_t value)
{
    touch_nes_mode = value;
}

//#define REST_PIN GET_PIN(D, 3)
#define REST_PIN GET_PIN(A, 15)  //实际上是触摸的中断引脚
int ft6236_init(void)
{
    struct rt_touch_config cfg;

    cfg.dev_name = "i2c2";
    rt_hw_ft6236_init("touch", &cfg, REST_PIN);

    touch = rt_device_find("touch");

    rt_device_open(touch, RT_DEVICE_FLAG_RDONLY);

    struct rt_touch_info info;
    rt_device_control(touch, RT_TOUCH_CTRL_GET_INFO, &info);
    LOG_I("type       :%d", info.type);
    LOG_I("vendor     :%d", info.vendor);
    LOG_I("point_num  :%d", info.point_num);
    LOG_I("range_x    :%d", info.range_x);
    LOG_I("range_y    :%d", info.range_y);

    ft6236_thread = rt_thread_create("touch", ft6236_thread_entry, RT_NULL, 2048, 3, 20);
    if (ft6236_thread == RT_NULL)
    {
        LOG_D("create ft6236 thread err");

        return -RT_ENOMEM;
    }
    rt_thread_startup(ft6236_thread);

    return RT_EOK;
}
INIT_APP_EXPORT(ft6236_init);

#endif /* MEDIA_IO_USING_TOUCH */

