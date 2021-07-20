/*
 * File      : at_device_m26.c
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
 * 2019-05-12     chenyong     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_m26.h>

#define LOG_TAG                        "at.dev.m26"
#include <at_log.h>

#ifdef AT_DEVICE_USING_M26

#define M26_WAIT_CONNECT_TIME          5000
#define M26_THREAD_STACK_SIZE          2048
#define M26_THREAD_PRIORITY            (RT_THREAD_PRIORITY_MAX/2)

static void m26_power_on(struct at_device *device)
{
    struct at_device_m26 *m26 = RT_NULL;

    m26 = (struct at_device_m26 *) device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (m26->power_pin == -1 || m26->power_status_pin == -1)
    {
        return;
    }

    if (rt_pin_read(m26->power_status_pin) == PIN_HIGH)
    {
        return;
    }
    rt_pin_write(m26->power_pin, PIN_HIGH);

    while (rt_pin_read(m26->power_status_pin) == PIN_LOW)
    {
        rt_thread_mdelay(10);
    }
    rt_pin_write(m26->power_pin, PIN_LOW);
}

static void m26_power_off(struct at_device *device)
{
    struct at_device_m26 *m26 = RT_NULL;

    m26 = (struct at_device_m26 *) device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (m26->power_pin == -1 || m26->power_status_pin == -1)
    {
        return;
    }

    if (rt_pin_read(m26->power_status_pin) == PIN_LOW)
    {
        return;
    }
    rt_pin_write(m26->power_pin, PIN_HIGH);

    while (rt_pin_read(m26->power_status_pin) == PIN_HIGH)
    {
        rt_thread_mdelay(10);
    }
    rt_pin_write(m26->power_pin, PIN_LOW);
}

/* =============================  m26 network interface operations ============================= */

