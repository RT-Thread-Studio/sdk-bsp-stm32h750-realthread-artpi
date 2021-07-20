/*
 * File      : at_socket_w60x.c
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
 * 2018-06-20     chenyong     first version
 * 2019-05-09     chenyong     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_w60x.h>

#define LOG_TAG                       "at.skt.w60x"
#include <at_log.h>

#if defined(AT_DEVICE_USING_W60X) && defined(AT_USING_SOCKET)

#define W60X_MODULE_SEND_MAX_SIZE   512

static rt_int32_t w60x_socket_fd[AT_DEVICE_W60X_SOCKETS_NUM] = {-1};

static at_evt_cb_t at_evt_cb_set[] = {
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
static int w60x_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    int wsk = w60x_socket_fd[device_socket];

    w60x_socket_fd[device_socket] = -1;
    resp = at_create_resp(64, 1, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    at_obj_set_end_sign(device->client, '\r');

    result = at_obj_exec_cmd(device->client, resp, "AT+SKCLS=%d", wsk);

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
static int w60x_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    int socket_fd = -1;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    resp = at_create_resp(64, 1, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    at_obj_set_end_sign(device->client, '\r');

    /* send the "AT+SKRPTM" commands */
    if (at_obj_exec_cmd(device->client, resp, "AT+SKRPTM=1") < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }
    rt_thread_mdelay(20);

    switch (type)
    {
        case AT_SOCKET_TCP:
            /* send AT commands */
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+SKCT=0,%d,%s,%d", is_client ? 0 : 1, is_client ? ip : 0, port) < 0)
            {
                result = -RT_ERROR;
            }
            break;

        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+SKCT=1,%d,%s,%d", is_client ? 0 : 1, is_client ? ip : 0, port) < 0)
            {
                result = -RT_ERROR;
            }
            break;

        default:
            LOG_E("not supported connect type %d.", type);
            result = -RT_ERROR;
            goto __exit;
    }

    if ((result != RT_EOK) || !rt_strstr(at_resp_get_line(resp, 1), "+OK="))
    {
        LOG_D("%s device socket connect failed.", device->name);
        result = -1;
        goto __exit;
    }

    socket_fd = atoi(rt_strstr(at_resp_get_line(resp, 1), "+OK=") + rt_strlen("+OK="));
    w60x_socket_fd[device_socket] = socket_fd;

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
static int w60x_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    int result = RT_EOK;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    struct at_device_w60x *w60x = (struct at_device_w60x *) device->user_data;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);
    RT_ASSERT(bfsz > 0);

    resp = at_create_resp(128, 1, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* set current socket for send URC event */
    w60x->user_data = (void *) device_socket;

    /* set AT client end sign to deal with '\n' sign */
    at_obj_set_end_sign(device->client, '\n');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < W60X_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = W60X_MODULE_SEND_MAX_SIZE;
        }

        rt_thread_mdelay(5);
        /* send the "AT+SKSND" commands */
        if (at_obj_exec_cmd(device->client, resp, "AT+SKSND=%d,%d", w60x_socket_fd[device_socket], cur_pkt_size) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (!rt_strstr(at_resp_get_line(resp, 1), "+OK="))
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
static int w60x_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY        5

    int i, result = -RT_ERROR;
    char recv_ip[16] = { 0 };
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char *pos;

    RT_ASSERT(name);
    RT_ASSERT(ip);

    device = at_device_get_first_initialized();
    if (device == RT_NULL)
    {
        LOG_E("get first init device failed.");
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 1, 20 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    at_obj_set_end_sign(device->client, '\r');

    for (i = 0; i < RESOLVE_RETRY; i++)
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+SKGHBN=%s", name) < 0)
        {
            goto __exit;
        }

        /* parse the third line of response data, get the IP address */
        pos = rt_strstr(resp->buf, "+OK=");
        if (!pos)
        {
            rt_thread_mdelay(100);
            /* resolve failed, maybe receive an URC CRLF */
            continue;
        }

        sscanf(pos, "+OK=\"%[^\"]\"", recv_ip);

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
 * set AT socket event notice callback
 *
 * @param event notice event
 * @param cb notice callback
 */
static void w60x_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}

static const struct at_socket_ops w60x_socket_ops =
{
    w60x_socket_connect,
    w60x_socket_close,
    w60x_socket_send,
    w60x_domain_resolve,
    w60x_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = -1;
    rt_int32_t timeout = 0;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, temp[8] = {0};
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    char recv_ip[16] = { 0 };
    rt_int32_t recv_port = 0;
    rt_uint8_t i;
    char *pos;
    int wsk;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    /* get the at deveice socket and receive buffer size by receive data */
    pos = rt_strstr(data, "+SKTRPT=");
    sscanf(pos, "+SKTRPT=%d,%d,%[^,],%d", &wsk, (int *) &bfsz, recv_ip, &recv_port);

    for (i = 0; i < AT_DEVICE_W60X_SOCKETS_NUM; i++)
    {
        if (wsk == w60x_socket_fd[i])
        {
            device_socket = i;
            break;
        }
    }

    /* set receive timeout by receive buffer length, not less than 10ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
        return;

    recv_buf = (char *) rt_calloc(1, bfsz);
    if (recv_buf == RT_NULL)
    {
        LOG_E("no memory receive buffer(%d).", bfsz);
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

    /* "\n\r\n" left in SERIAL */
    at_client_obj_recv(client, temp, 3, timeout);

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
    {"+SKTRPT=",     "\r", urc_recv_func},
    {"\r\n+SKTRPT=", "\r", urc_recv_func},
    {"\r+SKTRPT=",   "\r", urc_recv_func},
    {"\n+SKTRPT=",   "\r", urc_recv_func},
};

int w60x_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int w60x_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_W60X_SOCKETS_NUM;
    class->socket_ops = &w60x_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_W60X && AT_USING_SOCKET */
