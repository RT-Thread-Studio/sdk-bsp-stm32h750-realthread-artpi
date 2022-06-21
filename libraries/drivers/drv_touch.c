/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-06-20     Rbb666       Add GT9147 Touch Device
 */

#include "drv_touch.h"
#include <string.h>

#ifdef BSP_USING_TOUCH
#include "gt9147.h"
#include <lvgl.h>
#include <lv_port_indev.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME  "TOUCH"
#define DBG_LEVEL         DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#define THREAD_PRIORITY     25
#define THREAD_STACK_SIZE   1024
#define THREAD_TIMESLICE    10

static rt_sem_t gt9147_sem = RT_NULL;
static rt_device_t dev = RT_NULL;
static struct rt_touch_data *read_data;
static struct rt_touch_info info;

#ifdef BSP_USING_LVGL
static rt_bool_t touch_down = RT_FALSE;
static void post_down_event(rt_uint16_t x, rt_uint16_t y)
{
    touch_down = RT_TRUE;
    lv_port_indev_input(x, y, (touch_down == RT_TRUE) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
}

static void post_motion_event(rt_uint16_t x, rt_uint16_t y)
{
    lv_port_indev_input(x, y, (touch_down == RT_TRUE) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
}

static void post_up_event(rt_uint16_t x, rt_uint16_t y)
{
    touch_down = RT_FALSE;
    lv_port_indev_input(x, y, (touch_down == RT_TRUE) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
}
#endif

static void touch_thread_entry(void *parameter)
{
    rt_uint16_t point_x, point_y;

    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, &info);

    read_data = (struct rt_touch_data *) rt_malloc(sizeof(struct rt_touch_data) * info.point_num);

    while (1)
    {
        rt_sem_take(gt9147_sem, RT_WAITING_FOREVER);

        if (rt_device_read(dev, 0, read_data, info.point_num) == info.point_num)
        {
            point_x = 800 - read_data[0].x_coordinate;
            point_y = 480 - read_data[0].y_coordinate;
#ifdef BSP_USING_LVGL
            switch (read_data[0].event)
            {
            case TOUCH_EVENT_UP:
                post_up_event(point_x, point_y);
                break;
            case TOUCH_EVENT_DOWN:
                post_down_event(point_x, point_y);
                break;
            case TOUCH_EVENT_MOVE:
                post_motion_event(point_x, point_y);
                break;
            default:
                break;
            }
#else
            if (read_data[0].event == RT_TOUCH_EVENT_DOWN || read_data[0].event == RT_TOUCH_EVENT_MOVE)
            {
                rt_kprintf("%d %d %d %d %d\n", read_data[0].track_id, point_x,
                        point_y, read_data[0].timestamp, read_data[0].width);
            }
#endif
        }
        rt_device_control(dev, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);
    }
}

static rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(gt9147_sem);
    rt_device_control(dev, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);
    return 0;
}

static int touch_bg_init(const char *name, rt_uint16_t x, rt_uint16_t y)
{
    void *id;
    rt_thread_t tid = RT_NULL;

    dev = rt_device_find(name);
    if (dev == RT_NULL)
    {
        rt_kprintf("can't find device:%s\n", name);
        return -1;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    gt9147_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);
    if (gt9147_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t * read_id = (rt_uint8_t *) id;
    rt_kprintf("id = %d %d %d %d \n", read_id[0] - '0', read_id[1] - '0', read_id[2] - '0', read_id[3] - '0');

    rt_device_control(dev, RT_TOUCH_CTRL_SET_X_RANGE, &x); /* if possible you can set your x y coordinate*/
    rt_device_control(dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);
    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, id);
    rt_kprintf("range_x = %d \n", (*(struct rt_touch_info*) id).range_x);
    rt_kprintf("range_y = %d \n", (*(struct rt_touch_info*) id).range_y);
    rt_kprintf("point_num = %d \n", (*(struct rt_touch_info*) id).point_num);
    rt_free(id);
    rt_device_set_rx_indicate(dev, rx_callback);

    tid = rt_thread_create("touch", touch_thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

int touch_init(void)
{
    touch_bg_init("gt", LV_HOR_RES_MAX, LV_VER_RES_MAX);
    return RT_EOK;
}
INIT_APP_EXPORT(touch_init);
#endif
