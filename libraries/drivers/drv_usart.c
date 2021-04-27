/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2020-05-23     chenyaxing   modify stm32_uart_config
 * 2020-12-08     wanghaijing  support uart1 dma
 */

#include "board.h"
#include "drv_usart.h"

#ifdef RT_USING_SERIAL

//#define DRV_DEBUG
#define DBG_TAG              "drv.usart"
#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && !defined(BSP_USING_UART3) && \
    !defined(BSP_USING_UART4) && !defined(BSP_USING_UART5) && !defined(BSP_USING_UART6) && \
    !defined(BSP_USING_UART7) && !defined(BSP_USING_UART8) && !defined(BSP_USING_LPUART1)
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

#ifdef RT_SERIAL_USING_DMA
    static void stm32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag);
#endif

enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif

#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif

#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif

#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
};

static struct stm32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif

#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif

#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif

#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif

};

static rt_err_t stm32_uart_clk_enable(struct stm32_uart_config *config)
{
    /* uart clock enable */
    switch ((uint32_t)config->Instance)
    {
#ifdef BSP_USING_UART1
    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART1 */

#ifdef BSP_USING_UART2
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART2 */

#ifdef BSP_USING_UART3
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART3 */

#ifdef BSP_USING_UART4
    case (uint32_t)UART4:
        __HAL_RCC_UART4_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART4 */

    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t stm32_gpio_clk_enable(GPIO_TypeDef *gpiox)
{
    /* check the parameters */
    RT_ASSERT(IS_GPIO_ALL_INSTANCE(gpiox));

    /* gpio ports clock enable */
    switch ((uint32_t)gpiox)
    {
#if defined(__HAL_RCC_GPIOA_CLK_ENABLE)
    case (uint32_t)GPIOA:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOB_CLK_ENABLE)
    case (uint32_t)GPIOB:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOC_CLK_ENABLE)
    case (uint32_t)GPIOC:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOD_CLK_ENABLE)
    case (uint32_t)GPIOD:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOE_CLK_ENABLE)
    case (uint32_t)GPIOE:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOF_CLK_ENABLE)
    case (uint32_t)GPIOF:
        __HAL_RCC_GPIOF_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOG_CLK_ENABLE)
    case (uint32_t)GPIOG:
        __HAL_RCC_GPIOG_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOH_CLK_ENABLE)
    case (uint32_t)GPIOH:
        __HAL_RCC_GPIOH_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOI_CLK_ENABLE)
    case (uint32_t)GPIOI:
        __HAL_RCC_GPIOI_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOJ_CLK_ENABLE)
    case (uint32_t)GPIOJ:
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOK_CLK_ENABLE)
    case (uint32_t)GPIOK:
        __HAL_RCC_GPIOK_CLK_ENABLE();
        break;
#endif
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static int up_char(char * c)
{
    if ((*c >= 'a') && (*c <= 'z'))
    {
        *c = *c - 32;
    }
    return 0;
}

