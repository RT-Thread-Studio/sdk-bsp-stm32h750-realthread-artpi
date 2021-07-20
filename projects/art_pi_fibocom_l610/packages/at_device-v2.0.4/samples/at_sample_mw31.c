/*
 * File      : at_sample_mw31.c
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
 * 2019-06-23     flybreak     first version
 */

#include <at_device_mw31.h>

#define LOG_TAG                        "at.sample.mw31"
#include <at_log.h>

#define MW31_SAMPLE_DEIVCE_NAME     "mw0"

static struct at_device_mw31 mw0 =
{
    MW31_SAMPLE_DEIVCE_NAME,
    MW31_SAMPLE_CLIENT_NAME,

    MW31_SAMPLE_WIFI_SSID,
    MW31_SAMPLE_WIFI_PASSWORD,
    MW31_SAMPLE_RECV_BUFF_LEN,
};

static int mw31_device_register(void)
{
    struct at_device_mw31 *mw31 = &mw0;

    return at_device_register(&(mw31->device),
                              mw31->device_name,
                              mw31->client_name,
                              AT_DEVICE_CLASS_MW31,
                              (void *) mw31);
}
INIT_APP_EXPORT(mw31_device_register);
