/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-01     Forest-rain   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "multi_rtimer.h"
#include "hw_rtc_stm32.h"

static timer_event_t led0_timer;
static timer_event_t led1_timer;
static timer_event_t led2_timer;
static timer_event_t led3_timer;

uint32_t interval_ms = 1;


/*!
 * \brief Function executed on Led 0 Timeout event
 */
static void on_led0_timeout(void)
{
    rtimer_stop( &led0_timer );
    
    rt_kprintf("on_led0_timeout,%d,on shot\r\n",rtimer_get_current_time());

}

/*!
 * \brief Function executed on Led 1 Timeout event
 */
static void on_led1_timeout(void)
{
    rtimer_reset( &led1_timer );
    
    rt_kprintf("on_led1_timeout,%d\r\n",rtimer_get_current_time());
}

/*!
 * \brief Function executed on Led 2 Timeout event
 */
static void on_led2_timeout(void)
{
    rtimer_reset( &led2_timer );
    
    rt_kprintf("on_led2_timeout,%d\r\n",rtimer_get_current_time());
}

/*!
 * \brief Function executed on Led 3 Timeout event
 */
static void on_led3_timeout(void)
{
    interval_ms = interval_ms * 10;
    
    // stop timer then set new timer interval
    TimerSetValue( &led3_timer,interval_ms);
    TimerStart( &led3_timer );
    
    rt_kprintf("on_led3_timeout:%d,interval:%d ms\r\n",rtimer_get_current_time(),interval_ms);
}

int rt_multi_rtimer_init(void)
{
    hw_rtc_init();
    
    // on shot
    rtimer_init( &led0_timer, on_led0_timeout );
    rtimer_set_value( &led0_timer, 60000 ); // 60s
    
    rtimer_start( &led0_timer );
    rt_kprintf("led0_timer start,%d\r\n",rtimer_get_current_time());
    
    // periodicity
    rtimer_init( &led1_timer, on_led1_timeout );
    rtimer_set_value( &led1_timer, 1000 ); // 1000ms
    
    rtimer_start( &led1_timer );
    rt_kprintf("led1_timer start,%d\r\n",rtimer_get_current_time());

    rtimer_init( &led2_timer, on_led2_timeout );
    rtimer_set_value( &led2_timer, 500 ); // 500ms

    rtimer_start( &led2_timer );
    rt_kprintf("led2_timer start,%d\r\n",rtimer_get_current_time());
    
    // change the interval
    // Use LoRaWAN Timer API
    TimerInit( &led3_timer, on_led3_timeout );
    TimerSetValue( &led3_timer, interval_ms ); // 1ms * 10^n
    
    TimerStart( &led3_timer );
    rt_kprintf("led3_timer start,%d\r\n",rtimer_get_current_time());
    
    #ifdef RT_USING_PM
    rt_pm_request(PM_SLEEP_MODE_DEEP);
    #endif
    
    return 0;
}

INIT_APP_EXPORT(rt_multi_rtimer_init);


