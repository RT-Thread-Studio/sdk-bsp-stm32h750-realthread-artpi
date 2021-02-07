/*
 * qboot_stm32.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-08-31     qiyongzhong       first version
 */

#include <board.h>

#ifdef CHIP_FAMILY_STM32

#include <rtthread.h>
#include <rtdevice.h>
#include <qboot.h>

//#define QBOOT_APP_RUN_IN_QSPI_FLASH
#define QBOOT_QSPI_FLASH_DEVICE_NAME    "qspi10"

//#define QBOOT_DEBUG
#define QBOOT_USING_LOG
#define DBG_TAG "Qboot"

#ifdef QBOOT_DEBUG
#define DBG_LVL DBG_LOG
#else
#define DBG_LVL DBG_INFO
#endif

#ifdef QBOOT_USING_LOG
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#ifndef DBG_COLOR
#define DBG_COLOR
#endif
#endif

#include <rtdbg.h>
#include "w25qxx.h"

#define QBOOT_APP_RUN_IN_QSPI_FLASH

#ifdef QBOOT_APP_RUN_IN_QSPI_FLASH

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x90000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;

void qbt_jump_to_app(void)
{
    typedef void (*app_func_t)(void);
    app_func_t app_func;
    struct rt_qspi_device *device = NULL;
    u32 stk_addr = 0;
    u32 reset_handler = 0;
    const u8 send[4] = {0x03, 0x00, 0x00, 0x00};
    u8 recv[8] = {0};

    device = (struct rt_qspi_device *)rt_device_find(QBOOT_QSPI_FLASH_DEVICE_NAME);

    if(device == RT_NULL)
    {
        LOG_E("no found qspi flash.");
        return;
    }

    rt_qspi_send_then_recv(device, send, sizeof(send), recv, sizeof(recv));

    stk_addr = recv[0];
    stk_addr += ((u32)recv[1] << 8);
    stk_addr += ((u32)recv[2] << 16);
    stk_addr += ((u32)recv[3] << 24);

    reset_handler = recv[4];
    reset_handler += ((u32)recv[5] << 8);
    reset_handler += ((u32)recv[6] << 16);
    reset_handler += ((u32)recv[7] << 24);

    stk_addr &= 0x3FF00000;
    if(((reset_handler & 0xff000000) != QSPI_BASE) ||
        (stk_addr != 0x20000000 && stk_addr != 0x24000000 &&
            stk_addr != 0x30000000 && stk_addr != 0x38000000))
    {
        LOG_E("No legitimate application.");
        return;
    }

    W25QXX_ExitQPIMode();
    W25QXX_Reset();

    W25QXX_Init();

    W25Q_Memory_Mapped_Enable();

    #if (__MPU_PRESENT == 1)
    HAL_MPU_Disable();
    #endif

    SCB_DisableICache();
    SCB_DisableDCache();

    SysTick->CTRL = 0;
    __disable_irq();
    __set_CONTROL(0);
    __set_MSP(stk_addr);
    
    app_func = (app_func_t) (*(__IO uint32_t*) (QSPI_BASE + 4));
    app_func();

    LOG_E("Qboot jump to application fail.");
}
#else
void qbt_jump_to_app(void)
{
    typedef void (*app_func_t)(void);
    u32 app_addr = QBOOT_APP_ADDR;
    u32 stk_addr = *((__IO uint32_t *)app_addr);
    app_func_t app_func = (app_func_t)(*((__IO uint32_t *)(app_addr + 4)));

    if ((((u32)app_func & 0xff000000) != 0x08000000) || (((stk_addr & 0x2ff00000) != 0x20000000) && ((stk_addr & 0x2ff00000) != 0x24000000)))
    {
        LOG_E("No legitimate application.");
        return;
    }

    rt_kprintf("Jump to application running ... \n");
    rt_thread_mdelay(200);
    
    __disable_irq();
    HAL_DeInit();

    for(int i=0; i<128; i++)
    {
        HAL_NVIC_DisableIRQ(i);
        HAL_NVIC_ClearPendingIRQ(i);
    }
    
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    
    HAL_RCC_DeInit();
    
    __set_CONTROL(0);
    __set_MSP(stk_addr);
    
    app_func();//Jump to application running
    
    LOG_E("Qboot jump to application fail.");
}
#endif
#endif

