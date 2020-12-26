/*
 * File      : at_sample_rw007.c
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

#include <at_device_rw007.h>

#define LOG_TAG                        "at.sample.rw007"
#include <at_log.h>

#define RW007_SAMPLE_DEIVCE_NAME       "r0"

static struct at_device_rw007 r0 =
{
    RW007_SAMPLE_DEIVCE_NAME,
    RW007_SAMPLE_CLIENT_NAME,

    RW007_SAMPLE_WIFI_SSID,
    RW007_SAMPLE_WIFI_PASSWORD,
    RW007_SAMPLE_RECV_BUFF_LEN,
};

static int rw007_device_register(void)
{
    struct at_device_rw007 *rw007 = &r0;

    return at_device_register(&(rw007->device),
                              rw007->device_name,
                              rw007->client_name,
                              AT_DEVICE_CLASS_RW007,
                              (void *) rw007);
}
INIT_APP_EXPORT(rw007_device_register);