static void get_pin_by_name(const char* pin_name, GPIO_TypeDef **port, uint16_t *pin)
{
    int pin_num = atoi((char*) &pin_name[2]);
    char port_name = pin_name[1];
    up_char(&port_name);
    up_char(&port_name);
    *port = ((GPIO_TypeDef *) ((uint32_t) GPIOA
            + (uint32_t) (port_name - 'A') * ((uint32_t) GPIOB - (uint32_t) GPIOA)));
    *pin = (GPIO_PIN_0 << pin_num);
}
static uint16_t stm32_get_pin(GPIO_TypeDef *pin_port, rt_uint32_t pin_num)
{
    return (uint16_t)((16 * (((rt_base_t)pin_port - (rt_base_t)GPIOA_BASE)/(0x0400UL))) + (__rt_ffs(pin_num) - 1));
}
static rt_err_t stm32_gpio_configure(struct stm32_uart_config *config)
{
#define UART_IS_TX        (1U<<7)
#define UART_IS_RX        (0U)

    rt_uint16_t tx_pin_num = 0;
    int index = 0, tx_af_num = 0, rx_af_num = 0;
    uint8_t uart_num = 0;
    GPIO_TypeDef *tx_port;
    GPIO_TypeDef *rx_port;
    uint16_t tx_pin;
    uint16_t rx_pin;
    get_pin_by_name(config->rx_pin_name, &rx_port, &rx_pin);
    get_pin_by_name(config->tx_pin_name, &tx_port, &tx_pin);
    
    struct gpio_uart_af {
        /* index get by GET_PIN */
        uint16_t pin_index;
        struct {
            /* when is TX the bit7 is 1, bit6-bit0 is uart number (1-8)  */
            uint8_t uart_num;
            uint8_t af_num;
        } afs[1];
    };

    static const struct gpio_uart_af uart_afs[] =
    {
        { .pin_index = GET_PIN(A,  0), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
        { .pin_index = GET_PIN(A,  1), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
        { .pin_index = GET_PIN(A,  2), .afs[0] = {.uart_num = UART_IS_TX|2, .af_num =  7}},
        { .pin_index = GET_PIN(A,  3), .afs[0] = {.uart_num = UART_IS_RX|2, .af_num =  7}},
        { .pin_index = GET_PIN(A,  9), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  7}},
        { .pin_index = GET_PIN(A, 10), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  7}},
        { .pin_index = GET_PIN(A, 11), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  6}},
        { .pin_index = GET_PIN(A, 12), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  6}},
        { .pin_index = GET_PIN(A, 15), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num = 11}},
        { .pin_index = GET_PIN(A,  8), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num = 11}},
        { .pin_index = GET_PIN(B,  4), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num = 11}},
        { .pin_index = GET_PIN(B,  3), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num = 11}},
        { .pin_index = GET_PIN(B,  5), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num = 14}},
        { .pin_index = GET_PIN(B,  6), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num = 14}},
        { .pin_index = GET_PIN(B,  6), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  7}},
        { .pin_index = GET_PIN(B,  7), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  7}},
        { .pin_index = GET_PIN(B,  9), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
        { .pin_index = GET_PIN(B,  8), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
        { .pin_index = GET_PIN(B, 10), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
        { .pin_index = GET_PIN(B, 11), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
        { .pin_index = GET_PIN(B, 13), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num = 14}},
        { .pin_index = GET_PIN(B, 12), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num = 14}},
        { .pin_index = GET_PIN(B, 14), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  4}},
        { .pin_index = GET_PIN(B, 15), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  4}},
        { .pin_index = GET_PIN(C,  6), .afs[0] = {.uart_num = UART_IS_TX|6, .af_num =  7}},
        { .pin_index = GET_PIN(C,  7), .afs[0] = {.uart_num = UART_IS_RX|6, .af_num =  7}},
        { .pin_index = GET_PIN(C, 10), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
        { .pin_index = GET_PIN(C, 11), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
        { .pin_index = GET_PIN(C, 10), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
        { .pin_index = GET_PIN(C, 11), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
        { .pin_index = GET_PIN(C, 12), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num =  8}},
        { .pin_index = GET_PIN(D,  2), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num =  8}},
        { .pin_index = GET_PIN(D,  1), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
        { .pin_index = GET_PIN(D,  0), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
        { .pin_index = GET_PIN(D,  5), .afs[0] = {.uart_num = UART_IS_TX|2, .af_num =  7}},
        { .pin_index = GET_PIN(D,  6), .afs[0] = {.uart_num = UART_IS_RX|2, .af_num =  7}},
        { .pin_index = GET_PIN(D,  8), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
        { .pin_index = GET_PIN(D,  9), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
        { .pin_index = GET_PIN(E,  1), .afs[0] = {.uart_num = UART_IS_TX|8, .af_num =  8}},
        { .pin_index = GET_PIN(E,  0), .afs[0] = {.uart_num = UART_IS_RX|8, .af_num =  8}},
        { .pin_index = GET_PIN(E,  8), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num =  7}},
        { .pin_index = GET_PIN(E,  7), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num =  7}},
        { .pin_index = GET_PIN(F,  7), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num =  7}},
        { .pin_index = GET_PIN(F,  6), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num =  7}},
        { .pin_index = GET_PIN(G, 14), .afs[0] = {.uart_num = UART_IS_TX|6, .af_num =  7}},
        { .pin_index = GET_PIN(G,  9), .afs[0] = {.uart_num = UART_IS_RX|6, .af_num =  7}},
        { .pin_index = GET_PIN(H, 13), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
        { .pin_index = GET_PIN(H, 14), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
        { .pin_index = GET_PIN(G,  8), .afs[0] = {.uart_num = UART_IS_TX|8, .af_num =  8}},
        { .pin_index = GET_PIN(G,  9), .afs[0] = {.uart_num = UART_IS_RX|8, .af_num =  8}},
    };

   /* get tx/rx pin index */
   uart_num = config->name[4] - '0';
   tx_pin_num = stm32_get_pin(tx_port, tx_pin);

   for (index = 0; index < sizeof(uart_afs) / sizeof(struct gpio_uart_af); index = index + 2)
   {
       if (uart_afs[index].pin_index == tx_pin_num)
       {
           if(uart_afs[index].afs[0].uart_num == (uart_num|UART_IS_TX))
           {
               tx_af_num = uart_afs[index].afs[0].af_num;
               rx_af_num = uart_afs[index + 1].afs[0].af_num;
           }
       }
   }

   /* gpio Init */
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* gpio ports clock enable */
   stm32_gpio_clk_enable(tx_port);
   if (tx_port != rx_port)
   {
       stm32_gpio_clk_enable(rx_port);
   }

    /* rx pin initialize */
    GPIO_InitStruct.Pin = tx_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#if defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7)
    GPIO_InitStruct.Alternate = tx_af_num;
#endif
    HAL_GPIO_Init(tx_port, &GPIO_InitStruct);

    /* rx pin initialize */
    GPIO_InitStruct.Pin = rx_pin;
#if defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7)
    GPIO_InitStruct.Alternate = rx_af_num;
#endif
    HAL_GPIO_Init(rx_port, &GPIO_InitStruct);

    return RT_EOK;
}


static struct stm32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

static rt_err_t stm32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);

    /* uart clock enable */
    stm32_uart_clk_enable(uart->config);
    /* uart gpio clock enable and gpio pin init */
    stm32_gpio_configure(uart->config);

    uart->handle.Instance          = uart->config->Instance;
    uart->handle.Init.BaudRate     = cfg->baud_rate;
    uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart->handle.Init.Mode         = UART_MODE_TX_RX;
    uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(uart->handle.Instance == USART3)
    {
        uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_RTS_CTS;
    }

    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    case DATA_BITS_9:
        uart->handle.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    default:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }

    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    case STOP_BITS_2:
        uart->handle.Init.StopBits   = UART_STOPBITS_2;
        break;
    default:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    }

    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    case PARITY_ODD:
        uart->handle.Init.Parity     = UART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        uart->handle.Init.Parity     = UART_PARITY_EVEN;
        break;
    default:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    }

    if (HAL_UART_Init(&uart->handle) != HAL_OK)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t stm32_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct stm32_uart *uart;

    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    if(ctrl_arg & (RT_DEVICE_FLAG_RX_BLOCKING | RT_DEVICE_FLAG_RX_NON_BLOCKING))
    {
        if (uart->uart_dma_flag & RT_DEVICE_FLAG_DMA_RX)
            ctrl_arg = RT_DEVICE_FLAG_DMA_RX;
        else
            ctrl_arg = RT_DEVICE_FLAG_INT_RX;
    }
    else if(ctrl_arg & (RT_DEVICE_FLAG_TX_BLOCKING | RT_DEVICE_FLAG_TX_NON_BLOCKING))
    {
        if (uart->uart_dma_flag & RT_DEVICE_FLAG_DMA_TX)
            ctrl_arg = RT_DEVICE_FLAG_DMA_TX;
        else
            ctrl_arg = RT_DEVICE_FLAG_INT_TX;
    }

    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:

        NVIC_DisableIRQ(uart->config->irq_type);
        if (ctrl_arg == RT_DEVICE_FLAG_INT_RX)
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);
        else if (ctrl_arg == RT_DEVICE_FLAG_INT_TX)
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_TXE);
#ifdef RT_SERIAL_USING_DMA
        else if (ctrl_arg == RT_DEVICE_FLAG_DMA_RX)
        {
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);

            HAL_NVIC_DisableIRQ(uart->config->dma_rx->dma_irq);
            if (HAL_DMA_Abort(&(uart->dma_rx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }

            if (HAL_DMA_DeInit(&(uart->dma_rx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }
        }
        else if(ctrl_arg == RT_DEVICE_FLAG_DMA_TX)
        {
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_TC);

            HAL_NVIC_DisableIRQ(uart->config->dma_tx->dma_irq);
            if (HAL_DMA_DeInit(&(uart->dma_tx.handle)) != HAL_OK)
            {
                RT_ASSERT(0);
            }
        }
#endif
        break;

    case RT_DEVICE_CTRL_SET_INT:

        HAL_NVIC_SetPriority(uart->config->irq_type, 1, 0);
        HAL_NVIC_EnableIRQ(uart->config->irq_type);

        if (ctrl_arg == RT_DEVICE_FLAG_INT_RX)
            __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_RXNE);
        else if (ctrl_arg == RT_DEVICE_FLAG_INT_TX)
            __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_TXE);
        break;

    case RT_DEVICE_CTRL_CONFIG:
        if (ctrl_arg & (RT_DEVICE_FLAG_DMA_RX | RT_DEVICE_FLAG_DMA_TX))
            stm32_dma_config(serial, ctrl_arg);
        else
            stm32_control(serial, RT_DEVICE_CTRL_SET_INT, (void *)ctrl_arg);
        break;

    case RT_DEVICE_CHECK_OPTMODE:
        {
            if (ctrl_arg & RT_DEVICE_FLAG_DMA_TX)
                return RT_SERIAL_TX_BLOCKING_NO_BUFFER;
            else
                return RT_SERIAL_TX_BLOCKING_BUFFER;
        }
    case RT_DEVICE_CTRL_CLOSE:
        if (HAL_UART_DeInit(&(uart->handle)) != HAL_OK )
        {
            RT_ASSERT(0)
        }
        break;

    }
    return RT_EOK;
}

