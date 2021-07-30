/*
 * File      : at_socket_n58.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
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
 * 2020-05-22     shuobatian        first version
 */

#include <stdio.h>
#include <string.h>
#include <at_device_n58.h>

#if !defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10300
#error "This AT Client version is older, please check and update latest AT Client!"
#endif

#define LOG_TAG "at.skt"
#include <at_log.h>

#if defined(AT_DEVICE_USING_N58) && defined(AT_USING_SOCKET)

#define N58_MODULE_SEND_MAX_SIZE 1000

/* set real event by current socket and current state */
#define SET_EVENT(socket, event) (((socket + 1) << 16) | (event))

/* AT socket event type */
#define N58_EVENT_CONN_OK (1L << 0)
#define N58_EVENT_SEND_OK (1L << 1)
#define N58_EVENT_RECV_OK (1L << 2)
#define N58_EVNET_CLOSE_OK (1L << 3)
#define N58_EVENT_CONN_FAIL (1L << 4)
#define N58_EVENT_SEND_FAIL (1L << 5)

static at_evt_cb_t at_evt_cb_set[] = {
    [AT_SOCKET_EVT_RECV] = NULL,
    [AT_SOCKET_EVT_CLOSED] = NULL,
};

static int n58_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int)rt_event_send(device->socket_event, event);
}

