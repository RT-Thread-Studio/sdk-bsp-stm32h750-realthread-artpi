/**
  ******************************************************************************
  * @file    stm32l4xx_rcc.h
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
  * @brief   This file contains all the functions prototypes for the RCC firmware library.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4xx_RCC_H
#define __STM32L4xx_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"

/** @addtogroup STM32L4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t SYSCLK_Frequency; 
  uint32_t HCLK_Frequency;  
  uint32_t PCLK1_Frequency; 
  uint32_t PCLK2_Frequency; 
  uint32_t I2C1CLK_Frequency;  
  uint32_t I2C2CLK_Frequency;
  uint32_t I2C3CLK_Frequency;
  uint32_t USART1CLK_Frequency;
  uint32_t USART2CLK_Frequency;
  uint32_t USART3CLK_Frequency;
  uint32_t UART4CLK_Frequency;
  uint32_t UART5CLK_Frequency;
  uint32_t LPUARTCLK_Frequency;   
}RCC_ClocksTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup RCC_Exported_Constants
  * @{
  */
  
/** @defgroup RCC_HSE_configuration 
  * @{
  */
#define RCC_HSE_OFF                      ((uint8_t)0x00)
#define RCC_HSE_ON                       ((uint8_t)0x01)
#define RCC_HSE_Bypass                   ((uint8_t)0x05)
#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_Bypass))
/**
  * @}
  */ 
  
/** @defgroup RCC_PLL_Clock_Source 
  * @{
  */
#define RCC_PLLSource_MSI                ((uint32_t)0x00000001)
#define RCC_PLLSource_HSI                ((uint32_t)0x00000002)
#define RCC_PLLSource_HSE                ((uint32_t)0x00000003)


#define IS_RCC_PLL_SOURCE(SOURCE) (((SOURCE) == RCC_PLLSource_MSI) || \
                                   ((SOURCE) == RCC_PLLSource_HSI) || \
                                   ((SOURCE) == RCC_PLLSource_HSE))
    
/**
  * @}
  */ 

/** @defgroup RCC_MSI_Clock_Range 
  * @{
  */

#define RCC_MSIRange_0                   RCC_CR_MSIRANGE_0 /*!< MSI = 100 KHz  */
#define RCC_MSIRange_1                   RCC_CR_MSIRANGE_1 /*!< MSI = 200 KHz  */
#define RCC_MSIRange_2                   RCC_CR_MSIRANGE_2 /*!< MSI = 400 KHz  */
#define RCC_MSIRange_3                   RCC_CR_MSIRANGE_3 /*!< MSI = 800 KHz  */
#define RCC_MSIRange_4                   RCC_CR_MSIRANGE_4 /*!< MSI = 1 MHz    */
#define RCC_MSIRange_5                   RCC_CR_MSIRANGE_5 /*!< MSI = 2 MHz    */
#define RCC_MSIRange_6                   RCC_CR_MSIRANGE_6 /*!< MSI = 4 MHz    */
#define RCC_MSIRange_7                   RCC_CR_MSIRANGE_7 /*!< MSI = 8 MHz    */
#define RCC_MSIRange_8                   RCC_CR_MSIRANGE_8 /*!< MSI = 16 MHz   */
#define RCC_MSIRange_9                   RCC_CR_MSIRANGE_9 /*!< MSI = 24 MHz   */
#define RCC_MSIRange_10                  RCC_CR_MSIRANGE_10 /*!< MSI = 32 MHz  */
#define RCC_MSIRange_11                  RCC_CR_MSIRANGE_11 /*!< MSI = 48 MHz  */



#define IS_RCC_MSI_CLOCK_RANGE(RANGE) (((RANGE) == RCC_MSIRange_0) || \
                                       ((RANGE) == RCC_MSIRange_1) || \
                                       ((RANGE) == RCC_MSIRange_2) || \
                                       ((RANGE) == RCC_MSIRange_3) || \
                                       ((RANGE) == RCC_MSIRange_4) || \
                                       ((RANGE) == RCC_MSIRange_5) || \
                                       ((RANGE) == RCC_MSIRange_6) || \
                                       ((RANGE) == RCC_MSIRange_7) || \
                                       ((RANGE) == RCC_MSIRange_8) || \
                                       ((RANGE) == RCC_MSIRange_9) || \
                                       ((RANGE) == RCC_MSIRange_10) || \
                                       ((RANGE) == RCC_MSIRange_11))


/**
  * @}
  */ 

/** @defgroup RCC_PLL_Clock_Factors 
  * @{
  */
#define IS_RCC_PLLM_VALUE(VALUE) ((VALUE) <= 8)
#define IS_RCC_PLLN_VALUE(VALUE) ((8 <= (VALUE)) && ((VALUE) <= 86))
#define IS_RCC_PLLP_VALUE(VALUE) (((VALUE) == 7) || ((VALUE) == 17) )
#define IS_RCC_PLLQ_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                 ((VALUE) == 6)  || ((VALUE) == 8))
#define IS_RCC_PLLR_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                 ((VALUE) == 6)  || ((VALUE) == 8))
/**
  * @}
  */ 


