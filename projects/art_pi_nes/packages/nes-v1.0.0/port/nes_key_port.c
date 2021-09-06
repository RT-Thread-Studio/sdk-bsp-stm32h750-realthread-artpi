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
    //            ��λ---------------------->��λ
    //pdwPad1   : A��    B��   ѡ��  ��ʼ  ��  ��  ��  ��
    //pdwPad2   : A��    B��   ѡ��  ��ʼ  ��  ��  ��  ��
    //pdwSystem : �˳�  ȷ�� ȡ��  ��      ��  ��  ��  NULL
}

