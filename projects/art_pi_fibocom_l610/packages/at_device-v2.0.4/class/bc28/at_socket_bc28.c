/*
 * File      : at_socket_bc28.c
 * This file is part of RT-Thread RTOS
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
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
 * Date           Author            Notes
 * 2020-02-13     luhuadong         first version
 * 2020-07-19     luhuadong         support alloc socket
 */

#include <stdio.h>
#include <string.h>
#include <at_device_bc28.h>

#if !defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10301
#error "This AT Client version is older, please check and update latest AT Client!"
#endif

#define LOG_TAG                        "at.skt.bc28"
#include <at_log.h>

#if defined(AT_DEVICE_USING_BC28) && defined(AT_USING_SOCKET)

#define BC28_MODULE_SEND_MAX_SIZE       1358
#define BC28_MODULE_RECV_MAX_SIZE       1358

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define BC28_EVENT_CONN_OK             (1L << 0)
#define BC28_EVENT_SEND_OK             (1L << 1)
#define BC28_EVENT_RECV_OK             (1L << 2)
#define BC28_EVNET_CLOSE_OK            (1L << 3)
#define BC28_EVENT_CONN_FAIL           (1L << 4)
#define BC28_EVENT_SEND_FAIL           (1L << 5)
#define BC28_EVENT_DOMAIN_OK           (1L << 6)
#define BC28_EVENT_DOMAIN_FAIL         (1L << 7)

static at_evt_cb_t at_evt_cb_set[] = {

        [AT_SOCKET_EVT_RECV]   = NULL,
        [AT_SOCKET_EVT_CLOSED] = NULL,
};

static struct at_socket_ip_info
{
    char ip_addr[IP_ADDR_SIZE_MAX];
    int  port;
} bc28_sock_info[AT_DEVICE_BC28_SOCKETS_NUM];

/**
 * convert data from ASCII string to Hex string.
 *
 * @param str  input ASCII string
 * @param hex  output Hex string
 * @param len  length of str, or the size you want to convert
 *
 * @return =0: convert failed, or no data need to convert
 *         >0: the size of convert success
 */
static int string_to_hex(const char *str, char *hex, const rt_size_t len)
{
    RT_ASSERT(str && hex);

    int str_len = rt_strlen(str);
    int pos = 0, i;

    if (len < 1 || str_len < len)
    {
        return 0;
    }

    for (i = 0; i < len; i++, pos += 2)
    {
        rt_sprintf(&hex[pos], "%02X", str[i]);
    }

    return i;
}

/**
 * convert data from Hex string to ASCII string.
 *
 * @param hex  input Hex string
 * @param str  output ASCII string
 * @param len  length of str, or the size you want to convert
 *
 * @return =0: convert failed, or no data need to convert
 *         >0: the size of convert success
 */
static int hex_to_string(const char *hex, char *str, const rt_size_t len)
{
    RT_ASSERT(hex && str);

    int hex_len = rt_strlen(hex);
    int pos = 0, left, right, i;

    if (len < 1 || hex_len/2 < len)
    {
        return 0;
    }

    for (i = 0; i < len*2; i++, pos++)
    {
        left = hex[i++];
        right = hex[i];

        left  = (left  < 58) ? (left  - 48) : (left  - 55);
        right = (right < 58) ? (right - 48) : (right - 55);

        str[pos] = (left << 4) | right;
    }

    return pos;
}

