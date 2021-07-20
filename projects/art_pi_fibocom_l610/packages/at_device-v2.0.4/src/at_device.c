/*
 * File      : at_device.c
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
 * 2019-05-08     chenyong     first version
 */

#include <stdlib.h>
#include <string.h>

#include <at_device.h>

#define DBG_TAG              "at.dev"
#define DBG_LVL              DBG_INFO
#include <rtdbg.h>

/* The global list of at device */
static rt_slist_t at_device_list = RT_SLIST_OBJECT_INIT(at_device_list);
/* The global list of at device class */
static rt_slist_t at_device_class_list = RT_SLIST_OBJECT_INIT(at_device_class_list);

/**
 * This function will get the first initialized AT device.
 *
 * @return the AT device structure pointer
 */
struct at_device *at_device_get_first_initialized(void)
{
    rt_base_t level;
    rt_slist_t *node = RT_NULL;
    struct at_device *device = RT_NULL;

    level = rt_hw_interrupt_disable();

    rt_slist_for_each(node, &at_device_list)
    {
        device = rt_slist_entry(node, struct at_device, list);
        if (device && device->is_init == RT_TRUE)
        {
           rt_hw_interrupt_enable(level);
           return device;
        }
    }

    rt_hw_interrupt_enable(level);

    return RT_NULL;
}

/**
 * This function will get AT device by device name.
 *
 * @param type the name type
 * @param name the device name or the client name
 *
 * @return the AT device structure pointer
 */
struct at_device *at_device_get_by_name(int type, const char *name)
{
    rt_base_t level;
    rt_slist_t *node = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(name);

    level = rt_hw_interrupt_disable();

    rt_slist_for_each(node, &at_device_list)
    {
        device = rt_slist_entry(node, struct at_device, list);
        if (device)
        {
            if (((type == AT_DEVICE_NAMETYPE_DEVICE) || (type == AT_DEVICE_NAMETYPE_NETDEV)) &&
                (rt_strncmp(device->name, name, rt_strlen(name)) == 0))
            {
                rt_hw_interrupt_enable(level);
                return device;
            }
            else if ((type == AT_DEVICE_NAMETYPE_CLIENT) &&
                (rt_strncmp(device->client->device->parent.name, name, rt_strlen(name)) == 0))
            {
                rt_hw_interrupt_enable(level);
                return device;
            }
        }
    }

    rt_hw_interrupt_enable(level);

    return RT_NULL;
}

#ifdef AT_USING_SOCKET
/**
 * This function will get AT device by ip address.
 *
 * @param ip_addr input ip address
 * network
 * @return != NULL: network interface device object
 *            NULL: get failed
 */
struct at_device *at_device_get_by_ipaddr(ip_addr_t *ip_addr)
{
    rt_base_t level;
    rt_slist_t *node = RT_NULL;
    struct at_device *device = RT_NULL;

    level = rt_hw_interrupt_disable();

    rt_slist_for_each(node, &at_device_list)
    {
        device = rt_slist_entry(node, struct at_device, list);
        if (device && ip_addr_cmp(ip_addr, &(device->netdev->ip_addr)))
        {
           rt_hw_interrupt_enable(level);
           return device;
        }
    }

    rt_hw_interrupt_enable(level);

    return RT_NULL;

}
#endif /* AT_USING_SOCKET */


/**
 * This function will perform a variety of control functions on AT devices.
 *
 * @param device the pointer of AT device structure
 * @param cmd the command sent to AT device
 * @param arg the argument of command
 *
 * @return = 0: perform successfully
 *         < 0: perform failed
 */
int at_device_control(struct at_device *device, int cmd, void *arg)
{
    if (device->class->device_ops->control)
    {
        return device->class->device_ops->control(device, cmd, arg);
    }
    else
    {
        LOG_W("AT device(%s) not support control operations.", device->name);
        return RT_EOK;
    }
}

/**
 * This function registers an AT device class with specified device class ID.
 *
 * @param class the pointer of AT device class structure
 * @param class_id AT device class ID
 *
 * @return 0: register successfully
 */
int at_device_class_register(struct at_device_class *class, uint16_t class_id)
{
    rt_base_t level;

    RT_ASSERT(class);

    /* Fill AT device class */
    class->class_id = class_id;

    /* Initialize current AT device class single list */
    rt_slist_init(&(class->list));

    level = rt_hw_interrupt_disable();

    /* Add current AT device class to list */
    rt_slist_append(&at_device_class_list, &(class->list));

    rt_hw_interrupt_enable(level);

    return RT_EOK;
}

/* Get AT device class by client ID */
static struct at_device_class *at_device_class_get(uint16_t class_id)
{
    rt_base_t level;
    rt_slist_t *node = RT_NULL;
    struct at_device_class *class = RT_NULL;

    level = rt_hw_interrupt_disable();

    /* Get AT device class by class ID */
    rt_slist_for_each(node, &at_device_class_list)
    {
        class = rt_slist_entry(node, struct at_device_class, list);
        if (class && class->class_id == class_id)
        {
            rt_hw_interrupt_enable(level);
            return class;
        }
    }

    rt_hw_interrupt_enable(level);

    return RT_NULL;
}

/**
 * This function registers an AT device with specified device name and AT client name.
 *
 * @param device the pointer of AT device structure
 * @param device_name AT device name
 * @param at_client_name AT device client name
 * @param class_id AT device class ID
 * @param user_data user-specific data
 *
 * @return = 0: register successfully
 *         < 0: register failed
 */
int at_device_register(struct at_device *device, const char *device_name,
                        const char *at_client_name, uint16_t class_id, void *user_data)
{
    rt_base_t level;
    int result = 0;
    static int device_counts = 0;
    char name[RT_NAME_MAX] = {0};
    struct at_device_class *class = RT_NULL;

    RT_ASSERT(device);
    RT_ASSERT(device_name);
    RT_ASSERT(at_client_name);

    class = at_device_class_get(class_id);
    if (class == RT_NULL)
    {
        LOG_E("get AT device class(%d) failed.", class_id);
        result = -RT_ERROR;
        goto __exit;
    }

    /* Fill AT device object*/
#ifdef AT_USING_SOCKET
    device->sockets = (struct at_socket *) rt_calloc(class->socket_num, sizeof(struct at_socket));
    if (device->sockets == RT_NULL)
    {
        LOG_E("no memory for AT Socket number(%d) create.", class->socket_num);
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* create AT device socket event */
    rt_snprintf(name, RT_NAME_MAX, "at_se%d", device_counts++);
    device->socket_event = rt_event_create(name, RT_IPC_FLAG_FIFO);
    if (device->socket_event == RT_NULL)
    {
        LOG_E("no memory for AT device(%s) socket event create.", device_name);
        result = -RT_ENOMEM;
        goto __exit;
    }
#endif /* AT_USING_SOCKET */

    rt_memcpy(device->name, device_name, rt_strlen(device_name));
    device->class = class;
    device->user_data = user_data;

    /* Initialize current AT device single list */
    rt_slist_init(&(device->list));

    level = rt_hw_interrupt_disable();

    /* Add current AT device to device list */
    rt_slist_append(&at_device_list, &(device->list));

    rt_hw_interrupt_enable(level);

    /* Initialize AT device */
    result = class->device_ops->init(device);
    if (result < 0)
    {
        goto __exit;
    }

__exit:
    if (result < 0)
    {
        device->is_init = RT_FALSE;
    }
    else
    {
        device->is_init = RT_TRUE;
    }

    return RT_EOK;
}
