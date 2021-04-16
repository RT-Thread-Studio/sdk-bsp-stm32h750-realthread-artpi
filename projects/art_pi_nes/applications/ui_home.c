/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-01     Ghazigq      the first version
 */
#include "nes_cfg.h"
#include "ui_home.h"
#include "ui_nes_list.h"
#include "littlevgl2rtt.h"

#ifdef PKG_USING_PNG_DECODER
#include "png_decoder.h"
#endif

lv_obj_t *logo_png = NULL;
lv_obj_t * scr = NULL;


static void logo_event_cb(struct _lv_obj_t * obj, lv_event_t event)
{
    if(LV_EVENT_PRESSED == event)
    {
        lv_obj_del(logo_png);
        logo_png = NULL;
        ui_nes_list_enter(scr);
    }
}

extern const lv_img_dsc_t img_logo;

void ui_home_enter(lv_obj_t * obj)
{
    logo_png = lv_imgbtn_create(obj, NULL);
    lv_obj_set_size(logo_png, 210, 66);
    for(uint8_t i = 0;i < _LV_BTN_STATE_LAST;i ++)
        lv_imgbtn_set_src(logo_png, i, &img_logo);
    lv_obj_align(logo_png, obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(logo_png, logo_event_cb);
}

int ui_init(void)
{

    if(littlevgl2rtt_init("lcd") != 0)
        return -1;

#ifdef PKG_USING_PNG_DECODER
    png_decoder_init();
#endif

    scr = lv_cont_create(NULL, NULL);
    lv_disp_load_scr(scr);

    ui_home_enter(scr);

    return 0;
}
INIT_APP_EXPORT(ui_init);
