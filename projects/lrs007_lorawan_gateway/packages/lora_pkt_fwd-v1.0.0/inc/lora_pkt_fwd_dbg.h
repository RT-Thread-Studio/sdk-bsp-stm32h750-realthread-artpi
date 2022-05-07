/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LORA_PKT_FWD_DEBUG_H__
#define __LORA_PKT_FWD_DEBUG_H__

#include "rtconfig.h"

#ifdef RT_USING_ULOG
#include <rtdbg.h>
#include <ulog.h> 
#endif

/* Using this macro to control all LoRa Packet Forward debug features. */
#ifdef LORA_PKT_FWD_DEBUG

/* Turn on some of these (set to non-zero) to debug LoRa Packet Forward */

#ifndef LPF_DBG_GWMP
#define LPF_DBG_GWMP           0
#endif

#ifndef LPF_DBG_SHELL
#define LPF_DBG_SHELL          0
#endif

#ifndef LPF_DBG_PARAMS_CONF
#define LPF_DBG_PARAMS_CONF    0
#endif

#if defined RT_USING_ULOG

#define LPF_DEBUG_LOG(type, level, ...)                                       \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_output(level, LOG_TAG, RT_TRUE, __VA_ARGS__);                    \
    }                                                                         \
}                                                                             \
while (0)

#define LPF_DEBUG_LOG_RAW(type, ...)                                          \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_raw(__VA_ARGS__);                                                \
    }                                                                         \
}                                                                             \
while (0)

#define LPF_DEBUG_LOG_HEXDUMP(type, buf, size)                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_hexdump(LOG_TAG, 16, buf, size);                                 \
    }                                                                         \
}                                                                             \
while (0)

#else

#define LPF_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)

#define LPF_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)
#define LPF_DEBUG_LOG_HEXDUMP(type, buf, size)

#endif /* RT_USING_ULOG */


#else /* LPF_DEBUG */

#define LPF_DEBUG_LOG(type, level, ...)
#define LPF_DEBUG_RAW(type, ...)
#define LPF_DEBUG_LOG_HEXDUMP(type, buf, size)
#endif /* LORA_PACKET_SNIFFER_DEBUG */

#endif /* __LORA_PKT_FWD_DEBUG_H__ */
