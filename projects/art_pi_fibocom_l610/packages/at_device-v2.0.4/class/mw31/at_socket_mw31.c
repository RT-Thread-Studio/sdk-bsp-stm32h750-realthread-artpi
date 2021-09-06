/*
 * File      : at_socket_mw31.c
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
 * 2019-06-23     flybreak     first version
 */

#include <stdio.h>
#include <string.h>

#include <at_device_mw31.h>

#define LOG_TAG                       "at.skt.mw31"
#include <at_log.h>

#if defined(AT_DEVICE_USING_MW31) && defined(AT_USING_SOCKET)

#define MW31_MODULE_SEND_MAX_SIZE   1024
/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define MW31_EVENT_CONN_OK          (1L << 0)
#define MW31_EVENT_SEND_OK          (1L << 1)
#define MW31_EVENT_RECV_OK          (1L << 2)
#define MW31_EVNET_CLOSE_OK         (1L << 3)
#define MW31_EVENT_CONN_FAIL        (1L << 4)
#define MW31_EVENT_SEND_FAIL        (1L << 5)

static at_evt_cb_t at_evt_cb_set[] =
{
    [AT_SOCKET_EVT_RECV] = NULL,
    [AT_SOCKET_EVT_CLOSED] = NULL,
};


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
static int mw31_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    char type[15] = {0}, status[15] = {0};

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    at_obj_exec_cmd(device->client, resp, "AT+CIPSTATUS=%d", device_socket);

    if (at_resp_parse_line_args_by_kw(resp, "+CIPSTATU:", "+CIPSTATU:%[^,],%s", type, status) > 0)
    {
        LOG_D("%s\n%s\n", type, status);
    }

    if (status[0] == 'd' || status[2] == 'o')
    {
        goto __exit;
    }

    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTOP=%d", device_socket);

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
static int mw31_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    int result = RT_EOK;
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

__retry:
    if (is_client)
    {
        switch (type)
        {
        case AT_SOCKET_TCP:
            /* send AT commands to connect TCP server */
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+CIPSTART=%d,tcp_client,%s,%d,%d", device_socket, ip, port, device_socket) < 0)
            {
                result = -RT_ERROR;
            }
            break;

        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+CIPSTART=%d,udp_unicast,%s,%d,%d", device_socket, ip, port, device_socket) < 0)
            {
                result = -RT_ERROR;
            }
            break;

        default:
            LOG_E("not supported connect type %d.", type);
            result = -RT_ERROR;
            goto __exit;
        }
    }

    if (result != RT_EOK && retryed == RT_FALSE)
    {
        LOG_D("%s device socket (%d) connect failed, the socket was not be closed and now will connect retry.",
              device->name, device_socket);
        if (mw31_socket_close(socket) < 0)
        {
            goto __exit;
        }
        retryed = RT_TRUE;
        result = RT_EOK;
        goto __retry;
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
static int mw31_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    int result = RT_EOK;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    struct at_device_mw31 *mw31 = (struct at_device_mw31 *) device->user_data;
    rt_mutex_t lock = device->client->lock;
    char send_buf[20] = {0};

    RT_ASSERT(buff);
    RT_ASSERT(bfsz > 0);

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* set current socket for send URC event */
    mw31->user_data = (void *) device_socket;

    /* set AT client end sign to deal with '>' sign */
    at_obj_set_end_sign(device->client, '>');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < MW31_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = MW31_MODULE_SEND_MAX_SIZE;
        }

        sprintf(send_buf, "AT+CIPSEND=%d,%d", device_socket, cur_pkt_size);
        /* send the "AT+CIPSEND" commands to AT server than receive the '>' response on the first line */
        at_client_obj_send(device->client, send_buf, strlen(send_buf));

        at_client_obj_send(device->client, "\r", 1);

        /* send the real data to server or client */
        result = (int) at_client_obj_send(device->client, buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
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
static int mw31_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY        5

    int i, result = RT_EOK;
    rt_uint8_t recv_ip_num = 0;
    char recv_ip[16] = { 0 };
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

    resp = at_create_resp(128, 0, 20 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    for (i = 0; i < RESOLVE_RETRY; i++)
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+CIPDOMAIN=%s", name) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse the third line of response data, get the IP address */
        if (at_resp_parse_line_args_by_kw(resp, "+CIPDOMAIN:", "+CIPDOMAIN:%d", &recv_ip_num) < 0 ||
                recv_ip_num == 0 ||
                at_resp_parse_line_args(resp, 3, "%s", recv_ip) < 0)
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
static void mw31_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}

static const struct at_socket_ops mw31_socket_ops =
{
    mw31_socket_connect,
    mw31_socket_close,
    mw31_socket_send,
    mw31_domain_resolve,
    mw31_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    rt_int32_t timeout = 0;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, temp[8] = {0};
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    rt_uint8_t i;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    at_client_obj_recv(client, temp, 2, 1000);
    /* get the at deveice socket and receive buffer size by receive data */
    sscanf(temp, "%d,", &device_socket);
    temp[0] = 0;
    temp[1] = 0;
    for (i = 0; i < 6 && temp[i - 1] != ','; i++)
    {
        at_client_obj_recv(client, &temp[i], 1, 1000);
    }
    sscanf(temp, "%ld,", &bfsz);

    LOG_D("socket:%d, size:%ld\n", device_socket, bfsz);
    /* set receive timeout by receive buffer length, not less than 10 ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
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

    /* get at socket object by device socket descriptor */
    socket = &(device->sockets[device_socket]);

    /* notice the receive buffer and buffer size */
    if (at_evt_cb_set[AT_SOCKET_EVT_RECV])
    {
        at_evt_cb_set[AT_SOCKET_EVT_RECV](socket, AT_SOCKET_EVT_RECV, recv_buf, bfsz);
    }
}

static const struct at_urc urc_table[] =
{
    {"+CIPEVENT:SOCKET",   ",",              urc_recv_func},
    {":SOCKET",            ",",              urc_recv_func},
};

int mw31_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int mw31_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_MW31_SOCKETS_NUM;
    class->socket_ops = &mw31_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_MW31 && AT_USING_SOCKET */