/** @defgroup RCC_PLL_Clock_Output 
  * @{
  */
#define RCC_PLLCLk_P             ((uint32_t)0x00000000)
#define RCC_PLLCLk_Q             ((uint32_t)0x00000010)
#define RCC_PLLCLk_R             ((uint32_t)0x00000020)

#define IS_RCC_PLLCLOCKOUT_VALUE(VALUE) (((VALUE) == RCC_PLLCLk_P ) || ((VALUE) == RCC_PLLCLk_R) || \
                                             ((VALUE) == RCC_PLLCLk_Q))

/**                                                                     
  * @}
  */ 


/** @defgroup RCC_PLLSAI1_Clock_Factors 
  * @{
  */
#define IS_RCC_PLLSAI1N_VALUE(VALUE) ((8 <= (VALUE)) && ((VALUE) <= 86))
#define IS_RCC_PLLSAI1P_VALUE(VALUE) (((VALUE) == 7) || ((VALUE) == 17) )
#define IS_RCC_PLLSAI1Q_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                     ((VALUE) == 6)  || ((VALUE) == 8))
#define IS_RCC_PLLSAI1R_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                     ((VALUE) == 6)  || ((VALUE) == 8))

/**                                                                     
  * @}
  */ 
  
/** @defgroup RCC_PLLSAI1_Clock_Output 
  * @{
  */
#define RCC_PLLSAI1CLk_P             ((uint32_t)0x00000000)
#define RCC_PLLSAI1CLk_Q             ((uint32_t)0x00000010)
#define RCC_PLLSAI1CLk_R             ((uint32_t)0x00000020)

#define IS_RCC_PLLSAI1CLOCKOUT_VALUE(VALUE) (((VALUE) == RCC_PLLSAI1CLk_P ) || ((VALUE) == RCC_PLLSAI1CLk_R) || \
                                                 ((VALUE) == RCC_PLLSAI1CLk_Q))

/**                                                                     
  * @}
  */ 
/** @defgroup RCC_PLLSAI2_Clock_Factors 
  * @{
  */
#define IS_RCC_PLLSAI2N_VALUE(VALUE) ((8 <= (VALUE)) && ((VALUE) <= 86))
#define IS_RCC_PLLSAI2P_VALUE(VALUE) (((VALUE) == 7) || ((VALUE) == 17) )
#define IS_RCC_PLLSAI2Q_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                     ((VALUE) == 6)  || ((VALUE) == 8))
#define IS_RCC_PLLSAI2R_VALUE(VALUE) (((VALUE) == 2 ) || ((VALUE) == 4) || \
                                     ((VALUE) == 6)  || ((VALUE) == 8))


/**                                                                     
  * @}
  */ 

/** @defgroup RCC_PLLSAI2_Clock_Output 
  * @{
  */
#define RCC_PLLSAI2CLk_P             ((uint32_t)0x00000000)
#define RCC_PLLSAI2CLk_Q             ((uint32_t)0x00000010)
#define RCC_PLLSAI2CLk_R             ((uint32_t)0x00000020)


#define IS_RCC_PLLSAI2CLOCKOUT_VALUE(VALUE) (((VALUE) == RCC_PLLSAI2CLk_P ) || ((VALUE) == RCC_PLLSAI2CLk_R) || \
                                                 ((VALUE) == RCC_PLLSAI2CLk_Q))

/**                                                                     
  * @}
  */ 

/** @defgroup RCC_STOPWUP_Clock_Source 
  * @{
  */
#define RCC_STOPWUCK_MSI             ((uint32_t)0x00000000)
#define RCC_STOPWUCK_HSI             ((uint32_t)0x00000001)


#define IS_RCC_STOPWUP_VALUE(VALUE) (((VALUE) == RCC_STOPWUCK_MSI ) || ((VALUE) == RCC_STOPWUCK_HSI))
                                                

/**                                                                     
  * @}
  */ 
/** @defgroup RCC_System_Clock_Source 
  * @{
  */
#define RCC_SYSCLKSource_MSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSource_HSI             ((uint32_t)0x00000001)
#define RCC_SYSCLKSource_HSE             ((uint32_t)0x00000002)
#define RCC_SYSCLKSource_PLLCLK          ((uint32_t)0x00000003)
#define IS_RCC_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSource_MSI) || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSI) || \
                                      ((SOURCE) == RCC_SYSCLKSource_HSE) || \
                                      ((SOURCE) == RCC_SYSCLKSource_PLLCLK))
/**
  * @}
  */ 
  
/** @defgroup RCC_AHB_Clock_Source
  * @{
  */
