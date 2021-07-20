/*
 * File      : at_socket_ec20.c
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
 * 2018-06-12     chenyong     first version
 * 2018-08-12     Marcus       port to ec20
 * 2019-05-13     chenyong     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_ec20.h>

#define LOG_TAG                        "at.skt.ec20"
#include <at_log.h>

#if defined(AT_DEVICE_USING_EC20) && defined(AT_USING_SOCKET)

#define EC20_MODULE_SEND_MAX_SIZE       1460

/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define EC20_EVENT_CONN_OK             (1L << 0)
#define EC20_EVENT_SEND_OK             (1L << 1)
#define EC20_EVENT_RECV_OK             (1L << 2)
#define EC20_EVNET_CLOSE_OK            (1L << 3)
#define EC20_EVENT_CONN_FAIL           (1L << 4)
#define EC20_EVENT_SEND_FAIL           (1L << 5)
#define EC20_EVENT_DOMAIN_OK           (1L << 6)

static at_evt_cb_t at_evt_cb_set[] = {
        [AT_SOCKET_EVT_RECV] = NULL,
        [AT_SOCKET_EVT_CLOSED] = NULL,
};

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

#ifdef EC20_USING_HTTP
static void at_http_errcode_parse(int result)//HTTP
{
    switch(result)
    {
    case 0   : LOG_D("%d : Operation successful",         result); break;
    case 701 : LOG_E("%d : HTTP(S) unknown error",        result); break;
    case 702 : LOG_E("%d : HTTP(S) timeout",              result); break;
    case 703 : LOG_E("%d : HTTP(S) busy",                 result); break;
    case 704 : LOG_E("%d : HTTP(S) UART busy",            result); break;
    case 705 : LOG_E("%d : HTTP(S) no GET/POST requests", result); break;
    case 706 : LOG_E("%d : HTTP(S) network busy",         result); break;
    case 707 : LOG_E("%d : HTTP(S) network open failed",  result); break;
    case 708 : LOG_E("%d : HTTP(S) network no configuration", result); break;
    case 709 : LOG_E("%d : HTTP(S) network deactivated",  result); break;
    case 710 : LOG_E("%d : HTTP(S) network error",        result); break;
    case 711 : LOG_E("%d : HTTP(S) URL error",            result); break;
    case 712 : LOG_E("%d : HTTP(S) empty URL",            result); break;
    case 713 : LOG_E("%d : HTTP(S) IP address error",     result); break;
    case 714 : LOG_E("%d : HTTP(S) DNS error",            result); break;
    case 715 : LOG_E("%d : HTTP(S) socket create error",  result); break;
    case 716 : LOG_E("%d : HTTP(S) socket connect error", result); break;
    case 717 : LOG_E("%d : HTTP(S) socket read error",    result); break;
    case 718 : LOG_E("%d : HTTP(S) socket write error",   result); break;
    case 719 : LOG_E("%d : HTTP(S) socket closed",        result); break;
    case 720 : LOG_E("%d : HTTP(S) data encode error",    result); break;
    case 721 : LOG_E("%d : HTTP(S) data decode error",    result); break;
    case 722 : LOG_E("%d : HTTP(S) read timeout",         result); break;
    case 723 : LOG_E("%d : HTTP(S) response failed",      result); break;
    case 724 : LOG_E("%d : Incoming call busy",           result); break;
    case 725 : LOG_E("%d : Voice call busy",              result); break;
    case 726 : LOG_E("%d : Input timeout",                result); break;
    case 727 : LOG_E("%d : Wait data timeout",            result); break;
    case 728 : LOG_E("%d : Wait HTTP(S) response timeout", result); break;
    case 729 : LOG_E("%d : Memory allocation failed",     result); break;
    case 730 : LOG_E("%d : Invalid parameter",            result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}

static void at_http_rsponsecode_parse(int result)//HTTP
{
    switch(result)
    {
    case 200 : LOG_D("%d : OK",                           result); break;
    case 400 : LOG_E("%d : Bad request",                  result); break;
    case 403 : LOG_E("%d : Forbidden",                    result); break;
    case 404 : LOG_E("%d : Not found",                    result); break;
    case 409 : LOG_E("%d : Conflict",                     result); break;
    case 411 : LOG_E("%d : Length required",              result); break;
    case 500 : LOG_E("%d : Internal server error",        result); break;
    case 502 : LOG_E("%d : Bad gate way",                 result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}
#endif /* EC20_USING_HTTP */

