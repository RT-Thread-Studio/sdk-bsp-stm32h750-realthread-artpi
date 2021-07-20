/*
 * File      : at_device.h
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
 * 2019-05-08     chenyong     first version
 */

#ifndef __AT_DEVICE_H__
#define __AT_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <at.h>
#include <at_socket.h>

#if !defined(RT_USING_NETDEV) || (!defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10300)
#error "This RT-Thread version is older, please check and updata laster RT-Thread!"
#else

#include <arpa/inet.h>
#include <netdev.h>
#endif /* RT_USING_NETDEV */

#define AT_DEVICE_SW_VERSION           "2.0.4"
#define AT_DEVICE_SW_VERSION_NUM       0x20004

/* AT device class ID */
#define AT_DEVICE_CLASS_ESP8266        0x01U
#define AT_DEVICE_CLASS_M26_MC20       0x02U
#define AT_DEVICE_CLASS_EC20           0x03U
#define AT_DEVICE_CLASS_SIM800C        0x04U
#define AT_DEVICE_CLASS_SIM76XX        0x05U
#define AT_DEVICE_CLASS_RW007          0x06U
#define AT_DEVICE_CLASS_MW31           0x07U
#define AT_DEVICE_CLASS_ESP32          0x08U
#define AT_DEVICE_CLASS_W60X           0x09U
#define AT_DEVICE_CLASS_A9G            0x0AU
#define AT_DEVICE_CLASS_BC26           0x0BU
#define AT_DEVICE_CLASS_AIR720         0x0CU
#define AT_DEVICE_CLASS_ME3616         0x0DU
#define AT_DEVICE_CLASS_M6315          0x0EU
#define AT_DEVICE_CLASS_BC28           0x0FU
#define AT_DEVICE_CLASS_EC200X         0x10U
#define AT_DEVICE_CLASS_N21            0x11U
#define AT_DEVICE_CLASS_N58            0x12U
#define AT_DEVICE_CLASS_M5311          0X13U
#define AT_DEVICE_CLASS_N720           0X14U
#define AT_DEVICE_CLASS_L610           0X15U

/* Options and Commands for AT device control opreations */
#define AT_DEVICE_CTRL_POWER_ON        0x01L
#define AT_DEVICE_CTRL_POWER_OFF       0x02L
#define AT_DEVICE_CTRL_RESET           0x03L
#define AT_DEVICE_CTRL_LOW_POWER       0x04L
#define AT_DEVICE_CTRL_SLEEP           0x05L
#define AT_DEVICE_CTRL_WAKEUP          0x06L
#define AT_DEVICE_CTRL_NET_CONN        0x07L
#define AT_DEVICE_CTRL_NET_DISCONN     0x08L
#define AT_DEVICE_CTRL_SET_WIFI_INFO   0x09L
#define AT_DEVICE_CTRL_GET_SIGNAL      0x0AL
#define AT_DEVICE_CTRL_GET_GPS         0x0BL
#define AT_DEVICE_CTRL_GET_VER         0x0CL

/* Name type */
#define AT_DEVICE_NAMETYPE_DEVICE      0x01
#define AT_DEVICE_NAMETYPE_NETDEV      0x02
#define AT_DEVICE_NAMETYPE_CLIENT      0x03

struct at_device;

/* AT device wifi ssid and password information */
struct at_device_ssid_pwd
{
    char *ssid;
    char *password;
};

/* AT device operations */
struct at_device_ops
{
    int (*init)(struct at_device *device);
    int (*deinit)(struct at_device *device);
    int (*control)(struct at_device *device, int cmd, void *arg);
};

struct at_device_class
{
    uint16_t class_id;                           /* AT device class ID */
    const struct at_device_ops *device_ops;      /* AT device operaiotns */
#ifdef AT_USING_SOCKET
    uint32_t socket_num;                         /* The maximum number of sockets support */
    const struct at_socket_ops *socket_ops;      /* AT device socket operations */
#endif
    rt_slist_t list;                             /* AT device class list */
};

struct at_device
{
    char name[RT_NAME_MAX];                      /* AT device name */
    rt_bool_t is_init;                           /* AT device initialization completed */
    struct at_device_class *class;               /* AT device class object */
    struct at_client *client;                    /* AT Client object for AT device */
    struct netdev *netdev;                       /* Network interface device for AT device */
#ifdef AT_USING_SOCKET
    rt_event_t socket_event;                     /* AT device socket event */
    struct at_socket *sockets;                   /* AT device sockets list */
#endif
    rt_slist_t list;                             /* AT device list */

    void *user_data;                             /* User-specific data */
};

/* Get AT device object */
struct at_device *at_device_get_first_initialized(void);
struct at_device *at_device_get_by_name(int type, const char *name);
#ifdef AT_USING_SOCKET
struct at_device *at_device_get_by_socket(int at_socket);
#endif

/* AT device control operaions */
int at_device_control(struct at_device *device, int cmd, void *arg);
/* Register AT device class object */
int at_device_class_register(struct at_device_class *class, uint16_t class_id);
/* Register AT device object */
int at_device_register(struct at_device *device, const char *device_name,
                        const char *at_client_name, uint16_t class_id, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* __AT_DEVICE_H__ */
