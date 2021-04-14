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
#include "touch.h"

#define DBG_TAG "ft6236"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* FT6236 部分寄存器定义 */
#define FT_DEVIDE_MODE          0x00        /* FT6236模式控制寄存器 */
#define FT_REG_NUM_FINGER       0x02        /* 触摸状态寄存器 */

#define FT_TP1_REG              0X03        /* 第一个触摸点数据地址 */
#define FT_TP2_REG              0X09        /* 第二个触摸点数据地址 */
#define FT_TP3_REG              0X0F        /* 第三个触摸点数据地址 */
#define FT_TP4_REG              0X15        /* 第四个触摸点数据地址 */
#define FT_TP5_REG              0X1B        /* 第五个触摸点数据地址 */

#define FT_ID_G_LIB_VERSION     0xA1        /* 版本 */
#define FT_ID_G_MODE            0xA4        /* FT6236中断模式控制寄存器 */
#define FT_ID_G_THGROUP         0x80        /* 触摸有效值设置寄存器 */
#define FT_ID_G_PERIODACTIVE    0x88        /* 激活状态周期设置寄存器 */
#define Chip_Vendor_ID          0xA3        /* 芯片ID(0x36) */
#define ID_G_FT6236ID           0xA8        /* 0x11 */

/* FT6236 部分值定义 */
#define FT_EVT_TOUCH_MASK       0xC0
#define FT_EVT_TOUCH_DOWN       0x00
#define FT_EVT_TOUCH_UP         0x01
#define FT_EVT_TOUCH_CONTACT    0x02
#define FT_EVT_TOUCH_RESERVED   0x03

static void ft6236_write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t value)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;

    msg.addr = (0x38);
    msg.flags = RT_I2C_WR;
    msg.buf = buf;
    msg.len = 2;
   if (rt_i2c_transfer(bus, &msg, 1) != 1)
   {
       LOG_D("ft6236 write register error");
   }
}

static void ft6236_read_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *buf, rt_uint8_t len)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t temp_reg;

    temp_reg = reg;

    msg.addr = (0x38);
    msg.flags = RT_I2C_WR;
    msg.buf = &temp_reg;
    msg.len = 1;
    if (rt_i2c_transfer(bus, &msg, 1) != 1)
    {
        LOG_D("ft6236 write register error");
    }

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_RD;
    msg.buf = buf;
    msg.len = len;
    if (rt_i2c_transfer(bus, &msg, 1) != 1)
    {
        LOG_D("ft6236 read register error");
    }
}

static void ft6236_init(struct rt_i2c_bus_device *bus, rt_base_t pin)
{

    rt_pin_mode(pin, PIN_MODE_OUTPUT);

    rt_pin_write(pin, 0);
    rt_thread_mdelay(50);
    rt_pin_write(pin, 1);
    rt_thread_mdelay(100);

    ft6236_write_reg(bus, FT_DEVIDE_MODE, 0);
    ft6236_write_reg(bus, FT_ID_G_THGROUP, 12);
    ft6236_write_reg(bus, FT_ID_G_PERIODACTIVE, 12);
}

