/*
 * File      : at_sample_me3616.c
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
 * 2019-12-30     qiyongzhong       first version
 */

#include <at_device_me3616.h>

#define LOG_TAG                        "at.sample.me3616"
#include <at_log.h>

#define ME3616_SAMPLE_DEIVCE_NAME        "me3616"

static struct at_device_me3616 _dev =
{
    ME3616_SAMPLE_DEIVCE_NAME,
    ME3616_SAMPLE_CLIENT_NAME,

    ME3616_SAMPLE_POWER_PIN,
    ME3616_SAMPLE_STATUS_PIN,
    ME3616_SAMPLE_RECV_BUFF_LEN,
};

static int me3616_device_register(void)
{
    struct at_device_me3616 *me3616 = &_dev;

    return at_device_register(&(me3616->device),
                              me3616->device_name,
                              me3616->client_name,
                              AT_DEVICE_CLASS_ME3616,
                              (void *) me3616);
}
INIT_APP_EXPORT(me3616_device_register);

