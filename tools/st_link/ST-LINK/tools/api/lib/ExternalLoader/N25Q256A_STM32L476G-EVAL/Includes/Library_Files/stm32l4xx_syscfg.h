/**
  ******************************************************************************
  * @file    stm32l4xx_syscfg.h
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
  * @brief   This file contains all the functions prototypes for the SYSCFG firmware
  *          library. 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#ifndef __STM32L4xx_SYSCFG_H
#define __STM32L4xx_SYSCFG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"

/** @addtogroup STM32L4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SYSCFG
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
  
/** @defgroup SYSCFG_Exported_Constants 
  * @{
  */ 

/** @defgroup SYSCFG_EXTI_Port_Sources 
  * @{
  */ 

#define EXTI_PortSourceGPIOA       ((uint8_t)0x00)
#define EXTI_PortSourceGPIOB       ((uint8_t)0x01)
#define EXTI_PortSourceGPIOC       ((uint8_t)0x02)
#define EXTI_PortSourceGPIOD       ((uint8_t)0x03)
#define EXTI_PortSourceGPIOE       ((uint8_t)0x04)
#define EXTI_PortSourceGPIOF       ((uint8_t)0x05)
#define EXTI_PortSourceGPIOG       ((uint8_t)0x06)
#define EXTI_PortSourceGPIOH       ((uint8_t)0x07)

#define IS_EXTI_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == EXTI_PortSourceGPIOA) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOB) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOC) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOD) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOE) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOF) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOG) || \
                                         ((PORTSOURCE) == EXTI_PortSourceGPIOH))                                         
/**
  * @}
  */ 

/** @defgroup SYSCFG_EXTI_Pin_Sources 
  * @{
  */ 
#define EXTI_PinSource0            ((uint8_t)0x00)
#define EXTI_PinSource1            ((uint8_t)0x01)
#define EXTI_PinSource2            ((uint8_t)0x02)
#define EXTI_PinSource3            ((uint8_t)0x03)
#define EXTI_PinSource4            ((uint8_t)0x04)
#define EXTI_PinSource5            ((uint8_t)0x05)
#define EXTI_PinSource6            ((uint8_t)0x06)
#define EXTI_PinSource7            ((uint8_t)0x07)
#define EXTI_PinSource8            ((uint8_t)0x08)
#define EXTI_PinSource9            ((uint8_t)0x09)
#define EXTI_PinSource10           ((uint8_t)0x0A)
#define EXTI_PinSource11           ((uint8_t)0x0B)
#define EXTI_PinSource12           ((uint8_t)0x0C)
#define EXTI_PinSource13           ((uint8_t)0x0D)
#define EXTI_PinSource14           ((uint8_t)0x0E)
#define EXTI_PinSource15           ((uint8_t)0x0F)
#define IS_EXTI_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == EXTI_PinSource0)  || \
                                       ((PINSOURCE) == EXTI_PinSource1)  || \
                                       ((PINSOURCE) == EXTI_PinSource2)  || \
                                       ((PINSOURCE) == EXTI_PinSource3)  || \
                                       ((PINSOURCE) == EXTI_PinSource4)  || \
                                       ((PINSOURCE) == EXTI_PinSource5)  || \
                                       ((PINSOURCE) == EXTI_PinSource6)  || \
                                       ((PINSOURCE) == EXTI_PinSource7)  || \
                                       ((PINSOURCE) == EXTI_PinSource8)  || \
                                       ((PINSOURCE) == EXTI_PinSource9)  || \
                                       ((PINSOURCE) == EXTI_PinSource10) || \
                                       ((PINSOURCE) == EXTI_PinSource11) || \
                                       ((PINSOURCE) == EXTI_PinSource12) || \
                                       ((PINSOURCE) == EXTI_PinSource13) || \
                                       ((PINSOURCE) == EXTI_PinSource14) || \
                                       ((PINSOURCE) == EXTI_PinSource15))