#define RCC_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x00000080)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x00000090)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x000000A0)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div128                ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div256                ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div512                ((uint32_t)0x000000F0)
#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2) || \
                           ((HCLK) == RCC_SYSCLK_Div4) || ((HCLK) == RCC_SYSCLK_Div8) || \
                           ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                           ((HCLK) == RCC_SYSCLK_Div128) || ((HCLK) == RCC_SYSCLK_Div256) || \
                           ((HCLK) == RCC_SYSCLK_Div512))
/**
  * @}
  */ 
  

/** @defgroup RCC_I2C_clock_source 
  * @{
  */

#define RCC_I2C1CLK_PCLK                    ((uint32_t)0x10000000)
#define RCC_I2C1CLK_SYSCLK                  ((uint32_t)0x10001000)
#define RCC_I2C1CLK_HSI                     ((uint32_t)0x10002000)

#define RCC_I2C2CLK_PCLK                    ((uint32_t)0x20000000)
#define RCC_I2C2CLK_SYSCLK                  ((uint32_t)0x20004000)
#define RCC_I2C2CLK_HSI                     ((uint32_t)0x20008000)

#define RCC_I2C3CLK_PCLK                    ((uint32_t)0x30000000)
#define RCC_I2C3CLK_SYSCLK                  ((uint32_t)0x30010000)
#define RCC_I2C3CLK_HSI                     ((uint32_t)0x30020000)

#define IS_RCC_I2CCLK(I2CCLK) (((I2CCLK) == RCC_I2C1CLK_PCLK)   || ((I2CCLK) == RCC_I2C1CLK_SYSCLK) || \
                               ((I2CCLK) == RCC_I2C1CLK_HSI)    || ((I2CCLK) == RCC_I2C2CLK_PCLK) || \
                               ((I2CCLK) == RCC_I2C2CLK_SYSCLK) || ((I2CCLK) == RCC_I2C2CLK_HSI) || \
                               ((I2CCLK) == RCC_I2C3CLK_PCLK)   || ((I2CCLK) == RCC_I2C3CLK_SYSCLK) || \
                               ((I2CCLK) == RCC_I2C3CLK_HSI))

/**
  * @}
  */

/** @defgroup RCC_USART_clock_source 
  * @{
  */

#define RCC_USART1CLK_PCLK                  ((uint32_t)0x10000000)
#define RCC_USART1CLK_SYSCLK                ((uint32_t)0x10000001)
#define RCC_USART1CLK_HSI                   ((uint32_t)0x10000002)
#define RCC_USART1CLK_LSE                   ((uint32_t)0x10000003)

#define RCC_USART2CLK_PCLK                  ((uint32_t)0x20000000)
#define RCC_USART2CLK_SYSCLK                ((uint32_t)0x20000004)
#define RCC_USART2CLK_HSI                   ((uint32_t)0x20000008)
#define RCC_USART2CLK_LSE                   ((uint32_t)0x2000000C)

#define RCC_USART3CLK_PCLK                   ((uint32_t)0x30000000)
#define RCC_USART3CLK_SYSCLK                 ((uint32_t)0x30000010)
#define RCC_USART3CLK_HSI                    ((uint32_t)0x30000020)
#define RCC_USART3CLK_LSE                    ((uint32_t)0x30000030)

#define RCC_UART4CLK_PCLK                   ((uint32_t)0x40000000)
#define RCC_UART4CLK_SYSCLK                 ((uint32_t)0x40000040)
#define RCC_UART4CLK_HSI                    ((uint32_t)0x40000080)
#define RCC_UART4CLK_LSE                    ((uint32_t)0x400000C0)

#define RCC_UART5CLK_PCLK                   ((uint32_t)0x50000000)
#define RCC_UART5CLK_SYSCLK                 ((uint32_t)0x50000100)
#define RCC_UART5CLK_HSI                    ((uint32_t)0x50000200)
#define RCC_UART5CLK_LSE                    ((uint32_t)0x50000300)

#define RCC_LPUARTCLK_PCLK                  ((uint32_t)0x60000000)
#define RCC_LPUARTCLK_SYSCLK                ((uint32_t)0x60000400)
#define RCC_LPUARTCLK_HSI                   ((uint32_t)0x60000800)
#define RCC_LPUARTCLK_LSE                   ((uint32_t)0x60000C00)

#define IS_RCC_USARTCLK(USARTCLK) (((USARTCLK) == RCC_USART1CLK_PCLK) || ((USARTCLK) == RCC_USART1CLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_USART1CLK_LSE) || ((USARTCLK) == RCC_USART1CLK_HSI) ||\
                                   ((USARTCLK) == RCC_USART2CLK_PCLK) || ((USARTCLK) == RCC_USART2CLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_USART2CLK_LSE) || ((USARTCLK) == RCC_USART2CLK_HSI) || \
                                   ((USARTCLK) == RCC_USART3CLK_PCLK) || ((USARTCLK) == RCC_USART3CLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_USART3CLK_LSE) || ((USARTCLK) == RCC_USART3CLK_HSI) || \
                                   ((USARTCLK) == RCC_UART4CLK_PCLK) || ((USARTCLK) == RCC_UART4CLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_UART4CLK_LSE) || ((USARTCLK) == RCC_UART4CLK_HSI) ||\
                                   ((USARTCLK) == RCC_UART5CLK_PCLK) || ((USARTCLK) == RCC_UART5CLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_UART5CLK_LSE) || ((USARTCLK) == RCC_UART5CLK_HSI) || \
                                   ((USARTCLK) == RCC_LPUARTCLK_PCLK) || ((USARTCLK) == RCC_LPUARTCLK_SYSCLK) || \
                                   ((USARTCLK) == RCC_LPUARTCLK_LSE) || ((USARTCLK) == RCC_LPUARTCLK_HSI))