#ifdef EC20_USING_FTP
static void at_ftp_errcode_parse(int result)//FTP
{
    switch(result)
    {
    case 0   : LOG_D("%d : Operation successful",         result); break;
    case 601 : LOG_E("%d : Unknown error",                result); break;
    case 602 : LOG_E("%d : FTP(S) server blocked",        result); break;
    case 603 : LOG_E("%d : FTP(S) server busy",           result); break;
    case 604 : LOG_E("%d : DNS parse failed",             result); break;
    case 605 : LOG_E("%d : Network error",                result); break;
    case 606 : LOG_E("%d : Control connection closed.",   result); break;
    case 607 : LOG_E("%d : Data connection closed",       result); break;
    case 608 : LOG_E("%d : Socket closed by peer",        result); break;
    case 609 : LOG_E("%d : Timeout error",                result); break;
    case 610 : LOG_E("%d : Invalid parameter",            result); break;
    case 611 : LOG_E("%d : Failed to open file",          result); break;
    case 612 : LOG_E("%d : File position invalid",        result); break;
    case 613 : LOG_E("%d : File error",                   result); break;
    case 614 : LOG_E("%d : Service not available, closing control connection", result); break;
    case 615 : LOG_E("%d : Open data connection failed",  result); break;
    case 616 : LOG_E("%d : Connection closed; transfer aborted", result); break;
    case 617 : LOG_E("%d : Requested file action not taken", result); break;
    case 618 : LOG_E("%d : Requested action aborted: local error in processing", result); break;
    case 619 : LOG_E("%d : Requested action not taken: insufficient system storage", result); break;
    case 620 : LOG_E("%d : Syntax error, command unrecognized", result); break;
    case 621 : LOG_E("%d : Syntax error in parameters or arguments", result); break;
    case 622 : LOG_E("%d : Command not implemented",      result); break;
    case 623 : LOG_E("%d : Bad sequence of commands",     result); break;
    case 624 : LOG_E("%d : Command parameter not implemented", result); break;
    case 625 : LOG_E("%d : Not logged in",                result); break;
    case 626 : LOG_E("%d : Need account for storing files", result); break;
    case 627 : LOG_E("%d : Requested action not taken",   result); break;
    case 628 : LOG_E("%d : Requested action aborted: page type unknown", result); break;
    case 629 : LOG_E("%d : Requested file action aborted", result); break;
    case 630 : LOG_E("%d : Requested file name invalid",  result); break;
    case 631 : LOG_E("%d : SSL authentication failed",    result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}

static void at_ftp_protocol_errcode_parse(int result)//FTP_Protocol
{
    switch(result)
    {
    case 421 : LOG_E("%d : Service not available, closing control connection", result); break;
    case 425 : LOG_E("%d : Open data connection failed",  result); break;
    case 426 : LOG_E("%d : Connection closed; transfer aborted", result); break;
    case 450 : LOG_E("%d : Requested file action not taken", result); break;
    case 451 : LOG_E("%d : Requested action aborted: local error in processing", result); break;
    case 452 : LOG_E("%d : Requested action not taken: insufficient system storage", result); break;
    case 500 : LOG_E("%d : Syntax error, command unrecognized", result); break;
    case 501 : LOG_E("%d : Syntax error in parameters or arguments", result); break;
    case 502 : LOG_E("%d : Command not implemented",      result); break;
    case 503 : LOG_E("%d : Bad sequence of commands",     result); break;
    case 504 : LOG_E("%d : Command parameter not implemented", result); break;
    case 530 : LOG_E("%d : Not logged in",                result); break;
    case 532 : LOG_E("%d : Need account for storing files", result); break;
    case 550 : LOG_E("%d : Requested action not taken: file unavailable", result); break;
    case 551 : LOG_E("%d : Requested action aborted: page type unknown", result); break;
    case 552 : LOG_E("%d : Requested file action aborted: exceeded storage allocation", result); break;
    case 553 : LOG_E("%d : Requested action not taken: file name not allowed", result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}
#endif /* EC20_USING_FTP */

#ifdef EC20_USING_SMTP
static void at_smtp_errcode_parse(int result)//Email
{
    switch(result)
    {
    case 651 : LOG_E("%d : Unknown error",                result); break;
    case 652 : LOG_E("%d : The SMTP server is busy, such as uploading the body or sending an email.", result); break;
    case 653 : LOG_E("%d : Failed to get IP address according to the domain name.", result); break;
    case 654 : LOG_E("%d : Network error, such as failed to activate GPRS/CSD context, failed to establish the TCP connection with the SMTP server or failed to send an email to the SMTP server, etc.", result); break;
    case 655 : LOG_E("%d : Unsupported authentication type", result); break;
    case 656 : LOG_E("%d : The connection for the SMTP server is closed by peer.", result); break;
    case 657 : LOG_E("%d : GPRS/CSD context is deactivated.", result); break;
    case 658 : LOG_E("%d : Timeout",                      result); break;
    case 659 : LOG_E("%d : No recipient for the SMTP server", result); break;
    case 660 : LOG_E("%d : Failed to send an email",      result); break;
    case 661 : LOG_E("%d : Failed to open a file",        result); break;
    case 662 : LOG_E("%d : No enough memory for the attachment", result); break;
    case 663 : LOG_E("%d : Failed to save the attachment", result); break;
    case 664 : LOG_E("%d : The input parameter is wrong", result); break;
    case 665 : LOG_E("%d : SSL authentication failed",    result); break;
    case 666 : LOG_E("%d : Service not available, closing transmission channel", result); break;
    case 667 : LOG_E("%d : Requested mail action not taken: mailbox unavailable", result); break;
    case 668 : LOG_E("%d : Requested action aborted: local error in processing", result); break;
    case 669 : LOG_E("%d : Requested action not taken: insufficient system storage", result); break;
    case 670 : LOG_E("%d : Syntax error, command unrecognized", result); break;
    case 671 : LOG_E("%d : Syntax error in parameters or arguments", result); break;
    case 672 : LOG_E("%d : Command not implemented",      result); break;
    case 673 : LOG_E("%d : Bad sequence of commands",     result); break;
    case 674 : LOG_E("%d : Command parameter not implemented", result); break;
    case 675 : LOG_E("%d : <domain> does not accept mail (see RFC1846)", result); break;
    case 676 : LOG_E("%d : Access denied",                result); break;
    case 677 : LOG_E("%d : Authentication failed",        result); break;
    case 678 : LOG_E("%d : Requested action not taken: mailbox unavailable", result); break;
    case 679 : LOG_E("%d : User not local; please try <forward-path>", result); break;
    case 680 : LOG_E("%d : Requested mail action aborted: exceeded storage allocation", result); break;
    case 681 : LOG_E("%d : Requested action not taken: mailbox name not allowed", result); break;
    case 682 : LOG_E("%d : Transaction failed",           result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}

static void at_smtp_protocol_errcode_parse(int result)//Email_Protocol
{
    switch(result)
    {
    case 421 : LOG_E("%d : Service not available, closing transmission channel", result); break;
    case 450 : LOG_E("%d : Requested mail action not taken: mailbox unavailable", result); break;
    case 451 : LOG_E("%d : Requested action aborted: local error in processing", result); break;
    case 452 : LOG_E("%d : Requested action not taken: insufficient system storage", result); break;
    case 500 : LOG_E("%d : Syntax error, command unrecognized", result); break;
    case 501 : LOG_E("%d : Syntax error in parameters or arguments", result); break;
    case 502 : LOG_E("%d : Command not implemented",      result); break;
    case 503 : LOG_E("%d : Bad sequence of commands",     result); break;
    case 504 : LOG_E("%d : Command parameter not implemented", result); break;
    case 521 : LOG_E("%d : <domain> does not accept mail (see RFC1846)", result); break;
    case 530 : LOG_E("%d : Access denied",                result); break;
    case 535 : LOG_E("%d : Authentication failed",        result); break;
    case 550 : LOG_E("%d : Requested action not taken: mailbox unavailable", result); break;
    case 551 : LOG_E("%d : User not local; please try <forward-path>", result); break;
    case 552 : LOG_E("%d : Requested mail action aborted: exceeded storage allocation", result); break;
    case 553 : LOG_E("%d : Requested action not taken: mailbox name not allowed", result); break;
    case 554 : LOG_E("%d : Transaction failed",           result); break;
    default  : LOG_E("%d : Unknown err code",             result); break;
    }
}
#endif /* EC20_USING_SMTP */

static int ec20_socket_event_send(struct at_device *device, uint32_t event)
{
    return (int) rt_event_send(device->socket_event, event);
}

static int ec20_socket_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
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
static int ec20_socket_close(struct at_socket *socket)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(64, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* default connection timeout is 10 seconds, but it set to 1 seconds is convenient to use.*/
    result = at_obj_exec_cmd(device->client, resp, "AT+QICLOSE=%d,1", device_socket);

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
static int ec20_socket_connect(struct at_socket *socket, char *ip, int32_t port,
    enum at_socket_type type, rt_bool_t is_client)
{
    uint32_t event = 0;
    rt_bool_t retryed = RT_FALSE;
    at_response_t resp = RT_NULL;
    int result = 0, event_result = 0;
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
    /* clear socket connect event */
    event = SET_EVENT(device_socket, EC20_EVENT_CONN_OK | EC20_EVENT_CONN_FAIL);
    ec20_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    if (is_client)
    {
        switch (type)
        {
        case AT_SOCKET_TCP:
            /* send AT commands(AT+QIOPEN=<contextID>,<socket>,"<TCP/UDP>","<IP_address>/<domain_name>", */
            /* <remote_port>,<local_port>,<access_mode>) to connect TCP server */
            /* contextID   = 1 : use same contextID as AT+QICSGP & AT+QIACT */
            /* local_port  = 0 : local port assigned automatically */
            /* access_mode = 1 : Direct push mode */
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+QIOPEN=1,%d,\"TCP\",\"%s\",%d,0,1", device_socket, ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;

        case AT_SOCKET_UDP:
            if (at_obj_exec_cmd(device->client, resp,
                                "AT+QIOPEN=1,%d,\"UDP\",\"%s\",%d,0,1", device_socket, ip, port) < 0)
            {
                result = -RT_ERROR;
                goto __exit;
            }
            break;

        default:
            LOG_E("not supported connect type : %d.", type);
            return -RT_ERROR;
        }
    }

    /* waiting result event from AT URC, the device default connection timeout is 75 seconds, but it set to 10 seconds is convenient to use.*/
    if (ec20_socket_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
    {
        LOG_E("%s device socket(%d) wait connect result timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* waiting OK or failed result */
    event_result = ec20_socket_event_recv(device,
        EC20_EVENT_CONN_OK | EC20_EVENT_CONN_FAIL, 1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
    if (event_result < 0)
    {
        LOG_E("%s device socket(%d) wait connect OK|FAIL timeout.", device->name, device_socket);
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    /* check result */
    if (event_result & EC20_EVENT_CONN_FAIL)
    {
        if (retryed == RT_FALSE)
        {
            LOG_D("%s device socket(%d) connect failed, the socket was not be closed and now will connect retey.",
                    device->name, device_socket);
            /* default connection timeout is 10 seconds, but it set to 1 seconds is convenient to use.*/
            if (ec20_socket_close(socket) < 0)
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
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int at_get_send_size(struct at_socket *socket, size_t *size, size_t *acked, size_t *nacked)
{
    int result = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.", device->name);
        result = -RT_ENOMEM;
        goto __exit;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+QISEND=%d,0", device_socket) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+QISEND:", "+QISEND: %d,%d,%d", size, acked, nacked) <= 0)
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

static int at_wait_send_finish(struct at_socket *socket, size_t settings_size)
{
    /* get the timeout by the input data size */
    rt_tick_t timeout = rt_tick_from_millisecond(settings_size);
    rt_tick_t last_time = rt_tick_get();
    size_t size = 0, acked = 0, nacked = 0xFFFF;

    while (rt_tick_get() - last_time <= timeout)
    {
        at_get_send_size(socket, &size, &acked, &nacked);
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
static int ec20_socket_send(struct at_socket *socket, const char *buff, size_t bfsz, enum at_socket_type type)
{
    uint32_t event = 0;
    int result = 0, event_result = 0;
    size_t cur_pkt_size = 0, sent_size = 0;
    at_response_t resp = RT_NULL;
    int device_socket = (int) socket->user_data;
    struct at_device *device = (struct at_device *) socket->device;
    struct at_device_ec20 *ec20 = (struct at_device_ec20 *) device->user_data;
    rt_mutex_t lock = device->client->lock;

    RT_ASSERT(buff);

    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    rt_mutex_take(lock, RT_WAITING_FOREVER);

    /* set current socket for send URC event */
    ec20->user_data = (void *) device_socket;

    /* clear socket send event */
    event = SET_EVENT(device_socket, EC20_EVENT_SEND_OK | EC20_EVENT_SEND_FAIL);
    ec20_socket_event_recv(device, event, 0, RT_EVENT_FLAG_OR);

    /* set AT client end sign to deal with '>' sign.*/
    at_obj_set_end_sign(device->client, '>');

    while (sent_size < bfsz)
    {
        if (bfsz - sent_size < EC20_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = bfsz - sent_size;
        }
        else
        {
            cur_pkt_size = EC20_MODULE_SEND_MAX_SIZE;
        }

        /* send the "AT+QISEND" commands to AT server than receive the '>' response on the first line. */
        if (at_obj_exec_cmd(device->client, resp, "AT+QISEND=%d,%d", device_socket, cur_pkt_size) < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* send the real data to server or client */
        result = (int) at_client_send(buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting result event from AT URC */
        if (ec20_socket_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = ec20_socket_event_recv(device,
            EC20_EVENT_SEND_OK | EC20_EVENT_SEND_FAIL, 1 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            LOG_E("%s device socket(%d) wait sned OK|FAIL timeout.", device->name, device_socket);
            result = -RT_ETIMEOUT;
            goto __exit;
        }
        /* check result */
        if (event_result & EC20_EVENT_SEND_FAIL)
        {
            LOG_E("%s device socket(%d) send failed.", device->name, device_socket);
            result = -RT_ERROR;
            goto __exit;
        }

        if (type == AT_SOCKET_TCP)
        {
            // at_wait_send_finish(socket, cur_pkt_size);
            rt_thread_mdelay(10);
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
static int ec20_domain_resolve(const char *name, char ip[16])
{
#define RESOLVE_RETRY                  3

    int i, result;
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

    /* the maximum response time is 60 seconds, but it set to 10 seconds is convenient to use. */
    resp = at_create_resp(128, 0, 10 * RT_TICK_PER_SECOND);
    if (!resp)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* clear EC20_EVENT_DOMAIN_OK */
    ec20_socket_event_recv(device, EC20_EVENT_DOMAIN_OK, 0, RT_EVENT_FLAG_OR);

    result = at_obj_exec_cmd(device->client, resp, "AT+QIDNSGIP=1,\"%s\"", name);
    if (result < 0)
    {
        goto __exit;
    }

    if (result == RT_EOK)
    {
        for(i = 0; i < RESOLVE_RETRY; i++)
        {
            /* waiting result event from AT URC, the device default connection timeout is 60 seconds.*/
            if (ec20_socket_event_recv(device, EC20_EVENT_DOMAIN_OK, 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
            {
                continue;
            }
            else
            {
                struct at_device_ec20 *ec20 = (struct at_device_ec20 *) device->user_data;
                char *recv_ip = (char *) ec20->socket_data;

                if (rt_strlen(recv_ip) < 8)
                {
                    rt_thread_mdelay(100);
                    /* resolve failed, maybe receive an URC CRLF */
                    result = -RT_ERROR;
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
        }

        /* response timeout */
        if (i == RESOLVE_RETRY)
        {
            result = -RT_ENOMEM;
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
static void ec20_socket_set_event_cb(at_socket_evt_t event, at_evt_cb_t cb)
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

    sscanf(data, "+QIOPEN: %d,%d", &device_socket , &result);

    if (result == 0)
    {
        ec20_socket_event_send(device, SET_EVENT(device_socket, EC20_EVENT_CONN_OK));
    }
    else
    {
        at_tcp_ip_errcode_parse(result);
        ec20_socket_event_send(device, SET_EVENT(device_socket, EC20_EVENT_CONN_FAIL));
    }
}

static void urc_send_func(struct at_client *client, const char *data, rt_size_t size)
{
    int device_socket = 0;
    struct at_device *device = RT_NULL;
    struct at_device_ec20 *ec20 = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }
    ec20 = (struct at_device_ec20 *) device->user_data;
    device_socket = (int) ec20->user_data;

    if (rt_strstr(data, "SEND OK"))
    {
        ec20_socket_event_send(device, SET_EVENT(device_socket, EC20_EVENT_SEND_OK));
    }
    else if (rt_strstr(data, "SEND FAIL"))
    {
        ec20_socket_event_send(device, SET_EVENT(device_socket, EC20_EVENT_SEND_FAIL));
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

    sscanf(data, "+QIURC: \"closed\",%d", &device_socket);
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

    /* get the current socket and receive buffer size by receive data */
    sscanf(data, "+QIURC: \"recv\",%d,%d", &device_socket, (int *) &bfsz);
    /* set receive timeout by receive buffer length, not less than 10 ms */
    timeout = bfsz > 10 ? bfsz : 10;

    if (device_socket < 0 || bfsz == 0)
    {
        return;
    }

    recv_buf = (char *) rt_calloc(1, bfsz);
    if (recv_buf == RT_NULL)
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

static void urc_pdpdeact_func(struct at_client *client, const char *data, rt_size_t size)
{
    int connectID = 0;

    RT_ASSERT(data && size);

    sscanf(data, "+QIURC: \"pdpdeact\",%d", &connectID);

    LOG_E("context (%d) is deactivated.", connectID);
}

static void urc_dnsqip_func(struct at_client *client, const char *data, rt_size_t size)
{
    int i = 0, j = 0;
    char recv_ip[16] = {0};
    int result, ip_count, dns_ttl;
    struct at_device *device = RT_NULL;
    struct at_device_ec20 *ec20 = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }
    ec20 = (struct at_device_ec20 *) device->user_data;

    for (i = 0; i < size; i++)
    {
        if (*(data + i) == '.')
            j++;
    }
    /* There would be several dns result, we just pickup one */
    if (j == 3)
    {
        sscanf(data, "+QIURC: \"dnsgip\",\"%[^\"]", recv_ip);
        recv_ip[15] = '\0';

        /* set ec20 information socket data */
        if (ec20->socket_data == RT_NULL)
        {
            ec20->socket_data = rt_calloc(1, sizeof(recv_ip));
            if (ec20->socket_data == RT_NULL)
            {
                return;
            }
        }
        rt_memcpy(ec20->socket_data, recv_ip, sizeof(recv_ip));


        ec20_socket_event_send(device, EC20_EVENT_DOMAIN_OK);
    }
    else
    {
        sscanf(data, "+QIURC: \"dnsgip\",%d,%d,%d", &result, &ip_count, &dns_ttl);
        if (result)
        {
            at_tcp_ip_errcode_parse(result);
        }
    }
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data);

    LOG_I("URC data : %.*s", size, data);
}

static void urc_qiurc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data && size);

    switch(*(data + 9))
    {
    case 'c' : urc_close_func(client, data, size); break;//+QIURC: "closed"
    case 'r' : urc_recv_func(client, data, size); break;//+QIURC: "recv"
    case 'p' : urc_pdpdeact_func(client, data, size); break;//+QIURC: "pdpdeact"
    case 'd' : urc_dnsqip_func(client, data, size); break;//+QIURC: "dnsgip"
    default  : urc_func(client, data, size);      break;
    }
}

static const struct at_urc urc_table[] =
{
    {"SEND OK",     "\r\n",                 urc_send_func},
    {"SEND FAIL",   "\r\n",                 urc_send_func},
    {"+QIOPEN:",    "\r\n",                 urc_connect_func},
    {"+QIURC:",     "\r\n",                 urc_qiurc_func},
};

static const struct at_socket_ops ec20_socket_ops =
{
    ec20_socket_connect,
    ec20_socket_close,
    ec20_socket_send,
    ec20_domain_resolve,
    ec20_socket_set_event_cb,
#if defined(AT_SW_VERSION_NUM) && AT_SW_VERSION_NUM > 0x10300
    RT_NULL,
#endif
};

int ec20_socket_init(struct at_device *device)
{
    RT_ASSERT(device);

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

    return RT_EOK;
}

int ec20_socket_class_register(struct at_device_class *class)
{
    RT_ASSERT(class);

    class->socket_num = AT_DEVICE_EC20_SOCKETS_NUM;
    class->socket_ops = &ec20_socket_ops;

    return RT_EOK;
}


#endif /* AT_DEVICE_USING_EC20 && AT_USING_SOCKET */
