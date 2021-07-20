/*
 * File      : at_socket_air720.c
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
 * 2019-12-07     liang.shao     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <at_device_air720.h>

#if !defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10300
#error "This AT Client version is older, please check and update latest AT Client!"
#endif

#define LOG_TAG "at.dev"
#include <at_log.h>

#ifdef AT_DEVICE_USING_AIR720

#define AIR720_WAIT_CONNECT_TIME 30000
#define AIR720_THREAD_STACK_SIZE 2048
#define AIR720_THREAD_PRIORITY (RT_THREAD_PRIORITY_MAX / 2)
// #define AIR720_THREAD_PRIORITY 6

static void air720_power_on(struct at_device *device)
{

    struct at_device_air720 *air720 = RT_NULL;

    air720 = (struct at_device_air720 *)device->user_data;

    /* not nead to set pin configuration for air720 device power on */
    if (air720->power_pin == -1 || air720->power_status_pin == -1)
    {
        return;
    }
    rt_pin_write(air720->power_pin, PIN_LOW);
    rt_thread_mdelay(4000);
    rt_pin_write(air720->power_pin, PIN_HIGH);
}

static void air720_power_off(struct at_device *device)
{
    struct at_device_air720 *air720 = RT_NULL;

    air720 = (struct at_device_air720 *)device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (air720->power_pin == -1 || air720->power_status_pin == -1)
    {
        return;
    }
    rt_pin_write(air720->power_pin, PIN_LOW);
    rt_thread_mdelay(2000);
    rt_pin_write(air720->power_pin, PIN_HIGH);
}

/* =============================  sim76xx network interface operations ============================= */

