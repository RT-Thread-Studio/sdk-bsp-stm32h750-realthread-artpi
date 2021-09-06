/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-10     xqyjlj       the first version
 */
#include <rtthread.h>

#ifdef PKG_LD3320_DEMO_ROLE_ASR

#if (PKG_LD3320_DEMO_IRQ_PIN < 0)
#error Please specify a valid pin
#endif

#include "drv_spi.h"
#define DBG_TAG "ld3320"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "ld3320.h"


static void ld3320_a_asr_over_callback(uint8_t num)
{
    switch (num)
    {
        case 1:
        LOG_D("开始");
            break;
        case 2:
        LOG_D("关闭");
            break;
        case 3:
        LOG_D("暂停");
            break;
        default:
            break;
    }
}

static void ld3320_asr_thread(void * parameter)
{
    static ld3320_t _ld3320;
    _ld3320 = ld3320_create(PKG_LD3320_DEMO_SPI_DEV_NAME, PKG_LD3320_DEMO_WR_PIN, PKG_LD3320_DEMO_RST_PIN, PKG_LD3320_DEMO_IRQ_PIN, LD3320_MODE_ASR);
    ld3320_set_asr_over_callback(_ld3320, ld3320_a_asr_over_callback);

    ld3320_addcommand_tolist(_ld3320, "kai shi", 1);
    ld3320_addcommand_tolist(_ld3320, "guan bi", 2);
    ld3320_addcommand_tolist(_ld3320, "zan ting", 3);
    ld3320_addcommand_fromlist(_ld3320);

    ld3320_asr_start(_ld3320);
    while (1)
    {
        ld3320_run(_ld3320, LD3320_MODE_ASR);
        rt_thread_mdelay(100);
    }
}

static int create_ld3320_asr_thread(void)
{
    rt_thread_t thread = RT_NULL;
    thread = rt_thread_create("ld_asr", ld3320_asr_thread, RT_NULL, 500, 15, 100);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    return RT_EOK;
}

INIT_APP_EXPORT(create_ld3320_asr_thread);

#endif
