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
#include <netdev_ipaddr.h>
#include <netdev.h>
#include <dfs_fs.h>

#include "web.h"
#include "basic.h"
#include "monitor.h"
#include "bt_module.h"
#include "wifi.h"

#define DBG_COLOR
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <ulog.h> // 必须在 LOG_TAG 与 LOG_LVL 下面
#include <rtdbg.h>

int main(void)
{
    RT_ASSERT(ulog_init() >= 0);
    wifi_init();
    basic_init();
    sys_monitor_init();
    web_init();
    bluetooth_init();

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
