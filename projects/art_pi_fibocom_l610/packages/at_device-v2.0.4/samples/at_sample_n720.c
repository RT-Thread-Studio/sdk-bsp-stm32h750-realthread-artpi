/*
 * File      : at_sample_n720.c
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

#include <at_device_n720.h>

#define LOG_TAG                         "at.sample.n720"
#include <at_log.h>

#define N720_SAMPLE_DEIVCE_NAME         "n720"

#ifndef N720_SAMPLE_CLIENT_NAME
#define N720_SAMPLE_CLIENT_NAME         "uart4"
#endif

#ifndef N720_SAMPLE_POWER_PIN
#define N720_SAMPLE_POWER_PIN           23
#endif

#ifndef N720_SAMPLE_STATUS_PIN
#define N720_SAMPLE_STATUS_PIN          -1
#endif

#ifndef N720_SAMPLE_WAKEUP_PIN
#define N720_SAMPLE_WAKEUP_PIN          -1
#endif

#ifndef N720_SAMPLE_RECV_BUFF_LEN
#define N720_SAMPLE_RECV_BUFF_LEN       512
#endif

static struct at_device_n720 _dev =
{
    N720_SAMPLE_DEIVCE_NAME,
    N720_SAMPLE_CLIENT_NAME,

    N720_SAMPLE_POWER_PIN,
    N720_SAMPLE_STATUS_PIN,
    N720_SAMPLE_WAKEUP_PIN,
    N720_SAMPLE_RECV_BUFF_LEN,
    RT_NULL,
};

static int n720_device_register(void)
{
    struct at_device_n720 *n720 = &_dev;

    return at_device_register(&(n720->device),
                              n720->device_name,
                              n720->client_name,
                              AT_DEVICE_CLASS_N720,
                              (void *) n720);
}
INIT_APP_EXPORT(n720_device_register);

