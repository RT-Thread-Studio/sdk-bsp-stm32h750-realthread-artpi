/*************************************************
Copyright (c) 2020
All rights reserved.
File name:     monitor.c
Description:   
History:
1. Version:    V1.0.0
Date:      2020-09-13
Author:    WKJay
Modify:    
*************************************************/
#include "monitor.h"
#include "wifi.h"
#include <ntp.h>

#define SYS_MONITOR_INTERVAL_MS 1000
#define NTP_SYNC_CYCLE_MINUTES 30

#define DBG_TAG "monitor"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

struct _sys
{
    rt_uint32_t running_time;
    rt_uint32_t rtc_sync_cnt;
} sys;

static void sys_running_time_calc(void)
{
    sys.running_time++;
}

rt_uint32_t get_sys_running_time(void)
{
    return (sys.running_time * SYS_MONITOR_INTERVAL_MS) / 1000; //s
}

static void sys_monitor_handler(void)
{
    sys_running_time_calc();
}

static void ntp_sync_handler(void)
{
    if (!wifi_is_ready())
        return;

    if (sys.rtc_sync_cnt)
    {
        sys.rtc_sync_cnt--;
    }
    else
    {
        if (ntp_sync_to_rtc(NULL) == 0)
        {
            return;
        }
        LOG_I("NTP sync success");
        sys.rtc_sync_cnt = (NTP_SYNC_CYCLE_MINUTES * 60 * 1000) / SYS_MONITOR_INTERVAL_MS;
    }
}

static void sys_monitor_thread(void *param)
{
    while (1)
    {
        sys_monitor_handler();
        ntp_sync_handler();
        rt_thread_mdelay(SYS_MONITOR_INTERVAL_MS);
    }
}

int sys_monitor_init(void)
{
    rt_memset(&sys, 0, sizeof(sys));
    rt_thread_t tid = rt_thread_create("sys", sys_monitor_thread, NULL, 2048, 15, 5);
    if (tid)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("sys monitor init failed!");
        return -1;
    }
    return 0;
}
