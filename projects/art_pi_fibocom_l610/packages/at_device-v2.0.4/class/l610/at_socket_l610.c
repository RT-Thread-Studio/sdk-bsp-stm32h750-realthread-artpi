/*
 * File      : at_socket_l610.c
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
 * 2020-10-28     zhangyang    first version
 */

#include <stdio.h>
#include <string.h>

#include <at_device_l610.h>

#define LOG_TAG                     "at.skt.l610"
#include <at_log.h>

#define AT_USING_SOCKET
#define AT_DEVICE_USING_L610

#if defined(AT_DEVICE_USING_L610) && defined(AT_USING_SOCKET)

#define L610_MODULE_SEND_MAX_SIZE   2048
static int l610_socket_fd[AT_DEVICE_L610_SOCKETS_NUM] = {-1};

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define L610_EVENT_CONN_OK          (1L << 0)
#define L610_EVENT_SEND_OK          (1L << 1)
#define L610_EVENT_RECV_OK          (1L << 2)
#define L610_EVNET_CLOSE_OK         (1L << 3)
#define L610_EVENT_CONN_FAIL        (1L << 4)
#define L610_EVENT_SEND_FAIL        (1L << 5)



static at_evt_cb_t at_evt_cb_set[] = {
        [AT_SOCKET_EVT_RECV] = NULL,
        [AT_SOCKET_EVT_CLOSED] = NULL,
};


static int l610_get_socket_idx(int sock)
{
    int i;

    if (sock < 0)
    {
        return(-1);
    }

    for (i=0; i<AT_DEVICE_L610_SOCKETS_NUM; i++)
    {
        if (l610_socket_fd[i] == sock)
            return(i);
    }

    return(-1);
}

static int l610_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int) rt_event_send(device->socket_event, event);
}

