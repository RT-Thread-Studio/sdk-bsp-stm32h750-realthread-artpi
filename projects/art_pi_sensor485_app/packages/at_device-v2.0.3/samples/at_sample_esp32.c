/*
 * File      : at_sample_esp32.c
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

#include <at_device_esp32.h>

#define LOG_TAG                        "at.sample.esp"
#include <at_log.h>

#define ESP32_SAMPLE_DEIVCE_NAME     "esp32"

static struct at_device_esp32 esp0 =
{
    ESP32_SAMPLE_DEIVCE_NAME,
    ESP32_SAMPLE_CLIENT_NAME,

    ESP32_SAMPLE_WIFI_SSID,
    ESP32_SAMPLE_WIFI_PASSWORD,
    ESP32_SAMPLE_RECV_BUFF_LEN,
};

static int esp32_device_register(void)
{
    struct at_device_esp32 *esp32 = &esp0;

    return at_device_register(&(esp32->device),
                              esp32->device_name,
                              esp32->client_name,
                              AT_DEVICE_CLASS_ESP32,
                              (void *) esp32);
}
INIT_APP_EXPORT(esp32_device_register);
