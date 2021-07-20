/*
 * File      : at_device_me3616.c
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

#define LOG_TAG                         "at.dev.me3616"
#include <at_log.h>

#ifdef AT_DEVICE_USING_ME3616

#ifndef ME3616_DEEP_SLEEP_EN
#define ME3616_DEEP_SLEEP_EN              0//module support deep sleep mode
#endif

#define ME3616_WAIT_CONNECT_TIME          5000
#define ME3616_THREAD_STACK_SIZE          2048
#define ME3616_THREAD_PRIORITY            (RT_THREAD_PRIORITY_MAX/2)

static int me3616_power_on(struct at_device *device)
{
    struct at_device_me3616 *me3616 = RT_NULL;

    me3616 = (struct at_device_me3616 *)device->user_data;
    me3616->power_status = RT_TRUE;

    /* not nead to set pin configuration for me3616 device power on */
    if (me3616->power_pin == -1)
    {
        return(RT_EOK);
    }

    rt_pin_write(me3616->power_pin, PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(me3616->power_pin, PIN_LOW);

    LOG_D("power on success.");

    return(RT_EOK);
}

static int me3616_power_off(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_me3616 *me3616 = RT_NULL;

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    at_obj_exec_cmd(device->client, resp, "AT+ZTURNOFF");//command response does not have "\r\n" after "OK"
    /*if (at_obj_exec_cmd(device->client, resp, "AT+ZTURNOFF") != RT_EOK)
    {
        LOG_D("power off fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }*/

    at_delete_resp(resp);

    me3616 = (struct at_device_me3616 *)device->user_data;
    me3616->power_status = RT_FALSE;

    LOG_D("power off success.");

    return(RT_EOK);
}

