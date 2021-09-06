/*
 * File      : at_socket_a9g.c
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
 * 2019-11-23     luliang    first version
 */

#include <stdio.h>
#include <string.h>

#include <at_device_a9g.h>

#define LOG_TAG                        "at.skt.a9g"
#include <at_log.h>

#if defined(AT_DEVICE_USING_A9G) && defined(AT_USING_SOCKET)

#define A9G_MODULE_SEND_MAX_SIZE   1000

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define A9G_EVENT_CONN_OK          (1L << 0)
#define A9G_EVENT_SEND_OK          (1L << 1)
#define A9G_EVENT_RECV_OK          (1L << 2)
#define A9G_EVENT_CLOSE_OK         (1L << 3)
#define A9G_EVENT_CONN_FAIL        (1L << 4)
#define A9G_EVENT_SEND_FAIL        (1L << 5)

static at_evt_cb_t at_evt_cb_set[] = {
        [AT_SOCKET_EVT_RECV] = NULL,
        [AT_SOCKET_EVT_CLOSED] = NULL,
};

static int a9g_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int) rt_event_send(device->socket_event, event);
}

static int a9g_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
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
static int a9g_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(600));

    if (resp == RT_NULL)
    {
        LOG_E("no memory for a9g device(%s) response creat.", device->name);
        return -RT_ENOMEM;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE=%d", device_socket) < 0)
    {
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
static int a9g_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    uint32_t event = 0;
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int result = RT_EOK, event_result = 0;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for a9g device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

__retry:

    /* clear socket connect event */
    event = SET_EVENT(device_socket, A9G_EVENT_CONN_OK | A9G_EVENT_CONN_FAIL);
    a9g_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    if (is_client)
    {
        switch (type)
        {
        case AT_SOCKET_TCP:
            /* send AT commands(eg: AT+QIOPEN=0,"TCP","x.x.x.x", 1234) to connect TCP server */
            if (at_obj_exec_cmd(device->client, RT_NULL, "AT+CIPSTART=\"TCP\",\"%s\",%d", ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;

        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, RT_NULL, "AT+CIPSTART=\"UDP\",\"%s\",%d", ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;

        default:
            LOG_E("a9g device(%s) not supported connect type : %d.", device->name, type);
            result = -RT_ERROR;
            goto __exit;
        }
            }

    /* waiting result event from AT URC, the device default connection timeout is 75 seconds, but it set to 10 seconds is convenient to use */
    if (a9g_socket_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
    {
        LOG_E("a9g device(%s) socket(%d) connect failed, wait connect result timeout.",device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* waiting OK or failed result */
    event_result = a9g_socket_event_recv(device,
            A9G_EVENT_CONN_OK | A9G_EVENT_CONN_FAIL, 1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
    if (event_result < 0)
    {
        LOG_E("a9g device(%s) socket(%d) connect failed, wait connect OK|FAIL timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* check result */
    if (event_result & A9G_EVENT_CONN_FAIL)
    {
        if (retryed == RT_FALSE)
        {
            LOG_D("a9g device(%s) socket(%d) connect failed, maybe the socket was not be closed at the last time and now will retry.",
                    device->name, device_socket);
            if (a9g_socket_close(socket) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            retryed = RT_TRUE;
            goto __retry;
        }
        LOG_E("a9g device(%s) socket(%d) connect failed.", device->name, device_socket);
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
static int a9g_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    int result = RT_EOK;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);

    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for a9g device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* set AT client end sign to deal with '>' sign.*/
    at_obj_set_end_sign(device->client, '>');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < A9G_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = A9G_MODULE_SEND_MAX_SIZE;
        }

        /* send the "AT+CIPSEND" commands to AT server than receive the '>' response on the first line. */
        if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,%d", device_socket, cur_pkt_size) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* send the real data to server or client */
        result = (int) at_client_obj_send(device->client, buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting OK or failed result */
        at_resp_set_info(resp, 128, 0, 30 * RT_TICK_PER_SECOND);
        if (at_obj_exec_cmd(device->client, resp, "") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }
        at_resp_set_info(resp, 128, 2, 5 * RT_TICK_PER_SECOND);

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
static int a9g_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY                  5

    int i, result = RT_EOK;
    char recv_ip[16] = { 0 };
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(name);
    RT_ASSERT(ip);

    device = at_device_get_first_initialized();
    if (device == RT_NULL)
    {
        LOG_E("get first initialization a9g device failed.");
        return -RT_ERROR;
    }

    /* The maximum response time is 14 seconds, affected by network status */
    resp = at_create_resp(1024, 3, 14 * RT_TICK_PER_SECOND);

    if (resp == RT_NULL)
    {
        LOG_E("no memory for a9g device(%s) response structure.", device->name);
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
        if (at_resp_parse_line_args_by_kw(resp, "+CDNSGIP:", "+CDNSGIP: 0,%d", &err_code) > 0)
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
static void a9g_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
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

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get a9g device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "%d,%*s", &device_socket);

    if (strstr(data, "CONNECT OK"))
    {
        a9g_socket_event_send(device, SET_EVENT(device_socket, A9G_EVENT_CONN_OK));
    }
    else if (strstr(data, "CONNECT FAIL"))
    {
        a9g_socket_event_send(device, SET_EVENT(device_socket, A9G_EVENT_CONN_FAIL));
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
        LOG_E("get a9g device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "%d,%*s", &device_socket);

    if (rt_strstr(data, "SEND OK"))
    {
        a9g_socket_event_send(device, SET_EVENT(device_socket, A9G_EVENT_SEND_OK));
    }
    else if (rt_strstr(data, "SEND FAIL"))
    {
        a9g_socket_event_send(device, SET_EVENT(device_socket, A9G_EVENT_SEND_FAIL));
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
        LOG_E("get a9g device by client name(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "%d,%*s", &device_socket);

    if (rt_strstr(data, "CLOSE OK"))
    {
        a9g_socket_event_send(device, SET_EVENT(device_socket, A9G_EVENT_CLOSE_OK));
    }
    else if (rt_strstr(data, "CLOSED"))
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
    rt_int32_t timeout;
    rt_size_t temp_size = 0;
    char temp[8] = {0};
    rt_size_t bfsz = 0;
    char *recv_buf = RT_NULL;
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    /* get the current socket and receive buffer size by receive data */
    sscanf(data, "+CIPRCV,%d,%d:", &device_socket, (int *) &bfsz);
    /* get receive timeout by receive buffer length */
    timeout = bfsz;

    if (device_socket < 0 || bfsz == 0)
    {
        return;
    }

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get a9g device by client name(%s) failed.", client_name);
        return;
    }

    recv_buf = (char *) rt_calloc(1, bfsz);
    if (recv_buf == RT_NULL)
    {
        LOG_E("no memory for a9g device(%s) URC receive buffer (%d).", device->name, bfsz);
        temp_size = (size-(rt_strstr(data,":")+1-data)-2);
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
    temp_size = (size-(rt_strstr(data,":")+1-data)-2);
    rt_memcpy(recv_buf,rt_strstr(data,":")+1,temp_size);
    if (at_client_obj_recv(client, recv_buf+temp_size, bfsz-temp_size, timeout) != (bfsz-temp_size))
    {
        LOG_E("a9g device(%s) receive size(%d) data failed.", device->name, bfsz);
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

/* A9G device URC table for the socket data */
static const struct at_urc urc_table[] =
{
    {"",            "CONNECT OK\r\n",      urc_connect_func},
    {"",            ",CONNECT FAIL\r\n",   urc_connect_func},
    {"",            ",SEND FAIL\r\n",      urc_send_func},
    {"CLOSED:",     "\r\n",                urc_close_func},
    {"+CIPRCV,",    "\r\n",                urc_recv_func},
};

static const struct at_socket_ops a9g_socket_ops =
{
    a9g_socket_connect,
    a9g_socket_close,
    a9g_socket_send,
    a9g_domain_resolve,
    a9g_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int a9g_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int a9g_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_A9G_SOCKETS_NUM;
    class->socket_ops = &a9g_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_A9G && AT_USING_SOCKET */
