/*************************************************
Copyright (c) 2020
All rights reserved.
File name:     bluetooth.c
Description:   
History:
1. Version:    V1.0.0
Date:      2020-09-16
Author:    WKJay
Modify:    
*************************************************/
#include <rtthread.h>
#include "wifi.h"
#define DBG_TAG "bt"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define BT_FIRMWARE_PATH "/bt/BCM43430A1.hcd"
#define BT_DB_FILE_PATH "/bt/btstack_.tlv"
extern void bt_stack_port_main(void);

void bluetooth_thread(void *param)
{
    bt_stack_port_main();
}

int bluetooth_init(void)
{
    int fd = -1;
    rt_device_t wifi = NULL;
    //wait for wifi is ready
    while(wifi == NULL)
    {
        wifi = rt_device_find(WIFI_DEVICE_NAME);
        rt_thread_mdelay(500); 
    }
    LOG_D("wifi init success,start bluetooth");
    //wait for firmware is ready
    while(fd<0)
    {
        fd = open(BT_FIRMWARE_PATH,O_RDONLY);
        rt_thread_mdelay(1000);
    }
    close(fd);
    
    fd = -1;
    while(fd<0)
    {
        fd = open(BT_DB_FILE_PATH,O_CREAT);
        rt_thread_mdelay(1000);
    }
    close(fd);
    

    rt_thread_t tid = rt_thread_create("bt_thread", bluetooth_thread, NULL, 1024, 15, 5);
    if (tid)
    {
        rt_thread_startup(tid);
        return 0;
    }
    else
    {
        LOG_E("bluetooth thread create failed");
        return -1;
    }
}
