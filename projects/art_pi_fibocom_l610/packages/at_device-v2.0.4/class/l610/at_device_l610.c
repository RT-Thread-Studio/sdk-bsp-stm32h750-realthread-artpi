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
#include <ctype.h>

#include <at_device_l610.h>

#define LOG_TAG                     "at.dev.l610"
#include <at_log.h>



#ifndef L610_DEEP_SLEEP_EN
#define L610_DEEP_SLEEP_EN          0 //module support sleep mode
#endif

#ifdef AT_DEVICE_USING_L610

#define L610_WAIT_CONNECT_TIME      5000
#define L610_THREAD_STACK_SIZE      2048+1024
#define L610_THREAD_PRIORITY        (RT_THREAD_PRIORITY_MAX/2)



static char *CSTT_CHINA_MOBILE  = "AT+MIPCALL=1,\"CMNET\"";
static char *CSTT_CHINA_UNICOM  = "AT+MIPCALL=1,\"3GNET\"";
static char *CSTT_CHINA_TELECOM = "AT+MIPCALL=1,\"CTNET\"";


static int l610_power_on(struct at_device *device)
{
    struct at_device_l610 *l610= RT_NULL;

        l610 = (struct at_device_l610 *) device->user_data;
        l610->power_status = RT_TRUE;

     /* not nead to set pin configuration for me3616 device power on */
    if (l610->power_pin == -1)
    {
        return(RT_EOK);
    }

    rt_pin_write(l610->power_pin, PIN_LOW);
    rt_thread_mdelay(2000);
    rt_pin_write(l610->power_pin, PIN_HIGH);
    LOG_D("power on success.");

    return(RT_EOK);
}

static int l610_power_off(struct at_device *device)
{
    struct at_device_l610 *l610 = RT_NULL;

    l610 = (struct at_device_l610 *) device->user_data;

    /* not nead to set pin configuration for m26 device power on */
    if (l610->power_pin == -1)
    {
        return RT_EOK;
    }

    rt_pin_write(l610->power_pin, PIN_LOW);
    rt_thread_mdelay(2000);
    rt_pin_write(l610->power_pin, PIN_HIGH);

        l610->power_status = RT_FALSE;
        LOG_D("power off success.");
        return(RT_EOK);
}

