/**
  ******************************************************************************
  * @file    stm32l4xx_syscfg.c
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
  * @brief   This file provides firmware functions to manage the SYSCFG peripheral.
  *
 @verbatim
    
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions for:
            
       (#) Remapping the memory accessible in the code area using SYSCFG_MemoryRemapConfig()
            
       (#) Swapping the internal flash Bank1 and Bank2  
                
       (#) Manage the EXTI lines connection to the GPIOs using SYSCFG_EXTILineConfig()

       (#) Firewall Access Enable.

       (#) Enabling I2C fast mode plus driving capability for I2C plus

       (#) Configuring the CLASSB requirements

    
              
       -@- SYSCFG APB clock must be enabled to get write access to SYSCFG registers,
           using RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
                   
 @endverbatim      
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_syscfg.h"
#include "stm32l4xx_rcc.h"

#define assert_param(expr) ((void)0)
	
/** @addtogroup STM32L4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SYSCFG 
  * @brief SYSCFG driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define SYSCFG_OFFSET             (SYSCFG_BASE - PERIPH_BASE)

    /* ---  MEMRMP Register ---*/ 
/* Alias word address of UFB_MODE bit */ 
#define MEMRMP_OFFSET             SYSCFG_OFFSET 
#define UFB_MODE_BitNumber        ((uint8_t)0x8) 
#define UFB_MODE_BB               (PERIPH_BB_BASE + (MEMRMP_OFFSET * 32) + (UFB_MODE_BitNumber * 4)) 
#define QFS_MODE_BitNumber        ((uint8_t)0x3) 
#define QFS_MODE_BB               (PERIPH_BB_BASE + (MEMRMP_OFFSET * 32) + (QFS_MODE_BitNumber * 4))
    
/* ---  PMC Register ---*/ 
/* Alias word address of FWDIS bit */ 
#define PMC_OFFSET              (SYSCFG_OFFSET + 0x04) 
#define FWDIS_BitNumber         ((uint8_t)0x0) 
#define PMC_FWDIS_BB            (PERIPH_BB_BASE + (PMC_OFFSET * 32) + (FWDIS_BitNumber * 4)) 

/* --- GPC Register ---*/
/* Alias word address of BYP_ADDR_PAR bit */
#define GPC_OFFSET               (SYSCFG_OFFSET + 0x18)
#define BRER_BitNumber           ((uint8_t)0x0) 
#define GPC_BRER_BB              (PERIPH_BB_BASE + (GPC_OFFSET * 32) + (BRER_BitNumber * 4))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SYSCFG_Private_Functions
  * @{
  */ 

/**
  * @brief  Deinitializes the Alternate Functions (remap and EXTI configuration)
  *   registers to their default reset values.
  * @param  None
  * @retval None
  */
void SYSCFG_DeInit(void)
{
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, DISABLE);
}


/**
  * @brief  Changes the mapping of the specified pin.
  * @param  SYSCFG_Memory: selects the memory remapping.
  *         This parameter can be one of the following values:
  *            @arg SYSCFG_MemoryRemap_Flash:       Main Flash memory mapped at 0x00000000  
  *            @arg SYSCFG_MemoryRemap_SystemFlash: System Flash memory mapped at 0x00000000 
  *            @arg SYSCFG_MemoryRemap_FMC:         External memory through FMC  mapped at 0x00000000  
  *            @arg SYSCFG_MemoryRemap_SRAM:        Embedded SRAM (96kB) mapped at 0x00000000
  *            @arg SYSCFG_MemoryRemap_QSPI:        External Flash through QuadSPI mapped at at 0x00000000             
  * @retval None
  */
void SYSCFG_MemoryRemapConfig(uint8_t SYSCFG_MemoryRemap)
{
  /* Check the parameters */
  assert_param(IS_SYSCFG_MEMORY_REMAP_CONFING(SYSCFG_MemoryRemap));
 
  SYSCFG->MEMRMP &= ~ SYSCFG_MEMRMP_MEM_MODE ;
  SYSCFG->MEMRMP |= SYSCFG_MemoryRemap;

}

/**
  * @brief  Enables or disables the Interal FLASH Bank Swapping.
  * @param  NewState: new state of Interal FLASH Bank swapping.
  *          This parameter can be one of the following values:
  *            @arg ENABLE: Flash Bank2 mapped at 0x08000000 (and aliased @0x00000000) 
  *                         and Flash Bank1 mapped at 0x08080000 (and aliased at 0x00080000)   
  *            @arg DISABLE:(the default state) Flash Bank1 mapped at 0x08000000 (and aliased @0x0000 0000) 
                            and Flash Bank2 mapped at 0x08080000 (and aliased at 0x00080000)  
  * @retval None
  */