/**
  * @}
  */


/** @defgroup RCC_LPTIM_clock_source 
  * @{
  */

#define RCC_LPTIM1CLKSource_APB                     ((uint32_t)0x10000000)
#define RCC_LPTIM1CLKSource_LSI                     ((uint32_t)0x10040000)
#define RCC_LPTIM1CLKSource_HSI                     ((uint32_t)0x10080000)
#define RCC_LPTIM1CLKSource_LSE                     ((uint32_t)0x100C0000)

#define RCC_LPTIM2CLKSource_APB                     ((uint32_t)0x20000000)
#define RCC_LPTIM2CLKSource_LSI                     ((uint32_t)0x20100000)
#define RCC_LPTIM2CLKSource_HSI                     ((uint32_t)0x20200000)
#define RCC_LPTIM2CLKSource_LSE                     ((uint32_t)0x20300000)

#define IS_RCC_LPTIM(CLK)      (((CLK) == RCC_LPTIM1CLKSource_APB) || ((CLK) == RCC_LPTIM1CLKSource_LSI) || \
                               ((CLK) == RCC_LPTIM1CLKSource_HSI)  || ((CLK) == RCC_LPTIM1CLKSource_LSE) || \
                               ((CLK) == RCC_LPTIM2CLKSource_APB)  || ((CLK) == RCC_LPTIM2CLKSource_LSI) || \
                               ((CLK) == RCC_LPTIM2CLKSource_HSI)  || ((CLK) == RCC_LPTIM2CLKSource_LSE))
/**
  * @}
  */

/** @defgroup RCC_ADC_Clock_Source
  * @{
  */
#define RCC_ADCCLKSource_PLLSAI1               ((uint32_t)0x10000000)    /*!<  RCC PLL_SAI1 "R" clock selected as SAR ADCs clock */
#define RCC_ADCCLKSource_PLLSAI2               ((uint32_t)0x20000000)    /*!<  RCC PLL_SAI2 "R" clock selected as SAR ADCs clock */
#define RCC_ADCCLKSource_SYSCLK                ((uint32_t)0x30000000)    /*!<  System clock selected as SAR ADCs clock */

#define IS_RCC_ADC_CLOCK(CLOCK)      (((CLOCK) == RCC_ADCCLKSource_PLLSAI1) || \
                                      ((CLOCK) == RCC_ADCCLKSource_PLLSAI2) || \
                                      ((CLOCK) == RCC_ADCCLKSource_SYSCLK))
/**
  * @}
  */ 


/** @defgroup RCC_DFSDM_Clock_Source
  * @{
  */
#define RCC_DFSDMCLKSource_APB             ((uint8_t)0x00)
#define RCC_DFSDMCLKSource_SYS             ((uint8_t)0x01)

#define IS_RCC_DFSDMCLK_SOURCE(SOURCE) (((SOURCE) == RCC_DFSDMCLKSource_APB) || ((SOURCE) == RCC_DFSDMCLKSource_SYS))  


/**
  * @}
  */ 


/** @defgroup RCC_SWP_Clock_Source
  * @{
  */
#define RCC_SWPCLKSource_APB             ((uint8_t)0x00)
#define RCC_SWPCLKSource_HSI             ((uint8_t)0x01)

#define IS_RCC_SWPCLK_SOURCE(SOURCE) (((SOURCE) == RCC_SWPCLKSource_APB) || ((SOURCE) == RCC_SWPCLKSource_HSI))  


/**
  * @}
  */ 


/** @defgroup RCC_SAI_clock_source 
  * @{
  */

#define RCC_SAI1CLK_PLLSAI1                   ((uint32_t)0x10000000)
#define RCC_SAI1CLK_PLLSAI2                   ((uint32_t)0x10400000)
#define RCC_SAI1CLK_PLLSYS                    ((uint32_t)0x10800000)
#define RCC_SAI1CLK_PIN                       ((uint32_t)0x10C00000)

#define RCC_SAI2CLK_PLLSAI1                   ((uint32_t)0x20000000)
#define RCC_SAI2CLK_PLLSAI2                   ((uint32_t)0x21000000)
#define RCC_SAI2CLK_PLLSYS                    ((uint32_t)0x22000000)
#define RCC_SAI2CLK_PIN                       ((uint32_t)0x23000000)


