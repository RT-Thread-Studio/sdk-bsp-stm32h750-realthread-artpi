/*
 * File      : at_sample_bc28.c
 * This file is part of RT-Thread RTOS
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
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
 * 2020-02-13     luhuadong         first version
 */

#include <at_device_bc28.h>

#define LOG_TAG                        "at.sample.bc28"
#include <at_log.h>

#define BC28_SAMPLE_DEIVCE_NAME        "bc28"

static struct at_device_bc28 _dev =
{
    BC28_SAMPLE_DEIVCE_NAME,
    BC28_SAMPLE_CLIENT_NAME,

    BC28_SAMPLE_POWER_PIN,
    BC28_SAMPLE_STATUS_PIN,
    BC28_SAMPLE_RECV_BUFF_LEN,
};

static int bc28_device_register(void)
{
    struct at_device_bc28 *bc28 = &_dev;

    return at_device_register(&(bc28->device),
                              bc28->device_name,
                              bc28->client_name,
                              AT_DEVICE_CLASS_BC28,
                              (void *) bc28);
}
INIT_APP_EXPORT(bc28_device_register);

