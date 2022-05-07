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
#include "lora_pkt_sniffer.h"

#define LED_PIN GET_PIN(I, 8)

extern void wlan_autoconnect_init(void);

static void wifi_ready_handler(int event, struct rt_wlan_buff *buff, void *param)
{
    /* lora packet sniffer init */
    lps_init();
}

static int wifi_ready_register(void)
{
    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wifi_ready_handler, NULL);
    return 0;
}

int main(void)
{
    rt_uint32_t count = 1;
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    lps_nvm_init();

    if( lps_conf_sys.northboud_if == LPS_NORTHBOUND_IF_OVER_WIFI )
    {
        /* init Wi-Fi auto connect feature */
        wlan_autoconnect_init();
        
        /* enable auto reconnect on WLAN device */
        rt_wlan_config_autoreconnect(RT_TRUE);
        
        /* wifi ready event register */
        wifi_ready_register();
    }
    else
    {            
        /* lora packet sniffer init for nif uart */
        lps_init();
    }
    
    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