static int l610_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
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
static int l610_socket_close(struct at_socket *socket)
{
    uint32_t event = 0;
    int result = RT_EOK;
    int device_socket = (int) socket->user_data;
    int device_socket_id = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    if (l610_socket_fd[device_socket] == -1)
    {
        return RT_EOK;
    }
    device_socket_id=l610_socket_fd[device_socket];
    /* clear socket close event */
    event = SET_EVENT(device_socket_id, L610_EVNET_CLOSE_OK);
    l610_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    if (at_obj_exec_cmd(device->client, NULL, "AT+MIPCLOSE=%d", device_socket_id) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (l610_socket_event_recv(device, event, rt_tick_from_millisecond(300*3), RT_EVENT_FLAG_AND) < 0)
    {
        LOG_E("%s device socket(%d) wait close OK timeout.", device->name, device_socket_id);
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


static int l610_socket_connect(struct at_socket *socket, char *ip, int32_t port,enum at_socket_type type, rt_bool_t is_client)
{
    #define CONN_RESP_SIZE  128

    int type_code = 0;
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    int sock = -1;
    int connect_result;

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    if ( ! is_client)
    {
        return -RT_ERROR;
    }

    switch(type)
    {
        case AT_SOCKET_TCP:
            type_code = 0;  //0:tcp
            break;
        case AT_SOCKET_UDP:
            type_code = 1;   //:1udp
            break;
        default:
            LOG_E("%s device socket(%d)  connect type error.", device->name, device_socket);
            return -RT_ERROR;
    }

    resp = at_create_resp(CONN_RESP_SIZE, 0, rt_tick_from_millisecond(2000));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    if (l610_socket_fd[device_socket] != -1)
    {
        l610_socket_fd[device_socket] = -1;
    }



    if (at_obj_exec_cmd(device->client, resp, "AT+MIPOPEN?") < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+MIPOPEN: ", "+MIPOPEN:%d", &sock) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }


    at_resp_set_info(resp, CONN_RESP_SIZE, 4, (45*RT_TICK_PER_SECOND));

    if(at_obj_exec_cmd(device->client, resp,"AT+MIPOPEN=%d,,\"%s\",%d,%d",sock, ip, port,type_code) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+MIPOPEN: ", "+MIPOPEN: %d,%d", &sock,&connect_result)>0)
    {
        if(connect_result!=1)
        {
            result = -RT_ERROR;
            goto __exit;
        }

    }
    else
    {
        result = -RT_ERROR;
        goto __exit;
    }

    l610_socket_fd[device_socket] = sock;

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
static int l610_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    uint32_t event = 0;
    int result = RT_EOK, event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    rt_mutex_t lock = device->client->lock;
    int sock = -1;

       sock=l610_socket_fd[device_socket];
        if(sock<=0)
        {
            LOG_E("error socket ");
            return -RT_ERROR;
        }

    RT_ASSERT(buff);

    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* clear socket connect event */
    event = SET_EVENT(sock, L610_EVENT_SEND_OK | L610_EVENT_SEND_FAIL);
    l610_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    /* set AT client end sign to deal with '>' sign.*/
    at_obj_set_end_sign(device->client, '>');
    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < L610_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = L610_MODULE_SEND_MAX_SIZE;
        }

        /* send the "AT+QISEND" commands to AT server than receive the '>' response on the first line. */
        if (at_obj_exec_cmd(device->client, resp, "AT+MIPSEND=%d,%d", sock, cur_pkt_size) < 0)
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

        /* waiting result event from AT URC */
        if (l610_socket_event_recv(device, SET_EVENT(sock, 0), 15 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("%s device socket(%d) wait send result timeout.", device->name, sock);
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* waiting OK or failed result */
        event_result = l610_socket_event_recv(device,
                L610_EVENT_SEND_OK | L610_EVENT_SEND_FAIL, 5 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            LOG_E("%s device socket(%d) wait send connect OK|FAIL timeout.", device->name, sock);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* check result */
        if (event_result & L610_EVENT_SEND_FAIL)
        {
            LOG_E("%s device socket(%d) send failed.", device->name, sock);
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
static int l610_domain_resolve(const char *name, char ip[16])
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
        //      0: IPV4 address
        //      1: IPV6 address
        //      2: IPV4/IPV6 address
        //      <IP>: resolved IPV4 or IPV6 address (string without double quotes)
    result = at_obj_exec_cmd(device->client, resp, "AT+MIPDNS=\"%s\",2", name);
    if (result != RT_EOK)
    {
        LOG_E("%s device \"AT+MIPDNS=\"%s\"\" cmd error.", device->name, name);
        goto __exit;
    }
    //GET ip
    if (at_resp_parse_line_args_by_kw(resp, "+MIPDNS: ", "%*[^,],%[^,]",ip) <= 0)
    {
        LOG_E("%s device prase \"AT+MIPDNS=\"%s\",2\" cmd error.", device->name, name);
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
static void l610_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}


static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    int result;


    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    /* get the current socket by receive data */
    sscanf(data, "+MIPPUSH: %d,%d", &device_socket,&result);


    if (rt_strstr(data, "+MIPPUSH: ")){
        if(result==0)
        {
            l610_socket_event_send(device, SET_EVENT(device_socket, L610_EVENT_SEND_OK));
        }
        else
        {
            l610_socket_event_send(device, SET_EVENT(device_socket, L610_EVENT_SEND_FAIL));
        }
    }

}

static void urc_close_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    int result;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }
    /* get the current socket by receive data */
    sscanf(data, "+MIPCLOSE: %d,%d", &device_socket,&result);

    if(result==0)
    {
        l610_socket_event_send(device, SET_EVENT(device_socket, L610_EVNET_CLOSE_OK));
    }
    else
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


//Receive data from buffer
static void urc_recv_cmd(struct at_client *client, const char *data, rt_size_t size)
{
    int sock = -1;
    rt_size_t bfsz;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;


    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    sscanf(data,"+MIPREAD: %d,%d", &sock, (int *) &bfsz);

    if(bfsz>0)
    {
        if (at_obj_exec_cmd(client, NULL, "AT+MIPREAD=%d,%d", sock,bfsz) < 0)
            {
                LOG_E("send (%s) failed.", client_name);
                return;
            }
    }
}



static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    rt_int32_t timeout;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, temp[8] = {0};
    struct at_socket *socket = RT_NULL;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;
    int sock = -1;
    rt_size_t data_len;
    RT_ASSERT(data && size);


    /* get the current socket and receive buffer size by receive data */
    sscanf(data,"+MIPDATA: %d,%d", &sock, (int *) &bfsz);

    device_socket = l610_get_socket_idx(sock);

    if (device_socket < 0)
    {
        return;
    }

    /* set receive timeout by receive buffer length, not less than 10 ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
    {
        return;
    }

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

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
    data_len=at_client_obj_recv(client, recv_buf, bfsz, timeout);
    if (data_len != bfsz)
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


/*  l610 device URC table for the socket data */
static const struct at_urc urc_table[] =
{

    {"+MIPPUSH: ",      "\r\n",             urc_send_func},
    {"+MIPCLOSE: ",     "\r\n",             urc_close_func},
    {"+MIPREAD: ",      "\r\n",             urc_recv_cmd},
    {"+MIPDATA: ",      "\r\n",             urc_recv_func},

};

static const struct at_socket_ops l610_socket_ops =
{
    l610_socket_connect,
    l610_socket_close,
    l610_socket_send,
    l610_domain_resolve,
    l610_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int l610_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int l610_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_L610_SOCKETS_NUM;
    class->socket_ops = &l610_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_L610 && AT_USING_SOCKET */
