/*!
 * \file      lora-radio-timer.c
 *
 * \brief     lora-radio timer 
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \author    DerekChen
 */

#include "lora-radio-rtos-config.h"
#include <stdint.h>
#include "lora-radio-timer.h"

#if defined ( LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD ) || defined ( LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD_NANO )
#ifndef PKG_USING_MULTI_RTIMER

void rtick_timer_init( rtick_timer_event_t *obj, void ( *callback )( void ) )
{
    int count = 0;
    
    char name[RT_NAME_MAX];
    rt_snprintf(name,8,"rtk_%d",count++);

    rt_timer_init(&(obj->timer),name,(void (*)(void*))callback,RT_NULL,1000,RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
}

void rtick_timer_start( rtick_timer_event_t *obj )
{
    rt_timer_start(&(obj->timer));
}

void rtick_timer_stop( rtick_timer_event_t *obj )
{
    rt_timer_stop(&(obj->timer));
}

void rtick_timer_reset( rtick_timer_event_t *obj )
{
    rtick_timer_stop(obj);
    rtick_timer_start(obj);
}

void rtick_timer_set_value( rtick_timer_event_t *obj, uint32_t value )
{
    uint32_t tick = rt_tick_from_millisecond(value);
    rt_timer_control(&(obj->timer),RT_TIMER_CTRL_SET_TIME,&tick);
}

TimerTime_t rtick_timer_get_current_time( void )
{
    uint32_t now = rt_tick_get();
    return  now;
}

TimerTime_t rtick_timer_get_elapsed_time( TimerTime_t past )
{
    return rt_tick_get() - past;
}

#endif

#endif // End Of  ( LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD ) || defined ( LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD_NANO )

