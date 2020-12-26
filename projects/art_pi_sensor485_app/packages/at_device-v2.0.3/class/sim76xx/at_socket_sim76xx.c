/*
 * File      : at_socket_sim76xx.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-22    thomasonegd  first version
 * 2019-03-06    thomasonegd  fix udp connection.
 * 2019-03-08    thomasonegd  add power_on & power_off api
 * 2019-05-14    chenyong     multi AT socket client support
 * 2019-08-24    chenyong     add netdev support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_sim76xx.h>

#define LOG_TAG                        "at.skt.sim76"
#include <at_log.h>

#ifdef AT_DEVICE_USING_SIM76XX

#define SIM76XX_MODULE_SEND_MAX_SIZE   1500
#define SIM76XX_MAX_CONNECTIONS        10
#define SIM76XX_IPADDR_LEN             16

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define SIM76XX_EVENT_CONN_OK          (1L << 0)
#define SIM76XX_EVENT_SEND_OK          (1L << 1)
#define SIM76XX_EVENT_RECV_OK          (1L << 2)
#define SIM76XX_EVNET_CLOSE_OK         (1L << 3)
#define SIM76XX_EVENT_CONN_FAIL        (1L << 4)
#define SIM76XX_EVENT_SEND_FAIL        (1L << 5)

static at_evt_cb_t at_evt_cb_set[] =
{
    [AT_SOCKET_EVT_RECV] = NULL,
    [AT_SOCKET_EVT_CLOSED] = NULL,
};

static char udp_ipstr[SIM76XX_MAX_CONNECTIONS][SIM76XX_IPADDR_LEN] = {0};
static int udp_port[SIM76XX_MAX_CONNECTIONS] = {0};

/* unsolicited TCP/IP command<err> codes */
static void at_tcp_ip_errcode_parse(int result)
{
    switch(result)
    {
    case 0 : LOG_D("%d : operation succeeded ",             result); break;
    case 1 : LOG_E("%d : UNetwork failure",                 result); break;
    case 2 : LOG_E("%d : Network not opened",               result); break;
    case 3 : LOG_E("%d : Wrong parameter",                  result); break;
    case 4 : LOG_D("%d : Operation not supported",          result); break;
    case 5 : LOG_E("%d : Failed to create socket",          result); break;
    case 6 : LOG_E("%d : Failed to bind socket",            result); break;
    case 7 : LOG_E("%d : TCP server is already listening",  result); break;
    case 8 : LOG_E("%d : Busy",                             result); break;
    case 9 : LOG_E("%d : Sockets opened",                   result); break;
    case 10 : LOG_E("%d : Timeout ",                        result); break;
    case 11 : LOG_E("%d : DNS parse failed for AT+CIPOPEN", result); break;
    case 255 : LOG_E("%d : Unknown error",                  result); break;
    }
}

static int sim76xx_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int)rt_event_send(device->socket_event, event);
}

static int sim76xx_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
{
    int result = RT_EOK;
    rt_uint32_t recved;

    result = rt_event_recv(device->socket_event, event, option | RT_EVENT_FLAG_CLEAR, timeout, &recved);
    if (result != RT_EOK)
    {
        return -RT_ETIMEOUT;
    }

    return recved;
}

/**
 * close socket by AT commands.
 *
 * @param current socket
 *
 * @return  0: close socket success
 *         -1: send AT commands error
 *         -2: wait socket event timeout
 *         -5: no memory
 */
