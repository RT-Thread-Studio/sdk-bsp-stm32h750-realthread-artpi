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
#include <fal.h>
#include "wifi.h"
#define DBG_TAG "bt"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define BT_FIRMWARE_PARTITION_NAME "bt_image"
#define BT_FIRMWARE_PATH "/bt/BCM43430A1.hcd"
#define BT_DB_FILE_PATH "/bt/btstack_.tlv"

const uint8_t bt_image_magic[10] = {0x4C, 0xFC, 0x46, 0x00, 0x17, 0x21, 0x00, 0x42, 0x52, 0x43};
extern void bt_stack_port_main(void);

void bluetooth_thread(void *param)
{
    bt_stack_port_main();
}

int bluetooth_firmware_check(void)
{
    uint8_t magic[10];
    struct fal_partition *bt_image = fal_partition_find(BT_FIRMWARE_PARTITION_NAME);
    if (bt_image == NULL)
    {
        LOG_E("cannot find bt image partition");
        return -1;
    }

    if (fal_partition_read(bt_image, 0, magic, 10) < 0)
    {
        LOG_E("cannot read bt image partition");
        return -1;
    }

    if (rt_memcmp(magic, bt_image_magic, 10) != 0)
    {
        LOG_W("cannot find bt firmware,please update bt firmware");
        while(1)
        {
            rt_thread_mdelay(1000);
        }
    }
    else
    {
        LOG_I("Vertify bt firmware success");
        return 0;
    }
}

int bluetooth_init(void)
{
    int fd = -1;
    rt_device_t wifi = NULL,bt_firmware = NULL;
    //wait for wifi is ready
    while (wifi == NULL)
    {
        wifi = rt_device_find(WIFI_DEVICE_NAME);
        rt_thread_mdelay(500);
    }
    LOG_D("wifi init success,start bluetooth");
    //wait for firmware is ready
    bluetooth_firmware_check();
    //create bt device fs
    bt_firmware = fal_char_device_create(BT_FIRMWARE_PARTITION_NAME);
    if(bt_firmware == NULL){
        LOG_E("bt firmware device create failed");
        return -1;
    }

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
