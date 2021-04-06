
/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-19     liuduanfei   the first version
 */
 
#include <rtthread.h>
#include <rtdevice.h>
#include "ft6236.h"
#include "touch.h"
#include "drv_common.h"

#define DBG_TAG "ft6236_example"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


rt_thread_t ft6236_thread;
rt_device_t touch;

void ft6236_thread_entry(void *parameter)
{
    struct rt_touch_data *read_data;

    read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));

    while(1)
    {
        rt_device_read(touch, 0, read_data, 1);

        if (read_data->event == RT_TOUCH_EVENT_DOWN)
        {
            rt_kprintf("down x: %03d y: %03d", read_data->x_coordinate, read_data->y_coordinate);
            rt_kprintf(" t: %d\n", read_data->timestamp);
        }
        if (read_data->event == RT_TOUCH_EVENT_MOVE)
        {
            rt_kprintf("move x: %03d y: %03d", read_data->x_coordinate, read_data->y_coordinate);
            rt_kprintf(" t: %d\n", read_data->timestamp);
        }
        if (read_data->event == RT_TOUCH_EVENT_UP)
        {
            rt_kprintf("up   x: %03d y: %03d", read_data->x_coordinate, read_data->y_coordinate);
            rt_kprintf(" t: %d\n\n", read_data->timestamp);
        }
        rt_thread_delay(10);
    }

}

#define REST_PIN GET_PIN(D, 3)

int ft6236_example(void)
{
    struct rt_touch_config cfg;

    cfg.dev_name = "i2c1";
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

    ft6236_thread = rt_thread_create("touch", ft6236_thread_entry, RT_NULL, 800, 10, 20);
    if (ft6236_thread == RT_NULL)
    {
        LOG_D("create ft6236 thread err");

        return -RT_ENOMEM;
    }
    rt_thread_startup(ft6236_thread);

    return RT_EOK;
}
INIT_APP_EXPORT(ft6236_example);