static void at_tcp_ip_errcode_parse(int result)//TCP/IP_QIGETERROR
{
    switch(result)
    {
    case 0   : LOG_D("%d : Operation successful",         result); break;
    case 550 : LOG_E("%d : Unknown error",                result); break;
    case 551 : LOG_E("%d : Operation blocked",            result); break;
    case 552 : LOG_E("%d : Invalid parameters",           result); break;
    case 553 : LOG_E("%d : Memory not enough",            result); break;
    case 554 : LOG_E("%d : Create socket failed",         result); break;
    case 555 : LOG_E("%d : Operation not supported",      result); break;
    case 556 : LOG_E("%d : Socket bind failed",           result); break;
    case 557 : LOG_E("%d : Socket listen failed",         result); break;
    case 558 : LOG_E("%d : Socket write failed",          result); break;
    case 559 : LOG_E("%d : Socket read failed",           result); break;
    case 560 : LOG_E("%d : Socket accept failed",         result); break;
    case 561 : LOG_E("%d : Open PDP context failed",      result); break;
    case 562 : LOG_E("%d : Close PDP context failed",     result); break;
    case 563 : LOG_W("%d : Socket identity has been used", result); break;
    case 564 : LOG_E("%d : DNS busy",                     result); break;
    case 565 : LOG_E("%d : DNS parse failed",             result); break;
    case 566 : LOG_E("%d : Socket connect failed",        result); break;
    // case 567 : LOG_W("%d : Socket has been closed",       result); break;
    case 567 : break;
    case 568 : LOG_E("%d : Operation busy",               result); break;
    case 569 : LOG_E("%d : Operation timeout",            result); break;
    case 570 : LOG_E("%d : PDP context broken down",      result); break;
    case 571 : LOG_E("%d : Cancel send",                  result); break;
    case 572 : LOG_E("%d : Operation not allowed",        result); break;
    case 573 : LOG_E("%d : APN not configured",           result); break;
    case 574 : LOG_E("%d : Port busy",                    result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}

static int bc28_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int) rt_event_send(device->socket_event, event);
}

static int bc28_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
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
static int bc28_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(3000));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    result = at_obj_exec_cmd(device->client, resp, "AT+NSOCL=%d", device_socket);
    if (result < 0)
    {
        LOG_E("%s device close socket(%d) failed [%d].", device->name, device_socket, result);
    }
    else
    {
        LOG_D("%s device close socket(%d).", device->name, device_socket);
    }

    at_delete_resp(resp);

    return result;
}

/**
 * create socket by AT commands.
 *
 * @param type connect socket type(tcp, udp)
 *
 * @return >=0: create socket success, return the socket id (0-6)
 *          -1: send or exec AT commands error
 *          -5: no memory
 */
