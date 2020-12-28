/*
 * File      : at_device_bc26.c
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
 * 2019-12-13     qiyongzhong  first version
 */

#include <stdio.h>
#include <string.h>

#include <at_device_bc26.h>

#define LOG_TAG "at.dev.bc26"
#include <at_log.h>

#ifdef AT_DEVICE_USING_BC26

#define BC26_WAIT_CONNECT_TIME 5000
#define BC26_THREAD_STACK_SIZE 2048
#define BC26_THREAD_PRIORITY (RT_THREAD_PRIORITY_MAX / 2)

static int bc26_power_on(struct at_device *device)
{
    struct at_device_bc26 *bc26 = RT_NULL;

    bc26 = (struct at_device_bc26 *)device->user_data;
    bc26->power_status = RT_TRUE;

    /* not nead to set pin configuration for bc26 device power on */
    if (bc26->power_pin == -1)
    {
        return (RT_EOK);
    }

    rt_pin_write(bc26->power_pin, PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(bc26->power_pin, PIN_LOW);

    return (RT_EOK);
}

static int bc26_power_off(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_bc26 *bc26 = RT_NULL;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return (-RT_ERROR);
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+QPOWD=0") != RT_EOK)
    {
        LOG_D("power off fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    at_delete_resp(resp);

    bc26 = (struct at_device_bc26 *)device->user_data;
    bc26->power_status = RT_FALSE;

    return (RT_EOK);
}

static int bc26_sleep(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_bc26 *bc26 = RT_NULL;

    bc26 = (struct at_device_bc26 *)device->user_data;
    if (!bc26->power_status) //power off
    {
        return (RT_EOK);
    }
    if (bc26->sleep_status) //is sleep status
    {
        return (RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return (-RT_ERROR);
    }

    /* enable sleep mode */
    if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=1") != RT_EOK)

    {
        LOG_D("enable sleep fail.\"AT+QSCLK=1\" execute fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    /* enable PSM mode */
    if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=1,,,\"01000011\",\"00000001\"") != RT_EOK)

    {
        LOG_D("enable sleep fail.\"AT+CPSMS=1...\" execute fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+QRELLOCK") != RT_EOK)
    {
        LOG_D("startup entry into sleep fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    bc26->sleep_status = RT_TRUE;

    at_delete_resp(resp);
    return (RT_EOK);
}

static int bc26_wakeup(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_bc26 *bc26 = RT_NULL;

    bc26 = (struct at_device_bc26 *)device->user_data;
    if (!bc26->power_status) //power off
    {
        LOG_E("the power is off and the wake-up cannot be performed");
        return (-RT_ERROR);
    }
    if (!bc26->sleep_status) //no sleep status
    {
        return (RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return (-RT_ERROR);
    }

    if (bc26->power_pin != -1)
    {
        rt_pin_write(bc26->power_pin, PIN_HIGH);
        rt_thread_mdelay(100);
        rt_pin_write(bc26->power_pin, PIN_LOW);
        rt_thread_mdelay(200);
        rt_pin_write(bc26->power_pin, PIN_HIGH);
    }

    /* disable sleep mode */
    if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=0") != RT_EOK)
    {
        LOG_D("wake up fail. \"AT+QSCLK=0\" execute fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    /* disable PSM mode  */
    if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=0") != RT_EOK)
    {
        LOG_D("wake up fail.\"AT+CPSMS=0\" execute fail.");
        at_delete_resp(resp);
        return (-RT_ERROR);
    }

    bc26->sleep_status = RT_FALSE;

    at_delete_resp(resp);
    return (RT_EOK);
}

static int bc26_check_link_status(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_bc26 *bc26 = RT_NULL;
    int result = -RT_ERROR;

    bc26 = (struct at_device_bc26 *)device->user_data;
    if (!bc26->power_status) //power off
    {
        LOG_D("the power is off.");
        return (-RT_ERROR);
    }
    if (bc26->sleep_status) //is sleep status
    {
        if (bc26->power_pin != -1)
        {
            rt_pin_write(bc26->power_pin, PIN_HIGH);
            rt_thread_mdelay(100);
            rt_pin_write(bc26->power_pin, PIN_LOW);
            rt_thread_mdelay(200);
            rt_pin_write(bc26->power_pin, PIN_HIGH);
        }
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return (-RT_ERROR);
    }

    result = -RT_ERROR;
    if (at_obj_exec_cmd(device->client, resp, "AT+CGREG?") == RT_EOK)
    {
        int link_stat = 0;
        if (at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %*d,%d", &link_stat) > 0)
        {
            if (link_stat == 1 || link_stat == 5)
            {
                result = RT_EOK;
            }
        }
    }

    if (bc26->sleep_status) //is sleep status
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+QRELLOCK") != RT_EOK)
        {
            LOG_D("startup entry into sleep fail.");
        }
    }

    at_delete_resp(resp);

    return (result);
}

/* =============================  bc26 network interface operations ============================= */
/* set bc26 network interface device status and address information */
static int bc26_netdev_set_info(struct netdev *netdev)
{
#define BC26_INFO_RESP_SIZE 128
#define BC26_INFO_RESP_TIMO rt_tick_from_millisecond(300)

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

    resp = at_create_resp(BC26_INFO_RESP_SIZE, 0, BC26_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
#define BC26_NETDEV_HWADDR_LEN 8
#define BC26_IMEI_LEN 15

        char imei[BC26_IMEI_LEN] = {0};
        int i = 0, j = 0;

        /* send "AT+GSN" commond to get device IMEI */
        if (at_obj_exec_cmd(device->client, resp, "AT+GSN") != RT_EOK)
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

        netdev->hwaddr_len = BC26_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < BC26_NETDEV_HWADDR_LEN && j < BC26_IMEI_LEN; i++, j += 2)
        {
            if (j != BC26_IMEI_LEN - 1)
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

        /* send "AT+CGPADDR=1" commond to get IP address */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "+CGPADDR: 1,<IP_address>" */
        if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR: %*d,%s", ipaddr) <= 0)
        {
            LOG_E("%s device \"AT+CGPADDR=1\" cmd error.", device->name);
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
#define DNS_ADDR_SIZE_MAX 16
        char dns_server1[DNS_ADDR_SIZE_MAX] = {0}, dns_server2[DNS_ADDR_SIZE_MAX] = {0};

        /* send "AT+QIDNSCFG=1" commond to get DNS servers address */
        if (at_obj_exec_cmd(device->client, resp, "AT+QIDNSCFG=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "+QIDNSCFG: <contextID>,<pridnsaddr>,<secdnsaddr>" */
        if (at_resp_parse_line_args_by_kw(resp, "+QIDNSCFG:", "+QIDNSCFG: 1,\"%[^\"]\",\"%[^\"]\"",
                                          dns_server1, dns_server2) <= 0)
        {
            LOG_E("%s device prase \"AT+QIDNSCFG=1\" cmd error.", device->name);
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

static void bc26_check_link_status_entry(void *parameter)
{
#define BC26_LINK_DELAY_TIME (60 * RT_TICK_PER_SECOND)

    rt_bool_t is_link_up;
    struct at_device *device = RT_NULL;
    struct netdev *netdev = (struct netdev *)parameter;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    while (1)
    {
        is_link_up = (bc26_check_link_status(device) == RT_EOK);

        netdev_low_level_set_link_status(netdev, is_link_up);

        rt_thread_delay(BC26_LINK_DELAY_TIME);
    }
}

static int bc26_netdev_check_link_status(struct netdev *netdev)
{
#define BC26_LINK_THREAD_TICK 20
#define BC26_LINK_THREAD_STACK_SIZE (1024 + 512)
#define BC26_LINK_THREAD_PRIORITY (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    /* create bc26 link status polling thread  */
    tid = rt_thread_create(tname, bc26_check_link_status_entry, (void *)netdev,
                           BC26_LINK_THREAD_STACK_SIZE, BC26_LINK_THREAD_PRIORITY, BC26_LINK_THREAD_TICK);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int bc26_net_init(struct at_device *device);

static int bc26_netdev_set_up(struct netdev *netdev)
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
        bc26_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int bc26_netdev_set_down(struct netdev *netdev)
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
        bc26_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int bc26_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define BC26_DNS_RESP_LEN 64
#define BC26_DNS_RESP_TIMEO rt_tick_from_millisecond(300)

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

    resp = at_create_resp(BC26_DNS_RESP_LEN, 0, BC26_DNS_RESP_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* send "AT+QIDNSCFG=<pri_dns>[,<sec_dns>]" commond to set dns servers */
    if (at_obj_exec_cmd(device->client, resp, "AT+QIDNSCFG=%d,%s",
                        dns_num, inet_ntoa(*dns_server)) != RT_EOK)
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
static int bc26_netdev_ping(struct netdev *netdev, const char *host,
                            size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define BC26_PING_RESP_SIZE 128
#define BC26_PING_IP_SIZE 16
#define BC26_PING_TIMEO (5 * RT_TICK_PER_SECOND)

    rt_err_t result = RT_EOK;
    int response = -1, recv_data_len, ping_time, ttl;
    char ip_addr[BC26_PING_IP_SIZE] = {0};
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

    resp = at_create_resp(BC26_PING_RESP_SIZE, 4, BC26_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create");
        return -RT_ENOMEM;
    }

    /* send "AT+QPING=<contextID>"<host>"[,[<timeout>][,<pingnum>]]" commond to send ping request */
    if (at_obj_exec_cmd(device->client, resp, "AT+QPING=1,\"%s\",%d,1", host, timeout / RT_TICK_PER_SECOND) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    at_resp_parse_line_args_by_kw(resp, "+QPING:", "+QPING:%d", &response);
    /* Received the ping response from the server */
    if (response == 0)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+QPING:", "+QPING:%d,\"%[^\"]\",%d,%d,%d",
                                          &response, ip_addr, &recv_data_len, &ping_time, &ttl) <= 0)
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
        ping_resp->ticks = ping_time;
        ping_resp->ttl = ttl;
        result = RT_EOK;
        break;
    case 569:
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

const struct netdev_ops bc26_netdev_ops =
    {
        bc26_netdev_set_up,
        bc26_netdev_set_down,

        RT_NULL,
        bc26_netdev_set_dns_server,
        RT_NULL,

#ifdef NETDEV_USING_PING
        bc26_netdev_ping,
#endif
        RT_NULL,
};

static struct netdev *bc26_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU 1500
#define HWADDR_LEN 8
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
    netdev->ops = &bc26_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev * netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  bc26 device operations ============================= */

/* initialize for bc26 */
static void bc26_init_thread_entry(void *parameter)
{
#define INIT_RETRY 5
#define CPIN_RETRY 5
#define CSQ_RETRY 20
#define CGREG_RETRY 50
#define IPADDR_RETRY 10

    int i;
    int retry_num = INIT_RETRY;
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
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
        /* power on the bc26 device */
        bc26_power_on(device);
        rt_thread_mdelay(1000);

        /* wait bc26 startup finish, send AT every 500ms, if receive OK, SYNC success*/
        if (at_client_obj_wait_connect(client, BC26_WAIT_CONNECT_TIME))
        {
            result = -RT_ETIMEOUT;
            goto __exit;
        }

        /* disable echo */
        if (at_obj_exec_cmd(device->client, resp, "ATE0") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* disable sleep mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=0") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* disable eDRX mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+CEDRXS=0") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* disable PSM mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=0") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* Get the baudrate */
        if (at_obj_exec_cmd(device->client, resp, "AT+IPR?") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }
        at_resp_parse_line_args_by_kw(resp, "+IPR:", "+IPR: %d", &i);
        LOG_D("%s device baudrate %d", device->name, i);

        /* get module version */
        if (at_obj_exec_cmd(device->client, resp, "ATI") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }
        for (i = 0; i < (int)resp->line_counts - 1; i++)
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

                if (at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %d,%d", &signal_strength, &err_rate) > 0)
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
            if (at_obj_exec_cmd(device->client, resp, "AT+CGREG?") == RT_EOK)
            {
                int link_stat = 0;

                if (at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %*d,%d", &link_stat) > 0)
                {
                    if ((link_stat == 1) || (link_stat == 5))
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
            if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR=1") == RT_EOK)
            {
#define IP_ADDR_SIZE_MAX 16
                char ipaddr[IP_ADDR_SIZE_MAX] = {0};

                /* parse response data "+CGPADDR: 1,<IP_address>" */
                if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR: %*d,%s", ipaddr) > 0)
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
            /* power off the bc26 device */
            bc26_power_off(device);
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
        bc26_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            bc26_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success.", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
}

/* bc26 device network initialize */
static int bc26_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_BC26_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("bc26_net", bc26_init_thread_entry, (void *)device,
                           BC26_THREAD_STACK_SIZE, BC26_THREAD_PRIORITY, 20);
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
    bc26_init_thread_entry(device);
#endif /* AT_DEVICE_BC26_INIT_ASYN */

    return RT_EOK;
}

static int bc26_init(struct at_device *device)
{
    struct at_device_bc26 *bc26 = RT_NULL;

    RT_ASSERT(device);

    bc26 = (struct at_device_bc26 *)device->user_data;
    bc26->power_status = RT_FALSE; //default power is off.
    bc26->sleep_status = RT_FALSE; //default sleep is disabled.

    /* initialize AT client */
    at_client_init(bc26->client_name, bc26->recv_line_num);

    device->client = at_client_get(bc26->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", bc26->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
#ifdef AT_USING_SOCKET
    bc26_socket_init(device);
#endif

    /* add bc26 device to the netdev list */
    device->netdev = bc26_netdev_add(bc26->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", bc26->device_name);
        return -RT_ERROR;
    }

    /* initialize bc26 pin configuration */
    if (bc26->power_pin != -1)
    {
        rt_pin_write(bc26->power_pin, PIN_LOW);
        rt_pin_mode(bc26->power_pin, PIN_MODE_OUTPUT);
    }

    /* initialize bc26 device network */
    return bc26_netdev_set_up(device->netdev);
}

static int bc26_deinit(struct at_device *device)
{
    RT_ASSERT(device);

    return bc26_netdev_set_down(device->netdev);
}

static int bc26_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_SLEEP:
        result = bc26_sleep(device);
        break;
    case AT_DEVICE_CTRL_WAKEUP:
        result = bc26_wakeup(device);
        break;
    case AT_DEVICE_CTRL_POWER_ON:
    case AT_DEVICE_CTRL_POWER_OFF:
    case AT_DEVICE_CTRL_RESET:
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

const struct at_device_ops bc26_device_ops =
    {
        bc26_init,
        bc26_deinit,
        bc26_control,
};

static int bc26_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *)rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill bc26 device class object */
#ifdef AT_USING_SOCKET
    bc26_socket_class_register(class);
#endif
    class->device_ops = &bc26_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_BC26);
}
INIT_DEVICE_EXPORT(bc26_device_class_register);

#endif /* AT_DEVICE_USING_BC26 */