static int stm32_putc(struct rt_serial_device *serial, char c)
{
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);
    while (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) == RESET);
    UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
    UART_SET_TDR(&uart->handle, c);

    return 1;
}

static int stm32_getc(struct rt_serial_device *serial)
{
    int ch;
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    ch = -1;
    if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
        ch = UART_GET_RDR(&uart->handle);
    return ch;
}

static rt_size_t stm32_transmit(struct rt_serial_device     *serial,
                                       rt_uint8_t           *buf,
                                       rt_size_t             size,
                                       rt_uint32_t           tx_flag)
{
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(buf != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    if (uart->uart_dma_flag & RT_DEVICE_FLAG_DMA_TX)
    {
        HAL_UART_Transmit_DMA(&uart->handle, buf, size);

        if (tx_flag == RT_DEVICE_FLAG_TX_NON_BLOCKING)
        {
            struct rt_serial_tx_fifo *tx_fifo;
            tx_fifo = (struct rt_serial_tx_fifo *)serial->serial_tx;
            RT_ASSERT(tx_fifo != RT_NULL);
            tx_fifo->put_size -= size;
        }
        return size;
    }

    stm32_control(serial, RT_DEVICE_CTRL_SET_INT, (void *)tx_flag);

    return size;
}

#ifdef RT_SERIAL_USING_DMA
static void dma_recv_isr(struct rt_serial_device *serial, rt_uint8_t isr_flag)
{
    struct stm32_uart *uart;
    rt_base_t level;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    struct rt_serial_rx_fifo *rx_fifo;
    rx_fifo = (struct rt_serial_rx_fifo *) serial->serial_rx;
    RT_ASSERT(rx_fifo != RT_NULL);

    rt_uint16_t recv_len = 0;

    level = rt_hw_interrupt_disable();
    rt_uint16_t index = __HAL_DMA_GET_COUNTER(&(uart->dma_rx.handle));
    switch (isr_flag)
    {
    case UART_RX_DMA_IT_HT_FLAG:
        if(index < rx_fifo->rx_index)
            recv_len = rx_fifo->rx_index - index;
        break;

    case UART_RX_DMA_IT_TC_FLAG:
        if(index >= rx_fifo->rx_index)
            recv_len = serial->config.rx_bufsz + rx_fifo->rx_index - index;
        break;

    case UART_RX_DMA_IT_IDLE_FLAG:
        if(index < rx_fifo->rx_index)
            recv_len = rx_fifo->rx_index - index;
        break;

    default:
        break;
    }

    rx_fifo->rx_index = index;
    SCB_InvalidateDCache();
    if(rt_ringbuffer_put_update(&(rx_fifo->rb), recv_len) != recv_len)
    {
        /*
            Increase your code.
        */
    }
    rt_hw_interrupt_enable(level);

}
#endif  /* RT_SERIAL_USING_DMA */


/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);
    //If the Read data register is not empty and the RXNE interrupt is enabled  （RDR）
    if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_RXNE) != RESET))
    {
        struct rt_serial_rx_fifo *rx_fifo;
        rx_fifo = (struct rt_serial_rx_fifo *) serial->serial_rx;
        RT_ASSERT(rx_fifo != RT_NULL);

        rt_ringbuffer_putchar(&(rx_fifo->rb), UART_GET_RDR(&uart->handle));

        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }
    //If the Transmit data register is empty and the TXE interrupt enable is enabled  （TDR）
    else if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TXE) != RESET) &&
                (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_TXE)) != RESET)
    {
        struct rt_serial_tx_fifo *tx_fifo;
        tx_fifo = (struct rt_serial_tx_fifo *) serial->serial_tx;
        RT_ASSERT(tx_fifo != RT_NULL);

        if (tx_fifo->put_size)
        {
            rt_uint8_t put_char = 0;
            rt_ringbuffer_getchar(&(tx_fifo->rb), &put_char);
            UART_SET_TDR(&uart->handle, put_char);
            tx_fifo->put_size --;
        }
        else
        {
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_TC);
        }
    }
    else if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_TC) != RESET))
    {
        if (uart->uart_dma_flag & RT_DEVICE_FLAG_DMA_TX)
        {
            // The HAL_UART_TxCpltCallback will be triggered
            HAL_UART_IRQHandler(&(uart->handle));
        }
        else
        {
            //Transmission complete interrupt disable ( CR1 Register)
            __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_TC);
            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DONE);
        }
        // Clear Transmission complete interrupt flag ( ISR Register )
        UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
    }

