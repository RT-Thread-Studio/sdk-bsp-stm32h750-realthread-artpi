/*
 * File      : at_sample_ec20.c
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
 * 2019-05-13     chenyong     first version
 */

#include <at_device_ec20.h>

#define LOG_TAG                        "at.sample.ec20"
#include <at_log.h>

#define EC20_SAMPLE_DEIVCE_NAME        "e0"

static struct at_device_ec20 e0 =
{
    EC20_SAMPLE_DEIVCE_NAME,
    EC20_SAMPLE_CLIENT_NAME,

    EC20_SAMPLE_POWER_PIN,
    EC20_SAMPLE_STATUS_PIN,
    EC20_SAMPLE_RECV_BUFF_LEN,
};

static int ec20_device_register(void)
{
    struct at_device_ec20 *ec20 = &e0;

    return at_device_register(&(ec20->device),
                              ec20->device_name,
                              ec20->client_name,
                              AT_DEVICE_CLASS_EC20,
                              (void *) ec20);
}
INIT_APP_EXPORT(ec20_device_register);
