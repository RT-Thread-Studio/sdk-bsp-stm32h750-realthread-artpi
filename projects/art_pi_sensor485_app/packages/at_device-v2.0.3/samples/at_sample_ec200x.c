/*
 * File      : at_sample_ec200x.c
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
 * Date           Author            Notes
 * 2019-12-13     qiyongzhong       first version
 */

#include <at_device_ec200x.h>

#define LOG_TAG                        "at.sample.ec200x"
#include <at_log.h>

#define EC200X_SAMPLE_DEIVCE_NAME        "ec200x"

static struct at_device_ec200x _dev =
{
    EC200X_SAMPLE_DEIVCE_NAME,
    EC200X_SAMPLE_CLIENT_NAME,

    EC200X_SAMPLE_POWER_PIN,
    EC200X_SAMPLE_STATUS_PIN,
    EC200X_SAMPLE_WAKEUP_PIN,
    EC200X_SAMPLE_RECV_BUFF_LEN,
};

static int ec200x_device_register(void)
{
    struct at_device_ec200x *ec200x = &_dev;

    return at_device_register(&(ec200x->device),
                              ec200x->device_name,
                              ec200x->client_name,
                              AT_DEVICE_CLASS_EC200X,
                              (void *) ec200x);
}
INIT_APP_EXPORT(ec200x_device_register);