static int n58_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
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
static int n58_socket_close(struct at_socket *socket)
{
    uint32_t event = 0;
    int result = RT_EOK;
    int device_socket = (int)socket->user_data;
    enum at_socket_type type_socket = socket->type;
    struct at_device *device = (struct at_device *)socket->device;

    /* clear socket close event */
    event = SET_EVENT(device_socket, N58_EVNET_CLOSE_OK);
    n58_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    if (type_socket == AT_SOCKET_TCP)
    {
        if (at_obj_exec_cmd(device->client, NULL, "AT+TCPCLOSE=%d", device_socket) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }
    }
    else if (type_socket == AT_SOCKET_UDP)
    {
        if (at_obj_exec_cmd(device->client, NULL, "AT+UDPCLOSE=%d", device_socket) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }
    }

    if (n58_socket_event_recv(device, event, rt_tick_from_millisecond(300 * 3), RT_EVENT_FLAG_AND) < 0)
    {
        LOG_E("n58 device(%s) socket(%d) close failed, wait close OK timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }

__exit:
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
static int n58_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    uint32_t event = 0;
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int result = RT_EOK, event_result = 0;
    int device_socket = (int)socket->user_data;
    struct at_device *device = (struct at_device *)socket->device;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for n58 device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

__retry:

    /* clear socket connect event */
    event = SET_EVENT(device_socket, N58_EVENT_CONN_OK | N58_EVENT_CONN_FAIL);
    n58_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    if (is_client)
    {
        switch (type)
        {
        case AT_SOCKET_TCP:
            /* send AT commands(eg: AT+TCPSETUP=<n>,<ip>,<port>) to connect TCP server */
            if (at_obj_exec_cmd(device->client, RT_NULL,
                                "AT+TCPSETUP=%d,%s,%d", device_socket, ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;
            /* send AT commands(eg: AT+UDPSETUP=<n>,<ip>,<port>) to connect TCP server */
        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, RT_NULL,
                                "AT+UDPSETUP=%d,%s,%d", device_socket, ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;

        default:
            LOG_E("n58 device(%s) not supported connect type : %d.", device->name, type);
            result = -RT_ERROR;
            goto __exit;
        }
    }

    /* waiting result event from AT URC, the device default connection timeout is 75 seconds, but it set to 10 seconds is convenient to use */
    if (n58_socket_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
    {
        LOG_E("n58 device(%s) socket(%d) connect failed, wait connect result timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* waiting OK or failed result */
    event_result = n58_socket_event_recv(device,
                                         N58_EVENT_CONN_OK | N58_EVENT_CONN_FAIL, 1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
    if (event_result < 0)
    {
        LOG_E("n58 device(%s) socket(%d) connect failed, wait connect OK|FAIL timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* check result */
    if (event_result & N58_EVENT_CONN_FAIL)
    {
        if (retryed == RT_FALSE)
        {
            LOG_D("n58 device(%s) socket(%d) connect failed, maybe the socket was not be closed at the last time and now will retry.",
                  device->name, device_socket);
            if (n58_socket_close(socket) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            retryed = RT_TRUE;
            goto __retry;
        }
        LOG_E("n58 device(%s) socket(%d) connect failed.", device->name, device_socket);
        result = -RT_ERROR;
        goto __exit;
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
static int n58_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    uint32_t event = 0;
    int result = RT_EOK, event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int)socket->user_data;
    struct at_device *device = (struct at_device *)socket->device;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);

    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for n58 device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* clear socket connect event */
    event = SET_EVENT(device_socket, N58_EVENT_SEND_OK | N58_EVENT_SEND_FAIL);
    n58_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    /* set AT client end sign to deal with '>' sign.*/
    at_obj_set_end_sign(device->client, '>');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < N58_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = N58_MODULE_SEND_MAX_SIZE;
        }

        /* send the "AT+QISEND" commands to AT server than receive the '>' response on the first line. */
        if (type == AT_SOCKET_TCP)
        {
            if (at_obj_exec_cmd(device->client, resp, "AT+TCPSEND=%d,%d", device_socket, cur_pkt_size) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
        }
        else if (type == AT_SOCKET_UDP)
        {
            if (at_obj_exec_cmd(device->client, resp, "AT+UDPSEND=%d,%d", device_socket, cur_pkt_size) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
        }

        /* send the real data to server or client */
        result = (int)at_client_obj_send(device->client, buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting result event from AT URC */
        if (n58_socket_event_recv(device, SET_EVENT(device_socket, 0), 15 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("n58 device(%s) socket(%d) send failed, wait connect result timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = n58_socket_event_recv(device,
                                             N58_EVENT_SEND_OK | N58_EVENT_SEND_FAIL, 5 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            LOG_E("n58 device(%s) socket(%d) send failed, wait connect OK|FAIL timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* check result */
        if (event_result & N58_EVENT_SEND_FAIL)
        {
            LOG_E("n58 device(%s) socket(%d) send failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        sent_size += cur_pkt_size;
    }

__exit:
    /* reset the end sign for data conflict */
    at_obj_set_end_sign(device->client, 0);

    rt_mutex_release(lock);

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
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
static int n58_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY 5

    int i, result = RT_EOK;
    char recv_ip[16] = {0};
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(name);
    RT_ASSERT(ip);

    device = at_device_get_first_initialized();
    if (device == RT_NULL)
    {
        LOG_E("get first initialization n58 device failed.");
        return -RT_ERROR;
    }

    /* The maximum response time is 14 seconds, affected by network status */
    resp = at_create_resp(128, 4, 14 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for n58 device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

    for (i = 0; i < RESOLVE_RETRY; i++)
    {
        int err_code = 0;

        if (at_obj_exec_cmd(device->client, resp, "AT+CDNSGIP=\"%s\"", name) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* domain name prase error options */
        if (at_resp_parse_line_args_by_kw(resp, "+CDNSGIP: 0", "+CDNSGIP: 0,%d", &err_code) > 0)
        {
            /* 3 - network error, 8 - dns common error */
            if (err_code == 3 || err_code == 8)
            {
                result = -RT_ERROR;
                goto __exit;
            }
        }

        /* parse the third line of response data, get the IP address */
        if (at_resp_parse_line_args_by_kw(resp, "+CDNSGIP:", "%*[^,],%*[^,],\"%[^\"]", recv_ip) < 0)
        {
            rt_thread_mdelay(100);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }

        if (rt_strlen(recv_ip) < 8)
        {
            rt_thread_mdelay(100);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }
        else
        {
            rt_thread_mdelay(10);
            rt_strncpy(ip, recv_ip, 15);
            ip[15] = '\0';
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
 * set AT socket event notice callback
 *
 * @param event notice event
 * @param cb notice callback
 */
static void n58_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}

static void urc_connect_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    char constat[16] = {0};

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get n58 device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "%*[^ ]%d,%s", &device_socket, constat);

    LOG_D("data:%s", data);

    LOG_D("socket:%d,constat:%s", device_socket, constat);

    if (strstr(constat, "OK"))
    {
        LOG_D("socket %d:connect ok!", device_socket);
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVENT_CONN_OK));
    }
    else if (strstr(constat, "FAIL"))
    {
        LOG_D("socket %d:connect fail!", device_socket);
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVENT_CONN_FAIL));
    }
}

static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get n58 device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "%*[^ ] %d,%*d\r\n", &device_socket);

    if (rt_strstr(data, "OPERATION"))
    {
        LOG_E("input data timeout!");
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVENT_SEND_FAIL));
    }
    else if (rt_strstr(data, "ERROR")) //链路号错误
    {
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVENT_SEND_FAIL));
    }
    else //没有错误就是成功
    {
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVENT_SEND_OK));
    }
}

static void urc_close_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get n58 device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    /* +TCPCLOSE: 1,OK */
    sscanf(data, "%*[^ ]%d,%*s", &device_socket);

    if (rt_strstr(data, "OK"))
    {
        n58_socket_event_send(device, SET_EVENT(device_socket, N58_EVNET_CLOSE_OK));
    }
    else if (rt_strstr(data, "Link Closed"))
    {
        struct at_socket *socket = RT_NULL;

        /* get AT socket object by device socket descriptor */
        socket = &(device->sockets[device_socket]);

        /* notice the socket is disconnect by remote */
        if (at_evt_cb_set[AT_SOCKET_EVT_CLOSED])
        {
            at_evt_cb_set[AT_SOCKET_EVT_CLOSED](socket, AT_SOCKET_EVT_CLOSED, RT_NULL, 0);
        }
    }
}

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    rt_size_t bfsz = 0;
    int data_index = size - 1;
    char *recv_buf = RT_NULL;
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    /* get the current socket and receive buffer size by receive data */
    sscanf(data, "%*[^ ] %d,%d,", &device_socket, (int *)&bfsz);

    recv_buf = (char *)rt_calloc(1, bfsz + 1);

    if (recv_buf == RT_NULL)
    {
        LOG_E("no memory for n58 device(%s) URC receive buffer (%d).", device->name, bfsz);
        return;
    }

    data_index -= 2; //"\r\n"移除
    recv_buf[bfsz] = '\0';
    for (int i = bfsz - 1; i >= 0; i--)
    {
        recv_buf[i] = data[data_index];
        data_index--;
    }

    /* get receive timeout by receive buffer length */

    LOG_D("recv socket:%d", device_socket);

    if (device_socket < 0 || bfsz == 0)
    {
        return;
    }

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get n58 device by client name(%s) failed.", client_name);
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

/* n58 device URC table for the socket data */
static const struct at_urc urc_table[] =
{
    {"+TCPSETUP:", "\r\n", urc_connect_func},
    {"+UDPSETUP:", "\r\n", urc_connect_func},
    {"+TCPSEND:", "\r\n", urc_send_func},
    {"+UDPSEND:", "\r\n", urc_send_func},
    {"+TCPCLOSE:", "\r\n", urc_close_func},
    {"+UDPCLOSE:", "\r\n", urc_close_func},
    {"+TCPRECV:", "\r\n", urc_recv_func},
    {"+UDPRECV:", "\r\n", urc_recv_func},
};

static const struct at_socket_ops n58_socket_ops =
{
    n58_socket_connect,
    n58_socket_close,
    n58_socket_send,
    n58_domain_resolve,
    n58_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int n58_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int n58_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_N58_SOCKETS_NUM;
    class->socket_ops = &n58_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_n58 && AT_USING_SOCKET */
