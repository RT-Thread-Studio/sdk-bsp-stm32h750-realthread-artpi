/*
 * File      : at_socket_sim76xx.c
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
 * 2018-12-22    thomasonegd  first version
 * 2019-03-06    thomasonegd  fix udp connection.
 * 2019-03-08    thomasonegd  add power_on & power_off api
 * 2019-05-14    chenyong     multi AT socket client support
 * 2019-08-24    chenyong     add netdev support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_sim76xx.h>

#define LOG_TAG                        "at.dev.sim76"
#include <at_log.h>

#ifdef AT_DEVICE_USING_SIM76XX

#define SIM76XX_WAIT_CONNECT_TIME      5000
#define SIM76XX_THREAD_STACK_SIZE      2048
#define SIM76XX_THREAD_PRIORITY        (RT_THREAD_PRIORITY_MAX / 2)

/* power up sim76xx modem */
static void sim76xx_power_on(struct at_device *device)
{
    struct at_device_sim76xx *sim76xx = RT_NULL;

    sim76xx = (struct at_device_sim76xx *) device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (sim76xx->power_pin == -1 || sim76xx->power_status_pin == -1)
    {
        return;
    }

    if (rt_pin_read(sim76xx->power_status_pin) == PIN_HIGH)
    {
        return;
    }
    rt_pin_write(sim76xx->power_pin, PIN_HIGH);

    while (rt_pin_read(sim76xx->power_status_pin) == PIN_LOW)
    {
        rt_thread_mdelay(10);
    }
    rt_pin_write(sim76xx->power_pin, PIN_LOW);
}

/* power off sim76xx modem */
static void sim76xx_power_off(struct at_device *device)
{
    struct at_device_sim76xx *sim76xx = RT_NULL;

    sim76xx = (struct at_device_sim76xx *) device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (sim76xx->power_pin == -1 || sim76xx->power_status_pin == -1)
    {
        return;
    }

    if (rt_pin_read(sim76xx->power_status_pin) == PIN_LOW)
    {
        return;
    }
    rt_pin_write(sim76xx->power_pin, PIN_HIGH);

    while (rt_pin_read(sim76xx->power_status_pin) == PIN_HIGH)
    {
        rt_thread_mdelay(10);
    }
    rt_pin_write(sim76xx->power_pin, PIN_LOW);
}

/* =============================  sim76xx network interface operations ============================= */

