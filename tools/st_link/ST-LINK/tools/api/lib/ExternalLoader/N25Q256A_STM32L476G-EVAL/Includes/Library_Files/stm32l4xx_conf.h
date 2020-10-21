/**
  ******************************************************************************
  * @file    stm32l4xx_conf.h  
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
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
#ifndef __STM32L4xx_CONF_H
#define __STM32L4xx_CONF_H

 #if defined  (HSE_VALUE)
  // Set the right HSE value as configured in the FPGA platform
 #undef HSE_VALUE
 #define HSE_VALUE    ((uint32_t)60000000) 
 #endif // HSE_VALUE

 /* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32L4xx_CONF_H */

/* Includes ------------------------------------------------------------------*/
/* Uncomment the line below to enable peripheral header file inclusion */
//#include "stm32l4xx_adc.h"
//#include "stm32l4xx_can.h"
//#include "stm32l4xx_comp.h"
//#include "stm32l4xx_crc.h"
//#include "stm32l4xx_cryp.h"
//#include "stm32l4xx_dac.h"
//#include "stm32l4xx_dbgmcu.h"
//#include "stm32l4xx_dfsdm.h"
//#include "stm32l4xx_dma.h"
//#include "stm32l4xx_exti.h"
//#include "stm32l4xx_flash.h"
//#include "stm32l4xx_fmc.h"
#include "stm32l4xx_gpio.h"
//#include "stm32l4xx_i2c.h"
//#include "stm32l4xx_iwdg.h"
//#include "stm32l4xx_lcd.h"
//#include "stm32l4xx_lptim.h"
//#include "stm32l4xx_mifi.h"
//#include "stm32l4xx_opamp.h"
//#include "stm32l4xx_pwr.h"
#include "stm32l4xx_quadspi.h"
#include "stm32l4xx_rcc.h"
//#include "stm32l4xx_rng.h"
//#include "stm32l4xx_rtc.h"
//#include "stm32l4xx_sai.h"
//#include "stm32l4xx_sdio.h"
//#include "stm32l4xx_spi.h"
//#include "stm32l4xx_syscfg.h"
//#include "stm32l4xx_swpmi.h"
//#include "stm32l4xx_tim.h"
//#include "stm32l4xx_usart.h"
//#include "stm32l4xx_wwdg.h"
//#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* If an external clock source is used, then the value of the following define 
   should be set to the value of the external clock source, else, if no external 
   clock is used, keep this define commented */

/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */    



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
