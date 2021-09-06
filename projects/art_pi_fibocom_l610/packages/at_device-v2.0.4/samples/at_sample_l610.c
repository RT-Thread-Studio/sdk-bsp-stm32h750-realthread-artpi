/*
 * File      : at_sample_l610.c
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
 * 2020-10-28     zhangyang     first version
 */

#include <at_device_l610.h>

#define LOG_TAG                         "at.sample.l610"
#include <at_log.h>

#define L610_SAMPLE_DEIVCE_NAME         "l610"

static struct at_device_l610 sim0 =
{
    L610_SAMPLE_DEIVCE_NAME,
    L610_SAMPLE_CLIENT_NAME,

    L610_SAMPLE_POWER_PIN,
    L610_SAMPLE_STATUS_PIN,
    L610_SAMPLE_RECV_BUFF_LEN,
};

static int l610_device_register(void)
{
    struct at_device_l610 *l610 = &sim0;

    return at_device_register(&(l610->device),
                            l610->device_name,
                            l610->client_name,
                            AT_DEVICE_CLASS_L610,
                            (void *) l610);
}
INIT_APP_EXPORT(l610_device_register);
