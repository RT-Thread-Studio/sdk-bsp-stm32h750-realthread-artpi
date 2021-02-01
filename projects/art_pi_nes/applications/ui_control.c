/*
 #include <inc/InfoNES.h>
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-02     Ghazigq       the first version
 */
#include "ui_control.h"
#include "nes_cfg.h"
#include "touch.h"
#include "InfoNES.h"

enum
{
    CONTROL_A = 0,
    CONTROL_B,
    CONTROL_SELECT,
    CONTROL_START,
    CONTROL_UP,
    CONTROL_DOWN,
    CONTROL_LEFT,
    CONTROL_RIGHT,
    CONTROL_CANCEL,
    CONTROL_NUM,
};

struct
{
    int16_t x;
    int16_t y;
    uint8_t state;
} touch_input[2];

struct
{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
} nes_btn[CONTROL_NUM];

extern const lv_img_dsc_t img_A;
extern const lv_img_dsc_t img_B;
extern const lv_img_dsc_t img_select;
extern const lv_img_dsc_t img_start;
extern const lv_img_dsc_t img_up;
extern const lv_img_dsc_t img_down;
extern const lv_img_dsc_t img_left;
extern const lv_img_dsc_t img_right;
extern const lv_img_dsc_t img_cancel;

static uint8_t * control_img_src[CONTROL_NUM] = { (uint8_t *) &img_A, (uint8_t *) &img_B, (uint8_t *) &img_select,
        (uint8_t *) &img_start, (uint8_t *) &img_up, (uint8_t *) &img_down, (uint8_t *) &img_left,
        (uint8_t *) &img_right, (uint8_t *) &img_cancel, };

lv_obj_t *control_obj[CONTROL_NUM] = { NULL };
lv_obj_t *control_obj_direction = NULL;

static int offset = 15;
void ui_control_enter(lv_obj_t *obj)
{
    control_obj_direction = lv_cont_create(obj, NULL);
    lv_obj_set_size(control_obj_direction, 112, 112);
    lv_obj_align(control_obj_direction, obj, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_click(control_obj_direction, false);
    lv_obj_set_style_local_border_width(control_obj_direction, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    for (uint8_t i = 0; i < CONTROL_NUM; i++)
    {
        if (i >= CONTROL_UP && i <= CONTROL_RIGHT)
            control_obj[i] = lv_imgbtn_create(control_obj_direction, NULL);
        else
            control_obj[i] = lv_imgbtn_create(obj, NULL);
        lv_obj_set_size(control_obj[i], 48, 48);
        for (uint8_t j = 0; j < _LV_BTN_STATE_LAST; j++)
            lv_imgbtn_set_src(control_obj[i], j, control_img_src[i]);
    }

    lv_obj_align(control_obj[CONTROL_UP], control_obj_direction, LV_ALIGN_IN_TOP_MID, 0, -offset);
    lv_obj_align(control_obj[CONTROL_DOWN], control_obj_direction, LV_ALIGN_IN_BOTTOM_MID, 0, offset);
    lv_obj_align(control_obj[CONTROL_LEFT], control_obj_direction, LV_ALIGN_IN_LEFT_MID, -offset, 0);
    lv_obj_align(control_obj[CONTROL_RIGHT], control_obj_direction, LV_ALIGN_IN_RIGHT_MID, offset, 0);

    lv_obj_align(control_obj[CONTROL_START], obj, LV_ALIGN_IN_BOTTOM_MID, 0, -16);
    lv_obj_align(control_obj[CONTROL_SELECT], control_obj[CONTROL_START], LV_ALIGN_OUT_LEFT_MID, -offset, 0);
    lv_obj_align(control_obj[CONTROL_CANCEL], control_obj[CONTROL_START], LV_ALIGN_OUT_RIGHT_MID, offset, 0);

    lv_obj_align(control_obj[CONTROL_B], obj, LV_ALIGN_IN_BOTTOM_RIGHT, -offset / 2, -offset / 2);
    lv_obj_align(control_obj[CONTROL_A], control_obj[CONTROL_B], LV_ALIGN_OUT_LEFT_MID, -(offset / 4),
            -(48 + offset / 4));
}

void ui_control_exit(void)
{
    for (uint8_t i = 0; i < CONTROL_NUM; i++)
        lv_obj_del(control_obj[i]);
    lv_obj_del(control_obj_direction);
}

void nes_control_init(void)
{
    for (uint8_t i = 0; i < CONTROL_NUM; i++)
    {
        if (i >= CONTROL_UP && i <= CONTROL_RIGHT)
        {
            nes_btn[i].x = lv_obj_get_x(control_obj_direction) + lv_obj_get_x(control_obj[i]);
            nes_btn[i].y = lv_obj_get_y(control_obj_direction) + lv_obj_get_y(control_obj[i]);
        }
        else
        {
            nes_btn[i].x = lv_obj_get_x(control_obj[i]);
            nes_btn[i].y = lv_obj_get_y(control_obj[i]);
        }
        nes_btn[i].width = lv_obj_get_width(control_obj[i]);
        nes_btn[i].height = lv_obj_get_height(control_obj[i]);
    }

    nes_btn[CONTROL_UP].y += offset;
    nes_btn[CONTROL_DOWN].y -= offset;
    nes_btn[CONTROL_LEFT].x += offset;
    nes_btn[CONTROL_RIGHT].x -= offset;

//    for (uint8_t i = 0; i < CONTROL_NUM; i++)
//        rt_kprintf("x:%d\ty:%d\twidth:%d\theight:%d\t\n", nes_btn[i].x, nes_btn[i].y, nes_btn[i].width,
//                nes_btn[i].height);
}

void nes_control_send_input_event(uint8_t num, int16_t x, int16_t y, uint8_t state)
{
    if (num < sizeof(touch_input) / sizeof(touch_input[0]))
    {
        touch_input[num].x = x;
        touch_input[num].y = y;
        touch_input[num].state = state;
    }
}

uint16_t ui_control_get_key_value(void)
{
    uint16_t value = 0;

    for (uint8_t i = 0; i < sizeof(touch_input) / sizeof(touch_input[0]); i++)
    {
        if (touch_input[i].state == RT_TOUCH_EVENT_DOWN || touch_input[i].state == RT_TOUCH_EVENT_MOVE)
        {
            for (uint8_t j = CONTROL_A; j <= CONTROL_RIGHT; j++)
            {
                if (touch_input[i].x >= nes_btn[j].x && touch_input[i].x < nes_btn[j].x + nes_btn[j].width
                        && touch_input[i].y >= nes_btn[j].y && touch_input[i].y < nes_btn[j].y + nes_btn[j].height)
                {
                    value |= 1 << j;
                    break;
                }
            }
        }
    }
    return value;
}

uint16_t ui_control_get_cancel_value(void)
{
    uint16_t value = 0;

    for (uint8_t i = 0; i < sizeof(touch_input) / sizeof(touch_input[0]); i++)
    {
        if (touch_input[i].state == RT_TOUCH_EVENT_DOWN || touch_input[i].state == RT_TOUCH_EVENT_MOVE)
        {
            if (touch_input[i].x >= nes_btn[CONTROL_CANCEL].x
                    && touch_input[i].x < nes_btn[CONTROL_CANCEL].x + nes_btn[CONTROL_CANCEL].width
                    && touch_input[i].y >= nes_btn[CONTROL_CANCEL].y
                    && touch_input[i].y < nes_btn[CONTROL_CANCEL].y + nes_btn[CONTROL_CANCEL].height)
            {
                value |= PAD_SYS_QUIT;
                return value;
            }
        }
    }
    return value;
}

