/*************************************************
 Copyright (c) 2019
 All rights reserved.
 File name:     web.c
 Description:   
 History:
 1. Version:    
    Date:       2020-09-04
    Author:     wangjunjie
    Modify:     
*************************************************/
#include <rtthread.h>
#include "web.h"
#include <wlan_mgnt.h>

#define DBG_TAG "web_init"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static void web_init_thread(void *param)
{
    while (!rt_wlan_is_ready())
    {
        rt_thread_mdelay(500);
    }
    webnet_init();
}

int web_init(void)
{
    rt_thread_t tid = rt_thread_create("web_init", web_init_thread, NULL, 512, 15, 5);
    if (tid)
    {
        rt_thread_startup(tid);
        return 0;
    }
    else
    {
        LOG_E("web init thread create failed");
        return -1;
    }
}