static int l610_sleep(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_l610 *l610 = RT_NULL;

    l610 = (struct at_device_l610 *)device->user_data;
    if ( ! l610->power_status)//power off
    {
        return(RT_EOK);
    }
    if (l610->sleep_status)//is sleep status
    {
        return(RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    if (at_obj_exec_cmd(device->client, resp, "AT+GTWAKE=1,2") != RT_EOK)
    {
        LOG_D("enable sleep fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }

   #if L610_DEEP_SLEEP_EN
    if (at_obj_exec_cmd(device->client, resp, "ATS24=1") != RT_EOK)
    {
        LOG_D("startup entry into sleep fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }
    #endif
    at_delete_resp(resp);
    l610->sleep_status = RT_TRUE;

    LOG_D("sleep success.");

    return(RT_EOK);
}


static int l610_wakeup(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_l610 *l610 = RT_NULL;

    l610 = (struct at_device_l610 *)device->user_data;
    if ( ! l610->power_status)//power off
    {
        LOG_E("the power is off and the wake-up cannot be performed");
        return(-RT_ERROR);
    }
    if ( ! l610->sleep_status)//no sleep status
    {
    return(RT_EOK);
    }

    resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return(-RT_ERROR);
    }

    #if L610_DEEP_SLEEP_EN
    if (l610->power_pin != -1)
    {
        rt_pin_write(l610->power_pin, PIN_LOW);
        rt_thread_mdelay(2000);
        rt_pin_write(l610->power_pin, PIN_HIGH);
        rt_thread_mdelay(100);
    }
    #endif

    if (at_obj_exec_cmd(device->client, resp, "AT+GTWAKE=0,2") != RT_EOK)
    {
        LOG_D("wake up fail.");
        at_delete_resp(resp);
        return(-RT_ERROR);
    }

    at_delete_resp(resp);
    l610->sleep_status = RT_FALSE;

    LOG_D("wake up success.");

    return(RT_EOK);
}



static int l610_check_link_status(struct at_device *device)
{
    at_response_t resp = RT_NULL;
    struct at_device_l610 *l610 = RT_NULL;
    int result = -RT_ERROR;

    RT_ASSERT(device);

    l610 = (struct at_device_l610 *)device->user_data;
    if ( ! l610->power_status)//power off
    {
        LOG_D("the power is off.");
        return(-RT_ERROR);
    }

    #if L610_DEEP_SLEEP_EN
    if (l610->sleep_status)//is sleep status
    {
        if (l610->power_pin != -1)
        {
            rt_pin_write(l610->power_pin, PIN_LOW);
            rt_thread_mdelay(2000);
            rt_pin_write(l610->power_pin, PIN_HIGH);
            rt_thread_mdelay(100);
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

    #if L610_DEEP_SLEEP_EN
    if (l610->sleep_status)//is sleep status
    {
        if (at_obj_exec_cmd(device->client, resp, "ATS24=1") != RT_EOK)
        {
            LOG_D("startup entry into sleep fail.");
        }
    }
    #endif

    at_delete_resp(resp);

    return(result);
}



/* =============================  l610 network interface operations ============================= */

/* set l610 network interface device status and address information */
static int l610_netdev_set_info(struct netdev *netdev)
{
#define L610_IMEI_RESP_SIZE      64
#define L610_IPADDR_RESP_SIZE    32
#define L610_DNS_RESP_SIZE       96
#define L610_INFO_RESP_TIMO      rt_tick_from_millisecond(300)

    int result = RT_EOK;
    ip_addr_t addr;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.");
        return -RT_ERROR;
    }
    /* set network interface device status */
    netdev_low_level_set_status(netdev, RT_TRUE);
    netdev_low_level_set_link_status(netdev, RT_TRUE);
    netdev_low_level_set_dhcp_status(netdev, RT_TRUE);

    resp = at_create_resp(L610_IMEI_RESP_SIZE, 0, L610_INFO_RESP_TIMO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* set network interface device hardware address(IMEI) */
    {
        #define L610_NETDEV_HWADDR_LEN   8
        #define L610_IMEI_LEN            15

        char imei[L610_IMEI_LEN] = {0};
        int i = 0, j = 0;

        /* send "AT+GSN" commond to get device IMEI */
        if (at_obj_exec_cmd(device->client, resp, "AT+CGSN") < 0)
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

        LOG_D("%s IMEI : %s", device->name, imei);

        netdev->hwaddr_len = L610_NETDEV_HWADDR_LEN;
        /* get hardware address by IMEI */
        for (i = 0, j = 0; i < L610_NETDEV_HWADDR_LEN && j < L610_IMEI_LEN; i++, j += 2)
        {
            if (j != L610_IMEI_LEN - 1)
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
        if (at_obj_exec_cmd(device->client, resp, "AT+MIPCALL?") != RT_EOK)
        {
            result = -RT_ERROR;
            goto __exit;
        }

        /* parse response data "+MIPCALL: 1,<IP_address>" */
        if (at_resp_parse_line_args_by_kw(resp, "+MIPCALL: ","%*[^,],%s",ipaddr) <= 0)
        {
            LOG_E("%s device \"AT+MIPCALL?\" cmd error.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
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

static void l610_check_link_status_entry(void *parameter)
{

#define L610_LINK_DELAY_TIME  (30 * RT_TICK_PER_SECOND)

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
        /* send "AT+CGREG?" commond  to check netweork interface device link status */
        is_link_up = (l610_check_link_status(device) == RT_EOK);

        netdev_low_level_set_link_status(netdev, is_link_up);
        rt_thread_mdelay(L610_LINK_DELAY_TIME);
    }
}

static int l610_netdev_check_link_status(struct netdev *netdev)
{
#define L610_LINK_THREAD_TICK           20
#define L610_LINK_THREAD_STACK_SIZE     (1024 + 512)
#define L610_LINK_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX - 2)

    rt_thread_t tid;
    char tname[RT_NAME_MAX] = {0};

    RT_ASSERT(netdev);

    rt_snprintf(tname, RT_NAME_MAX, "%s", netdev->name);

    tid = rt_thread_create(tname, l610_check_link_status_entry, (void *) netdev,
            L610_LINK_THREAD_STACK_SIZE, L610_LINK_THREAD_PRIORITY, L610_LINK_THREAD_TICK);
    if (tid)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

static int l610_net_init(struct at_device *device);

static int l610_netdev_set_up(struct netdev *netdev)
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
        l610_net_init(device);
        device->is_init = RT_TRUE;

        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status.", netdev->name);
    }

    return RT_EOK;
}

static int l610_netdev_set_down(struct netdev *netdev)
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
        l610_power_off(device);
        device->is_init = RT_FALSE;

        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status.", netdev->name);
    }

    return RT_EOK;
}

static int l610_ping_domain_resolve(struct at_device *device, const char *name, char ip[16])
{
    int result = RT_EOK;
    char recv_ipv4[16] = { 0 };
    at_response_t resp = RT_NULL;

    /* The maximum response time is 14 seconds, affected by network status */
    resp = at_create_resp(128, 4, 14 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    //0: IPV4 address
    //1: IPV6 address
    //2: IPV4/IPV6 address
    //<IP>: resolved IPV4 or IPV6 address (string without double quotes)
    result = at_obj_exec_cmd(device->client, resp, "AT+MIPDNS=\"%s\",0", name);
    if (result != RT_EOK)
    {
        LOG_E("%s device \"AT+MIPDNS=\"%s\"\" cmd error.", device->name, name);
        goto __exit;
    }

    /* parse the third line of response data, get the IP address */
    if (at_resp_parse_line_args_by_kw(resp, "+MIPDNS: ", "%*[^,],%s", recv_ipv4) <= 0)
    {
        LOG_E("%s device prase \"AT+MIPDNS=\"%s\",2\" cmd error.", device->name, name);
        result = -RT_ERROR;
        goto __exit;
    }

      if (at_resp_get_line_by_kw(resp, "OK")==0)
    {
        LOG_E("l610_ping_domain_resolve fail");
        result = -RT_ERROR;
        goto __exit;
    }
    if (rt_strlen(recv_ipv4) < 8)
    {
        rt_thread_mdelay(100);
        /* resolve failed, maybe receive an URC CRLF */
    }
    else
    {
        rt_strncpy(ip, recv_ipv4, 15);
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
static int l610_netdev_ping(struct netdev *netdev, const char *host,
        size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define L610_PING_RESP_SIZE         128
#define L610_PING_IP_SIZE           16
#define L610_PING_TIMEO             (5 * RT_TICK_PER_SECOND)

#define L610_PING_ERR_TIME          L610_PING_TIMEO

    int result = RT_EOK;
    int time, ttl=64,i,type, err_code = 0;
    char ip_addr[L610_PING_IP_SIZE] = {0};
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_D("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    for (i = 0; i < rt_strlen(host) && !isalpha(host[i]); i++);

    if (i < strlen(host))
    {
        /* check domain name is usable */
        if (l610_ping_domain_resolve(device, host, ip_addr) < 0)
        {
            LOG_E("l610_ping_domain_resolve error");
            return -RT_ERROR;
        }
        rt_memset(ip_addr, 0x00, L610_PING_IP_SIZE);
    }

    resp = at_create_resp(L610_PING_RESP_SIZE,6, L610_PING_TIMEO);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;

    }

   // +MPING=<mode>[,<Destination_IP/hostname>[,<count>[,<size>[,<TTL>[,<TOS>[,<TimeOut>]]]]]]
    if (at_obj_exec_cmd(device->client, resp, "AT+MPING=1,\"%s\",1,%d,%d,0,%d", host,data_len,ttl,L610_PING_TIMEO) != RT_EOK)
    {
        result = -RT_ERROR;
        goto __exit;
    }

    if (at_resp_get_line_by_kw(resp, "OK")==0)
    {
        LOG_E("ping AT send fail");
        result = -RT_ERROR;
        goto __exit;
    }

    //+MPING: <Destination_IP>,<type>,<code> [,<RTT>]
    if (at_resp_parse_line_args_by_kw(resp, "+MPING: ","+MPING: \"%[^\"]\",%d,%d,%d\r\n",
             ip_addr,&type,&err_code,&time)<= 0)
    {
        LOG_E("+MPING error ");
        result = -RT_ERROR;
        goto __exit;
    }

    /* the ping request timeout expires*/
    if (time>=L610_PING_ERR_TIME)
    {
        result = -RT_ETIMEOUT;
        goto __exit;
    }
    inet_aton(ip_addr, &(ping_resp->ip_addr));
    ping_resp->data_len = data_len;
    /* reply time, in units of ms */
    ping_resp->ticks = time ;
    ping_resp->ttl = ttl;

 __exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

const struct netdev_ops l610_netdev_ops =
{
    l610_netdev_set_up,
    l610_netdev_set_down,

    RT_NULL, /* not support set ip, netmask, gatway address */
    RT_NULL,
    RT_NULL, /* not support set DHCP status */

#ifdef NETDEV_USING_PING
    l610_netdev_ping,
#endif
    RT_NULL,
};

static struct netdev *l610_netdev_add(const char *netdev_name)
{
#define L610_NETDEV_MTU     1500
#define HWADDR_LEN          8
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
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = L610_NETDEV_MTU;
    netdev->ops = &l610_netdev_ops;
        netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  sim76xx device operations ============================= */

#define AT_SEND_CMD(client, resp, resp_line, timeout, cmd)                                         \
    do {                                                                                           \
        (resp) = at_resp_set_info((resp), 128, (resp_line), rt_tick_from_millisecond(timeout));    \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                                          \
        {                                                                                          \
            result = -RT_ERROR;                                                                    \
            goto __exit;                                                                           \
        }                                                                                          \
    } while(0)                                                                                     \

/* init for l610 */




static void l610_init_thread_entry(void *parameter)
{
#define INIT_RETRY                      5
#define CPIN_RETRY                      10
#define CSQ_RETRY                       10
#define CREG_RETRY                      10
#define CGREG_RETRY                     20

    int i, qimux, retry_num = INIT_RETRY;
    char parsed_data[32] = {0};
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

    while (retry_num--)
    {
        rt_memset(parsed_data, 0, sizeof(parsed_data));
        rt_thread_mdelay(500);
        l610_power_on(device);

        rt_thread_mdelay(1000);

        /* wait l610 startup finish */
        if (at_client_obj_wait_connect(client, L610_WAIT_CONNECT_TIME))
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
            LOG_E("%s device SIM card detection failed.", device->name);
            result = -RT_ERROR;
            goto __exit;
        }
        /* waiting for dirty data to be digested */
        rt_thread_mdelay(10);

        /* check the GSM network is registered */
        for (i = 0; i < CREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CREG?");

            at_resp_parse_line_args_by_kw(resp, "+CREG:", "+CREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
            {

                LOG_D("%s device GSM is registered(%s),", device->name, parsed_data);
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
            AT_SEND_CMD(client, resp, 0, 300, "AT+CGREG?");

            at_resp_parse_line_args_by_kw(resp, "+CGREG:", "+CGREG: %s", &parsed_data);
            if (!strncmp(parsed_data, "0,1", sizeof(parsed_data)) ||
                !strncmp(parsed_data, "0,5", sizeof(parsed_data)))
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

        /* check signal strength */
        for (i = 0; i < CSQ_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+CSQ?");
            at_resp_parse_line_args_by_kw(resp, "+CSQ:", "+CSQ: %s", &parsed_data);
            if (strncmp(parsed_data, "99,99", sizeof(parsed_data)))
            {
                LOG_D("%s device signal strength: %s", device->name, parsed_data);
                break;
            }
            rt_thread_mdelay(1000);
        }
        if (i == CSQ_RETRY)
        {
            LOG_E("%s device signal strength check failed (%s)", device->name, parsed_data);
            result = -RT_ERROR;
            goto __exit;
        }

// 0: Received data with "+MIPRTCP:" or "+MIPRUDP:" and the data is encoded.
// 1: Received data only and the data are without encoded. In received character string, Module doesn't accede to any <CR><LF> symbol.
// 2: Received data with "+MIPRTCP:" or "+MIPRUDP:" and the data is without encoded. In received character string, Module will accede to <CR><LF> before "+MIPRTCP:" or "+MIPRUDP:".
// 5: Data read mode.
        /* */
        AT_SEND_CMD(client, resp, 2, 5 * 1000, "AT+GTSET=\"IPRFMT\",5");
        rt_thread_mdelay(100);

        /* Set to multiple connections */
        AT_SEND_CMD(client, resp, 0, 300, "AT+MIPCALL?");


        at_resp_parse_line_args_by_kw(resp, "+MIPCALL:", "+MIPCALL: %d", &qimux);

        if (qimux == 0)
        {
        AT_SEND_CMD(client, resp, 0, 300, "AT+COPS?");
        at_resp_parse_line_args_by_kw(resp, "+COPS:", "+COPS: %*[^\"]\"%[^\"]", &parsed_data);

        if (rt_strcmp(parsed_data, "CHINA MOBILE") == 0)
        {
            /* "CMCC" */
            LOG_I("%s device network operator: %s", device->name, parsed_data);
            AT_SEND_CMD(client, resp, 0, 300, CSTT_CHINA_MOBILE);

        }
        else if (rt_strcmp(parsed_data, "CHN-UNICOM") == 0)
        {
            /* "UNICOM" */
            LOG_I("%s device network operator: %s", device->name, parsed_data);
            AT_SEND_CMD(client, resp, 0, 300, CSTT_CHINA_UNICOM);
        }
        else if (rt_strcmp(parsed_data, "CHN-CT") == 0)
        {
            AT_SEND_CMD(client, resp, 0, 300, CSTT_CHINA_TELECOM);
            /* "CT" */
            LOG_I("%s device network operator: %s", device->name, parsed_data);
        }

            }
  /* check the GPRS network is registered */
        for (i = 0; i < CGREG_RETRY; i++)
        {
            AT_SEND_CMD(client, resp, 0, 300, "AT+MIPCALL?");

            at_resp_parse_line_args_by_kw(resp, "+MIPCALL: ", "+MIPCALL: %s", &parsed_data);

            if(parsed_data!=NULL)
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

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            /* power off the L610 device */
            l610_power_off(device);
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
        l610_netdev_set_info(device->netdev);
        /* check and create link staus sync thread  */
        if (rt_thread_find(device->netdev->name) == RT_NULL)
        {
            l610_netdev_check_link_status(device->netdev);
        }

        LOG_I("%s device network initialize success!", device->name);

    }
    else
    {
        LOG_E("%s device network initialize failed(%d)!", device->name, result);
    }
}

static int l610_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_L610_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("l610_net", l610_init_thread_entry, (void *)device,
                L610_THREAD_STACK_SIZE, L610_THREAD_PRIORITY, 20);
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
    L610_init_thread_entry(device);
#endif /* AT_DEVICE_L610_INIT_ASYN */

    return RT_EOK;
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    RT_ASSERT(data);

    LOG_I("URC data : %.*s", size, data);
}

/* l610 device URC table for the device control */
static const struct at_urc urc_table[] =
{
        {"RDY",         "\r\n",             urc_func},
};

static int l610_init(struct at_device *device)
{
    struct at_device_l610 *l610 = (struct at_device_l610 *) device->user_data;

    /* initialize AT client */
    at_client_init(l610->client_name, l610->recv_line_num);

    device->client = at_client_get(l610->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", l610->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#ifdef AT_USING_SOCKET

    l610_socket_init(device);
#endif

    /* add l610 device to the netdev list */
    device->netdev = l610_netdev_add(l610->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("get netdev(%s) failed.", l610->device_name);
        return -RT_ERROR;
    }

    /* initialize l610 pin configuration */
    if (l610->power_pin != -1 && l610->power_status_pin != -1)
    {
        rt_pin_mode(l610->power_pin, PIN_MODE_OUTPUT);
        rt_pin_mode(l610->power_status_pin, PIN_MODE_INPUT);
    }

    /* initialize l610 device network */
    return l610_netdev_set_up(device->netdev);
}

static int l610_deinit(struct at_device *device)
{
    return l610_netdev_set_down(device->netdev);
}

static int l610_control(struct at_device *device, int cmd, void *arg)
{
    int result = -RT_ERROR;

    RT_ASSERT(device);

    switch (cmd)
    {
    case AT_DEVICE_CTRL_SLEEP:
        result = l610_sleep(device);
        break;
    case AT_DEVICE_CTRL_WAKEUP:
        result = l610_wakeup(device);
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

const struct at_device_ops l610_device_ops =
{
    l610_init,
    l610_deinit,
    l610_control,
};

static int l610_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill l610 device class object */
#ifdef AT_USING_SOCKET
    l610_socket_class_register(class);
#endif
    class->device_ops = &l610_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_L610);
}
INIT_DEVICE_EXPORT(l610_device_class_register);

#endif /* AT_DEVICE_USING_L610 */