#ifdef RT_SERIAL_USING_DMA
    else if ((uart->uart_dma_flag) && (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_IDLE) != RESET)
             && (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_IDLE) != RESET))
    {
        dma_recv_isr(serial, UART_RX_DMA_IT_IDLE_FLAG);
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
        __HAL_UART_CLEAR_IDLEFLAG(&uart->handle);
    }
#endif
    else
    {
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_NE) != RESET)
        {
            __HAL_UART_CLEAR_NEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_FE) != RESET)
        {
            __HAL_UART_CLEAR_FEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_PE) != RESET)
        {
            __HAL_UART_CLEAR_PEFLAG(&uart->handle);
        }
#if !defined(SOC_SERIES_STM32L4) && !defined(SOC_SERIES_STM32F7) && !defined(SOC_SERIES_STM32F0) \
    && !defined(SOC_SERIES_STM32L0) && !defined(SOC_SERIES_STM32G0) && !defined(SOC_SERIES_STM32H7) \
    && !defined(SOC_SERIES_STM32G4)
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_LBD) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_LBD);
        }
#endif
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_CTS) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_CTS);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TXE) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TXE);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_RXNE);
        }
    }
}

#if defined(BSP_USING_UART1)
void USART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART1_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA)
void UART1_DMA_RX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_rx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA)
void UART1_DMA_TX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_tx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA) */
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
void USART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART2_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA)
void UART2_DMA_RX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_rx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA)
void UART2_DMA_TX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_tx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA) */
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
void USART3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART3_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_RX_USING_DMA)
void UART3_DMA_RX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART3_INDEX].dma_rx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(BSP_UART_USING_DMA_RX) && defined(BSP_UART3_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_TX_USING_DMA)
void UART3_DMA_TX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART3_INDEX].dma_tx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(BSP_UART_USING_DMA_TX) && defined(BSP_UART3_TX_USING_DMA) */
#endif /* BSP_USING_UART3*/

