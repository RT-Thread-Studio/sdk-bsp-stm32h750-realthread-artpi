/*
 * File      : at_device_ec200x.c
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

#include <at_device_ec200x.h>

#define LOG_TAG                         "at.dev.ec200x"
#include <at_log.h>

#ifdef AT_DEVICE_USING_EC200X

#define EC200X_WAIT_CONNECT_TIME          10000
#define EC200X_THREAD_STACK_SIZE          2048
#define EC200X_THREAD_PRIORITY            (RT_THREAD_PRIORITY_MAX/2)

static int ec200x_power_on(struct at_device *device)
{
    struct at_device_ec200x *ec200x = RT_NULL;

    ec200x = (struct at_device_ec200x *)device->user_data;

    if (ec200x->power_pin == -1)//no power on pin
    {
        return(RT_EOK);
    }
    if (ec200x->power_status_pin != -1)//use power status pin
    {
        ec200x->power_status = rt_pin_read(ec200x->power_status_pin);//read power status
    }
    if (ec200x->power_status)//power is on
    {
        return(RT_EOK);
    }

    rt_pin_write(ec200x->power_pin, PIN_HIGH);

    if (ec200x->power_status_pin != -1)//use power status pin
    {
        while (rt_pin_read(ec200x->power_status_pin) == PIN_LOW)
        {
            rt_thread_mdelay(10);
        }
    }

    rt_thread_mdelay(500);

    rt_pin_write(ec200x->power_pin, PIN_LOW);

    ec200x->power_status = RT_TRUE;

    return(RT_EOK);
}

static int ec200x_power_off(struct at_device *device)
{
    struct at_device_ec200x *ec200x = RT_NULL;

    ec200x = (struct at_device_ec200x *)device->user_data;

    if (ec200x->power_pin == -1)//no power on pin
    {
        return(RT_EOK);
    }
    if (ec200x->power_status_pin != -1)//use power status pin
    {
        ec200x->power_status = rt_pin_read(ec200x->power_status_pin);//read power status
    }
    if ( ! ec200x->power_status)//power is off
    {
        return(RT_EOK);
    }

    if (ec200x->power_status_pin != -1)//use power status pin
    {
        rt_pin_write(ec200x->power_pin, PIN_HIGH);
        rt_thread_mdelay(1000);
        rt_pin_write(ec200x->power_pin, PIN_LOW);

        while (rt_pin_read(ec200x->power_status_pin) == PIN_HIGH)//wait power down
        {
            rt_thread_mdelay(100);
        }
    }
    else
    {
        at_obj_exec_cmd(device->client, RT_NULL, "AT+QPOWD=0");
        rt_thread_mdelay(5*1000);
    }

    ec200x->power_status = RT_FALSE;

    return(RT_EOK);
}

static int ec200x_sleep(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_ec200x *ec200x = RT_NULL;

    ec200x = (struct at_device_ec200x *)device->user_data;
    if ( ! ec200x->power_status)//power off
    {
        return(RT_EOK);
    }
    if (ec200x->sleep_status)//is sleep status
    {
        return(RT_EOK);
    }
    if (ec200x->wakeup_pin == -1)//use wakeup pin
    {
        LOG_E("no config wakeup pin, can not entry into sleep mode.");
        return(-RT_ERROR);
    }
    /*
    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return(-RT_ERROR);
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=1") != RT_EOK)//enable sleep mode

    {
        LOG_D("enable sleep fail.\"AT+QSCLK=1\" execute fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }

    at_delete_resp(resp);
    */

    rt_pin_write(ec200x->wakeup_pin, PIN_HIGH);

    ec200x->sleep_status = RT_TRUE;

    return(RT_EOK);
}

static int ec200x_wakeup(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_ec200x *ec200x = RT_NULL;

    ec200x = (struct at_device_ec200x *)device->user_data;
    if ( ! ec200x->power_status)//power off
    {
        LOG_E("the power is off and the wake-up cannot be performed");
        return(-RT_ERROR);
    }
    if ( ! ec200x->sleep_status)//no sleep status
    {
        return(RT_EOK);
    }

    rt_pin_write(ec200x->wakeup_pin, PIN_LOW);
    rt_thread_mdelay(200);

    /*
    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return(-RT_ERROR);
    }
    if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=0") != RT_EOK)//disable sleep mode
    {
        LOG_D("wake up fail. \"AT+QSCLK=0\" execute fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }
    at_delete_resp(resp);
    */

    ec200x->sleep_status = RT_FALSE;

    return(RT_EOK);
}

