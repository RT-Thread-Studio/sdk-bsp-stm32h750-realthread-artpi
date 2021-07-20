/*
 * File      : at_sample_bc26.c
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

#include <at_device_bc26.h>

#define LOG_TAG                        "at.sample.bc26"
#include <at_log.h>

#define BC26_SAMPLE_DEIVCE_NAME        "bc26"

static struct at_device_bc26 _dev =
{
    BC26_SAMPLE_DEIVCE_NAME,
    BC26_SAMPLE_CLIENT_NAME,

    BC26_SAMPLE_POWER_PIN,
    BC26_SAMPLE_STATUS_PIN,
    BC26_SAMPLE_RECV_BUFF_LEN,
};

static int bc26_device_register(void)
{
    struct at_device_bc26 *bc26 = &_dev;

    return at_device_register(&(bc26->device),
                              bc26->device_name,
                              bc26->client_name,
                              AT_DEVICE_CLASS_BC26,
                              (void *) bc26);
}
INIT_APP_EXPORT(bc26_device_register);

