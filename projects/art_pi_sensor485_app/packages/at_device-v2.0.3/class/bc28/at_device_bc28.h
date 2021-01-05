 /*
 * File      : at_device_bc28.h
 * This file is part of RT-Thread RTOS
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
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
 * 2020-02-12     luhuadong         first version
 */

#ifndef __AT_DEVICE_BC28_H__
#define __AT_DEVICE_BC28_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include <at_device.h>

/* The maximum number of sockets supported by the BC28 device */
#define AT_DEVICE_BC28_SOCKETS_NUM  7
#define IP_ADDR_SIZE_MAX            16

#define AT_DEVICE_BC28_MIN_SOCKET   BC28_SAMPLE_MIN_SOCKET
#define BC28_AT_CLIENT_BAUD_RATE    BC28_SAMPLE_BAUD_RATE

struct at_device_bc28
{
    char *device_name;
    char *client_name;

    int power_pin;          /* BC28 has not power_en, it should be reset pin */
    int power_status_pin;   /* ADC */
    size_t recv_bufsz;
    struct at_device device;

    void *socket_data;
    void *user_data;

    rt_bool_t power_status;
    rt_bool_t sleep_status;
};

#ifdef AT_USING_SOCKET

/* bc28 device socket initialize */
int bc28_socket_init(struct at_device *device);

/* bc28 device class socket register */
int bc28_socket_class_register(struct at_device_class *class);

#endif /* AT_USING_SOCKET */

#ifdef __cplusplus
}
#endif

#endif /* __AT_DEVICE_BC28_H__ */
