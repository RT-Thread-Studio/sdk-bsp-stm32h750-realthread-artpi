/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-08     xqyjlj       the first version
 */
#include "ld3320_base.h"

#ifdef PKG_USING_LD3320_RECV_REPORT
#define DBG_TAG "ld_base"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#endif
static rt_err_t ld3320_spi_send(struct rt_spi_device *device, uint8_t *send_buf1, uint8_t *send_buf2, uint8_t *send_buf3)
{
    rt_err_t result;

    struct rt_spi_message message;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);

    result = rt_mutex_take(&(device->bus->lock), RT_WAITING_FOREVER);
    if (result == RT_EOK)
    {
        if (device->bus->owner != device)
        {
            result = device->bus->ops->configure(device, &device->config);
            if (result == RT_EOK)
            {
                device->bus->owner = device;
            }
            else
            {
                result = -RT_EIO;
                goto __exit;
            }
        }

        /* send data1 */
        message.send_buf = send_buf1;
        message.recv_buf = RT_NULL;
        message.length = 1;
        message.cs_take = 1;
        message.cs_release = 0;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        /* send data2 */
        message.send_buf = send_buf2;
        message.recv_buf = RT_NULL;
        message.length = 1;
        message.cs_take = 0;
        message.cs_release = 0;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        /* send data3 */
        message.send_buf = send_buf3;
        message.recv_buf = RT_NULL;
        message.length = 1;
        message.cs_take = 0;
        message.cs_release = 1;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        result = RT_EOK;
    }
    else
    {
        return -RT_EIO;
    }

__exit: rt_mutex_release(&(device->bus->lock));

    return result;
}

static rt_err_t ld3320_spi_recv(struct rt_spi_device *device, uint8_t *send_buf1, uint8_t *send_buf2, uint8_t *recv_buf)
{
    rt_err_t result;
    struct rt_spi_message message;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);

    result = rt_mutex_take(&(device->bus->lock), RT_WAITING_FOREVER);
    if (result == RT_EOK)
    {
        if (device->bus->owner != device)
        {
            result = device->bus->ops->configure(device, &device->config);
            if (result == RT_EOK)
            {
                device->bus->owner = device;
            }
            else
            {
                result = -RT_EIO;
                goto __exit;
            }
        }

        /* send data1 */
        message.send_buf = send_buf1;
        message.recv_buf = RT_NULL;
        message.length = 1;
        message.cs_take = 1;
        message.cs_release = 0;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        /* send data2 */
        message.send_buf = send_buf2;
        message.recv_buf = RT_NULL;
        message.length = 1;
        message.cs_take = 0;
        message.cs_release = 0;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        /* recv data */
        message.send_buf = RT_NULL;
        message.recv_buf = recv_buf;
        message.length = 1;
        message.cs_take = 0;
        message.cs_release = 1;
        message.next = RT_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -RT_EIO;
            goto __exit;
        }

        result = RT_EOK;
    }
    else
    {
        return -RT_EIO;
    }

__exit: rt_mutex_release(&(device->bus->lock));

    return result;
}


void ld3320_write_reg(struct rt_spi_device *device, uint8_t addr, uint8_t data)
{
    static uint8_t tmp_addr;
    static uint8_t tmp_data;
    static uint8_t tmp_write = 0x04;
    tmp_addr = addr;
    tmp_data = data;

    ld3320_spi_send(device, &tmp_write, &tmp_addr, &tmp_data);
}
uint8_t ld3320_read_reg(struct rt_spi_device *device, uint8_t addr)
{
    static uint8_t tmp_addr;
    static uint8_t tmp_read = 0x05;
    tmp_addr = addr;
    static uint8_t data;
    ld3320_spi_recv(device, &tmp_read, &tmp_addr, &data);
#ifdef PKG_USING_LD3320_RECV_REPORT
    LOG_D("read: 0x%02X->0x%02X", tmp_addr, data);
#endif
    return data;
}