void SYSCFG_MemorySwappingBank(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) UFB_MODE_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables the QUADSPI/FMC memory Swapping.
  * @param  NewState: new state of QUADSPI/FMC Bank swapping.
  *          This parameter can be one of the following values:
  *            @arg ENABLE: FMC PC CARD bank is mapped at 0xB0000000 
  *                          and Quad SPI memory is mapped at 0x90000000 
  *                          (Quad SPI memory is executable through S BUS, FMC PC CARD bank is not)
  *            @arg DISABLE:(the default state) FMC PC CARD bank is mapped at 0x90000000 
  *                          and Quad SPI memory is mapped at 0xB0000000 
  *                          (FMC PC CARD bank is executable through S BUS, QSPI memory is not)
  * @retval None
  */
void SYSCFG_ExternalMemorySwapping(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) QFS_MODE_BB = (uint32_t)NewState;
}

/**
  * @brief  FIREWALL access enable.
  * @note  Set by default (after reset), can be cleared by software but once cleared 
  * it can’t be set by software.  This bit can’t be set by the APB reset. 
  * A system reset is needed
  *
  * @param  None
  * @retval None
  */
void SYSCFG_FIREWALLAccessEnable(void)
{
  *(__IO uint32_t *) PMC_FWDIS_BB = (uint8_t)0x00000000;
}

/**
  * @brief  Enables or disables the selected SYSCFG interrupts.
  * @param  SYSCFG_IT: specifies the SYSCFG interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg SYSCFG_IT_IXC: Inexact Interrupt
  *     @arg SYSCFG_IT_IDC: Input denormal Interrupt
  *     @arg SYSCFG_IT_OFC: Overflow Interrupt
  *     @arg SYSCFG_IT_UFC: Underflow Interrupt
  *     @arg SYSCFG_IT_DZC: Divide-by-zero Interrupt
  *     @arg SYSCFG_IT_IOC: Invalid operation Interrupt
  * @param  NewState: new state of the specified SYSCFG interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SYSCFG_ITConfig(uint32_t SYSCFG_IT, FunctionalState NewState)  
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SYSCFG_IT(SYSCFG_IT)); 

  if (NewState != DISABLE)
  {
    /* Enable the selected SYSCFG interrupts */
    SYSCFG->PMC |= SYSCFG_IT;
  }
  else
  {
    /* Disable the selected SYSCFG interrupts */
    SYSCFG->PMC &= ((uint32_t)~SYSCFG_IT);
  }
}
/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
  *          EXTI lines where x can be (A..H).
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  *           This parameter can be EXTI_PinSourcex where x can be (0..15, except
  *           for EXTI_PortSourceGPIOH x can be (0,1)
  * @retval None
  */
void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
  uint32_t tmp = 0x00;

  /* Check the parameters */
  assert_param(IS_EXTI_PORT_SOURCE(EXTI_PortSourceGPIOx));
  assert_param(IS_EXTI_PIN_SOURCE(EXTI_PinSourcex));

  tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03));
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] &= ~tmp;
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
}

/**
  * @brief  Configures the I2C fast mode plus driving capability.
  * @param  SYSCFG_I2CFastModePlus: selects the pin.
  *   This parameter can be one of the following values:
  *     @arg SYSCFG_I2CFastModePlus_PB6: Configure fast mode plus driving capability for PB6
  *     @arg SYSCFG_I2CFastModePlus_PB7: Configure fast mode plus driving capability for PB7
  *     @arg SYSCFG_I2CFastModePlus_PB8: Configure fast mode plus driving capability for PB8
  *     @arg SYSCFG_I2CFastModePlus_PB9: Configure fast mode plus driving capability for PB9
  *     @arg SYSCFG_I2CFastModePlus_I2C1: Configure fast mode plus driving capability for I2C1 pins
  *     @arg SYSCFG_I2CFastModePlus_I2C2: Configure fast mode plus driving capability for I2C2 pins
  *     @arg SYSCFG_I2CFastModePlus_I2C3: Configure fast mode plus driving capability for I2C3 pins
  * @param  NewState: new state of I2C Pin. 
  *         This parameter can be:
  *     @arg ENABLE: Enable fast mode plus driving capability for selected I2C pin
  *     @arg DISABLE: Disable fast mode plus driving capability for selected I2C pin
  * @note  For I2C1, fast mode plus driving capability can be enabled on all selected
  *        I2C1 pins using SYSCFG_I2CFastModePlus_I2C1 parameter or independently
  *        on each one of the following pins PB6, PB7, PB8 and PB9.
  * @note  For remaing I2C1 pins  fast mode plus driving capability
  *        can be enabled only by using SYSCFG_I2CFastModePlus_I2C1 parameter.
  * @note  For all I2C2 pins fast mode plus driving capability can be enabled
  *        only by using SYSCFG_I2CFastModePlus_I2C2 parameter.
  * @note  For all I2C3 pins fast mode plus driving capability can be enabled
  *        only by using SYSCFG_I2CFastModePlus_I2C3 parameter.
  * @retval None
  */
