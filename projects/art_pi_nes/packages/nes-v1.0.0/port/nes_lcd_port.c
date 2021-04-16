/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     Ghazigq       the first version
 */
#include <nes_cfg.h>
#include <InfoNES_Types.h>
#include <InfoNES.h>
#include <rtthread.h>
#include <rtdevice.h>

static uint8_t nes_lcd_is_init = 0;
static rt_device_t lcd_device = NULL;
static struct rt_device_graphic_info info;
/*===================================================================*/
/*                                                                   */
/*      InfoNES_LoadFrame() :                                        */
/*           Transfer the contents of work frame on the screen       */
/*                                                                   */
/*===================================================================*/
void InfoNES_LoadFrame()
{
    /*
     *  Transfer the contents of work frame on the screen
     *
     */
#include <lcd_spi_port.h>
    struct rt_device_rect_info rect_info;
    WORD *wColor = WorkFrame;

    if (!nes_lcd_is_init)
    {
        lcd_device = rt_device_find("lcd");
        RT_ASSERT(lcd_device != RT_NULL);
        if (rt_device_open(lcd_device, RT_DEVICE_OFLAG_RDWR) == RT_EOK)
            rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
        nes_lcd_is_init = 1;
    }
    rect_info.x = 0;
    rect_info.y = 0;
    rect_info.width = NES_DISP_WIDTH;
    rect_info.height = NES_DISP_HEIGHT;

    for (int y = rect_info.y; y < rect_info.y + rect_info.height; y ++)
    {
        for (int x = rect_info.x; x < rect_info.x + rect_info.width; x ++)
        {
            wColor ++;
            /* Exchange 16-bit to 24-bit  RGB565 to RGB888*/
            info.framebuffer[3 * (x + y * info.width)] = ((*wColor & 0xf800) >> 10) << 3;
            info.framebuffer[3 * (x + y * info.width) + 1] = ((*wColor & 0x07e0) >> 5) << 3;
            info.framebuffer[3 * (x + y * info.width) + 2] = (*wColor & 0x001f) << 3;
        }
    }
    rt_device_control(lcd_device, RTGRAPHIC_CTRL_RECT_UPDATE, &rect_info);
}
