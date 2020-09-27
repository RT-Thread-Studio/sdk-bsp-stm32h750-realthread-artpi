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
#define SYS_MONITOR_INTERVAL_MS 1000

#define DBG_TAG "monitor"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

struct _sys
{
    rt_uint32_t running_time;
} sys;

static void sys_running_time_calc(void)
{
    sys.running_time++;
}

rt_uint32_t get_sys_running_time(void)
{
    return (sys.running_time * SYS_MONITOR_INTERVAL_MS)/1000; //s
}

static void sys_monitor_handler(void)
{
    sys_running_time_calc();
}

static void sys_monitor_thread(void)
{
    while (1)
    {
        sys_monitor_handler();
        rt_thread_mdelay(SYS_MONITOR_INTERVAL_MS);
    }
}

int sys_monitor_init(void)
{
    rt_memset(&sys, 0, sizeof(sys));
    rt_thread_t tid = rt_thread_create("ram_wave", sys_monitor_thread, NULL, 1024, 15, 5);
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
