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
    extern uint8_t *nes_buffer;
    extern void nes_area_flush(uint8_t * dat);
    nes_area_flush(nes_buffer);
}
