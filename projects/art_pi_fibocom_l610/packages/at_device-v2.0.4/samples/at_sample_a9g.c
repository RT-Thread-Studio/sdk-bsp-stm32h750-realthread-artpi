/*
 * File      : at_sample_a9g.c
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
 * 2019-11-23     luliang     first version
 */

#include <at_device_a9g.h>

#define LOG_TAG                   "at.sample.a9g"
#include <at_log.h>

#define A9G_SAMPLE_DEIVCE_NAME     "a9g0"

static struct at_device_a9g a9g0 =
{
    A9G_SAMPLE_DEIVCE_NAME,
    A9G_SAMPLE_CLIENT_NAME,

    A9G_SAMPLE_POWER_PIN,
    A9G_SAMPLE_STATUS_PIN,
    A9G_SAMPLE_RECV_BUFF_LEN,
};

static int a9g_device_register(void)
{
    struct at_device_a9g *a9g = &a9g0;

    return at_device_register(&(a9g->device),
                              a9g->device_name,
                              a9g->client_name,
                              AT_DEVICE_CLASS_A9G,
                              (void *) a9g);
}
INIT_APP_EXPORT(a9g_device_register);
