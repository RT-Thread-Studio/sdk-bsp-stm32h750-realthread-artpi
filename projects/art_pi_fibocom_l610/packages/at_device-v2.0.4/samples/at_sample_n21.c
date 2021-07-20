/*
 * File      : at_sample_n21.c
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
 * 2019-12-07     liang.shao     first version
 */

#include <at_device_n21.h>

#define LOG_TAG                        "at.sample"
#include <at_log.h>

#define N21_SAMPLE_DEIVCE_NAME     "n21"

static struct at_device_n21 sim0 =
{
    N21_SAMPLE_DEIVCE_NAME,
    N21_SAMPLE_CLIENT_NAME,

    N21_SAMPLE_POWER_PIN,
    N21_SAMPLE_STATUS_PIN,
    N21_SAMPLE_RECV_BUFF_LEN,
};

static int n21_device_register(void)
{
    struct at_device_n21 *n21 = &sim0;

    return at_device_register(&(n21->device),
                              n21->device_name,
                              n21->client_name,
                              AT_DEVICE_CLASS_N21,
                              (void *) n21);
}
INIT_APP_EXPORT(n21_device_register);
//MSH_CMD_EXPORT(n21_device_register,n21_device_register);