/* set air720 network interface device status and address information */
static int air720_netdev_set_info(struct netdev *netdev)
{
#define air720_IEMI_RESP_SIZE 32
#define air720_IPADDR_RESP_SIZE 32
#define air720_DNS_RESP_SIZE 96
#define air720_INFO_RESP_TIMO rt_tick_from_millisecond(300)

    int result = RT_EOK;
    ip_addr_t addr;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    if (netdev == RT_NULL)
    {
        LOG_E("input network interface device is NULL.");
        return -RT_ERROR;
    }

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    /* set network interface device status */
    netdev_low_level_set_status(netdev, RT_TRUE);
    netdev_low_level_set_link_status(netdev, RT_TRUE);
    netdev_low_level_set_dhcp_status(netdev, RT_TRUE);

    resp = at_create_resp(air720_IEMI_RESP_SIZE, 0, air720_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("air720 device(%s) set IP address failed, no memory for response object.", device->name);
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
#define air720_NETDEV_HWADDR_LEN 8
#define air720_IEMI_LEN 15

        char imei[air720_IEMI_LEN] = {0};
        int i = 0, j = 0;

        /* send "AT+CGSN" commond to get device IMEI */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGSN") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args(resp, 2, "%s", imei) <= 0)
        {
            LOG_E("%s device prase \"AT+CGSN\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        netdev->hwaddr_len = air720_NETDEV_HWADDR_LEN;
        /* get hardware address by IEMI */
        for (i = 0, j = 0; i < air720_NETDEV_HWADDR_LEN && j < air720_IEMI_LEN; i++, j += 2)
        {
            if (j != air720_IEMI_LEN - 1)
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
#define IP_ADDR_SIZE_MAX 16
        char ipaddr[IP_ADDR_SIZE_MAX] = {0};

        at_resp_set_info(resp, air720_IPADDR_RESP_SIZE, 2, air720_INFO_RESP_TIMO);

        /* send "AT+CIFSR" commond to get IP address */
        if (at_obj_exec_cmd(device->client, resp, "AT+CIFSR") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, ".", "%s", ipaddr) <= 0)
        {
            LOG_E("air720 device(%s) prase \"AT+CIFSR\" commands resposne data error!", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_I("air720 device(%s) IP address: %s", device->name, ipaddr);

        /* set network interface address information */
        inet_aton(ipaddr, &addr);
        netdev_low_level_set_ipaddr(netdev, &addr);
    }

    /* set network interface device dns server */
    {
#define DNS_ADDR_SIZE_MAX 16
        char dns_server1[DNS_ADDR_SIZE_MAX] = {0}, dns_server2[DNS_ADDR_SIZE_MAX] = {0};

        at_resp_set_info(resp, air720_DNS_RESP_SIZE, 0, air720_INFO_RESP_TIMO);

        /* send "AT+CDNSCFG?" commond to get DNS servers address */
        if (at_obj_exec_cmd(device->client, resp, "AT+CDNSCFG?") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, "PrimaryDns:", "PrimaryDns:%s", dns_server1) <= 0 ||
            at_resp_parse_line_args_by_kw(resp, "SecondaryDns:", "SecondaryDns:%s", dns_server2) <= 0)
        {
            LOG_E("Prase \"AT+CDNSCFG?\" commands resposne data error!");
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("air720 device(%s) primary DNS server address: %s", device->name, dns_server1);
        LOG_D("air720 device(%s) secondary DNS server address: %s", device->name, dns_server2);

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

static int air720_reboot(struct at_device *device);

static void check_link_status_entry(void *parameter)
{
#define air720_LINK_STATUS_OK 1
#define air720_LINK_RESP_SIZE 64
#define air720_LINK_RESP_TIMO (3 * RT_TICK_PER_SECOND)
#define air720_LINK_DELAY_TIME (30 * RT_TICK_PER_SECOND)

    at_response_t resp = RT_NULL;
    int result_code, link_status;
    struct at_device *device = RT_NULL;
    struct at_device_air720 *air720 = RT_NULL;

    char parsed_data[10] = {0};
    struct netdev *netdev = (struct netdev *)parameter;

    LOG_D("statrt air720 device(%s) link status check \n");

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return;
    }
    air720 = (struct at_device_air720 *)device->user_data;
    resp = at_create_resp(air720_LINK_RESP_SIZE, 0, air720_LINK_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("air720 device(%s) set check link status failed, no memory for response object.", device->name);
        return;
    }

    while (1)
    {
        /* send "AT+CGREG?" commond  to check netweork interface device link status */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGREG?") < 0)
        {
            rt_thread_mdelay(air720_LINK_DELAY_TIME);
            LOG_E("air720 device(%s) send cgreg failed", device->name);
            continue;
        }

        link_status = -1;
        at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %d,%d", &result_code, &link_status);

        /* check the network interface device link status  */
        if ((air720_LINK_STATUS_OK == link_status) != netdev_is_link_up(netdev))
        {
            netdev_low_level_set_link_status(netdev, (air720_LINK_STATUS_OK == link_status));
        }

        if (rt_pin_read(air720->power_status_pin) == PIN_HIGH) //check the module_status , if moduble_status is Low, user can do your logic here
        {
            if (at_obj_exec_cmd(device->client, resp, "AT+CSQ") == 0)
            {
                at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %s", &parsed_data);
                if (strncmp(parsed_data, "99,99", sizeof(parsed_data)))
                {
                    LOG_D("air720 device(%s) signal strength: %s", device->name, parsed_data);
                }
            }
        }
        else
        {
            //LTE down
            LOG_E("the lte pin is low");
            air720_reboot(device);
        }

        rt_thread_mdelay(air720_LINK_DELAY_TIME);
    }
}

static int air720_netdev_check_link_status(struct netdev *netdev)
{

#define air720_LINK_THREAD_TICK 20
#define air720_LINK_THREAD_STACK_SIZE 1024
#define air720_LINK_THREAD_PRIORITY (RT_THREAD_PRIORITY_MAX - 22)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    if (netdev == RT_NULL)
    {
        LOG_E("input network interface device is NULL.\n");
        return -RT_ERROR;
    }

    rt_snprintf(tname, RT_NAME_MAX, "%s_link", netdev->name);

    tid = rt_thread_create(tname, check_link_status_entry, (void *)netdev,
                           air720_LINK_THREAD_STACK_SIZE, air720_LINK_THREAD_PRIORITY, air720_LINK_THREAD_TICK);
    if (tid)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int air720_net_init(struct at_device *device);

static int air720_netdev_set_up(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    if (device->is_init == RT_FALSE)
    {
        air720_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("the network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int air720_netdev_set_down(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    if (device->is_init == RT_TRUE)
    {
        air720_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("the network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int air720_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define air720_DNS_RESP_LEN 8
#define air720_DNS_RESP_TIMEO rt_tick_from_millisecond(300)

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(dns_server);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(air720_DNS_RESP_LEN, 0, air720_DNS_RESP_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_D("air720 set dns server failed, no memory for response object.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* send "AT+CDNSCFG=<pri_dns>[,<sec_dns>]" commond to set dns servers */
    if (at_obj_exec_cmd(device->client, resp, "AT+CDNSCFG=\"%s\"", inet_ntoa(*dns_server)) < 0)
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

static int air720_ping_domain_resolve(struct at_device *device, const char *name, char ip[16])
{
    int result = RT_EOK;
    char recv_ip[16] = {0};
    at_response_t resp = RT_NULL;

    /* The maximum response time is 14 seconds, affected by network status */
    resp = at_create_resp(128, 4, 14 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for air720 device(%s) response structure.", device->name);
        return -RT_ENOMEM;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+CDNSGIP=\"%s\"", name) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    /* parse the third line of response data, get the IP address */
    if (at_resp_parse_line_args_by_kw(resp, "+CDNSGIP:", "%*[^,],%*[^,],\"%[^\"]", recv_ip) < 0)
    {
        rt_thread_mdelay(100);
        /* resolve failed, maybe receive an URC CRLF */
    }

    if (rt_strlen(recv_ip) < 8)
    {
        rt_thread_mdelay(100);
        /* resolve failed, maybe receive an URC CRLF */
    }
    else
    {
        rt_strncpy(ip, recv_ip, 15);
        ip[15] = '\0';
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

#ifdef NETDEV_USING_PING
static int air720_netdev_ping(struct netdev *netdev, const char *host,
                              size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define air720_PING_RESP_SIZE 128
#define air720_PING_IP_SIZE 16
#define air720_PING_TIMEO (10 * RT_TICK_PER_SECOND)

#define air720_PING_ERR_TIME 600
#define air720_PING_ERR_TTL 255

    int result = RT_EOK;
    int response, time, ttl, i, err_code = 0;
    char ip_addr[air720_PING_IP_SIZE] = {0};
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get air720 device by netdev name(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    for (i = 0; i < rt_strlen(host) && !isalpha(host[i]); i++)
        ;

    if (i < strlen(host))
    {
        /* check domain name is usable */
        if (air720_ping_domain_resolve(device, host, ip_addr) < 0)
        {
            return -RT_ERROR;
        }
        rt_memset(ip_addr, 0x00, air720_PING_IP_SIZE);
    }

    resp = at_create_resp(air720_PING_RESP_SIZE, 0, air720_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("air720 device(%s) set dns server failed, no memory for response object.", device->name);
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

    /* send "AT+CIPPING=<IP addr>[,<retryNum>[,<dataLen>[,<timeout>[,<ttl>]]]]" commond to send ping request */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPPING=%s,1,%d,%d,64",
                        host, data_len, air720_PING_TIMEO / (RT_TICK_PER_SECOND / 10)) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+CIPPING:", "+CIPPING:%d,\"%[^\"]\",%d,%d",
                                      &response, ip_addr, &time, &ttl) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    /* the ping request timeout expires, the response time settting to 600 and ttl setting to 255 */
    if (time == air720_PING_ERR_TIME && ttl == air720_PING_ERR_TTL)
    {
        result = -RT_ETIMEOUT;
        goto __exit;
    }

    inet_aton(ip_addr, &(ping_resp->ip_addr));
    ping_resp->data_len = data_len;
    /* reply time, in units of 100 ms */
    ping_resp->ticks = time;
    ping_resp->ttl = ttl;

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

#ifdef NETDEV_USING_NETSTAT
void air720_netdev_netstat(struct netdev *netdev)
{
    // TODO netstat support
}
#endif /* NETDEV_USING_NETSTAT */

const struct netdev_ops air720_netdev_ops =
    {
        air720_netdev_set_up,
        air720_netdev_set_down,

        RT_NULL, /* not support set ip, netmask, gatway address */
        air720_netdev_set_dns_server,
        RT_NULL, /* not support set DHCP status */

#ifdef NETDEV_USING_PING
        air720_netdev_ping,
#endif
#ifdef NETDEV_USING_NETSTAT
        air720_netdev_netstat,
#endif
};

static struct netdev *air720_netdev_add(const char *netdev_name)
{
#define air720_NETDEV_MTU 1500
    struct netdev *netdev = RT_NULL;

    RT_ASSERT(netdev_name);

    netdev = netdev_get_by_name(netdev_name);
    if (netdev != RT_NULL)
    {
        return (netdev);
    }

    netdev = (struct netdev *)rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for air720 device(%s) netdev structure.", netdev_name);
        return RT_NULL;
    }

    netdev->mtu = air720_NETDEV_MTU;
    netdev->ops = &air720_netdev_ops;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev * netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  sim76xx device operations ============================= */

#define AT_SEND_CMD(client, resp, resp_line, timeout, cmd)                                      \
    do                                                                                          \
    {                                                                                           \
        (resp) = at_resp_set_info((resp), 128, (resp_line), rt_tick_from_millisecond(timeout)); \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                                       \
        {                                                                                       \
            result = -RT_ERROR;                                                                 \
            goto __exit;                                                                        \
        }                                                                                       \
    } while (0)

/* init for air720 */
static void air720_init_thread_entry(void *parameter)
{
#define INIT_RETRY 5
#define CPIN_RETRY 10
#define CSQ_RETRY 10
#define CREG_RETRY 10
#define CGREG_RETRY 30
#define CGATT_RETRY 10

    int i, qimux, retry_num = INIT_RETRY;

    char parsed_data[10] = {0};
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = (struct at_device *)parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for air720 device(%s) response structure.", device->name);
        return;
    }

    while (retry_num--)
    {
        rt_memset(parsed_data, 0, sizeof(parsed_data));
        rt_thread_mdelay(1000);
        air720_power_on(device);
        rt_thread_mdelay(5000); //check the air720 hardware manual, when we use the pow_key to start air720, it takes about 20s,so we put 25s here to ensure starting air720 normally.

        LOG_I("start initializing the air720 device(%s)", device->name);
        /* wait air720 startup finish */
        if (at_client_obj_wait_connect(client, AIR720_WAIT_CONNECT_TIME))
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* disable echo */
        AT_SEND_CMD(client, resp, 0, 300, "ATE0");
        /* get module version */
        AT_SEND_CMD(client, resp, 0, 300, "ATI");
        /* show module version */
        for (i = 0; i < (int)resp->line_counts - 1; i++)
        {
            LOG_I("%s", at_resp_get_line(resp, i + 1));
        }
        /* check SIM card */
        for (i = 0; i < CPIN_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 2, 5 * RT_TICK_PER_SECOND, "AT+CPIN?");

            if (at_resp_get_line_by_kw(resp, "READY"))
            {
                LOG_I("air720 device(%s) SIM card detection success.", device->name);
                break;
            }
            rt_thread_mdelay(500);
        }
        if (i == CPIN_RETRY)
        {
            LOG_E("air720 device(%s) SIM card detection failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
        /* waiting for dirty data to be digested */
        rt_thread_mdelay(100);

        /* check the GSM network is registered */
        for (i = 0; i < CREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CREG?");
            at_resp_parse_line_args_by_kw(resp, "+CREG:", "+CREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {
                LOG_I("air720 device(%s) GSM network is registered(%s),", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CREG_RETRY)
        {
            LOG_E("air720 device(%s) GSM network is register failed(%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }
        /* check the GPRS network is registered */
        for (i = 0; i < CGREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CGREG?");
            at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {
                LOG_I("air720 device(%s) GPRS network is registered(%s).", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CGREG_RETRY)
        {
            LOG_E("air720 device(%s) GPRS network is register failed(%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check signal strength */
        for (i = 0; i < CSQ_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CSQ");
            at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %s", &parsed_data);
            if (strncmp(parsed_data, "99,99", sizeof(parsed_data)))
            {
                LOG_I("air720 device(%s) signal strength: %s", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CSQ_RETRY)
        {
            LOG_E("air720 device(%s) signal strength check failed (%s)", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

        for (i = 0; i < CGATT_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CGATT?");
            at_resp_parse_line_args_by_kw(resp, "+CGATT:", "+CGATT: %s", &parsed_data);
            if (strncmp(parsed_data, "1", sizeof(parsed_data)) == 0)
            {
                LOG_I("air720 device(%s) attach GPRS", device->name);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CGATT_RETRY)
        {
            LOG_E("air720 device(%s) can't attach GPRS ", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* the device default response timeout is 40 seconds, but it set to 15 seconds is convenient to use. */
        AT_SEND_CMD(client, resp, 2, 2 * 1000, "AT+CIPSHUT");

        /* Set to multiple connections */
        AT_SEND_CMD(client, resp, 0, 300, "AT+CIPMUX?");
        at_resp_parse_line_args_by_kw(resp, "+CIPMUX:", "+CIPMUX: %d", &qimux);
        if (qimux == 0)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CIPMUX=1");
        }

        AT_SEND_CMD(client, resp, 0, 300, "AT+CIPQSEND=1"); //fast send mode ,recommend

        AT_SEND_CMD(client, resp, 0, 300, "AT+CSTT");

        AT_SEND_CMD(client, resp, 0, 20 * 1000, "AT+CIICR");

        AT_SEND_CMD(client, resp, 2, 300, "AT+CIFSR");

        if (at_resp_get_line_by_kw(resp, "ERROR") != RT_NULL)
        {
            LOG_E("air720 device(%s) get the local address failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
        result = RT_EOK;

    __exit:
        if (result == RT_EOK)
        {
            break;
        }
        else
        {
            /* power off the air720 device */
            air720_power_off(device);
            rt_thread_mdelay(1000);

            LOG_I("air720 device(%s) initialize retry...", device->name);
        }
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    if (result == RT_EOK)
    {
        /* set network interface device status and address information */
        air720_netdev_set_info(device->netdev);

        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            air720_netdev_check_link_status(device->netdev);
        }

        LOG_I("air720 device(%s) network initialize success!", device->name);
    }
    else
    {
        LOG_E("air720 device(%s) network initialize failed(%d)!", device->name, result);
    }
}

static int air720_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_AIR720_INIT_ASYN
    rt_thread_t tid;
    tid = rt_thread_create("air720_net_init", air720_init_thread_entry, (void *)device,
                           AIR720_THREAD_STACK_SIZE, AIR720_THREAD_PRIORITY, 20);
    if (tid)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create air720 device(%s) initialization thread failed.", device->name);
        return -RT_ERROR;
    }
#else
    air720_init_thread_entry(device);
#endif /* AT_DEVICE_air720_INIT_ASYN */

    return RT_EOK;
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data);

    LOG_I("URC data : %.*s", size, data);
}

/* air720 device URC table for the device control */
static const struct at_urc urc_table[] =
    {
        {"RDY", "\r\n", urc_func},
        {"CLOSED", "\r\n", urc_func},
        {"SMS READY", "\r\n", urc_func},
        {"UART Boot Completed", "\r\n", urc_func},
};

static int air720_init(struct at_device *device)
{
    struct at_device_air720 *air720 = (struct at_device_air720 *)device->user_data;

    /* initialize AT client */
    at_client_init(air720->client_name, air720->recv_line_num);

    device->client = at_client_get(air720->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("air720 device(%s) initialize failed, get AT client(%s) failed.", air720->device_name, air720->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#ifdef AT_USING_SOCKET
    air720_socket_init(device);
#endif

    /* add air720 device to the netdev list */
    device->netdev = air720_netdev_add(air720->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("air720 device(%s) initialize failed, get network interface device failed.", air720->device_name);
        return -RT_ERROR;
    }

    /* initialize air720 pin configuration */
    if (air720->power_pin != -1 && air720->power_status_pin != -1)
    {
        rt_pin_mode(air720->power_pin, PIN_MODE_OUTPUT);
        rt_pin_mode(air720->power_status_pin, PIN_MODE_INPUT);
    }

    /* initialize air720 device network */
    return air720_netdev_set_up(device->netdev);
}

static int air720_deinit(struct at_device *device)
{
    return air720_netdev_set_down(device->netdev);
}

static int air720_reboot(struct at_device *device)
{
    air720_power_off(device);
    rt_thread_mdelay(2000);
    air720_power_on(device);
    device->is_init = RT_FALSE;
    air720_net_init(device);
    device->is_init = RT_TRUE;
    return RT_EOK;
}

static int air720_reset(struct at_device *device)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    /* send "AT+RST" commonds to mw31 device */
    result = at_obj_exec_cmd(client, RT_NULL, "AT+RESET");
    rt_thread_mdelay(1000);

    /* waiting 10 seconds for mw31 device reset */
    device->is_init = RT_FALSE;
    if (at_client_obj_wait_connect(client, AIR720_WAIT_CONNECT_TIME))
    {
        return -RT_ETIMEOUT;
    }

    /* initialize air720 device network */
    air720_net_init(device);

    device->is_init = RT_TRUE;

    return result;
}

static int air720_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_POWER_ON:
    case AT_DEVICE_CTRL_POWER_OFF:
    case AT_DEVICE_CTRL_LOW_POWER:
    case AT_DEVICE_CTRL_SLEEP:
    case AT_DEVICE_CTRL_WAKEUP:
    case AT_DEVICE_CTRL_NET_CONN:
    case AT_DEVICE_CTRL_NET_DISCONN:
    case AT_DEVICE_CTRL_SET_WIFI_INFO:
    case AT_DEVICE_CTRL_GET_SIGNAL:
    case AT_DEVICE_CTRL_GET_GPS:
    case AT_DEVICE_CTRL_GET_VER:
        LOG_W("air720 not support the control command(%d).", cmd);
        break;
    case AT_DEVICE_CTRL_RESET:
        result = air720_reset(device);
        break;
    //case AT_DEVICE_CTRL_REBOOT:
    //    result = air720_reboot(device);
    //    break;
    default:
        LOG_E("input error control command(%d).", cmd);
        break;
    }

    return result;
}

const struct at_device_ops air720_device_ops =
    {
        air720_init,
        air720_deinit,
        air720_control,
};

static int air720_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *)rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for air720 device class create.");
        return -RT_ENOMEM;
    }

    /* fill air720 device class object */
#ifdef AT_USING_SOCKET
    air720_socket_class_register(class);
#endif
    class->device_ops = &air720_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_AIR720);
}
INIT_DEVICE_EXPORT(air720_device_class_register);

#endif /* AT_DEVICE_USING_air720 */