/* set sim76xx network interface device status and address information */
static int sim76xx_netdev_set_info(struct netdev *netdev)
{
#define SIM76XX_IMEI_RESP_SIZE      256
#define SIM76XX_IPADDR_RESP_SIZE    64
#define SIM76XX_DNS_RESP_SIZE       96
#define SIM76XX_INFO_RESP_TIMO      rt_tick_from_millisecond(300)

    int result = RT_EOK;
    ip_addr_t addr;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    /* set network interface device status */
    netdev_low_level_set_status(netdev, RT_TRUE);
    netdev_low_level_set_link_status(netdev, RT_TRUE);
    netdev_low_level_set_dhcp_status(netdev, RT_TRUE);

    resp = at_create_resp(SIM76XX_IMEI_RESP_SIZE, 0, SIM76XX_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define SIM76XX_NETDEV_HWADDR_LEN   8
        #define SIM76XX_IMEI_LEN            15

        char imei[SIM76XX_IMEI_LEN] = {0};
        int i = 0, j = 0;

        /* send "ATI" commond to get device IMEI */
        if (at_obj_exec_cmd(device->client, resp, "ATI") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, "IMEI:", "IMEI: %s", imei) <= 0)
        {
            LOG_E("%s device prase \"ATI\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("%s device IMEI number: %s", device->name, imei);

        netdev->hwaddr_len = SIM76XX_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < SIM76XX_NETDEV_HWADDR_LEN && j < SIM76XX_IMEI_LEN; i++, j += 2)
        {
            if (j != SIM76XX_IMEI_LEN - 1)
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

        at_resp_set_info(resp, SIM76XX_IPADDR_RESP_SIZE, 2, SIM76XX_INFO_RESP_TIMO);

        /* send "AT+IPADDR" commond to get IP address */
        if (at_obj_exec_cmd(device->client, resp, "AT+IPADDR") < 0)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args_by_kw(resp, "+IPADDR:", "+IPADDR: %s", ipaddr) <= 0)
        {
            LOG_E("%s device prase \"AT+IPADDR\" cmd error!", device->name);
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
        #define DEF_DNS_ADDR   "114.114.114.114"
        const char *dns_server = DEF_DNS_ADDR;
        ip_addr_t addr;

        /* not support get dns server address, using default dns server address */
        inet_aton(dns_server, &addr);
        netdev_low_level_set_dns_server(netdev, 0, &addr);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

/* check sim76xx device link_up status */
static void check_link_status_entry(void *parameter)
{
#define SIM76XX_LINK_STATUS_OK   1
#define SIM76XX_LINK_RESP_SIZE   64
#define SIM76XX_LINK_RESP_TIMO   (3 * RT_TICK_PER_SECOND)
#define SIM76XX_LINK_DELAY_TIME  (30 * RT_TICK_PER_SECOND)

    at_response_t resp = RT_NULL;
    int result_code, link_status;
    struct at_device *device = RT_NULL;
    struct netdev *netdev = (struct netdev *)parameter;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    resp = at_create_resp(SIM76XX_LINK_RESP_SIZE, 0, SIM76XX_LINK_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    while (1)
    {
        /* send "AT+CGREG?" commond  to check netweork interface device link status */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGREG?") < 0)
        {
            rt_thread_mdelay(SIM76XX_LINK_DELAY_TIME);

            continue;
        }

        link_status = -1;
        at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %d,%d", &result_code, &link_status);

        /* check the network interface device link status  */
        if ((SIM76XX_LINK_STATUS_OK == link_status) != netdev_is_link_up(netdev))
        {
            netdev_low_level_set_link_status(netdev, (SIM76XX_LINK_STATUS_OK == link_status));
        }

        rt_thread_mdelay(SIM76XX_LINK_DELAY_TIME);
    }
}

static int sim76xx_netdev_check_link_status(struct netdev *netdev)
{
#define SIM76XX_LINK_THREAD_TICK           20
#define SIM76XX_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define SIM76XX_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    tid = rt_thread_create(tname, check_link_status_entry, (void *)netdev,
                           SIM76XX_LINK_THREAD_STACK_SIZE, SIM76XX_LINK_THREAD_PRIORITY, SIM76XX_LINK_THREAD_TICK);
    if (tid)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int sim76xx_net_init(struct at_device *device);

/* sim76xx network interface device set up status */
static int sim76xx_netdev_set_up(struct netdev *netdev)
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
        sim76xx_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network intterface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

/* sim76xx network interface device set down status */
static int sim76xx_netdev_set_down(struct netdev *netdev)
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
        sim76xx_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

#ifdef NETDEV_USING_PING
/* sim76xx network interface device ping feature */
static int sim76xx_netdev_ping(struct netdev *netdev, const char *host,
        size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define SIM76XX_PING_RESP_SIZE         128
#define SIM76XX_PING_IP_SIZE           16
#define SIM76XX_PING_TIMEO             (12 * RT_TICK_PER_SECOND)

    int result = RT_EOK;
    int response, pkg_size, time, ttl;
    char ip_addr[SIM76XX_PING_IP_SIZE] = {0};
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(SIM76XX_PING_RESP_SIZE, 6, SIM76XX_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ERROR;
        goto __exit;
    }

    /* send "AT+CIPPING=<dest_addr>,<dest_addr_type>[,<num_pings>[,<package_size>[,<interval_time>[,<wait_timer>[,<TTL>]]]]]"
       commond to send ping request */
    if (at_obj_exec_cmd(device->client, resp, "AT+CPING=\"%s\",1,1,%d,,,64", host, data_len) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+CPING:", "+CPING:%d,%[^,],%d,%d,%d",
             &response, ip_addr, &pkg_size, &time, &ttl) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    /* 2 - ping timeout */
    if (response == 2)
    {
        result = -RT_ETIMEOUT;
        goto __exit;
    }

    inet_aton(ip_addr, &(ping_resp->ip_addr));
    ping_resp->data_len = pkg_size;
    /* reply time, in units of ms */
    ping_resp->ticks = rt_tick_from_millisecond(time);
    ping_resp->ttl = ttl;

 __exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

/* sim76xx network interface device operations */
const struct netdev_ops sim76xx_netdev_ops =
{
    sim76xx_netdev_set_up,
    sim76xx_netdev_set_down,

    RT_NULL, /* not support set ip, netmask, gatway address */
    RT_NULL, /* not support set DNS server address */
    RT_NULL, /* not support set DHCP status */

#ifdef NETDEV_USING_PING
    sim76xx_netdev_ping,
#endif
    RT_NULL, /* not support netstat feature */
};

/* register sim76xx network interface */
static struct netdev *sim76xx_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU 1500
#define HWADDR_LEN 8
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
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = ETHERNET_MTU;
    netdev->hwaddr_len = HWADDR_LEN;
    netdev->ops = &sim76xx_netdev_ops;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev * netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  sim76xx device operations ============================= */

#define AT_SEND_CMD(client, resp, cmd)                                          \
    do {                                                                        \
        (resp) = at_resp_set_info((resp), 256, 0, 5 * RT_TICK_PER_SECOND);      \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                       \
        {                                                                       \
            result = -RT_ERROR;                                                 \
            goto __exit;                                                        \
        }                                                                       \
    } while(0)                                                                  \

/* initialize the sim76xx device network connection by command */
static void sim76xx_init_thread_entry(void *parameter)
{
#define INIT_RETRY                     5
#define CPIN_RETRY                     5
#define CSQ_RETRY                      10
#define CREG_RETRY                     10
#define CGREG_RETRY                    10
#define CGATT_RETRY                    10
#define CCLK_RETRY                     10

    at_response_t resp = RT_NULL;
    rt_err_t result = RT_EOK;
    rt_size_t i, qi_arg[3] = {0};
    int retry_num = INIT_RETRY;
    char parsed_data[20] = {0};
    struct at_device *device = (struct at_device *)parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    LOG_D("start init %s device.", device->name);

    while (retry_num--)
    {
        /* power-up sim76xx */
        sim76xx_power_on(device);

        /* wait SIM76XX startup finish, Send AT every 5s, if receive OK, SYNC success*/
        if (at_client_wait_connect(SIM76XX_WAIT_CONNECT_TIME))
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* disable echo */
        AT_SEND_CMD(client, resp, "ATE0");

        /* get module version */
        AT_SEND_CMD(client, resp, "ATI");

        /* show module version */
        for (i = 0; i < (int)resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }
        /* check SIM card */

        rt_thread_mdelay(1000);
        for (i = 0; i < CPIN_RETRY; i++)
        {
            at_obj_exec_cmd(client, resp, "AT+CPIN?");
            if (at_resp_get_line_by_kw(resp, "READY"))
            {
                LOG_D("%s device SIM card detection success.", device->name);
                break;
            }
            LOG_I("\"AT+CPIN\" commands send retry...");
            rt_thread_mdelay(1000);
        }

        if (i == CPIN_RETRY)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* waiting for dirty data to be digested */
        rt_thread_mdelay(10);
        /* check signal strength */
        for (i = 0; i < CSQ_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, "AT+CSQ");
            at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %d,%d", &qi_arg[0], &qi_arg[1]);
            if (qi_arg[0] != 99)
            {
                LOG_D("%s device signal strength: %d  Channel bit error rate: %d", device->name, qi_arg[0], qi_arg[1]);
                break;
            }
            rt_thread_mdelay(1000);
        }

        if (i == CSQ_RETRY)
        {
            LOG_E("%s device signal strength check failed (%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

        /* do not show the prompt when receiving data */
        AT_SEND_CMD(client, resp, "AT+CIPSRIP=0");

        /* check the GSM network is registered */
        for (i = 0; i < CREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, "AT+CREG?");
            at_resp_parse_line_args_by_kw(resp, "+CREG:", "+CREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) || !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {
                LOG_D("%s device GSM is registered(%s).", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CREG_RETRY)
        {
            LOG_E("%s device GSM is register failed(%s).", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check the GPRS network is registered */
        for (i = 0; i < CGREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, "AT+CGREG?");
            at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) || !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
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

        /* check packet domain attach or detach */
        for (i = 0; i < CGATT_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, "AT+CGATT?");
            at_resp_parse_line_args_by_kw(resp, "+CGATT:", "+CGATT: %s", &parsed_data);
            if (!strncmp(parsed_data, "1", 1))
            {
                LOG_D("%s device Packet domain attach.", device->name);
                break;
            }

            rt_thread_mdelay(1000);
        }

        if (i == CGATT_RETRY)
        {
            LOG_E("%s device GPRS attach failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* configure context */
        AT_SEND_CMD(client, resp, "AT+CGDCONT=1,\"IP\",\"CMNET\"");

        /* activate context */
        {
            int net_status = 0;

            AT_SEND_CMD(client, resp, "AT+NETOPEN?");
            at_resp_parse_line_args_by_kw(resp, "+NETOPEN:", "+NETOPEN: %d", &net_status);
            /* 0 - netwoek close, 1 - network open */
            if (net_status == 0)
            {
                AT_SEND_CMD(client, resp, "AT+NETOPEN");
            }
        }

        /* set active PDP context's profile number */
        AT_SEND_CMD(client, resp, "AT+CSOCKSETPN=1");

#ifdef RT_USING_RTC
        /* get real time */
        int year, month, day, hour, min, sec;

        for (i = 0; i < CCLK_RETRY; i++)
        {
            if (at_obj_exec_cmd(device->client, at_resp_set_info(resp, 256, 0, 5 * RT_TICK_PER_SECOND), "AT+CCLK?") < 0)
            {
                rt_thread_mdelay(500);
                continue;
            }

            /* +CCLK: "18/12/22,18:33:12+32" */
            if (at_resp_parse_line_args_by_kw(resp, "+CCLK:", "+CCLK: \"%d/%d/%d,%d:%d:%d",
                                              &year, &month, &day, &hour, &min, &sec) < 0)
            {
                rt_thread_mdelay(500);
                continue;
            }

            set_date(year + 2000, month, day);
            set_time(hour, min, sec);

            break;
        }

        if (i == CCLK_RETRY)
        {
            LOG_E("%s device GPRS attach failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
#endif /* RT_USING_RTC */

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            /* power off the sim76xx device */
            sim76xx_power_off(device);
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
        /* set network interface device status and address information */
        sim76xx_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            sim76xx_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success!", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d)!", device->name, result);
    }
}

int sim76xx_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_SIM76XX_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("sim76_net", sim76xx_init_thread_entry, (void *)device,
                           SIM76XX_THREAD_STACK_SIZE, SIM76XX_THREAD_PRIORITY, 20);
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
    sim76xx_init_thread_entry(device);
#endif /* AT_DEVICE_SIM76XX_INIT_ASYN */

    return RT_EOK;
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(sim76xx_net_init, at_net_init, initialize AT network);
#endif

/* initialize the sim76xx device network connection and register network interface device */
static int sim76xx_init(struct at_device *device)
{
    struct at_device_sim76xx *sim76xx = (struct at_device_sim76xx *) device->user_data;

    /* initialize AT client */
    at_client_init(sim76xx->client_name, sim76xx->recv_line_num);

    device->client = at_client_get(sim76xx->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", sim76xx->client_name);
        return -RT_ERROR;
    }

#ifdef AT_USING_SOCKET
    sim76xx_socket_init(device);
#endif

    /* add sim76xx device to the netdev list */
    device->netdev = sim76xx_netdev_add(sim76xx->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("get netdev(%s) failed.", sim76xx->device_name);
        return -RT_ERROR;
    }

    /* initialize sim76xx pin configuration */
    if (sim76xx->power_pin != -1 && sim76xx->power_status_pin != -1)
    {
        rt_pin_mode(sim76xx->power_pin, PIN_MODE_OUTPUT);
        rt_pin_mode(sim76xx->power_status_pin, PIN_MODE_INPUT);
    }

    /* initialize sim76xx device network */
    return sim76xx_netdev_set_up(device->netdev);
}

/* deinit sim76xx device network connect */
static int sim76xx_deinit(struct at_device *device)
{
    return sim76xx_netdev_set_down(device->netdev);
}

/* custom device control operations */
static int sim76xx_control(struct at_device *device, int cmd, void *arg)
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

const struct at_device_ops sim76xx_device_ops =
{
    sim76xx_init,
    sim76xx_deinit,
    sim76xx_control,
};

/* register sim76xx class to the global at_device class list */
static int sim76xx_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *)rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill sim76xx device class object */
#ifdef AT_USING_SOCKET
    sim76xx_socket_class_register(class);
#endif
    class->device_ops = &sim76xx_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_SIM76XX);
}
INIT_DEVICE_EXPORT(sim76xx_device_class_register);

#endif /* AT_DEVICE_SIM76XX */
