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

/*===================================================================*/
/*                                                                   */
/*             InfoNES_PadState() : Get a joypad state               */
/*                                                                   */
/*===================================================================*/
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
    extern void nes_get_key( unsigned long *pdwPad1, unsigned long *pdwPad2, unsigned long *pdwSystem);
    nes_get_key( pdwPad1, pdwPad2, pdwSystem);
}

