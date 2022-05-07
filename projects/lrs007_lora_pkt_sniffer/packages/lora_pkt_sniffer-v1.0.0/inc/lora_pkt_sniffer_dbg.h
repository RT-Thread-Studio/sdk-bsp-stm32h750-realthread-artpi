/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LORA_PACKET_SNIFFER_DEBUG_H__
#define __LORA_PACKET_SNIFFER_DEBUG_H__

#include "rtconfig.h"

#ifdef RT_USING_ULOG
#include <rtdbg.h>
#include <ulog.h> 
#endif

/* Using this macro to control all LoRa Packet Driver debug features. */
#ifdef LORA_PKT_SNIFFER_DEBUG

/* Turn on some of these (set to non-zero) to debug LoRa Packet Driver */

#ifndef LPS_DBG_SNF
#define LPS_DBG_SNF           0
#endif

#ifndef LPS_DBG_UDP
#define LPS_DBG_UDP           0
#endif

#ifndef LPS_DBG_SHELL
#define LPS_DBG_SHELL         0
#endif

#ifndef LPS_DBG_NVM
#define LPS_DBG_NVM           0
#endif

#if defined RT_USING_ULOG

#define LPS_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_output(level, LOG_TAG, RT_TRUE, __VA_ARGS__);                    \
    }                                                                         \
}                                                                             \
while (0)

#define LPS_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_raw(__VA_ARGS__);                                                \
    }                                                                         \
}                                                                             \
while (0)

#define LPS_DEBUG_LOG_HEXDUMP(type, buf, size)                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_hexdump(LOG_TAG, 16, buf, size);                                 \
    }                                                                         \
}                                                                             \
while (0)

#else

#define LPS_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)

#define LPS_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)
#define LPS_DEBUG_LOG_HEXDUMP(type, buf, size)

#endif /* RT_USING_ULOG */

#else /* LPS_DEBUG */

#define LPS_DEBUG_LOG(type, level, ...)
#define LPS_DEBUG_RAW(type, ...)
#define LPS_DEBUG_LOG_HEXDUMP(type, buf, size)
#endif /* LORA_PACKET_SNIFFER_DEBUG */

#endif /* __LORA_PACKET_SNIFFER_DEBUG_H__ */
