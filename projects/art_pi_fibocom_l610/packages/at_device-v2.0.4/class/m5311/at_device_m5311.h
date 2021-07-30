/*
 * File      : at_device_m5311.h
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
 * 2020-03-09     LXGMAX     first version
 */

#ifndef __AT_DEVICE_M5311_H__
#define __AT_DEVICE_M5311_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include <at_device.h>

/* Max number of sockets supported by the m5311 device */
#define AT_DEVICE_M5311_SOCKETS_NUM     5

struct at_device_m5311{
        char *device_name;
        char *client_name;

        int power_pin;
        size_t recieve_line_num;
        struct at_device device;

        void *socket_data;
        void *user_data;
};

#ifdef AT_USING_SOCKET
/* m5311 device socket initialize */
int m5311_socket_init(struct at_device *device);

/* m5311 device class socket register */
int m5311_socket_class_register(struct at_device_class *class);

#endif /* AT_USING_SOCKET */

#ifdef __cplusplus
}
#endif

#endif /* __AT_DEVICE_M5311_H__ */