static rt_size_t ft6236_readpoint(struct rt_touch_device *touch, void *data, rt_size_t touch_num)
{
    static rt_uint8_t is_default_value = 1; //默认为0，0 down事件
    rt_uint8_t buf[4];
    rt_uint8_t event = FT_EVT_TOUCH_RESERVED;
    struct rt_i2c_bus_device * i2c_bus = RT_NULL;

    struct rt_touch_data *temp_data;

    temp_data = (struct rt_touch_data *)data;

    if(touch_num > 2)
        return -RT_ERROR;

    i2c_bus = rt_i2c_bus_device_find(touch->config.dev_name);
    if (i2c_bus == RT_NULL)
    {
        LOG_D("can not find i2c bus");
        return -RT_EIO;
    }

    for(rt_size_t i = 0;i < touch_num;i ++)
    {
        ft6236_read_reg(i2c_bus, FT_TP1_REG + i * 6, buf, 4);

        event = (buf[0] & FT_EVT_TOUCH_MASK) >> 6;

        if(event == FT_EVT_TOUCH_RESERVED || event == FT_EVT_TOUCH_UP)
        {
            if(temp_data->event == RT_TOUCH_EVENT_MOVE || temp_data->event == RT_TOUCH_EVENT_DOWN)
                temp_data->event = RT_TOUCH_EVENT_UP;
            else
                temp_data->event = RT_TOUCH_EVENT_NONE;
            is_default_value = 0;
        }
        else if(event == FT_EVT_TOUCH_CONTACT)
        {
            if(temp_data->event == RT_TOUCH_EVENT_NONE || temp_data->event == RT_TOUCH_EVENT_UP)
                temp_data->event = RT_TOUCH_EVENT_DOWN; //防止出现未检测到down事件
            else {
                temp_data->event = RT_TOUCH_EVENT_MOVE;
            }
            temp_data->x_coordinate = ((buf[0]&0X0F)<<8)+buf[1];
            temp_data->y_coordinate = ((buf[2]&0X0F)<<8)+buf[3];
            is_default_value = 0;
        }
        else {
            if(is_default_value)
                temp_data->event = RT_TOUCH_EVENT_NONE;
            else {
                temp_data->event = RT_TOUCH_EVENT_DOWN;
            }
            temp_data->x_coordinate = ((buf[0]&0X0F)<<8)+buf[1];
            temp_data->y_coordinate = ((buf[2]&0X0F)<<8)+buf[3];
        }

        temp_data->timestamp = rt_touch_get_ts();

        temp_data ++;
    }

    return RT_EOK;
}
static rt_err_t ft6236_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    struct rt_touch_info *info;

    switch(cmd)
    {
    case RT_TOUCH_CTRL_GET_ID:
        break;
    case RT_TOUCH_CTRL_GET_INFO:
        info = (struct rt_touch_info *)arg;
        info->point_num = touch->info.point_num;
        info->range_x = touch->info.range_x;
        info->range_y = touch->info.range_y;
        info->type = touch->info.type;
        info->vendor = touch->info.vendor;

        break;
    case RT_TOUCH_CTRL_SET_MODE:
        break;
    case RT_TOUCH_CTRL_SET_X_RANGE:
        break;
    case RT_TOUCH_CTRL_SET_Y_RANGE:
        break;
    case RT_TOUCH_CTRL_SET_X_TO_Y:
        break;
    case RT_TOUCH_CTRL_DISABLE_INT:
        break;
    case RT_TOUCH_CTRL_ENABLE_INT:
        break;
    default:
        break;
    }

    return RT_EOK;
}

const struct rt_touch_ops ops =
{
    ft6236_readpoint,
    ft6236_control,
};

struct rt_touch_info info =
{
    RT_TOUCH_TYPE_CAPACITANCE,
    RT_TOUCH_VENDOR_FT,
    2,
    320,
    480,
};

int rt_hw_ft6236_init(const char *name, struct rt_touch_config *cfg,  rt_base_t pin)
{
    rt_touch_t touch_device = RT_NULL;
    struct rt_i2c_bus_device * i2c_bus = RT_NULL;

    touch_device = (rt_touch_t)rt_calloc(1, sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL)
    {
        return -RT_ENOMEM;
    }

    i2c_bus = rt_i2c_bus_device_find(cfg->dev_name);
    if (i2c_bus == RT_NULL)
    {
        return -RT_EIO;
    }

    ft6236_init(i2c_bus, pin);

    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));

    touch_device->info = info;
    touch_device->ops = &ops;

    if (rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_RDONLY, RT_NULL) != RT_EOK)
    {
        return -RT_EIO;
    }

    return RT_EOK;
}
