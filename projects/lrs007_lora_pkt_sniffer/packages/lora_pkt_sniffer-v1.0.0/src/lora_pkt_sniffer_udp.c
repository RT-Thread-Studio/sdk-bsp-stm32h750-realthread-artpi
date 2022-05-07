/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date             Author      Notes
 *                  Forest-Rain
 */
#include <rtthread.h>
#include <sys/socket.h> /* BSD socket*/
#include <netdb.h>
#include <string.h>
#include <finsh.h>
#include "lora_pkt_sniffer.h"

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LPS.UDP"
#include "lora_pkt_sniffer_dbg.h"

#if defined LORA_PKT_SNIFFER_USING_WIRESHARK_MODE

struct rt_semaphore lps_udp_up_sem;
struct rt_thread lps_udp_up_thread;
rt_uint32_t lps_udp_up_stack[4096];

struct rt_messagequeue lps_udp_up_mq;
rt_uint8_t lps_udp_up_msg_pool[2048];

int sock_udp = -1;
struct hostent *host;
struct sockaddr_in server_addr_udp;

int lps_udp_send(uint8_t *payload, uint16_t size)
{
    if(sock_udp != -1 )
    {
        // send by UDP
        sendto(sock_udp, payload, size, 0,
               (struct sockaddr *)&server_addr_udp, sizeof(struct sockaddr));
        LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "\r\nSocket sendto:%d\r\n", size);
        return RT_EOK;
    }
    else
    {
        LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "Socket Not Created\r\n");
        return -RT_ERROR;
    }
}

int lps_udp_client_init(void)
{
    int port;
    const char *url;

    url = lps_conf_srv.server_addr;
    port = atoi(lps_conf_srv.server_port_up);

    /* get url host address（DNS for domain name） */
    host = (struct hostent *) gethostbyname(url);

    /* creat a socket，SOCK_DGRAM，UDP */
    if ((sock_udp = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "Socket error\n");
        return -RT_ERROR;
    }

    /* server address */
    server_addr_udp.sin_family = AF_INET;
    server_addr_udp.sin_port = htons(port);
    server_addr_udp.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr_udp.sin_zero), 0, sizeof(server_addr_udp.sin_zero));

    LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "udpclient socket(%d) creat sucess", sock_udp);
    
    return RT_EOK;
}

static void lps_udp_up_thread_entry(void* parameter)
{
    struct lps_msg_s msg_ptr;

    while(1)
    {
        if (rt_mq_recv(&lps_udp_up_mq, (void*)&msg_ptr, sizeof(struct lps_msg_s), RT_WAITING_FOREVER) == RT_EOK)
        {
            lps_udp_send(msg_ptr.data_ptr, msg_ptr.data_size);
            LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "UDP Send:%d\n", msg_ptr.data_size);
        }
    }
}

int lps_udp_up_thread_init(void)
{
    rt_err_t result;

    result = rt_mq_init(&lps_udp_up_mq, "lps_udp_mq", &lps_udp_up_msg_pool[0],
                        sizeof(struct lps_msg_s),
                        sizeof(lps_udp_up_msg_pool),
                        RT_IPC_FLAG_FIFO);

    if (result != RT_EOK)
    {
        LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "init message queue failed.\n");
    }

    result = rt_thread_init(&lps_udp_up_thread,
                            "lps-udp-up",
                            lps_udp_up_thread_entry,
                            RT_NULL,
                            &lps_udp_up_stack[0],
                            sizeof(lps_udp_up_stack),
                            7,
                            10);

    if (result == RT_EOK)
    {
        rt_thread_startup(&lps_udp_up_thread);
    }
    else
    {
        LPS_DEBUG_LOG(LPS_DBG_UDP, DBG_LVL, "init static lps_udp_up_thread thread failed.\n");
    }

    result = lps_udp_client_init();

    return result;
}

#endif
