/*
 * File      : at_socket_m5311.c
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
 * 2020-03-17     LXGMAX       the first version
 * 2020-07-12     LXGMAX       fix parameters and remove redundant content
 * 2020-07-31     LXGMAX       fix a bug of connect function
 */

#include <stdio.h>
#include <string.h>

#include <at_device_m5311.h>

#define LOG_TAG                        "at.skt.m5311"
#include <at_log.h>
/* socket require increase AT_CMD_MAX_LEN and RT_SERIAL_RB_BUFSZ */

#if defined(AT_DEVICE_USING_M5311) && defined(AT_USING_SOCKET)

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define M5311_EVENT_CONN_OK              (1L << 0)
#define M5311_EVENT_SEND_OK              (1L << 1)
#define M5311_EVENT_RECV_OK              (1L << 2)
#define M5311_EVNET_CLOSE_OK             (1L << 3)
#define M5311_EVENT_CONN_FAIL            (1L << 4)
#define M5311_EVENT_SEND_FAIL            (1L << 5)

/**
 * convert data from string to ASCII string.
 * @param source
 * @param dest
 * @param max_dest_len
 * @return
 */
static int str_to_hex(const char *source, char *dest, int max_dest_len)
{
    int i = 0, j = 0;
    char ch1, ch2;

    while (j + 1 < max_dest_len)
    {
        ch1 = (source[i] & 0xF0) >> 4;
        ch2 = source[i] & 0x0F;

        if(ch1 <= 9)
            *(dest + j) = ch1 + '0';
        else
            *(dest + j) = ch1 + 'A' - 10;

        if(ch2 <= 9)
            *(dest + j + 1) = ch2 + '0';
        else
            *(dest + j + 1) = ch2 + 'A' - 10;

        i++;
        j += 2;
    }

    *(dest + j) = '\0';

    return j;
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

static at_evt_cb_t at_evt_cb_set[] =
{
    [AT_SOCKET_EVT_RECV]    = NULL,
    [AT_SOCKET_EVT_CLOSED]  = NULL,
};

static struct at_socket_ip_info
{
    char ip_addr[16];
    int  port;
} m5311_sock_info[AT_DEVICE_M5311_SOCKETS_NUM];

static int m5311_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int) rt_event_send(device->socket_event, event);
}