/**
  * @}
  */ 

/** @defgroup SYSCFG_Memory_Remap_Config 
  * @{
  */ 
#define SYSCFG_MemoryRemap_Flash       ((uint8_t)0x00)
#define SYSCFG_MemoryRemap_SystemFlash ((uint8_t)0x01)
#define SYSCFG_MemoryRemap_FMC         ((uint8_t)0x02) 
#define SYSCFG_MemoryRemap_SRAM        ((uint8_t)0x03) 
#define SYSCFG_MemoryRemap_QSPI        ((uint8_t)0x06)

#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SystemFlash) || \
                                               ((REMAP) == SYSCFG_MemoryRemap_FMC)         || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SRAM)        || \
                                               ((REMAP) == SYSCFG_MemoryRemap_QSPI))
                                                                                              
/**
  * @}
  */ 

 /** @defgroup SYSCFG_I2C_FastModePlus_Config 
  * @{
  */ 
#define SYSCFG_I2CFastModePlus_PB6       SYSCFG_PMC_I2C_PB6_FMP  /*!< Enable Fast Mode Plus on PB6 */
#define SYSCFG_I2CFastModePlus_PB7       SYSCFG_PMC_I2C_PB7_FMP  /*!< Enable Fast Mode Plus on PB7 */
#define SYSCFG_I2CFastModePlus_PB8       SYSCFG_PMC_I2C_PB8_FMP  /*!< Enable Fast Mode Plus on PB8 */
#define SYSCFG_I2CFastModePlus_PB9       SYSCFG_PMC_I2C_PB9_FMP  /*!< Enable Fast Mode Plus on PB9 */
#define SYSCFG_I2CFastModePlus_I2C1      SYSCFG_PMC_I2C1_FMP     /*!< Enable Fast Mode Plus on I2C1 pins */
#define SYSCFG_I2CFastModePlus_I2C2      SYSCFG_PMC_I2C2_FMP     /*!< Enable Fast Mode Plus on I2C2 pins */
#define SYSCFG_I2CFastModePlus_I2C3      SYSCFG_PMC_I2C3_FMP     /*!< Enable Fast Mode Plus on I2C3 pins */

#define IS_SYSCFG_I2C_FMP(PIN) (((PIN) == SYSCFG_I2CFastModePlus_PB6)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB7)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB8)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB9)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_I2C1) || \
                                ((PIN) == SYSCFG_I2CFastModePlus_I2C2) || \
                                ((PIN) == SYSCFG_I2CFastModePlus_I2C3))

/**
  * @}
  */

/** @defgroup SYSCFG_FPU_Interrupt_Config 
  * @{
  */ 
#define SYSCFG_IT_IXC              SYSCFG_PMC_FPU_IE_5  /*!< Inexact Interrupt enable (interrupt disabled by default) */
#define SYSCFG_IT_IDC              SYSCFG_PMC_FPU_IE_4  /*!< Input denormal Interrupt enable */
#define SYSCFG_IT_OFC              SYSCFG_PMC_FPU_IE_3  /*!< Overflow Interrupt enable */
#define SYSCFG_IT_UFC              SYSCFG_PMC_FPU_IE_2  /*!< Underflow Interrupt enable */
#define SYSCFG_IT_DZC              SYSCFG_PMC_FPU_IE_1  /*!< Divide-by-zero Interrupt enable */
#define SYSCFG_IT_IOC              SYSCFG_PMC_FPU_IE_0  /*!< Invalid operation Interrupt enable */

#define IS_SYSCFG_IT(IT) ((((IT) & (uint32_t)0x03FFFFFF) == 0) && ((IT) != 0))

/**
  * @}
  */

/** @defgroup SYSCFG_Lock_Config
  * @{
  */
