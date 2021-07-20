/*
 * File      : at_socket_rw007.c
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
 * 2019-05-12     chenyong     multi AT socket client support
 */

#include <stdio.h>
#include <string.h>

#include <at_device_rw007.h>

#define LOG_TAG                        "at.dev.rw007"

#include <at_log.h>

#ifdef AT_DEVICE_USING_RW007

#define RW007_WAIT_CONNECT_TIME        5000
#define RW007_THREAD_STACK_SIZE        2048
#define RW007_THREAD_PRIORITY          (RT_THREAD_PRIORITY_MAX / 2)

/* =============================  rw007 network interface operations ============================= */

static struct netdev *rw007_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          6
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
        return RT_NULL;
    }

    /* TODO: improve netdev adaptation */
    netdev->mtu = ETHERNET_MTU;
    netdev->hwaddr_len = HWADDR_LEN;
    netdev->ops = RT_NULL;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    /*TODO: improve netdev adaptation */
    netdev_low_level_set_status(netdev, RT_TRUE);
    netdev_low_level_set_link_status(netdev, RT_TRUE);
    netdev_low_level_set_dhcp_status(netdev, RT_TRUE);
    netdev->flags |= NETDEV_FLAG_INTERNET_UP;

    return netdev;
}

/* =============================  rw007 device operations ============================= */

#define AT_SEND_CMD(client, resp, cmd)                                     \
    do {                                                                   \
        (resp) = at_resp_set_info((resp), 256, 0, 5 * RT_TICK_PER_SECOND); \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                  \
        {                                                                  \
            result = -RT_ERROR;                                            \
            goto __exit;                                                   \
        }                                                                  \
    } while(0)                                                             \

static void rw007_init_thread_entry(void *parameter)
{
#define INIT_RETRY    5

    struct at_device *device = (struct at_device *) parameter;
    struct at_device_rw007 *rw007 = (struct at_device_rw007 *) device->user_data;
    struct at_client *client = device->client;
    at_response_t resp = RT_NULL;
    rt_err_t result = RT_EOK;
    rt_size_t i = 0, retry_num = INIT_RETRY;

    /* wait rw007 device startup finish */
    if (at_client_obj_wait_connect(client, RW007_WAIT_CONNECT_TIME))
    {
        return;
    }

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    while (retry_num--)
    {
        /* reset module */
        AT_SEND_CMD(client, resp, "AT+RST");
        /* reset waiting delay */
        rt_thread_mdelay(1000);
        /* disable echo */
        AT_SEND_CMD(client, resp, "ATE0");
        /* set current mode to Wi-Fi station */
        AT_SEND_CMD(client, resp, "AT+CWMODE=1");
        /* get module version */
        AT_SEND_CMD(client, resp, "AT+GMR");
        /* show module version */
        for (i = 0; i < resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }
        /* connect to WiFi AP */
        if (at_obj_exec_cmd(client, at_resp_set_info(resp, 128, 0, 20 * RT_TICK_PER_SECOND),
                    "AT+CWJAP=\"%s\",\"%s\"", rw007->wifi_ssid, rw007->wifi_password) != RT_EOK)
        {
            LOG_E("%s device wifi connect failed, check ssid(%s) and password(%s).",
                    device->name, rw007->wifi_ssid, rw007->wifi_password);
            result = -RT_ERROR;
            goto __exit;
        }

        AT_SEND_CMD(client, resp, "AT+CIPMUX=1");

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
        netdev_low_level_set_status(device->netdev, RT_TRUE);
        LOG_I("%s device network initialize success.", device->name);
    }
}

int rw007_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_RW007_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("rw007_net", rw007_init_thread_entry,
                           (void *)device, RW007_THREAD_STACK_SIZE, RW007_THREAD_PRIORITY, 20);
    if (tid)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create %s device initialization thread failed.", device->name);
        return -RT_ERROR;
    }
#else
    rw007_init_thread_entry(device);
#endif /* AT_DEVICE_RW007_INIT_ASYN */

    return RT_EOK;
}

static void urc_busy_p_func(struct at_client *client, const char *data, rt_size_t size)
{
    LOG_D("system is processing a commands...");
}

