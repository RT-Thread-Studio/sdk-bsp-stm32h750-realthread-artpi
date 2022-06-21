/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-01-28     Rudy Lo       The first version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>

#define LV_USE_PERF_MONITOR 1
#define LV_DISP_DEF_REFR_PERIOD 5

#ifdef BSP_USING_ILI9488
#   define LV_COLOR_16_SWAP         0
#   define LV_COLOR_DEPTH           32
#   define LV_HOR_RES_MAX           480
#   define LV_VER_RES_MAX           320
#else
#   define LV_COLOR_DEPTH           16
#   define LV_USE_GPU_STM32_DMA2D   1
#   define LV_GPU_DMA2D_CMSIS_INCLUDE "stm32h750xx.h"
#   define LV_HOR_RES_MAX           800
#   define LV_VER_RES_MAX           480
#endif

#define LV_USE_DEMO_RTT_MUSIC       1
#define LV_DEMO_RTT_MUSIC_AUTO_PLAY 1

#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_16       1

#endif
