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
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem)
{
    //            低位---------------------->高位
    //pdwPad1   : A键    B键   选择  开始  上  下  左  右
    //pdwPad2   : A键    B键   选择  开始  上  下  左  右
    //pdwSystem : 退出  确认 取消  上      下  左  右  NULL
}

