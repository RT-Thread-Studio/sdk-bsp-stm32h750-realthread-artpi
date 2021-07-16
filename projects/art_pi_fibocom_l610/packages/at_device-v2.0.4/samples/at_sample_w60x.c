/*
 * File      : at_sample_w60x.c
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
 * 2019-05-10     chenyong     first version
 */

#include <at_device_w60x.h>

#define LOG_TAG                        "at.sample.w60x"
#include <at_log.h>

#define W60X_SAMPLE_DEIVCE_NAME     "w60x0"

static struct at_device_w60x w60x0 =
{
    W60X_SAMPLE_DEIVCE_NAME,
    W60X_SAMPLE_CLIENT_NAME,

    W60X_SAMPLE_WIFI_SSID,
    W60X_SAMPLE_WIFI_PASSWORD,
    W60X_SAMPLE_RECV_BUFF_LEN,
};

static int w60x_device_register(void)
{
    struct at_device_w60x *w60x = &w60x0;

    return at_device_register(&(w60x->device),
                              w60x->device_name,
                              w60x->client_name,
                              AT_DEVICE_CLASS_W60X,
                              (void *) w60x);
}
INIT_APP_EXPORT(w60x_device_register);
