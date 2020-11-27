/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-10-23     liuduanfei     first version
 */

#include <rtthread.h>
#ifdef MEDIA_IO_USING_TOUCH

#include "touch.h"
#include <js_persim.h>

#define DBG_TAG "touch"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

rt_thread_t touch_thread;
rt_device_t touch;

void touch_thread_entry(void *parameter)
{
    struct rt_touch_data *read_data;

    read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));

    while(1)
    {
        rt_device_read(touch, 0, read_data, 1);

        if ((read_data->event == RT_TOUCH_EVENT_DOWN) || (read_data->event == RT_TOUCH_EVENT_MOVE))
        {
            LOG_D("down");
#ifdef PKG_USING_PERSIMMON_SRC
            rtgui_server_post_touch(read_data->x_coordinate, read_data->y_coordinate, 1);
#endif /* PKG_USING_PERSIMMON_SRC */
        }

        if (read_data->event == RT_TOUCH_EVENT_UP)
        {
            LOG_D("up");
#ifdef PKG_USING_PERSIMMON_SRC
            rtgui_server_post_touch(read_data->x_coordinate, read_data->y_coordinate, 0);
#endif /* PKG_USING_PERSIMMON_SRC */
        }
        rt_thread_delay(10);
    }
}

int touch_init(void)
{
    touch = rt_device_find("touch");

    rt_device_open(touch, RT_DEVICE_FLAG_RDONLY);

    struct rt_touch_info info;
    rt_device_control(touch, RT_TOUCH_CTRL_GET_INFO, &info);
    LOG_I("type       :%d", info.type);
    LOG_I("vendor     :%d", info.vendor);
    LOG_I("point_num  :%d", info.point_num);
    LOG_I("range_x    :%d", info.range_x);
    LOG_I("range_y    :%d", info.range_y);

    touch_thread = rt_thread_create("touch", touch_thread_entry, RT_NULL, 800, 10, 20);
    if (touch_thread == RT_NULL)
    {
        LOG_D("create touch thread err");

        return -RT_ENOMEM;
    }
    rt_thread_startup(touch_thread);

    return RT_EOK;
}
//INIT_APP_EXPORT(touch_init);

#endif /* MEDIA_IO_USING_TOUCH */

#ifdef PKG_USING_FT6236

#include "ft6236.h"
#include <drv_common.h>

#define REST_PIN GET_PIN(D, 3)

int ft6236_init(void)
{
    struct rt_touch_config config;
    config.dev_name = "i2c2";

    rt_hw_ft6236_init("touch", &config, REST_PIN);

    return 0;
}
INIT_ENV_EXPORT(ft6236_init);

#endif /* PKG_USING_FT6236 */