#define SYSCFG_Break_PVD                     SYSCFG_CBR_PVDL  /*!< Enables and locks the PVD connection with TIM1/8/15/16/17 Break Input and also the PVD_EN and PVDSEL[2:0] bits of the Power Control Interface */
#define SYSCFG_Break_SRAMParity              SYSCFG_CBR_SPL  /*!< Enables and locks the SRAM_PARITY error signal with Break Input of TIM1/8/15/16/17 */
#define SYSCFG_Break_Lockup                  SYSCFG_CBR_CLL  /*!< Enables and locks the LOCKUP output of CortexM4 with Break Input of TIM1/8/15/16/17 */
#define SYSCFG_Break_ECC                     SYSCFG_CBR_ECCL /*!< Enables and locks the FLASH ECC error with Break Input of TIM1/8/15/16/17 */

#define IS_SYSCFG_LOCK_CONFIG(CONFIG) (((CONFIG) == SYSCFG_Break_PVD)        || \
                                       ((CONFIG) == SYSCFG_Break_SRAMParity) || \
                                       ((CONFIG) == SYSCFG_Break_Lockup)     || \
                                       ((CONFIG) == SYSCFG_Break_ECC))

/**
  * @}
  */                                                
/** @defgroup SYSCFG_SRAM2WRP_Config
  * @{
  */
#define SYSCFG_SRAM2WRP_Page0                 SYSCFG_SWP_PAGE0 /*!< SRAM2 Write protection page 0 */
#define SYSCFG_SRAM2WRP_Page1                 SYSCFG_SWP_PAGE1 /*!< SRAM2 Write protection page 1 */
#define SYSCFG_SRAM2WRP_Page2                 SYSCFG_SWP_PAGE2 /*!< SRAM2 Write protection page 2 */
#define SYSCFG_SRAM2WRP_Page3                 SYSCFG_SWP_PAGE3 /*!< SRAM2 Write protection page 3 */
#define SYSCFG_SRAM2WRP_Page4                 SYSCFG_SWP_PAGE4 /*!< SRAM2 Write protection page 4 */
#define SYSCFG_SRAM2WRP_Page5                 SYSCFG_SWP_PAGE5 /*!< SRAM2 Write protection page 5 */
#define SYSCFG_SRAM2WRP_Page6                 SYSCFG_SWP_PAGE6 /*!< SRAM2 Write protection page 6 */
#define SYSCFG_SRAM2WRP_Page7                 SYSCFG_SWP_PAGE7 /*!< SRAM2 Write protection page 7 */
#define SYSCFG_SRAM2WRP_Page8                 SYSCFG_SWP_PAGE8 /*!< SRAM2 Write protection page 8 */
#define SYSCFG_SRAM2WRP_Page9                 SYSCFG_SWP_PAGE9 /*!< SRAM2 Write protection page 9 */
#define SYSCFG_SRAM2WRP_Page10                SYSCFG_SWP_PAGE10 /*!< SRAM2 Write protection page 10 */
#define SYSCFG_SRAM2WRP_Page11                SYSCFG_SWP_PAGE11 /*!< SRAM2 Write protection page 11 */
#define SYSCFG_SRAM2WRP_Page12                SYSCFG_SWP_PAGE12 /*!< SRAM2 Write protection page 12 */
#define SYSCFG_SRAM2WRP_Page13                SYSCFG_SWP_PAGE13 /*!< SRAM2 Write protection page 13 */
#define SYSCFG_SRAM2WRP_Page14                SYSCFG_SWP_PAGE14 /*!< SRAM2 Write protection page 14 */
#define SYSCFG_SRAM2WRP_Page15                SYSCFG_SWP_PAGE15 /*!< SRAM2 Write protection page 15 */
#define SYSCFG_SRAM2WRP_Page16                SYSCFG_SWP_PAGE16 /*!< SRAM2 Write protection page 16 */
#define SYSCFG_SRAM2WRP_Page17                SYSCFG_SWP_PAGE17 /*!< SRAM2 Write protection page 17 */
#define SYSCFG_SRAM2WRP_Page18                SYSCFG_SWP_PAGE18 /*!< SRAM2 Write protection page 18 */
#define SYSCFG_SRAM2WRP_Page19                SYSCFG_SWP_PAGE19 /*!< SRAM2 Write protection page 19 */
#define SYSCFG_SRAM2WRP_Page20                SYSCFG_SWP_PAGE20 /*!< SRAM2 Write protection page 20 */
#define SYSCFG_SRAM2WRP_Page21                SYSCFG_SWP_PAGE21 /*!< SRAM2 Write protection page 21 */
#define SYSCFG_SRAM2WRP_Page22                SYSCFG_SWP_PAGE22 /*!< SRAM2 Write protection page 22 */
#define SYSCFG_SRAM2WRP_Page23                SYSCFG_SWP_PAGE23 /*!< SRAM2 Write protection page 23 */
#define SYSCFG_SRAM2WRP_Page24                SYSCFG_SWP_PAGE24 /*!< SRAM2 Write protection page 24 */
#define SYSCFG_SRAM2WRP_Page25                SYSCFG_SWP_PAGE25 /*!< SRAM2 Write protection page 25 */
#define SYSCFG_SRAM2WRP_Page26                SYSCFG_SWP_PAGE26 /*!< SRAM2 Write protection page 26 */
#define SYSCFG_SRAM2WRP_Page27                SYSCFG_SWP_PAGE27 /*!< SRAM2 Write protection page 27 */
#define SYSCFG_SRAM2WRP_Page28                SYSCFG_SWP_PAGE28 /*!< SRAM2 Write protection page 28 */
#define SYSCFG_SRAM2WRP_Page29                SYSCFG_SWP_PAGE29 /*!< SRAM2 Write protection page 29 */
#define SYSCFG_SRAM2WRP_Page30                SYSCFG_SWP_PAGE30 /*!< SRAM2 Write protection page 30 */
#define SYSCFG_SRAM2WRP_Page31                SYSCFG_SWP_PAGE31 /*!< SRAM2 Write protection page 31 */
#define IS_SYSCFG_PAGE(PAGE)((((PAGE) & (uint32_t)0x00000000) == 0x00000000) && ((PAGE) != 0x00000000))

