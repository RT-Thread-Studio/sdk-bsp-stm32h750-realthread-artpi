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
#include "mesh_command.h"
#include "mesh_node.h"
#include "hex_utils.h"

#define FIRMWARE_VERSION "V1.0.0"
#define BLUE_LED_DEV_NAME "dev0"
#define RED_LED_DEV_NAME "dev1"
#define BEEP_DEV_NAME "dev2"

#define NORFLASH_PATH "/flash"
#define SD_PATH "/sdcard"

#define BLUE_LED GET_PIN(I, 8)
#define RED_LED GET_PIN(C, 15)

extern void mesh_send_command(command_opcode_t opc, uint16_t addr, uint8_t *data);

static char *board_control(uint16_t addr, command_opcode_t code, uint8_t *data)
{
    mesh_send_command(code, addr, data);

    return json_create_web_response(0, "ok");
}

static char *json_create_basic_info(void)
{
    //ram usage
    cJSON *root = mesh_nodes_dump();

    char *json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_data;
}

static void cgi_basic_info(struct webnet_session *session)
{
    cgi_head();
    request = request; //clear warning
    body = json_create_basic_info();
    webnet_session_printf(session, body);
    rt_free(body);
}

static void cgi_board_control(struct webnet_session *session)
{
    cgi_head();
    request = request; //clear warning
    if (session->request->query)
    {
        cJSON *root = cJSON_Parse(session->request->query);
        if (root)
        {
            cJSON *addr = cJSON_GetObjectItem(root, "address");
            cJSON *opcode = cJSON_GetObjectItem(root, "opcode");

            if (opcode->valueint == COMMAND_OPCODE_SET)
            {
                cJSON *data = cJSON_GetObjectItem(root, "data");

                uint8_t len = strlen(data->valuestring);
                uint8_t buf[len / 2];

                for (uint8_t i = 0; i < len / 2; i++)
                {
                    buf[i] = hex_to_uint8(&data->valuestring[i * 2]);
                }

                body = board_control((uint16_t)addr->valueint, (uint8_t)opcode->valueint, buf);
            }
            else
            {
                body = board_control((uint16_t)addr->valueint, (uint8_t)opcode->valueint, NULL);
            }

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
