/*
 * File      : at_socket_me3616.c
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
 * 2019-12-30     qiyongzhong  first version
 */

#include <stdio.h>
#include <string.h>

#include <at_device_me3616.h>

#define LOG_TAG                        "at.skt.me3616"
#include <at_log.h>

#if defined(AT_DEVICE_USING_ME3616) && defined(AT_USING_SOCKET)

#define ME3616_MODULE_SEND_MAX_SIZE       512

static int me3616_socket_fd[AT_DEVICE_ME3616_SOCKETS_NUM] = {0};

static at_evt_cb_t at_evt_cb_set[] = {
        [AT_SOCKET_EVT_RECV] = NULL,
        [AT_SOCKET_EVT_CLOSED] = NULL,
};

static int me3616_get_socket_idx(int sock)
{
    int i;

    if (sock < 0)
    {
        return(-1);
    }

    for (i=0; i<AT_DEVICE_ME3616_SOCKETS_NUM; i++)
    {
        if (me3616_socket_fd[i] == sock)
            return(i);
    }

    return(-1);
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
static int me3616_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    if (me3616_socket_fd[device_socket] == -1)
    {
        return RT_EOK;
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    result = at_obj_exec_cmd(device->client, resp, "AT+ESOCL=%d", me3616_socket_fd[device_socket]);
    me3616_socket_fd[device_socket] = -1;

    at_delete_resp(resp);

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
static int me3616_socket_connect(struct at_socket *socket, char *ip, int32_t port,
    enum at_socket_type type, rt_bool_t is_client)
{
    #define CONN_RESP_SIZE  128

    int type_code = 0;
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    int sock = -1;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    if ( ! is_client)
    {
        return -RT_ERROR;
    }

    switch(type)
    {
        case AT_SOCKET_TCP:
            type_code = 1;
            break;
        case AT_SOCKET_UDP:
            type_code = 2;
            break;
        default:
            LOG_E("%s device socket(%d)  connect type error.", device->name, device_socket);
            return -RT_ERROR;
    }

    resp = at_create_resp(CONN_RESP_SIZE, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    if (me3616_socket_fd[device_socket] != -1)
    {
        at_obj_exec_cmd(device->client, resp, "AT+ESOCL=%d", me3616_socket_fd[device_socket]);
        me3616_socket_fd[device_socket] = -1;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+ESOC=1,%d,1", type_code) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+ESOC=", "+ESOC=%d", &sock) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    at_resp_set_info(resp, CONN_RESP_SIZE, 0, (45*RT_TICK_PER_SECOND));
    if (at_obj_exec_cmd(device->client, resp, "AT+ESOCON=%d,%d,\"%s\"", sock, port, ip) < 0)
    {
        at_resp_set_info(resp, CONN_RESP_SIZE, 0, rt_tick_from_millisecond(300));
        at_obj_exec_cmd(device->client, resp, "AT+ESOCL=%d", sock);
        result = -RT_ERROR;
        goto __exit;
    }

    me3616_socket_fd[device_socket] = sock;

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int at_get_send_size(struct at_socket *socket, size_t *nacked)
{
    int result = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    int remain_size;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.", device->name);
        return -RT_ENOMEM;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+ESOTCPBUF=%d", me3616_socket_fd[device_socket]) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+ESOTCPBUF=", "+ESOTCPBUF=%d", &remain_size) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    *nacked = 4096 - remain_size;

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int at_wait_send_finish(struct at_socket *socket, rt_tick_t timeout)
{
    rt_tick_t last_time = rt_tick_get();
    size_t nacked = 0xFFFF;

    while (rt_tick_get() - last_time <= timeout)
    {
        at_get_send_size(socket, &nacked);
        if (nacked == 0)
        {
            return RT_EOK;
        }
        rt_thread_mdelay(50);
    }

    return -RT_ETIMEOUT;
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
static int me3616_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    #define SEND_RESP_SIZE      128

    int result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);

    resp = at_create_resp(SEND_RESP_SIZE, 2, RT_TICK_PER_SECOND/2);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < ME3616_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = ME3616_MODULE_SEND_MAX_SIZE;
        }

        at_resp_set_info(resp, SEND_RESP_SIZE, 2, RT_TICK_PER_SECOND/2);
        if (at_obj_exec_cmd(device->client, resp, "AT+ESOSENDRAW=%d,%d", me3616_socket_fd[device_socket], (int)cur_pkt_size) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_get_line_by_kw(resp, "CONNECT") == RT_NULL)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        rt_thread_mdelay(5);//delay at least 4 ms

        /* send the real data to server or client */
        result = (int) at_client_obj_send(device->client, buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* wait respone "NO CARRIER ... OK " */
        at_resp_set_info(resp, SEND_RESP_SIZE, 0, (2*RT_TICK_PER_SECOND));
        if (at_obj_exec_cmd(device->client, resp, "") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (type == AT_SOCKET_TCP)
        {
            at_wait_send_finish(socket, (5*RT_TICK_PER_SECOND));
        }
        else
        {
            rt_thread_mdelay(10);//delay at least 10 ms
        }

        sent_size += cur_pkt_size;
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
static int me3616_domain_resolve(const char *name, char ip[16])
{
    int result;
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

    resp = at_create_resp(128, 0, (15 * RT_TICK_PER_SECOND));
    if (!resp)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    result = at_obj_exec_cmd(device->client, resp, "AT+EDNS=\"%s\"", name);
    if (result != RT_EOK)
    {
        LOG_E("%s device \"AT+EDNS=\"%s\"\" cmd error.", device->name, name);
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "IPV4:", "IPV4:%s\r", ip) <= 0)
    {
        LOG_E("%s device prase \"AT+EDNS=\"%s\"\" cmd error.", device->name, name);
        result = -RT_ERROR;
        goto __exit;
    }

    ip[15] = 0;
    if (rt_strlen(ip) < 8)
    {
        result = -RT_ERROR;
    }
    else
    {
        result = RT_EOK;
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
static void me3616_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}

static void urc_close_func(struct at_client *client, const char *data, rt_size_t size)
{
    int sock = -1;
    int err_code = 0;
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

    sscanf(data, "+ESOERR=%d,%d", &sock, &err_code);

    device_socket = me3616_get_socket_idx(sock);
    if (device_socket < 0 || err_code < 0 || err_code > 4)
    {
        return;
    }

    /* get at socket object by device socket descriptor */
    socket = &(device->sockets[device_socket]);

    /* notice the socket is disconnect by remote */
    if (at_evt_cb_set[AT_SOCKET_EVT_CLOSED])
    {
        at_evt_cb_set[AT_SOCKET_EVT_CLOSED](socket, AT_SOCKET_EVT_CLOSED, NULL, 0);
    }
}

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int sock = -1;
    int device_socket = 0;
    rt_int32_t timeout;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, temp[8] = {0};
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

    sscanf(data, "+ESONMI=%d,", &sock);
    device_socket = me3616_get_socket_idx(sock);
    if (device_socket < 0)
    {
        return;
    }

    while(temp_size < sizeof(temp))
    {
        at_client_obj_recv(client, temp+temp_size, 1, 10);
        if ( *(temp+temp_size) == ',')
        {
            *(temp+temp_size) = 0;
            break;
        }
        temp_size++;
    }
    if (temp_size == sizeof(temp))
    {
        return;
    }

    sscanf(temp, "%d", (int *)&bfsz);
    if(bfsz == 0)
    {
        return;
    }

    timeout = bfsz > 10 ? bfsz : 10;

    recv_buf = (char *) rt_calloc(1, bfsz);
    if (recv_buf == RT_NULL)
    {
        LOG_E("no memory for URC receive buffer(%d).", bfsz);
        /* read and clean the coming data */
        temp_size = 0;
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

    if (at_client_obj_recv(client, recv_buf, bfsz, timeout) != bfsz)
    {
        LOG_E("%s device receive size(%d) data failed.", device->name, bfsz);
        rt_free(recv_buf);
        return;
    }

    /* read end "\r\n" */
    at_client_obj_recv(client, temp, 2, 5);

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
    {"+ESOERR=",    "\r\n",                 urc_close_func},
    {"+ESONMI=",    ",",                    urc_recv_func},
};

static const struct at_socket_ops me3616_socket_ops =
{
    me3616_socket_connect,
    me3616_socket_close,
    me3616_socket_send,
    me3616_domain_resolve,
    me3616_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int me3616_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    rt_memset(me3616_socket_fd, -1, sizeof(me3616_socket_fd));

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int me3616_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_ME3616_SOCKETS_NUM;
    class->socket_ops = &me3616_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_ME3616 && AT_USING_SOCKET */

