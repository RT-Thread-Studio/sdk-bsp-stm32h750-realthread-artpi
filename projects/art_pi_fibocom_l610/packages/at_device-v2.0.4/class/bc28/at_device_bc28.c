 /*
 * File      : at_device_bc28.c
 * This file is part of RT-Thread RTOS
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
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
 * Date           Author            Notes
 * 2020-02-12     luhuadong         first version
 */

#include <stdio.h>
#include <string.h>
#include <at_device_bc28.h>

#if !defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10301
#error "This AT Client version is older, please check and update latest AT Client!"
#endif

#define LOG_TAG                        "at.dev.bc28"
#include <at_log.h>

#ifdef AT_DEVICE_USING_BC28

#define BC28_WAIT_CONNECT_TIME          5000
#define BC28_THREAD_STACK_SIZE          2048
#define BC28_THREAD_PRIORITY            (RT_THREAD_PRIORITY_MAX/2)

static int bc28_sleep(struct at_device *device)
{
    return(RT_EOK);
}

static int bc28_wakeup(struct at_device *device)
{
    return(RT_EOK);
}

static int bc28_reset(struct at_device *device)
{
    struct at_device_bc28 *bc28 = (struct at_device_bc28 *)device->user_data;

    rt_pin_mode(bc28->power_pin, PIN_MODE_OUTPUT);
    rt_pin_write(bc28->power_pin, PIN_HIGH);
    rt_thread_mdelay(300);
    rt_pin_write(bc28->power_pin, PIN_LOW);
    bc28->power_status = 1;

    return(RT_EOK);
}