/**
  * @}
  */
/** @defgroup SYSCFG_flags_definition 
  * @{
  */

#define SYSCFG_FLAG_PE               SYSCFG_CBR_SPF
#define SYSCFG_FLAG_BREO             SYSCFG_GPC_BREO
    
#define IS_SYSCFG_FLAG(FLAG) (((FLAG) == SYSCFG_FLAG_PE) || \
                              ((FLAG) == SYSCFG_FLAG_BREO))

/**
  * @}
  */

/**
  * @}
  */
   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 
 
void SYSCFG_DeInit(void);
void SYSCFG_MemoryRemapConfig(uint8_t SYSCFG_MemoryRemap);
void SYSCFG_MemorySwappingBank(FunctionalState NewState);
void SYSCFG_ExternalMemorySwapping(FunctionalState NewState);
void SYSCFG_FIREWALLAccessEnable(void);
void SYSCFG_ITConfig(uint32_t SYSCFG_IT, FunctionalState NewState);  
void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);
void SYSCFG_I2CFastModePlusConfig(uint32_t SYSCFG_I2CFastModePlus, FunctionalState NewState);
void SYSCFG_BreakConfig(uint32_t SYSCFG_Break);
void SYSCFG_SRAM2Erase(void);
void SYSCFG_SRAM2WRPEnable(uint32_t SYSCFG_SRAM2WRP);
FlagStatus SYSCFG_GetFlagStatus(uint32_t SYSCFG_Flag);
void SYSCFG_ClearFlag(uint32_t SYSCFG_Flag);





#ifdef __cplusplus
}
#endif

#endif /*__STM32L4xx_SYSCFG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
