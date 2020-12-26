/*
 * File      : at_device_w60x.c
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
 * 2018-06-20     chenyong     first version
 * 2019-05-09     chenyong     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_w60x.h>

#define LOG_TAG                        "at.dev.w60x"

#include <at_log.h>

#ifdef AT_DEVICE_USING_W60X

#define W60X_WAIT_CONNECT_TIME      5000
#define W60X_THREAD_STACK_SIZE      2048
#define W60X_THREAD_PRIORITY        (RT_THREAD_PRIORITY_MAX / 2)

/* =============================  w60x network interface operations ============================= */

static int w60x_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server);

static rt_bool_t w60x_is_join_start = RT_FALSE;

static void w60x_get_netdev_info(struct rt_work *work, void *work_data)
{
#define AT_ADDR_LEN          32
#define AT_ERR_DNS_SERVER    "0.0.0.0"
#define AT_DEF_DNS_SERVER    "114.114.114.114"

    at_response_t resp = RT_NULL;
    char ip[AT_ADDR_LEN] = {0}, mac[AT_ADDR_LEN] = {0};
    char gateway[AT_ADDR_LEN] = {0}, netmask[AT_ADDR_LEN] = {0};
    char dns_server1[AT_ADDR_LEN] = {0}, dns_server2[AT_ADDR_LEN] = {0};
    const char *resp_expr1 = "+OK=%s";
    const char *resp_expr2 = "+OK=%d,\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"";
    const char *resp_expr3 = "+OK=%d,%[^,],%[^,],%[^,],%[^,]";
    rt_int32_t link_status = 0;
    ip_addr_t ip_addr;
    rt_uint32_t mac_addr[6] = {0};
    rt_uint32_t num = 0;
    rt_int32_t dhcp_stat = 0;
    struct rt_delayed_work *delay_work = (struct rt_delayed_work *)work;
    struct at_device *device = (struct at_device *)work_data;
    struct netdev *netdev = device->netdev;
    struct at_client *client = device->client;
    char *pos;

    if (delay_work)
    {
        rt_free(delay_work);
    }

    resp = at_create_resp(512, 1, rt_tick_from_millisecond(3000));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    at_obj_set_end_sign(device->client, '\r');

    /* send mac addr query commond "AT+QMAC" and wait response */
    if (at_obj_exec_cmd(client, resp, "AT+QMAC") < 0)
    {
        goto __exit;
    }

    pos = rt_strstr(resp->buf, "+OK=");
    if (!pos)
    {
        LOG_E("%s device parse \"AT+QMAC\" cmd error.", device->name);
        goto __exit;
    }

    sscanf(pos, resp_expr1, mac);

    /* send addr info query commond "AT+CIPSTA?" and wait response */
    if (at_obj_exec_cmd(client, resp, "AT+LKSTT") < 0)
    {
        LOG_E("%s device send \"AT+LKSTT\" cmd error.", device->name);
        goto __exit;
    }

    pos = rt_strstr(resp->buf, "+OK=");
    if (!pos)
    {
        LOG_E("%s device prase \"AT+LKSTT\" cmd error.", device->name);
        goto __exit;
    }

    sscanf(pos, resp_expr2, &link_status, ip, netmask, gateway, dns_server1, dns_server2);

    /* set netdev info */
    inet_aton(gateway, &ip_addr);
    netdev_low_level_set_gw(netdev, &ip_addr);
    inet_aton(netmask, &ip_addr);
    netdev_low_level_set_netmask(netdev, &ip_addr);
    inet_aton(ip, &ip_addr);
    netdev_low_level_set_ipaddr(netdev, &ip_addr);
    sscanf(mac, "%02x%02x%02x%02x%02x%02x",
            &mac_addr[0], &mac_addr[1], &mac_addr[2], &mac_addr[3], &mac_addr[4], &mac_addr[5]);
    for (num = 0; num < netdev->hwaddr_len; num++)
    {
        netdev->hwaddr[num] = mac_addr[num];
    }

    /* set primary DNS server address */
    if (rt_strlen(dns_server1) > 0 &&
            rt_strncmp(dns_server1, AT_ERR_DNS_SERVER, rt_strlen(AT_ERR_DNS_SERVER)) != 0)
    {
        inet_aton(dns_server1, &ip_addr);
        netdev_low_level_set_dns_server(netdev, 0, &ip_addr);
    }
    else
    {
        inet_aton(AT_DEF_DNS_SERVER, &ip_addr);
        w60x_netdev_set_dns_server(netdev, 0, &ip_addr);
    }

    /* set secondary DNS server address */
    if (rt_strlen(dns_server2) > 0 )
    {
        inet_aton(dns_server2, &ip_addr);
        netdev_low_level_set_dns_server(netdev, 1, &ip_addr);
    }

    /* send DHCP query commond " AT+NIP" and wait response */
    if (at_obj_exec_cmd(client, resp, "AT+NIP") < 0)
    {
        goto __exit;
    }

    pos = rt_strstr(resp->buf, "+OK=");
 
    /* parse response data, get the DHCP status */
    if (!pos)
    {
        LOG_E("%s device prase DHCP status error.", device->name);
        goto __exit;
    }

    sscanf(pos, resp_expr3, &dhcp_stat, ip, netmask, gateway, dns_server1);

    /* 0 - DHCP, 1 - static ip */
    netdev_low_level_set_dhcp_status(netdev, dhcp_stat ? RT_FALSE : RT_TRUE);

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
}

