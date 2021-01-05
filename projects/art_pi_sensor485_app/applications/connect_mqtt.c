#include <connect_mqtt.h>
/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-12-10     liuda123.club
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rtthread.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME    "mqtt.sample"
#define DBG_LEVEL           DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include "paho_mqtt.h"

/* 设置代理信息 */
#define MQTT_URI                "tcp://115.159.217.166:1883"  //这里可以改成你自己的mqtt服务器地址
#define MQTT_USERNAME           "liuda123.club"               //用户名随便定义，我改为 liuda123.club是我博客地址
#define MQTT_PASSWORD           ""
#define MQTT_SUBTOPIC           "sub_test"
#define MQTT_PUBTOPIC_H         "sensor/humidity"
#define MQTT_PUBTOPIC_T         "sensor/temper"
#define MQTT_WILLMSG            "Goodbye!"

static MQTTClient client;
static int is_started = 0;



extern char tem[10];
extern char hum[10];
extern char aa[10];

static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub callback: %.*s %.*s",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
}

void mqtt_emqx_entry(void *parameter)
{
    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
    static char cid[20] = { 0 };
    /* config MQTT context param*/
    client.isconnected = 0;
    client.uri = MQTT_URI;

    /* generate the random client ID */
    rt_snprintf(cid, sizeof(cid), "rtthread%d", rt_tick_get());
    /* config connect param */
    memcpy(&client.condata, &condata, sizeof(condata));
    client.condata.clientID.cstring = cid;
    client.condata.keepAliveInterval = 30;
    client.condata.cleansession = 1;
    client.condata.username.cstring = MQTT_USERNAME;
    client.condata.password.cstring = MQTT_PASSWORD;

    /* config MQTT will param. */
    client.condata.willFlag = 1;
    client.condata.will.qos = 1;
    client.condata.will.retained = 0;
    client.condata.will.topicName.cstring = MQTT_PUBTOPIC_T;
    client.condata.will.message.cstring = MQTT_WILLMSG;

    /* malloc buffer. */
    client.buf_size = client.readbuf_size = 1024;
    client.buf = rt_calloc(1, client.buf_size);
    client.readbuf = rt_calloc(1, client.readbuf_size);
    if (!(client.buf && client.readbuf))
    {
       LOG_E("no memory for MQTT client buffer!");
       return;
    }

    /* set subscribe table and event callback */
    client.messageHandlers[0].topicFilter = rt_strdup(MQTT_SUBTOPIC);
    client.messageHandlers[0].callback = mqtt_sub_callback;
    client.messageHandlers[0].qos = QOS1;

    paho_mqtt_start(&client);
    is_started = 1;


char one[5]={"11"};
    while(1)
    {
        if (is_started == 0)
        {
           LOG_E("mqtt client is not connected.");
           return;
        }
        else
        {
            //温度数发送到服务器
           paho_mqtt_publish(&client, QOS1, MQTT_PUBTOPIC_T,hum);
           rt_thread_mdelay(600);

           //湿度数发送到服务器
           paho_mqtt_publish(&client, QOS1, MQTT_PUBTOPIC_H,tem);
           rt_thread_mdelay(600);
        }
        rt_thread_mdelay(10000);
    }
}

/* 创建线程 */
int mqtt_emqx(void)
{
    rt_thread_t tid;    //线程句柄

    tid = rt_thread_create("mqtt_emqx",
                            mqtt_emqx_entry,
                            RT_NULL,
                            512,
                            9,
                            10);
   if(tid != RT_NULL)
   {
        //线程创建成功，启动线程
        rt_thread_startup(tid);
   }

   return 0;
}

MSH_CMD_EXPORT(mqtt_emqx, mqtt emqx test);