void SYSCFG_I2CFastModePlusConfig(uint32_t SYSCFG_I2CFastModePlus, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SYSCFG_I2C_FMP(SYSCFG_I2CFastModePlus));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable fast mode plus driving capability for selected I2C pin */
    SYSCFG->PMC |= (uint32_t)SYSCFG_I2CFastModePlus;
  }
  else
  {
    /* Disable fast mode plus driving capability for selected I2C pin */
    SYSCFG->PMC &= (uint32_t)(~SYSCFG_I2CFastModePlus);
  }
}

/**
  * @brief  Connects the selected parameter to the break input of TIM1.
  * @note   The selected configuration is locked and can be unlocked by system reset
  * @param  SYSCFG_Break: selects the configuration to be connected to break
  *         input of TIM1
  *   This parameter can be any combination of the following values:
  *     @arg SYSCFG_Break_PVD: PVD interrupt is connected to the break input of TIM1.
  *     @arg SYSCFG_Break_SRAMParity: SRAM Parity error is connected to the break input of TIM1.
  *     @arg SYSCFG_Break_Lockup: Lockup output of CortexM4 is connected to the break input of TIM1.
  *     @arg SYSCFG_Break_ECC
  * @retval None
  */
void SYSCFG_BreakConfig(uint32_t SYSCFG_Break)
{
  /* Check the parameter */
  assert_param(IS_SYSCFG_LOCK_CONFIG(SYSCFG_Break));

  SYSCFG->CBR |= (uint32_t) SYSCFG_Break;
}

/**
  * @brief  Starts a hardware SRAM2 erase operation
  * @note   As long as SRAM2 is not erased the BREO bit will be set. 
            This bit is automatically reset at the end of the SRAM2 erase operation
  * @param  None
  * @retval None
  */
void SYSCFG_SRAM2Erase(void)
{
  
  /* Disable the write protection for RTC registers */
   SYSCFG->SKR = 0xCA;
   SYSCFG->SKR = 0x53;
  /* Starts a hardware SRAM2 erase operation*/
  *(__IO uint32_t *) GPC_BRER_BB = (uint8_t)0x00000001;
}

/**
  * @brief  Enables SRAM2 write protection.
  * @note   Enabling  SRAM2 write protection locks the configuration bit.
  *         To disable SRAM2 write protection perform a system reset.
  * @param  SYSCFG_SRAM2WRP: The page number to be write protected.
  *         this parameter can be a value between SYSCFG_SRAM2WRP_Page0 and SYSCFG_SRAM2WRP_Page31.
  * @retval None
  */
void SYSCFG_SRAM2WRPEnable(uint32_t SYSCFG_SRAM2WRP)
{
  /* Check the parameter */
  assert_param(IS_SYSCFG_PAGE(SYSCFG_SRAM2WRP));

  /* Enable the write-protection on the selected  SRAM2 page */
  SYSCFG->SWP |= (uint32_t)SYSCFG_SRAM2WRP;
}
/**
  * @brief  Checks whether the specified SYSCFG flag is set or not.
  * @param  SYSCFG_Flag: specifies the SYSCFG flag to check. 
  *   This parameter can be one of the following values:
  *     @arg SYSCFG_FLAG_PE: SRAM parity error flag.
  *     @arg SYSCFG_FLAG_BREO: SRAM2 erase on going.
  * @retval The new state of SYSCFG_Flag (SET or RESET).
  */
FlagStatus SYSCFG_GetFlagStatus(uint32_t SYSCFG_Flag)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameter */
  assert_param(IS_SYSCFG_FLAG(SYSCFG_Flag));

  if (SYSCFG_Flag == SYSCFG_FLAG_PE)
  {    
  /* Check the status of SPF flag */
  if ((SYSCFG->CBR & SYSCFG_CBR_SPF) != (uint32_t)RESET)
  {
    /* SYSCFG_Flag is set */
    bitstatus = SET;
  }
  else
  {
    /* SYSCFG_Flag is reset */
    bitstatus = RESET;
  }
  
  }
  
  else {
    
     /* Check the status of BREO flag */
  if ((SYSCFG->GPC & SYSCFG_GPC_BREO) != (uint32_t)RESET)
  {
    /* SYSCFG_Flag is set */
    bitstatus = SET;
  }
  else
  {
    /* SYSCFG_Flag is reset */
    bitstatus = RESET;
  }
    
  }
  /* Return the SYSCFG_Flag status */
  return  bitstatus;
}
/**
  * @brief  Clears the selected SYSCFG flag.
  * @param  SYSCFG_Flag: selects the flag to be cleared.
  *   This parameter can be one of the following values:
  *     @arg SYSCFG_FLAG_PE: SRAM parity error flag.
  * @retval None
  */
void SYSCFG_ClearFlag(uint32_t SYSCFG_Flag)
{
  /* Check the parameter */
  assert_param(IS_SYSCFG_FLAG(SYSCFG_Flag));

  SYSCFG->CBR |= (uint32_t) SYSCFG_Flag;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/   