static int ec200x_check_link_status(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_ec200x *ec200x = RT_NULL;
    int result = -RT_ERROR;

    ec200x = (struct at_device_ec200x *)device->user_data;
    if ( ! ec200x->power_status)//power off
    {
        LOG_D("the power is off.");
        return(-RT_ERROR);
    }
    if (ec200x->sleep_status)//is sleep status
    {
        rt_pin_write(ec200x->wakeup_pin, PIN_LOW);
        rt_thread_mdelay(200);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_D("no memory for resp create.");
        return(-RT_ERROR);
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

    at_delete_resp(resp);

    if (ec200x->sleep_status)//is sleep status
    {
        rt_pin_write(ec200x->wakeup_pin, PIN_HIGH);
    }

    return(result);
}


/* =============================  ec200x network interface operations ============================= */
/* set ec200x network interface device status and address information */
static int ec200x_netdev_set_info(struct netdev *netdev)
{
#define EC200X_INFO_RESP_SIZE      128
#define EC200X_INFO_RESP_TIMO      rt_tick_from_millisecond(1000)

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

    resp = at_create_resp(EC200X_INFO_RESP_SIZE, 0, EC200X_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define EC200X_NETDEV_HWADDR_LEN   8
        #define EC200X_IMEI_LEN            15

        char imei[EC200X_IMEI_LEN] = {0};
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

        netdev->hwaddr_len = EC200X_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < EC200X_NETDEV_HWADDR_LEN && j < EC200X_IMEI_LEN; i++, j+=2)
        {
            if (j != EC200X_IMEI_LEN - 1)
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

        /* send "AT+CGPADDR=1" commond to get IP address */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "+CGPADDR: 1,<IP_address>" */
        if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR: %*[^\"]\"%[^\"]", ipaddr) <= 0)
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
        #define DNS_ADDR_SIZE_MAX   16
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

static void ec200x_check_link_status_entry(void *parameter)
{
#define EC200X_LINK_DELAY_TIME    (60 * RT_TICK_PER_SECOND)

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
        rt_thread_delay(EC200X_LINK_DELAY_TIME);

        is_link_up = (ec200x_check_link_status(device) == RT_EOK);

        netdev_low_level_set_link_status(netdev, is_link_up);
    }
}

static int ec200x_netdev_check_link_status(struct netdev *netdev)
{
#define EC200X_LINK_THREAD_TICK           20
#define EC200X_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define EC200X_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    /* create ec200x link status polling thread  */
    tid = rt_thread_create(tname, ec200x_check_link_status_entry, (void *)netdev,
                           EC200X_LINK_THREAD_STACK_SIZE, EC200X_LINK_THREAD_PRIORITY, EC200X_LINK_THREAD_TICK);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int ec200x_net_init(struct at_device *device);

static int ec200x_netdev_set_up(struct netdev *netdev)
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
        ec200x_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int ec200x_netdev_set_down(struct netdev *netdev)
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
        ec200x_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int ec200x_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define EC200X_DNS_RESP_LEN    64
#define EC200X_DNS_RESP_TIMEO  rt_tick_from_millisecond(300)

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

    resp = at_create_resp(EC200X_DNS_RESP_LEN, 0, EC200X_DNS_RESP_TIMEO);
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
static int ec200x_netdev_ping(struct netdev *netdev, const char *host,
        size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define EC200X_PING_RESP_SIZE       128
#define EC200X_PING_IP_SIZE         16
#define EC200X_PING_TIMEO           (5 * RT_TICK_PER_SECOND)

    rt_err_t result = RT_EOK;
    int response = -1, recv_data_len, ping_time, ttl;
    char ip_addr[EC200X_PING_IP_SIZE] = {0};
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

    resp = at_create_resp(EC200X_PING_RESP_SIZE, 4, EC200X_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create");
        return -RT_ENOMEM;
    }

    /* send "AT+QPING=<contextID>"<host>"[,[<timeout>][,<pingnum>]]" commond to send ping request */
    if (at_obj_exec_cmd(device->client, resp, "AT+QPING=1,%s,%d,1", host, timeout / RT_TICK_PER_SECOND) < 0)
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



const struct netdev_ops ec200x_netdev_ops =
{
    ec200x_netdev_set_up,
    ec200x_netdev_set_down,

    RT_NULL,
    ec200x_netdev_set_dns_server,
    RT_NULL,

#ifdef NETDEV_USING_PING
    ec200x_netdev_ping,
#endif
    RT_NULL,
};

static struct netdev *ec200x_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          8
    struct netdev *netdev = RT_NULL;

    netdev = netdev_get_by_name(netdev_name);
    if(netdev != RT_NULL)
    {
        return netdev;
    }

    netdev = (struct netdev *)rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = ETHERNET_MTU;
    netdev->ops = &ec200x_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  ec200x device operations ============================= */

/* initialize for ec200x */
static void ec200x_init_thread_entry(void *parameter)
{
#define RESP_SIZE                      128
#define INIT_RETRY                     5
#define CPIN_RETRY                     10
#define CSQ_RETRY                      20
#define CGREG_RETRY                    50
#define IPADDR_RETRY                   10

    int i;
    int retry_num = INIT_RETRY;
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = (struct at_device *) parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(RESP_SIZE, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    LOG_D("start init %s device.", device->name);

    while (retry_num--)
    {
        /* power on the ec200x device */
        ec200x_power_on(device);
        rt_thread_mdelay(1000);

        /* wait ec200x startup finish, send AT every 500ms, if receive OK, SYNC success*/
        if (at_client_obj_wait_connect(client, EC200X_WAIT_CONNECT_TIME))
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

        if (((struct at_device_ec200x *)(device->user_data))->wakeup_pin != -1)//use wakeup pin
        {
            if (at_obj_exec_cmd(device->client, resp, "AT+QSCLK=1") != RT_EOK)// enable sleep mode fail
            {
                result = -RT_ERROR;
                goto __exit;
            }
        }

        /* Close Echo the Data */
        if (at_obj_exec_cmd(device->client, resp, "AT+QISDE=0") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* Deactivate context profile */
        resp = at_resp_set_info(resp, RESP_SIZE, 0, rt_tick_from_millisecond(40*1000));
        if (at_obj_exec_cmd(device->client, resp, "AT+QIDEACT=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* Activate context profile */
        resp = at_resp_set_info(resp, RESP_SIZE, 0, rt_tick_from_millisecond(150*1000));
        if (at_obj_exec_cmd(device->client, resp, "AT+QIACT=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            /* power off the ec200x device */
            ec200x_power_off(device);
            rt_thread_mdelay(3000);

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
        ec200x_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            ec200x_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success.", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
}

/* ec200x device network initialize */
static int ec200x_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_EC200X_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("ec200x_net", ec200x_init_thread_entry, (void *)device,
                           EC200X_THREAD_STACK_SIZE, EC200X_THREAD_PRIORITY, 20);
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
    ec200x_init_thread_entry(device);
#endif /* AT_DEVICE_EC200X_INIT_ASYN */

    return RT_EOK;
}

static int ec200x_init(struct at_device *device)
{
    struct at_device_ec200x *ec200x = RT_NULL;

    RT_ASSERT(device);

    ec200x = (struct at_device_ec200x *) device->user_data;
    ec200x->power_status = RT_FALSE;//default power is off.
    ec200x->sleep_status = RT_FALSE;//default sleep is disabled.

    /* initialize AT client */
    at_client_init(ec200x->client_name, ec200x->recv_line_num);

    device->client = at_client_get(ec200x->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", ec200x->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
#ifdef AT_USING_SOCKET
    ec200x_socket_init(device);
#endif

    /* add ec200x device to the netdev list */
    device->netdev = ec200x_netdev_add(ec200x->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", ec200x->device_name);
        return -RT_ERROR;
    }

    /* initialize ec200x pin configuration */
    if (ec200x->power_pin != -1)
    {
        rt_pin_write(ec200x->power_pin, PIN_LOW);
        rt_pin_mode(ec200x->power_pin, PIN_MODE_OUTPUT);
    }
    if (ec200x->power_status_pin != -1)
    {
        rt_pin_mode(ec200x->power_status_pin, PIN_MODE_INPUT);
    }
    if (ec200x->wakeup_pin != -1)
    {
        rt_pin_write(ec200x->wakeup_pin, PIN_LOW);
        rt_pin_mode(ec200x->wakeup_pin, PIN_MODE_OUTPUT);
    }

    /* initialize ec200x device network */
    return ec200x_netdev_set_up(device->netdev);
}

static int ec200x_deinit(struct at_device *device)
{
    RT_ASSERT(device);

    return ec200x_netdev_set_down(device->netdev);
}

static int ec200x_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_SLEEP:
        result = ec200x_sleep(device);
        break;
    case AT_DEVICE_CTRL_WAKEUP:
        result = ec200x_wakeup(device);
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

const struct at_device_ops ec200x_device_ops =
{
    ec200x_init,
    ec200x_deinit,
    ec200x_control,
};

static int ec200x_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill ec200x device class object */
#ifdef AT_USING_SOCKET
    ec200x_socket_class_register(class);
#endif
    class->device_ops = &ec200x_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_EC200X);
}
INIT_DEVICE_EXPORT(ec200x_device_class_register);

#endif /* AT_DEVICE_USING_EC200X */