#if defined(BSP_USING_UART4)
void UART4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART4_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_RX_USING_DMA)
void UART4_DMA_RX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART4_INDEX].dma_rx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_TX_USING_DMA)
void UART4_DMA_TX_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&uart_obj[UART4_INDEX].dma_tx.handle);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_TX_USING_DMA) */
#endif /* BSP_USING_UART4 */

static void stm32_uart_get_config(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
#ifdef BSP_USING_UART1
    uart_obj[UART1_INDEX].serial.config = config;
    uart_obj[UART1_INDEX].uart_dma_flag = 0;

    uart_obj[UART1_INDEX].serial.config.rx_bufsz = BSP_UART1_RX_BUFSIZE;
    uart_obj[UART1_INDEX].serial.config.tx_bufsz = BSP_UART1_TX_BUFSIZE;

#ifdef BSP_UART1_RX_USING_DMA
    uart_obj[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart1_dma_rx = UART1_DMA_RX_CONFIG;
    uart_config[UART1_INDEX].dma_rx = &uart1_dma_rx;
#endif

#ifdef BSP_UART1_TX_USING_DMA
    uart_obj[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart1_dma_tx = UART1_DMA_TX_CONFIG;
    uart_config[UART1_INDEX].dma_tx = &uart1_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART2
    uart_obj[UART2_INDEX].serial.config = config;
    uart_obj[UART2_INDEX].uart_dma_flag = 0;

    uart_obj[UART2_INDEX].serial.config.rx_bufsz = BSP_UART2_RX_BUFSIZE;
    uart_obj[UART2_INDEX].serial.config.tx_bufsz = BSP_UART2_TX_BUFSIZE;

#ifdef BSP_UART2_RX_USING_DMA
    uart_obj[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart2_dma_rx = UART2_DMA_RX_CONFIG;
    uart_config[UART2_INDEX].dma_rx = &uart2_dma_rx;
#endif

#ifdef BSP_UART2_TX_USING_DMA
    uart_obj[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart2_dma_tx = UART2_DMA_TX_CONFIG;
    uart_config[UART2_INDEX].dma_tx = &uart2_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART3
    uart_obj[UART3_INDEX].serial.config = config;
    uart_obj[UART3_INDEX].uart_dma_flag = 0;

    uart_obj[UART3_INDEX].serial.config.rx_bufsz = BSP_UART3_RX_BUFSIZE;
    uart_obj[UART3_INDEX].serial.config.tx_bufsz = BSP_UART3_TX_BUFSIZE;

#ifdef BSP_UART3_RX_USING_DMA
    uart_obj[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart3_dma_rx = UART3_DMA_RX_CONFIG;
    uart_config[UART3_INDEX].dma_rx = &uart3_dma_rx;
#endif

#ifdef BSP_UART3_TX_USING_DMA
    uart_obj[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart3_dma_tx = UART3_DMA_TX_CONFIG;
    uart_config[UART3_INDEX].dma_tx = &uart3_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART4
    uart_obj[UART4_INDEX].serial.config = config;
    uart_obj[UART4_INDEX].uart_dma_flag = 0;

    uart_obj[UART4_INDEX].serial.config.rx_bufsz = BSP_UART4_RX_BUFSIZE;
    uart_obj[UART4_INDEX].serial.config.tx_bufsz = BSP_UART4_TX_BUFSIZE;

#ifdef BSP_UART4_RX_USING_DMA
    uart_obj[UART4_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart4_dma_rx = UART4_DMA_RX_CONFIG;
    uart_config[UART4_INDEX].dma_rx = &uart4_dma_rx;
#endif

#ifdef BSP_UART4_TX_USING_DMA
    uart_obj[UART4_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart4_dma_tx = UART4_DMA_TX_CONFIG;
    uart_config[UART4_INDEX].dma_tx = &uart4_dma_tx;
#endif
#endif
}

#ifdef RT_SERIAL_USING_DMA
static void stm32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag)
{
    struct rt_serial_rx_fifo *rx_fifo;
    DMA_HandleTypeDef *DMA_Handle;
    struct dma_config *dma_config;
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        DMA_Handle = &uart->dma_rx.handle;
        dma_config = uart->config->dma_rx;
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        DMA_Handle = &uart->dma_tx.handle;
        dma_config = uart->config->dma_tx;
    }
    LOG_D("%s dma config start", uart->config->name);

    {
        rt_uint32_t tmpreg = 0x00U;

#if defined(SOC_SERIES_STM32H7)
        /* enable DMA clock && Delay after an RCC peripheral clock enabling*/
        SET_BIT(RCC->AHB1ENR, dma_config->dma_rcc);
        tmpreg = READ_BIT(RCC->AHB1ENR, dma_config->dma_rcc);
#endif
        UNUSED(tmpreg);   /* To avoid compiler warnings */
    }

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmarx, uart->dma_rx.handle);
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmatx, uart->dma_tx.handle);
    }

#if defined(SOC_SERIES_STM32H7)
    DMA_Handle->Instance                 = dma_config->Instance;
    DMA_Handle->Init.Request             = dma_config->request;
#endif
    DMA_Handle->Init.PeriphInc           = DMA_PINC_DISABLE;
    DMA_Handle->Init.MemInc              = DMA_MINC_ENABLE;
    DMA_Handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_Handle->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_PERIPH_TO_MEMORY;
        DMA_Handle->Init.Mode                = DMA_CIRCULAR;
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_MEMORY_TO_PERIPH;
        DMA_Handle->Init.Mode                = DMA_NORMAL;
    }

    DMA_Handle->Init.Priority            = DMA_PRIORITY_MEDIUM;
#if defined(SOC_SERIES_STM32H7)
    DMA_Handle->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
#endif
    if (HAL_DMA_DeInit(DMA_Handle) != HAL_OK)
    {
        RT_ASSERT(0);
    }

    if (HAL_DMA_Init(DMA_Handle) != HAL_OK)
    {
        RT_ASSERT(0);
    }

    /* enable interrupt */
    if (flag == RT_DEVICE_FLAG_DMA_RX)
    {
        rx_fifo = (struct rt_serial_rx_fifo *)serial->serial_rx;
        /* Start DMA transfer */
        if (HAL_UART_Receive_DMA(&(uart->handle), rx_fifo->buffer, serial->config.rx_bufsz) != HAL_OK)
        {
            /* Transfer error in reception process */
            RT_ASSERT(0);
        }
        CLEAR_BIT(uart->handle.Instance->CR3, USART_CR3_EIE);
        __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_IDLE);
    }

    /* DMA irq should set in DMA TX mode, or HAL_UART_TxCpltCallback function will not be called */
    HAL_NVIC_SetPriority(dma_config->dma_irq, 0, 0);
    HAL_NVIC_EnableIRQ(dma_config->dma_irq);

    HAL_NVIC_SetPriority(uart->config->irq_type, 1, 0);
    HAL_NVIC_EnableIRQ(uart->config->irq_type);

    LOG_D("%s dma %s instance: %x", uart->config->name, flag == RT_DEVICE_FLAG_DMA_RX ? "RX" : "TX", DMA_Handle->Instance);
    LOG_D("%s dma config done", uart->config->name);
}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    RT_ASSERT(huart != NULL);
    struct stm32_uart *uart = (struct stm32_uart *)huart;
    LOG_D("%s: %s %d\n", __FUNCTION__, uart->config->name, huart->ErrorCode);
    UNUSED(uart);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    RT_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;

    dma_recv_isr(&uart->serial, UART_RX_DMA_IT_TC_FLAG);
}