#define IS_RCC_SAICLK(SAICLK)  (((SAICLK) == RCC_SAI1CLK_PLLSAI1)  || ((SAICLK) == RCC_SAI1CLK_PLLSAI2) || \
                               ((SAICLK) == RCC_SAI1CLK_PIN)       || ((SAICLK) == RCC_SAI1CLK_PLLSYS)  || \
                               ((SAICLK) == RCC_SAI2CLK_PLLSAI1)   || ((SAICLK) == RCC_SAI2CLK_PLLSAI2) || \
                               ((SAICLK) == RCC_SAI2CLK_PLLSYS)    || ((SAICLK) == RCC_SAI2CLK_PIN)) 
                               
/**
  * @}
  */

/** @defgroup RCC_CLK48_clock_source 
  * @{
  */

#define RCC_CLK48Source_PLLSAI1                  ((uint32_t)0x04000000)
#define RCC_CLK48Source_PLLSYS                   ((uint32_t)0x08000000)
#define RCC_CLK48Source_MSI                      ((uint32_t)0x0C000000)


#define IS_RCC_CLK48(CLK48)  (((CLK48) == RCC_CLK48Source_PLLSAI1)  || ((CLK48) == RCC_CLK48Source_PLLSYS) || \
                              ((CLK48) == RCC_CLK48Source_MSI))
                               
/**
  * @}
  */
/** @defgroup RCC_APB1_APB2_Clock_Source
  * @{
  */
#define RCC_HCLK_Div1                    ((uint32_t)0x00000000)
#define RCC_HCLK_Div2                    ((uint32_t)0x00000400)
#define RCC_HCLK_Div4                    ((uint32_t)0x00000500)
#define RCC_HCLK_Div8                    ((uint32_t)0x00000600)
#define RCC_HCLK_Div16                   ((uint32_t)0x00000700)
#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                           ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8) || \
                           ((PCLK) == RCC_HCLK_Div16))
/**
  * @}
  */ 
  
/** @defgroup RCC_Interrupt_Source 
  * @{
  */
#define RCC_IT_LSIRDY                    ((uint32_t)0x00000001)
#define RCC_IT_LSERDY                    ((uint32_t)0x00000002)
#define RCC_IT_MSIRDY                    ((uint32_t)0x00000004)
#define RCC_IT_HSIRDY                    ((uint32_t)0x00000008)
#define RCC_IT_HSERDY                    ((uint32_t)0x00000010)
#define RCC_IT_PLLRDY                    ((uint32_t)0x00000020)
#define RCC_IT_PLLSAI1RDY                ((uint32_t)0x00000040)
#define RCC_IT_PLLSAI2RDY                ((uint32_t)0x00000080)
#define RCC_IT_HSECSS                    ((uint32_t)0x00000100)
#define RCC_IT_LSECSS                    ((uint32_t)0x00000200)


#define IS_RCC_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLSAI1RDY) || ((IT) == RCC_IT_PLLSAI2RDY)|| \
                           ((IT) == RCC_IT_LSECSS))

#define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLSAI1RDY) || ((IT) == RCC_IT_PLLSAI2RDY)|| \
                           ((IT) == RCC_IT_LSECSS)|| ((IT) == RCC_IT_HSECSS))


#define IS_RCC_CLEAR_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLSAI1RDY) || ((IT) == RCC_IT_PLLSAI2RDY)|| \
                           ((IT) == RCC_IT_LSECSS)|| ((IT) == RCC_IT_HSECSS))


/**
  * @}
  */ 
  
/** @defgroup RCC_LSE_Configuration 
  * @{
  */
#define RCC_LSE_OFF                      ((uint8_t)0x00)
#define RCC_LSE_ON                       ((uint8_t)0x01)
#define RCC_LSE_Bypass                   ((uint8_t)0x04)
#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || ((LSE) == RCC_LSE_ON) || \
                         ((LSE) == RCC_LSE_Bypass))
/**
  * @}
  */ 
  
/** @defgroup RCC_RTC_Clock_Source
  * @{
  */
#define RCC_RTCCLKSource_LSE             ((uint32_t)0x00000100)
#define RCC_RTCCLKSource_LSI             ((uint32_t)0x00000200)
#define RCC_RTCCLKSource_HSE             ((uint32_t)0x00000300)

#define IS_RCC_RTCCLK_SOURCE(SOURCE) (((SOURCE) == RCC_RTCCLKSource_LSE) || \
                                      ((SOURCE) == RCC_RTCCLKSource_LSI) || \
                                      ((SOURCE) == RCC_RTCCLKSource_HSE) )
                                     

/** @defgroup RCC_TIM_PRescaler_Selection
  * @{
  */
#define RCC_TIMPrescDesactivated             ((uint8_t)0x00)
#define RCC_TIMPrescActivated                ((uint8_t)0x01)

