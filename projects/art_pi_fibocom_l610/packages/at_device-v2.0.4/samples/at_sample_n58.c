/*
 * File      : at_sample_n58.c
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
 * Date           Author         Notes
 * 2020-05-22     shuobatian     first version
 */

#include <at_device_n58.h>

#define LOG_TAG                        "at.sample"
#include <at_log.h>

#define N58_SAMPLE_DEIVCE_NAME     "n58"

static struct at_device_n58 sim0 =
{
    N58_SAMPLE_DEIVCE_NAME,
    N58_SAMPLE_CLIENT_NAME,

    N58_SAMPLE_POWER_PIN,
    N58_SAMPLE_STATUS_PIN,
    N58_SAMPLE_RECV_BUFF_LEN,
};

static int n58_device_register(void)
{
    struct at_device_n58 *n58 = &sim0;

    return at_device_register(&(n58->device),
                              n58->device_name,
                              n58->client_name,
                              AT_DEVICE_CLASS_N58,
                              (void *) n58);
}
INIT_APP_EXPORT(n58_device_register);
//MSH_CMD_EXPORT(n58_device_register,n58_device_register);