/**
  * @brief  Rx Half transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx Half transfer,
  *         and you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    RT_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;

    dma_recv_isr(&uart->serial, UART_RX_DMA_IT_HT_FLAG);
}

/**
  * @brief  HAL_UART_TxCpltCallback
  * @param  huart: UART handle
  * @note   This callback can be called by two functions, first in UART_EndTransmit_IT when 
  *         UART Tx complete and second in UART_DMATransmitCplt function in DMA Circular mode.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    struct rt_serial_device *serial;
    rt_size_t trans_total_index;
    rt_base_t level;

    RT_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;
    serial = &uart->serial;
    RT_ASSERT(serial != RT_NULL);

    level = rt_hw_interrupt_disable();
    trans_total_index = __HAL_DMA_GET_COUNTER(&(uart->dma_tx.handle));
    rt_hw_interrupt_enable(level);

    if (trans_total_index) return;

    if (serial->parent.open_flag & RT_DEVICE_FLAG_TX_NON_BLOCKING)
    {
        struct rt_serial_tx_fifo *tx_fifo;
        tx_fifo = (struct rt_serial_tx_fifo *)serial->serial_tx;
        RT_ASSERT(tx_fifo != RT_NULL);
        rt_uint8_t *put_ptr = RT_NULL;
        rt_uint8_t data_len = 0, put_size = 0;

        level = rt_hw_interrupt_disable();
        data_len = rt_ringbuffer_data_len(&(tx_fifo->rb));
        put_size = data_len - tx_fifo->put_size;
        rt_ringbuffer_peak(&(tx_fifo->rb), &put_ptr, put_size);
        rt_hw_interrupt_enable(level);
    }
    rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DONE);

}
#endif  /* RT_SERIAL_USING_DMA */

static const struct rt_uart_ops stm32_uart_ops =
{
    .configure = stm32_configure,
    .control = stm32_control,
    .putc = stm32_putc,
    .getc = stm32_getc,
    .transmit = stm32_transmit
};

int rt_hw_usart_init(void)
{
    rt_err_t result = 0;
    rt_size_t obj_num = sizeof(uart_obj) / sizeof(struct stm32_uart);

    stm32_uart_get_config();
    for (int i = 0; i < obj_num; i++)
    {
        /* init UART object */
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops = &stm32_uart_ops;
        /* register UART device */
        result = rt_hw_serial_register(&uart_obj[i].serial,
                                        uart_obj[i].config->name,
                                        RT_DEVICE_FLAG_RDWR,
                                        NULL);
        RT_ASSERT(result == RT_EOK);
    }

    return result;
}

#endif /* RT_USING_SERIAL */
