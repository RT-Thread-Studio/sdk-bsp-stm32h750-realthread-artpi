/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-10     zylx         first version
 */

#ifndef __LCD_PORT_H__
#define __LCD_PORT_H__

struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;

    struct rt_semaphore lcd_lock;

    /* 0:front_buf is being used 1: back_buf is being used*/
    rt_uint8_t cur_buf;
    rt_uint8_t *front_buf;
    rt_uint8_t *back_buf;
};

/* rt-thread 3.5 inch screen, 320 * 480 */
#define LCD_WIDTH           320
#define LCD_HEIGHT          480
#define LCD_BITS_PER_PIXEL  24
#define LCD_BYTES_PER_PIXEL  (LCD_BITS_PER_PIXEL / 8)
#define LCD_BUF_SIZE        (LCD_WIDTH * LCD_HEIGHT * LCD_BYTES_PER_PIXEL)
#define LCD_PIXEL_FORMAT    RTGRAPHIC_PIXEL_FORMAT_RGB888

#define LCD_BACKLIGHT_USING_GPIO
#define LCD_BL_PIN          GET_PIN(C, 6)
#define LCD_RES_PIN         GET_PIN(A, 3)
/* rt-thread 3.5 inch screen, 320 * 480 */

#endif /* __LCD_PORT_H__ */
