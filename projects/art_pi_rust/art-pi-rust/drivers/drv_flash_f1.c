/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-5      SummerGift   first version
 * 2020-03-05     redoc        support stm32f103vg
 *
 */

#include "board.h"

#ifdef BSP_USING_ON_CHIP_FLASH
#include "drv_config.h"
#include "drv_flash.h"

#if defined(PKG_USING_FAL)
#include "fal.h"
#endif

//#define DRV_DEBUG
#define LOG_TAG                "drv.flash"
#include <drv_log.h>

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t addr)
{
    uint32_t page = 0;
    page = RT_ALIGN_DOWN(addr, FLASH_PAGE_SIZE);
    return page;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
int stm32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    size_t i;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        LOG_E("read outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }

    for (i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(rt_uint8_t *) addr;
    }

    return size;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
int stm32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size)
{
    rt_err_t result        = RT_EOK;
    rt_uint32_t end_addr   = addr + size;

    if (addr % 4 != 0)
    {
        LOG_E("write addr must be 4-byte alignment");
        return -RT_EINVAL;
    }

    if ((end_addr) > STM32_FLASH_END_ADDRESS)
    {
        LOG_E("write outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }

    HAL_FLASH_Unlock();

    while (addr < end_addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *((rt_uint32_t *)buf)) == HAL_OK)
        {
            if (*(rt_uint32_t *)addr != *(rt_uint32_t *)buf)
            {
                result = -RT_ERROR;
                break;
            }
            addr += 4;
            buf  += 4;
        }
        else
        {
            result = -RT_ERROR;
            break;
        }
    }

    HAL_FLASH_Lock();

    if (result != RT_EOK)
    {
        return result;
    }

    return size;
}

/**
 * Erase data on flash with bank.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param bank flash bank
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase_bank(uint32_t bank, rt_uint32_t addr, size_t size)
{
    rt_err_t result = RT_EOK;
    uint32_t PAGEError = 0;

    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        LOG_E("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)(addr + size));
        return -RT_EINVAL;
    }

    HAL_FLASH_Unlock();

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = GetPage(addr);
    EraseInitStruct.NbPages     = (size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    EraseInitStruct.Banks       = bank;
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        result = -RT_ERROR;
        goto __exit;
    }

__exit:
    HAL_FLASH_Lock();

    if (result != RT_EOK)
    {
        return result;
    }

    LOG_D("erase done: addr (0x%p), size %d", (void *)addr, size);
    return size;
}

/**
 * Erase data on flash .
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase(rt_uint32_t addr, size_t size)
{
#if defined(FLASH_BANK2_END)    
    rt_err_t result = RT_EOK;   
    rt_uint32_t addr_bank1 = 0;
    rt_uint32_t size_bank1 = 0;
    rt_uint32_t addr_bank2 = 0;
    rt_uint32_t size_bank2 = 0;

    if((addr + size) <= FLASH_BANK1_END)
    {
        addr_bank1 = addr;
        size_bank1 = size;
        size_bank2 = 0;
    }
    else if(addr > FLASH_BANK1_END)
    {
        size_bank1 = 0;
        addr_bank2 = addr;
        size_bank2 = size; 
    }
    else
    {
        addr_bank1 = addr;
        size_bank1 = FLASH_BANK1_END + 1 - addr_bank1;
        addr_bank2 = FLASH_BANK1_END + 1;
        size_bank2 = addr + size - (FLASH_BANK1_END + 1);
    }

    if(size_bank1)
    {
        LOG_D("bank1: addr (0x%p), size %d", (void *)addr_bank1, size_bank1);
        if(size_bank1 != stm32_flash_erase_bank(FLASH_BANK_1, addr_bank1, size_bank1))
        {
            result = -RT_ERROR;
            goto __exit;
        }
    }
    
    if(size_bank2)
    {
        LOG_D("bank2: addr (0x%p), size %d", (void *)addr_bank2, size_bank2);
        if(size_bank2 != stm32_flash_erase_bank(FLASH_BANK_2, addr_bank2, size_bank2))
        {
            result = -RT_ERROR;
            goto __exit;
        }
    }

__exit:    
    if(result != RT_EOK)
    {
        return result;
    }
    
    return size_bank1 + size_bank2;
#else
    return stm32_flash_erase_bank(FLASH_BANK_1, addr, size);
#endif
}


#if defined(PKG_USING_FAL)

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size);
static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size);
static int fal_flash_erase(long offset, size_t size);

const struct fal_flash_dev stm32_onchip_flash = { "onchip_flash", STM32_FLASH_START_ADRESS, STM32_FLASH_SIZE, FLASH_PAGE_SIZE, {NULL, fal_flash_read, fal_flash_write, fal_flash_erase} };

static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size)
{
    return stm32_flash_read(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size)
{
    return stm32_flash_write(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_erase(long offset, size_t size)
{
    return stm32_flash_erase(stm32_onchip_flash.addr + offset, size);
}

#endif
#endif /* BSP_USING_ON_CHIP_FLASH */
