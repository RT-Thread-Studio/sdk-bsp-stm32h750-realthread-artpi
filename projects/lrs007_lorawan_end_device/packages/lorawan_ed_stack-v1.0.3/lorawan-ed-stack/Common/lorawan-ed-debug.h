/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LORAWAN_ED_DEBUG_H__
#define __LORAWAN_ED_DEBUG_H__

#include "rtconfig.h"

#ifdef RT_USING_ULOG
#include <rtdbg.h>
#include <ulog.h> 
#endif

/* Using this macro to control all LoRa Radio Driver debug features. */
#ifdef LORAWAN_ED_STACK_DEBUG

/* Turn on some of these (set to non-zero) to debug LoRaWAN End-Device Stack */
#ifndef LORAWAN_ED_STACK_DEBUG_APP
#define LORAWAN_ED_STACK_DEBUG_APP                0
#endif

#ifndef LORAWAN_ED_STACK_DEBUG_APP_DATA
#define LORAWAN_ED_STACK_DEBUG_APP_DATA           0
#endif

#ifndef LORAWAN_ED_STACK_DEBUG_APS
#define LORAWAN_ED_STACK_DEBUG_APS                0
#endif

#ifndef LORAWAN_ED_STACK_DEBUG_MAC
#define LORAWAN_ED_STACK_DEBUG_MAC                0
#endif

#ifndef LORAWAN_ED_STACK_DEBUG_MAC_COMMAND
#define LORAWAN_ED_STACK_DEBUG_MAC_COMMAND        0
#endif

#ifndef LORAWAN_ED_STACK_DEBUG_PHY_REGION
#define LORAWAN_ED_STACK_DEBUG_PHY_REGION         0
#endif

#if ( defined LORAWAN_ED_STACK_USING_ON_RTOS_RT_THREAD ) // || ( defined LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD_NANO )

#if defined RT_USING_ULOG

#define LORAWAN_ED_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_output(level, LOG_TAG, RT_TRUE, __VA_ARGS__);                    \
    }                                                                         \
}                                                                             \
while (0)

#define LORAWAN_ED_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_raw(__VA_ARGS__);                                                \
    }                                                                         \
}                                                                             \
while (0)

#define LORAWAN_ED_DEBUG_LOG_HEXDUMP(type, buf, size)                         \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_hexdump(LOG_TAG, 16, buf, size);                                 \
    }                                                                         \
}                                                                             \
while (0)

#else

#define LORAWAN_ED_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)

#define LORAWAN_ED_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)
#endif /* RT_USING_ULOG */

#endif  /* defined LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD ) || ( defined LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD_NANO  */

#else /* LORAWAN_ED_DEBUG */

#define LORAWAN_ED_DEBUG_LOG(type, level, ...)
#define LORAWAN_ED_DEBUG_RAW(type, ...)
#define LORAWAN_ED_DEBUG_LOG_HEXDUMP(type, buf, size)
#endif /* LORAWAN_ED_DEBUG */

#endif /* __LORAWAN_ED_DEBUG_H__ */
