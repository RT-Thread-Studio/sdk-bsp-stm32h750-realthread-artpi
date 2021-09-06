/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RTDEBUG_H__
#define __RTDEBUG_H__

#include <rtconfig.h>

/* settings depend check */
#ifdef RT_USING_POSIX
#if !defined(RT_USING_DFS) || !defined(RT_USING_DFS_DEVFS)
#error "POSIX poll/select, stdin need file system(RT_USING_DFS) and device file system(RT_USING_DFS_DEVFS)"
#endif

#if !defined(RT_USING_LIBC)
#error "POSIX layer need standard C library(RT_USING_LIBC)"
#endif

#endif

#ifdef RT_USING_POSIX_TERMIOS
#if !defined(RT_USING_POSIX)
#error "termios need POSIX layer(RT_USING_POSIX)"
#endif
#endif

/* Using this macro to control all kernel debug features. */
#ifdef RT_DEBUG

/* Turn on some of these (set to non-zero) to debug kernel */
#ifndef RT_DEBUG_MEM
#define RT_DEBUG_MEM                   0
#endif

#ifndef RT_DEBUG_MEMHEAP
#define RT_DEBUG_MEMHEAP               0
#endif

#ifndef RT_DEBUG_MODULE
#define RT_DEBUG_MODULE                0
#endif

#ifndef RT_DEBUG_SCHEDULER
#define RT_DEBUG_SCHEDULER             0
#endif

#ifndef RT_DEBUG_SLAB
#define RT_DEBUG_SLAB                  0
#endif

#ifndef RT_DEBUG_THREAD
#define RT_DEBUG_THREAD                0
#endif

#ifndef RT_DEBUG_TIMER
#define RT_DEBUG_TIMER                 0
#endif

#ifndef RT_DEBUG_IRQ
#define RT_DEBUG_IRQ                   0
#endif

#ifndef RT_DEBUG_IPC
#define RT_DEBUG_IPC                   0
#endif

#ifndef RT_DEBUG_INIT
#define RT_DEBUG_INIT                  0
#endif

/* Turn on this to enable context check */
#ifndef RT_DEBUG_CONTEXT_CHECK
#define RT_DEBUG_CONTEXT_CHECK         1
#endif

#define RT_DEBUG_LOG(type, message)                                           \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
        rt_kprintf message;                                                   \
}                                                                             \
while (0)

#define RT_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    rt_assert_handler(#EX, __FUNCTION__, __LINE__);                           \
}

/* Macro to check current context */
#if RT_DEBUG_CONTEXT_CHECK
#define RT_DEBUG_NOT_IN_INTERRUPT                                             \
do                                                                            \
{                                                                             \
    rt_base_t level;                                                          \
    level = rt_hw_interrupt_disable();                                        \
    if (rt_interrupt_get_nest() != 0)                                         \
    {                                                                         \
        rt_kprintf("Function[%s] shall not be used in ISR\n", __FUNCTION__);  \
        RT_ASSERT(0)                                                          \
    }                                                                         \
    rt_hw_interrupt_enable(level);                                            \
}                                                                             \
while (0)

/* "In thread context" means:
 *     1) the scheduler has been started
 *     2) not in interrupt context.
 */
#define RT_DEBUG_IN_THREAD_CONTEXT                                            \
do                                                                            \
{                                                                             \
    rt_base_t level;                                                          \
    level = rt_hw_interrupt_disable();                                        \
    if (rt_thread_self() == RT_NULL)                                          \
    {                                                                         \
        rt_kprintf("Function[%s] shall not be used before scheduler start\n", \
                   __FUNCTION__);                                             \
        RT_ASSERT(0)                                                          \
    }                                                                         \
    RT_DEBUG_NOT_IN_INTERRUPT;                                                \
    rt_hw_interrupt_enable(level);                                            \
}                                                                             \
while (0)
#else
#define RT_DEBUG_NOT_IN_INTERRUPT
#define RT_DEBUG_IN_THREAD_CONTEXT
#endif

#else /* RT_DEBUG */

#define RT_ASSERT(EX)
#define RT_DEBUG_LOG(type, message)
#define RT_DEBUG_NOT_IN_INTERRUPT
#define RT_DEBUG_IN_THREAD_CONTEXT

#endif /* RT_DEBUG */

#endif /* __RTDEBUG_H__ */