static int bc28_socket_create(struct at_device *device, enum at_socket_type type)
{
    const char *type_str = RT_NULL;
    uint32_t protocol = 0;
    uint32_t port = 0; /* range: 0-65535, if 0 means get a random port */
    at_response_t resp = RT_NULL;
    int socket = -1, result = 0;

    switch(type)
    {
        case AT_SOCKET_TCP:
            type_str = "STREAM";
            protocol = 6;
            break;
        case AT_SOCKET_UDP:
            type_str = "DGRAM";
            protocol = 17;
            break;
        default:
            return -RT_ERROR;
    }

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* create socket */
    if (at_obj_exec_cmd(device->client, resp, "AT+NSOCR=%s,%d,%d,1", type_str, protocol, port) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    /* check socket */
    if (at_resp_parse_line_args(resp, 2, "%d", &socket) <= 0)
    {
        LOG_E("%s device create %s socket failed.", device->name, type_str);
        result = -RT_ERROR;
        goto __exit;
    }
    else
    {
        LOG_D("%s device create a %s socket(%d).", device->name, type_str, socket);
        result = socket;
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
static int bc28_socket_connect(struct at_socket *socket, char *ip, int32_t port,
                               enum at_socket_type type, rt_bool_t is_client)
{
#define CONN_RETRY  3

    int i = 0;
    uint32_t event = 0;
    at_response_t resp = RT_NULL;
    int result = 0, event_result = 0;
    int device_socket = (int) socket->user_data;
    int return_socket = -1;
    struct at_device *device = (struct at_device *) socket->device;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    if (!is_client)
    {
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* if the protocol is not tcp, no need connect to server */
    if (type != AT_SOCKET_TCP)
    {
        if (type == AT_SOCKET_UDP)
        {
            rt_strncpy(bc28_sock_info[device_socket].ip_addr, ip, IP_ADDR_SIZE_MAX);
            bc28_sock_info[device_socket].port = port;
        }
        return RT_EOK;
    }

    for(i=0; i<CONN_RETRY; i++)
    {
        /* clear socket connect event */
        event = SET_EVENT(device_socket, BC28_EVENT_CONN_OK | BC28_EVENT_CONN_FAIL);
        bc28_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

        if (at_obj_exec_cmd(device->client, resp, "AT+NSOCO=%d,%s,%d", device_socket, ip, port) < 0)
        {
            result = -RT_ERROR;
            continue;
        }
        LOG_D("%s device socket(%d) try connect to %s:%d.", device->name, device_socket, ip, port);

        if(!at_resp_get_line_by_kw(resp, "OK"))
        {
            result = -RT_ERROR;
            continue;
        }

        /* waiting result event from AT URC, the device default connection timeout is 30 seconds*/
        if (bc28_socket_event_recv(device, SET_EVENT(device_socket, 0),
                                   30 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_D("%s device socket(%d) wait connect result timeout.", device->name, device_socket);
            /* No news is good news */
            result = RT_EOK;
            break;
        }

        /* waiting OK or failed result */
        event_result = bc28_socket_event_recv(device, BC28_EVENT_CONN_OK | BC28_EVENT_CONN_FAIL,
                                              1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result & BC28_EVENT_CONN_FAIL)
        {
            LOG_E("%s device socket(%d) connect failed.", device->name, device_socket);
            result = -RT_ERROR;
            continue;
        }
        else
        {
            result = RT_EOK;
            break;
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
static int bc28_socket_send(struct at_socket *socket, const char *buff,
                            size_t bfsz, enum at_socket_type type)
{
    uint32_t event = 0;
    int result = 0, event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    struct at_device_bc28 *bc28 = (struct at_device_bc28 *) device->user_data;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* clear socket send event */
    event = SET_EVENT(device_socket, BC28_EVENT_SEND_OK | BC28_EVENT_SEND_FAIL);
    bc28_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    /* only use for UDP socket */
    const char *ip = bc28_sock_info[device_socket].ip_addr;
    const int port = bc28_sock_info[device_socket].port;

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < BC28_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = BC28_MODULE_SEND_MAX_SIZE;
        }

        size_t i = 0, ind = 0;
        char hex_data[cur_pkt_size * 2 + 1];
        rt_memset(hex_data, 0, sizeof(hex_data));

        for (i=0, ind=0; i<cur_pkt_size; i++, ind+=2)
        {
            sprintf(&hex_data[ind], "%02X", buff[sent_size + i]);
        }

        switch (type)
        {
        case AT_SOCKET_TCP:
            /* AT+NSOSD=<socket>,<length>,<data>[,<flag>[,<sequence>]] */
            if (at_obj_exec_cmd(device->client, resp, "AT+NSOSD=%d,%d,%s,0x100,1", device_socket,
                                (int)cur_pkt_size, hex_data) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            LOG_D("%s device tcp socket(%d) send %d bytes.\n>> %s", device->name, device_socket, (int)cur_pkt_size, hex_data);
            break;

        case AT_SOCKET_UDP:
            /* AT+NSOST=<socket>,<remote_addr>,<remote_port>,<length>,<data>[,<sequence>] */
            if (at_obj_exec_cmd(device->client, resp, "AT+NSOST=%d,%s,%d,%d,%s,1", device_socket,
                                ip, port, (int)cur_pkt_size, hex_data) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            LOG_D("%s device udp socket(%d) send %d bytes to %s:%d.\n>> %s", device->name, device_socket, ip, port, (int)cur_pkt_size, hex_data);
            break;

        default:
            LOG_E("not supported send type %d.", type);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check if sent ok */
        if (!at_resp_get_line_by_kw(resp, "OK"))
        {
            LOG_E("%s device socket(%d) send data failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        int return_socket = -1, return_size = -1;

        if (at_resp_parse_line_args(resp, 2, "%d,%d", &return_socket, &return_size) <= 0)
        {
            LOG_E("%s device socket(%d) send data failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        if (return_socket != device_socket || return_size != cur_pkt_size)
        {
            LOG_E("%s device socket(%d) send data incompletely.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting result event from AT URC, the device default timeout is 60 seconds*/
        if (bc28_socket_event_recv(device, SET_EVENT(device_socket, 0),
                                   60 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("%s device socket(%d) wait send result timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = bc28_socket_event_recv(device, BC28_EVENT_SEND_OK | BC28_EVENT_SEND_FAIL,
                                              1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result & BC28_EVENT_SEND_FAIL)
        {
            LOG_E("%s device socket(%d) send failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }
        else
        {
            LOG_D("%s device socket(%d) send success.", device->name, device_socket);
            sent_size += cur_pkt_size;
            result = sent_size;
        }
    }

__exit:
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
 *         -1: send AT commands error or response error
 *         -2: wait socket event timeout
 *         -5: no memory
 */
int bc28_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY  1

    int i, result, event_result = 0;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    struct at_device_bc28 *bc28 = RT_NULL;

    RT_ASSERT(name);
    RT_ASSERT(ip);

    device = at_device_get_first_initialized();
    if (device == RT_NULL)
    {
        LOG_E("get first init device failed.");
        return -RT_ERROR;
    }

    /* the maximum response time is 60 seconds, but it set to 10 seconds is convenient to use. */
    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (!resp)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* clear BC28_EVENT_DOMAIN_OK */
    bc28_socket_event_recv(device, BC28_EVENT_DOMAIN_OK, 0, RT_EVENT_FLAG_OR);

    bc28 = (struct at_device_bc28 *) device->user_data;
    bc28->socket_data = ip;

    if (at_obj_exec_cmd(device->client, resp, "AT+QDNS=0,%s", name) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    for(i = 0; i < RESOLVE_RETRY; i++)
    {
        /* waiting result event from AT URC, the device default connection timeout is 30 seconds.*/
        event_result = bc28_socket_event_recv(device, BC28_EVENT_DOMAIN_OK | BC28_EVENT_DOMAIN_FAIL,
                                              30 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            result = -RT_ETIMEOUT;
            continue;
        }
        else if (event_result & BC28_EVENT_DOMAIN_FAIL)
        {
            LOG_E("%d device resolve domain name failed.", device->name);
            result = -RT_ERROR;
            continue;
        }
        else
        {
            result = RT_EOK;
            break;
        }
    }

 __exit:
    bc28->socket_data = RT_NULL;
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
static void bc28_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
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

    /* only for firmware version base BC28JAR02xxx */
    sscanf(data, "+QTCPIND: %d,%d", &device_socket , &result);

    if (result == 0)
    {
        bc28_socket_event_send(device, SET_EVENT(device_socket, BC28_EVENT_CONN_OK));
    }
    else
    {
        at_tcp_ip_errcode_parse(result);
        bc28_socket_event_send(device, SET_EVENT(device_socket, BC28_EVENT_CONN_FAIL));
    }
}

static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0, sequence = 0, status = 0;
    struct at_device *device = RT_NULL;
    struct at_device_bc28 *bc28 = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    sscanf(data, "+NSOSTR:%d,%d,%d", &device_socket, &sequence, &status);

    if (1 == status)
    {
        bc28_socket_event_send(device, SET_EVENT(device_socket, BC28_EVENT_SEND_OK));
    }
    else
    {
        bc28_socket_event_send(device, SET_EVENT(device_socket, BC28_EVENT_SEND_FAIL));
    }
}

static void urc_close_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    sscanf(data, "+NSOCLI: %d", &device_socket);

    bc28_socket_event_send(device, SET_EVENT(device_socket, BC28_EVENT_CONN_FAIL));

    if (device_socket >= 0)
    {
        /* get at socket object by device socket descriptor */
        socket = &(device->sockets[device_socket]);

        /* notice the socket is disconnect by remote */
        if (at_evt_cb_set[AT_SOCKET_EVT_CLOSED])
        {
            at_evt_cb_set[AT_SOCKET_EVT_CLOSED](socket, AT_SOCKET_EVT_CLOSED, NULL, 0);
        }
    }
}

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    rt_int32_t timeout;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, *hex_buf = RT_NULL, temp[8] = {0};
    char remote_addr[IP_ADDR_SIZE_MAX] = {0};
    int remote_port = -1, return_socket = -1, data_length = 0, remaining_length = 0;

    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    hex_buf = (char *) rt_calloc(1, BC28_MODULE_RECV_MAX_SIZE * 2 + 1);

    /* get the current socket and receive buffer size by receive data */

    /* mode 2 => +NSONMI:<socket>,<remote_addr>, <remote_port>,<length>,<data> */
    sscanf(data, "+NSONMI:%d,%[0-9.],%d,%d,%s", &device_socket, remote_addr, &remote_port, (int *) &bfsz, hex_buf);
    LOG_D("%s device socket(%d) recv %d bytes from %s:%d\n>> %s", device_socket, bfsz, remote_addr, remote_port, hex_buf);

    /* set receive timeout by receive buffer length, not less than 10 ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
    {
        return;
    }

    recv_buf = (char *) rt_calloc(1, bfsz + 1);

    if (recv_buf == RT_NULL || hex_buf == RT_NULL)
    {
        LOG_E("no memory for URC receive buffer(%d).", bfsz);
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

        if (recv_buf) rt_free(recv_buf);
        if (hex_buf)  rt_free(hex_buf);
        return;
    }

    /* convert receive data */
    hex_to_string(hex_buf, recv_buf, bfsz);
    rt_free(hex_buf);

    /* get at socket object by device socket descriptor */
    socket = &(device->sockets[device_socket]);

    /* notice the receive buffer and buffer size */
    if (at_evt_cb_set[AT_SOCKET_EVT_RECV])
    {
        at_evt_cb_set[AT_SOCKET_EVT_RECV](socket, AT_SOCKET_EVT_RECV, recv_buf, bfsz);
    }
}

static void urc_dns_func(struct at_client *client, const char *data, rt_size_t size)
{
    int i = 0, j = 0;
    char recv_ip[16] = {0};
    int result, ip_count, dns_ttl;
    struct at_device *device = RT_NULL;
    struct at_device_bc28 *bc28 = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    bc28 = (struct at_device_bc28 *) device->user_data;
    if (bc28->socket_data == RT_NULL)
    {
        LOG_D("%s device socket_data no config.", bc28->device_name);
        return;
    }

    sscanf(data, "+QDNS:%s", recv_ip);
    recv_ip[15] = '\0';

    if (rt_strstr(recv_ip, "FAIL"))
    {
        bc28_socket_event_send(device, BC28_EVENT_DOMAIN_FAIL);
    }
    else
    {
        rt_memcpy(bc28->socket_data, recv_ip, sizeof(recv_ip));
        bc28_socket_event_send(device, BC28_EVENT_DOMAIN_OK);
    }
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data);

    LOG_I("URC data : %.*s", size, data);
}

/* +NSOSTR:<socket>,<sequence>,<status> */
static const struct at_urc urc_table[] =
{
    {"+QDNS:",      "\r\n",       urc_dns_func},
    {"+QTCPIND:",   "\r\n",       urc_connect_func},
    {"+NSOSTR:",    "\r\n",       urc_send_func},
    {"+NSONMI:",    "\r\n",       urc_recv_func},
    {"+NSOCLI:",    "\r\n",       urc_close_func},
};

static const struct at_socket_ops bc28_socket_ops =
{
    bc28_socket_connect,
    bc28_socket_close,
    bc28_socket_send,
    bc28_domain_resolve,
    bc28_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    bc28_socket_create,
#endif
};

int bc28_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int bc28_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_BC28_SOCKETS_NUM;
    class->socket_ops = &bc28_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_BC28 && AT_USING_SOCKET */
