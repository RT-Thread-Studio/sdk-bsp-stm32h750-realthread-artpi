/*************************************************
Copyright (c) 2020
All rights reserved.
File name:     wifi.c
Description:   
History:
1. Version:    V1.0.0
Date:      2020-09-15
Author:    WKJay
Modify:    
*************************************************/
#include <rtthread.h>
#include <rtdevice.h>
#include <cJSON.h>
#include <wlan_mgnt.h>
#include <netdev_ipaddr.h>
#include <netdev.h>

#include "wifi.h"

#define DBG_TAG "wifi"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MAX_SSID_PASSWD_STR_LEN 50
#define BT_SEND_TIMES 1
#define BT_SEND_FAIL_RETRY 3

extern int bt_stack_blufi_send(uint8_t *string, uint32_t length);
extern int adb_socket_init(void);

struct _wifi
{
    char ssid[MAX_SSID_PASSWD_STR_LEN];
    char passwd[MAX_SSID_PASSWD_STR_LEN];
} wifi;

int wifi_connect(char *conn_str)
{
    cJSON *conn = cJSON_Parse(conn_str);
    if (conn)
    {
        cJSON *ssid = cJSON_GetObjectItem(conn, "ssid");
        cJSON *passwd = cJSON_GetObjectItem(conn, "passwd");
        if (ssid && passwd)
        {
            if (rt_strlen(ssid->valuestring) > MAX_SSID_PASSWD_STR_LEN ||
                rt_strlen(passwd->valuestring) > MAX_SSID_PASSWD_STR_LEN)
            {
                LOG_E("invalid ssid or passwd length,max %d", MAX_SSID_PASSWD_STR_LEN);
            }
            else
            {
                rt_memcpy(wifi.ssid, ssid->valuestring, rt_strlen(ssid->valuestring));
                rt_memcpy(wifi.passwd, passwd->valuestring, rt_strlen(passwd->valuestring));
                return rt_wlan_connect(wifi.ssid, wifi.passwd);
            }
        }
        else
        {
            LOG_E("cannot find ssid or password.");
        }

        cJSON_Delete(conn);
    }
    else
    {
        LOG_E("invalid wifi connection string.");
    }
    return -1;
}

int wifi_is_ready(void)
{
    return rt_wlan_is_ready();
}

char *wifi_get_ip(void)
{
    struct netdev *dev = netdev_get_by_name(WIFI_DEVICE_NAME);
    if (dev)
    {
        return inet_ntoa(dev->ip_addr);
    }
    else
    {
        return NULL;
    }
}

static void wifi_ready_handler(void *param)
{
    int cnt = BT_SEND_TIMES;
    char temp_string[100];
    //adb init
    adb_socket_init();

    //wifi status send
    memset(temp_string, 0, sizeof(temp_string));
    while (cnt--)
    {
        int retry_cnt = BT_SEND_FAIL_RETRY;
        uint8_t wifi_status = wifi_is_ready();
        char *wifi_ip = wifi_get_ip();
        sprintf(temp_string, "{wifi:'%s', url:'%s'}", wifi_status ? "on" : "off", wifi_ip);
        while (bt_stack_blufi_send(temp_string, strlen(temp_string)) < 0)
        {
            if (retry_cnt == 0)
                break;
            retry_cnt--;
            rt_thread_mdelay(1000);
        }
        rt_thread_mdelay(5000);
    }
}

int wifi_init(void)
{
    rt_memset(&wifi, 0, sizeof(wifi));
    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wifi_ready_handler, NULL);
    return 0;
}