static int sim76xx_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    int lnk_stat[10] = {0};
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(64, 0, RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_thread_mdelay(100);

    /* check socket link_state */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE?") < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+CIPCLOSE:", "+CIPCLOSE: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                                      &lnk_stat[0], &lnk_stat[1], &lnk_stat[2], &lnk_stat[3], &lnk_stat[4],
                                      &lnk_stat[5], &lnk_stat[6], &lnk_stat[7], &lnk_stat[8], &lnk_stat[9]) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (lnk_stat[device_socket])
    {
        #define CLOSE_COUNTS   5
        int i = 0;

        /* close tcp or udp socket if connected */
        if (at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE=%d", device_socket) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* wait sim76xx device sockt closed */
        for (i = 0; i < CLOSE_COUNTS; i++)
        {
            if (at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE?") < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }

            at_resp_parse_line_args_by_kw(resp, "+CIPCLOSE:", "+CIPCLOSE: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                                            &lnk_stat[0], &lnk_stat[1], &lnk_stat[2], &lnk_stat[3], &lnk_stat[4],
                                            &lnk_stat[5], &lnk_stat[6], &lnk_stat[7], &lnk_stat[8], &lnk_stat[9]);
            if (lnk_stat[device_socket] == 0)
            {
                break;
            }
            rt_thread_delay(1000);
        }
    }

 __exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

/**
 * create TCP/UDP client or server connect by AT commands.
 *
 * @param socket current socket
 * @param ip server or client IP address
 * @param port server or client port
 * @param type connect socket type(tcp, udp)
 * @param is_client connection is client
 *
 * @return   0: connect success
 *          -1: connect failed, send commands error or type error
 *          -2: wait socket event timeout
 *          -5: no memory
 */
static int sim76xx_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    int result = RT_EOK, event_result = 0;
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

	rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* check and close current socket */
    sim76xx_socket_close(socket);

__retry:
    if (is_client)
    {
        switch (type)
        {
        case AT_SOCKET_TCP:
            /* send AT commands to connect TCP server */
            if (at_obj_exec_cmd(device->client, resp, "AT+CIPOPEN=%d,\"TCP\",\"%s\",%d", device_socket, ip, port) < 0)
            {
                result = -RT_ERROR;
            }
            break;

        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, resp, "AT+CIPOPEN=%d,\"UDP\",,,%d", device_socket, port) < 0)
            {
                result = -RT_ERROR;
            }
            rt_strncpy(udp_ipstr[device_socket], ip, SIM76XX_IPADDR_LEN);
            udp_port[device_socket] = port;
            break;

        default:
            LOG_E("not supported connect type : %d.", type);
            result = -RT_ERROR;
            goto __exit;
        }
    }

    /* waiting result event from AT URC, the device default connection timeout is 75 seconds, but it set to 10 seconds is convenient to use.*/
    if (sim76xx_socket_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
    {
        LOG_E("%s device socket(%d) wait connect result timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* waiting OK or failed result */
    event_result = sim76xx_socket_event_recv(device, SIM76XX_EVENT_CONN_OK | SIM76XX_EVENT_CONN_FAIL,
                                        1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
    if (event_result < 0)
    {
        LOG_E("%s device socket(%d) wait connect OK|FAIL timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* check result */
    if (event_result & SIM76XX_EVENT_CONN_FAIL)
    {
        if (retryed == RT_FALSE)
        {
            LOG_D("socket(%d) connect failed, the socket was not be closed and now will connect retry.", device_socket);
            if (sim76xx_socket_close(socket) < 0)
            {
			    result = -RT_ERROR;
                goto __exit;
            }
            retryed = RT_TRUE;
            goto __retry;
        }
        LOG_E("%s device socket(%d) connect failed.", device->name, device_socket);
        result = -RT_ERROR;
        goto __exit;
    }

__exit:
    rt_mutex_release(lock);

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

/**
 * send data to server or client by AT commands.
 *
 * @param socket current socket
 * @param buff send buffer
 * @param bfsz send buffer size
 * @param type connect socket type(tcp, udp)
 *
 * @return >=0: the size of send success
 *          -1: send AT commands error or send data error
 *          -2: waited socket event timeout
 *          -5: no memory
 */
static int sim76xx_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    int result = RT_EOK;
    int event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    struct at_device_sim76xx *sim76xx = (struct at_device_sim76xx *) device->user_data;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);
    RT_ASSERT(bfsz > 0);

    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* set current socket for send URC event */
    sim76xx->user_data = (void *) device_socket;
    /* set AT client end sign to deal with '>' sign.*/
    at_obj_set_end_sign(device->client, '>');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < SIM76XX_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = SIM76XX_MODULE_SEND_MAX_SIZE;
        }

        switch (socket->type)
        {
        case AT_SOCKET_TCP:
            /* send the "AT+CIPSEND" commands to AT server than receive the '>' response on the first line. */
            if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,%d", device_socket, cur_pkt_size) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;
        case AT_SOCKET_UDP:
            /* send the "AT+CIPSEND" commands to AT server than receive the '>' response on the first line. */
            if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,%d,\"%s\",%d",
                                device_socket, cur_pkt_size, udp_ipstr[device_socket], udp_port[device_socket]) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;
        default:
            LOG_E("input socket type(%d) error.", socket->type);
            break;
        }

        /* send the real data to server or client */
        result = (int) at_client_send(buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting result event from AT URC */
        if (sim76xx_socket_event_recv(device, SET_EVENT(device_socket, 0), 5 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("%s device socket (%d) wait send result timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = sim76xx_socket_event_recv(device, SIM76XX_EVENT_SEND_OK | SIM76XX_EVENT_SEND_FAIL,
                                                 5 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            LOG_E("%s device socket(%d) wait send OK|FAIL timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* check result */
        if (event_result & SIM76XX_EVENT_SEND_FAIL)
        {
            LOG_E("%s device socket(%d) send failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        sent_size += cur_pkt_size;
    }

__exit:
    /* reset the end sign for data */
    at_obj_set_end_sign(device->client, 0);

    rt_mutex_release(lock);

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result > 0 ? sent_size : result;
}

/**
 * domain resolve by AT commands.
 *
 * @param name domain name
 * @param ip parsed IP address, it's length must be 16
 *
 * @return  0: domain resolve success
 *         -2: wait socket event timeout
 *         -5: no memory
 */
static int sim76xx_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY        5

    int i, result = RT_EOK;
    char domain[32] = {0};
    char domain_ip[16] = {0};
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(name);
    RT_ASSERT(ip);

    device = at_device_get_first_initialized();
    if (device == RT_NULL)
    {
        LOG_E("get first init device failed.");
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    for (i = 0; i < RESOLVE_RETRY; i++)
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+CDNSGIP=\"%s\"", name) < 0)
        {
            rt_thread_mdelay(200);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }

        /* parse the third line of response data, get the IP address */
        /* +CDNSGIP: 1,"www.baidu.com","14.215.177.39" */
        if (at_resp_parse_line_args_by_kw(resp, "+CDNSGIP:", "+CDNSGIP: 1,%[^,],\"%[^\"]", domain, domain_ip) < 0)
        {
            rt_thread_mdelay(200);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }

        if (rt_strlen(domain_ip) < 8)
        {
            rt_thread_mdelay(200);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }
        else
        {
            rt_strncpy(ip, domain_ip, 15);
            ip[15] = '\0';
            break;
        }
    }

    if (i == RESOLVE_RETRY)
    {
        result = -RT_ERROR;
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

/**
 * set AT socket event notice callback
 *
 * @param event notice event
 * @param cb notice callback
 */
static void sim76xx_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < (sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1])))
    {
        at_evt_cb_set[event] = cb;
    }
}

static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0, rqst_size, cnf_size;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.");
        return;
    }

    sscanf(data, "+CIPSEND: %d,%d,%d", &device_socket, &rqst_size, &cnf_size);
    sim76xx_socket_event_send(device, SET_EVENT(device_socket, SIM76XX_EVENT_SEND_OK));
}

static void urc_connect_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0, result = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    sscanf(data, "+CIPOPEN: %d,%d", &device_socket, &result);

    if (result == 0)
    {
        sim76xx_socket_event_send(device, SET_EVENT(device_socket, SIM76XX_EVENT_CONN_OK));
    }
    else
    {
        at_tcp_ip_errcode_parse(result);
        sim76xx_socket_event_send(device, SET_EVENT(device_socket, SIM76XX_EVENT_CONN_FAIL));
    }
}

