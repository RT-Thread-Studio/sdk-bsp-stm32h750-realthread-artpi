/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018.10.30     SummerGift   first version
 * 2019.03.05     whj4674672   add stm32h7
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <drv_common.h>

#include "string.h"
#include "stdlib.h"
#include "uart_config.h"

int rt_hw_usart_init(void);


#if defined(RT_SERIAL_USING_DMA)
#include "dma_config.h"
#endif


#if defined(SOC_SERIES_STM32H7)
#define UART_INSTANCE_CLEAR_FUNCTION        __HAL_UART_CLEAR_IT
#define UART_SET_TDR(__HANDLE__, __DATA__)  ((__HANDLE__)->Instance->TDR = (__DATA__))
#define UART_GET_RDR(__HANDLE__)            ((__HANDLE__)->Instance->RDR & 0xFF)
#endif

#ifdef RT_SERIAL_USING_DMA

#define UART_RX_DMA_IT_HT_FLAG              0x01
#define UART_RX_DMA_IT_TC_FLAG              0x02
#define UART_RX_DMA_IT_IDLE_FLAG            0x00

#define DMA_INSTANCE_TYPE                   DMA_Stream_TypeDef

struct dma_config {
    DMA_INSTANCE_TYPE *Instance;
    rt_uint32_t dma_rcc;
    IRQn_Type dma_irq;
    rt_uint32_t request;
};
#endif

/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;

#ifdef RT_SERIAL_USING_DMA
    struct dma_config *dma_rx;
    struct dma_config *dma_tx;
#endif
    const char *tx_pin_name;
    const char *rx_pin_name;
};

/* stm32 uart dirver class */
struct stm32_uart
{
    UART_HandleTypeDef handle;
    struct stm32_uart_config *config;

#ifdef RT_SERIAL_USING_DMA
    struct
    {
        DMA_HandleTypeDef handle;
        rt_size_t last_index;
    } dma_rx;
    struct
    {
        DMA_HandleTypeDef handle;
    } dma_tx;
#endif
    rt_uint16_t uart_dma_flag;
    struct rt_serial_device serial;
};

#endif  /* __DRV_USART_H__ */