static int me3616_sleep(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_me3616 *me3616 = RT_NULL;

    me3616 = (struct at_device_me3616 *)device->user_data;
    if ( ! me3616->power_status)//power off
    {
        return(RT_EOK);
    }
    if (me3616->sleep_status)//is sleep status
    {
        return(RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=1,,,\"00111110\",\"00000001\"") != RT_EOK)
    {
        LOG_D("enable sleep fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }

    #if ME3616_DEEP_SLEEP_EN
    if (at_obj_exec_cmd(device->client, resp, "AT+ZSLR") != RT_EOK)
    {
        LOG_D("startup entry into sleep fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }
    #endif

    at_delete_resp(resp);
    me3616->sleep_status = RT_TRUE;

    LOG_D("sleep success.");

    return(RT_EOK);
}

static int me3616_wakeup(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_me3616 *me3616 = RT_NULL;

    me3616 = (struct at_device_me3616 *)device->user_data;
    if ( ! me3616->power_status)//power off
    {
        LOG_E("the power is off and the wake-up cannot be performed");
        return(-RT_ERROR);
    }
    if ( ! me3616->sleep_status)//no sleep status
    {
        return(RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    #if ME3616_DEEP_SLEEP_EN
    if (me3616->power_pin != -1)
    {
        rt_pin_write(me3616->power_pin, PIN_HIGH);
        rt_thread_mdelay(100);
        rt_pin_write(me3616->power_pin, PIN_LOW);
        rt_thread_mdelay(200);
    }
    #endif

    if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=0") != RT_EOK)
    {
        LOG_D("wake up fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }

    at_delete_resp(resp);
    me3616->sleep_status = RT_FALSE;

    LOG_D("wake up success.");

    return(RT_EOK);
}

static int me3616_check_link_status(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_me3616 *me3616 = RT_NULL;
    int result = -RT_ERROR;

    RT_ASSERT(device);

    me3616 = (struct at_device_me3616 *)device->user_data;
    if ( ! me3616->power_status)//power off
    {
        LOG_D("the power is off.");
        return(-RT_ERROR);
    }

    #if ME3616_DEEP_SLEEP_EN
    if (me3616->sleep_status)//is sleep status
    {
        if (me3616->power_pin != -1)
        {
            rt_pin_write(me3616->power_pin, PIN_HIGH);
            rt_thread_mdelay(100);
            rt_pin_write(me3616->power_pin, PIN_LOW);
            rt_thread_mdelay(200);
        }
    }
    #endif

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
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

    #if ME3616_DEEP_SLEEP_EN
    if (me3616->sleep_status)//is sleep status
    {
        if (at_obj_exec_cmd(device->client, resp, "AT+ZSLR") != RT_EOK)
        {
            LOG_D("startup entry into sleep fail.");
        }
    }
    #endif

    at_delete_resp(resp);

    return(result);
}


/* =============================  me3616 network interface operations ============================= */
/* set me3616 network interface device status and address information */
static int me3616_netdev_set_info(struct netdev *netdev)
{
#define ME3616_INFO_RESP_SIZE      128
#define ME3616_INFO_RESP_TIMO      rt_tick_from_millisecond(300)

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

    resp = at_create_resp(ME3616_INFO_RESP_SIZE, 0, ME3616_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define ME3616_NETDEV_HWADDR_LEN   8
        #define ME3616_IMEI_LEN            15

        char imei[ME3616_IMEI_LEN] = {0};
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

        netdev->hwaddr_len = ME3616_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < ME3616_NETDEV_HWADDR_LEN && j < ME3616_IMEI_LEN; i++, j+=2)
        {
            if (j != ME3616_IMEI_LEN - 1)
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

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static void me3616_check_link_status_entry(void *parameter)
{
#define ME3616_LINK_DELAY_TIME    (60 * RT_TICK_PER_SECOND)

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
        is_link_up = (me3616_check_link_status(device) == RT_EOK);

        netdev_low_level_set_link_status(netdev, is_link_up);

        rt_thread_delay(ME3616_LINK_DELAY_TIME);
    }
}

static int me3616_netdev_check_link_status(struct netdev *netdev)
{
#define ME3616_LINK_THREAD_TICK           20
#define ME3616_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define ME3616_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    /* create me3616 link status polling thread  */
    tid = rt_thread_create(tname, me3616_check_link_status_entry, (void *)netdev,
                           ME3616_LINK_THREAD_STACK_SIZE, ME3616_LINK_THREAD_PRIORITY, ME3616_LINK_THREAD_TICK);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int me3616_net_init(struct at_device *device);

static int me3616_netdev_set_up(struct netdev *netdev)
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
        me3616_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int me3616_netdev_set_down(struct netdev *netdev)
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
        me3616_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

#ifdef NETDEV_USING_PING
static int me3616_netdev_ping(struct netdev *netdev, const char *host,
        size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define ME3616_PING_RESP_SIZE       256
#define ME3616_PING_IP_SIZE         16
#define ME3616_PING_TIMEO           (10 * RT_TICK_PER_SECOND)

    rt_err_t result = RT_EOK;
    int response = -1, recv_data_len, ping_time, ttl;
    char ip_addr[ME3616_PING_IP_SIZE] = {0};
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

    resp = at_create_resp(ME3616_PING_RESP_SIZE, 0, ME3616_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create");
        return -RT_ENOMEM;
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+EDNS=\"%s\"", host) != RT_EOK)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "IPV4:", "IPV4:%s\r", ip_addr) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    at_resp_set_info(resp, ME3616_PING_RESP_SIZE, 8, timeout);

    /* send "AT+PING=<host>[-l/L <p_size>] [-n/N <count>][-w/W <time>][-6][-i <value>][-d<value>] */
    if (at_obj_exec_cmd(device->client, resp, "AT+PING=%s -d 1 -n 1 -w %d", ip_addr, timeout) < 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "received=", "%*[^=]=%d%*[^=]=%d%*[^=]=%d",
                                        &recv_data_len, &ping_time, &ttl) <= 0)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    inet_aton(ip_addr, &(ping_resp->ip_addr));
    ping_resp->data_len = recv_data_len;
    ping_resp->ticks = ping_time;
    ping_resp->ttl = ttl;

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

const struct netdev_ops me3616_netdev_ops =
{
    me3616_netdev_set_up,
    me3616_netdev_set_down,

    RT_NULL,
    RT_NULL,
    RT_NULL,

#ifdef NETDEV_USING_PING
    me3616_netdev_ping,
    RT_NULL,
#endif
    RT_NULL,
};

static struct netdev *me3616_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          8
    struct netdev *netdev = RT_NULL;

    netdev = netdev_get_by_name(netdev_name);
    if(netdev != RT_NULL)
    {
        return(netdev);
    }

    netdev = (struct netdev *)rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = ETHERNET_MTU;
    netdev->ops = &me3616_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  me3616 device operations ============================= */

/* initialize for me3616 */
static void me3616_init_thread_entry(void *parameter)
{
#define INIT_RETRY                     5
#define CPIN_RETRY                     5
#define CSQ_RETRY                      20
#define CGREG_RETRY                    60
#define IPADDR_RETRY                   10

    int i;
    int retry_num = INIT_RETRY;
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = (struct at_device *) parameter;
    struct at_client *client = device->client;

    resp = at_create_resp(256, 0, rt_tick_from_millisecond(500));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    LOG_D("start init %s device.", device->name);

    while (retry_num--)
    {
        /* power on the me3616 device */
        me3616_power_on(device);
        rt_thread_mdelay(2000);

        /* wait me3616 startup finish, send AT every 500ms, if receive OK, SYNC success*/
        if (at_client_obj_wait_connect(client, ME3616_WAIT_CONNECT_TIME))
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

        /* disable PSM mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+CPSMS=0") != RT_EOK)
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

        /* disable low power mode  */
        if (at_obj_exec_cmd(device->client, resp, "AT+ESOSETRPT=1") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* disable sleep function  */
        if (at_obj_exec_cmd(device->client, resp, "AT+ZSLR=0") != RT_EOK)
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
        at_resp_parse_line_args_by_kw(resp, "+IPR:", "+IPR: %d\r", &i);
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

        /* check the GPRS network IP address */
        for (i = 0; i < IPADDR_RETRY; i++)
        {
            rt_thread_mdelay(1000);
            if (at_obj_exec_cmd(device->client, resp, "AT+CGPADDR=1") == RT_EOK)
            {
                #define IP_ADDR_SIZE_MAX    16
                char ipaddr[IP_ADDR_SIZE_MAX] = {0};

                /* parse response data "+CGPADDR: 1,<IP_address>" */
                if (at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR: %*[^\"]\"%[^\"]", ipaddr) > 0)
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
            /* power off the me3616 device */
            me3616_power_off(device);
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
        me3616_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            me3616_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success.", device->name);
    }
    else
    {
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
}

/* me3616 device network initialize */
static int me3616_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_ME3616_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("me3616_net", me3616_init_thread_entry, (void *)device,
                           ME3616_THREAD_STACK_SIZE, ME3616_THREAD_PRIORITY, 20);
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
    me3616_init_thread_entry(device);
#endif /* AT_DEVICE_ME3616_INIT_ASYN */

    return RT_EOK;
}

static int me3616_init(struct at_device *device)
{
    struct at_device_me3616 *me3616 = RT_NULL;

    RT_ASSERT(device);

    me3616 = (struct at_device_me3616 *) device->user_data;
    me3616->power_status = RT_FALSE;//default power is off.
    me3616->sleep_status = RT_FALSE;//default sleep is disabled.

    /* initialize AT client */
    at_client_init(me3616->client_name, me3616->recv_line_num);

    device->client = at_client_get(me3616->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", me3616->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
#ifdef AT_USING_SOCKET
    me3616_socket_init(device);
#endif

    /* add me3616 device to the netdev list */
    device->netdev = me3616_netdev_add(me3616->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", me3616->device_name);
        return -RT_ERROR;
    }

    /* initialize me3616 pin configuration */
    if (me3616->power_pin != -1)
    {
        rt_pin_write(me3616->power_pin, PIN_LOW);
        rt_pin_mode(me3616->power_pin, PIN_MODE_OUTPUT);
    }

    /* initialize me3616 device network */
    return me3616_netdev_set_up(device->netdev);
}

static int me3616_deinit(struct at_device *device)
{
    RT_ASSERT(device);

    return me3616_netdev_set_down(device->netdev);
}

static int me3616_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_SLEEP:
        result = me3616_sleep(device);
        break;
    case AT_DEVICE_CTRL_WAKEUP:
        result = me3616_wakeup(device);
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

const struct at_device_ops me3616_device_ops =
{
    me3616_init,
    me3616_deinit,
    me3616_control,
};

static int me3616_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill me3616 device class object */
#ifdef AT_USING_SOCKET
    me3616_socket_class_register(class);
#endif
    class->device_ops = &me3616_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_ME3616);
}
INIT_DEVICE_EXPORT(me3616_device_class_register);

#endif /* AT_DEVICE_USING_ME3616 */