#define IS_RCC_TIMCLK_PRESCALER(VALUE) (((VALUE) == RCC_TIMPrescDesactivated) || ((VALUE) == RCC_TIMPrescActivated))
/**
  * @}
  */
  
/** @defgroup RCC_AHB1_Peripherals 
  * @{
  */ 
#define  RCC_AHB1Periph_DMA1                ((uint32_t)0x00000001)
#define  RCC_AHB1Periph_DMA2                ((uint32_t)0x00000002)
#define  RCC_AHB1Periph_FLASH               ((uint32_t)0x00000100)
#define  RCC_AHB1Periph_SRAM1               ((uint32_t)0x00000200)
#define  RCC_AHB1Periph_CRC                 ((uint32_t)0x00001000)
#define  RCC_AHB1Periph_TSC                 ((uint32_t)0x00010000)


#define IS_RCC_AHB1_CLOCK_PERIPH(PERIPH) ((((PERIPH) & 0xFFFEEEFC) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_LPMODE_PERIPH(PERIPH) ((((PERIPH)& 0xFFFEECFC) == 0x00) && ((PERIPH) != 0x00))


/**
  * @}
  */ 
  
/** @defgroup RCC_AHB2_Peripherals 
  * @{
  */  
#define  RCC_AHB2Periph_GPIOA                 ((uint32_t)0x00000001)
#define  RCC_AHB2Periph_GPIOB                 ((uint32_t)0x00000002)
#define  RCC_AHB2Periph_GPIOC                 ((uint32_t)0x00000004)
#define  RCC_AHB2Periph_GPIOD                 ((uint32_t)0x00000008)
#define  RCC_AHB2Periph_GPIOE                 ((uint32_t)0x00000010)
#define  RCC_AHB2Periph_GPIOF                 ((uint32_t)0x00000020)
#define  RCC_AHB2Periph_GPIOG                 ((uint32_t)0x00000040)
#define  RCC_AHB2Periph_GPIOH                 ((uint32_t)0x00000080)
#define  RCC_AHB2Periph_SRAM2                 ((uint32_t)0x00000200)
#define  RCC_AHB2Periph_OTGFS                 ((uint32_t)0x00001000)
#define  RCC_AHB2Periph_ADC                   ((uint32_t)0x00002000)
#define  RCC_AHB2Periph_AES                   ((uint32_t)0x00010000)
#define  RCC_AHB2Periph_RNG                   ((uint32_t)0x00040000)


#define IS_RCC_AHB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFFACF00) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB2_LPMODE_PERIPH(PERIPH) ((((PERIPH)& 0xFFFA8D00) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 
  
/** @defgroup RCC_AHB3_Peripherals 
  * @{
  */ 