static int w60x_net_init(struct at_device *device);

static int w60x_netdev_set_up(struct netdev *netdev)
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
        w60x_net_init(device);
        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status", netdev->name);
    }

    return RT_EOK;
}

static int w60x_netdev_set_down(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    if (device->is_init == RT_TRUE)
    {
        device->is_init = RT_FALSE;
        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status", netdev->name);
    }

    return RT_EOK;
}

static int w60x_netdev_set_addr_info(struct netdev *netdev, ip_addr_t *ip_addr, ip_addr_t *netmask, ip_addr_t *gw)
{
#define IPADDR_SIZE            16

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_str[IPADDR_SIZE] = {0};
    char gw_str[IPADDR_SIZE] = {0};
    char netmask_str[IPADDR_SIZE] = {0};
    char dns_str[IPADDR_SIZE] = {0};
    rt_uint8_t dhcp_stat = 0;
    const char *resp_expr = "+OK=%d,%s,%s,%s,%s";

    RT_ASSERT(netdev);
    RT_ASSERT(ip_addr || netmask || gw);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 1, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* send DHCP query commond " AT+NIP" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP") < 0)
    {
        goto __exit;
    }

    if (at_resp_parse_line_args(resp, 1, resp_expr, dhcp_stat, ip_str, netmask_str, gw_str, dns_str) <= 0)
    {
        LOG_E("%s device prase DHCP status error.", device->name);
        goto __exit;
    }

    /* Convert numeric IP address into decimal dotted ASCII representation. */
    if (ip_addr)
        rt_memcpy(ip_str, inet_ntoa(*ip_addr), IPADDR_SIZE);
    else
        rt_memcpy(ip_str, inet_ntoa(netdev->ip_addr), IPADDR_SIZE);

    if (gw)
        rt_memcpy(gw_str, inet_ntoa(*gw), IPADDR_SIZE);
    else
        rt_memcpy(gw_str, inet_ntoa(netdev->gw), IPADDR_SIZE);

    if (netmask)
        rt_memcpy(netmask_str, inet_ntoa(*netmask), IPADDR_SIZE);
    else
        rt_memcpy(netmask_str, inet_ntoa(netdev->netmask), IPADDR_SIZE);

    /* send addr info set commond "AT+NIP=dhcptype,ip,gateway,netmask,dnsserver" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP=%hhu,%s,%s,%s,%s",
                        dhcp_stat, ip_str, gw_str, netmask_str, dns_str) < 0)
    {
        LOG_E("%s device set address failed.", device->name);
        result = -RT_ERROR;
    }
    else
    {
        /* Update netdev information */
        if (ip_addr)
            netdev_low_level_set_ipaddr(netdev, ip_addr);

        if (gw)
            netdev_low_level_set_gw(netdev, gw);

        if (netmask)
            netdev_low_level_set_netmask(netdev, netmask);

        LOG_D("%s device set address success.", device->name);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int w60x_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_str[IPADDR_SIZE] = {0};
    char gw_str[IPADDR_SIZE] = {0};
    char netmask_str[IPADDR_SIZE] = {0};
    char dns_str[IPADDR_SIZE] = {0};
    rt_uint8_t dhcp_stat = 0;
    const char *resp_expr = "+OK=%d,%s,%s,%s,%s";

    RT_ASSERT(netdev);
    RT_ASSERT(dns_server);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 1, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* send DHCP query commond " AT+NIP" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP") < 0)
    {
        goto __exit;
    }

    if (at_resp_parse_line_args(resp, 1, resp_expr, dhcp_stat, ip_str, netmask_str, gw_str, dns_str) <= 0)
    {
        LOG_E("%s device prase DHCP status error.", device->name);
        goto __exit;
    }

    if (dns_server)
        rt_memcpy(dns_str, inet_ntoa(*dns_server), IPADDR_SIZE);
    else
        rt_memcpy(dns_str, inet_ntoa(netdev->dns_servers), IPADDR_SIZE);

    /* send addr info set commond "AT+NIP=dhcptype,ip,gateway,netmask,dnsserver" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP=%hhu,%s,%s,%s,%s",
                        dhcp_stat, ip_str, gw_str, netmask_str, dns_str) < 0)
    {
        LOG_E("%s device set DNS failed.", device->name);
        result = -RT_ERROR;
    }
    else
    {
        netdev_low_level_set_dns_server(netdev, dns_num, dns_server);
        LOG_D("%s device set DNS(%s) success.", device->name, inet_ntoa(*dns_server));
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int w60x_netdev_set_dhcp(struct netdev *netdev, rt_bool_t is_enabled)
{
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_str[IPADDR_SIZE] = {0};
    char gw_str[IPADDR_SIZE] = {0};
    char netmask_str[IPADDR_SIZE] = {0};
    char dns_str[IPADDR_SIZE] = {0};
    rt_uint8_t dhcp_stat = 0;
    const char *resp_expr = "+OK=%d,%s,%s,%s,%s";

    RT_ASSERT(netdev);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 1, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp struct.");
        return -RT_ENOMEM;
    }

    /* send DHCP query commond " AT+NIP" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP") < 0)
    {
        goto __exit;
    }

    if (at_resp_parse_line_args(resp, 1, resp_expr, dhcp_stat, ip_str, netmask_str, gw_str, dns_str) <= 0)
    {
        LOG_E("%s device prase DHCP status error.", device->name);
        goto __exit;
    }

    /* send addr info set commond "AT+NIP=dhcptype,ip,gateway,netmask,dnsserver" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+NIP=%hhu,%s,%s,%s,%s",
                        is_enabled ? 0 : 1, ip_str, gw_str, netmask_str, dns_str) < 0)
    {
        LOG_E("%s device set DHCP status(%d) failed.", device->name, is_enabled);
        result = -RT_ERROR;
        goto __exit;
    }
    else
    {
        netdev_low_level_set_dhcp_status(netdev, is_enabled);
        LOG_D("%s device set DHCP status(%d) ok.", device->name, is_enabled);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

#ifdef NETDEV_USING_PING
static int w60x_netdev_ping(struct netdev *netdev, const char *host,
                size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define W60X_IP_SIZE         16

    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_addr[W60X_IP_SIZE] = {0};
    char *pos;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return -RT_ERROR;
    }

    resp = at_create_resp(256, 1, timeout);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    at_obj_set_end_sign(device->client, '\r');

    /* send domain commond "AT+SKGHBN=<domain name>" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+SKGHBN=%s", host) < 0)
    {
        LOG_E("AT+SKGHBN failed.");
        result = -RT_ERROR;
        goto __exit;
    }

    /* parse the third line of response data, get the IP address */
    pos = rt_strstr(resp->buf, "+OK=");
    if (!pos)
    {
        LOG_E("AT+SKGHBN resp error.");
        result = -RT_ERROR;
        goto __exit;
    }

    sscanf(pos, "+OK=\"%[^\"]\"", ip_addr);

    inet_aton(ip_addr, &(ping_resp->ip_addr));
    ping_resp->data_len = data_len;
    ping_resp->ttl = 0;
    ping_resp->ticks = 0;

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

#ifdef NETDEV_USING_NETSTAT
void w60x_netdev_netstat(struct netdev *netdev)
{
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
 
    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    resp = at_create_resp(128, 1, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.", device->name);
        goto __exit;
    }

    /* send network connection information commond "AT+LKSTT" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+LKSTT") < 0)
    {
        goto __exit;
    }

    LOG_RAW("%s\n", at_resp_get_line(resp, 1));

    /* maybe query "AT+SKSTT"... */

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
}
#endif /* NETDEV_USING_NETSTAT */

static const struct netdev_ops w60x_netdev_ops =
{
    w60x_netdev_set_up,
    w60x_netdev_set_down,

    w60x_netdev_set_addr_info,
    w60x_netdev_set_dns_server,
    w60x_netdev_set_dhcp,

#ifdef NETDEV_USING_PING
    w60x_netdev_ping,
#endif
#ifdef NETDEV_USING_NETSTAT
    w60x_netdev_netstat,
#endif
};

static struct netdev *w60x_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          6
    struct netdev *netdev = RT_NULL;

    RT_ASSERT(netdev_name);

    netdev = (struct netdev *) rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = ETHERNET_MTU;
    netdev->ops = &w60x_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  w60x device operations ============================= */

#define AT_SEND_CMD(client, resp, cmd)                                     \
    do {                                                                   \
        (resp) = at_resp_set_info((resp), 256, 1, 3 * RT_TICK_PER_SECOND); \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                  \
        {                                                                  \
            result = -RT_ERROR;                                            \
            goto __exit;                                                   \
        }                                                                  \
    } while(0)                                                             \

static void w60x_netdev_start_delay_work(struct at_device *device)
{
    struct rt_delayed_work *net_work = RT_NULL;
    net_work = (struct rt_delayed_work *)rt_calloc(1, sizeof(struct rt_delayed_work));
    if (net_work == RT_NULL)
    {
        return;
    }

    rt_delayed_work_init(net_work, w60x_get_netdev_info, (void *)device);
    rt_work_submit(&(net_work->work), RT_TICK_PER_SECOND);
}

static void w60x_init_thread_entry(void *parameter)
{
#define INIT_RETRY    5

    struct at_device *device = (struct at_device *) parameter;
    struct at_device_w60x *w60x = (struct at_device_w60x *) device->user_data;
    struct at_client *client = device->client;
    at_response_t resp = RT_NULL;
    rt_err_t result = RT_EOK;
    rt_size_t i = 0, retry_num = INIT_RETRY;
    rt_bool_t wifi_is_conn = RT_FALSE;
    rt_uint8_t atcmd_str[128];

    LOG_D("%s device initialize start.", device->name);

    /* wait w60x device startup finish */
    rt_thread_mdelay(1000);

    resp = at_create_resp(256, 1, 3 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    while (retry_num--)
    {
        /* reset module */
        AT_SEND_CMD(client, resp, "AT+Z");
        /* reset waiting delay */
        rt_thread_mdelay(1000);
        /* get module version */
        AT_SEND_CMD(client, resp, "AT+QVER");
        /* show module version */
        for (i = 0; i < resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }

        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            rt_thread_mdelay(1000);
            LOG_I("%s device initialize retry...", device->name);
        }
    }

    /* set current mode to Wi-Fi station */
    rt_sprintf(atcmd_str, "AT+WPRT=%d", 0);
    AT_SEND_CMD(client, resp, atcmd_str);
    rt_sprintf(atcmd_str, "AT+SSID=\"%s\"", w60x->wifi_ssid);
    AT_SEND_CMD(client, resp, atcmd_str);
    rt_sprintf(atcmd_str, "AT+KEY=1,0,\"%s\"", w60x->wifi_password);
    AT_SEND_CMD(client, resp, atcmd_str);
    AT_SEND_CMD(client, resp, "AT+PMTF");

    /* connect to WiFi AP */
    w60x_is_join_start = RT_TRUE;
    if (at_obj_exec_cmd(client, at_resp_set_info(resp, 128, 1, 20 * RT_TICK_PER_SECOND),
                        "AT+WJOIN") != RT_EOK)
    {
        LOG_W("%s device wifi connect failed, check ssid(%s) and password(%s).",
              device->name, w60x->wifi_ssid, w60x->wifi_password);
        w60x_is_join_start = RT_FALSE;
    }
    else
    {
        wifi_is_conn = RT_TRUE;
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    if (result != RT_EOK)
    {
        netdev_low_level_set_status(device->netdev, RT_FALSE);
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
    else
    {
        device->is_init = RT_TRUE;
        netdev_low_level_set_status(device->netdev, RT_TRUE);
        if (wifi_is_conn)
        {
            netdev_low_level_set_link_status(device->netdev, RT_TRUE);
        }
        w60x_netdev_start_delay_work(device);
        LOG_I("%s device network initialize successfully.", device->name);
    }
}

static int w60x_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_W60X_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("w60x_net", w60x_init_thread_entry, (void *) device,
            W60X_THREAD_STACK_SIZE, W60X_THREAD_PRIORITY, 20);
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
    w60x_init_thread_entry(device);
#endif /* AT_DEVICE_W60X_INIT_ASYN */

    return RT_EOK;
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    struct at_device *device = RT_NULL;
    char *client_name = client->device->parent.name;

    RT_ASSERT(client && data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    if (w60x_is_join_start && !rt_strncmp(data, "+OK=", rt_strlen("+OK=")))
    {
        w60x_is_join_start = RT_FALSE;
    
        LOG_I("%s device wifi is connected.", device->name);

        if (device->is_init)
        {
            netdev_low_level_set_link_status(device->netdev, RT_TRUE);

            w60x_netdev_start_delay_work(device);
        }
    }
    else if (w60x_is_join_start && !rt_strncmp(data, "+ERR=", rt_strlen("+ERR=")))
    {
        w60x_is_join_start = RT_FALSE;

        LOG_I("%s device wifi is disconnect.", device->name);

        if (device->is_init)
        {
            netdev_low_level_set_link_status(device->netdev, RT_FALSE);
        }
    }
}

static const struct at_urc urc_table[] =
{
    {"+OK=",  "\r\n", urc_func},
    {"+ERR=", "\r\n", urc_func},
};

static int w60x_init(struct at_device *device)
{
    struct at_device_w60x *w60x = (struct at_device_w60x *) device->user_data;

    /* initialize AT client */
    at_client_init(w60x->client_name, w60x->recv_line_num);

    device->client = at_client_get(w60x->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", w60x->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#ifdef AT_USING_SOCKET
    w60x_socket_init(device);
#endif

    /* add w60x device to the netdev list */
    device->netdev = w60x_netdev_add(w60x->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", w60x->device_name);
        return -RT_ERROR;
    }

    /* initialize w60x device network */
    return w60x_netdev_set_up(device->netdev);
}

static int w60x_deinit(struct at_device *device)
{
    return w60x_netdev_set_down(device->netdev);
}

/* reset w60x device and initialize device network again */
static int w60x_reset(struct at_device *device)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    /* send "AT+Z" commonds to w60x device */
    result = at_obj_exec_cmd(client, RT_NULL, "AT+Z");
    rt_thread_mdelay(1000);

    /* waiting 10 seconds for w60x device reset */
    device->is_init = RT_FALSE;
    rt_thread_mdelay(1000);

    /* initialize w60x device network */
    w60x_net_init(device);

    device->is_init = RT_TRUE;

    return result;
}

/* change w60x wifi ssid and password information */
static int w60x_wifi_info_set(struct at_device *device, struct at_device_ssid_pwd *info)
{
    int result = RT_EOK;
    struct at_response *resp = RT_NULL;
    rt_uint8_t atcmd_str[128];

    if (info->ssid == RT_NULL || info->password == RT_NULL)
    {
        LOG_E("input wifi ssid(%s) and password(%s) error.", info->ssid, info->password);
        return -RT_ERROR;
    }

    resp = at_create_resp(128, 1, 20 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* set current mode to Wi-Fi station */
    rt_sprintf(atcmd_str, "AT+WPRT=%d", 0);
    AT_SEND_CMD(device->client, resp, atcmd_str);
    rt_sprintf(atcmd_str, "AT+SSID=\"%s\"", info->ssid);
    AT_SEND_CMD(device->client, resp, atcmd_str);
    rt_sprintf(atcmd_str, "AT+KEY=1,0,\"%s\"", info->password);
    AT_SEND_CMD(device->client, resp, atcmd_str);
    AT_SEND_CMD(device->client, resp, "AT+PMTF");

    /* connect to input wifi ap */
    w60x_is_join_start = RT_TRUE;
    if (at_obj_exec_cmd(device->client, resp, "AT+WJOIN") != RT_EOK)
    {
        LOG_E("%s device wifi connect failed, check ssid(%s) and password(%s).",
              device->name, info->ssid, info->password);
        result = -RT_ERROR;
        w60x_is_join_start = RT_FALSE;
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int w60x_control(struct at_device *device, int cmd, void *arg)
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
    case AT_DEVICE_CTRL_GET_SIGNAL:
    case AT_DEVICE_CTRL_GET_GPS:
    case AT_DEVICE_CTRL_GET_VER:
        LOG_W("not support the control cmd(%d).", cmd);
        break;
    case AT_DEVICE_CTRL_RESET:
        result = w60x_reset(device);
        break;
    case AT_DEVICE_CTRL_SET_WIFI_INFO:
        result = w60x_wifi_info_set(device, (struct at_device_ssid_pwd *) arg);
        break;
    default:
        LOG_E("input error control cmd(%d).", cmd);
        break;
    }

    return result;
}

static const struct at_device_ops w60x_device_ops =
{
    w60x_init,
    w60x_deinit,
    w60x_control,
};

static int w60x_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for class create.");
        return -RT_ENOMEM;
    }

    /* fill W60X device class object */
#ifdef AT_USING_SOCKET
    w60x_socket_class_register(class);
#endif
    class->device_ops = &w60x_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_W60X);
}
INIT_DEVICE_EXPORT(w60x_device_class_register);

#endif /* AT_DEVICE_USING_W60X */