static void urc_busy_s_func(struct at_client *client, const char *data, rt_size_t size)
{
    LOG_D("system is sending data...");
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

    if (rt_strstr(data, "WIFI CONNECTED"))
    {
        LOG_I("%s device wifi is connected.", device->name);
    }
    else if (rt_strstr(data, "WIFI DISCONNECT"))
    {
        LOG_I("%s device wifi is disconnect.", device->name);
    }
}

static struct at_urc urc_table[] = {
        {"busy p",           "\r\n",           urc_busy_p_func},
        {"busy s",           "\r\n",           urc_busy_s_func},
        {"WIFI CONNECTED",   "\r\n",           urc_func},
        {"WIFI DISCONNECT",  "\r\n",           urc_func},
};

static int rw007_init(struct at_device *device)
{
    struct at_device_rw007 *rw007 = (struct at_device_rw007 *) device->user_data;

    /* initialize AT client */
    at_client_init(rw007->client_name, rw007->recv_line_num);

    device->client = at_client_get(rw007->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", rw007->client_name);
        return -RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#ifdef AT_USING_SOCKET
    rw007_socket_init(device);
#endif

    /* add rw007 device to the netdev list */
    device->netdev = rw007_netdev_add(rw007->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("get netdev(%s) failed.", rw007->device_name);
        return -RT_ERROR;
    }

    /* initialize rw007 device network */
    return rw007_net_init(device);
}

static int rw007_deinit(struct at_device *device)
{
    // TODO add netdev operation
    device->is_init = RT_FALSE;
    return RT_EOK;
}

/* reset eap8266 device and initialize device network again */
static int rw007_reset(struct at_device *device)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    /* send "AT+RST" commonds to rw007 device */
    result = at_obj_exec_cmd(client, RT_NULL, "AT+RST");
    rt_thread_delay(1000);

    /* waiting 10 seconds for rw007 device reset */
    device->is_init = RT_FALSE;
    if (at_client_obj_wait_connect(client, RW007_WAIT_CONNECT_TIME))
    {
        return -RT_ETIMEOUT;
    }

    /* initialize rw007 device network */
    rw007_net_init(device);

    device->is_init = RT_TRUE;

    return result;
}

/* change eap8266 wifi ssid and password information */
static int rw007_wifi_info_set(struct at_device *device, struct at_device_ssid_pwd *info)
{
    int result = RT_EOK;
    struct at_response *resp = RT_NULL;

    if (info->ssid == RT_NULL || info->password == RT_NULL)
    {
        LOG_E("input wifi ssid(%s) and password(%s) error.", info->ssid, info->password);
         return -RT_ERROR;
    }

    resp = at_create_resp(128, 0, 20 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return -RT_ENOMEM;
    }

    /* connect to input wifi ap */
    if (at_obj_exec_cmd(device->client, resp, "AT+CWJAP=\"%s\",\"%s\"", info->ssid, info->password) != RT_EOK)
    {
        LOG_E("%s device wifi connect failed, check ssid(%s) and password(%s).",
                device->name, info->ssid, info->password);
        result = -RT_ERROR;
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
static int rw007_control(struct at_device *device, int cmd, void *arg)
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
        LOG_W("not support the control command(%d).", cmd);
        break;
    case AT_DEVICE_CTRL_RESET:
        result = rw007_reset(device);
        break;
    case AT_DEVICE_CTRL_SET_WIFI_INFO:
        result = rw007_wifi_info_set(device, (struct at_device_ssid_pwd *) arg);
        break;
    default:
        LOG_E("input error control command(%d).", cmd);
        break;
    }

    return result;
}

const struct at_device_ops rw007_device_ops =
{
    rw007_init,
    rw007_deinit,
    rw007_control,
};

static int rw007_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for device class create.");
        return -RT_ENOMEM;
    }

    /* fill rw007 device class object */
#ifdef AT_USING_SOCKET
    rw007_socket_class_register(class);
#endif
    class->device_ops = &rw007_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_RW007);
}
INIT_DEVICE_EXPORT(rw007_device_class_register);

#endif /* AT_DEVICE_USING_RW007 */