#define  RCC_AHB3Periph_FMC                ((uint32_t)0x00000001)
#define  RCC_AHB3Periph_QSPI               ((uint32_t)0x00000100)
#define  IS_RCC_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFEFE) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCC_APB1_Peripherals 
  * @{
  */ 
#define  RCC_APB1LPeriph_TIM2                ((uint32_t)0x00000001)
#define  RCC_APB1LPeriph_TIM3                ((uint32_t)0x00000002)
#define  RCC_APB1LPeriph_TIM4                ((uint32_t)0x00000004)
#define  RCC_APB1LPeriph_TIM5                ((uint32_t)0x00000008)
#define  RCC_APB1LPeriph_TIM6                ((uint32_t)0x00000010)
#define  RCC_APB1LPeriph_TIM7                ((uint32_t)0x00000020)
#define  RCC_APB1LPeriph_LCD                 ((uint32_t)0x00000200)
#define  RCC_APB1LPeriph_WWDG                ((uint32_t)0x00000800)
#define  RCC_APB1LPeriph_SPI2                ((uint32_t)0x00004000)
#define  RCC_APB1LPeriph_SPI3                ((uint32_t)0x00008000)
#define  RCC_APB1LPeriph_USART2              ((uint32_t)0x00020000)
#define  RCC_APB1LPeriph_USART3              ((uint32_t)0x00040000)
#define  RCC_APB1LPeriph_UART4               ((uint32_t)0x00080000)
#define  RCC_APB1LPeriph_UART5               ((uint32_t)0x00100000)
#define  RCC_APB1LPeriph_I2C1                ((uint32_t)0x00200000)
#define  RCC_APB1LPeriph_I2C2                ((uint32_t)0x00400000)
#define  RCC_APB1LPeriph_I2C3                ((uint32_t)0x00800000)
#define  RCC_APB1LPeriph_CAN                 ((uint32_t)0x02000000)
#define  RCC_APB1LPeriph_PWR                 ((uint32_t)0x10000000)
#define  RCC_APB1LPeriph_DAC                 ((uint32_t)0x20000000)
#define  RCC_APB1LPeriph_OPAMP               ((uint32_t)0x40000000)
#define  RCC_APB1LPeriph_LPTIM1              ((uint32_t)0x80000000)


#define IS_RCC_APB1L_PERIPH(PERIPH) ((((PERIPH) &0xD0135C0) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB1L_RESET_PERIPH(PERIPH) ((((PERIPH) &0xD013DC0) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB1L_LP_PERIPH(PERIPH)     IS_RCC_APB1L_PERIPH(PERIPH)


/**
  * @}
  */ 
  

/** @defgroup RCC_APB2_Peripherals 
  * @{
  */ 


#define  RCC_APB1HPeriph_LPUART               ((uint32_t)0x00000001)
#define  RCC_APB1HPeriph_SWP                  ((uint32_t)0x00000004)
#define  RCC_APB1HPeriph_LPTIM2               ((uint32_t)0x00000020)

#define IS_RCC_APB1H_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFDA) == 0x00) && ((PERIPH) != 0x00))

/** @defgroup RCC_APB2_Peripherals 
  * @{
  */ 
#define  RCC_APB2Periph_SYSCFG                ((uint32_t)0x00000001)
#define  RCC_APB2Periph_COMP                  ((uint32_t)0x00000001)
#define  RCC_APB2Periph_FIREWALL              ((uint32_t)0x00000080)
#define  RCC_APB2Periph_SDIO                  ((uint32_t)0x00000400)
#define  RCC_APB2Periph_TIM1                  ((uint32_t)0x00000800)
#define  RCC_APB2Periph_SPI1                  ((uint32_t)0x00001000)
#define  RCC_APB2Periph_TIM8                  ((uint32_t)0x00002000)
#define  RCC_APB2Periph_USART1                ((uint32_t)0x00004000)
#define  RCC_APB2Periph_TIM15                 ((uint32_t)0x00010000)
#define  RCC_APB2Periph_TIM16                 ((uint32_t)0x00020000)
#define  RCC_APB2Periph_TIM17                 ((uint32_t)0x00040000)
#define  RCC_APB2Periph_SAI1                  ((uint32_t)0x00200000)
#define  RCC_APB2Periph_SAI2                  ((uint32_t)0x00400000)
#define  RCC_APB2Periph_DFSDM                 ((uint32_t)0x01000000)


#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFE98837E) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xFE9883FE) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 
  
/** @defgroup RCC_MCO1_Clock_Source_Prescaler
  * @{
  */
#define RCC_MCOSource_SYSCLK            ((uint32_t)0x01000000)
#define RCC_MCOSource_MSI               ((uint32_t)0x02000000)
#define RCC_MCOSource_HSI               ((uint32_t)0x03000000)
#define RCC_MCOSource_HSE               ((uint32_t)0x04000000)
#define RCC_MCOSource_PLLCLK            ((uint32_t)0x05000000)
#define RCC_MCOSource_LSI               ((uint32_t)0x06000000)
#define RCC_MCOSource_LSE               ((uint32_t)0x07000000)    
    
#define RCC_MCODiv_1                    ((uint32_t)0x00000000)
#define RCC_MCODiv_2                    ((uint32_t)0x10000000)
#define RCC_MCODiv_4                    ((uint32_t)0x20000000)
#define RCC_MCODiv_8                    ((uint32_t)0x30000000)
#define RCC_MCODiv_16                   ((uint32_t)0x40000000)
    
#define IS_RCC_MCOSOURCE(SOURCE) (((SOURCE) == RCC_MCOSource_SYSCLK) || ((SOURCE) == RCC_MCOSource_MSI) || \
                                   ((SOURCE) == RCC_MCOSource_HSI) || ((SOURCE) == RCC_MCOSource_HSE) \
                                   || ((SOURCE) == RCC_MCOSource_PLLCLK) || ((SOURCE) == RCC_MCOSource_LSI) \
                                   || ((SOURCE) == RCC_MCOSource_LSE))
    
#define IS_RCC_MCODIV(DIV) (((DIV) == RCC_MCODiv_1) || ((DIV) == RCC_MCODiv_2) || \
                             ((DIV) == RCC_MCODiv_4) || ((DIV) == RCC_MCODiv_8) || \
                             ((DIV) == RCC_MCODiv_16)) 

/**
  * @}
  */ 
  
/** @defgroup RCC_Flag 
  * @{
  */
#define RCC_FLAG_MSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x2A)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_PLLSAI1RDY              ((uint8_t)0x3B)
#define RCC_FLAG_PLLSAI2RDY              ((uint8_t)0x3D)

#define RCC_FLAG_LSERDY                  ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)

#define RCC_FLAG_RMVF                    ((uint8_t)0x77)
#define RCC_FLAG_FIREWALLRST             ((uint8_t)0x78)
#define RCC_FLAG_OBLRST                  ((uint8_t)0x79)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7F)


