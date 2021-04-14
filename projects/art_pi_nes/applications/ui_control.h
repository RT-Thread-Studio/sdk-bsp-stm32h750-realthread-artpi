/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-02     Ghazigq       the first version
 */
#ifndef _UI_CONTROL_H_
#define _UI_CONTROL_H_

#include "lvgl.h"
#include "stdint.h"

void ui_control_enter(lv_obj_t *obj);
void ui_control_exit(void);
uint16_t ui_control_get_key_value(void);
uint16_t ui_control_get_cancel_value(void);
void nes_control_init(void);
void nes_control_send_input_event(uint8_t num, int16_t x, int16_t y, uint8_t state);

#endif /* _UI_CONTROL_H_ */
