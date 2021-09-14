/*!
 * \file      loragw_board.c
 *
 * \brief     spi peripheral initlize,it depend on mcu platform.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */
#include "board.h"
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_SPI
#include "drv_spi.h"
#endif
#include "loragw_board.h"

#define LOG_TAG "lora.gw.board"
#include "loragw_dbg.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

/* for STM32 get GPIO fort,eg GPIOA */
#ifndef GET_GPIO_PORT
#define GET_GPIO_PORT(pin) (GPIO_TypeDef *)( GPIOA_BASE + (uint32_t) ( pin >> 4 ) * 0x0400UL )
#endif
#ifndef GET_GPIO_PIN
#define GET_GPIO_PIN(pin) (rt_uint16_t)( 1 << ( pin & 0x0F ) ) // for get GPIO_PIN, eg: GPIO_PIN_6
#endif
/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

extern struct rt_spi_device *spi_dev_sx1302; 
int lgw_spi_init(void)
{
    rt_err_t res;

    res = rt_hw_spi_device_attach(LORA_GW_DRIVER_SPI_BUS_NAME, LORA_GW_DRIVER_SPI_DEVICE_NAME, GET_GPIO_PORT(LORA_GW_DRIVER_SPI_NSS_PIN),GET_GPIO_PIN(LORA_GW_DRIVER_SPI_NSS_PIN));

    if (res != RT_EOK)
    {
        LGD_DEBUG_LOG(LGD_DBG_SPI, LOG_LVL_DBG,"rt_spi_bus_attach_device!\r\n");
        return res;
    }
    /* 190807 get spi handle */
    spi_dev_sx1302 = (struct rt_spi_device *) rt_device_find(LORA_GW_DRIVER_SPI_DEVICE_NAME);
    
    if (!spi_dev_sx1302)
    {
        LGD_DEBUG_LOG(LGD_DBG_SPI, LOG_LVL_DBG,"sx1302 spi run failed! can't find %s device!\n", LORA_GW_DRIVER_SPI_DEVICE_NAME);
    }
    else
    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;

        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        cfg.max_hz = 8 * 1000 * 1000; /* max 10M */

        res = rt_spi_configure(spi_dev_sx1302, &cfg);
        if (res != RT_EOK)
        {
            LGD_DEBUG_LOG(LGD_DBG_SPI, LOG_LVL_DBG,"rt_spi_configure failed!\r\n");
            return res;
        }

        /* take bus and init spi */
        res = rt_spi_take_bus(spi_dev_sx1302);
        if (res != RT_EOK)
        {
            LGD_DEBUG_LOG(LGD_DBG_SPI, LOG_LVL_DBG,"rt_spi_take_bus failed!\r\n");
            return res;
        }
        
        res = rt_spi_release_bus(spi_dev_sx1302);
        
        if(res != RT_EOK)
        {
            LGD_DEBUG_LOG(LGD_DBG_SPI, LOG_LVL_DBG,"rt_spi_release_bus failed!\r\n");
            return res;
        }
    }

    return RT_EOK;
}

void lgw_io_init(void)
{
    // POWER_ON pin for sx1250 power   
    rt_pin_mode(LORA_GW_DRIVER_POWER_ON_PIN, PIN_MODE_OUTPUT); 
    // 1 - power on
    rt_pin_write(LORA_GW_DRIVER_POWER_ON_PIN, 1); 
    rt_thread_mdelay(100);

    // GPIO6 pin  
    rt_pin_mode(LORA_GW_DRIVER_GPIO6_PIN, PIN_MODE_INPUT); 

    // PPS pin  
    rt_pin_mode(LORA_GW_DRIVER_PPS_PIN, PIN_MODE_INPUT); 
}

extern struct rt_i2c_bus_device *i2c_bus_sx1302_temperture;
int lgw_i2c_init(void)
{   
    i2c_bus_sx1302_temperture = rt_i2c_bus_device_find(LORA_GW_DRIVER_I2C_DEVICE_NAME);
    if (i2c_bus_sx1302_temperture == RT_NULL)
    {
        LGD_DEBUG_LOG(LGD_DBG_I2C, LOG_LVL_DBG,"Failed to find bus %s\n", LORA_GW_DRIVER_I2C_DEVICE_NAME);
        return -RT_ERROR;
    }
    LGD_DEBUG_LOG(LGD_DBG_I2C, LOG_LVL_DBG,"find bus %s OK\n", LORA_GW_DRIVER_I2C_DEVICE_NAME);
  
    return RT_EOK;
}


/* --- EOF ------------------------------------------------------------------ */