static int m5311_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
{
    int result = 0;
    rt_uint32_t recved = 0;

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
static int m5311_socket_close(struct at_socket *socket)
{
    int result = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device  = (struct at_device *) socket->device;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(500));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.", device->name);
        return -RT_ENOMEM;
    }

    /* clear socket close event */
    m5311_socket_event_recv(device, SET_EVENT(device_socket, M5311_EVNET_CLOSE_OK), 0, RT_EVENT_FLAG_OR);

    result = at_obj_exec_cmd(device->client, resp, "AT+IPCLOSE=%d", device_socket);
    if (result == 0)
    {
        LOG_I("%s device close socket(%d).", device->name, device_socket);
    }
    else
    {
        LOG_E("%s device socket(%d) close failed, wait close OK timeout.", device->name, device_socket);
    }

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
static int m5311_socket_connect(struct at_socket *socket, char *ip, int32_t port, enum at_socket_type type, rt_bool_t is_client)
{
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int result = 0, event_result = 0;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(128, 0, 3 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    RT_ASSERT(ip);
    RT_ASSERT(port >= 0);

    if (!is_client)
        return -RT_ERROR;

    if (type == AT_SOCKET_UDP)
    {
        rt_strncpy(m5311_sock_info[device_socket].ip_addr, ip, 16);
        m5311_sock_info[device_socket].port = port;
    }

__retry:
    /* clear socket connect event */
    event_result = SET_EVENT(device_socket, M5311_EVENT_CONN_OK | M5311_EVENT_CONN_FAIL);
    m5311_socket_event_recv(device, event_result, 0, RT_EVENT_FLAG_OR);

    switch (type)
    {
    case AT_SOCKET_TCP:
        resp = at_resp_set_info(resp, 128, 3, 10 * RT_TICK_PER_SECOND);
        /* send AT commands(eg: AT+IPSTART=0,"TCP","x.x.x.x", 1234) to connect TCP server */
        /* AT+IPSTART=<sockid>,<type>,<addr>,<port>[,<cid>[,<domian>[,<protocol>]]] */
        if (at_obj_exec_cmd(device->client, resp,
                "AT+IPSTART=%d,\"TCP\",\"%s\",%d", device_socket, ip, port) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args(resp, 3, "CONNECT OK") < 0)
        {
            result = -RT_ERROR;
        }
        break;

    case AT_SOCKET_UDP:
        if (at_obj_exec_cmd(device->client, resp,
                "AT+IPSTART=%d,\"UDP\",\"%s\",%d", device_socket, ip, port) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }
        break;

    default:
        LOG_E("%s device not supported connect type : %d.", device->name, type);
        return -RT_ERROR;
    }

    if(!at_resp_get_line_by_kw(resp, "OK"))
    {
        result = -RT_ERROR;
    }

    if (result != RT_EOK && retryed == RT_FALSE)
    {
        LOG_D("%s device socket(%d) connect failed, now retry.",
              device->name, device_socket);
        if (m5311_socket_close(socket) < 0)
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
static int m5311_socket_send(struct at_socket *socket, const char *buff,
                            size_t bfsz, enum at_socket_type type)
{
    uint32_t event = 0;
    int result = 0, event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
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
    event = SET_EVENT(device_socket, M5311_EVENT_SEND_OK | M5311_EVENT_SEND_FAIL);
    m5311_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    /* only use for UDP socket */
    const char *ip = m5311_sock_info[device_socket].ip_addr;
    const int port = m5311_sock_info[device_socket].port;

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < M5311_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = M5311_MODULE_SEND_MAX_SIZE;
        }

        char hex_data[bfsz * 2];
        rt_memset(hex_data, 0, sizeof(hex_data));
        str_to_hex(buff, hex_data, bfsz * 2);

        switch (type)
        {
        case AT_SOCKET_TCP:
            /* TCP : AT+IPSEND=<socket_id>,[<data_len>],<data>[,<pri_flag>] */
            if (at_obj_exec_cmd(device->client, resp, "AT+IPSEND=%d,%d,%s",
                                device_socket, (int)cur_pkt_size, hex_data) < 0)
            {
                LOG_D("%s", buff);
                result = -RT_ERROR;
                goto __exit;
            }
            LOG_D("%s device TCP socket(%d) send %d bytes.\n>> %s", device->name, device_socket, (int)cur_pkt_size, buff);
            break;

        case AT_SOCKET_UDP:
            /* UDP : AT+IPSEND=<socket_id>,[<data_len>],<data>[,<addr>,<port>[,<pri_flag>]] */
            if (at_obj_exec_cmd(device->client, resp, "AT+IPSEND=%d,%d,\"%s\",%s,%d,1",
                                device_socket, (int)cur_pkt_size, hex_data, ip, port) < 0)
            {

                result = -RT_ERROR;
                goto __exit;
            }
            LOG_D("%s device UDP socket(%d) send %d bytes to %s:%d.", device->name, device_socket, (int)cur_pkt_size, ip, port);
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

        /* waiting result event from AT URC, the device default timeout is 60 seconds*/
        if (m5311_socket_event_recv(device, SET_EVENT(device_socket, 0),
                                   60 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("%s device socket(%d) wait send result timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = m5311_socket_event_recv(device, M5311_EVENT_SEND_OK | M5311_EVENT_SEND_FAIL,
                                              1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result & M5311_EVENT_SEND_FAIL)
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
        rt_memset(hex_data, 0, bfsz * 2);
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
static int m5311_domain_resolve(const char *name, char ip[16])
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
        LOG_E("get first init device failed.");
        return -RT_ERROR;
    }

    /* The maximum response time is 3 seconds, affected by network status */
    resp = at_create_resp(256, 4, 3 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    for(i = 0; i < RESOLVE_RETRY; i++)
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+CMDNS=\"%s\"", name) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* get the IP address */
        if(at_resp_parse_line_args_by_kw(resp, "+CMDNS: ", "+CMDNS: %[^\r]", recv_ip) < 0)
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
            rt_strncpy(ip, recv_ip, 16);
            ip[15] = '\0';
            LOG_D("domain:%s,IP address:%s", name, ip);
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
static void m5311_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
{
    if (event < sizeof(at_evt_cb_set) / sizeof(at_evt_cb_set[1]))
    {
        at_evt_cb_set[event] = cb;
    }
}

static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0, data_size = 0;
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    sscanf(data, "+IPSEND: %d,%d", &device_socket, &data_size);

    if (data_size > 0)
    {
        m5311_socket_event_send(device, SET_EVENT(device_socket, M5311_EVENT_SEND_OK));
    }
    else
    {
        m5311_socket_event_send(device, SET_EVENT(device_socket, M5311_EVENT_SEND_FAIL));
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

    sscanf(data, "+IPCLOSE: %d", &device_socket);
    socket = &(device->sockets[device_socket]);

    if (at_evt_cb_set[AT_SOCKET_EVT_CLOSED])
    {
        at_evt_cb_set[AT_SOCKET_EVT_CLOSED](socket, AT_SOCKET_EVT_CLOSED, RT_NULL, 0);
    }
}

static void urc_recv_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    rt_int32_t timeout;
    rt_size_t bfsz = 0, temp_size = 0;
    char *recv_buf = RT_NULL, *hex_buf = RT_NULL, temp[8] = {0};
    char remote_addr[16] = {0};
    int remote_port = -1;

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

    hex_buf = (char *) rt_calloc(1, M5311_MODULE_RECV_MAX_SIZE * 2 + 1);

    /* get the current socket and receive buffer size by receive data */
    /* mode 2 => +IPRD: <socket>,<remote_addr>, <remote_port>,<length>,<data> */
    sscanf(data, "+IPRD: %d,\"%[0-9.]\",%d,%d,%s", &device_socket, remote_addr, &remote_port, (int *) &bfsz, hex_buf);

    /* set receive timeout by receive buffer length, not less than 10 ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
        return;

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

static const struct at_urc urc_table[] =
{
    {"+IPSEND: ",    "\r\n",                 urc_send_func},
    {"+IPCLOSE: ",   "\r\n",                 urc_close_func},
    {"+IPRD: ",      "\r\n",                 urc_recv_func},
};

static const struct at_socket_ops m5311_socket_ops =
{
    m5311_socket_connect,
    m5311_socket_close,
    m5311_socket_send,
    m5311_domain_resolve,
    m5311_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int m5311_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int m5311_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_M5311_SOCKETS_NUM;
    class->socket_ops = &m5311_socket_ops;

    return RT_EOK;
}

#endif /* AT_DEVICE_USING_M5311 && AT_USING_SOCKET */
