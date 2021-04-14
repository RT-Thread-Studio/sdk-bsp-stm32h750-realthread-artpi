/*
#include <inc/InfoNES.h>
#include <ui_nes.h>
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-01     Ghazigq     the first version
 */
#include "ui_nes.h"
#include "rtdevice.h"
#include "ui_control.h"
#include "ui_home.h"
#include "InfoNES.h"

uint8_t *nes_buffer = NULL;

extern void nesmain(const char *name);

static lv_img_dsc_t nes_img = {
  .header.always_zero = 0,
  .header.w = NES_DISP_WIDTH,
  .header.h = NES_DISP_HEIGHT,
  .data_size = NES_DISP_WIDTH * NES_DISP_HEIGHT * 32 / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR,
};
static lv_obj_t *ui_obj = NULL;
static lv_obj_t *nes_img_obj = NULL;
static bool nes_is_open = false;

extern void touch_set_nes_mode(rt_bool_t value);

//nes interface

void nes_area_flush(uint8_t * dat)
{
    if(nes_img_obj != RT_NULL && dat)
    {
        for(int i = 0;i < NES_DISP_HEIGHT * NES_DISP_WIDTH;i ++)
        {
            WORD wColor = WorkFrame[i];
            /* Exchange 16-bit to 24-bit  RGB565 to ARGB8888*/
    //        nes_buffer[4*i + 3] = 0;  //Alpha you need't set value when initialize nes_buffer to zero
            dat[4*i + 2] = ((wColor & 0xf800) >> 10) << 3;
            dat[4*i + 1] = ((wColor & 0x07e0) >> 5) << 3;
            dat[4*i + 0] = (wColor & 0x001f) << 3;
        }
        nes_img.data = dat;
        lv_img_set_src(nes_img_obj, &nes_img);
    }
}

void nes_get_key( unsigned long *pdwPad1, unsigned long *pdwPad2, unsigned long *pdwSystem)
{
    *pdwPad1 = ui_control_get_key_value();
    *pdwSystem = ui_control_get_cancel_value();
}

//app
static void nes_entry(void *parameter)
{
    nes_buffer = rt_malloc(NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(lv_color_t));

    touch_set_nes_mode(true);
    if(NULL != nes_buffer)
    {
        rt_memset(nes_buffer, 0x00, NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(lv_color_t));

        ui_control_enter(ui_obj);
        nes_control_init();

        nes_img.data = nes_buffer;
        nes_img_obj = lv_img_create(ui_obj, NULL);
        lv_img_set_src(nes_img_obj, &nes_img);
        lv_obj_set_size(nes_img_obj, NES_DISP_WIDTH, NES_DISP_HEIGHT);
        lv_obj_align(nes_img_obj, ui_obj, LV_ALIGN_IN_TOP_MID, 0, 0);

        nesmain((const char *)parameter);

        lv_obj_del(nes_img_obj);
        ui_control_exit();
        rt_free(nes_buffer);
        rt_free(parameter);
    }
    nes_is_open = false;

    touch_set_nes_mode(false);
    extern lv_obj_t *file_list;
    lv_obj_set_hidden(file_list, false);
}

void ui_nes_enter(lv_obj_t *obj, uint8_t *nes_name)
{
    if(nes_is_open || obj == NULL)
        return;
    nes_is_open = true;
    ui_obj = obj;
    rt_thread_t tid = rt_thread_create("nes", nes_entry, nes_name, 4096, 8, 10);
    if(NULL != tid);
        rt_thread_startup(tid);
}

