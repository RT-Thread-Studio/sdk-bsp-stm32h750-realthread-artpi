/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <js_persim.h>
#include <ftp.h>
#include "mp3.h"

void persim_auto_start(void);

#define LED_PIN GET_PIN(I, 8)

extern void wlan_autoconnect_init(void);

static void rt_wlan_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("event %d\n", event);
//    rt_thread_mdelay(5000);
}

int main(void)
{
    rt_uint32_t count = 1;
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    /* init Wi-Fi auto connect feature */
    wlan_autoconnect_init();
    /* enable auto reconnect on WLAN device */
    rt_wlan_config_autoreconnect(RT_TRUE);

    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, rt_wlan_handler, RT_NULL);

    persim_auto_start();

    /* ftp init */
    ftp_set_max_session_num(20);
    ftp_init(2048, 27, 100);

    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
    return RT_EOK;
}

extern int js_user_init(void);
extern void persim_auto_start(void);
extern int jp_app_ready_init(void);
extern int rtgui_system_server_init(void);
extern void turn_on_lcd_backlight(void);
extern int touch_init(void);
static rt_bool_t persim_is_init = RT_FALSE;
extern rt_bool_t sdcard_mount_ok;

static void persim_start_entry(void *param)
{
    while(1)
    {
        if (sdcard_mount_ok)
        {
            rt_thread_mdelay(1000);
            js_user_init();
            jp_app_ready_init();
            rtgui_system_server_init();
            js_persim_thread_init("/flash/player/app.js");
            js_app_ready_sem_release();
            persim_is_init = RT_TRUE;
            turn_on_lcd_backlight();

            player_init();

            touch_init();

            break;
        }
        rt_thread_mdelay(1000);
    }
}

void persim_auto_start(void)
{
    rt_thread_t tid;

    if (persim_is_init)
    {
        rt_kprintf("Persimmon is already start!\n");
        return;
    }

    tid = rt_thread_create("persim_start", persim_start_entry, RT_NULL, 2048, 20, 10);
    if (tid)
    {
        rt_thread_startup(tid);
    }
}
MSH_CMD_EXPORT(persim_auto_start, .......);

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);