/* set m26 network interface device status and address information */
static int m26_netdev_set_info(struct netdev *netdev)
{
#define M26_IMEI_RESP_SIZE      32
#define M26_IPADDR_RESP_SIZE    32
#define M26_DNS_RESP_SIZE       96
#define M26_INFO_RESP_TIMO      rt_tick_from_millisecond(300)

    int result = RT_EOK;
    ip_addr_t addr;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    struct at_client *client = RT_NULL;

    RT_ASSERT(netdev);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }
    client = device->client;

    /* set network interface device up status */
    netdev_low_level_set_status(netdev, RT_TRUE);
    netdev_low_level_set_dhcp_status(netdev, RT_TRUE);

    resp = at_create_resp(M26_IMEI_RESP_SIZE, 0, M26_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define M26_NETDEV_HWADDR_LEN   8
        #define M26_IMEI_LEN            15

        char imei[M26_IMEI_LEN] = {0};
        int i = 0, j = 0;

        /* send "AT+GSN" commond to get device IMEI */
        if (at_obj_exec_cmd(client, resp, "AT+GSN") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args(resp, 2, "%s", imei) <= 0)
        {
            LOG_E("%s device prase \"AT+GSN\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("%s device IMEI number: %s", device->name, imei);

        netdev->hwaddr_len = M26_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < M26_NETDEV_HWADDR_LEN && j < M26_IMEI_LEN; i++, j+=2)
        {
            if (j != M26_IMEI_LEN - 1)
            {
                netdev->hwaddr[i] = (imei[j] - '0') * 10 + (imei[j + 1] - '0');
            }
            else
            {
                netdev->hwaddr[i] = (imei[j] - '0');
            }
        }
    }

    /* set network interface device IP address */
    {
        #define IP_ADDR_SIZE_MAX    16
        char ipaddr[IP_ADDR_SIZE_MAX] = {0};

        at_resp_set_info(resp, M26_IPADDR_RESP_SIZE, 2, M26_INFO_RESP_TIMO);

        /* send "AT+QILOCIP" commond to get IP address */
        if (at_obj_exec_cmd(client, resp, "AT+QILOCIP") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, ".", "%s", ipaddr) <= 0)
        {
            LOG_E("%s device prase \"AT+QILOCIP\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("%s device IP address: %s", device->name, ipaddr);

        /* set network interface address information */
        inet_aton(ipaddr, &addr);
        netdev_low_level_set_ipaddr(netdev, &addr);
    }

    /* set network interface device dns server */
    {
        #define DNS_ADDR_SIZE_MAX   16
        char dns_server1[DNS_ADDR_SIZE_MAX] = {0}, dns_server2[DNS_ADDR_SIZE_MAX] = {0};

        at_resp_set_info(resp, M26_DNS_RESP_SIZE, 0, M26_INFO_RESP_TIMO);

        /* send "AT+QIDNSCFG?" commond to get DNS servers address */
        if (at_obj_exec_cmd(client, resp, "AT+QIDNSCFG?") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, "PrimaryDns:", "PrimaryDns:%s", dns_server1) <= 0 ||
                at_resp_parse_line_args_by_kw(resp, "SecondaryDns:", "SecondaryDns:%s", dns_server2) <= 0)
        {
            LOG_E("%s device prase \"AT+QIDNSCFG?\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("%s device primary DNS server address: %s", device->name, dns_server1);
        LOG_D("%s device secondary DNS server address: %s", device->name, dns_server2);

        inet_aton(dns_server1, &addr);
        netdev_low_level_set_dns_server(netdev, 0, &addr);

        inet_aton(dns_server2, &addr);
        netdev_low_level_set_dns_server(netdev, 1, &addr);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static void check_link_status_entry(void *parameter)
{
#define M26_LINK_STATUS_OK   0
#define M26_LINK_RESP_SIZE   64
#define M26_LINK_RESP_TIMO   (3 * RT_TICK_PER_SECOND)
#define M26_LINK_DELAY_TIME  (30 * RT_TICK_PER_SECOND)

    struct netdev *netdev = (struct netdev *)parameter;
    struct at_device *device = RT_NULL;
    at_response_t resp = RT_NULL;
    int link_status;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    resp = at_create_resp(M26_LINK_RESP_SIZE, 0, M26_LINK_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

   while (1)
    {

        /* send "AT+QNSTATUS" commond  to check netweork interface device link status */
        if (at_obj_exec_cmd(device->client, resp, "AT+QNSTATUS") < 0)
        {
            rt_thread_mdelay(M26_LINK_DELAY_TIME);

           continue;
        }

        link_status = -1;
        at_resp_parse_line_args_by_kw(resp, "+QNSTATUS:", "+QNSTATUS: %d", &link_status);

        /* check the network interface device link status  */
        if ((M26_LINK_STATUS_OK == link_status) != netdev_is_link_up(netdev))
        {
            netdev_low_level_set_link_status(netdev, (M26_LINK_STATUS_OK == link_status));
        }

        rt_thread_mdelay(M26_LINK_DELAY_TIME);
    }
}

static int m26_netdev_check_link_status(struct netdev *netdev)
{
#define M26_LINK_THREAD_TICK           20
#define M26_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define M26_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    tid = rt_thread_create(tname, check_link_status_entry, (void *)netdev,
                           M26_LINK_THREAD_STACK_SIZE, M26_LINK_THREAD_PRIORITY, M26_LINK_THREAD_TICK);
    if (tid)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int m26_net_init(struct at_device *device);

static int m26_netdev_set_up(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    if (device->is_init == RT_FALSE)
    {
        m26_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int m26_netdev_set_down(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    if (device->is_init == RT_TRUE)
    {
        m26_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int m26_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define M26_DNS_RESP_LEN    8
#define M26_DNS_RESP_TIMEO   rt_tick_from_millisecond(300)

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(dns_server);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return - RT_ERROR;
    }

    resp = at_create_resp(M26_DNS_RESP_LEN, 0, M26_DNS_RESP_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* send "AT+QIDNSCFG=<pri_dns>[,<sec_dns>]" commond to set dns servers */
    if (at_exec_cmd(resp, "AT+QIDNSCFG=\"%s\"", inet_ntoa(*dns_server)) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    netdev_low_level_set_dns_server(netdev, dns_num, dns_server);

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}

#ifdef NETDEV_USING_PING
static int m26_netdev_ping(struct netdev *netdev, const char *host,
            size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define M26_PING_RESP_SIZE       128
#define M26_PING_IP_SIZE         16
#define M26_PING_TIMEO           (5 * RT_TICK_PER_SECOND)

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    char ip_addr[M26_PING_IP_SIZE] = {0};
    int response, recv_data_len, time, ttl;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return - RT_ERROR;
    }

    resp = at_create_resp(M26_PING_RESP_SIZE, 5, M26_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return  -RT_ENOMEM;
    }
    /* send "AT+QPING="<host>"[,[<timeout>][,<pingnum>]]" commond to send ping request */
    if (at_obj_exec_cmd(device->client, resp, "AT+QPING=\"%s\",%d,1", host, M26_PING_TIMEO / RT_TICK_PER_SECOND) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    at_resp_parse_line_args_by_kw(resp, "+QPING:","+QPING:%d", &response);
    /* Received the ping response from the server */
    if (response == 0)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+QPING:", "+QPING:%d,%[^,],%d,%d,%d",
                                          &response, ip_addr, &recv_data_len, &time, &ttl) <= 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }
    }

    /* prase response number */
    switch (response)
    {
    case 0:
        inet_aton(ip_addr, &(ping_resp->ip_addr));
        ping_resp->data_len = recv_data_len;
        ping_resp->ticks = time;
        ping_resp->ttl = ttl;
        result = RT_EOK;
        break;
    case 1:
        result = -RT_ETIMEOUT;
        break;
    default:
        result = -RT_ERROR;
        break;
    }

 __exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

const struct netdev_ops m26_netdev_ops =
{
    m26_netdev_set_up,
    m26_netdev_set_down,

    RT_NULL, /* not support set ip, netmask, gatway address */
    m26_netdev_set_dns_server,
    RT_NULL, /* not support set DHCP status */

#ifdef NETDEV_USING_PING
    m26_netdev_ping,
#endif
    RT_NULL,
};

static struct netdev *m26_netdev_add(const char *netdev_name)
{
#define M26_NETDEV_MTU       1500

    struct netdev *netdev = RT_NULL;

    RT_ASSERT(netdev_name);

    netdev = netdev_get_by_name(netdev_name);
    if (netdev != RT_NULL)
    {
        return (netdev);
    }

    netdev = (struct netdev *) rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for netdev ceate.");
        return RT_NULL;
    }

    netdev->mtu = M26_NETDEV_MTU;
    netdev->ops = &m26_netdev_ops;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  m26 device operations ============================= */

#define AT_SEND_CMD(client, resp, resp_line, timeout, cmd)                                         \
    do {                                                                                           \
        (resp) = at_resp_set_info((resp), 128, (resp_line), rt_tick_from_millisecond(timeout));    \
        if (at_obj_exec_cmd((client),(resp), (cmd)) < 0)                                           \
        {                                                                                          \
            result = -RT_ERROR;                                                                    \
            goto __exit;                                                                           \
        }                                                                                          \
    } while(0);                                                                                    \

/* init for m26 or mc20 */
static void m26_init_thread_entry(void *parameter)
{
#define INIT_RETRY                     5
#define CPIN_RETRY                     10
#define CSQ_RETRY                      10
#define CREG_RETRY                     10
#define CGREG_RETRY                    20

    at_response_t resp = RT_NULL;
    int i, qimux, qimode;
    int retry_num = INIT_RETRY;
    char parsed_data[10];
    rt_err_t result = RT_EOK;
    struct at_device *device = (struct at_device *)parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    LOG_D("start init m26/mc20 device(%s)", device->name);

    while (retry_num--)
    {
        /* power on the m26 device */
        m26_power_on(device);
        rt_thread_mdelay(1000);

        /* wait m26|mc20 startup finish */
        if (at_client_obj_wait_connect(client, M26_WAIT_CONNECT_TIME))
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* disable echo */
        AT_SEND_CMD(client, resp, 0, 300, "ATE0");
        /* get module version */
        AT_SEND_CMD(client, resp, 0, 300, "ATI");
        /* show module version */
        for (i = 0; i < (int) resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }
        /* check SIM card */
        for (i = 0; i < CPIN_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 2, 5 * RT_TICK_PER_SECOND, "AT+CPIN?");

            if (at_resp_get_line_by_kw(resp, "READY"))
            {
                LOG_D("%s device SIM card detection success.", device->name);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CPIN_RETRY)
        {
            LOG_E("SIM card detection failed!");
            result = -RT_ERROR;
            goto __exit;
        }
        /* waiting for dirty data to be digested */
        rt_thread_mdelay(10);
        /* check signal strength */
        for (i = 0; i < CSQ_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CSQ");
            at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %s", &parsed_data);
            if (rt_strncmp(parsed_data, "99,99", sizeof(parsed_data)))
            {
                LOG_D("%s device signal strength: %s", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CSQ_RETRY)
        {
            LOG_E("%s device signal strength check failed(%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }
        /* check the GSM network is registered */
        for (i = 0; i < CREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CREG?");
            at_resp_parse_line_args_by_kw(resp, "+CREG:", "+CREG: %s", &parsed_data);
            if (!rt_strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                    !rt_strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {
                LOG_D("%s device GSM is registered(%s).", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CREG_RETRY)
        {
            LOG_E("%s device GSM is register failed(%s)", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }
        /* check the GPRS network is registered */
        for (i = 0; i < CGREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CGREG?");
            at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %s", &parsed_data);
            if (!rt_strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                    !rt_strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {
                LOG_D("%s device GPRS is registered(%s).", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CGREG_RETRY)
        {
            LOG_E("%s device GPRS is register failed(%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

        AT_SEND_CMD(client, resp, 0, 300, "AT+QIFGCNT=0");
        AT_SEND_CMD(client, resp, 0, 300, "AT+QICSGP=1, \"CMNET\"");
        AT_SEND_CMD(client, resp, 0, 300, "AT+QIMODE?");

        at_resp_parse_line_args_by_kw(resp, "+QIMODE:", "+QIMODE: %d", &qimode);
        if (qimode == 1)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+QIMODE=0");
        }

        /* the device default response timeout is 40 seconds, but it set to 15 seconds is convenient to use. */
        AT_SEND_CMD(client, resp, 2, 20 * 1000, "AT+QIDEACT");

        /* Set to multiple connections */
        AT_SEND_CMD(client, resp, 0, 300, "AT+QIMUX?");
        at_resp_parse_line_args_by_kw(resp, "+QIMUX:", "+QIMUX: %d", &qimux);
        if (qimux == 0)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+QIMUX=1");
        }

        AT_SEND_CMD(client, resp, 0, 300, "AT+QIREGAPP");

        /* the device default response timeout is 150 seconds, but it set to 20 seconds is convenient to use. */
        AT_SEND_CMD(client, resp, 0, 20 * 1000, "AT+QIACT");

        AT_SEND_CMD(client, resp, 2, 300, "AT+QILOCIP");

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            /* power off the m26 device */
            m26_power_off(device);
            rt_thread_mdelay(1000);

            LOG_I("%s device initialize retry...", device->name);
        }
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    if (result == RT_EOK)
    {
        m26_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            m26_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success.", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
}

static int m26_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_M26_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("m26_net", m26_init_thread_entry, (void *)device,
            M26_THREAD_STACK_SIZE, M26_THREAD_PRIORITY, 20);
    if (tid)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create %s device init thread failed.", device->name);
        return -RT_ERROR;
    }
#else
    m26_init_thread_entry(device);
#endif /* AT_DEVICE_M26_INIT_ASYN */

    return RT_EOK;
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data);

    LOG_I("URC data : %.*s", size, data);
}

static const struct at_urc urc_table[] = {
        {"RING",        "\r\n",                 urc_func},
        {"Call Ready",  "\r\n",                 urc_func},
        {"RDY",         "\r\n",                 urc_func},
        {"NO CARRIER",  "\r\n",                 urc_func},
};

static int m26_init(struct at_device *device)
{
    struct at_device_m26 *m26 = (struct at_device_m26 *) device->user_data;

    /* initialize AT client */
    at_client_init(m26->client_name, m26->recv_line_num);

    device->client = at_client_get(m26->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", m26->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#ifdef AT_USING_SOCKET
    m26_socket_init(device);
#endif

    /* add m26 netdev to the netdev list */
    device->netdev = m26_netdev_add(m26->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("get netdev(%s) failed.", m26->device_name);
        return -RT_ERROR;
    }

    /* initialize m26 pin configuration */
    if (m26->power_pin != -1 && m26->power_status_pin != -1)
    {
        rt_pin_mode(m26->power_pin, PIN_MODE_OUTPUT);
        rt_pin_mode(m26->power_status_pin, PIN_MODE_INPUT);
    }

    /* initialize m26 device network */
    return m26_netdev_set_up(device->netdev);
}

static int m26_deinit(struct at_device *device)
{
    return m26_netdev_set_down(device->netdev);
}

static int m26_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_POWER_ON:
    case AT_DEVICE_CTRL_POWER_OFF:
    case AT_DEVICE_CTRL_RESET:
    case AT_DEVICE_CTRL_LOW_POWER:
    case AT_DEVICE_CTRL_SLEEP:
    case AT_DEVICE_CTRL_WAKEUP:
    case AT_DEVICE_CTRL_NET_CONN:
    case AT_DEVICE_CTRL_NET_DISCONN:
    case AT_DEVICE_CTRL_SET_WIFI_INFO:
    case AT_DEVICE_CTRL_GET_SIGNAL:
    case AT_DEVICE_CTRL_GET_GPS:
    case AT_DEVICE_CTRL_GET_VER:
        LOG_W("not support the control command(%d).", cmd);
        break;
    default:
        LOG_E("input error control command(%d).", cmd);
        break;
    }

    return result;
}
static const struct at_device_ops m26_device_ops =
{
    m26_init,
    m26_deinit,
    m26_control,
};

static int m26_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill m26 device class object */
#ifdef AT_USING_SOCKET
    m26_socket_class_register(class);
#endif
    class->device_ops = &m26_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_M26_MC20);
}
INIT_DEVICE_EXPORT(m26_device_class_register);

#endif /* AT_DEVICE_USING_M26 */
