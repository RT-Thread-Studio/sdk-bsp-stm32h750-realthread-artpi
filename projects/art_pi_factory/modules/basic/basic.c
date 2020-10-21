/*************************************************
Copyright (c) 2020
All rights reserved.
File name:     basic.c
Description:   
History:
1. Version:    V1.0.0
Date:      2020-09-06
Author:    WKJay
Modify:    
*************************************************/
#include <rtthread.h>
#include <drv_common.h>
#include <web.h>
#include <dfs_fs.h>
#include <ntp.h>
#include <wlan_mgnt.h>
#include "json_common.h"
#include "monitor.h"
#include "netdev.h"

#define FIRMWARE_VERSION "V1.0.0"
#define BLUE_LED_DEV_NAME "dev0"
#define RED_LED_DEV_NAME "dev1"
#define BEEP_DEV_NAME "dev2"

#define NORFLASH_PATH "/flash"
#define SD_PATH "/sdcard"

#define BLUE_LED GET_PIN(I, 8)
#define RED_LED GET_PIN(C, 15)
static uint8_t blue_led_stat = 0, red_led_stat = 0;

static char *board_control(const char *dev, uint8_t code)
{
    if (strcmp(dev, BLUE_LED_DEV_NAME) == 0)
    {
        rt_pin_write(BLUE_LED, !code);
        blue_led_stat = code;
    }
    else if (strcmp(dev, RED_LED_DEV_NAME) == 0)
    {
        rt_pin_write(RED_LED, !code);
        red_led_stat = code;
    }
    else
    {
        return json_create_web_response(-1, "unknown control device!");
    }
    return json_create_web_response(0, "ok");
}

static char *json_create_basic_info(void)
{
    char temp[100];

    //ram usage
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "code", cJSON_CreateNumber(0));
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);

    rt_memset(temp, 0, sizeof(temp));

    rt_uint32_t total_mem = 0, used_mem = 0, max_used_mem = 0;
    rt_object_t mem_obj = rt_object_find("heap", RT_Object_Class_MemHeap);
    if (mem_obj)
    {
        struct rt_memheap *mh = (struct rt_memheap *)mem_obj;
        total_mem += mh->pool_size;
        used_mem += (mh->pool_size - mh->available_size);
        max_used_mem += mh->max_used_size;
    }
    mem_obj = rt_object_find("sdram", RT_Object_Class_MemHeap);
    if (mem_obj)
    {
        struct rt_memheap *mh = (struct rt_memheap *)mem_obj;
        total_mem += mh->pool_size;
        used_mem += (mh->pool_size - mh->available_size);
        max_used_mem += mh->max_used_size;
    }

    snprintf(temp, sizeof(temp), "%u", total_mem);
    cJSON_AddItemToObject(payload, "id0", cJSON_CreateString(temp));
    snprintf(temp, sizeof(temp), "%u", used_mem);
    cJSON_AddItemToObject(payload, "id1", cJSON_CreateString(temp));
    snprintf(temp, sizeof(temp), "%u", max_used_mem);
    cJSON_AddItemToObject(payload, "id2", cJSON_CreateString(temp));

    //server date
    time_t server_time = ntp_get_local_time(NULL);
    if (server_time)
    {
        cJSON_AddItemToObject(payload, "id4", cJSON_CreateString(ctime((const time_t *)&server_time)));
    }

    //norflash usage
    struct statfs usage_stat;
    if (dfs_statfs(NORFLASH_PATH, &usage_stat) != 0)
    {
        snprintf(temp, sizeof(temp), "%d", 0);
        cJSON_AddItemToObject(payload, "id6", cJSON_CreateString(temp));
        cJSON_AddItemToObject(payload, "id7", cJSON_CreateString(temp));
    }
    else
    {
        uint64_t total_fbsize = ((uint64_t)usage_stat.f_bsize) * ((uint64_t)usage_stat.f_blocks);
        snprintf(temp, sizeof(temp), "%llu", total_fbsize);
        cJSON_AddItemToObject(payload, "id6", cJSON_CreateString(temp));

        uint64_t used_fbsize = ((uint64_t)usage_stat.f_bsize) * ((uint64_t)(usage_stat.f_blocks - usage_stat.f_bfree));
        snprintf(temp, sizeof(temp), "%llu", used_fbsize);
        cJSON_AddItemToObject(payload, "id7", cJSON_CreateString(temp));
    }

    //sd usage
    if (dfs_statfs(SD_PATH, &usage_stat) != 0)
    {
        snprintf(temp, sizeof(temp), "%d", 0);
        cJSON_AddItemToObject(payload, "id8", cJSON_CreateString(temp));
        cJSON_AddItemToObject(payload, "id9", cJSON_CreateString(temp));
    }
    else
    {
        uint64_t total_fbsize = ((uint64_t)usage_stat.f_bsize) * ((uint64_t)usage_stat.f_blocks);
        snprintf(temp, sizeof(temp), "%llu", total_fbsize);
        cJSON_AddItemToObject(payload, "id8", cJSON_CreateString(temp));

        uint64_t used_fbsize = ((uint64_t)usage_stat.f_bsize) * ((uint64_t)(usage_stat.f_blocks - usage_stat.f_bfree));
        snprintf(temp, sizeof(temp), "%llu", used_fbsize);
        cJSON_AddItemToObject(payload, "id9", cJSON_CreateString(temp));
    }

    cJSON_AddItemToObject(payload, "id12", cJSON_CreateString(FIRMWARE_VERSION)); //BLUE LED
    cJSON_AddItemToObject(payload, "id14", cJSON_CreateBool(blue_led_stat));      //BLUE LED
    cJSON_AddItemToObject(payload, "id15", cJSON_CreateBool(red_led_stat));       //RED LED

    snprintf(temp, sizeof(temp), "%d", rt_wlan_get_rssi());
    cJSON_AddItemToObject(payload, "id16", cJSON_CreateString(temp)); //WIFI RSSI

    if (netdev_default)
    {
        cJSON_AddItemToObject(payload, "id17", cJSON_CreateBool(netdev_is_internet_up(netdev_default))); //internet
    }

    snprintf(temp, sizeof(temp), "%u", get_sys_running_time());
    cJSON_AddItemToObject(payload, "id18", cJSON_CreateString(temp)); //running time

    char *json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_data;
}

static void cgi_basic_info(struct webnet_session *session)
{
    cgi_head();
    request = request;//clear warning
    body = json_create_basic_info();
    webnet_session_printf(session, body);
    rt_free(body);
}

static void cgi_board_control(struct webnet_session *session)
{
    cgi_head();
    request = request;//clear warning
    if (session->request->query)
    {
        cJSON *root = cJSON_Parse(session->request->query);
        if (root)
        {
            cJSON *id = cJSON_GetObjectItem(root, "id");
            cJSON *code = cJSON_GetObjectItem(root, "code");
            body = board_control(id->valuestring, code->valueint);
            cJSON_Delete(root);
        }
        else
        {
            body = json_create_web_response(-1, "command error");
        }
    }
    else
    {
        body = json_create_web_response(-1, "no command");
    }

    webnet_session_printf(session, body);
    rt_free(body);
}

int basic_init(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(BLUE_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(RED_LED, PIN_MODE_OUTPUT);
    rt_pin_write(BLUE_LED, PIN_HIGH);
    rt_pin_write(RED_LED, PIN_HIGH);

    webnet_cgi_register("basic_info", cgi_basic_info);
    webnet_cgi_register("board_control", cgi_board_control);
    return 0;
}
