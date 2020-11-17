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
#include <rt_ota.h>
#include "wifi.h"

#define DBG_TAG "bt"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define BT_FIRMWARE_PARTITION_NAME "bt_image"
static const struct fal_partition *bt_partition = RT_NULL;

extern void bt_stack_port_main(void);

void bluetooth_thread(void *param)
{
    bt_stack_port_main();
}

int bluetooth_firmware_check(void)
{
    bt_partition = fal_partition_find(BT_FIRMWARE_PARTITION_NAME);
    if (bt_partition == NULL)
    {
        LOG_E("%s partition is not exist, please check your configuration!", BT_FIRMWARE_PARTITION_NAME);
        return -1;
    }
    else
    { //try to update the bt_image
        int result = 0;
        /* verify OTA download partition */
        if (rt_ota_init() >= 0 && rt_ota_part_fw_verify(fal_partition_find(RT_OTA_DL_PART_NAME)) >= 0)
        {
            /* do upgrade when check upgrade OK */
            if (rt_ota_check_upgrade() && (result = rt_ota_upgrade()) < 0)
            {
                log_e("OTA upgrade failed!");
                //TODO upgrade to safe image?
            }
            /* when upgrade success, erase download partition firmware.
             * The purpose is to prevent other programs from using.
             */
            if (result >= 0)
            {
                fal_partition_erase(fal_partition_find(RT_OTA_DL_PART_NAME), 0, 4096);
            }
        }
    }
    //check firmware validity
    if (rt_ota_part_fw_verify(bt_partition) < 0)
    {
        LOG_E("BT image was NOT found on %s partition!", BT_FIRMWARE_PARTITION_NAME);
        return -1;
    }
    return 0;
}

int bluetooth_init(void)
{
    rt_device_t wifi = NULL, bt_firmware = NULL;
    //wait for wifi is ready
    while (wifi == NULL)
    {
        wifi = rt_device_find(WIFI_DEVICE_NAME);
        rt_thread_mdelay(500);
    }
    LOG_D("bluetooth start init");
    //wait for firmware is ready
    if (bluetooth_firmware_check() < 0)
    {
        return -1;
    }
    //create bt device fs
    bt_firmware = fal_char_device_create(BT_FIRMWARE_PARTITION_NAME);
    if (bt_firmware == NULL)
    {
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
