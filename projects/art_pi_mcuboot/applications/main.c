/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-15     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_common.h>
#include "w25qxx.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "bootutil/fault_injection_hardening.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(I, 8)

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x90000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;
struct boot_rsp g_mcuboot_rsp;

struct arm_vector_table {
    uint32_t msp;
    uint32_t reset;
};

/**
  * @brief 启动 image
  * @param struct boot_rsp * rsp: 
  * retval N/A.
  */
static void do_boot(struct boot_rsp * rsp)
{
    struct arm_vector_table *vt;

    LOG_I("imgae_off=%x flashid=%d", rsp->br_image_off, rsp->br_flash_dev_id);
    LOG_I("raw_imgae_off=%x", (rsp->br_image_off + rsp->br_hdr->ih_hdr_size));

    W25Q_Memory_Mapped_Enable();
    vt = (struct arm_vector_table *)(rsp->br_image_off + rsp->br_hdr->ih_hdr_size);
    SysTick->CTRL = 0;

    SCB->VTOR = (uint32_t)vt;
    __set_MSP(*(__IO uint32_t *)vt->msp);
    ((void (*)(void))vt->reset)();

    while(1)
    {
        rt_thread_mdelay(500);
    }
}

int main(void)
{
    fih_int fih_ret;

    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    W25QXX_Init();

    SCB_DisableICache();
    SCB_DisableDCache();

    LOG_I("Hello mcuboot .");
    fih_ret = boot_go(&g_mcuboot_rsp);
    if (FIH_SUCCESS == fih_ret)
    {
        LOG_I("Get rsp success.");
        do_boot(&g_mcuboot_rsp);
    }
    else
    {
        LOG_E("Get rsp failed.");
    }

    return RT_ERROR;
}

/**
  * @brief Mark swap when next boot
  * @param int argc: 
  * @param char * argv[]: 
  * retval .
  */
static int do_mark_swap(int argc, char * argv[])
{
    boot_set_pending(0);

    return 0;
}
MSH_CMD_EXPORT(do_mark_swap, mark swap for mcuboot);
