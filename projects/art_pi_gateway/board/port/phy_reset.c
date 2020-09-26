/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-23     flybreak     first version
 */

#include <board.h>
#include <rtdevice.h>

#define RESET_IO GET_PIN(A, 3)

void phy_reset(void)
{
    rt_pin_write(RESET_IO, PIN_LOW);
    rt_thread_mdelay(50);
    rt_pin_write(RESET_IO, PIN_HIGH);
}

int phy_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    rt_pin_mode(RESET_IO, PIN_MODE_OUTPUT);
    rt_pin_write(RESET_IO, PIN_HIGH);
    return RT_EOK;
}
INIT_BOARD_EXPORT(phy_init);