static void urc_close_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0, reason = 0;
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", device->name);
        return;
    }

    sscanf(data, "+IPCLOSE %d,%d", &device_socket, &reason);
    /* get AT socket object by device socket descriptor */
    socket = &(device->sockets[device_socket]);

    /* notice the socket is disconnect by remote */
    if (at_evt_cb_set[AT_SOCKET_EVT_CLOSED])
    {
        at_evt_cb_set[AT_SOCKET_EVT_CLOSED](socket, AT_SOCKET_EVT_CLOSED, RT_NULL, 0);
    }
}

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    rt_size_t bfsz = 0, temp_size = 0;
    rt_int32_t timeout;
    char *recv_buf = RT_NULL, temp[8] = {0};
    int device_socket = 0;
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    struct at_device_sim76xx *sim76xx = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }
    sim76xx = (struct at_device_sim76xx *) device->user_data;
    device_socket = (int) sim76xx->user_data;

    /* get the current socket and receive buffer size by receive data */
    sscanf(data, "+IPD%d:", (int *)&bfsz);
    /* get receive timeout by receive buffer length */
    timeout = bfsz * 10;

    if (bfsz == 0)
        return;

    recv_buf = (char *) rt_calloc(1, bfsz);
    if (recv_buf == RT_NULL)
    {
        LOG_E("no memory for receive buffer(%d).", bfsz);
        /* read and clean the coming data */
        while (temp_size < bfsz)
        {
            if (bfsz - temp_size > sizeof(temp))


            {
                at_client_obj_recv(client, temp, sizeof(temp), timeout);
            }
            else
            {
                at_client_obj_recv(client, temp, bfsz - temp_size, timeout);
            }
            temp_size += sizeof(temp);
        }
        return;
    }

    /* sync receive data */
    if (at_client_obj_recv(client, recv_buf, bfsz, timeout) != bfsz)
    {
        LOG_E("%s device receive size(%d) data failed.", device->name, bfsz);
        rt_free(recv_buf);
        return;
    }

    /* get AT socket object by device socket descriptor */
    socket = &(device->sockets[device_socket]);

    /* notice the receive buffer and buffer size */
    if (at_evt_cb_set[AT_SOCKET_EVT_RECV])
    {
        at_evt_cb_set[AT_SOCKET_EVT_RECV](socket, AT_SOCKET_EVT_RECV, recv_buf, bfsz);
    }
}

static struct at_urc urc_table[] =
{
    {"+CIPSEND:",      "\r\n",           urc_send_func},
    {"+CIPOPEN:",      "\r\n",           urc_connect_func},
    {"+IPCLOSE",       "\r\n",           urc_close_func},
    {"+IPD",           "\r\n",           urc_recv_func},
};

static const struct at_socket_ops sim76xx_socket_ops =
{
    sim76xx_socket_connect,
    sim76xx_socket_close,
    sim76xx_socket_send,
    sim76xx_domain_resolve,
    sim76xx_socket_set_event_cb,
};

/* initialize sim76xx device network URC feature */
int sim76xx_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

/* resgiter sim76xx device socket operations */
int sim76xx_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_SIM76XX_SOCKETS_NUM;
    class->socket_ops = &sim76xx_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_SIM76XX */