#define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_MSIRDY)     || ((FLAG) == RCC_FLAG_HSIRDY) || \
                           ((FLAG) == RCC_FLAG_HSERDY)     || ((FLAG) == RCC_FLAG_PLLRDY) || \
                           ((FLAG) == RCC_FLAG_PLLSAI1RDY) || ((FLAG) == RCC_FLAG_PLLSAI2RDY) || \
                           ((FLAG) == RCC_FLAG_LSERDY)     || ((FLAG) == RCC_FLAG_LSIRDY) || \
                           ((FLAG) == RCC_FLAG_RMVF)       || ((FLAG) == RCC_FLAG_FIREWALLRST)|| \
                           ((FLAG) == RCC_FLAG_OBLRST)     || ((FLAG) == RCC_FLAG_PINRST)|| \
                           ((FLAG) == RCC_FLAG_PORRST)     || ((FLAG) == RCC_FLAG_SFTRST)|| \
                           ((FLAG) == RCC_FLAG_IWDGRST)    || ((FLAG) == RCC_FLAG_WWDGRST)|| \
                           ((FLAG) == RCC_FLAG_LPWRRST))

#define IS_RCC_HSI_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)
#define IS_RCC_MSI_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0xFF)
/**
  * @}
  */ 

/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 

/* Function used to set the RCC clock configuration to the default reset state */
void RCC_DeInit(void);

/* Internal/external clocks, PLL, CSS and MCO configuration functions *********/
void        RCC_HSEConfig(uint8_t RCC_HSE);
ErrorStatus RCC_WaitForHSEStartUp(void);
void RCC_MSIRangeConfig(uint32_t RCC_MSIRange);
void RCC_MSIStandbyRangeConfig(uint32_t RCC_MSIRange);
void RCC_AdjustMSICalibrationValue(uint32_t MSICalibrationValue);
void RCC_MSICmd(FunctionalState NewState);
void        RCC_AdjustHSICalibrationValue(uint32_t HSICalibrationValue);
void        RCC_HSICmd(FunctionalState NewState);
void        RCC_LSEConfig(uint8_t RCC_LSE);
void        RCC_LSICmd(FunctionalState NewState);

void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ , uint32_t PLLR);
void RCC_PLLCmd(FunctionalState NewState);
void RCC_PLLClockOutCmd(uint32_t RCC_PLLClockOut, FunctionalState NewState);

void RCC_PLLSAI1Config(uint32_t PLLSAI1N, uint32_t PLLSAI1P,uint32_t PLLSAI1Q, uint32_t PLLSAI1R);
void RCC_PLLSAI1Cmd(FunctionalState NewState);
void RCC_PLLSAI1ClockOutCmd(uint32_t RCC_PLLSAI1ClockOut, FunctionalState NewState);

void RCC_PLLSAI2Config(uint32_t PLLSAIN,uint32_t PLLSAI1P, uint32_t PLLSAI2Q, uint32_t PLLSAI2R);
void RCC_PLLSAI2Cmd(FunctionalState NewState);
void RCC_PLLSAI2ClockOutCmd(uint32_t RCC_PLLSAI2ClockOut, FunctionalState NewState);

void RCC_HSEClockSecuritySystemCmd(FunctionalState NewState);
void RCC_LSEClockSecuritySystemCmd(FunctionalState NewState);
void RCC_StopWakeUpClock(uint32_t RCC_STOPWUCK_Clock);
void RCC_MCOConfig(uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
/* System, AHB and APB busses clocks configuration functions ******************/
void    RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
uint8_t RCC_GetSYSCLKSource(void);
void    RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void    RCC_PCLK1Config(uint32_t RCC_HCLK);
void    RCC_PCLK2Config(uint32_t RCC_HCLK);
void    RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

/* Peripheral clocks configuration functions **********************************/
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_RTCCLKCmd(FunctionalState NewState);
void RCC_BackupResetCmd(FunctionalState NewState);
void RCC_I2CCLKConfig(uint32_t RCC_I2CCLK);
void RCC_USARTCLKConfig(uint32_t RCC_USARTCLK);
void RCC_LPTIMCLKConfig(uint32_t RCC_LPTIMxCLKSource);
void RCC_ADCCLKConfig(uint32_t RCC_ADCCLKSource);
void RCC_DFSDMCLKConfig(uint32_t RCC_DFSDMCLKSource);
void RCC_SWPCLKConfig(uint32_t RCC_SWPCLKSource);
void RCC_SAICLKConfig(uint32_t RCC_SAICLKSource);
void RCC_CLK48Config(uint32_t RCC_CLK48Source);

void RCC_TIMCLKPresConfig(uint32_t RCC_TIMCLKPrescaler);

void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_APB1LPeriphClockCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState);
void RCC_APB1HPeriphClockCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_APB1LPeriphResetCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState);
void RCC_APB1HPeriphResetCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState);
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_APB1LPeriphClockLPModeCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState);
void RCC_APB1HPeriphClockLPModeCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void       RCC_ITConfig(uint32_t RCC_IT, FunctionalState NewState);
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG);
void       RCC_ClearFlag(void);
ITStatus   RCC_GetITStatus(uint32_t RCC_IT);
void       RCC_ClearITPendingBit(uint32_t RCC_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4xx_RCC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