static int bc28_check_link_status(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_bc28 *bc28 = RT_NULL;
    int result = -RT_ERROR;

    bc28 = (struct at_device_bc28 *)device->user_data;

    if ( ! bc28->power_status) // power off
    {
        LOG_E("the power is off.");
        return(-RT_ERROR);
    }
    if (bc28->sleep_status)    // is sleep status
    {
        if (bc28->power_pin != -1)
        {
            rt_pin_write(bc28->power_pin, PIN_HIGH);
            rt_thread_mdelay(100);
            rt_pin_write(bc28->power_pin, PIN_LOW);
            rt_thread_mdelay(200);
        }
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    result = -RT_ERROR;
    if (at_obj_exec_cmd(device->client, resp, "AT+CGATT?") == RT_EOK)
    {
        int link_stat = 0;
        if (at_resp_parse_line_args_by_kw(resp, "+CGATT:", "+CGATT:%d", &link_stat) > 0)
        {
            if (link_stat == 1)
            {
                result = RT_EOK;
            }
        }
    }

    at_delete_resp(resp);
    return(result);
}

/* =============================  bc28 network interface operations ============================= */
/* set bc28 network interface device status and address information */
static int bc28_netdev_set_info(struct netdev *netdev)
{
#define BC28_INFO_RESP_SIZE        128
#define BC28_INFO_RESP_TIMOUT      rt_tick_from_millisecond(5000)

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

    resp = at_create_resp(BC28_INFO_RESP_SIZE, 0, BC28_INFO_RESP_TIMOUT);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define BC28_NETDEV_HWADDR_LEN   8
        #define BC28_IMEI_LEN            15

        char imei[BC28_IMEI_LEN] = {0};
        int i = 0, j = 0;

        /* send "AT+CGSN=1" commond to get device IMEI */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGSN=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        if (at_resp_parse_line_args(resp, 2, "+CGSN:%s", imei) <= 0)
        {
            LOG_E("%s device prase \"AT+CGSN=1\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        LOG_D("%s device IMEI number: %s", device->name, imei);

        netdev->hwaddr_len = BC28_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < BC28_NETDEV_HWADDR_LEN && j < BC28_IMEI_LEN; i++, j+=2)
        {
            if (j != BC28_IMEI_LEN - 1)
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
        char ipaddr[IP_ADDR_SIZE_MAX] = {0};

        /* send "AT+CGPADDR" commond to get IP address */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "+CGPADDR: 0,<IP_address>" */
        if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR:%*d,%s", ipaddr) <= 0)
        {
            LOG_E("%s device \"AT+CGPADDR\" cmd error.", device->name);
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

        /* send "AT+QIDNSCFG?" commond to get DNS servers address */
        if (at_obj_exec_cmd(device->client, resp, "AT+QIDNSCFG?") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "PrimaryDns:<pri_dns>"
         *                     "SecondaryDns:<sec_dns>"
        */
        if (at_resp_parse_line_args_by_kw(resp, "PrimaryDns:", "PrimaryDns: %s", dns_server1) <= 0)
        {
            LOG_E("%s device prase \"AT+QIDNSCFG?\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args_by_kw(resp, "SecondaryDns:", "SecondaryDns: %s", dns_server2) <= 0)
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

static void bc28_check_link_status_entry(void *parameter)
{
#define BC28_LINK_DELAY_TIME    (60 * RT_TICK_PER_SECOND)

    rt_bool_t is_link_up;
    struct at_device *device = RT_NULL;
    struct netdev *netdev = (struct netdev *) parameter;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    while (1)
    {
        is_link_up = (bc28_check_link_status(device) == RT_EOK);

        netdev_low_level_set_link_status(netdev, is_link_up);

        rt_thread_delay(BC28_LINK_DELAY_TIME);
    }
}

static int bc28_netdev_check_link_status(struct netdev *netdev)
{
#define BC28_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define BC28_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    /* create bc28 link status polling thread  */
    tid = rt_thread_create(tname, bc28_check_link_status_entry, (void *)netdev,
                           BC28_LINK_THREAD_STACK_SIZE, BC28_LINK_THREAD_PRIORITY, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int bc28_net_init(struct at_device *device);

static int bc28_netdev_set_up(struct netdev *netdev)
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
        bc28_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int bc28_netdev_set_down(struct netdev *netdev)
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
        //bc28_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int bc28_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define BC28_DNS_RESP_LEN      64
#define BC28_DNS_RESP_TIMEOUT  rt_tick_from_millisecond(300)

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(dns_server);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(BC28_DNS_RESP_LEN, 0, BC28_DNS_RESP_TIMEOUT);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+QIDNSCFG=%s", inet_ntoa(*dns_server)) != RT_EOK)
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

#ifdef AT_USING_SOCKET
    int bc28_domain_resolve(const char *name, char ip[16]);
#endif
#ifdef NETDEV_USING_PING
static int bc28_netdev_ping(struct netdev *netdev, const char *host,
        size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define BC28_PING_RESP_SIZE       128
#define BC28_PING_IP_SIZE         16
#define BC28_PING_TIMEOUT         (10 * RT_TICK_PER_SECOND)

    rt_err_t result = RT_EOK;
    int response = -1, recv_data_len, ping_time, ttl;
    char ip_addr[BC28_PING_IP_SIZE] = {0};
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

    resp = at_create_resp(BC28_PING_RESP_SIZE, 4, BC28_PING_TIMEOUT);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create");
        return -RT_ENOMEM;
    }

    /* DNS resolve */
    struct in_addr inp;
    if (inet_aton(host, &inp) > 0)
    {
        rt_strncpy(ip_addr, host, BC28_PING_IP_SIZE);
    }
#ifdef AT_USING_SOCKET
    else
    {
        if(0 > bc28_domain_resolve(host, ip_addr))
        {
            LOG_E("can not resolve domain");
            goto __exit;
        }
    }
#endif

    if (at_obj_exec_cmd(device->client, resp, "AT+NPING=%s,%d,%d",
                        ip_addr, data_len, timeout*1000/RT_TICK_PER_SECOND) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+NPINGERR:", "+NPINGERR:%d", &response) > 0)
    {
        switch (response)
        {
        case 1:
            result = -RT_ETIMEOUT;
            break;
        case 2:
        default:
            result = -RT_ERROR;
            break;
        }
    }
    else if (at_resp_parse_line_args_by_kw(resp, "+NPING:", "+NPING:%[^,],%d,%d",
                                            ip_addr, &ttl, &ping_time) > 0 )
    {
        inet_aton(ip_addr, &(ping_resp->ip_addr));
        ping_resp->data_len = data_len;
        ping_resp->ticks = rt_tick_from_millisecond(ping_time);
        ping_resp->ttl = ttl;
        result = RT_EOK;
    }
    else
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
#endif /* NETDEV_USING_PING */

const struct netdev_ops bc28_netdev_ops =
{
    bc28_netdev_set_up,
    bc28_netdev_set_down,

    RT_NULL,
    bc28_netdev_set_dns_server,
    RT_NULL,

#ifdef NETDEV_USING_PING
    bc28_netdev_ping,
#endif
    RT_NULL,
};

static struct netdev *bc28_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          8
    struct netdev *netdev = RT_NULL;

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
    netdev->ops = &bc28_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  bc28 device operations ============================= */

/* initialize for bc28 */
static void bc28_init_thread_entry(void *parameter)
{
#define INIT_RETRY                     5
#define CPIN_RETRY                     5
#define CSQ_RETRY                      20
#define CGREG_RETRY                    50
#define IPADDR_RETRY                   10
#define AT_DEFAULT_TIMEOUT             5000

    int i;
    int retry_num = INIT_RETRY;
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = (struct at_device *) parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(128, 0, rt_tick_from_millisecond(AT_DEFAULT_TIMEOUT));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    LOG_D("start init %s device.", device->name);

    while (retry_num--)
    {
        /* reset the bc28 device */
        bc28_reset(device);
        rt_thread_mdelay(1000);

        /* wait bc28 startup finish, send AT every 500ms, if receive OK, SYNC success*/
        if (at_client_obj_wait_connect(client, BC28_WAIT_CONNECT_TIME))
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* disable echo */
        if (at_obj_exec_cmd(device->client, resp, "ATE0") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E("ATE0");
            goto __exit;
        }

        /* disable auto register */
        if (at_obj_exec_cmd(device->client, resp, "AT+QREGSWT=2") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+QREGSWT=2");
            goto __exit;
        }

        /* disable auto connect */
        if (at_obj_exec_cmd(device->client, resp, "AT+NCONFIG=AUTOCONNECT,FALSE") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+NCONFIG=AUTOCONNECT,FALSE");
            goto __exit;
        }

        /* reboot */
        at_obj_exec_cmd(device->client, resp, "AT+NRB");
        rt_thread_mdelay(5000);

        while (at_obj_exec_cmd(device->client, resp, "AT") != RT_EOK)
        {
            rt_thread_mdelay(1000);
        }

        /* check IMEI */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGSN=1") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CGSN=1");
            goto __exit;
        }

        /* search band */
        if (at_obj_exec_cmd(device->client, resp, "AT+NBAND=%d", AT_DEVICE_BC28_OP_BAND) != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+NBAND=8");
            goto __exit;
        }

        /* set max function */
        if (at_obj_exec_cmd(device->client, resp, "AT+CFUN=1") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CFUN=1");
            goto __exit;
        }

        /* auto report recv from tcp */
        if (at_obj_exec_cmd(device->client, resp, "AT+NSONMI=2") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+NSONMI=2");
            goto __exit;
        }

        /* disable eDRX mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+CEDRXS=0,5") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CEDRXS=0,5");
            goto __exit;
        }

        /* disable PSM mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=0") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CPSMS=0");
            goto __exit;
        }

        /* check IMSI */
        if (at_obj_exec_cmd(device->client, resp, "AT+CIMI") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CIMI");
            goto __exit;
        }

        /* attach */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGATT=1") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+CGATT=1");
            goto __exit;
        }

        /* Get the baudrate */
        if (at_obj_exec_cmd(device->client, resp, "AT+NATSPEED?") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> AT+NATSPEED?");
            goto __exit;
        }
        at_resp_parse_line_args_by_kw(resp, "+NATSPEED:", "+NATSPEED:%d", &i);
        LOG_D("%s device baudrate %d", device->name, i);

        /* get module version */
        if (at_obj_exec_cmd(device->client, resp, "ATI") != RT_EOK)
        {
            result = -RT_ERROR;
            LOG_E(">> ATI");
            goto __exit;
        }
        for (i = 0; i < (int) resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }

        /* check SIM card */
        for (i = 0; i < CPIN_RETRY; i++)
        {
            rt_thread_mdelay(1000);
            if (at_obj_exec_cmd(device->client, resp, "AT+CPIN?") == RT_EOK)
            {
                if (at_resp_get_line_by_kw(resp, "READY") != RT_NULL)
                    break;
            }
        }
        if (i == CPIN_RETRY)
        {
            LOG_E("%s device SIM card detection failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check signal strength */
        for (i = 0; i < CSQ_RETRY; i++)
        {
            rt_thread_mdelay(1000);
            if (at_obj_exec_cmd(device->client, resp, "AT+CSQ") == RT_EOK)
            {
                int signal_strength = 0, err_rate = 0;

                if (at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ:%d,%d", &signal_strength, &err_rate) > 0)
                {
                    if ((signal_strength != 99) && (signal_strength != 0))
                    {
                        LOG_D("%s device signal strength: %d, channel bit error rate: %d",
                                device->name, signal_strength, err_rate);
                        break;
                    }
                }
            }
        }
        if (i == CSQ_RETRY)
        {
            LOG_E("%s device signal strength check failed", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check the GPRS network is registered */
        for (i = 0; i < CGREG_RETRY; i++)
        {
            rt_thread_mdelay(1000);
            if (at_obj_exec_cmd(device->client, resp, "AT+CGATT?") == RT_EOK)
            {
                int link_stat = 0;

                if (at_resp_parse_line_args_by_kw(resp, "+CGATT:", "+CGATT:%d", &link_stat) > 0)
                {
                    if (link_stat == 1)
                    {
                        LOG_D("%s device GPRS is registered", device->name);
                        break;
                    }
                }
            }
        }
        if (i == CGREG_RETRY)
        {
            LOG_E("%s device GPRS is register failed", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* check the GPRS network IP address */
        for (i = 0; i < IPADDR_RETRY; i++)
        {
            rt_thread_mdelay(1000);
            if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR") == RT_EOK)
            {
                char ipaddr[IP_ADDR_SIZE_MAX] = {0};

                /* parse response data "+CGPADDR: 0,<IP_address>" */
                if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR:%*d,%s", ipaddr) > 0)
                {
                    LOG_D("%s device IP address: %s", device->name, ipaddr);
                    break;
                }
            }
        }
        if (i == IPADDR_RETRY)
        {
            LOG_E("%s device GPRS is get IP address failed", device->name);
            result = -RT_ERROR;
            goto __exit;
        }

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            /* power off the bc28 device */
            //bc28_power_off(device);
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
        bc28_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            bc28_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success.", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
}

/* bc28 device network initialize */
static int bc28_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_BC28_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("bc28_net", bc28_init_thread_entry, (void *)device,
                           BC28_THREAD_STACK_SIZE, BC28_THREAD_PRIORITY, 20);
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
    bc28_init_thread_entry(device);
#endif /* AT_DEVICE_BC28_INIT_ASYN */

    return RT_EOK;
}

static int bc28_init(struct at_device *device)
{
    struct at_device_bc28 *bc28 = (struct at_device_bc28 *)device->user_data;

    /* configure AT client */
    rt_device_t serial = rt_device_find(bc28->client_name);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    config.baud_rate = BC28_AT_CLIENT_BAUD_RATE;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.bufsz     = bc28->recv_bufsz;
    config.parity    = PARITY_NONE;

    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
    rt_device_close(serial);

    /* initialize AT client */
    at_client_init(bc28->client_name, bc28->recv_bufsz);

    device->client = at_client_get(bc28->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", bc28->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
#ifdef AT_USING_SOCKET
    bc28_socket_init(device);
#endif

    /* add bc28 device to the netdev list */
    device->netdev = bc28_netdev_add(bc28->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", bc28->device_name);
        return -RT_ERROR;
    }

    /* initialize bc28 pin configuration */
    if (bc28->power_pin != -1)
    {
        rt_pin_mode(bc28->power_pin, PIN_MODE_OUTPUT);
        rt_pin_write(bc28->power_pin, PIN_LOW);
    }

    /* initialize bc28 device network */
    return bc28_netdev_set_up(device->netdev);
}

static int bc28_deinit(struct at_device *device)
{
    RT_ASSERT(device);

    return bc28_netdev_set_down(device->netdev);
}

static int bc28_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_RESET:
        result = bc28_reset(device);
        break;
    case AT_DEVICE_CTRL_SLEEP:
    case AT_DEVICE_CTRL_WAKEUP:
    case AT_DEVICE_CTRL_POWER_ON:
    case AT_DEVICE_CTRL_POWER_OFF:
    case AT_DEVICE_CTRL_LOW_POWER:
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

const struct at_device_ops bc28_device_ops =
{
    bc28_init,
    bc28_deinit,
    bc28_control,
};

static int bc28_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill bc28 device class object */
#ifdef AT_USING_SOCKET
    bc28_socket_class_register(class);
#endif
    class->device_ops = &bc28_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_BC28);
}
INIT_DEVICE_EXPORT(bc28_device_class_register);

#endif /* AT_DEVICE_USING_BC28 */
