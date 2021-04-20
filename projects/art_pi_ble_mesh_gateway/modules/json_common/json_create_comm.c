/*************************************************
 Copyright (c) 2020
 All rights reserved.
 File name:     json_create_comm.c
 Description:   
 History:
 1. Version:    
    Date:       2020-09-08
    Author:     wangjunjie
    Modify:     
*************************************************/
#include <string.h>
#include <cJSON.h>


//创建网页响应
char *json_create_web_response(int code, const char *msg)
{
    char *json_data = NULL;
    cJSON *root = cJSON_CreateObject();

    if (msg == NULL)
    {
        msg = " ";
    }

    cJSON_AddItemToObject(root, "code", cJSON_CreateNumber(code));
    cJSON_AddItemToObject(root, "msg", cJSON_CreateString(msg));

    json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_data;
}

