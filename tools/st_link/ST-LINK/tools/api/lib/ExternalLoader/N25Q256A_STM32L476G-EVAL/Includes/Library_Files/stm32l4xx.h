/**
  ******************************************************************************
  * @file    stm32l4xx.h
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for STM32L4xx devices.            
  *            
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The device used in the target application
  *              - To use or not the peripheral’s drivers in application code(i.e. 
  *                code will be based on direct access to peripheral’s registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_STDPERIPH_DRIVER"
  *              - To change few application-specific parameters such as the HSE 
  *                crystal frequency
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *           - Macros to access peripheral’s registers hardware
  *  
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l4xx
  * @{
  */
    
#ifndef __STM32L4xx_H
#define __STM32L4xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
  
/** @addtogroup Library_configuration_section
  * @{
  */
  
/* Uncomment the line below according to the target STM32 device used in your
   application 
  */

#if !defined (STM32L4XX) 
  #define STM32L4XX  
#endif


/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */

#if !defined (STM32L4XX) 
 #error "Please select first the target STM32L4XX device used in your application (in stm32l4xx.h file)"
#endif

#if !defined  (USE_STDPERIPH_DRIVER)
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will 
   be based on direct access to peripherals registers 
   */
  /*#define USE_STDPERIPH_DRIVER */
#endif /* USE_STDPERIPH_DRIVER */

/**
 * @brief In the following line adjust the value of External High Speed oscillator (HSE)
   used in your application 
   
   Tip: To avoid modifying this file each time you need to use different HSE, you
        can define the HSE value in your toolchain compiler preprocessor.
  */
#if !defined  (HSE_VALUE)
#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif

/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup 
   Timeout value 
   */
#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT   ((uint16_t)0x0500) /*!< Time out for HSE start up */
#endif

/**
 * @brief In the following line adjust the Internal High Speed oscillator (HSI) Startup 
   Timeout value 
   */
#if !defined  (HSI_STARTUP_TIMEOUT)
#define HSI_STARTUP_TIMEOUT   ((uint16_t)0x0500) /*!< Time out for HSI start up */
#endif

#if !defined  (HSI_VALUE)
#define HSI_VALUE  ((uint32_t)16000000) /*!< Value of the Internal High Speed oscillator in Hz.
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
#endif

#if !defined  (LSI_VALUE)
#define LSI_VALUE  ((uint32_t)37000)    /*!< Value of the Internal Low Speed oscillator in Hz
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
#endif

#if !defined  (LSE_VALUE)
#define LSE_VALUE  ((uint32_t)32768)    /*!< Value of the External Low Speed oscillator in Hz */
#endif

     

/**
 * @brief STM32L4XX Standard Peripherals Library version number V1.2.0CR1
   */
#define __STM32L4XX_STDPERIPH_VERSION_MAIN   (0x01) /*!< [31:24] main version */                                  
#define __STM32L4XX_STDPERIPH_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __STM32L4XX_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32L4XX_STDPERIPH_VERSION_RC     (0x01) /*!< [7:0]  release candidate */ 
#define __STM32L4XX_STDPERIPH_VERSION        ((__STM32L4XX_STDPERIPH_VERSION_MAIN << 24)\
                                             |(__STM32L4XX_STDPERIPH_VERSION_SUB1 << 16)\
                                             |(__STM32L4XX_STDPERIPH_VERSION_SUB2 << 8)\
                                             |(__STM32L4XX_STDPERIPH_VERSION_RC))
                                             
/**
  * @}
  */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
 * @brief Configuration of the Cortex-M4 Processor and Core Peripherals 
 */
#define __CM4_REV                 0x0001  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1       /*!< STM32L4XX provides an MPU                     */
#define __NVIC_PRIO_BITS          4       /*!< STM32L4XX uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1       /*!< FPU present                                   */

/**
 * @brief STM32L4XX Interrupt Number Definition, according to the selected device 
 *        in @ref Library_configuration_section 
 */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  HardFault_IRQn              = -13,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                                    */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                                    */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                                    */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                                    */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                                    */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                                    */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                                    */
  ADC1_2_IRQn                 = 18,      /*!< ADC1, ADC2 SAR global Interrupts                                  */
  CAN_TX_IRQn                = 19,       /*!< CAN TX Interrupt                                                 */
  CAN_RX0_IRQn               = 20,       /*!< CAN RX0 Interrupt                                                */
  CAN_RX1_IRQn               = 21,       /*!< CAN RX1 Interrupt                                                */
  CAN_SCE_IRQn               = 22,       /*!< CAN SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break interrupt and TIM15 global interrupt                    */
  TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM16 global interrupt                  */
  TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM17 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */  
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  DFSDM4_IRQn                 = 42,     /*!< SD Filter 4 global Interrupt                                      */
  TIM8_BRK_IRQn               = 43,     /*!< TIM8 Break Interrupt                                              */
  TIM8_IRQn                   = 44,     /*!< TIM8 Update Interrupt                                             */
  TIM8_TRG_COM_IRQn           = 45,     /*!< TIM8 Trigger and Commutation Interrupt                            */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  ADC3_IRQn                   = 47,     /*!< ADC3 global  Interrupt                                            */
  FMC_IRQn                    = 48,     /*!< FMC global Interrupt                                             */
  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                                   */
  DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                                   */
  DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                                   */
  DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                                   */
  DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                                   */
  DFSDM1_IRQn                 = 61,     /*!< SD Filter 1 global Interrupt                                      */
  DFSDM2_IRQn                 = 62,     /*!< SD Filter 2 global Interrupt                                      */
  DFSDM3_IRQn                 = 63,     /*!< SD Filter 3 global Interrupt                                      */
  COMP_IRQn                   = 64,     /*!< COMP Interrupt                                                    */
  LPTIM1_IRQn                 = 65,     /*!< LP TIM1 interrupt                                                 */
  LPTIM2_IRQn                 = 66,     /*!< LP TIM2 interrupt                                                 */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Channel6_IRQn          = 68,     /*!< DMA2 Channel 6 global interrupt                                   */
  DMA2_Channel7_IRQn          = 69,     /*!< DMA2 Channel 7 global interrupt                                   */
  LPUART_IRQn                 = 70,     /*!< LP UART interrupt                                                 */
  QUADSPI_IRQn                = 71,     /*!< Quad SPI global interrupt                                         */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  SAI1_IRQn                   = 74,     /*!< Serial Audio Interface 1 global interrupt                         */
  SAI2_IRQn                   = 75,     /*!< Serial Audio Interface 2 global interrupt                         */
  SWP_IRQn                    = 76,     /*!< Serial Wire Interface global interrupt                            */
  TSC_IRQn                    = 77,     /*!< Touch Sense Controller global interrupt                           */
  LCD_IRQn                    = 78,     /*!< LCD global interrupt                                              */
  AES_IRQn                   = 79,      /*!< AES global interrupt                                      */
  HASH_RNG_IRQn               = 80,     /*!< Hash and Rng global interrupt                                     */
  FPU_IRQn                    = 81,     /*!< FPU global interrupt                                              */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#include "system_stm32l4xx.h"
#include <stdint.h>

/** @addtogroup Exported_types
  * @{
  */  
/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/**
  * @}
  */

/** @addtogroup Peripheral_registers_structures
  * @{
  */   

/** 
  * @brief Analog to Digital Converter  
  */

typedef struct
{
  __IO uint32_t ISR;              /*!< ADC Interrupt and Status Register,                 Address offset: 0x00 */
  __IO uint32_t IER;              /*!< ADC Interrupt Enable Register,                     Address offset: 0x04 */
  __IO uint32_t CR;               /*!< ADC control register,                              Address offset: 0x08 */
  __IO uint32_t CFGR;             /*!< ADC Configuration register,                        Address offset: 0x0C */
  __IO uint32_t CFGR2;            /*!< ADC Configuration register 2,                      Address offset: 0x10 */
  __IO uint32_t SMPR1;            /*!< ADC sample time register 1,                        Address offset: 0x14 */
  __IO uint32_t SMPR2;            /*!< ADC sample time register 2,                        Address offset: 0x18 */
  uint32_t      RESERVED1;        /*!< Reserved, 0x01C                                                         */
  __IO uint32_t TR1;              /*!< ADC watchdog threshold register 1,                 Address offset: 0x20 */
  __IO uint32_t TR2;              /*!< ADC watchdog threshold register 2,                 Address offset: 0x24 */
  __IO uint32_t TR3;              /*!< ADC watchdog threshold register 3,                 Address offset: 0x28 */
  uint32_t      RESERVED2;        /*!< Reserved, 0x02C                                                         */
  __IO uint32_t SQR1;             /*!< ADC regular sequence register 1,                   Address offset: 0x30 */
  __IO uint32_t SQR2;             /*!< ADC regular sequence register 2,                   Address offset: 0x34 */
  __IO uint32_t SQR3;             /*!< ADC regular sequence register 3,                   Address offset: 0x38 */
  __IO uint32_t SQR4;             /*!< ADC regular sequence register 4,                   Address offset: 0x3C */
  __IO uint32_t DR;               /*!< ADC regular data register,                         Address offset: 0x40 */
  uint32_t      RESERVED3;        /*!< Reserved, 0x044                                                         */
  uint32_t      RESERVED4;        /*!< Reserved, 0x048                                                         */
  __IO uint32_t JSQR;             /*!< ADC injected sequence register,                    Address offset: 0x4C */
  uint32_t      RESERVED5[4];     /*!< Reserved, 0x050 - 0x05C                                                 */
  __IO uint32_t OFR1;             /*!< ADC offset register 1,                             Address offset: 0x60 */
  __IO uint32_t OFR2;             /*!< ADC offset register 2,                             Address offset: 0x64 */
  __IO uint32_t OFR3;             /*!< ADC offset register 3,                             Address offset: 0x68 */
  __IO uint32_t OFR4;             /*!< ADC offset register 4,                             Address offset: 0x6C */
  uint32_t      RESERVED6[4];     /*!< Reserved, 0x070 - 0x07C                                                 */
  __IO uint32_t JDR1;             /*!< ADC injected data register 1,                      Address offset: 0x80 */
  __IO uint32_t JDR2;             /*!< ADC injected data register 2,                      Address offset: 0x84 */
  __IO uint32_t JDR3;             /*!< ADC injected data register 3,                      Address offset: 0x88 */
  __IO uint32_t JDR4;             /*!< ADC injected data register 4,                      Address offset: 0x8C */
  uint32_t      RESERVED7[4];     /*!< Reserved, 0x090 - 0x09C                                                 */
  __IO uint32_t AWD2CR;           /*!< ADC  Analog Watchdog 2 Configuration Register,     Address offset: 0xA0 */
  __IO uint32_t AWD3CR;           /*!< ADC  Analog Watchdog 3 Configuration Register,     Address offset: 0xA4 */
  uint32_t      RESERVED8;        /*!< Reserved, 0x0A8                                                         */
  uint32_t      RESERVED9;        /*!< Reserved, 0x0AC                                                         */
  __IO uint32_t DIFSEL;           /*!< ADC  Differential Mode Selection Register,         Address offset: 0xB0 */
  __IO uint32_t CALFACT;          /*!< ADC  Calibration Factors,                          Address offset: 0xB4 */

} ADC_TypeDef;

typedef struct
{
  __IO uint32_t CSR;            /*!< ADC Common status register,                  Address offset: ADC1/3 base address + 0x300 */
  uint32_t      RESERVED;       /*!< Reserved, ADC1/3 base address + 0x304                                                    */
  __IO uint32_t CCR;            /*!< ADC common control register,                 Address offset: ADC1/3 base address + 0x308 */
  __IO uint32_t CDR;            /*!< ADC common regular data register for dual    Address offset: ADC1/3 base address + 0x30C */
} ADC_Common_TypeDef;


/** 
  * @brief Controller Area Network TxMailBox 
  */

typedef struct
{
  __IO uint32_t TIR;  /*!< CAN TX mailbox identifier register */
  __IO uint32_t TDTR; /*!< CAN mailbox data length control and time stamp register */
  __IO uint32_t TDLR; /*!< CAN mailbox data low register */
  __IO uint32_t TDHR; /*!< CAN mailbox data high register */
} CAN_TxMailBox_TypeDef;

/** 
  * @brief Controller Area Network FIFOMailBox 
  */
  
typedef struct
{
  __IO uint32_t RIR;  /*!< CAN receive FIFO mailbox identifier register */
  __IO uint32_t RDTR; /*!< CAN receive FIFO mailbox data length control and time stamp register */
  __IO uint32_t RDLR; /*!< CAN receive FIFO mailbox data low register */
  __IO uint32_t RDHR; /*!< CAN receive FIFO mailbox data high register */
} CAN_FIFOMailBox_TypeDef;

/** 
  * @brief Controller Area Network FilterRegister 
  */
  
typedef struct
{
  __IO uint32_t FR1; /*!< CAN Filter bank register 1 */
  __IO uint32_t FR2; /*!< CAN Filter bank register 1 */
} CAN_FilterRegister_TypeDef;

/** 
  * @brief Controller Area Network 
  */
  
typedef struct
{
  __IO uint32_t              MCR;                 /*!< CAN master control register,         Address offset: 0x00          */
  __IO uint32_t              MSR;                 /*!< CAN master status register,          Address offset: 0x04          */
  __IO uint32_t              TSR;                 /*!< CAN transmit status register,        Address offset: 0x08          */
  __IO uint32_t              RF0R;                /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
  __IO uint32_t              RF1R;                /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
  __IO uint32_t              IER;                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
  __IO uint32_t              ESR;                 /*!< CAN error status register,           Address offset: 0x18          */
  __IO uint32_t              BTR;                 /*!< CAN bit timing register,             Address offset: 0x1C          */
  uint32_t                   RESERVED0[88];       /*!< Reserved, 0x020 - 0x17F                                            */
  CAN_TxMailBox_TypeDef      sTxMailBox[3];       /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
  CAN_FIFOMailBox_TypeDef    sFIFOMailBox[2];     /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
  uint32_t                   RESERVED1[12];       /*!< Reserved, 0x1D0 - 0x1FF                                            */
  __IO uint32_t              FMR;                 /*!< CAN filter master register,          Address offset: 0x200         */
  __IO uint32_t              FM1R;                /*!< CAN filter mode register,            Address offset: 0x204         */
  uint32_t                   RESERVED2;           /*!< Reserved, 0x208                                                    */
  __IO uint32_t              FS1R;                /*!< CAN filter scale register,           Address offset: 0x20C         */
  uint32_t                   RESERVED3;           /*!< Reserved, 0x210                                                    */
  __IO uint32_t              FFA1R;               /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
  uint32_t                   RESERVED4;           /*!< Reserved, 0x218                                                    */
  __IO uint32_t              FA1R;                /*!< CAN filter activation register,      Address offset: 0x21C         */
  uint32_t                   RESERVED5[8];        /*!< Reserved, 0x220-0x23F                                              */ 
  CAN_FilterRegister_TypeDef sFilterRegister[28]; /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
} CAN_TypeDef;


/** 
  * @brief CRC calculation unit 
  */

typedef struct
{
  __IO uint32_t DR;          /*!< CRC Data register,                           Address offset: 0x00 */
  __IO uint8_t  IDR;         /*!< CRC Independent data register,               Address offset: 0x04 */
  uint8_t       RESERVED0;   /*!< Reserved,                                                    0x05 */
  uint16_t      RESERVED1;   /*!< Reserved,                                                    0x06 */
  __IO uint32_t CR;          /*!< CRC Control register,                        Address offset: 0x08 */
  uint32_t      RESERVED2;   /*!< Reserved,                                                    0x0C */
  __IO uint32_t INIT;        /*!< Initial CRC value register,                  Address offset: 0x10 */
  __IO uint32_t POL;         /*!< CRC polynomial register,                     Address offset: 0x14 */
} CRC_TypeDef;

/** 
  * @brief Digital to Analog Converter
  */

typedef struct
{
  __IO uint32_t CR;       /*!< DAC control register,                                    Address offset: 0x00 */
  __IO uint32_t SWTRIGR;  /*!< DAC software trigger register,                           Address offset: 0x04 */
  __IO uint32_t DHR12R1;  /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
  __IO uint32_t DHR12L1;  /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
  __IO uint32_t DHR8R1;   /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
  __IO uint32_t DHR12R2;  /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
  __IO uint32_t DHR12L2;  /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
  __IO uint32_t DHR8R2;   /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
  __IO uint32_t DHR12RD;  /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
  __IO uint32_t DHR12LD;  /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
  __IO uint32_t DHR8RD;   /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
  __IO uint32_t DOR1;     /*!< DAC channel1 data output register,                       Address offset: 0x2C */
  __IO uint32_t DOR2;     /*!< DAC channel2 data output register,                       Address offset: 0x30 */
  __IO uint32_t SR;       /*!< DAC status register,                                     Address offset: 0x34 */
  __IO uint32_t CCR;      /*!< DAC calibration control register,                        Address offset: 0x38 */
  __IO uint32_t MCR;      /*!< DAC mode control register,                               Address offset: 0x3C */
  __IO uint32_t SHSR1;    /*!< DAC Sample and Hold sample time register 1,              Address offset: 0x40 */
  __IO uint32_t SHSR2;    /*!< DAC Sample and Hold sample time register 2,              Address offset: 0x44 */
  __IO uint32_t SHHR;     /*!< DAC Sample and Hold hold time register,                  Address offset: 0x48 */
  __IO uint32_t SHRR;     /*!< DAC Sample and Hold refresh time register,               Address offset: 0x4C */
} DAC_TypeDef;

/**
  * @brief DFSDM module registers
  */
typedef struct
{
  __IO uint32_t CR1;          /*!< DFSDM control register1,                          Address offset: 0x100 */
  __IO uint32_t CR2;          /*!< DFSDM control register2,                          Address offset: 0x104 */
  __IO uint32_t ISR;          /*!< DFSDM interrupt and status register,              Address offset: 0x108 */
  __IO uint32_t ICR;          /*!< DFSDM interrupt flag clear register,              Address offset: 0x10C */
  __IO uint32_t JCHGR;        /*!< DFSDM injected channel group selection register,  Address offset: 0x110 */
  __IO uint32_t FCR;          /*!< DFSDM filter control register,                    Address offset: 0x114 */
  __IO uint32_t JDATAR;       /*!< DFSDM data register for injected group,           Address offset: 0x118 */
  __IO uint32_t RDATAR;       /*!< DFSDM data register for regular group,            Address offset: 0x11C */
  __IO uint32_t AWHTR;        /*!< DFSDM analog watchdog high threshold register,    Address offset: 0x120 */
  __IO uint32_t AWLTR;        /*!< DFSDM analog watchdog low threshold register,     Address offset: 0x124 */
  __IO uint32_t AWSR;         /*!< DFSDM analog watchdog status register             Address offset: 0x128 */
  __IO uint32_t AWCFR;        /*!< DFSDM analog watchdog clear flag register         Address offset: 0x12C */
  __IO uint32_t EXMAX;        /*!< DFSDM extreme detector maximum register,          Address offset: 0x130 */
  __IO uint32_t EXMIN;        /*!< DFSDM extreme detector minimum register           Address offset: 0x134 */
  __IO uint32_t CNVTIMR;      /*!< DFSDM conversion timer,                           Address offset: 0x138 */
} DFSDM_TypeDef;

/**
  * @brief DFSDM channel configuration registers
  */
typedef struct
{
  __IO uint32_t CHCFGR1;      /*!< DFSDM channel configuration register1,            Address offset: 0x00 */
  __IO uint32_t CHCFGR2;      /*!< DFSDM channel configuration register2,            Address offset: 0x04 */
  __IO uint32_t AWSCDR;       /*!< DFSDM channel analog watchdog and
                                   short circuit detector register,                  Address offset: 0x08 */
  __IO uint32_t CHWDATAR;     /*!< DFSDM channel watchdog filter data register,      Address offset: 0x0C */
  __IO uint32_t CHDATINR;     /*!< DFSDM channel data input register,                Address offset: 0x10 */
} DFSDM_Channel_TypeDef;


/** 
  * @brief Debug MCU
  */

typedef struct
{
  __IO uint32_t IDCODE;  /*!< MCU device ID code,                 Address offset: 0x00 */
  __IO uint32_t CR;      /*!< Debug MCU configuration register,   Address offset: 0x04 */
  __IO uint32_t APB1LFZ; /*!< Debug MCU APB1L freeze register,    Address offset: 0x08 */
  __IO uint32_t APB1HFZ; /*!< Debug MCU APB1H freeze register,    Address offset: 0x0C */
  __IO uint32_t APB2FZ;  /*!< Debug MCU APB2 freeze register,     Address offset: 0x10 */
}DBGMCU_TypeDef;



/** 
  * @brief DMA Controller
  */

typedef struct
{
  __IO uint32_t CCR;          /*!< DMA channel x configuration register        */
  __IO uint32_t CNDTR;        /*!< DMA channel x number of data register       */
  __IO uint32_t CPAR;         /*!< DMA channel x peripheral address register   */
  __IO uint32_t CMAR;         /*!< DMA channel x memory address register       */
} DMA_Channel_TypeDef;

typedef struct
{
  __IO uint32_t ISR;          /*!< DMA interrupt status register,               Address offset: 0x00 */
  __IO uint32_t IFCR;         /*!< DMA interrupt flag clear register,           Address offset: 0x04 */   
} DMA_TypeDef;

typedef struct
{
  __IO uint32_t CSELR;           /*!< DMA option register,                  Address offset: 0x00 */
} DMA_request_TypeDef;


/** 
  * @brief External Interrupt/Event Controller
  */

typedef struct
{
  __IO uint32_t IMR;        /*!< EXTI Interrupt mask register,                Address offset: 0x00 */
  __IO uint32_t EMR;        /*!< EXTI Event mask register,                    Address offset: 0x04 */
  __IO uint32_t RTSR;       /*!< EXTI Rising trigger selection register,      Address offset: 0x08 */
  __IO uint32_t FTSR;       /*!< EXTI Falling trigger selection register,     Address offset: 0x0C */
  __IO uint32_t SWIER;      /*!< EXTI Software interrupt event register,      Address offset: 0x10 */
  __IO uint32_t PR;         /*!< EXTI Pending register,                       Address offset: 0x14 */
  uint32_t      RESERVED1;  /*!< Reserved, 0x18                                                    */
  uint32_t      RESERVED2;  /*!< Reserved, 0x1C                                                    */
  __IO uint32_t IMR2;       /*!< EXTI Interrupt mask register,                Address offset: 0x20 */
  __IO uint32_t EMR2;       /*!< EXTI Event mask register,                    Address offset: 0x24 */
  __IO uint32_t RTSR2;      /*!< EXTI Rising trigger selection register,      Address offset: 0x28 */
  __IO uint32_t FTSR2;      /*!< EXTI Falling trigger selection register,     Address offset: 0x2C */
  __IO uint32_t SWIER2;     /*!< EXTI Software interrupt event register,      Address offset: 0x30 */
  __IO uint32_t PR2;        /*!< EXTI Pending register,                       Address offset: 0x34 */
}EXTI_TypeDef;


/** 
  * @brief FLASH Registers
  */

typedef struct
{
  __IO uint32_t ACR;              /*!< FLASH access control register,    Address offset: 0x00 */
  __IO uint32_t PDKEYR;           /*!< FLASH power down key register,    Address offset: 0x04 */
  __IO uint32_t KEYR;             /*!< FLASH key register,               Address offset: 0x08 */
  __IO uint32_t OPTKEYR;          /*!< FLASH option key register,        Address offset: 0x0C */
  __IO uint32_t SR;               /*!< FLASH status register,            Address offset: 0x10 */
  __IO uint32_t CR;               /*!< FLASH control register,           Address offset: 0x14 */
  __IO uint32_t ECCR;             /*!< FLASH option control register 1,  Address offset: 0x18 */
  __IO uint32_t RESERVED1;        /*!< Reserved1,                        Address offset: 0x1C */
  __IO uint32_t OPTCR;            /*!< FLASH option control register ,   Address offset: 0x20 */
  __IO uint32_t PCROP1SR;         /*!< FLASH start PCROP register bank1, Address offset: 0x24 */
  __IO uint32_t PCROP1ER;         /*!< FLASH end PCROP register bank1,   Address offset: 0x28 */
  __IO uint32_t WRPA1R;           /*!< FLASH WRPA register bank1,        Address offset: 0x2C */
  __IO uint32_t WRPB1R;           /*!< FLASH WRPB register bank1,        Address offset: 0x30 */
       uint32_t RESERVED2[4];     /*!< Reserved2,                        Address offset: 0x34 */
  __IO uint32_t PCROP2SR;         /*!< FLASH start PCROP register bank2, Address offset: 0x44 */
  __IO uint32_t PCROP2ER;         /*!< FLASH end PCROP register bank2,   Address offset: 0x48 */
  __IO uint32_t WRPA2R;           /*!< FLASH WRPA register bank2,        Address offset: 0x4C */
  __IO uint32_t WRPB2R;           /*!< FLASH WRPB register bank2,        Address offset: 0x50 */
} FLASH_TypeDef;



/** 
  * @brief Flexible Memory Controller
  */

typedef struct
{
  __IO uint32_t BTCR[8];    /*!< NOR/PSRAM chip-select control register(BCR) and chip-select timing register(BTR), Address offset: 0x00-1C */   
} FMC_Bank1_TypeDef; 

/** 
  * @brief Flexible Memory Controller Bank1E
  */
  
typedef struct
{
  __IO uint32_t BWTR[7];    /*!< NOR/PSRAM write timing registers, Address offset: 0x104-0x11C */
} FMC_Bank1E_TypeDef;

/** 
  * @brief Flexible Memory Controller Bank2
  */
  
typedef struct
{
  __IO uint32_t PCR2;       /*!< NAND Flash control register 2,                       Address offset: 0x60 */
  __IO uint32_t SR2;        /*!< NAND Flash FIFO status and interrupt register 2,     Address offset: 0x64 */
  __IO uint32_t PMEM2;      /*!< NAND Flash Common memory space timing register 2,    Address offset: 0x68 */
  __IO uint32_t PATT2;      /*!< NAND Flash Attribute memory space timing register 2, Address offset: 0x6C */
  uint32_t      RESERVED0;  /*!< Reserved, 0x70                                                            */
  __IO uint32_t ECCR2;      /*!< NAND Flash ECC result registers 2,                   Address offset: 0x74 */
} FMC_Bank2_TypeDef;

/** 
  * @brief Flexible Memory Controller Bank3
  */
  
typedef struct
{
  __IO uint32_t PCR3;       /*!< NAND Flash control register 3,                       Address offset: 0x80 */
  __IO uint32_t SR3;        /*!< NAND Flash FIFO status and interrupt register 3,     Address offset: 0x84 */
  __IO uint32_t PMEM3;      /*!< NAND Flash Common memory space timing register 3,    Address offset: 0x88 */
  __IO uint32_t PATT3;      /*!< NAND Flash Attribute memory space timing register 3, Address offset: 0x8C */
  uint32_t      RESERVED0;  /*!< Reserved, 0x90                                                            */
  __IO uint32_t ECCR3;      /*!< NAND Flash ECC result registers 3,                   Address offset: 0x94 */
} FMC_Bank3_TypeDef;

/** 
  * @brief Flexible Memory Controller Bank4
  */
  
typedef struct
{
  __IO uint32_t PCR4;       /*!< PC Card  control register 4,                       Address offset: 0xA0 */
  __IO uint32_t SR4;        /*!< PC Card  FIFO status and interrupt register 4,     Address offset: 0xA4 */
  __IO uint32_t PMEM4;      /*!< PC Card  Common memory space timing register 4,    Address offset: 0xA8 */
  __IO uint32_t PATT4;      /*!< PC Card  Attribute memory space timing register 4, Address offset: 0xAC */
  __IO uint32_t PIO4;       /*!< PC Card  I/O space timing register 4,              Address offset: 0xB0 */
} FMC_Bank4_TypeDef; 



/** 
  * @brief General Purpose I/O
  */

typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
  __IO uint32_t BRR;     /*!< GPIO Bit Reset register,                Address offset: 0x28       */
  __IO uint32_t RLAR;     /*!< GPIO reduce leakage on analog register, Address offset: 0x2C     */

} GPIO_TypeDef;

/** 
  * @brief Operational Amplifier (OPAMP)
  */

typedef struct
{
  __IO uint32_t CSR;          /*!< OPAMP control/status register,                     Address offset: 0x00 */
  __IO uint32_t OTR;          /*!< OPAMP offset trimming register for normal mode,    Address offset: 0x04 */ 
  __IO uint32_t LPOTR;        /*!< OPAMP offset trimming register for low power mode, Address offset: 0x08 */
} OPAMP_TypeDef;

/** 
  * @brief System configuration controller
  */
  
typedef struct
{
  __IO uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
  __IO uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
  __IO uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
  __IO uint32_t GPC;          /*!< SYSCFG General Purpose Configuration register      Address offset: 0x18      */
  __IO uint32_t CBR;          /*!< SYSCFG Class B register                            Address offset: 0x1C      */
  __IO uint32_t SWP;          /*!< SYSCFG SRAM2 Write Protection register,            Address offset: 0x20      */
  __IO uint32_t SKR;          /*!< SYSCFG SRAM2 Key Register register,                Address offset: 0x24      */
   uint32_t RESERVED0[2];     /*!< Reserved0,                                         Address offset: 0x24-0x2C */ 
  __IO uint32_t VCSR;         /*!< VREF_BUF Control and Status register               Address offset: 0x30      */
  __IO uint32_t VCCR;         /*!< VREF_BUF Calibration Control register              Address offset: 0x34      */
 
} SYSCFG_TypeDef;

/** 
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
  __IO uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */  
  __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */  
} I2C_TypeDef;

/** 
  * @brief Independent WATCHDOG
  */

typedef struct
{
  __IO uint32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  __IO uint32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  __IO uint32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  __IO uint32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  __IO uint32_t WINR; /*!< IWDG Window register,    Address offset: 0x10 */
} IWDG_TypeDef;

/** 
  * @brief LCD
  */

typedef struct
{
  __IO uint32_t CR;        /*!< LCD control register,              Address offset: 0x00 */
  __IO uint32_t FCR;       /*!< LCD frame control register,        Address offset: 0x04 */
  __IO uint32_t SR;        /*!< LCD status register,               Address offset: 0x08 */
  __IO uint32_t CLR;       /*!< LCD clear register,                Address offset: 0x0C */
  uint32_t RESERVED;       /*!< Reserved,                          Address offset: 0x10 */
  __IO uint32_t RAM[16];   /*!< LCD display memory,           Address offset: 0x14-0x50 */
} LCD_TypeDef;


/** 
  * @brief Firewall
  */

typedef struct
{
  __IO uint32_t CSSA;     /*!< Code Segment Start Address register,              Address offset: 0x00 */
  __IO uint32_t CSL;      /*!< Code Segment Length register,                      Address offset: 0x04 */
  __IO uint32_t NVDSSA;   /*!< NON volatile data Segment Start Address register,  Address offset: 0x08 */
  __IO uint32_t NVDSL;    /*!< NON volatile data Segment Length register,         Address offset: 0x0C */
  __IO uint32_t VDSSA ;   /*!< Volatile data Segment Start Address register,      Address offset: 0x10 */
  __IO uint32_t VDSL ;    /*!< Volatile data Segment Length register,             Address offset: 0x14 */
  __IO uint32_t LSSA ;    /*!< Library Segment Start Address register,            Address offset: 0x18 */
  __IO uint32_t LSL ;     /*!< Library Segment Length register,                   Address offset: 0x1C */
  __IO uint32_t CR ;      /*!< Configuration  register,                           Address offset: 0x20 */
 
} FIREWALL_TypeDef;




/** 
  * @brief Power Control
  */

typedef struct
{
  __IO uint32_t CR1;   /*!< PWR power control register 1,        Address offset: 0x00 */
  __IO uint32_t CR2;   /*!< PWR power control register 2,        Address offset: 0x04 */
  __IO uint32_t CR3;   /*!< PWR power control register 3,        Address offset: 0x08 */
  __IO uint32_t CR4;   /*!< PWR power control register 4,        Address offset: 0x0C */
  __IO uint32_t SR1;   /*!< PWR power status register 1,         Address offset: 0x10 */
  __IO uint32_t SR2;   /*!< PWR power status register 2,         Address offset: 0x14 */
  __IO uint32_t SRR;   /*!< PWR power status reset register,     Address offset: 0x18 */
    uint32_t RESERVED;       /*!< Reserved,                      Address offset: 0x1C */
  __IO uint32_t PUCRA; /*!< Pull_up control register of portA,   Address offset: 0x20 */
  __IO uint32_t PDCRA; /*!< Pull_Down control register of portA, Address offset: 0x24 */
  __IO uint32_t PUCRB; /*!< Pull_up control register of portB,   Address offset: 0x28 */
  __IO uint32_t PDCRB; /*!< Pull_Down control register of portB, Address offset: 0x2C */
  __IO uint32_t PUCRC; /*!< Pull_up control register of portC,   Address offset: 0x30 */
  __IO uint32_t PDCRC; /*!< Pull_Down control register of portC, Address offset: 0x34 */
  __IO uint32_t PUCRD; /*!< Pull_up control register of portD,   Address offset: 0x38 */
  __IO uint32_t PDCRD; /*!< Pull_Down control register of portD, Address offset: 0x3C */
  __IO uint32_t PUCRE; /*!< Pull_up control register of portE,   Address offset: 0x40 */
  __IO uint32_t PDCRE; /*!< Pull_Down control register of portE, Address offset: 0x44 */
  __IO uint32_t PUCRF; /*!< Pull_up control register of portF,   Address offset: 0x48 */
  __IO uint32_t PDCRF; /*!< Pull_Down control register of portF, Address offset: 0x4C */
  __IO uint32_t PUCRG; /*!< Pull_up control register of portG,   Address offset: 0x50 */
  __IO uint32_t PDCRG; /*!< Pull_Down control register of portG, Address offset: 0x54 */
  __IO uint32_t PUCRH; /*!< Pull_up control register of portH,   Address offset: 0x58 */
  __IO uint32_t PDCRH; /*!< Pull_Down control register of portH, Address offset: 0x5C */

} PWR_TypeDef;

/** 
  * @brief Reset and Clock Control
  */

typedef struct
{ 
  __IO uint32_t CR;             /*!< RCC clock control register,                                              Address offset: 0x00 */
  __IO uint32_t ICSCR;          /*!< RCC Internal Clock Sources Calibration Register,                         Address offset: 0x04 */
  __IO uint32_t CFGR;           /*!< RCC clock configuration register,                                        Address offset: 0x08 */
  __IO uint32_t PLLCFGR;     /*!< RCC System PLL configuration register,                                      Address offset: 0x0C */
  __IO uint32_t PLLSAI1CFGR;    /*!< RCC PLL SAI1 Configuration Register,                                     Address offset: 0x10 */
  __IO uint32_t PLLSAI2CFGR;    /*!< RCC PLL SAI2 Configuration Register,                                     Address offset: 0x14 */
  __IO uint32_t CIER;           /*!< RCC Clock Interrupt Enable Register,                                     Address offset: 0x18 */
  __IO uint32_t CIFR;           /*!< RCC Clock Interrupt Flag Register,                                       Address offset: 0x1C */
  __IO uint32_t CICR;           /*!< RCC Clock Interrupt Clear Register,                                      Address offset: 0x20 */
  uint32_t      RESERVED0;      /*!< Reserved,                                                                Address offset: 0x24 */
  __IO uint32_t AHB1RSTR;       /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x28 */
  __IO uint32_t AHB2RSTR;       /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x2C */
  __IO uint32_t AHB3RSTR;       /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x30 */
  uint32_t      RESERVED1;      /*!< Reserved,                                                                Address offset: 0x34 */
  __IO uint32_t APB1LRSTR;      /*!< RCC LowSpeed APB1 macrocells resets Low Word,                            Address offset: 0x38 */
  __IO uint32_t APB1HRSTR;      /*!< RCC LowSpeed APB1 macrocells resets High Word,                           Address offset: 0x3C */
  __IO uint32_t APB2RSTR;       /*!< RCC High Speed APB macrocells resets,                                    Address offset: 0x40 */
  uint32_t      RESERVED2;      /*!< Reserved,                                                                Address offset: 0x44 */
  __IO uint32_t AHB1ENR;        /*!< RCC AHB1 peripheral clock enable register,                               Address offset: 0x48 */
  __IO uint32_t AHB2ENR;        /*!< RCC AHB2 peripheral clock enable register,                               Address offset: 0x4C */
  __IO uint32_t AHB3ENR;        /*!< RCC AHB3 peripheral clock enable register,                               Address offset: 0x50 */
  uint32_t      RESERVED3;      /*!< Reserved,                                                                Address offset: 0x54 */
  __IO uint32_t APB1LENR;       /*!< RCC LowSpeed APB1 macrocells clock enables Low Word,                     Address offset: 0x58 */
  __IO uint32_t APB1HENR;       /*!< RCC LowSpeed APB1 macrocells clock enables High Word,                    Address offset: 0x5C */
  __IO uint32_t APB2ENR;        /*!< RCC High Speed APB macrocells clock enabled,                             Address offset: 0x60 */
  uint32_t      RESERVED4;      /*!< Reserved,                                                                Address offset: 0x64 */
  __IO uint32_t AHB1LPENR;      /*!< RCC AHB1 macrocells clocks enables in sleep mode,                        Address offset: 0x60 */
  __IO uint32_t AHB2LPENR;      /*!< RCC AHB2 macrocells clock enables in sleep mode,                         Address offset: 0x64 */
  __IO uint32_t AHB3LPENR;      /*!< RCC AHB3 macrocells clock enables in sleep mode,                         Address offset: 0x70 */
  uint32_t      RESERVED5;      /*!< Reserved,                                                                Address offset: 0x74 */
  __IO uint32_t APB1LLPENR;     /*!< RCC LowSpeed APB1 macrocells clock enables in sleep mode Low Word,       Address offset: 0x78 */
  __IO uint32_t APB1HLPENR;     /*!< RCC LowSpeed APB1 macrocells clock enables in sleep mode High Word,      Address offset: 0x7C */
  __IO uint32_t APB2LPENR;      /*!< RCC High Speed APB macrocells clock enabled in sleep mode,               Address offset: 0x80 */
  uint32_t      RESERVED6;      /*!< Reserved,                                                                Address offset: 0x84 */
  __IO uint32_t CCIPR;          /*!< RCC IPs Clocks Configuration Register,                                   Address offset: 0x88 */
  __IO uint32_t RESERVED7;      /*!< Reserved,                                                                Address offset: 0x8C */
  __IO uint32_t BDCR;           /*!< RCC Vswitch Backup Domain Control Register,                              Address offset: 0x90 */
  __IO uint32_t CSR;            /*!< RCC clock control & status register,                                     Address offset: 0x94 */

} RCC_TypeDef;


/** 
  * @brief Real-Time Clock
  */

typedef struct
{
  __IO uint32_t TR;         /*!< RTC time register,                                         Address offset: 0x00 */
  __IO uint32_t DR;         /*!< RTC date register,                                         Address offset: 0x04 */
  __IO uint32_t CR;         /*!< RTC control register,                                      Address offset: 0x08 */                                                                                            
  __IO uint32_t ISR;        /*!< RTC initialization and status register,                    Address offset: 0x0C */
  __IO uint32_t PRER;       /*!< RTC prescaler register,                                    Address offset: 0x10 */
  __IO uint32_t WUTR;       /*!< RTC wakeup timer register,                                 Address offset: 0x14 */
       uint32_t reserved;   /*!< Reserved  */
  __IO uint32_t ALRMAR;     /*!< RTC alarm A register,                                      Address offset: 0x1C */
  __IO uint32_t ALRMBR;     /*!< RTC alarm B register,                                      Address offset: 0x20 */
  __IO uint32_t WPR;        /*!< RTC write protection register,                             Address offset: 0x24 */
  __IO uint32_t SSR;        /*!< RTC sub second register,                                   Address offset: 0x28 */
  __IO uint32_t SHIFTR;     /*!< RTC shift control register,                                Address offset: 0x2C */
  __IO uint32_t TSTR;       /*!< RTC time stamp time register,                              Address offset: 0x30 */
  __IO uint32_t TSDR;       /*!< RTC time stamp date register,                              Address offset: 0x34 */
  __IO uint32_t TSSSR;      /*!< RTC time-stamp sub second register,                        Address offset: 0x38 */
  __IO uint32_t CALR;       /*!< RTC calibration register,                                  Address offset: 0x3C */
  __IO uint32_t TAFCR;      /*!< RTC tamper and alternate function configuration register,  Address offset: 0x40 */
  __IO uint32_t ALRMASSR;   /*!< RTC alarm A sub second register,                           Address offset: 0x44 */
  __IO uint32_t ALRMBSSR;   /*!< RTC alarm B sub second register,                           Address offset: 0x48 */
  __IO uint32_t OR;         /*!< RTC option register,                                       Address offset: 0x4C */
  __IO uint32_t BKP0R;      /*!< RTC backup register 0,                                     Address offset: 0x50 */
  __IO uint32_t BKP1R;      /*!< RTC backup register 1,                                     Address offset: 0x54 */
  __IO uint32_t BKP2R;      /*!< RTC backup register 2,                                     Address offset: 0x58 */
  __IO uint32_t BKP3R;      /*!< RTC backup register 3,                                     Address offset: 0x5C */
  __IO uint32_t BKP4R;      /*!< RTC backup register 4,                                     Address offset: 0x60 */
  __IO uint32_t BKP5R;      /*!< RTC backup register 5,                                     Address offset: 0x64 */
  __IO uint32_t BKP6R;      /*!< RTC backup register 6,                                     Address offset: 0x68 */
  __IO uint32_t BKP7R;      /*!< RTC backup register 7,                                     Address offset: 0x6C */
  __IO uint32_t BKP8R;      /*!< RTC backup register 8,                                     Address offset: 0x70 */
  __IO uint32_t BKP9R;      /*!< RTC backup register 9,                                     Address offset: 0x74 */
  __IO uint32_t BKP10R;     /*!< RTC backup register 10,                                    Address offset: 0x78 */
  __IO uint32_t BKP11R;     /*!< RTC backup register 11,                                    Address offset: 0x7C */
  __IO uint32_t BKP12R;     /*!< RTC backup register 12,                                    Address offset: 0x80 */
  __IO uint32_t BKP13R;     /*!< RTC backup register 13,                                    Address offset: 0x84 */
  __IO uint32_t BKP14R;     /*!< RTC backup register 14,                                    Address offset: 0x88 */
  __IO uint32_t BKP15R;     /*!< RTC backup register 15,                                    Address offset: 0x8C */
  __IO uint32_t BKP16R;     /*!< RTC backup register 16,                                    Address offset: 0x90 */
  __IO uint32_t BKP17R;     /*!< RTC backup register 17,                                    Address offset: 0x94 */
  __IO uint32_t BKP18R;     /*!< RTC backup register 18,                                    Address offset: 0x98 */
  __IO uint32_t BKP19R;     /*!< RTC backup register 19,                                    Address offset: 0x9C */
  __IO uint32_t BKP20R;     /*!< RTC backup register 20,                                    Address offset: 0xA0 */
  __IO uint32_t BKP21R;     /*!< RTC backup register 21,                                    Address offset: 0xA4 */
  __IO uint32_t BKP22R;     /*!< RTC backup register 22,                                    Address offset: 0xA8 */
  __IO uint32_t BKP23R;     /*!< RTC backup register 23,                                    Address offset: 0xAC */
  __IO uint32_t BKP24R;     /*!< RTC backup register 24,                                    Address offset: 0xB0 */
  __IO uint32_t BKP25R;     /*!< RTC backup register 25,                                    Address offset: 0xB4 */
  __IO uint32_t BKP26R;     /*!< RTC backup register 26,                                    Address offset: 0xB8 */
  __IO uint32_t BKP27R;     /*!< RTC backup register 27,                                    Address offset: 0xBC */
  __IO uint32_t BKP28R;     /*!< RTC backup register 28,                                    Address offset: 0xC0 */
  __IO uint32_t BKP29R;     /*!< RTC backup register 29,                                    Address offset: 0xC4 */
  __IO uint32_t BKP30R;     /*!< RTC backup register 30,                                    Address offset: 0xC8 */
  __IO uint32_t BKP31R;     /*!< RTC backup register 31,                                    Address offset: 0xCC */
} RTC_TypeDef;


/** 
  * @brief Serial Audio Interface
  */
  
typedef struct
{
  __IO uint32_t GCR;      /*!< SAI global configuration register,        Address offset: 0x00 */
} SAI_TypeDef;

typedef struct
{
  __IO uint32_t CR1;      /*!< SAI block x configuration register 1,     Address offset: 0x04 */
  __IO uint32_t CR2;      /*!< SAI block x configuration register 2,     Address offset: 0x08 */
  __IO uint32_t FRCR;     /*!< SAI block x frame configuration register, Address offset: 0x0C */
  __IO uint32_t SLOTR;    /*!< SAI block x slot register,                Address offset: 0x10 */
  __IO uint32_t IMR;      /*!< SAI block x interrupt mask register,      Address offset: 0x14 */
  __IO uint32_t SR;       /*!< SAI block x status register,              Address offset: 0x18 */
  __IO uint32_t CLRFR;    /*!< SAI block x clear flag register,          Address offset: 0x1C */
  __IO uint32_t DR;       /*!< SAI block x data register,                Address offset: 0x20 */
} SAI_Block_TypeDef;


/** 
  * @brief SD host Interface
  */

typedef struct
{
  __IO uint32_t POWER;          /*!< SDIO power control register,    Address offset: 0x00 */
  __IO uint32_t CLKCR;          /*!< SDI clock control register,     Address offset: 0x04 */
  __IO uint32_t ARG;            /*!< SDIO argument register,         Address offset: 0x08 */
  __IO uint32_t CMD;            /*!< SDIO command register,          Address offset: 0x0C */
  __I uint32_t  RESPCMD;        /*!< SDIO command response register, Address offset: 0x10 */
  __I uint32_t  RESP1;          /*!< SDIO response 1 register,       Address offset: 0x14 */
  __I uint32_t  RESP2;          /*!< SDIO response 2 register,       Address offset: 0x18 */
  __I uint32_t  RESP3;          /*!< SDIO response 3 register,       Address offset: 0x1C */
  __I uint32_t  RESP4;          /*!< SDIO response 4 register,       Address offset: 0x20 */
  __IO uint32_t DTIMER;         /*!< SDIO data timer register,       Address offset: 0x24 */
  __IO uint32_t DLEN;           /*!< SDIO data length register,      Address offset: 0x28 */
  __IO uint32_t DCTRL;          /*!< SDIO data control register,     Address offset: 0x2C */
  __I uint32_t  DCOUNT;         /*!< SDIO data counter register,     Address offset: 0x30 */
  __I uint32_t  STA;            /*!< SDIO status register,           Address offset: 0x34 */
  __IO uint32_t ICR;            /*!< SDIO interrupt clear register,  Address offset: 0x38 */
  __IO uint32_t MASK;           /*!< SDIO mask register,             Address offset: 0x3C */
  uint32_t      RESERVED0[2];   /*!< Reserved, 0x40-0x44                                  */
  __I uint32_t  FIFOCNT;        /*!< SDIO FIFO counter register,     Address offset: 0x48 */
  uint32_t      RESERVED1[13];  /*!< Reserved, 0x4C-0x7C                                  */
  __IO uint32_t FIFO;           /*!< SDIO data FIFO register,        Address offset: 0x80 */
} SDIO_TypeDef;

/** 
  * @brief Serial Peripheral Interface
  */

typedef struct
{
  __IO uint16_t CR1;      /*!< SPI Control register 1,                              Address offset: 0x00 */
  uint16_t      RESERVED0;  /*!< Reserved, 0x02                                                           */
  __IO uint16_t CR2;      /*!< SPI Control register 2,                              Address offset: 0x04 */
  uint16_t      RESERVED1;  /*!< Reserved, 0x06                                                           */
  __IO uint16_t SR;       /*!< SPI Status register,                                 Address offset: 0x08 */
  uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                                           */
  __IO uint16_t DR;         /*!< SPI data register,                                  Address offset: 0x0C */
  uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                                           */
  __IO uint16_t CRCPR;    /*!< SPI CRC polynomial register,                         Address offset: 0x10 */
  uint16_t      RESERVED4;  /*!< Reserved, 0x12                                                           */
  __IO uint16_t RXCRCR;   /*!< SPI Rx CRC register,                                 Address offset: 0x14 */
  uint16_t      RESERVED5;  /*!< Reserved, 0x16                                                           */
  __IO uint16_t TXCRCR;   /*!< SPI Tx CRC register,                                 Address offset: 0x18 */
  uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                                           */
  uint32_t  RESERVED7;    /*!< Reserved,                                            Address offset: 0x1C */
  uint32_t  RESERVED8;    /*!< Reserved,                                            Address offset: 0x20 */    
} SPI_TypeDef;

/** 
  * @brief QUAD Serial Peripheral Interface
  */

typedef struct
{
  __IO uint32_t CR;       /*!< QUADSPI Control register,                           Address offset: 0x00 */
  __IO uint32_t DCR;      /*!< QUADSPI Device Configuration register,              Address offset: 0x04 */
  __IO uint32_t SR;       /*!< QUADSPI Status register,                            Address offset: 0x08 */
  __IO uint32_t FCR;      /*!< QUADSPI Flag Clear register,                        Address offset: 0x0C */
  __IO uint32_t DLR;      /*!< QUADSPI Data Length register,                       Address offset: 0x10 */
  __IO uint32_t CCR;      /*!< QUADSPI Communication Configuration register,       Address offset: 0x14 */
  __IO uint32_t AR;       /*!< QUADSPI Address register,                           Address offset: 0x18 */
  __IO uint32_t ABR;      /*!< QUADSPI Alternate Bytes register,                   Address offset: 0x1C */
  __IO uint32_t DR;       /*!< QUADSPI Data register,                              Address offset: 0x20 */
  __IO uint32_t PSMKR;    /*!< QUADSPI Polling Status Mask register,               Address offset: 0x24 */
  __IO uint32_t PSMAR;    /*!< QUADSPI Polling Status Match register,              Address offset: 0x28 */                  
  __IO uint32_t PIR;      /*!< QUADSPI Polling Interval register,                  Address offset: 0x2C */
  __IO uint32_t LPTR;     /*!< QUADSPI Low Power Timeout register,                 Address offset: 0x30 */    
} QUADSPI_TypeDef;

/** 
  * @brief TIM
  */

typedef struct
{
  __IO uint16_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
  uint16_t      RESERVED0;   /*!< Reserved, 0x02                                                 */
  __IO uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
  __IO uint16_t PSC;         /*!< TIM prescaler,                            Address offset: 0x28 */
  uint16_t      RESERVED9;   /*!< Reserved, 0x2A                                                 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
  __IO uint16_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
  uint16_t      RESERVED10;  /*!< Reserved, 0x32                                                 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
  __IO uint16_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
  uint16_t      RESERVED12;  /*!< Reserved, 0x4A                                                 */
  __IO uint16_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
  uint16_t      RESERVED13;  /*!< Reserved, 0x4E                                                 */
  __IO uint16_t OR;          /*!< TIM option register,                      Address offset: 0x50 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare register5,            Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare register6,            Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM Alternate function option register 1  Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM Alternate function option register 2, Address offset: 0x64 */

} TIM_TypeDef;

/** 
  * @brief LPTIMIMER
  */
typedef struct
{
  __IO uint32_t ISR;      /*!< LPTIM Interrupt and Status register,                Address offset: 0x00 */
  __IO uint32_t ICR;      /*!< LPTIM Interrupt Clear register,                     Address offset: 0x04 */
  __IO uint32_t IER;      /*!< LPTIM Interrupt Enable register,                    Address offset: 0x08 */
  __IO uint32_t CFGR;     /*!< LPTIM Configuration register,                       Address offset: 0x0C */
  __IO uint32_t CR;       /*!< LPTIM Control register,                             Address offset: 0x10 */
  __IO uint32_t CMP;      /*!< LPTIM Compare register,                             Address offset: 0x14 */
  __IO uint32_t ARR;      /*!< LPTIM Autoreload register,                          Address offset: 0x18 */
  __IO uint32_t CNT;      /*!< LPTIM Counter register,                             Address offset: 0x1C */
  __IO uint32_t OR;       /*!< LPTIM Option register,                              Address offset: 0x20 */
} LPTIM_TypeDef;

/**
  * @brief Comparator
  */
typedef struct
{
  __IO uint32_t CTRL;        /*!< Comparator control Status register,                 Address offset: 0x00 */
} COMP_TypeDef;

/** 
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
 
typedef struct
{
  __IO uint32_t CR1;    /*!< USART Control register 1,                 Address offset: 0x00 */ 
  __IO uint32_t CR2;    /*!< USART Control register 2,                 Address offset: 0x04 */ 
  __IO uint32_t CR3;    /*!< USART Control register 3,                 Address offset: 0x08 */
  __IO uint32_t BRR;    /*!< USART Baud rate register,                 Address offset: 0x0C */                                               
  __IO uint16_t GTPR;   /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
  uint16_t  RESERVED2;  /*!< Reserved, 0x12                                                 */
  __IO uint32_t RTOR;   /*!< USART Receiver Time Out register,         Address offset: 0x14 */  
  __IO uint16_t RQR;    /*!< USART Request register,                   Address offset: 0x18 */
  uint16_t  RESERVED3;  /*!< Reserved, 0x1A                                                 */
  __IO uint32_t ISR;    /*!< USART Interrupt and status register,      Address offset: 0x1C */
  __IO uint32_t ICR;    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
  __IO uint16_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
  uint16_t  RESERVED4;  /*!< Reserved, 0x26                                                 */
  __IO uint16_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
  uint16_t  RESERVED5;  /*!< Reserved, 0x2A                                                 */
} USART_TypeDef;

/** 
  * @brief Single Wire Protocol Master Interface SPWMI
  */
typedef struct
{
  __IO uint16_t CR;          /*!< SWPMI Configuration/Control register,     Address offset: 0x00 */
  uint16_t      RESERVED0;   /*!< Reserved, 0x02                                                 */
  __IO uint16_t BRR;         /*!< SWPMI bitrate register,                   Address offset: 0x04 */
  uint16_t      RESERVED1;   /*!< Reserved, 0x06                                                 */
  __IO uint16_t TRIM;        /*!< SWPMI trimming register,                  Address offset: 0x08 */
  uint16_t      RESERVED2;   /*!< Reserved, 0x0A                                                 */
  __IO uint16_t ISR;         /*!< SWPMI Interrupt and Status register,      Address offset: 0x0C */
  uint16_t      RESERVED3;   /*!< Reserved, 0x0E                                                 */
  __IO uint16_t ICR;         /*!< SWPMI Interrupt Flag Clear register,      Address offset: 0x10 */
  uint16_t      RESERVED4;   /*!< Reserved, 0x12                                                 */
  __IO uint16_t IER;         /*!< SWPMI Interrupt Enable register,          Address offset: 0x14 */
  uint16_t      RESERVED5;   /*!< Reserved, 0x16                                                 */
  __IO uint16_t RFL;         /*!< SWPMI Receive Frame Length register,      Address offset: 0x18 */
  uint16_t      RESERVED6;   /*!< Reserved, 0x1A                                                 */    
  __IO uint32_t TDR;         /*!< SWPMI Transmit data register,             Address offset: 0x1C */ 
  __IO uint32_t RDR;         /*!< SWPMI Receive data register,              Address offset: 0x20 */
  __IO uint16_t OR;          /*!< SWPMI Option register,                    Address offset: 0x24 */ 
  uint16_t      RESERVED7;   /*!< Reserved, 0x26                                                 */                                                   
} SWPMI_TypeDef;

/** 
  * @brief Window WATCHDOG
  */

typedef struct
{
  __IO uint32_t CR;   /*!< WWDG Control register,       Address offset: 0x00 */
  __IO uint32_t CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
  __IO uint32_t SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

/**
  * @brief AES hardware accelerator
  */

typedef struct
{
  __IO uint32_t CR;           /*!< AES control register,                        Address offset: 0x00 */
  __IO uint32_t SR;           /*!< AES status register,                         Address offset: 0x04 */
  __IO uint32_t DINR;         /*!< AES data input register,                     Address offset: 0x08 */
  __IO uint32_t DOUTR;        /*!< AES data output register,                    Address offset: 0x0C */
  __IO uint32_t KEYR0;        /*!< AES key register 0,                          Address offset: 0x10 */
  __IO uint32_t KEYR1;        /*!< AES key register 1,                          Address offset: 0x14 */
  __IO uint32_t KEYR2;        /*!< AES key register 2,                          Address offset: 0x18 */
  __IO uint32_t KEYR3;        /*!< AES key register 3,                          Address offset: 0x1C */
  __IO uint32_t IVR0;         /*!< AES initialization vector register 0,        Address offset: 0x20 */
  __IO uint32_t IVR1;         /*!< AES initialization vector register 1,        Address offset: 0x24 */
  __IO uint32_t IVR2;         /*!< AES initialization vector register 2,        Address offset: 0x28 */
  __IO uint32_t IVR3;         /*!< AES initialization vector register 3,        Address offset: 0x2C */
  __IO uint32_t KEYR4;        /*!< AES key register 4,                          Address offset: 0x30 */
  __IO uint32_t KEYR5;        /*!< AES key register 5,                          Address offset: 0x34 */
  __IO uint32_t KEYR6;        /*!< AES key register 6,                          Address offset: 0x38 */
  __IO uint32_t KEYR7;        /*!< AES key register 7,                          Address offset: 0x3C */
  __IO uint32_t SUSP0R;       /*!< AES Suspend register 0,                      Address offset: 0x40 */
  __IO uint32_t SUSP1R;       /*!< AES Suspend register 1,                      Address offset: 0x44 */
  __IO uint32_t SUSP2R;       /*!< AES Suspend register 2,                      Address offset: 0x48 */
  __IO uint32_t SUSP3R;       /*!< AES Suspend register 3,                      Address offset: 0x4C */
  __IO uint32_t SUSP4R;       /*!< AES Suspend register 4,                      Address offset: 0x50 */
  __IO uint32_t SUSP5R;       /*!< AES Suspend register 5,                      Address offset: 0x54 */
  __IO uint32_t SUSP6R;       /*!< AES Suspend register 6,                      Address offset: 0x58 */
  __IO uint32_t SUSP7R;       /*!< AES Suspend register 7,                      Address offset: 0x6C */
} AES_TypeDef;


/** 
  * @brief RNG
  */
  
typedef struct 
{
  __IO uint32_t CR;  /*!< RNG control register, Address offset: 0x00 */
  __IO uint32_t SR;  /*!< RNG status register,  Address offset: 0x04 */
  __IO uint32_t DR;  /*!< RNG data register,    Address offset: 0x08 */
} RNG_TypeDef;

/**
  * @}
  */
  
/** @addtogroup Peripheral_memory_map
  * @{
  */
#define FLASH_BASE            ((uint32_t)0x08000000) /*!< FLASH(up to 1 MB) base address */
#define SRAM1_BASE            ((uint32_t)0x20000000) /*!< SRAM1(96 KB) base address*/
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address */
#define SRAM2_BASE            ((uint32_t)0x10000000) /*!< SRAM2(32 KB) base address*/
#define FMC_R_BASE            ((uint32_t)0xA0000000) /*!< FMC  control registers base address */
#define QSPI_R_BASE           ((uint32_t)0xA0001000) /*!< QUADSPI control registers base address */
#define SRAM1_BB_BASE         ((uint32_t)0x22000000) /*!< SRAM1(96 KB) base address in the bit-band region */
#define PERIPH_BB_BASE        ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region */
#define SRAM2_BB_BASE         ((uint32_t)0x12000000) /*!< SRAM2(32 KB) base address in the bit-band region */

/* Legacy defines */
#define SRAM_BASE             SRAM1_BASE
#define SRAM_BB_BASE          SRAM1_BB_BASE


/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000)



/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define LCD_BASE              (APB1PERIPH_BASE + 0x2400)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
#define CAN_BASE             (APB1PERIPH_BASE + 0x6400)
#define LPTIM1_BASE           (APB1PERIPH_BASE + 0x7C00)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)
#define OPAMP_BASE            (APB1PERIPH_BASE + 0x7800)
#define OPAMP1_BASE           (APB1PERIPH_BASE + 0x7800)
#define OPAMP2_BASE           (APB1PERIPH_BASE + 0x7810)
#define LPUART_BASE           (APB1PERIPH_BASE + 0x8000)
#define SWPMI_BASE            (APB1PERIPH_BASE + 0x8800)
#define LPTIM2_BASE           (APB1PERIPH_BASE + 0x9400)


/*!< APB2 peripherals */
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x0000)
#define COMP_BASE             (APB2PERIPH_BASE + 0x0200)
#define COMP1_BASE            (APB2PERIPH_BASE + 0x0200)
#define COMP2_BASE            (APB2PERIPH_BASE + 0x0204)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)
#define FIREWALL_BASE         (APB2PERIPH_BASE + 0x1C00)
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)
#define TIM15_BASE            (APB2PERIPH_BASE + 0x4000)
#define TIM16_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIM17_BASE            (APB2PERIPH_BASE + 0x4800)
#define SAI1_BASE             (APB2PERIPH_BASE + 0x5400)
#define SAI1_Block_A_BASE     (SAI1_BASE + 0x004)
#define SAI1_Block_B_BASE     (SAI1_BASE + 0x024)
#define SAI2_BASE             (APB2PERIPH_BASE + 0x5800)
#define SAI2_Block_A_BASE     (SAI2_BASE + 0x004)
#define SAI2_Block_B_BASE     (SAI2_BASE + 0x024)
#define DFSDM_BASE            (APB2PERIPH_BASE + 0x6000)
#define DFSDM_Channel0_BASE   (DFSDM_BASE + 0x00)
#define DFSDM_Channel1_BASE   (DFSDM_BASE + 0x20)
#define DFSDM_Channel2_BASE   (DFSDM_BASE + 0x40)
#define DFSDM_Channel3_BASE   (DFSDM_BASE + 0x60)
#define DFSDM_Channel4_BASE   (DFSDM_BASE + 0x80)
#define DFSDM_Channel5_BASE   (DFSDM_BASE + 0xA0)
#define DFSDM_Channel6_BASE   (DFSDM_BASE + 0xC0)
#define DFSDM_Channel7_BASE   (DFSDM_BASE + 0xE0)
#define DFSDM0_BASE           (DFSDM_BASE + 0x100)
#define DFSDM1_BASE           (DFSDM_BASE + 0x180)
#define DFSDM2_BASE           (DFSDM_BASE + 0x200)
#define DFSDM3_BASE           (DFSDM_BASE + 0x280)

/*!< AHB1 peripherals */
#define DMA1_BASE             (AHB1PERIPH_BASE)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x0400)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x1000)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x2000)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000)
#define TSC_BASE              (AHB1PERIPH_BASE + 0x4000)


#define DMA1_Channel1_BASE    (DMA1_BASE + 0x0008)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x001C)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x0030)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x0044)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x0058)
#define DMA1_Channel6_BASE    (DMA1_BASE + 0x006C)
#define DMA1_Channel7_BASE    (DMA1_BASE + 0x0080)
#define DMA1_CSELR_BASE       (DMA1_BASE + 0x00A8)


#define DMA2_Channel1_BASE    (DMA2_BASE + 0x0008)
#define DMA2_Channel2_BASE    (DMA2_BASE + 0x001C)
#define DMA2_Channel3_BASE    (DMA2_BASE + 0x0030)
#define DMA2_Channel4_BASE    (DMA2_BASE + 0x0044)
#define DMA2_Channel5_BASE    (DMA2_BASE + 0x0058)
#define DMA2_Channel6_BASE    (DMA2_BASE + 0x006C)
#define DMA2_Channel7_BASE    (DMA2_BASE + 0x0080)
#define DMA2_CSELR_BASE       (DMA2_BASE + 0x00A8)


/*!< AHB2 peripherals */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB2PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB2PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB2PERIPH_BASE + 0x1C00)

#define USBOTG_BASE           (AHB2PERIPH_BASE + 0x8000000)
#define ADC1_BASE             (AHB2PERIPH_BASE + 0x8040000)
#define ADC2_BASE             (AHB2PERIPH_BASE + 0x8040100)
#define ADC3_BASE             (AHB2PERIPH_BASE + 0x8040200)
#define ADC123_BASE           (AHB2PERIPH_BASE + 0x8040300)

#define AES_BASE              (AHB2PERIPH_BASE + 0x08060000)
#define RNG_BASE              (AHB2PERIPH_BASE + 0x8060800)

/*!< FMC Banks registers base  address */
#define FMC_Bank1_R_BASE      (FMC_R_BASE + 0x0000)
#define FMC_Bank1E_R_BASE     (FMC_R_BASE + 0x0104)
#define FMC_Bank2_R_BASE      (FMC_R_BASE + 0x0060)
#define FMC_Bank3_R_BASE      (FMC_R_BASE + 0x0080)
#define FMC_Bank4_R_BASE      (FMC_R_BASE + 0x00A0)

/* Debug MCU registers base address */
#define DBGMCU_BASE           ((uint32_t )0xE0042000)

/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */  
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define LCD                 ((LCD_TypeDef *) LCD_BASE)
#define RTC                 ((RTC_TypeDef *) RTC_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define SPI3                ((SPI_TypeDef *) SPI3_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
#define I2C3                ((I2C_TypeDef *) I2C3_BASE)
#define CAN                 ((CAN_TypeDef *) CAN_BASE)
#define LPTIM1              ((LPTIM_TypeDef *) LPTIM1_BASE)
#define PWR                 ((PWR_TypeDef *) PWR_BASE)
#define DAC                 ((DAC_TypeDef *) DAC_BASE)
#define OPAMP               ((OPAMP_TypeDef *) OPAMP_BASE)
#define OPAMP1              ((OPAMP_TypeDef *) OPAMP1_BASE)
#define OPAMP2              ((OPAMP_TypeDef *) OPAMP2_BASE)
#define LPUART             ((USART_TypeDef *) LPUART_BASE)
#define SWPMI                ((SWPMI_TypeDef *) SWPMI_BASE)
#define LPTIM2              ((LPTIM_TypeDef *) LPTIM2_BASE)

#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define COMP                ((COMP_TypeDef *) COMP_BASE)
#define COMP1               ((COMP_TypeDef *) COMP1_BASE)
#define COMP2               ((COMP_TypeDef *) COMP2_BASE) 
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define FIREWALL            ((FIREWALL_TypeDef *) FIREWALL_BASE)
#define SDIO                ((SDIO_TypeDef *) SDIO_BASE)
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE) 
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)
#define SAI1                ((SAI_TypeDef *) SAI1_BASE)
#define SAI1_Block_A        ((SAI_Block_TypeDef *)SAI1_Block_A_BASE)
#define SAI1_Block_B        ((SAI_Block_TypeDef *)SAI1_Block_B_BASE)
#define SAI2                ((SAI_TypeDef *) SAI2_BASE)
#define SAI2_Block_A        ((SAI_Block_TypeDef *)SAI2_Block_A_BASE)
#define SAI2_Block_B        ((SAI_Block_TypeDef *)SAI2_Block_B_BASE)
#define DFSDM_Channel0      ((DFSDM_Channel_TypeDef *) DFSDM_Channel0_BASE)
#define DFSDM_Channel1      ((DFSDM_Channel_TypeDef *) DFSDM_Channel1_BASE)
#define DFSDM_Channel2      ((DFSDM_Channel_TypeDef *) DFSDM_Channel2_BASE)
#define DFSDM_Channel3      ((DFSDM_Channel_TypeDef *) DFSDM_Channel3_BASE)
#define DFSDM_Channel4      ((DFSDM_Channel_TypeDef *) DFSDM_Channel4_BASE)
#define DFSDM_Channel5      ((DFSDM_Channel_TypeDef *) DFSDM_Channel5_BASE)
#define DFSDM_Channel6      ((DFSDM_Channel_TypeDef *) DFSDM_Channel6_BASE)
#define DFSDM_Channel7      ((DFSDM_Channel_TypeDef *) DFSDM_Channel7_BASE)
#define DFSDM0              ((DFSDM_TypeDef *) DFSDM0_BASE)
#define DFSDM1              ((DFSDM_TypeDef *) DFSDM1_BASE)
#define DFSDM2              ((DFSDM_TypeDef *) DFSDM2_BASE)
#define DFSDM3              ((DFSDM_TypeDef *) DFSDM3_BASE)
#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)
//#define TSC               ((TSC_TypeDef *) TSC_BASE)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
//#define USBOTG_BASE         ((USBOTG_TypeDef *) USBOTG_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC2                ((ADC_TypeDef *) ADC2_BASE)
#define ADC3                ((ADC_TypeDef *) ADC3_BASE)
#define ADC123              ((ADC_Common_TypeDef *) ADC123_BASE)
#define AES                 ((AES_TypeDef *) AES_BASE)
#define RNG                 ((RNG_TypeDef *) RNG_BASE)


#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)
#define DMA1_Channel6       ((DMA_Channel_TypeDef *) DMA1_Channel6_BASE)
#define DMA1_Channel7       ((DMA_Channel_TypeDef *) DMA1_Channel7_BASE)
#define DMA1_CSELR          ((DMA_request_TypeDef *) DMA1_CSELR_BASE) 


#define DMA2_Channel1       ((DMA_Channel_TypeDef *) DMA2_Channel1_BASE)
#define DMA2_Channel2       ((DMA_Channel_TypeDef *) DMA2_Channel2_BASE)
#define DMA2_Channel3       ((DMA_Channel_TypeDef *) DMA2_Channel3_BASE)
#define DMA2_Channel4       ((DMA_Channel_TypeDef *) DMA2_Channel4_BASE)
#define DMA2_Channel5       ((DMA_Channel_TypeDef *) DMA2_Channel5_BASE)
#define DMA2_Channel6       ((DMA_Channel_TypeDef *) DMA2_Channel6_BASE)
#define DMA2_Channel7       ((DMA_Channel_TypeDef *) DMA2_Channel7_BASE)
#define DMA2_CSELR          ((DMA_request_TypeDef *) DMA2_CSELR_BASE) 


#define FMC_Bank1           ((FMC_Bank1_TypeDef *) FMC_Bank1_R_BASE)
#define FMC_Bank1E          ((FMC_Bank1E_TypeDef *) FMC_Bank1E_R_BASE)
#define FMC_Bank2           ((FMC_Bank2_TypeDef *) FMC_Bank2_R_BASE)
#define FMC_Bank3           ((FMC_Bank3_TypeDef *) FMC_Bank3_R_BASE)
#define FMC_Bank4           ((FMC_Bank4_TypeDef *) FMC_Bank4_R_BASE)

#define QUADSPI               ((QUADSPI_TypeDef *) QSPI_R_BASE)

#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)

/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */
  
  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */
    
/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        Analog to Digital Converter                         */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for ADC_ISR register  ********************/
#define ADC_ISR_ADRD          ((uint32_t)0x00000001)   /*!< ADC Ready (ADRDY) flag  */
#define ADC_ISR_EOSMP         ((uint32_t)0x00000002) /*!< ADC End of Sampling flag */
#define ADC_ISR_EOC           ((uint32_t)0x00000004) /*!< ADC End of Regular Conversion flag */
#define ADC_ISR_EOS           ((uint32_t)0x00000008) /*!< ADC End of Regular sequence of Conversions flag */
#define ADC_ISR_OVR           ((uint32_t)0x00000010) /*!< ADC overrun flag */
#define ADC_ISR_JEOC          ((uint32_t)0x00000020) /*!< ADC End of Injected Conversion flag */
#define ADC_ISR_JEOS          ((uint32_t)0x00000040) /*!< ADC End of Injected sequence of Conversions flag */
#define ADC_ISR_AWD1          ((uint32_t)0x00000080) /*!< ADC Analog watchdog 1 flag */
#define ADC_ISR_AWD2          ((uint32_t)0x00000100) /*!< ADC Analog watchdog 2 flag */
#define ADC_ISR_AWD3          ((uint32_t)0x00000200) /*!< ADC Analog watchdog 3 flag */
#define ADC_ISR_JQOVF         ((uint32_t)0x00000400) /*!< ADC Injected Context Queue Overflow flag */

/********************  Bit definition for ADC_IER register  ********************/
#define ADC_IER_RDY           ((uint32_t)0x00000001) /*!< ADC Ready (ADRDY) interrupt source */
#define ADC_IER_EOSMP         ((uint32_t)0x00000002) /*!< ADC End of Sampling interrupt source */
#define ADC_IER_EOC           ((uint32_t)0x00000004) /*!< ADC End of Regular Conversion interrupt source */
#define ADC_IER_EOS           ((uint32_t)0x00000008) /*!< ADC End of Regular sequence of Conversions interrupt source */
#define ADC_IER_OVR           ((uint32_t)0x00000010) /*!< ADC overrun interrupt source */
#define ADC_IER_JEOC          ((uint32_t)0x00000020) /*!< ADC End of Injected Conversion interrupt source */
#define ADC_IER_JEOS          ((uint32_t)0x00000040) /*!< ADC End of Injected sequence of Conversions interrupt source */
#define ADC_IER_AWD1          ((uint32_t)0x00000080) /*!< ADC Analog watchdog 1 interrupt source */
#define ADC_IER_AWD2          ((uint32_t)0x00000100) /*!< ADC Analog watchdog 2 interrupt source */
#define ADC_IER_AWD3          ((uint32_t)0x00000200) /*!< ADC Analog watchdog 3 interrupt source */
#define ADC_IER_JQOVF         ((uint32_t)0x00000400) /*!< ADC Injected Context Queue Overflow interrupt source */

/********************  Bit definition for ADC_CR register  ********************/
#define ADC_CR_ADEN          ((uint32_t)0x00000001) /*!< ADC Enable control */
#define ADC_CR_ADDIS         ((uint32_t)0x00000002) /*!< ADC Disable command */
#define ADC_CR_ADSTART       ((uint32_t)0x00000004) /*!< ADC Start of Regular conversion */
#define ADC_CR_JADSTART      ((uint32_t)0x00000008) /*!< ADC Start of injected conversion */
#define ADC_CR_ADSTP         ((uint32_t)0x00000010) /*!< ADC Stop of Regular conversion */
#define ADC_CR_JADSTP        ((uint32_t)0x00000020) /*!< ADC Stop of injected conversion */
#define ADC_CR_ADVREGEN      ((uint32_t)0x10000000) /*!< ADC Voltage regulator Enable */
#define ADC_CR_DEEPPWD       ((uint32_t)0x20000000) /*!< ADC Deep power down Enable */
#define ADC_CR_ADCALDIF      ((uint32_t)0x40000000) /*!< ADC Differential Mode for calibration */
#define ADC_CR_ADCAL         ((uint32_t)0x80000000) /*!< ADC Calibration */

/********************  Bit definition for ADC_CFGR register  ********************/
#define ADC_CFGR_DMAEN     ((uint32_t)0x00000001) /*!< ADC DMA Enable */
#define ADC_CFGR_DMACFG    ((uint32_t)0x00000002) /*!< ADC DMA configuration */

#define ADC_CFGR_RES       ((uint32_t)0x00000018) /*!< ADC Data resolution */
#define ADC_CFGR_RES_0     ((uint32_t)0x00000008) /*!< ADC RES bit 0 */
#define ADC_CFGR_RES_1     ((uint32_t)0x00000010) /*!< ADC RES bit 1 */

#define ADC_CFGR_ALIGN     ((uint32_t)0x00000020) /*!< ADC Data Alignement */

#define ADC_CFGR_EXTSEL   ((uint32_t)0x000003C0) /*!< ADC External trigger selection for regular group */
#define ADC_CFGR_EXTSEL_0 ((uint32_t)0x00000040) /*!< ADC EXTSEL bit 0 */
#define ADC_CFGR_EXTSEL_1 ((uint32_t)0x00000080) /*!< ADC EXTSEL bit 1 */
#define ADC_CFGR_EXTSEL_2 ((uint32_t)0x00000100) /*!< ADC EXTSEL bit 2 */
#define ADC_CFGR_EXTSEL_3 ((uint32_t)0x00000200) /*!< ADC EXTSEL bit 3 */

#define ADC_CFGR_EXTEN     ((uint32_t)0x00000C00) /*!< ADC External trigger enable and polarity selection for regular channels */
#define ADC_CFGR_EXTEN_0   ((uint32_t)0x00000400) /*!< ADC EXTEN bit 0 */
#define ADC_CFGR_EXTEN_1   ((uint32_t)0x00000800) /*!< ADC EXTEN bit 1 */

#define ADC_CFGR_OVRMOD    ((uint32_t)0x00001000) /*!< ADC overrun mode */
#define ADC_CFGR_CONT      ((uint32_t)0x00002000) /*!< ADC Single/continuous conversion mode for regular conversion */
#define ADC_CFGR_AUTDLY   ((uint32_t)0x00004000) /*!< ADC Delayed conversion mode */
#define ADC_CFGR_AUTOFF    ((uint32_t)0x00008000) /*!< ADC Auto power OFF */
#define ADC_CFGR_DISCEN    ((uint32_t)0x00010000) /*!< ADC Discontinuous mode for regular channels */

#define ADC_CFGR_DISCNUM   ((uint32_t)0x000E0000) /*!< ADC Discontinuous mode channel count */
#define ADC_CFGR_DISCNUM_0 ((uint32_t)0x00020000) /*!< ADC DISCNUM bit 0 */
#define ADC_CFGR_DISCNUM_1 ((uint32_t)0x00040000) /*!< ADC DISCNUM bit 1 */
#define ADC_CFGR_DISCNUM_2 ((uint32_t)0x00080000) /*!< ADC DISCNUM bit 2 */

#define ADC_CFGR_JDISCEN   ((uint32_t)0x00100000) /*!< ADC Discontinuous mode on injected channels */
#define ADC_CFGR_JQM       ((uint32_t)0x00200000) /*!< ADC JSQR Queue mode */
#define ADC_CFGR_AWD1SGL   ((uint32_t)0x00400000) /*!< Enable the watchdog 1 on a single channel or on all channels */
#define ADC_CFGR_AWD1EN    ((uint32_t)0x00800000) /*!< ADC Analog watchdog 1 enable on regular Channels */
#define ADC_CFGR_JAWD1EN   ((uint32_t)0x01000000) /*!< ADC Analog watchdog 1 enable on injected Channels */
#define ADC_CFGR_JAUTO     ((uint32_t)0x02000000) /*!< ADC Automatic injected group conversion */

#define ADC_CFGR_AWD1CH    ((uint32_t)0x7C000000) /*!< ADC Analog watchdog 1 Channel selection */
#define ADC_CFGR_AWD1CH_0  ((uint32_t)0x04000000) /*!< ADC AWD1CH bit 0 */
#define ADC_CFGR_AWD1CH_1  ((uint32_t)0x08000000) /*!< ADC AWD1CH bit 1  */
#define ADC_CFGR_AWD1CH_2  ((uint32_t)0x10000000) /*!< ADC AWD1CH bit 2  */
#define ADC_CFGR_AWD1CH_3  ((uint32_t)0x20000000) /*!< ADC AWD1CH bit 3  */
#define ADC_CFGR_AWD1CH_4  ((uint32_t)0x40000000) /*!< ADC AWD1CH bit 4  */

#define ADC_CFGR_JQDIS     ((uint32_t)0x80000000) /*!< ADC Injected queue disable */

/********************  Bit definition for ADC_CFGR2 register  ********************/
#define ADC_CFGR2_ROVSE    ((uint32_t)0x00000001) /*!< ADC Regular group oversampler enable */
#define ADC_CFGR2_JOVSE    ((uint32_t)0x00000002) /*!< ADC Injected group oversampler enable */

#define ADC_CFGR2_OVSR     ((uint32_t)0x0000001C) /*!< ADC Regular group oversampler enable */
#define ADC_CFGR2_OVSR_0   ((uint32_t)0x00000004) /*!< ADC OVSR bit 0 */
#define ADC_CFGR2_OVSR_1   ((uint32_t)0x00000008) /*!< ADC OVSR bit 1 */
#define ADC_CFGR2_OVSR_2   ((uint32_t)0x00000010) /*!< ADC OVSR bit 2 */

#define ADC_CFGR2_OVSS     ((uint32_t)0x000000E0) /*!< ADC Regular Oversampling shift */
#define ADC_CFGR2_OVSS_0   ((uint32_t)0x00000020) /*!< ADC OVSS bit 0 */
#define ADC_CFGR2_OVSS_1   ((uint32_t)0x00000040) /*!< ADC OVSS bit 1 */
#define ADC_CFGR2_OVSS_2   ((uint32_t)0x00000080) /*!< ADC OVSS bit 2 */

#define ADC_CFGR2_TROVS   ((uint32_t)0x00000200) /*!< ADC Triggered regular Oversampling */
#define ADC_CFGR2_ROVSM   ((uint32_t)0x00000400) /*!< ADC Regular oversampling mode */

/********************  Bit definition for ADC_SMPR1 register  ********************/
#define ADC_SMPR1_SMP0     ((uint32_t)0x00000007) /*!< ADC Channel 0 Sampling time selection  */
#define ADC_SMPR1_SMP0_0   ((uint32_t)0x00000001) /*!< ADC SMP0 bit 0 */
#define ADC_SMPR1_SMP0_1   ((uint32_t)0x00000002) /*!< ADC SMP0 bit 1 */
#define ADC_SMPR1_SMP0_2   ((uint32_t)0x00000004) /*!< ADC SMP0 bit 2 */

#define ADC_SMPR1_SMP1     ((uint32_t)0x00000038) /*!< ADC Channel 1 Sampling time selection  */
#define ADC_SMPR1_SMP1_0   ((uint32_t)0x00000008) /*!< ADC SMP1 bit 0 */
#define ADC_SMPR1_SMP1_1   ((uint32_t)0x00000010) /*!< ADC SMP1 bit 1 */
#define ADC_SMPR1_SMP1_2   ((uint32_t)0x00000020) /*!< ADC SMP1 bit 2 */

#define ADC_SMPR1_SMP2     ((uint32_t)0x000001C0) /*!< ADC Channel 2 Sampling time selection  */
#define ADC_SMPR1_SMP2_0   ((uint32_t)0x00000040) /*!< ADC SMP2 bit 0 */
#define ADC_SMPR1_SMP2_1   ((uint32_t)0x00000080) /*!< ADC SMP2 bit 1 */
#define ADC_SMPR1_SMP2_2   ((uint32_t)0x00000100) /*!< ADC SMP2 bit 2 */

#define ADC_SMPR1_SMP3     ((uint32_t)0x00000E00) /*!< ADC Channel 3 Sampling time selection  */
#define ADC_SMPR1_SMP3_0   ((uint32_t)0x00000200) /*!< ADC SMP3 bit 0 */
#define ADC_SMPR1_SMP3_1   ((uint32_t)0x00000400) /*!< ADC SMP3 bit 1 */
#define ADC_SMPR1_SMP3_2   ((uint32_t)0x00000800) /*!< ADC SMP3 bit 2 */

#define ADC_SMPR1_SMP4     ((uint32_t)0x00007000) /*!< ADC Channel 4 Sampling time selection  */
#define ADC_SMPR1_SMP4_0   ((uint32_t)0x00001000) /*!< ADC SMP4 bit 0 */
#define ADC_SMPR1_SMP4_1   ((uint32_t)0x00002000) /*!< ADC SMP4 bit 1 */
#define ADC_SMPR1_SMP4_2   ((uint32_t)0x00004000) /*!< ADC SMP4 bit 2 */

#define ADC_SMPR1_SMP5     ((uint32_t)0x00038000) /*!< ADC Channel 5 Sampling time selection  */
#define ADC_SMPR1_SMP5_0   ((uint32_t)0x00008000) /*!< ADC SMP5 bit 0 */
#define ADC_SMPR1_SMP5_1   ((uint32_t)0x00010000) /*!< ADC SMP5 bit 1 */
#define ADC_SMPR1_SMP5_2   ((uint32_t)0x00020000) /*!< ADC SMP5 bit 2 */

#define ADC_SMPR1_SMP6     ((uint32_t)0x001C0000) /*!< ADC Channel 6 Sampling time selection  */
#define ADC_SMPR1_SMP6_0   ((uint32_t)0x00040000) /*!< ADC SMP6 bit 0 */
#define ADC_SMPR1_SMP6_1   ((uint32_t)0x00080000) /*!< ADC SMP6 bit 1 */
#define ADC_SMPR1_SMP6_2   ((uint32_t)0x00100000) /*!< ADC SMP6 bit 2 */

#define ADC_SMPR1_SMP7     ((uint32_t)0x00E00000) /*!< ADC Channel 7 Sampling time selection  */
#define ADC_SMPR1_SMP7_0   ((uint32_t)0x00200000) /*!< ADC SMP7 bit 0 */
#define ADC_SMPR1_SMP7_1   ((uint32_t)0x00400000) /*!< ADC SMP7 bit 1 */
#define ADC_SMPR1_SMP7_2   ((uint32_t)0x00800000) /*!< ADC SMP7 bit 2 */

#define ADC_SMPR1_SMP8     ((uint32_t)0x07000000) /*!< ADC Channel 8 Sampling time selection  */
#define ADC_SMPR1_SMP8_0   ((uint32_t)0x01000000) /*!< ADC SMP8 bit 0 */
#define ADC_SMPR1_SMP8_1   ((uint32_t)0x02000000) /*!< ADC SMP8 bit 1 */
#define ADC_SMPR1_SMP8_2   ((uint32_t)0x04000000) /*!< ADC SMP8 bit 2 */

#define ADC_SMPR1_SMP9     ((uint32_t)0x38000000) /*!< ADC Channel 9 Sampling time selection  */
#define ADC_SMPR1_SMP9_0   ((uint32_t)0x08000000) /*!< ADC SMP9 bit 0 */
#define ADC_SMPR1_SMP9_1   ((uint32_t)0x10000000) /*!< ADC SMP9 bit 1 */
#define ADC_SMPR1_SMP9_2   ((uint32_t)0x20000000) /*!< ADC SMP9 bit 2 */

/********************  Bit definition for ADC_SMPR2 register  ********************/
#define ADC_SMPR2_SMP10     ((uint32_t)0x00000007) /*!< ADC Channel 10 Sampling time selection  */
#define ADC_SMPR2_SMP10_0   ((uint32_t)0x00000001) /*!< ADC SMP10 bit 0 */
#define ADC_SMPR2_SMP10_1   ((uint32_t)0x00000002) /*!< ADC SMP10 bit 1 */
#define ADC_SMPR2_SMP10_2   ((uint32_t)0x00000004) /*!< ADC SMP10 bit 2 */

#define ADC_SMPR2_SMP11     ((uint32_t)0x00000038) /*!< ADC Channel 11 Sampling time selection  */
#define ADC_SMPR2_SMP11_0   ((uint32_t)0x00000008) /*!< ADC SMP11 bit 0 */
#define ADC_SMPR2_SMP11_1   ((uint32_t)0x00000010) /*!< ADC SMP11 bit 1 */
#define ADC_SMPR2_SMP11_2   ((uint32_t)0x00000020) /*!< ADC SMP11 bit 2 */

#define ADC_SMPR2_SMP12     ((uint32_t)0x000001C0) /*!< ADC Channel 12 Sampling time selection  */
#define ADC_SMPR2_SMP12_0   ((uint32_t)0x00000040) /*!< ADC SMP12 bit 0 */
#define ADC_SMPR2_SMP12_1   ((uint32_t)0x00000080) /*!< ADC SMP12 bit 1 */
#define ADC_SMPR2_SMP12_2   ((uint32_t)0x00000100) /*!< ADC SMP12 bit 2 */

#define ADC_SMPR2_SMP13     ((uint32_t)0x00000E00) /*!< ADC Channel 13 Sampling time selection  */
#define ADC_SMPR2_SMP13_0   ((uint32_t)0x00000200) /*!< ADC SMP13 bit 0 */
#define ADC_SMPR2_SMP13_1   ((uint32_t)0x00000400) /*!< ADC SMP13 bit 1 */
#define ADC_SMPR2_SMP13_2   ((uint32_t)0x00000800) /*!< ADC SMP13 bit 2 */

#define ADC_SMPR2_SMP14     ((uint32_t)0x00007000) /*!< ADC Channel 14 Sampling time selection  */
#define ADC_SMPR2_SMP14_0   ((uint32_t)0x00001000) /*!< ADC SMP14 bit 0 */
#define ADC_SMPR2_SMP14_1   ((uint32_t)0x00002000) /*!< ADC SMP14 bit 1 */
#define ADC_SMPR2_SMP14_2   ((uint32_t)0x00004000) /*!< ADC SMP14 bit 2 */

#define ADC_SMPR2_SMP15     ((uint32_t)0x00038000) /*!< ADC Channel 15 Sampling time selection  */
#define ADC_SMPR2_SMP15_0   ((uint32_t)0x00008000) /*!< ADC SMP15 bit 0 */
#define ADC_SMPR2_SMP15_1   ((uint32_t)0x00010000) /*!< ADC SMP15 bit 1 */
#define ADC_SMPR2_SMP15_2   ((uint32_t)0x00020000) /*!< ADC SMP15 bit 2 */

#define ADC_SMPR2_SMP16     ((uint32_t)0x001C0000) /*!< ADC Channel 16 Sampling time selection  */
#define ADC_SMPR2_SMP16_0   ((uint32_t)0x00040000) /*!< ADC SMP16 bit 0 */
#define ADC_SMPR2_SMP16_1   ((uint32_t)0x00080000) /*!< ADC SMP16 bit 1 */
#define ADC_SMPR2_SMP16_2   ((uint32_t)0x00100000) /*!< ADC SMP16 bit 2 */

#define ADC_SMPR2_SMP17     ((uint32_t)0x00E00000) /*!< ADC Channel 17 Sampling time selection  */
#define ADC_SMPR2_SMP17_0   ((uint32_t)0x00200000) /*!< ADC SMP17 bit 0 */
#define ADC_SMPR2_SMP17_1   ((uint32_t)0x00400000) /*!< ADC SMP17 bit 1 */
#define ADC_SMPR2_SMP17_2   ((uint32_t)0x00800000) /*!< ADC SMP17 bit 2 */

#define ADC_SMPR2_SMP18     ((uint32_t)0x07000000) /*!< ADC Channel 18 Sampling time selection  */
#define ADC_SMPR2_SMP18_0   ((uint32_t)0x01000000) /*!< ADC SMP18 bit 0 */
#define ADC_SMPR2_SMP18_1   ((uint32_t)0x02000000) /*!< ADC SMP18 bit 1 */
#define ADC_SMPR2_SMP18_2   ((uint32_t)0x04000000) /*!< ADC SMP18 bit 2 */

/********************  Bit definition for ADC_TR1 register  ********************/
#define ADC_TR1_LT1         ((uint32_t)0x00000FFF) /*!< ADC Analog watchdog 1 lower threshold */
#define ADC_TR1_LT1_0       ((uint32_t)0x00000001) /*!< ADC LT1 bit 0 */
#define ADC_TR1_LT1_1       ((uint32_t)0x00000002) /*!< ADC LT1 bit 1 */
#define ADC_TR1_LT1_2       ((uint32_t)0x00000004) /*!< ADC LT1 bit 2 */
#define ADC_TR1_LT1_3       ((uint32_t)0x00000008) /*!< ADC LT1 bit 3 */
#define ADC_TR1_LT1_4       ((uint32_t)0x00000010) /*!< ADC LT1 bit 4 */
#define ADC_TR1_LT1_5       ((uint32_t)0x00000020) /*!< ADC LT1 bit 5 */
#define ADC_TR1_LT1_6       ((uint32_t)0x00000040) /*!< ADC LT1 bit 6 */
#define ADC_TR1_LT1_7       ((uint32_t)0x00000080) /*!< ADC LT1 bit 7 */
#define ADC_TR1_LT1_8       ((uint32_t)0x00000100) /*!< ADC LT1 bit 8 */
#define ADC_TR1_LT1_9       ((uint32_t)0x00000200) /*!< ADC LT1 bit 9 */
#define ADC_TR1_LT1_10      ((uint32_t)0x00000400) /*!< ADC LT1 bit 10 */
#define ADC_TR1_LT1_11      ((uint32_t)0x00000800) /*!< ADC LT1 bit 11 */

#define ADC_TR1_HT1         ((uint32_t)0x0FFF0000) /*!< ADC Analog watchdog 1 higher threshold */
#define ADC_TR1_HT1_0       ((uint32_t)0x00010000) /*!< ADC HT1 bit 0 */
#define ADC_TR1_HT1_1       ((uint32_t)0x00020000) /*!< ADC HT1 bit 1 */
#define ADC_TR1_HT1_2       ((uint32_t)0x00040000) /*!< ADC HT1 bit 2 */
#define ADC_TR1_HT1_3       ((uint32_t)0x00080000) /*!< ADC HT1 bit 3 */
#define ADC_TR1_HT1_4       ((uint32_t)0x00100000) /*!< ADC HT1 bit 4 */
#define ADC_TR1_HT1_5       ((uint32_t)0x00200000) /*!< ADC HT1 bit 5 */
#define ADC_TR1_HT1_6       ((uint32_t)0x00400000) /*!< ADC HT1 bit 6 */
#define ADC_TR1_HT1_7       ((uint32_t)0x00800000) /*!< ADC HT1 bit 7 */
#define ADC_TR1_HT1_8       ((uint32_t)0x01000000) /*!< ADC HT1 bit 8 */
#define ADC_TR1_HT1_9       ((uint32_t)0x02000000) /*!< ADC HT1 bit 9 */
#define ADC_TR1_HT1_10      ((uint32_t)0x04000000) /*!< ADC HT1 bit 10 */
#define ADC_TR1_HT1_11      ((uint32_t)0x08000000) /*!< ADC HT1 bit 11 */

/********************  Bit definition for ADC_TR2 register  ********************/
#define ADC_TR2_LT2         ((uint32_t)0x000000FF) /*!< ADC Analog watchdog 2 lower threshold */
#define ADC_TR2_LT2_0       ((uint32_t)0x00000001) /*!< ADC LT2 bit 0 */
#define ADC_TR2_LT2_1       ((uint32_t)0x00000002) /*!< ADC LT2 bit 1 */
#define ADC_TR2_LT2_2       ((uint32_t)0x00000004) /*!< ADC LT2 bit 2 */
#define ADC_TR2_LT2_3       ((uint32_t)0x00000008) /*!< ADC LT2 bit 3 */
#define ADC_TR2_LT2_4       ((uint32_t)0x00000010) /*!< ADC LT2 bit 4 */
#define ADC_TR2_LT2_5       ((uint32_t)0x00000020) /*!< ADC LT2 bit 5 */
#define ADC_TR2_LT2_6       ((uint32_t)0x00000040) /*!< ADC LT2 bit 6 */
#define ADC_TR2_LT2_7       ((uint32_t)0x00000080) /*!< ADC LT2 bit 7 */

#define ADC_TR2_HT2         ((uint32_t)0x00FF0000) /*!< ADC Analog watchdog 2 higher threshold */
#define ADC_TR2_HT2_0       ((uint32_t)0x00010000) /*!< ADC HT2 bit 0 */
#define ADC_TR2_HT2_1       ((uint32_t)0x00020000) /*!< ADC HT2 bit 1 */
#define ADC_TR2_HT2_2       ((uint32_t)0x00040000) /*!< ADC HT2 bit 2 */
#define ADC_TR2_HT2_3       ((uint32_t)0x00080000) /*!< ADC HT2 bit 3 */
#define ADC_TR2_HT2_4       ((uint32_t)0x00100000) /*!< ADC HT2 bit 4 */
#define ADC_TR2_HT2_5       ((uint32_t)0x00200000) /*!< ADC HT2 bit 5 */
#define ADC_TR2_HT2_6       ((uint32_t)0x00400000) /*!< ADC HT2 bit 6 */
#define ADC_TR2_HT2_7       ((uint32_t)0x00800000) /*!< ADC HT2 bit 7 */

/********************  Bit definition for ADC_TR3 register  ********************/
#define ADC_TR3_LT3         ((uint32_t)0x000000FF) /*!< ADC Analog watchdog 3 lower threshold */
#define ADC_TR3_LT3_0       ((uint32_t)0x00000001) /*!< ADC LT3 bit 0 */
#define ADC_TR3_LT3_1       ((uint32_t)0x00000002) /*!< ADC LT3 bit 1 */
#define ADC_TR3_LT3_2       ((uint32_t)0x00000004) /*!< ADC LT3 bit 2 */
#define ADC_TR3_LT3_3       ((uint32_t)0x00000008) /*!< ADC LT3 bit 3 */
#define ADC_TR3_LT3_4       ((uint32_t)0x00000010) /*!< ADC LT3 bit 4 */
#define ADC_TR3_LT3_5       ((uint32_t)0x00000020) /*!< ADC LT3 bit 5 */
#define ADC_TR3_LT3_6       ((uint32_t)0x00000040) /*!< ADC LT3 bit 6 */
#define ADC_TR3_LT3_7       ((uint32_t)0x00000080) /*!< ADC LT3 bit 7 */
  
#define ADC_TR3_HT3         ((uint32_t)0x00FF0000) /*!< ADC Analog watchdog 3 higher threshold */
#define ADC_TR3_HT3_0       ((uint32_t)0x00010000) /*!< ADC HT3 bit 0 */
#define ADC_TR3_HT3_1       ((uint32_t)0x00020000) /*!< ADC HT3 bit 1 */
#define ADC_TR3_HT3_2       ((uint32_t)0x00040000) /*!< ADC HT3 bit 2 */
#define ADC_TR3_HT3_3       ((uint32_t)0x00080000) /*!< ADC HT3 bit 3 */
#define ADC_TR3_HT3_4       ((uint32_t)0x00100000) /*!< ADC HT3 bit 4 */
#define ADC_TR3_HT3_5       ((uint32_t)0x00200000) /*!< ADC HT3 bit 5 */
#define ADC_TR3_HT3_6       ((uint32_t)0x00400000) /*!< ADC HT3 bit 6 */
#define ADC_TR3_HT3_7       ((uint32_t)0x00800000) /*!< ADC HT3 bit 7 */

/********************  Bit definition for ADC_SQR1 register  ********************/
#define ADC_SQR1_L          ((uint32_t)0x0000000F) /*!< ADC regular channel sequence lenght */
#define ADC_SQR1_L_0        ((uint32_t)0x00000001) /*!< ADC L bit 0 */
#define ADC_SQR1_L_1        ((uint32_t)0x00000002) /*!< ADC L bit 1 */
#define ADC_SQR1_L_2        ((uint32_t)0x00000004) /*!< ADC L bit 2 */
#define ADC_SQR1_L_3        ((uint32_t)0x00000008) /*!< ADC L bit 3 */

#define ADC_SQR1_SQ1        ((uint32_t)0x000007C0) /*!< ADC 1st conversion in regular sequence */
#define ADC_SQR1_SQ1_0      ((uint32_t)0x00000040) /*!< ADC SQ1 bit 0 */
#define ADC_SQR1_SQ1_1      ((uint32_t)0x00000080) /*!< ADC SQ1 bit 1 */
#define ADC_SQR1_SQ1_2      ((uint32_t)0x00000100) /*!< ADC SQ1 bit 2 */
#define ADC_SQR1_SQ1_3      ((uint32_t)0x00000200) /*!< ADC SQ1 bit 3 */
#define ADC_SQR1_SQ1_4      ((uint32_t)0x00000400) /*!< ADC SQ1 bit 4 */

#define ADC_SQR1_SQ2        ((uint32_t)0x0001F000) /*!< ADC 2nd conversion in regular sequence */
#define ADC_SQR1_SQ2_0      ((uint32_t)0x00001000) /*!< ADC SQ2 bit 0 */
#define ADC_SQR1_SQ2_1      ((uint32_t)0x00002000) /*!< ADC SQ2 bit 1 */
#define ADC_SQR1_SQ2_2      ((uint32_t)0x00004000) /*!< ADC SQ2 bit 2 */
#define ADC_SQR1_SQ2_3      ((uint32_t)0x00008000) /*!< ADC SQ2 bit 3 */
#define ADC_SQR1_SQ2_4      ((uint32_t)0x00010000) /*!< ADC SQ2 bit 4 */

#define ADC_SQR1_SQ3        ((uint32_t)0x007C0000) /*!< ADC 3rd conversion in regular sequence */
#define ADC_SQR1_SQ3_0      ((uint32_t)0x00040000) /*!< ADC SQ3 bit 0 */
#define ADC_SQR1_SQ3_1      ((uint32_t)0x00080000) /*!< ADC SQ3 bit 1 */
#define ADC_SQR1_SQ3_2      ((uint32_t)0x00100000) /*!< ADC SQ3 bit 2 */
#define ADC_SQR1_SQ3_3      ((uint32_t)0x00200000) /*!< ADC SQ3 bit 3 */
#define ADC_SQR1_SQ3_4      ((uint32_t)0x00400000) /*!< ADC SQ3 bit 4 */

#define ADC_SQR1_SQ4        ((uint32_t)0x1F000000) /*!< ADC 4th conversion in regular sequence */
#define ADC_SQR1_SQ4_0      ((uint32_t)0x01000000) /*!< ADC SQ4 bit 0 */
#define ADC_SQR1_SQ4_1      ((uint32_t)0x02000000) /*!< ADC SQ4 bit 1 */
#define ADC_SQR1_SQ4_2      ((uint32_t)0x04000000) /*!< ADC SQ4 bit 2 */
#define ADC_SQR1_SQ4_3      ((uint32_t)0x08000000) /*!< ADC SQ4 bit 3 */
#define ADC_SQR1_SQ4_4      ((uint32_t)0x10000000) /*!< ADC SQ4 bit 4 */

/********************  Bit definition for ADC_SQR2 register  ********************/
#define ADC_SQR2_SQ5        ((uint32_t)0x0000001F) /*!< ADC 5th conversion in regular sequence */
#define ADC_SQR2_SQ5_0      ((uint32_t)0x00000001) /*!< ADC SQ5 bit 0 */
#define ADC_SQR2_SQ5_1      ((uint32_t)0x00000002) /*!< ADC SQ5 bit 1 */
#define ADC_SQR2_SQ5_2      ((uint32_t)0x00000004) /*!< ADC SQ5 bit 2 */
#define ADC_SQR2_SQ5_3      ((uint32_t)0x00000008) /*!< ADC SQ5 bit 3 */
#define ADC_SQR2_SQ5_4      ((uint32_t)0x00000010) /*!< ADC SQ5 bit 4 */

#define ADC_SQR2_SQ6        ((uint32_t)0x000007C0) /*!< ADC 6th conversion in regular sequence */
#define ADC_SQR2_SQ6_0      ((uint32_t)0x00000040) /*!< ADC SQ6 bit 0 */
#define ADC_SQR2_SQ6_1      ((uint32_t)0x00000080) /*!< ADC SQ6 bit 1 */
#define ADC_SQR2_SQ6_2      ((uint32_t)0x00000100) /*!< ADC SQ6 bit 2 */
#define ADC_SQR2_SQ6_3      ((uint32_t)0x00000200) /*!< ADC SQ6 bit 3 */
#define ADC_SQR2_SQ6_4      ((uint32_t)0x00000400) /*!< ADC SQ6 bit 4 */

#define ADC_SQR2_SQ7        ((uint32_t)0x0001F000) /*!< ADC 7th conversion in regular sequence */
#define ADC_SQR2_SQ7_0      ((uint32_t)0x00001000) /*!< ADC SQ7 bit 0 */
#define ADC_SQR2_SQ7_1      ((uint32_t)0x00002000) /*!< ADC SQ7 bit 1 */
#define ADC_SQR2_SQ7_2      ((uint32_t)0x00004000) /*!< ADC SQ7 bit 2 */
#define ADC_SQR2_SQ7_3      ((uint32_t)0x00008000) /*!< ADC SQ7 bit 3 */
#define ADC_SQR2_SQ7_4      ((uint32_t)0x00010000) /*!< ADC SQ7 bit 4 */

#define ADC_SQR2_SQ8        ((uint32_t)0x007C0000) /*!< ADC 8th conversion in regular sequence */
#define ADC_SQR2_SQ8_0      ((uint32_t)0x00040000) /*!< ADC SQ8 bit 0 */
#define ADC_SQR2_SQ8_1      ((uint32_t)0x00080000) /*!< ADC SQ8 bit 1 */
#define ADC_SQR2_SQ8_2      ((uint32_t)0x00100000) /*!< ADC SQ8 bit 2 */
#define ADC_SQR2_SQ8_3      ((uint32_t)0x00200000) /*!< ADC SQ8 bit 3 */
#define ADC_SQR2_SQ8_4      ((uint32_t)0x00400000) /*!< ADC SQ8 bit 4 */

#define ADC_SQR2_SQ9        ((uint32_t)0x1F000000) /*!< ADC 9th conversion in regular sequence */
#define ADC_SQR2_SQ9_0      ((uint32_t)0x01000000) /*!< ADC SQ9 bit 0 */
#define ADC_SQR2_SQ9_1      ((uint32_t)0x02000000) /*!< ADC SQ9 bit 1 */
#define ADC_SQR2_SQ9_2      ((uint32_t)0x04000000) /*!< ADC SQ9 bit 2 */
#define ADC_SQR2_SQ9_3      ((uint32_t)0x08000000) /*!< ADC SQ9 bit 3 */
#define ADC_SQR2_SQ9_4      ((uint32_t)0x10000000) /*!< ADC SQ9 bit 4 */

/********************  Bit definition for ADC_SQR3 register  ********************/
#define ADC_SQR3_SQ10       ((uint32_t)0x0000001F) /*!< ADC 10th conversion in regular sequence */
#define ADC_SQR3_SQ10_0     ((uint32_t)0x00000001) /*!< ADC SQ10 bit 0 */
#define ADC_SQR3_SQ10_1     ((uint32_t)0x00000002) /*!< ADC SQ10 bit 1 */
#define ADC_SQR3_SQ10_2     ((uint32_t)0x00000004) /*!< ADC SQ10 bit 2 */
#define ADC_SQR3_SQ10_3     ((uint32_t)0x00000008) /*!< ADC SQ10 bit 3 */
#define ADC_SQR3_SQ10_4     ((uint32_t)0x00000010) /*!< ADC SQ10 bit 4 */

#define ADC_SQR3_SQ11       ((uint32_t)0x000007C0) /*!< ADC 11th conversion in regular sequence */
#define ADC_SQR3_SQ11_0     ((uint32_t)0x00000040) /*!< ADC SQ11 bit 0 */
#define ADC_SQR3_SQ11_1     ((uint32_t)0x00000080) /*!< ADC SQ11 bit 1 */
#define ADC_SQR3_SQ11_2     ((uint32_t)0x00000100) /*!< ADC SQ11 bit 2 */
#define ADC_SQR3_SQ11_3     ((uint32_t)0x00000200) /*!< ADC SQ11 bit 3 */
#define ADC_SQR3_SQ11_4     ((uint32_t)0x00000400) /*!< ADC SQ11 bit 4 */

#define ADC_SQR3_SQ12       ((uint32_t)0x0001F000) /*!< ADC 12th conversion in regular sequence */
#define ADC_SQR3_SQ12_0     ((uint32_t)0x00001000) /*!< ADC SQ12 bit 0 */
#define ADC_SQR3_SQ12_1     ((uint32_t)0x00002000) /*!< ADC SQ12 bit 1 */
#define ADC_SQR3_SQ12_2     ((uint32_t)0x00004000) /*!< ADC SQ12 bit 2 */
#define ADC_SQR3_SQ12_3     ((uint32_t)0x00008000) /*!< ADC SQ12 bit 3 */
#define ADC_SQR3_SQ12_4     ((uint32_t)0x00010000) /*!< ADC SQ12 bit 4 */

#define ADC_SQR3_SQ13       ((uint32_t)0x007C0000) /*!< ADC 13th conversion in regular sequence */
#define ADC_SQR3_SQ13_0     ((uint32_t)0x00040000) /*!< ADC SQ13 bit 0 */
#define ADC_SQR3_SQ13_1     ((uint32_t)0x00080000) /*!< ADC SQ13 bit 1 */
#define ADC_SQR3_SQ13_2     ((uint32_t)0x00100000) /*!< ADC SQ13 bit 2 */
#define ADC_SQR3_SQ13_3     ((uint32_t)0x00200000) /*!< ADC SQ13 bit 3 */
#define ADC_SQR3_SQ13_4     ((uint32_t)0x00400000) /*!< ADC SQ13 bit 4 */

#define ADC_SQR3_SQ14       ((uint32_t)0x1F000000) /*!< ADC 14th conversion in regular sequence */
#define ADC_SQR3_SQ14_0     ((uint32_t)0x01000000) /*!< ADC SQ14 bit 0 */
#define ADC_SQR3_SQ14_1     ((uint32_t)0x02000000) /*!< ADC SQ14 bit 1 */
#define ADC_SQR3_SQ14_2     ((uint32_t)0x04000000) /*!< ADC SQ14 bit 2 */
#define ADC_SQR3_SQ14_3     ((uint32_t)0x08000000) /*!< ADC SQ14 bit 3 */
#define ADC_SQR3_SQ14_4     ((uint32_t)0x10000000) /*!< ADC SQ14 bit 4 */

/********************  Bit definition for ADC_SQR4 register  ********************/
#define ADC_SQR3_SQ15       ((uint32_t)0x0000001F) /*!< ADC 15th conversion in regular sequence */
#define ADC_SQR3_SQ15_0     ((uint32_t)0x00000001) /*!< ADC SQ15 bit 0 */
#define ADC_SQR3_SQ15_1     ((uint32_t)0x00000002) /*!< ADC SQ15 bit 1 */
#define ADC_SQR3_SQ15_2     ((uint32_t)0x00000004) /*!< ADC SQ15 bit 2 */
#define ADC_SQR3_SQ15_3     ((uint32_t)0x00000008) /*!< ADC SQ15 bit 3 */
#define ADC_SQR3_SQ15_4     ((uint32_t)0x00000010) /*!< ADC SQ105 bit 4 */

#define ADC_SQR3_SQ16       ((uint32_t)0x000007C0) /*!< ADC 16th conversion in regular sequence */
#define ADC_SQR3_SQ16_0     ((uint32_t)0x00000040) /*!< ADC SQ16 bit 0 */
#define ADC_SQR3_SQ16_1     ((uint32_t)0x00000080) /*!< ADC SQ16 bit 1 */
#define ADC_SQR3_SQ16_2     ((uint32_t)0x00000100) /*!< ADC SQ16 bit 2 */
#define ADC_SQR3_SQ16_3     ((uint32_t)0x00000200) /*!< ADC SQ16 bit 3 */
#define ADC_SQR3_SQ16_4     ((uint32_t)0x00000400) /*!< ADC SQ16 bit 4 */
/********************  Bit definition for ADC_DR register  ********************/
#define ADC_DR_RDATA        ((uint32_t)0x0000FFFF) /*!< ADC regular Data converted */
#define ADC_DR_RDATA_0      ((uint32_t)0x00000001) /*!< ADC RDATA bit 0 */
#define ADC_DR_RDATA_1      ((uint32_t)0x00000002) /*!< ADC RDATA bit 1 */
#define ADC_DR_RDATA_2      ((uint32_t)0x00000004) /*!< ADC RDATA bit 2 */
#define ADC_DR_RDATA_3      ((uint32_t)0x00000008) /*!< ADC RDATA bit 3 */
#define ADC_DR_RDATA_4      ((uint32_t)0x00000010) /*!< ADC RDATA bit 4 */
#define ADC_DR_RDATA_5      ((uint32_t)0x00000020) /*!< ADC RDATA bit 5 */
#define ADC_DR_RDATA_6      ((uint32_t)0x00000040) /*!< ADC RDATA bit 6 */
#define ADC_DR_RDATA_7      ((uint32_t)0x00000080) /*!< ADC RDATA bit 7 */
#define ADC_DR_RDATA_8      ((uint32_t)0x00000100) /*!< ADC RDATA bit 8 */
#define ADC_DR_RDATA_9      ((uint32_t)0x00000200) /*!< ADC RDATA bit 9 */
#define ADC_DR_RDATA_10     ((uint32_t)0x00000400) /*!< ADC RDATA bit 10 */
#define ADC_DR_RDATA_11     ((uint32_t)0x00000800) /*!< ADC RDATA bit 11 */
#define ADC_DR_RDATA_12     ((uint32_t)0x00001000) /*!< ADC RDATA bit 12 */
#define ADC_DR_RDATA_13     ((uint32_t)0x00002000) /*!< ADC RDATA bit 13 */
#define ADC_DR_RDATA_14     ((uint32_t)0x00004000) /*!< ADC RDATA bit 14 */
#define ADC_DR_RDATA_15     ((uint32_t)0x00008000) /*!< ADC RDATA bit 15 */

/********************  Bit definition for ADC_JSQR register  ********************/
#define ADC_JSQR_JL         ((uint32_t)0x00000003) /*!< ADC injected channel sequence length */
#define ADC_JSQR_JL_0       ((uint32_t)0x00000001) /*!< ADC JL bit 0 */
#define ADC_JSQR_JL_1       ((uint32_t)0x00000002) /*!< ADC JL bit 1 */

#define ADC_JSQR_JEXTSEL    ((uint32_t)0x0000003C) /*!< ADC external trigger selection for injected group */
#define ADC_JSQR_JEXTSEL_0  ((uint32_t)0x00000004) /*!< ADC JEXTSEL bit 0 */
#define ADC_JSQR_JEXTSEL_1  ((uint32_t)0x00000008) /*!< ADC JEXTSEL bit 1 */
#define ADC_JSQR_JEXTSEL_2  ((uint32_t)0x00000010) /*!< ADC JEXTSEL bit 2 */
#define ADC_JSQR_JEXTSEL_3  ((uint32_t)0x00000020) /*!< ADC JEXTSEL bit 3 */

#define ADC_JSQR_JEXTEN     ((uint32_t)0x000000C0) /*!< ADC external trigger enable and polarity selection for injected channels */
#define ADC_JSQR_JEXTEN_0   ((uint32_t)0x00000040) /*!< ADC JEXTEN bit 0 */
#define ADC_JSQR_JEXTEN_1   ((uint32_t)0x00000080) /*!< ADC JEXTEN bit 1 */

#define ADC_JSQR_JSQ1       ((uint32_t)0x00001F00) /*!< ADC 1st conversion in injected sequence */
#define ADC_JSQR_JSQ1_0     ((uint32_t)0x00000100) /*!< ADC JSQ1 bit 0 */
#define ADC_JSQR_JSQ1_1     ((uint32_t)0x00000200) /*!< ADC JSQ1 bit 1 */
#define ADC_JSQR_JSQ1_2     ((uint32_t)0x00000400) /*!< ADC JSQ1 bit 2 */
#define ADC_JSQR_JSQ1_3     ((uint32_t)0x00000800) /*!< ADC JSQ1 bit 3 */
#define ADC_JSQR_JSQ1_4     ((uint32_t)0x00001000) /*!< ADC JSQ1 bit 4 */

#define ADC_JSQR_JSQ2       ((uint32_t)0x0007C000) /*!< ADC 2nd conversion in injected sequence */
#define ADC_JSQR_JSQ2_0     ((uint32_t)0x00004000) /*!< ADC JSQ2 bit 0 */
#define ADC_JSQR_JSQ2_1     ((uint32_t)0x00008000) /*!< ADC JSQ2 bit 1 */
#define ADC_JSQR_JSQ2_2     ((uint32_t)0x00010000) /*!< ADC JSQ2 bit 2 */
#define ADC_JSQR_JSQ2_3     ((uint32_t)0x00020000) /*!< ADC JSQ2 bit 3 */
#define ADC_JSQR_JSQ2_4     ((uint32_t)0x00040000) /*!< ADC JSQ2 bit 4 */

#define ADC_JSQR_JSQ3       ((uint32_t)0x01F00000) /*!< ADC 3rd conversion in injected sequence */
#define ADC_JSQR_JSQ3_0     ((uint32_t)0x00100000) /*!< ADC JSQ3 bit 0 */
#define ADC_JSQR_JSQ3_1     ((uint32_t)0x00200000) /*!< ADC JSQ3 bit 1 */
#define ADC_JSQR_JSQ3_2     ((uint32_t)0x00400000) /*!< ADC JSQ3 bit 2 */
#define ADC_JSQR_JSQ3_3     ((uint32_t)0x00800000) /*!< ADC JSQ3 bit 3 */
#define ADC_JSQR_JSQ3_4     ((uint32_t)0x01000000) /*!< ADC JSQ3 bit 4 */

#define ADC_JSQR_JSQ4       ((uint32_t)0x7C000000) /*!< ADC 4th conversion in injected sequence */
#define ADC_JSQR_JSQ4_0     ((uint32_t)0x04000000) /*!< ADC JSQ4 bit 0 */
#define ADC_JSQR_JSQ4_1     ((uint32_t)0x08000000) /*!< ADC JSQ4 bit 1 */
#define ADC_JSQR_JSQ4_2     ((uint32_t)0x10000000) /*!< ADC JSQ4 bit 2 */
#define ADC_JSQR_JSQ4_3     ((uint32_t)0x20000000) /*!< ADC JSQ4 bit 3 */
#define ADC_JSQR_JSQ4_4     ((uint32_t)0x40000000) /*!< ADC JSQ4 bit 4 */

/********************  Bit definition for ADC_OFR1 register  ********************/
#define ADC_OFR1_OFFSET1    ((uint32_t)0x00000FFF) /*!< ADC data offset 1 for channel programmed into bits OFFSET1_CH[4:0] */
#define ADC_OFR1_OFFSET1_0  ((uint32_t)0x00000001) /*!< ADC OFFSET1 bit 0 */
#define ADC_OFR1_OFFSET1_1  ((uint32_t)0x00000002) /*!< ADC OFFSET1 bit 1 */
#define ADC_OFR1_OFFSET1_2  ((uint32_t)0x00000004) /*!< ADC OFFSET1 bit 2 */
#define ADC_OFR1_OFFSET1_3  ((uint32_t)0x00000008) /*!< ADC OFFSET1 bit 3 */
#define ADC_OFR1_OFFSET1_4  ((uint32_t)0x00000010) /*!< ADC OFFSET1 bit 4 */
#define ADC_OFR1_OFFSET1_5  ((uint32_t)0x00000020) /*!< ADC OFFSET1 bit 5 */
#define ADC_OFR1_OFFSET1_6  ((uint32_t)0x00000040) /*!< ADC OFFSET1 bit 6 */
#define ADC_OFR1_OFFSET1_7  ((uint32_t)0x00000080) /*!< ADC OFFSET1 bit 7 */
#define ADC_OFR1_OFFSET1_8  ((uint32_t)0x00000100) /*!< ADC OFFSET1 bit 8 */
#define ADC_OFR1_OFFSET1_9  ((uint32_t)0x00000200) /*!< ADC OFFSET1 bit 9 */
#define ADC_OFR1_OFFSET1_10 ((uint32_t)0x00000400) /*!< ADC OFFSET1 bit 10 */
#define ADC_OFR1_OFFSET1_11 ((uint32_t)0x00000800) /*!< ADC OFFSET1 bit 11 */

#define ADC_OFR1_OFFSET1_CH     ((uint32_t)0x7C000000) /*!< ADC Channel selection for the data offset 1 */
#define ADC_OFR1_OFFSET1_CH_0  ((uint32_t)0x04000000) /*!< ADC OFFSET1_CH bit 0 */
#define ADC_OFR1_OFFSET1_CH_1  ((uint32_t)0x08000000) /*!< ADC OFFSET1_CH bit 1 */
#define ADC_OFR1_OFFSET1_CH_2  ((uint32_t)0x10000000) /*!< ADC OFFSET1_CH bit 2 */
#define ADC_OFR1_OFFSET1_CH_3  ((uint32_t)0x20000000) /*!< ADC OFFSET1_CH bit 3 */
#define ADC_OFR1_OFFSET1_CH_4  ((uint32_t)0x40000000) /*!< ADC OFFSET1_CH bit 4 */

#define ADC_OFR1_OFFSET1_EN ((uint32_t)0x80000000) /*!< ADC offset 1 enable */

/********************  Bit definition for ADC_OFR2 register  ********************/
#define ADC_OFR2_OFFSET2    ((uint32_t)0x00000FFF) /*!< ADC data offset 2 for channel programmed into bits OFFSET2_CH[4:0] */
#define ADC_OFR2_OFFSET2_0  ((uint32_t)0x00000001) /*!< ADC OFFSET2 bit 0 */
#define ADC_OFR2_OFFSET2_1  ((uint32_t)0x00000002) /*!< ADC OFFSET2 bit 1 */
#define ADC_OFR2_OFFSET2_2  ((uint32_t)0x00000004) /*!< ADC OFFSET2 bit 2 */
#define ADC_OFR2_OFFSET2_3  ((uint32_t)0x00000008) /*!< ADC OFFSET2 bit 3 */
#define ADC_OFR2_OFFSET2_4  ((uint32_t)0x00000010) /*!< ADC OFFSET2 bit 4 */
#define ADC_OFR2_OFFSET2_5  ((uint32_t)0x00000020) /*!< ADC OFFSET2 bit 5 */
#define ADC_OFR2_OFFSET2_6  ((uint32_t)0x00000040) /*!< ADC OFFSET2 bit 6 */
#define ADC_OFR2_OFFSET2_7  ((uint32_t)0x00000080) /*!< ADC OFFSET2 bit 7 */
#define ADC_OFR2_OFFSET2_8  ((uint32_t)0x00000100) /*!< ADC OFFSET2 bit 8 */
#define ADC_OFR2_OFFSET2_9  ((uint32_t)0x00000200) /*!< ADC OFFSET2 bit 9 */
#define ADC_OFR2_OFFSET2_10 ((uint32_t)0x00000400) /*!< ADC OFFSET2 bit 10 */
#define ADC_OFR2_OFFSET2_11 ((uint32_t)0x00000800) /*!< ADC OFFSET2 bit 11 */

#define ADC_OFR2_OFFSET2_CH     ((uint32_t)0x7C000000) /*!< ADC Channel selection for the data offset 2 */
#define ADC_OFR2_OFFSET2_CH_0  ((uint32_t)0x04000000) /*!< ADC OFFSET2_CH bit 0 */
#define ADC_OFR2_OFFSET2_CH_1  ((uint32_t)0x08000000) /*!< ADC OFFSET2_CH bit 1 */
#define ADC_OFR2_OFFSET2_CH_2  ((uint32_t)0x10000000) /*!< ADC OFFSET2_CH bit 2 */
#define ADC_OFR2_OFFSET2_CH_3  ((uint32_t)0x20000000) /*!< ADC OFFSET2_CH bit 3 */
#define ADC_OFR2_OFFSET2_CH_4  ((uint32_t)0x40000000) /*!< ADC OFFSET2_CH bit 4 */

#define ADC_OFR2_OFFSET2_EN ((uint32_t)0x80000000) /*!< ADC offset 2 enable */

/********************  Bit definition for ADC_OFR3 register  ********************/
#define ADC_OFR3_OFFSET3    ((uint32_t)0x00000FFF) /*!< ADC data offset 3 for channel programmed into bits OFFSET3_CH[4:0] */
#define ADC_OFR3_OFFSET3_0  ((uint32_t)0x00000001) /*!< ADC OFFSET3 bit 0 */
#define ADC_OFR3_OFFSET3_1  ((uint32_t)0x00000002) /*!< ADC OFFSET3 bit 1 */
#define ADC_OFR3_OFFSET3_2  ((uint32_t)0x00000004) /*!< ADC OFFSET3 bit 2 */
#define ADC_OFR3_OFFSET3_3  ((uint32_t)0x00000008) /*!< ADC OFFSET3 bit 3 */
#define ADC_OFR3_OFFSET3_4  ((uint32_t)0x00000010) /*!< ADC OFFSET3 bit 4 */
#define ADC_OFR3_OFFSET3_5  ((uint32_t)0x00000020) /*!< ADC OFFSET3 bit 5 */
#define ADC_OFR3_OFFSET3_6  ((uint32_t)0x00000040) /*!< ADC OFFSET3 bit 6 */
#define ADC_OFR3_OFFSET3_7  ((uint32_t)0x00000080) /*!< ADC OFFSET3 bit 7 */
#define ADC_OFR3_OFFSET3_8  ((uint32_t)0x00000100) /*!< ADC OFFSET3 bit 8 */
#define ADC_OFR3_OFFSET3_9  ((uint32_t)0x00000200) /*!< ADC OFFSET3 bit 9 */
#define ADC_OFR3_OFFSET3_10 ((uint32_t)0x00000400) /*!< ADC OFFSET3 bit 10 */
#define ADC_OFR3_OFFSET3_11 ((uint32_t)0x00000800) /*!< ADC OFFSET3 bit 11 */

#define ADC_OFR3_OFFSET3_CH     ((uint32_t)0x7C000000) /*!< ADC Channel selection for the data offset 3 */
#define ADC_OFR3_OFFSET3_CH_0  ((uint32_t)0x04000000) /*!< ADC OFFSET3_CH bit 0 */
#define ADC_OFR3_OFFSET3_CH_1  ((uint32_t)0x08000000) /*!< ADC OFFSET3_CH bit 1 */
#define ADC_OFR3_OFFSET3_CH_2  ((uint32_t)0x10000000) /*!< ADC OFFSET3_CH bit 2 */
#define ADC_OFR3_OFFSET3_CH_3  ((uint32_t)0x20000000) /*!< ADC OFFSET3_CH bit 3 */
#define ADC_OFR3_OFFSET3_CH_4  ((uint32_t)0x40000000) /*!< ADC OFFSET3_CH bit 4 */

#define ADC_OFR3_OFFSET3_EN ((uint32_t)0x80000000) /*!< ADC offset 3 enable */

/********************  Bit definition for ADC_OFR4 register  ********************/
#define ADC_OFR4_OFFSET4    ((uint32_t)0x00000FFF) /*!< ADC data offset 4 for channel programmed into bits OFFSET4_CH[4:0] */
#define ADC_OFR4_OFFSET4_0  ((uint32_t)0x00000001) /*!< ADC OFFSET4 bit 0 */
#define ADC_OFR4_OFFSET4_1  ((uint32_t)0x00000002) /*!< ADC OFFSET4 bit 1 */
#define ADC_OFR4_OFFSET4_2  ((uint32_t)0x00000004) /*!< ADC OFFSET4 bit 2 */
#define ADC_OFR4_OFFSET4_3  ((uint32_t)0x00000008) /*!< ADC OFFSET4 bit 3 */
#define ADC_OFR4_OFFSET4_4  ((uint32_t)0x00000010) /*!< ADC OFFSET4 bit 4 */
#define ADC_OFR4_OFFSET4_5  ((uint32_t)0x00000020) /*!< ADC OFFSET4 bit 5 */
#define ADC_OFR4_OFFSET4_6  ((uint32_t)0x00000040) /*!< ADC OFFSET4 bit 6 */
#define ADC_OFR4_OFFSET4_7  ((uint32_t)0x00000080) /*!< ADC OFFSET4 bit 7 */
#define ADC_OFR4_OFFSET4_8  ((uint32_t)0x00000100) /*!< ADC OFFSET4 bit 8 */
#define ADC_OFR4_OFFSET4_9  ((uint32_t)0x00000200) /*!< ADC OFFSET4 bit 9 */
#define ADC_OFR4_OFFSET4_10 ((uint32_t)0x00000400) /*!< ADC OFFSET4 bit 10 */
#define ADC_OFR4_OFFSET4_11 ((uint32_t)0x00000800) /*!< ADC OFFSET4 bit 11 */

#define ADC_OFR4_OFFSET4_CH     ((uint32_t)0x7C000000) /*!< ADC Channel selection for the data offset 4 */
#define ADC_OFR4_OFFSET4_CH_0  ((uint32_t)0x04000000) /*!< ADC OFFSET4_CH bit 0 */
#define ADC_OFR4_OFFSET4_CH_1  ((uint32_t)0x08000000) /*!< ADC OFFSET4_CH bit 1 */
#define ADC_OFR4_OFFSET4_CH_2  ((uint32_t)0x10000000) /*!< ADC OFFSET4_CH bit 2 */
#define ADC_OFR4_OFFSET4_CH_3  ((uint32_t)0x20000000) /*!< ADC OFFSET4_CH bit 3 */
#define ADC_OFR4_OFFSET4_CH_4  ((uint32_t)0x40000000) /*!< ADC OFFSET4_CH bit 4 */

#define ADC_OFR4_OFFSET4_EN ((uint32_t)0x80000000) /*!< ADC offset 4 enable */

/********************  Bit definition for ADC_JDR1 register  ********************/
#define ADC_JDR1_JDATA      ((uint32_t)0x0000FFFF) /*!< ADC Injected DATA */
#define ADC_JDR1_JDATA_0    ((uint32_t)0x00000001) /*!< ADC JDATA bit 0 */
#define ADC_JDR1_JDATA_1    ((uint32_t)0x00000002) /*!< ADC JDATA bit 1 */
#define ADC_JDR1_JDATA_2    ((uint32_t)0x00000004) /*!< ADC JDATA bit 2 */
#define ADC_JDR1_JDATA_3    ((uint32_t)0x00000008) /*!< ADC JDATA bit 3 */
#define ADC_JDR1_JDATA_4    ((uint32_t)0x00000010) /*!< ADC JDATA bit 4 */
#define ADC_JDR1_JDATA_5    ((uint32_t)0x00000020) /*!< ADC JDATA bit 5 */
#define ADC_JDR1_JDATA_6    ((uint32_t)0x00000040) /*!< ADC JDATA bit 6 */
#define ADC_JDR1_JDATA_7    ((uint32_t)0x00000080) /*!< ADC JDATA bit 7 */
#define ADC_JDR1_JDATA_8    ((uint32_t)0x00000100) /*!< ADC JDATA bit 8 */
#define ADC_JDR1_JDATA_9    ((uint32_t)0x00000200) /*!< ADC JDATA bit 9 */
#define ADC_JDR1_JDATA_10   ((uint32_t)0x00000400) /*!< ADC JDATA bit 10 */
#define ADC_JDR1_JDATA_11   ((uint32_t)0x00000800) /*!< ADC JDATA bit 11 */
#define ADC_JDR1_JDATA_12   ((uint32_t)0x00001000) /*!< ADC JDATA bit 12 */
#define ADC_JDR1_JDATA_13   ((uint32_t)0x00002000) /*!< ADC JDATA bit 13 */
#define ADC_JDR1_JDATA_14   ((uint32_t)0x00004000) /*!< ADC JDATA bit 14 */
#define ADC_JDR1_JDATA_15   ((uint32_t)0x00008000) /*!< ADC JDATA bit 15 */

/********************  Bit definition for ADC_JDR2 register  ********************/
#define ADC_JDR2_JDATA      ((uint32_t)0x0000FFFF) /*!< ADC Injected DATA */
#define ADC_JDR2_JDATA_0    ((uint32_t)0x00000001) /*!< ADC JDATA bit 0 */
#define ADC_JDR2_JDATA_1    ((uint32_t)0x00000002) /*!< ADC JDATA bit 1 */
#define ADC_JDR2_JDATA_2    ((uint32_t)0x00000004) /*!< ADC JDATA bit 2 */
#define ADC_JDR2_JDATA_3    ((uint32_t)0x00000008) /*!< ADC JDATA bit 3 */
#define ADC_JDR2_JDATA_4    ((uint32_t)0x00000010) /*!< ADC JDATA bit 4 */
#define ADC_JDR2_JDATA_5    ((uint32_t)0x00000020) /*!< ADC JDATA bit 5 */
#define ADC_JDR2_JDATA_6    ((uint32_t)0x00000040) /*!< ADC JDATA bit 6 */
#define ADC_JDR2_JDATA_7    ((uint32_t)0x00000080) /*!< ADC JDATA bit 7 */
#define ADC_JDR2_JDATA_8    ((uint32_t)0x00000100) /*!< ADC JDATA bit 8 */
#define ADC_JDR2_JDATA_9    ((uint32_t)0x00000200) /*!< ADC JDATA bit 9 */
#define ADC_JDR2_JDATA_10   ((uint32_t)0x00000400) /*!< ADC JDATA bit 10 */
#define ADC_JDR2_JDATA_11   ((uint32_t)0x00000800) /*!< ADC JDATA bit 11 */
#define ADC_JDR2_JDATA_12   ((uint32_t)0x00001000) /*!< ADC JDATA bit 12 */
#define ADC_JDR2_JDATA_13   ((uint32_t)0x00002000) /*!< ADC JDATA bit 13 */
#define ADC_JDR2_JDATA_14   ((uint32_t)0x00004000) /*!< ADC JDATA bit 14 */
#define ADC_JDR2_JDATA_15   ((uint32_t)0x00008000) /*!< ADC JDATA bit 15 */

/********************  Bit definition for ADC_JDR3 register  ********************/
#define ADC_JDR3_JDATA      ((uint32_t)0x0000FFFF) /*!< ADC Injected DATA */
#define ADC_JDR3_JDATA_0    ((uint32_t)0x00000001) /*!< ADC JDATA bit 0 */
#define ADC_JDR3_JDATA_1    ((uint32_t)0x00000002) /*!< ADC JDATA bit 1 */
#define ADC_JDR3_JDATA_2    ((uint32_t)0x00000004) /*!< ADC JDATA bit 2 */
#define ADC_JDR3_JDATA_3    ((uint32_t)0x00000008) /*!< ADC JDATA bit 3 */
#define ADC_JDR3_JDATA_4    ((uint32_t)0x00000010) /*!< ADC JDATA bit 4 */
#define ADC_JDR3_JDATA_5    ((uint32_t)0x00000020) /*!< ADC JDATA bit 5 */
#define ADC_JDR3_JDATA_6    ((uint32_t)0x00000040) /*!< ADC JDATA bit 6 */
#define ADC_JDR3_JDATA_7    ((uint32_t)0x00000080) /*!< ADC JDATA bit 7 */
#define ADC_JDR3_JDATA_8    ((uint32_t)0x00000100) /*!< ADC JDATA bit 8 */
#define ADC_JDR3_JDATA_9    ((uint32_t)0x00000200) /*!< ADC JDATA bit 9 */
#define ADC_JDR3_JDATA_10   ((uint32_t)0x00000400) /*!< ADC JDATA bit 10 */
#define ADC_JDR3_JDATA_11   ((uint32_t)0x00000800) /*!< ADC JDATA bit 11 */
#define ADC_JDR3_JDATA_12   ((uint32_t)0x00001000) /*!< ADC JDATA bit 12 */
#define ADC_JDR3_JDATA_13   ((uint32_t)0x00002000) /*!< ADC JDATA bit 13 */
#define ADC_JDR3_JDATA_14   ((uint32_t)0x00004000) /*!< ADC JDATA bit 14 */
#define ADC_JDR3_JDATA_15   ((uint32_t)0x00008000) /*!< ADC JDATA bit 15 */

/********************  Bit definition for ADC_JDR4 register  ********************/
#define ADC_JDR4_JDATA      ((uint32_t)0x0000FFFF) /*!< ADC Injected DATA */
#define ADC_JDR4_JDATA_0    ((uint32_t)0x00000001) /*!< ADC JDATA bit 0 */
#define ADC_JDR4_JDATA_1    ((uint32_t)0x00000002) /*!< ADC JDATA bit 1 */
#define ADC_JDR4_JDATA_2    ((uint32_t)0x00000004) /*!< ADC JDATA bit 2 */
#define ADC_JDR4_JDATA_3    ((uint32_t)0x00000008) /*!< ADC JDATA bit 3 */
#define ADC_JDR4_JDATA_4    ((uint32_t)0x00000010) /*!< ADC JDATA bit 4 */
#define ADC_JDR4_JDATA_5    ((uint32_t)0x00000020) /*!< ADC JDATA bit 5 */
#define ADC_JDR4_JDATA_6    ((uint32_t)0x00000040) /*!< ADC JDATA bit 6 */
#define ADC_JDR4_JDATA_7    ((uint32_t)0x00000080) /*!< ADC JDATA bit 7 */
#define ADC_JDR4_JDATA_8    ((uint32_t)0x00000100) /*!< ADC JDATA bit 8 */
#define ADC_JDR4_JDATA_9    ((uint32_t)0x00000200) /*!< ADC JDATA bit 9 */
#define ADC_JDR4_JDATA_10   ((uint32_t)0x00000400) /*!< ADC JDATA bit 10 */
#define ADC_JDR4_JDATA_11   ((uint32_t)0x00000800) /*!< ADC JDATA bit 11 */
#define ADC_JDR4_JDATA_12   ((uint32_t)0x00001000) /*!< ADC JDATA bit 12 */
#define ADC_JDR4_JDATA_13   ((uint32_t)0x00002000) /*!< ADC JDATA bit 13 */
#define ADC_JDR4_JDATA_14   ((uint32_t)0x00004000) /*!< ADC JDATA bit 14 */
#define ADC_JDR4_JDATA_15   ((uint32_t)0x00008000) /*!< ADC JDATA bit 15 */

/********************  Bit definition for ADC_AWD2CR register  ********************/
#define ADC_AWD2CR_AWD2CH    ((uint32_t)0x0007FFFE) /*!< ADC Analog watchdog 2 channel selection */
#define ADC_AWD2CR_AWD2CH_0  ((uint32_t)0x00000002) /*!< ADC AWD2CH bit 0 */
#define ADC_AWD2CR_AWD2CH_1  ((uint32_t)0x00000004) /*!< ADC AWD2CH bit 1 */
#define ADC_AWD2CR_AWD2CH_2  ((uint32_t)0x00000008) /*!< ADC AWD2CH bit 2 */
#define ADC_AWD2CR_AWD2CH_3  ((uint32_t)0x00000010) /*!< ADC AWD2CH bit 3 */
#define ADC_AWD2CR_AWD2CH_4  ((uint32_t)0x00000020) /*!< ADC AWD2CH bit 4 */
#define ADC_AWD2CR_AWD2CH_5  ((uint32_t)0x00000040) /*!< ADC AWD2CH bit 5 */
#define ADC_AWD2CR_AWD2CH_6  ((uint32_t)0x00000080) /*!< ADC AWD2CH bit 6 */
#define ADC_AWD2CR_AWD2CH_7  ((uint32_t)0x00000100) /*!< ADC AWD2CH bit 7 */
#define ADC_AWD2CR_AWD2CH_8  ((uint32_t)0x00000200) /*!< ADC AWD2CH bit 8 */
#define ADC_AWD2CR_AWD2CH_9  ((uint32_t)0x00000400) /*!< ADC AWD2CH bit 9 */
#define ADC_AWD2CR_AWD2CH_10 ((uint32_t)0x00000800) /*!< ADC AWD2CH bit 10 */
#define ADC_AWD2CR_AWD2CH_11 ((uint32_t)0x00001000) /*!< ADC AWD2CH bit 11 */
#define ADC_AWD2CR_AWD2CH_12 ((uint32_t)0x00002000) /*!< ADC AWD2CH bit 12 */
#define ADC_AWD2CR_AWD2CH_13 ((uint32_t)0x00004000) /*!< ADC AWD2CH bit 13 */
#define ADC_AWD2CR_AWD2CH_14 ((uint32_t)0x00008000) /*!< ADC AWD2CH bit 14 */
#define ADC_AWD2CR_AWD2CH_15 ((uint32_t)0x00010000) /*!< ADC AWD2CH bit 15 */
#define ADC_AWD2CR_AWD2CH_16 ((uint32_t)0x00020000) /*!< ADC AWD2CH bit 16 */
#define ADC_AWD2CR_AWD2CH_17 ((uint32_t)0x00030000) /*!< ADC AWD2CH bit 17 */

/********************  Bit definition for ADC_AWD3CR register  ********************/
#define ADC_AWD3CR_AWD3CH    ((uint32_t)0x0007FFFE) /*!< ADC Analog watchdog 2 channel selection */
#define ADC_AWD3CR_AWD3CH_0  ((uint32_t)0x00000002) /*!< ADC AWD3CH bit 0 */
#define ADC_AWD3CR_AWD3CH_1  ((uint32_t)0x00000004) /*!< ADC AWD3CH bit 1 */
#define ADC_AWD3CR_AWD3CH_2  ((uint32_t)0x00000008) /*!< ADC AWD3CH bit 2 */
#define ADC_AWD3CR_AWD3CH_3  ((uint32_t)0x00000010) /*!< ADC AWD3CH bit 3 */
#define ADC_AWD3CR_AWD3CH_4  ((uint32_t)0x00000020) /*!< ADC AWD3CH bit 4 */
#define ADC_AWD3CR_AWD3CH_5  ((uint32_t)0x00000040) /*!< ADC AWD3CH bit 5 */
#define ADC_AWD3CR_AWD3CH_6  ((uint32_t)0x00000080) /*!< ADC AWD3CH bit 6 */
#define ADC_AWD3CR_AWD3CH_7  ((uint32_t)0x00000100) /*!< ADC AWD3CH bit 7 */
#define ADC_AWD3CR_AWD3CH_8  ((uint32_t)0x00000200) /*!< ADC AWD3CH bit 8 */
#define ADC_AWD3CR_AWD3CH_9  ((uint32_t)0x00000400) /*!< ADC AWD3CH bit 9 */
#define ADC_AWD3CR_AWD3CH_10 ((uint32_t)0x00000800) /*!< ADC AWD3CH bit 10 */
#define ADC_AWD3CR_AWD3CH_11 ((uint32_t)0x00001000) /*!< ADC AWD3CH bit 11 */
#define ADC_AWD3CR_AWD3CH_12 ((uint32_t)0x00002000) /*!< ADC AWD3CH bit 12 */
#define ADC_AWD3CR_AWD3CH_13 ((uint32_t)0x00004000) /*!< ADC AWD3CH bit 13 */
#define ADC_AWD3CR_AWD3CH_14 ((uint32_t)0x00008000) /*!< ADC AWD3CH bit 14 */
#define ADC_AWD3CR_AWD3CH_15 ((uint32_t)0x00010000) /*!< ADC AWD3CH bit 15 */
#define ADC_AWD3CR_AWD3CH_16 ((uint32_t)0x00020000) /*!< ADC AWD3CH bit 16 */
#define ADC_AWD3CR_AWD3CH_17 ((uint32_t)0x00030000) /*!< ADC AWD3CH bit 17 */

/********************  Bit definition for ADC_DIFSEL register  ********************/
#define ADC_DIFSEL_DIFSEL    ((uint32_t)0x0007FFFE) /*!< ADC differential modes for channels 1 to 18 */
#define ADC_DIFSEL_DIFSEL_0  ((uint32_t)0x00000002) /*!< ADC DIFSEL bit 0 */
#define ADC_DIFSEL_DIFSEL_1  ((uint32_t)0x00000004) /*!< ADC DIFSEL bit 1 */
#define ADC_DIFSEL_DIFSEL_2  ((uint32_t)0x00000008) /*!< ADC DIFSEL bit 2 */
#define ADC_DIFSEL_DIFSEL_3  ((uint32_t)0x00000010) /*!< ADC DIFSEL bit 3 */
#define ADC_DIFSEL_DIFSEL_4  ((uint32_t)0x00000020) /*!< ADC DIFSEL bit 4 */
#define ADC_DIFSEL_DIFSEL_5  ((uint32_t)0x00000040) /*!< ADC DIFSEL bit 5 */
#define ADC_DIFSEL_DIFSEL_6  ((uint32_t)0x00000080) /*!< ADC DIFSEL bit 6 */
#define ADC_DIFSEL_DIFSEL_7  ((uint32_t)0x00000100) /*!< ADC DIFSEL bit 7 */
#define ADC_DIFSEL_DIFSEL_8  ((uint32_t)0x00000200) /*!< ADC DIFSEL bit 8 */
#define ADC_DIFSEL_DIFSEL_9  ((uint32_t)0x00000400) /*!< ADC DIFSEL bit 9 */
#define ADC_DIFSEL_DIFSEL_10 ((uint32_t)0x00000800) /*!< ADC DIFSEL bit 10 */
#define ADC_DIFSEL_DIFSEL_11 ((uint32_t)0x00001000) /*!< ADC DIFSEL bit 11 */
#define ADC_DIFSEL_DIFSEL_12 ((uint32_t)0x00002000) /*!< ADC DIFSEL bit 12 */
#define ADC_DIFSEL_DIFSEL_13 ((uint32_t)0x00004000) /*!< ADC DIFSEL bit 13 */
#define ADC_DIFSEL_DIFSEL_14 ((uint32_t)0x00008000) /*!< ADC DIFSEL bit 14 */
#define ADC_DIFSEL_DIFSEL_15 ((uint32_t)0x00010000) /*!< ADC DIFSEL bit 15 */
#define ADC_DIFSEL_DIFSEL_16 ((uint32_t)0x00020000) /*!< ADC DIFSEL bit 16 */
#define ADC_DIFSEL_DIFSEL_17 ((uint32_t)0x00030000) /*!< ADC DIFSEL bit 17 */

/********************  Bit definition for ADC_CALFACT register  ********************/
#define ADC_CALFACT_CALFACT_S   ((uint32_t)0x0000007F) /*!< ADC calibration factors in single-ended mode */
#define ADC_CALFACT_CALFACT_S_0 ((uint32_t)0x00000001) /*!< ADC CALFACT_S bit 0 */
#define ADC_CALFACT_CALFACT_S_1 ((uint32_t)0x00000002) /*!< ADC CALFACT_S bit 1 */
#define ADC_CALFACT_CALFACT_S_2 ((uint32_t)0x00000004) /*!< ADC CALFACT_S bit 2 */
#define ADC_CALFACT_CALFACT_S_3 ((uint32_t)0x00000008) /*!< ADC CALFACT_S bit 3 */
#define ADC_CALFACT_CALFACT_S_4 ((uint32_t)0x00000010) /*!< ADC CALFACT_S bit 4 */
#define ADC_CALFACT_CALFACT_S_5 ((uint32_t)0x00000020) /*!< ADC CALFACT_S bit 5 */
#define ADC_CALFACT_CALFACT_S_6 ((uint32_t)0x00000040) /*!< ADC CALFACT_S bit 6 */
#define ADC_CALFACT_CALFACT_D   ((uint32_t)0x007F0000) /*!< ADC calibration factors in differential mode */
#define ADC_CALFACT_CALFACT_D_0 ((uint32_t)0x00010000) /*!< ADC CALFACT_D bit 0 */
#define ADC_CALFACT_CALFACT_D_1 ((uint32_t)0x00020000) /*!< ADC CALFACT_D bit 1 */
#define ADC_CALFACT_CALFACT_D_2 ((uint32_t)0x00040000) /*!< ADC CALFACT_D bit 2 */
#define ADC_CALFACT_CALFACT_D_3 ((uint32_t)0x00080000) /*!< ADC CALFACT_D bit 3 */
#define ADC_CALFACT_CALFACT_D_4 ((uint32_t)0x00100000) /*!< ADC CALFACT_D bit 4 */
#define ADC_CALFACT_CALFACT_D_5 ((uint32_t)0x00200000) /*!< ADC CALFACT_D bit 5 */
#define ADC_CALFACT_CALFACT_D_6 ((uint32_t)0x00400000) /*!< ADC CALFACT_D bit 6 */

/*************************  ADC Common registers  *****************************/
/********************  Bit definition for ADC_CSR register  ********************/
#define ADC123_CSR_ADRDY_MST         ((uint32_t)0x00000001) /*!< Master ADC ready */
#define ADC123_CSR_ADRDY_EOSMP_MST   ((uint32_t)0x00000002) /*!< End of sampling phase flag of the master ADC */
#define ADC123_CSR_ADRDY_EOC_MST     ((uint32_t)0x00000004) /*!< End of regular conversion of the master ADC */
#define ADC123_CSR_ADRDY_EOS_MST     ((uint32_t)0x00000008) /*!< End of regular sequence flag of the master ADC */
#define ADC123_CSR_ADRDY_OVR_MST     ((uint32_t)0x00000010) /*!< Overrun flag of the master ADC */
#define ADC123_CSR_ADRDY_JEOC_MST    ((uint32_t)0x00000020) /*!< End of injected conversion of the master ADC */
#define ADC123_CSR_ADRDY_JEOS_MST    ((uint32_t)0x00000040) /*!< End of injected sequence flag of the master ADC */
#define ADC123_CSR_AWD1_MST          ((uint32_t)0x00000080) /*!< Analog watchdog 1 flag of the master ADC */
#define ADC123_CSR_AWD2_MST          ((uint32_t)0x00000100) /*!< Analog watchdog 2 flag of the master ADC */
#define ADC123_CSR_AWD3_MST          ((uint32_t)0x00000200) /*!< Analog watchdog 3 flag of the master ADC */
#define ADC123_CSR_JQOVF_MST         ((uint32_t)0x00000400) /*!< Injected context queue overflow flag of the master ADC */
#define ADC123_CSR_ADRDY_SLV         ((uint32_t)0x00010000) /*!< Slave ADC ready */
#define ADC123_CSR_ADRDY_EOSMP_SLV   ((uint32_t)0x00020000) /*!< End of sampling phase flag of the slave ADC */
#define ADC123_CSR_ADRDY_EOC_SLV     ((uint32_t)0x00040000) /*!< End of regular conversion of the slave ADC */
#define ADC123_CSR_ADRDY_EOS_SLV     ((uint32_t)0x00080000) /*!< End of regular sequence flag of the slave ADC */
#define ADC123_CSR_ADRDY_OVR_SLV     ((uint32_t)0x00100000) /*!< Overrun flag of the slave ADC */
#define ADC123_CSR_ADRDY_JEOC_SLV    ((uint32_t)0x00200000) /*!< End of injected conversion of the slave ADC */
#define ADC123_CSR_ADRDY_JEOS_SLV    ((uint32_t)0x00400000) /*!< End of injected sequence flag of the slave ADC */
#define ADC123_CSR_AWD1_SLV          ((uint32_t)0x00800000) /*!< Analog watchdog 1 flag of the slave ADC */
#define ADC123_CSR_AWD2_SLV          ((uint32_t)0x01000000) /*!< Analog watchdog 2 flag of the slave ADC */
#define ADC123_CSR_AWD3_SLV          ((uint32_t)0x02000000) /*!< Analog watchdog 3 flag of the slave ADC */
#define ADC123_CSR_JQOVF_SLV         ((uint32_t)0x04000000) /*!< Injected context queue overflow flag of the slave ADC */

/********************  Bit definition for ADC_CCR register  ********************/
#define ADC123_CCR_DUAL             ((uint32_t)0x0000001F) /*!< Dual ADC mode selection */
#define ADC123_CCR_DUAL_0           ((uint32_t)0x00000001) /*!< Dual bit 0 */
#define ADC123_CCR_DUAL_1           ((uint32_t)0x00000002) /*!< Dual bit 1 */
#define ADC123_CCR_DUAL_2           ((uint32_t)0x00000004) /*!< Dual bit 2 */
#define ADC123_CCR_DUAL_3           ((uint32_t)0x00000008) /*!< Dual bit 3 */
#define ADC123_CCR_DUAL_4           ((uint32_t)0x00000010) /*!< Dual bit 4 */

#define ADC123_CCR_DELAY             ((uint32_t)0x00000F00) /*!< Delay between 2 sampling phases */
#define ADC123_CCR_DELAY_0           ((uint32_t)0x00000100) /*!< DELAY bit 0 */
#define ADC123_CCR_DELAY_1           ((uint32_t)0x00000200) /*!< DELAY bit 1 */
#define ADC123_CCR_DELAY_2           ((uint32_t)0x00000400) /*!< DELAY bit 2 */
#define ADC123_CCR_DELAY_3           ((uint32_t)0x00000800) /*!< DELAY bit 3 */

#define ADC123_CCR_DMACFG            ((uint32_t)0x00002000) /*!< DMA configuration for multi-ADC mode */

#define ADC123_CCR_MDMA              ((uint32_t)0x0000C000) /*!< DMA mode for multi-ADC mode */
#define ADC123_CCR_MDMA_0            ((uint32_t)0x00004000) /*!< MDMA bit 0 */
#define ADC123_CCR_MDMA_1            ((uint32_t)0x00008000) /*!< MDMA bit 1 */

#define ADC123_CCR_CKMODE            ((uint32_t)0x00030000) /*!< ADC clock mode */
#define ADC123_CCR_CKMODE_0          ((uint32_t)0x00010000) /*!< CKMODE bit 0 */
#define ADC123_CCR_CKMODE_1          ((uint32_t)0x00020000) /*!< CKMODE bit 1 */

#define ADC123_CCR_PRESC             ((uint32_t)0x00000F00) /*!< ADC prescaler */
#define ADC123_CCR_PRESC_0           ((uint32_t)0x00000100) /*!< ADC prescaler bit 0 */
#define ADC123_CCR_PRESC_1           ((uint32_t)0x00000200) /*!< ADC prescaler bit 1 */
#define ADC123_CCR_PRESC_2           ((uint32_t)0x00000400) /*!< ADC prescaler bit 2 */
#define ADC123_CCR_PRESC_3           ((uint32_t)0x00000800) /*!< ADC prescaler bit 3 */

#define ADC123_CCR_VREFEN            ((uint32_t)0x00400000) /*!< VREFINT enable */
#define ADC123_CCR_TSEN              ((uint32_t)0x00800000) /*!< Temperature sensor enable */
#define ADC123_CCR_VBATEN            ((uint32_t)0x01000000) /*!< VBAT enable */

/********************  Bit definition for ADC_CDR register  ********************/
#define ADC123_CDR_RDATA_MST         ((uint32_t)0x0000FFFF) /*!< Regular Data of the master ADC */
#define ADC123_CDR_RDATA_MST_0       ((uint32_t)0x00000001) /*!< RDATA_MST bit 0 */
#define ADC123_CDR_RDATA_MST_1       ((uint32_t)0x00000002) /*!< RDATA_MST bit 1 */
#define ADC123_CDR_RDATA_MST_2       ((uint32_t)0x00000004) /*!< RDATA_MST bit 2 */
#define ADC123_CDR_RDATA_MST_3       ((uint32_t)0x00000008) /*!< RDATA_MST bit 3 */
#define ADC123_CDR_RDATA_MST_4       ((uint32_t)0x00000010) /*!< RDATA_MST bit 4 */
#define ADC123_CDR_RDATA_MST_5       ((uint32_t)0x00000020) /*!< RDATA_MST bit 5 */
#define ADC123_CDR_RDATA_MST_6       ((uint32_t)0x00000040) /*!< RDATA_MST bit 6 */
#define ADC123_CDR_RDATA_MST_7       ((uint32_t)0x00000080) /*!< RDATA_MST bit 7 */
#define ADC123_CDR_RDATA_MST_8       ((uint32_t)0x00000100) /*!< RDATA_MST bit 8 */
#define ADC123_CDR_RDATA_MST_9       ((uint32_t)0x00000200) /*!< RDATA_MST bit 9 */
#define ADC123_CDR_RDATA_MST_10      ((uint32_t)0x00000400) /*!< RDATA_MST bit 10 */
#define ADC123_CDR_RDATA_MST_11      ((uint32_t)0x00000800) /*!< RDATA_MST bit 11 */
#define ADC123_CDR_RDATA_MST_12      ((uint32_t)0x00001000) /*!< RDATA_MST bit 12 */
#define ADC123_CDR_RDATA_MST_13      ((uint32_t)0x00002000) /*!< RDATA_MST bit 13 */
#define ADC123_CDR_RDATA_MST_14      ((uint32_t)0x00004000) /*!< RDATA_MST bit 14 */
#define ADC123_CDR_RDATA_MST_15      ((uint32_t)0x00008000) /*!< RDATA_MST bit 15 */

#define ADC123_CDR_RDATA_SLV         ((uint32_t)0xFFFF0000) /*!< Regular Data of the master ADC */
#define ADC123_CDR_RDATA_SLV_0       ((uint32_t)0x00010000) /*!< RDATA_SLV bit 0 */
#define ADC123_CDR_RDATA_SLV_1       ((uint32_t)0x00020000) /*!< RDATA_SLV bit 1 */
#define ADC123_CDR_RDATA_SLV_2       ((uint32_t)0x00040000) /*!< RDATA_SLV bit 2 */
#define ADC123_CDR_RDATA_SLV_3       ((uint32_t)0x00080000) /*!< RDATA_SLV bit 3 */
#define ADC123_CDR_RDATA_SLV_4       ((uint32_t)0x00100000) /*!< RDATA_SLV bit 4 */
#define ADC123_CDR_RDATA_SLV_5       ((uint32_t)0x00200000) /*!< RDATA_SLV bit 5 */
#define ADC123_CDR_RDATA_SLV_6       ((uint32_t)0x00400000) /*!< RDATA_SLV bit 6 */
#define ADC123_CDR_RDATA_SLV_7       ((uint32_t)0x00800000) /*!< RDATA_SLV bit 7 */
#define ADC123_CDR_RDATA_SLV_8       ((uint32_t)0x01000000) /*!< RDATA_SLV bit 8 */
#define ADC123_CDR_RDATA_SLV_9       ((uint32_t)0x02000000) /*!< RDATA_SLV bit 9 */
#define ADC123_CDR_RDATA_SLV_10      ((uint32_t)0x04000000) /*!< RDATA_SLV bit 10 */
#define ADC123_CDR_RDATA_SLV_11      ((uint32_t)0x08000000) /*!< RDATA_SLV bit 11 */
#define ADC123_CDR_RDATA_SLV_12      ((uint32_t)0x10000000) /*!< RDATA_SLV bit 12 */
#define ADC123_CDR_RDATA_SLV_13      ((uint32_t)0x20000000) /*!< RDATA_SLV bit 13 */
#define ADC123_CDR_RDATA_SLV_14      ((uint32_t)0x40000000) /*!< RDATA_SLV bit 14 */
#define ADC123_CDR_RDATA_SLV_15      ((uint32_t)0x80000000) /*!< RDATA_SLV bit 15 */

/******************************************************************************/
/*                                                                            */
/*                         Controller Area Network                            */
/*                                                                            */
/******************************************************************************/
/*!<CAN control and status registers */
/*******************  Bit definition for CAN_MCR register  ********************/
#define  CAN_MCR_INRQ                        ((uint16_t)0x0001)            /*!<Initialization Request */
#define  CAN_MCR_SLEEP                       ((uint16_t)0x0002)            /*!<Sleep Mode Request */
#define  CAN_MCR_TXFP                        ((uint16_t)0x0004)            /*!<Transmit FIFO Priority */
#define  CAN_MCR_RFLM                        ((uint16_t)0x0008)            /*!<Receive FIFO Locked Mode */
#define  CAN_MCR_NART                        ((uint16_t)0x0010)            /*!<No Automatic Retransmission */
#define  CAN_MCR_AWUM                        ((uint16_t)0x0020)            /*!<Automatic Wakeup Mode */
#define  CAN_MCR_ABOM                        ((uint16_t)0x0040)            /*!<Automatic Bus-Off Management */
#define  CAN_MCR_TTCM                        ((uint16_t)0x0080)            /*!<Time Triggered Communication Mode */
#define  CAN_MCR_RESET                       ((uint16_t)0x8000)            /*!<bxCAN software master reset */

/*******************  Bit definition for CAN_MSR register  ********************/
#define  CAN_MSR_INAK                        ((uint16_t)0x0001)            /*!<Initialization Acknowledge */
#define  CAN_MSR_SLAK                        ((uint16_t)0x0002)            /*!<Sleep Acknowledge */
#define  CAN_MSR_ERRI                        ((uint16_t)0x0004)            /*!<Error Interrupt */
#define  CAN_MSR_WKUI                        ((uint16_t)0x0008)            /*!<Wakeup Interrupt */
#define  CAN_MSR_SLAKI                       ((uint16_t)0x0010)            /*!<Sleep Acknowledge Interrupt */
#define  CAN_MSR_TXM                         ((uint16_t)0x0100)            /*!<Transmit Mode */
#define  CAN_MSR_RXM                         ((uint16_t)0x0200)            /*!<Receive Mode */
#define  CAN_MSR_SAMP                        ((uint16_t)0x0400)            /*!<Last Sample Point */
#define  CAN_MSR_RX                          ((uint16_t)0x0800)            /*!<CAN Rx Signal */

/*******************  Bit definition for CAN_TSR register  ********************/
#define  CAN_TSR_RQCP0                       ((uint32_t)0x00000001)        /*!<Request Completed Mailbox0 */
#define  CAN_TSR_TXOK0                       ((uint32_t)0x00000002)        /*!<Transmission OK of Mailbox0 */
#define  CAN_TSR_ALST0                       ((uint32_t)0x00000004)        /*!<Arbitration Lost for Mailbox0 */
#define  CAN_TSR_TERR0                       ((uint32_t)0x00000008)        /*!<Transmission Error of Mailbox0 */
#define  CAN_TSR_ABRQ0                       ((uint32_t)0x00000080)        /*!<Abort Request for Mailbox0 */
#define  CAN_TSR_RQCP1                       ((uint32_t)0x00000100)        /*!<Request Completed Mailbox1 */
#define  CAN_TSR_TXOK1                       ((uint32_t)0x00000200)        /*!<Transmission OK of Mailbox1 */
#define  CAN_TSR_ALST1                       ((uint32_t)0x00000400)        /*!<Arbitration Lost for Mailbox1 */
#define  CAN_TSR_TERR1                       ((uint32_t)0x00000800)        /*!<Transmission Error of Mailbox1 */
#define  CAN_TSR_ABRQ1                       ((uint32_t)0x00008000)        /*!<Abort Request for Mailbox 1 */
#define  CAN_TSR_RQCP2                       ((uint32_t)0x00010000)        /*!<Request Completed Mailbox2 */
#define  CAN_TSR_TXOK2                       ((uint32_t)0x00020000)        /*!<Transmission OK of Mailbox 2 */
#define  CAN_TSR_ALST2                       ((uint32_t)0x00040000)        /*!<Arbitration Lost for mailbox 2 */
#define  CAN_TSR_TERR2                       ((uint32_t)0x00080000)        /*!<Transmission Error of Mailbox 2 */
#define  CAN_TSR_ABRQ2                       ((uint32_t)0x00800000)        /*!<Abort Request for Mailbox 2 */
#define  CAN_TSR_CODE                        ((uint32_t)0x03000000)        /*!<Mailbox Code */

#define  CAN_TSR_TME                         ((uint32_t)0x1C000000)        /*!<TME[2:0] bits */
#define  CAN_TSR_TME0                        ((uint32_t)0x04000000)        /*!<Transmit Mailbox 0 Empty */
#define  CAN_TSR_TME1                        ((uint32_t)0x08000000)        /*!<Transmit Mailbox 1 Empty */
#define  CAN_TSR_TME2                        ((uint32_t)0x10000000)        /*!<Transmit Mailbox 2 Empty */

#define  CAN_TSR_LOW                         ((uint32_t)0xE0000000)        /*!<LOW[2:0] bits */
#define  CAN_TSR_LOW0                        ((uint32_t)0x20000000)        /*!<Lowest Priority Flag for Mailbox 0 */
#define  CAN_TSR_LOW1                        ((uint32_t)0x40000000)        /*!<Lowest Priority Flag for Mailbox 1 */
#define  CAN_TSR_LOW2                        ((uint32_t)0x80000000)        /*!<Lowest Priority Flag for Mailbox 2 */

/*******************  Bit definition for CAN_RF0R register  *******************/
#define  CAN_RF0R_FMP0                       ((uint8_t)0x03)               /*!<FIFO 0 Message Pending */
#define  CAN_RF0R_FULL0                      ((uint8_t)0x08)               /*!<FIFO 0 Full */
#define  CAN_RF0R_FOVR0                      ((uint8_t)0x10)               /*!<FIFO 0 Overrun */
#define  CAN_RF0R_RFOM0                      ((uint8_t)0x20)               /*!<Release FIFO 0 Output Mailbox */

/*******************  Bit definition for CAN_RF1R register  *******************/
#define  CAN_RF1R_FMP1                       ((uint8_t)0x03)               /*!<FIFO 1 Message Pending */
#define  CAN_RF1R_FULL1                      ((uint8_t)0x08)               /*!<FIFO 1 Full */
#define  CAN_RF1R_FOVR1                      ((uint8_t)0x10)               /*!<FIFO 1 Overrun */
#define  CAN_RF1R_RFOM1                      ((uint8_t)0x20)               /*!<Release FIFO 1 Output Mailbox */

/********************  Bit definition for CAN_IER register  *******************/
#define  CAN_IER_TMEIE                       ((uint32_t)0x00000001)        /*!<Transmit Mailbox Empty Interrupt Enable */
#define  CAN_IER_FMPIE0                      ((uint32_t)0x00000002)        /*!<FIFO Message Pending Interrupt Enable */
#define  CAN_IER_FFIE0                       ((uint32_t)0x00000004)        /*!<FIFO Full Interrupt Enable */
#define  CAN_IER_FOVIE0                      ((uint32_t)0x00000008)        /*!<FIFO Overrun Interrupt Enable */
#define  CAN_IER_FMPIE1                      ((uint32_t)0x00000010)        /*!<FIFO Message Pending Interrupt Enable */
#define  CAN_IER_FFIE1                       ((uint32_t)0x00000020)        /*!<FIFO Full Interrupt Enable */
#define  CAN_IER_FOVIE1                      ((uint32_t)0x00000040)        /*!<FIFO Overrun Interrupt Enable */
#define  CAN_IER_EWGIE                       ((uint32_t)0x00000100)        /*!<Error Warning Interrupt Enable */
#define  CAN_IER_EPVIE                       ((uint32_t)0x00000200)        /*!<Error Passive Interrupt Enable */
#define  CAN_IER_BOFIE                       ((uint32_t)0x00000400)        /*!<Bus-Off Interrupt Enable */
#define  CAN_IER_LECIE                       ((uint32_t)0x00000800)        /*!<Last Error Code Interrupt Enable */
#define  CAN_IER_ERRIE                       ((uint32_t)0x00008000)        /*!<Error Interrupt Enable */
#define  CAN_IER_WKUIE                       ((uint32_t)0x00010000)        /*!<Wakeup Interrupt Enable */
#define  CAN_IER_SLKIE                       ((uint32_t)0x00020000)        /*!<Sleep Interrupt Enable */

/********************  Bit definition for CAN_ESR register  *******************/
#define  CAN_ESR_EWGF                        ((uint32_t)0x00000001)        /*!<Error Warning Flag */
#define  CAN_ESR_EPVF                        ((uint32_t)0x00000002)        /*!<Error Passive Flag */
#define  CAN_ESR_BOFF                        ((uint32_t)0x00000004)        /*!<Bus-Off Flag */

#define  CAN_ESR_LEC                         ((uint32_t)0x00000070)        /*!<LEC[2:0] bits (Last Error Code) */
#define  CAN_ESR_LEC_0                       ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  CAN_ESR_LEC_1                       ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  CAN_ESR_LEC_2                       ((uint32_t)0x00000040)        /*!<Bit 2 */

#define  CAN_ESR_TEC                         ((uint32_t)0x00FF0000)        /*!<Least significant byte of the 9-bit Transmit Error Counter */
#define  CAN_ESR_REC                         ((uint32_t)0xFF000000)        /*!<Receive Error Counter */

/*******************  Bit definition for CAN_BTR register  ********************/
#define  CAN_BTR_BRP                         ((uint32_t)0x000003FF)        /*!<Baud Rate Prescaler */
#define  CAN_BTR_TS1                         ((uint32_t)0x000F0000)        /*!<Time Segment 1 */
#define  CAN_BTR_TS2                         ((uint32_t)0x00700000)        /*!<Time Segment 2 */
#define  CAN_BTR_SJW                         ((uint32_t)0x03000000)        /*!<Resynchronization Jump Width */
#define  CAN_BTR_LBKM                        ((uint32_t)0x40000000)        /*!<Loop Back Mode (Debug) */
#define  CAN_BTR_SILM                        ((uint32_t)0x80000000)        /*!<Silent Mode */

/*!<Mailbox registers */
/******************  Bit definition for CAN_TI0R register  ********************/
#define  CAN_TI0R_TXRQ                       ((uint32_t)0x00000001)        /*!<Transmit Mailbox Request */
#define  CAN_TI0R_RTR                        ((uint32_t)0x00000002)        /*!<Remote Transmission Request */
#define  CAN_TI0R_IDE                        ((uint32_t)0x00000004)        /*!<Identifier Extension */
#define  CAN_TI0R_EXID                       ((uint32_t)0x001FFFF8)        /*!<Extended Identifier */
#define  CAN_TI0R_STID                       ((uint32_t)0xFFE00000)        /*!<Standard Identifier or Extended Identifier */

/******************  Bit definition for CAN_TDT0R register  *******************/
#define  CAN_TDT0R_DLC                       ((uint32_t)0x0000000F)        /*!<Data Length Code */
#define  CAN_TDT0R_TGT                       ((uint32_t)0x00000100)        /*!<Transmit Global Time */
#define  CAN_TDT0R_TIME                      ((uint32_t)0xFFFF0000)        /*!<Message Time Stamp */

/******************  Bit definition for CAN_TDL0R register  *******************/
#define  CAN_TDL0R_DATA0                     ((uint32_t)0x000000FF)        /*!<Data byte 0 */
#define  CAN_TDL0R_DATA1                     ((uint32_t)0x0000FF00)        /*!<Data byte 1 */
#define  CAN_TDL0R_DATA2                     ((uint32_t)0x00FF0000)        /*!<Data byte 2 */
#define  CAN_TDL0R_DATA3                     ((uint32_t)0xFF000000)        /*!<Data byte 3 */

/******************  Bit definition for CAN_TDH0R register  *******************/
#define  CAN_TDH0R_DATA4                     ((uint32_t)0x000000FF)        /*!<Data byte 4 */
#define  CAN_TDH0R_DATA5                     ((uint32_t)0x0000FF00)        /*!<Data byte 5 */
#define  CAN_TDH0R_DATA6                     ((uint32_t)0x00FF0000)        /*!<Data byte 6 */
#define  CAN_TDH0R_DATA7                     ((uint32_t)0xFF000000)        /*!<Data byte 7 */

/*******************  Bit definition for CAN_TI1R register  *******************/
#define  CAN_TI1R_TXRQ                       ((uint32_t)0x00000001)        /*!<Transmit Mailbox Request */
#define  CAN_TI1R_RTR                        ((uint32_t)0x00000002)        /*!<Remote Transmission Request */
#define  CAN_TI1R_IDE                        ((uint32_t)0x00000004)        /*!<Identifier Extension */
#define  CAN_TI1R_EXID                       ((uint32_t)0x001FFFF8)        /*!<Extended Identifier */
#define  CAN_TI1R_STID                       ((uint32_t)0xFFE00000)        /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_TDT1R register  ******************/
#define  CAN_TDT1R_DLC                       ((uint32_t)0x0000000F)        /*!<Data Length Code */
#define  CAN_TDT1R_TGT                       ((uint32_t)0x00000100)        /*!<Transmit Global Time */
#define  CAN_TDT1R_TIME                      ((uint32_t)0xFFFF0000)        /*!<Message Time Stamp */

/*******************  Bit definition for CAN_TDL1R register  ******************/
#define  CAN_TDL1R_DATA0                     ((uint32_t)0x000000FF)        /*!<Data byte 0 */
#define  CAN_TDL1R_DATA1                     ((uint32_t)0x0000FF00)        /*!<Data byte 1 */
#define  CAN_TDL1R_DATA2                     ((uint32_t)0x00FF0000)        /*!<Data byte 2 */
#define  CAN_TDL1R_DATA3                     ((uint32_t)0xFF000000)        /*!<Data byte 3 */

/*******************  Bit definition for CAN_TDH1R register  ******************/
#define  CAN_TDH1R_DATA4                     ((uint32_t)0x000000FF)        /*!<Data byte 4 */
#define  CAN_TDH1R_DATA5                     ((uint32_t)0x0000FF00)        /*!<Data byte 5 */
#define  CAN_TDH1R_DATA6                     ((uint32_t)0x00FF0000)        /*!<Data byte 6 */
#define  CAN_TDH1R_DATA7                     ((uint32_t)0xFF000000)        /*!<Data byte 7 */

/*******************  Bit definition for CAN_TI2R register  *******************/
#define  CAN_TI2R_TXRQ                       ((uint32_t)0x00000001)        /*!<Transmit Mailbox Request */
#define  CAN_TI2R_RTR                        ((uint32_t)0x00000002)        /*!<Remote Transmission Request */
#define  CAN_TI2R_IDE                        ((uint32_t)0x00000004)        /*!<Identifier Extension */
#define  CAN_TI2R_EXID                       ((uint32_t)0x001FFFF8)        /*!<Extended identifier */
#define  CAN_TI2R_STID                       ((uint32_t)0xFFE00000)        /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_TDT2R register  ******************/  
#define  CAN_TDT2R_DLC                       ((uint32_t)0x0000000F)        /*!<Data Length Code */
#define  CAN_TDT2R_TGT                       ((uint32_t)0x00000100)        /*!<Transmit Global Time */
#define  CAN_TDT2R_TIME                      ((uint32_t)0xFFFF0000)        /*!<Message Time Stamp */

/*******************  Bit definition for CAN_TDL2R register  ******************/
#define  CAN_TDL2R_DATA0                     ((uint32_t)0x000000FF)        /*!<Data byte 0 */
#define  CAN_TDL2R_DATA1                     ((uint32_t)0x0000FF00)        /*!<Data byte 1 */
#define  CAN_TDL2R_DATA2                     ((uint32_t)0x00FF0000)        /*!<Data byte 2 */
#define  CAN_TDL2R_DATA3                     ((uint32_t)0xFF000000)        /*!<Data byte 3 */

/*******************  Bit definition for CAN_TDH2R register  ******************/
#define  CAN_TDH2R_DATA4                     ((uint32_t)0x000000FF)        /*!<Data byte 4 */
#define  CAN_TDH2R_DATA5                     ((uint32_t)0x0000FF00)        /*!<Data byte 5 */
#define  CAN_TDH2R_DATA6                     ((uint32_t)0x00FF0000)        /*!<Data byte 6 */
#define  CAN_TDH2R_DATA7                     ((uint32_t)0xFF000000)        /*!<Data byte 7 */

/*******************  Bit definition for CAN_RI0R register  *******************/
#define  CAN_RI0R_RTR                        ((uint32_t)0x00000002)        /*!<Remote Transmission Request */
#define  CAN_RI0R_IDE                        ((uint32_t)0x00000004)        /*!<Identifier Extension */
#define  CAN_RI0R_EXID                       ((uint32_t)0x001FFFF8)        /*!<Extended Identifier */
#define  CAN_RI0R_STID                       ((uint32_t)0xFFE00000)        /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_RDT0R register  ******************/
#define  CAN_RDT0R_DLC                       ((uint32_t)0x0000000F)        /*!<Data Length Code */
#define  CAN_RDT0R_FMI                       ((uint32_t)0x0000FF00)        /*!<Filter Match Index */
#define  CAN_RDT0R_TIME                      ((uint32_t)0xFFFF0000)        /*!<Message Time Stamp */

/*******************  Bit definition for CAN_RDL0R register  ******************/
#define  CAN_RDL0R_DATA0                     ((uint32_t)0x000000FF)        /*!<Data byte 0 */
#define  CAN_RDL0R_DATA1                     ((uint32_t)0x0000FF00)        /*!<Data byte 1 */
#define  CAN_RDL0R_DATA2                     ((uint32_t)0x00FF0000)        /*!<Data byte 2 */
#define  CAN_RDL0R_DATA3                     ((uint32_t)0xFF000000)        /*!<Data byte 3 */

/*******************  Bit definition for CAN_RDH0R register  ******************/
#define  CAN_RDH0R_DATA4                     ((uint32_t)0x000000FF)        /*!<Data byte 4 */
#define  CAN_RDH0R_DATA5                     ((uint32_t)0x0000FF00)        /*!<Data byte 5 */
#define  CAN_RDH0R_DATA6                     ((uint32_t)0x00FF0000)        /*!<Data byte 6 */
#define  CAN_RDH0R_DATA7                     ((uint32_t)0xFF000000)        /*!<Data byte 7 */

/*******************  Bit definition for CAN_RI1R register  *******************/
#define  CAN_RI1R_RTR                        ((uint32_t)0x00000002)        /*!<Remote Transmission Request */
#define  CAN_RI1R_IDE                        ((uint32_t)0x00000004)        /*!<Identifier Extension */
#define  CAN_RI1R_EXID                       ((uint32_t)0x001FFFF8)        /*!<Extended identifier */
#define  CAN_RI1R_STID                       ((uint32_t)0xFFE00000)        /*!<Standard Identifier or Extended Identifier */

/*******************  Bit definition for CAN_RDT1R register  ******************/
#define  CAN_RDT1R_DLC                       ((uint32_t)0x0000000F)        /*!<Data Length Code */
#define  CAN_RDT1R_FMI                       ((uint32_t)0x0000FF00)        /*!<Filter Match Index */
#define  CAN_RDT1R_TIME                      ((uint32_t)0xFFFF0000)        /*!<Message Time Stamp */

/*******************  Bit definition for CAN_RDL1R register  ******************/
#define  CAN_RDL1R_DATA0                     ((uint32_t)0x000000FF)        /*!<Data byte 0 */
#define  CAN_RDL1R_DATA1                     ((uint32_t)0x0000FF00)        /*!<Data byte 1 */
#define  CAN_RDL1R_DATA2                     ((uint32_t)0x00FF0000)        /*!<Data byte 2 */
#define  CAN_RDL1R_DATA3                     ((uint32_t)0xFF000000)        /*!<Data byte 3 */

/*******************  Bit definition for CAN_RDH1R register  ******************/
#define  CAN_RDH1R_DATA4                     ((uint32_t)0x000000FF)        /*!<Data byte 4 */
#define  CAN_RDH1R_DATA5                     ((uint32_t)0x0000FF00)        /*!<Data byte 5 */
#define  CAN_RDH1R_DATA6                     ((uint32_t)0x00FF0000)        /*!<Data byte 6 */
#define  CAN_RDH1R_DATA7                     ((uint32_t)0xFF000000)        /*!<Data byte 7 */

/*!<CAN filter registers */
/*******************  Bit definition for CAN_FMR register  ********************/
#define  CAN_FMR_FINIT                       ((uint8_t)0x01)               /*!<Filter Init Mode */

/*******************  Bit definition for CAN_FM1R register  *******************/
#define  CAN_FM1R_FBM                        ((uint16_t)0x3FFF)            /*!<Filter Mode */
#define  CAN_FM1R_FBM0                       ((uint16_t)0x0001)            /*!<Filter Init Mode bit 0 */
#define  CAN_FM1R_FBM1                       ((uint16_t)0x0002)            /*!<Filter Init Mode bit 1 */
#define  CAN_FM1R_FBM2                       ((uint16_t)0x0004)            /*!<Filter Init Mode bit 2 */
#define  CAN_FM1R_FBM3                       ((uint16_t)0x0008)            /*!<Filter Init Mode bit 3 */
#define  CAN_FM1R_FBM4                       ((uint16_t)0x0010)            /*!<Filter Init Mode bit 4 */
#define  CAN_FM1R_FBM5                       ((uint16_t)0x0020)            /*!<Filter Init Mode bit 5 */
#define  CAN_FM1R_FBM6                       ((uint16_t)0x0040)            /*!<Filter Init Mode bit 6 */
#define  CAN_FM1R_FBM7                       ((uint16_t)0x0080)            /*!<Filter Init Mode bit 7 */
#define  CAN_FM1R_FBM8                       ((uint16_t)0x0100)            /*!<Filter Init Mode bit 8 */
#define  CAN_FM1R_FBM9                       ((uint16_t)0x0200)            /*!<Filter Init Mode bit 9 */
#define  CAN_FM1R_FBM10                      ((uint16_t)0x0400)            /*!<Filter Init Mode bit 10 */
#define  CAN_FM1R_FBM11                      ((uint16_t)0x0800)            /*!<Filter Init Mode bit 11 */
#define  CAN_FM1R_FBM12                      ((uint16_t)0x1000)            /*!<Filter Init Mode bit 12 */
#define  CAN_FM1R_FBM13                      ((uint16_t)0x2000)            /*!<Filter Init Mode bit 13 */

/*******************  Bit definition for CAN_FS1R register  *******************/
#define  CAN_FS1R_FSC                        ((uint16_t)0x3FFF)            /*!<Filter Scale Configuration */
#define  CAN_FS1R_FSC0                       ((uint16_t)0x0001)            /*!<Filter Scale Configuration bit 0 */
#define  CAN_FS1R_FSC1                       ((uint16_t)0x0002)            /*!<Filter Scale Configuration bit 1 */
#define  CAN_FS1R_FSC2                       ((uint16_t)0x0004)            /*!<Filter Scale Configuration bit 2 */
#define  CAN_FS1R_FSC3                       ((uint16_t)0x0008)            /*!<Filter Scale Configuration bit 3 */
#define  CAN_FS1R_FSC4                       ((uint16_t)0x0010)            /*!<Filter Scale Configuration bit 4 */
#define  CAN_FS1R_FSC5                       ((uint16_t)0x0020)            /*!<Filter Scale Configuration bit 5 */
#define  CAN_FS1R_FSC6                       ((uint16_t)0x0040)            /*!<Filter Scale Configuration bit 6 */
#define  CAN_FS1R_FSC7                       ((uint16_t)0x0080)            /*!<Filter Scale Configuration bit 7 */
#define  CAN_FS1R_FSC8                       ((uint16_t)0x0100)            /*!<Filter Scale Configuration bit 8 */
#define  CAN_FS1R_FSC9                       ((uint16_t)0x0200)            /*!<Filter Scale Configuration bit 9 */
#define  CAN_FS1R_FSC10                      ((uint16_t)0x0400)            /*!<Filter Scale Configuration bit 10 */
#define  CAN_FS1R_FSC11                      ((uint16_t)0x0800)            /*!<Filter Scale Configuration bit 11 */
#define  CAN_FS1R_FSC12                      ((uint16_t)0x1000)            /*!<Filter Scale Configuration bit 12 */
#define  CAN_FS1R_FSC13                      ((uint16_t)0x2000)            /*!<Filter Scale Configuration bit 13 */

/******************  Bit definition for CAN_FFA1R register  *******************/
#define  CAN_FFA1R_FFA                       ((uint16_t)0x3FFF)            /*!<Filter FIFO Assignment */
#define  CAN_FFA1R_FFA0                      ((uint16_t)0x0001)            /*!<Filter FIFO Assignment for Filter 0 */
#define  CAN_FFA1R_FFA1                      ((uint16_t)0x0002)            /*!<Filter FIFO Assignment for Filter 1 */
#define  CAN_FFA1R_FFA2                      ((uint16_t)0x0004)            /*!<Filter FIFO Assignment for Filter 2 */
#define  CAN_FFA1R_FFA3                      ((uint16_t)0x0008)            /*!<Filter FIFO Assignment for Filter 3 */
#define  CAN_FFA1R_FFA4                      ((uint16_t)0x0010)            /*!<Filter FIFO Assignment for Filter 4 */
#define  CAN_FFA1R_FFA5                      ((uint16_t)0x0020)            /*!<Filter FIFO Assignment for Filter 5 */
#define  CAN_FFA1R_FFA6                      ((uint16_t)0x0040)            /*!<Filter FIFO Assignment for Filter 6 */
#define  CAN_FFA1R_FFA7                      ((uint16_t)0x0080)            /*!<Filter FIFO Assignment for Filter 7 */
#define  CAN_FFA1R_FFA8                      ((uint16_t)0x0100)            /*!<Filter FIFO Assignment for Filter 8 */
#define  CAN_FFA1R_FFA9                      ((uint16_t)0x0200)            /*!<Filter FIFO Assignment for Filter 9 */
#define  CAN_FFA1R_FFA10                     ((uint16_t)0x0400)            /*!<Filter FIFO Assignment for Filter 10 */
#define  CAN_FFA1R_FFA11                     ((uint16_t)0x0800)            /*!<Filter FIFO Assignment for Filter 11 */
#define  CAN_FFA1R_FFA12                     ((uint16_t)0x1000)            /*!<Filter FIFO Assignment for Filter 12 */
#define  CAN_FFA1R_FFA13                     ((uint16_t)0x2000)            /*!<Filter FIFO Assignment for Filter 13 */

/*******************  Bit definition for CAN_FA1R register  *******************/
#define  CAN_FA1R_FACT                       ((uint16_t)0x3FFF)            /*!<Filter Active */
#define  CAN_FA1R_FACT0                      ((uint16_t)0x0001)            /*!<Filter 0 Active */
#define  CAN_FA1R_FACT1                      ((uint16_t)0x0002)            /*!<Filter 1 Active */
#define  CAN_FA1R_FACT2                      ((uint16_t)0x0004)            /*!<Filter 2 Active */
#define  CAN_FA1R_FACT3                      ((uint16_t)0x0008)            /*!<Filter 3 Active */
#define  CAN_FA1R_FACT4                      ((uint16_t)0x0010)            /*!<Filter 4 Active */
#define  CAN_FA1R_FACT5                      ((uint16_t)0x0020)            /*!<Filter 5 Active */
#define  CAN_FA1R_FACT6                      ((uint16_t)0x0040)            /*!<Filter 6 Active */
#define  CAN_FA1R_FACT7                      ((uint16_t)0x0080)            /*!<Filter 7 Active */
#define  CAN_FA1R_FACT8                      ((uint16_t)0x0100)            /*!<Filter 8 Active */
#define  CAN_FA1R_FACT9                      ((uint16_t)0x0200)            /*!<Filter 9 Active */
#define  CAN_FA1R_FACT10                     ((uint16_t)0x0400)            /*!<Filter 10 Active */
#define  CAN_FA1R_FACT11                     ((uint16_t)0x0800)            /*!<Filter 11 Active */
#define  CAN_FA1R_FACT12                     ((uint16_t)0x1000)            /*!<Filter 12 Active */
#define  CAN_FA1R_FACT13                     ((uint16_t)0x2000)            /*!<Filter 13 Active */

/*******************  Bit definition for CAN_F0R1 register  *******************/
#define  CAN_F0R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F0R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F0R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F0R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F0R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F0R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F0R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F0R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F0R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F0R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F0R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F0R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F0R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F0R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F0R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F0R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F0R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F0R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F0R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F0R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F0R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F0R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F0R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F0R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F0R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F0R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F0R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F0R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F0R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F0R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F0R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F0R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F1R1 register  *******************/
#define  CAN_F1R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F1R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F1R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F1R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F1R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F1R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F1R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F1R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F1R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F1R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F1R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F1R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F1R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F1R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F1R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F1R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F1R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F1R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F1R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F1R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F1R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F1R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F1R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F1R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F1R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F1R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F1R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F1R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F1R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F1R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F1R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F1R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F2R1 register  *******************/
#define  CAN_F2R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F2R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F2R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F2R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F2R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F2R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F2R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F2R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F2R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F2R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F2R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F2R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F2R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F2R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F2R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F2R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F2R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F2R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F2R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F2R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F2R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F2R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F2R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F2R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F2R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F2R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F2R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F2R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F2R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F2R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F2R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F2R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F3R1 register  *******************/
#define  CAN_F3R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F3R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F3R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F3R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F3R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F3R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F3R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F3R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F3R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F3R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F3R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F3R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F3R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F3R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F3R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F3R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F3R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F3R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F3R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F3R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F3R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F3R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F3R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F3R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F3R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F3R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F3R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F3R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F3R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F3R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F3R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F3R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F4R1 register  *******************/
#define  CAN_F4R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F4R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F4R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F4R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F4R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F4R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F4R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F4R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F4R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F4R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F4R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F4R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F4R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F4R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F4R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F4R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F4R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F4R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F4R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F4R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F4R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F4R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F4R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F4R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F4R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F4R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F4R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F4R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F4R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F4R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F4R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F4R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F5R1 register  *******************/
#define  CAN_F5R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F5R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F5R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F5R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F5R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F5R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F5R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F5R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F5R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F5R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F5R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F5R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F5R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F5R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F5R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F5R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F5R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F5R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F5R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F5R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F5R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F5R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F5R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F5R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F5R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F5R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F5R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F5R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F5R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F5R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F5R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F5R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F6R1 register  *******************/
#define  CAN_F6R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F6R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F6R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F6R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F6R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F6R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F6R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F6R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F6R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F6R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F6R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F6R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F6R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F6R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F6R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F6R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F6R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F6R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F6R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F6R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F6R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F6R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F6R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F6R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F6R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F6R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F6R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F6R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F6R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F6R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F6R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F6R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F7R1 register  *******************/
#define  CAN_F7R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F7R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F7R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F7R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F7R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F7R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F7R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F7R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F7R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F7R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F7R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F7R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F7R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F7R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F7R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F7R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F7R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F7R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F7R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F7R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F7R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F7R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F7R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F7R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F7R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F7R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F7R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F7R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F7R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F7R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F7R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F7R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F8R1 register  *******************/
#define  CAN_F8R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F8R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F8R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F8R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F8R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F8R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F8R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F8R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F8R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F8R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F8R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F8R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F8R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F8R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F8R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F8R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F8R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F8R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F8R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F8R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F8R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F8R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F8R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F8R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F8R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F8R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F8R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F8R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F8R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F8R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F8R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F8R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F9R1 register  *******************/
#define  CAN_F9R1_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F9R1_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F9R1_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F9R1_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F9R1_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F9R1_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F9R1_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F9R1_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F9R1_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F9R1_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F9R1_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F9R1_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F9R1_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F9R1_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F9R1_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F9R1_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F9R1_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F9R1_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F9R1_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F9R1_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F9R1_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F9R1_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F9R1_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F9R1_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F9R1_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F9R1_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F9R1_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F9R1_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F9R1_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F9R1_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F9R1_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F9R1_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F10R1 register  ******************/
#define  CAN_F10R1_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F10R1_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F10R1_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F10R1_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F10R1_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F10R1_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F10R1_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F10R1_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F10R1_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F10R1_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F10R1_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F10R1_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F10R1_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F10R1_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F10R1_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F10R1_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F10R1_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F10R1_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F10R1_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F10R1_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F10R1_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F10R1_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F10R1_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F10R1_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F10R1_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F10R1_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F10R1_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F10R1_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F10R1_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F10R1_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F10R1_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F10R1_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F11R1 register  ******************/
#define  CAN_F11R1_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F11R1_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F11R1_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F11R1_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F11R1_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F11R1_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F11R1_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F11R1_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F11R1_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F11R1_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F11R1_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F11R1_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F11R1_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F11R1_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F11R1_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F11R1_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F11R1_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F11R1_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F11R1_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F11R1_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F11R1_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F11R1_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F11R1_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F11R1_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F11R1_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F11R1_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F11R1_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F11R1_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F11R1_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F11R1_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F11R1_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F11R1_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F12R1 register  ******************/
#define  CAN_F12R1_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F12R1_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F12R1_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F12R1_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F12R1_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F12R1_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F12R1_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F12R1_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F12R1_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F12R1_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F12R1_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F12R1_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F12R1_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F12R1_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F12R1_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F12R1_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F12R1_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F12R1_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F12R1_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F12R1_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F12R1_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F12R1_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F12R1_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F12R1_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F12R1_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F12R1_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F12R1_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F12R1_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F12R1_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F12R1_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F12R1_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F12R1_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F13R1 register  ******************/
#define  CAN_F13R1_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F13R1_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F13R1_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F13R1_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F13R1_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F13R1_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F13R1_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F13R1_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F13R1_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F13R1_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F13R1_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F13R1_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F13R1_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F13R1_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F13R1_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F13R1_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F13R1_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F13R1_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F13R1_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F13R1_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F13R1_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F13R1_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F13R1_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F13R1_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F13R1_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F13R1_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F13R1_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F13R1_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F13R1_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F13R1_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F13R1_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F13R1_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F0R2 register  *******************/
#define  CAN_F0R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F0R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F0R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F0R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F0R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F0R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F0R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F0R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F0R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F0R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F0R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F0R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F0R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F0R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F0R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F0R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F0R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F0R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F0R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F0R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F0R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F0R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F0R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F0R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F0R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F0R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F0R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F0R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F0R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F0R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F0R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F0R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F1R2 register  *******************/
#define  CAN_F1R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F1R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F1R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F1R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F1R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F1R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F1R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F1R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F1R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F1R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F1R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F1R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F1R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F1R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F1R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F1R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F1R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F1R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F1R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F1R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F1R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F1R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F1R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F1R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F1R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F1R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F1R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F1R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F1R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F1R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F1R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F1R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F2R2 register  *******************/
#define  CAN_F2R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F2R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F2R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F2R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F2R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F2R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F2R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F2R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F2R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F2R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F2R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F2R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F2R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F2R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F2R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F2R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F2R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F2R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F2R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F2R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F2R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F2R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F2R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F2R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F2R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F2R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F2R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F2R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F2R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F2R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F2R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F2R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F3R2 register  *******************/
#define  CAN_F3R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F3R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F3R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F3R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F3R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F3R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F3R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F3R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F3R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F3R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F3R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F3R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F3R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F3R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F3R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F3R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F3R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F3R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F3R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F3R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F3R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F3R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F3R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F3R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F3R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F3R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F3R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F3R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F3R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F3R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F3R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F3R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F4R2 register  *******************/
#define  CAN_F4R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F4R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F4R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F4R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F4R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F4R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F4R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F4R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F4R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F4R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F4R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F4R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F4R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F4R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F4R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F4R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F4R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F4R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F4R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F4R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F4R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F4R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F4R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F4R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F4R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F4R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F4R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F4R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F4R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F4R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F4R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F4R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F5R2 register  *******************/
#define  CAN_F5R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F5R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F5R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F5R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F5R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F5R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F5R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F5R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F5R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F5R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F5R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F5R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F5R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F5R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F5R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F5R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F5R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F5R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F5R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F5R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F5R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F5R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F5R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F5R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F5R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F5R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F5R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F5R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F5R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F5R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F5R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F5R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F6R2 register  *******************/
#define  CAN_F6R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F6R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F6R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F6R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F6R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F6R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F6R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F6R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F6R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F6R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F6R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F6R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F6R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F6R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F6R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F6R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F6R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F6R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F6R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F6R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F6R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F6R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F6R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F6R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F6R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F6R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F6R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F6R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F6R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F6R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F6R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F6R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F7R2 register  *******************/
#define  CAN_F7R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F7R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F7R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F7R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F7R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F7R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F7R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F7R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F7R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F7R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F7R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F7R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F7R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F7R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F7R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F7R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F7R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F7R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F7R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F7R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F7R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F7R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F7R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F7R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F7R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F7R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F7R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F7R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F7R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F7R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F7R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F7R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F8R2 register  *******************/
#define  CAN_F8R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F8R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F8R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F8R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F8R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F8R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F8R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F8R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F8R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F8R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F8R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F8R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F8R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F8R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F8R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F8R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F8R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F8R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F8R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F8R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F8R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F8R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F8R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F8R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F8R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F8R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F8R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F8R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F8R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F8R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F8R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F8R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F9R2 register  *******************/
#define  CAN_F9R2_FB0                        ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F9R2_FB1                        ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F9R2_FB2                        ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F9R2_FB3                        ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F9R2_FB4                        ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F9R2_FB5                        ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F9R2_FB6                        ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F9R2_FB7                        ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F9R2_FB8                        ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F9R2_FB9                        ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F9R2_FB10                       ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F9R2_FB11                       ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F9R2_FB12                       ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F9R2_FB13                       ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F9R2_FB14                       ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F9R2_FB15                       ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F9R2_FB16                       ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F9R2_FB17                       ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F9R2_FB18                       ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F9R2_FB19                       ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F9R2_FB20                       ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F9R2_FB21                       ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F9R2_FB22                       ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F9R2_FB23                       ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F9R2_FB24                       ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F9R2_FB25                       ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F9R2_FB26                       ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F9R2_FB27                       ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F9R2_FB28                       ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F9R2_FB29                       ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F9R2_FB30                       ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F9R2_FB31                       ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F10R2 register  ******************/
#define  CAN_F10R2_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F10R2_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F10R2_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F10R2_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F10R2_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F10R2_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F10R2_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F10R2_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F10R2_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F10R2_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F10R2_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F10R2_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F10R2_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F10R2_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F10R2_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F10R2_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F10R2_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F10R2_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F10R2_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F10R2_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F10R2_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F10R2_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F10R2_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F10R2_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F10R2_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F10R2_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F10R2_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F10R2_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F10R2_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F10R2_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F10R2_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F10R2_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F11R2 register  ******************/
#define  CAN_F11R2_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F11R2_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F11R2_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F11R2_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F11R2_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F11R2_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F11R2_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F11R2_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F11R2_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F11R2_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F11R2_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F11R2_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F11R2_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F11R2_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F11R2_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F11R2_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F11R2_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F11R2_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F11R2_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F11R2_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F11R2_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F11R2_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F11R2_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F11R2_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F11R2_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F11R2_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F11R2_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F11R2_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F11R2_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F11R2_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F11R2_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F11R2_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F12R2 register  ******************/
#define  CAN_F12R2_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F12R2_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F12R2_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F12R2_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F12R2_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F12R2_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F12R2_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F12R2_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F12R2_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F12R2_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F12R2_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F12R2_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F12R2_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F12R2_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F12R2_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F12R2_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F12R2_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F12R2_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F12R2_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F12R2_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F12R2_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F12R2_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F12R2_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F12R2_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F12R2_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F12R2_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F12R2_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F12R2_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F12R2_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F12R2_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F12R2_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F12R2_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/*******************  Bit definition for CAN_F13R2 register  ******************/
#define  CAN_F13R2_FB0                       ((uint32_t)0x00000001)        /*!<Filter bit 0 */
#define  CAN_F13R2_FB1                       ((uint32_t)0x00000002)        /*!<Filter bit 1 */
#define  CAN_F13R2_FB2                       ((uint32_t)0x00000004)        /*!<Filter bit 2 */
#define  CAN_F13R2_FB3                       ((uint32_t)0x00000008)        /*!<Filter bit 3 */
#define  CAN_F13R2_FB4                       ((uint32_t)0x00000010)        /*!<Filter bit 4 */
#define  CAN_F13R2_FB5                       ((uint32_t)0x00000020)        /*!<Filter bit 5 */
#define  CAN_F13R2_FB6                       ((uint32_t)0x00000040)        /*!<Filter bit 6 */
#define  CAN_F13R2_FB7                       ((uint32_t)0x00000080)        /*!<Filter bit 7 */
#define  CAN_F13R2_FB8                       ((uint32_t)0x00000100)        /*!<Filter bit 8 */
#define  CAN_F13R2_FB9                       ((uint32_t)0x00000200)        /*!<Filter bit 9 */
#define  CAN_F13R2_FB10                      ((uint32_t)0x00000400)        /*!<Filter bit 10 */
#define  CAN_F13R2_FB11                      ((uint32_t)0x00000800)        /*!<Filter bit 11 */
#define  CAN_F13R2_FB12                      ((uint32_t)0x00001000)        /*!<Filter bit 12 */
#define  CAN_F13R2_FB13                      ((uint32_t)0x00002000)        /*!<Filter bit 13 */
#define  CAN_F13R2_FB14                      ((uint32_t)0x00004000)        /*!<Filter bit 14 */
#define  CAN_F13R2_FB15                      ((uint32_t)0x00008000)        /*!<Filter bit 15 */
#define  CAN_F13R2_FB16                      ((uint32_t)0x00010000)        /*!<Filter bit 16 */
#define  CAN_F13R2_FB17                      ((uint32_t)0x00020000)        /*!<Filter bit 17 */
#define  CAN_F13R2_FB18                      ((uint32_t)0x00040000)        /*!<Filter bit 18 */
#define  CAN_F13R2_FB19                      ((uint32_t)0x00080000)        /*!<Filter bit 19 */
#define  CAN_F13R2_FB20                      ((uint32_t)0x00100000)        /*!<Filter bit 20 */
#define  CAN_F13R2_FB21                      ((uint32_t)0x00200000)        /*!<Filter bit 21 */
#define  CAN_F13R2_FB22                      ((uint32_t)0x00400000)        /*!<Filter bit 22 */
#define  CAN_F13R2_FB23                      ((uint32_t)0x00800000)        /*!<Filter bit 23 */
#define  CAN_F13R2_FB24                      ((uint32_t)0x01000000)        /*!<Filter bit 24 */
#define  CAN_F13R2_FB25                      ((uint32_t)0x02000000)        /*!<Filter bit 25 */
#define  CAN_F13R2_FB26                      ((uint32_t)0x04000000)        /*!<Filter bit 26 */
#define  CAN_F13R2_FB27                      ((uint32_t)0x08000000)        /*!<Filter bit 27 */
#define  CAN_F13R2_FB28                      ((uint32_t)0x10000000)        /*!<Filter bit 28 */
#define  CAN_F13R2_FB29                      ((uint32_t)0x20000000)        /*!<Filter bit 29 */
#define  CAN_F13R2_FB30                      ((uint32_t)0x40000000)        /*!<Filter bit 30 */
#define  CAN_F13R2_FB31                      ((uint32_t)0x80000000)        /*!<Filter bit 31 */

/******************************************************************************/
/*                                                                            */
/*                          CRC calculation unit                              */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint32_t)0xFFFFFFFF) /*!< Data register bits */

/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint8_t)0xFF)        /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint32_t)0x00000001) /*!< RESET the CRC computation unit bit */
#define  CRC_CR_POLSIZE                      ((uint32_t)0x00000018) /*!< Polynomial size bits */
#define  CRC_CR_POLSIZE_0                    ((uint32_t)0x00000008) /*!< Polynomial size bit 0 */
#define  CRC_CR_POLSIZE_1                    ((uint32_t)0x00000010) /*!< Polynomial size bit 1 */
#define  CRC_CR_REV_IN                       ((uint32_t)0x00000060) /*!< REV_IN Reverse Input Data bits */
#define  CRC_CR_REV_IN_0                     ((uint32_t)0x00000020) /*!< Bit 0 */
#define  CRC_CR_REV_IN_1                     ((uint32_t)0x00000040) /*!< Bit 1 */
#define  CRC_CR_REV_OUT                      ((uint32_t)0x00000080) /*!< REV_OUT Reverse Output Data bits */

/*******************  Bit definition for CRC_INIT register  *******************/
#define  CRC_INIT_INIT                       ((uint32_t)0xFFFFFFFF) /*!< Initial CRC value bits */

/*******************  Bit definition for CRC_POL register  ********************/
#define  CRC_POL_POL                         ((uint32_t)0xFFFFFFFF) /*!< Coefficients of the polynomial */

/******************************************************************************/
/*                                                                            */
/*                       Advanced Encryption Standard (AES)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for AES_CR register  *********************/
#define  AES_CR_EN                           ((uint32_t)0x00000001)        /*!< AES Enable */
#define  AES_CR_DATATYPE                     ((uint32_t)0x00000006)        /*!< Data type selection */
#define  AES_CR_DATATYPE_0                   ((uint32_t)0x00000002)        /*!< Bit 0 */
#define  AES_CR_DATATYPE_1                   ((uint32_t)0x00000004)        /*!< Bit 1 */

#define  AES_CR_MODE                         ((uint32_t)0x00000018)        /*!< AES Mode Of Operation */
#define  AES_CR_MODE_0                       ((uint32_t)0x00000008)        /*!< Bit 0 */
#define  AES_CR_MODE_1                       ((uint32_t)0x00000010)        /*!< Bit 1 */

#define  AES_CR_CHMOD                        ((uint32_t)0x00000060)        /*!< AES Chaining Mode */
#define  AES_CR_CHMOD_0                      ((uint32_t)0x00000020)        /*!< Bit 0 */
#define  AES_CR_CHMOD_1                      ((uint32_t)0x00000040)        /*!< Bit 1 */
#define  AES_CR_CHMOD_2                      ((uint32_t)0x00010000)        /*!< Bit 2 */

#define  AES_CR_CCFC                         ((uint32_t)0x00000080)        /*!< Computation Complete Flag Clear */
#define  AES_CR_ERRC                         ((uint32_t)0x00000100)        /*!< Error Clear */
#define  AES_CR_CCIE                         ((uint32_t)0x00000200)        /*!< Computation Complete Interrupt Enable */
#define  AES_CR_ERRIE                        ((uint32_t)0x00000400)        /*!< Error Interrupt Enable */
#define  AES_CR_DMAINEN                      ((uint32_t)0x00000800)        /*!< DMA ENable managing the data input phase */
#define  AES_CR_DMAOUTEN                     ((uint32_t)0x00001000)        /*!< DMA Enable managing the data output phase */

#define  AES_CR_GCMPH                         ((uint32_t)0x00006000)        /*!< GCM Phase */
#define  AES_CR_GCMPH_0                       ((uint32_t)0x00002000)        /*!< Bit 0 */
#define  AES_CR_GCMPH_1                       ((uint32_t)0x00004000)        /*!< Bit 1 */

#define  AES_CR_KEYSIZE                       ((uint32_t)0x00040000)        /*!< Key size selection */

/*******************  Bit definition for AES_SR register  *********************/
#define  AES_SR_CCF                          ((uint32_t)0x00000001)        /*!< Computation Complete Flag */
#define  AES_SR_RDERR                        ((uint32_t)0x00000002)        /*!< Read Error Flag */
#define  AES_SR_WRERR                        ((uint32_t)0x00000004)        /*!< Write Error Flag */
#define  AES_SR_BUSY                         ((uint32_t)0x00000008)        /*!< Busy Flag */

/*******************  Bit definition for AES_DINR register  *******************/
#define  AES_DINR                            ((uint32_t)0xFFFFFFFF)        /*!< AES Data Input Register */

/*******************  Bit definition for AES_DOUTR register  ******************/
#define  AES_DOUTR                           ((uint32_t)0xFFFFFFFF)        /*!< AES Data Output Register */

/*******************  Bit definition for AES_KEYR0 register  ******************/
#define  AES_KEYR0                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 0 */

/*******************  Bit definition for AES_KEYR1 register  ******************/
#define  AES_KEYR1                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 1 */

/*******************  Bit definition for AES_KEYR2 register  ******************/
#define  AES_KEYR2                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 2 */

/*******************  Bit definition for AES_KEYR3 register  ******************/
#define  AES_KEYR3                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 3 */

/*******************  Bit definition for AES_KEYR4 register  ******************/
#define  AES_KEYR4                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 4 */

/*******************  Bit definition for AES_KEYR5 register  ******************/
#define  AES_KEYR5                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 5 */

/*******************  Bit definition for AES_KEYR6 register  ******************/
#define  AES_KEYR6                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 6 */

/*******************  Bit definition for AES_KEYR7 register  ******************/
#define  AES_KEYR7                           ((uint32_t)0xFFFFFFFF)        /*!< AES Key Register 7 */

/*******************  Bit definition for AES_IVR0 register   ******************/
#define  AES_IVR0                            ((uint32_t)0xFFFFFFFF)        /*!< AES Initialization Vector Register 0 */

/*******************  Bit definition for AES_IVR1 register   ******************/
#define  AES_IVR1                            ((uint32_t)0xFFFFFFFF)        /*!< AES Initialization Vector Register 1 */

/*******************  Bit definition for AES_IVR2 register   ******************/
#define  AES_IVR2                            ((uint32_t)0xFFFFFFFF)        /*!< AES Initialization Vector Register 2 */

/*******************  Bit definition for AES_IVR3 register   ******************/
#define  AES_IVR3                            ((uint32_t)0xFFFFFFFF)        /*!< AES Initialization Vector Register 3 */

/*******************  Bit definition for AES_SUSP0R register  ******************/
#define  AES_SUSP0R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 0 */

/*******************  Bit definition for AES_SUSP1R register  ******************/
#define  AES_SUSP1R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 1 */

/*******************  Bit definition for AES_SUSP2R register  ******************/
#define  AES_SUSP2R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 2 */

/*******************  Bit definition for AES_SUSP3R register  ******************/
#define  AES_SUSP3R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 3 */

/*******************  Bit definition for AES_SUSP4R register  ******************/
#define  AES_SUSP4R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 4 */

/*******************  Bit definition for AES_SUSP5R register  ******************/
#define  AES_SUSP5R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 5 */

/*******************  Bit definition for AES_SUSP6R register  ******************/
#define  AES_SUSP6R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 6 */

/*******************  Bit definition for AES_SUSP7R register  ******************/
#define  AES_SUSP7R                          ((uint32_t)0xFFFFFFFF)        /*!< AES Suspend registers 7 */

/******************************************************************************/
/*                                                                            */
/*                      Digital to Analog Converter                           */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for DAC_CR register  ********************/
#define  DAC_CR_EN1                          ((uint32_t)0x00000001)        /*!<DAC channel1 enable */
#define  DAC_CR_TEN1                         ((uint32_t)0x00000004)        /*!<DAC channel1 Trigger enable */

#define  DAC_CR_TSEL1                        ((uint32_t)0x00000038)        /*!<TSEL1[2:0] (DAC channel1 Trigger selection) */
#define  DAC_CR_TSEL1_0                      ((uint32_t)0x00000008)        /*!<Bit 0 */
#define  DAC_CR_TSEL1_1                      ((uint32_t)0x00000010)        /*!<Bit 1 */
#define  DAC_CR_TSEL1_2                      ((uint32_t)0x00000020)        /*!<Bit 2 */

#define  DAC_CR_WAVE1                        ((uint32_t)0x000000C0)        /*!<WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE1_0                      ((uint32_t)0x00000040)        /*!<Bit 0 */
#define  DAC_CR_WAVE1_1                      ((uint32_t)0x00000080)        /*!<Bit 1 */

#define  DAC_CR_MAMP1                        ((uint32_t)0x00000F00)        /*!<MAMP1[3:0] (DAC channel1 Mask/Amplitude selector) */
#define  DAC_CR_MAMP1_0                      ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  DAC_CR_MAMP1_1                      ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  DAC_CR_MAMP1_2                      ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  DAC_CR_MAMP1_3                      ((uint32_t)0x00000800)        /*!<Bit 3 */

#define  DAC_CR_DMAEN1                       ((uint32_t)0x00001000)        /*!<DAC channel1 DMA enable */
#define  DAC_CR_DMAUDRIE1                    ((uint32_t)0x00002000)        /*!<DAC channel 1 DMA underrun interrupt enable  >*/
#define  DAC_CR_CEN1                         ((uint32_t)0x00004000)        /*!<DAC channel 1 calibration enable >*/

#define  DAC_CR_EN2                          ((uint32_t)0x00010000)        /*!<DAC channel2 enable */
#define  DAC_CR_TEN2                         ((uint32_t)0x00040000)        /*!<DAC channel2 Trigger enable */

#define  DAC_CR_TSEL2                        ((uint32_t)0x00380000)        /*!<TSEL2[2:0] (DAC channel2 Trigger selection) */
#define  DAC_CR_TSEL2_0                      ((uint32_t)0x00080000)        /*!<Bit 0 */
#define  DAC_CR_TSEL2_1                      ((uint32_t)0x00100000)        /*!<Bit 1 */
#define  DAC_CR_TSEL2_2                      ((uint32_t)0x00200000)        /*!<Bit 2 */

#define  DAC_CR_WAVE2                        ((uint32_t)0x00C00000)        /*!<WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE2_0                      ((uint32_t)0x00400000)        /*!<Bit 0 */
#define  DAC_CR_WAVE2_1                      ((uint32_t)0x00800000)        /*!<Bit 1 */

#define  DAC_CR_MAMP2                        ((uint32_t)0x0F000000)        /*!<MAMP2[3:0] (DAC channel2 Mask/Amplitude selector) */
#define  DAC_CR_MAMP2_0                      ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  DAC_CR_MAMP2_1                      ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  DAC_CR_MAMP2_2                      ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  DAC_CR_MAMP2_3                      ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  DAC_CR_DMAEN2                       ((uint32_t)0x10000000)        /*!<DAC channel2 DMA enabled */
#define  DAC_CR_DMAUDRIE2                    ((uint32_t)0x20000000)        /*!<DAC channel2 DMA underrun interrupt enable  >*/
#define  DAC_CR_CEN2                         ((uint32_t)0x40000000)        /*!<DAC channel2 calibration enable >*/

/*****************  Bit definition for DAC_SWTRIGR register  ******************/
#define  DAC_SWTRIGR_SWTRIG1                 ((uint8_t)0x01)               /*!<DAC channel1 software trigger */
#define  DAC_SWTRIGR_SWTRIG2                 ((uint8_t)0x02)               /*!<DAC channel2 software trigger */

/*****************  Bit definition for DAC_DHR12R1 register  ******************/
#define  DAC_DHR12R1_DACC1DHR                ((uint16_t)0x0FFF)            /*!<DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L1 register  ******************/
#define  DAC_DHR12L1_DACC1DHR                ((uint16_t)0xFFF0)            /*!<DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R1 register  ******************/
#define  DAC_DHR8R1_DACC1DHR                 ((uint8_t)0xFF)               /*!<DAC channel1 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12R2 register  ******************/
#define  DAC_DHR12R2_DACC2DHR                ((uint16_t)0x0FFF)            /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L2 register  ******************/
#define  DAC_DHR12L2_DACC2DHR                ((uint16_t)0xFFF0)            /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R2 register  ******************/
#define  DAC_DHR8R2_DACC2DHR                 ((uint8_t)0xFF)               /*!<DAC channel2 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12RD register  ******************/
#define  DAC_DHR12RD_DACC1DHR                ((uint32_t)0x00000FFF)        /*!<DAC channel1 12-bit Right aligned data */
#define  DAC_DHR12RD_DACC2DHR                ((uint32_t)0x0FFF0000)        /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12LD register  ******************/
#define  DAC_DHR12LD_DACC1DHR                ((uint32_t)0x0000FFF0)        /*!<DAC channel1 12-bit Left aligned data */
#define  DAC_DHR12LD_DACC2DHR                ((uint32_t)0xFFF00000)        /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8RD register  ******************/
#define  DAC_DHR8RD_DACC1DHR                 ((uint16_t)0x00FF)            /*!<DAC channel1 8-bit Right aligned data */
#define  DAC_DHR8RD_DACC2DHR                 ((uint16_t)0xFF00)            /*!<DAC channel2 8-bit Right aligned data */

/*******************  Bit definition for DAC_DOR1 register  *******************/
#define  DAC_DOR1_DACC1DOR                   ((uint16_t)0x0FFF)            /*!<DAC channel1 data output */

/*******************  Bit definition for DAC_DOR2 register  *******************/
#define  DAC_DOR2_DACC2DOR                   ((uint16_t)0x0FFF)            /*!<DAC channel2 data output */

/********************  Bit definition for DAC_SR register  ********************/
#define  DAC_SR_DMAUDR1                      ((uint32_t)0x00002000)        /*!<DAC channel1 DMA underrun flag */
#define  DAC_SR_CAL_FLAG1                    ((uint32_t)0x00004000)        /*!<DAC channel1 calibration offset status */
#define  DAC_SR_BWST1                        ((uint32_t)0x20008000)        /*!<DAC channel1 busy writing sample time flag */

#define  DAC_SR_DMAUDR2                      ((uint32_t)0x20000000)        /*!<DAC channel2 DMA underrun flag */
#define  DAC_SR_CAL_FLAG2                    ((uint32_t)0x40000000)        /*!<DAC channel2 calibration offset status */
#define  DAC_SR_BWST2                        ((uint32_t)0x80000000)        /*!<DAC channel2 busy writing sample time flag */

/*******************  Bit definition for DAC_CCR register  ********************/
#define  DAC_CCR_OTRIM1                      ((uint32_t)0x0000001F)        /*!<DAC channel1 offset trimming value */
#define  DAC_CCR_OTRIM2                      ((uint32_t)0x001F0000)        /*!<DAC channel2 offset trimming value */

/*******************  Bit definition for DAC_MCR register  *******************/
#define  DAC_MCR_MODE1                        ((uint32_t)0x00000007)        /*!<MODE1[2:0] (DAC channel1 mode) */
#define  DAC_MCR_MODE1_0                      ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  DAC_MCR_MODE1_1                      ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  DAC_MCR_MODE1_2                      ((uint32_t)0x00000004)        /*!<Bit 2 */

#define  DAC_MCR_MODE2                        ((uint32_t)0x00070000)        /*!<MODE2[2:0] (DAC channel2 mode) */
#define  DAC_MCR_MODE2_0                      ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  DAC_MCR_MODE2_1                      ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  DAC_MCR_MODE2_2                      ((uint32_t)0x00040000)        /*!<Bit 2 */

/******************  Bit definition for DAC_SHSR1 register  ******************/
#define  DAC_SHSR1_TSAMPLE1                   ((uint32_t)0x000003FF)        /*!<DAC channel1 sample time */

/******************  Bit definition for DAC_SHSR2 register  ******************/
#define  DAC_SHSR1_TSAMPLE2                   ((uint32_t)0x000003FF)        /*!<DAC channel2 sample time */

/******************  Bit definition for DAC_SHHR register  ******************/
#define  DAC_SHHR_THOLD1                      ((uint32_t)0x000003FF)        /*!<DAC channel1 hold time */
#define  DAC_SHHR_THOLD2                      ((uint32_t)0x03FF0000)        /*!<DAC channel2 hold time */

/******************  Bit definition for DAC_SHRR register  ******************/
#define  DAC_SHRR_TREFRESH1                   ((uint32_t)0x000000FF)        /*!<DAC channel1 refresh time */
#define  DAC_SHRR_TREFRESH2                   ((uint32_t)0x00FF0000)        /*!<DAC channel2 refresh time */

/******************************************************************************/
/*                                                                            */
/*                                 Debug MCU                                  */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                 Digital Filter for Sigma Delta Modulators                  */
/*                                                                            */
/******************************************************************************/

/****************   DFSDM channel configuration registers  ********************/

/***************  Bit definition for DFSDM_CHCFGR1 register  ******************/
#define  DFSDM_CHCFGR1_DFSDMEN                ((uint32_t)0x80000000)            /*!< Global enable for DFSDM interface */
#define  DFSDM_CHCFGR1_CKOUTSRC               ((uint32_t)0x40000000)            /*!< Output serial clock source selection */

#define  DFSDM_CHCFGR1_CKOUTDIV               ((uint32_t)0x00FF0000)            /*!< CKOUTDIV[7:0] output serial clock divider */

#define  DFSDM_CHCFGR1_DATPACK                ((uint32_t)0x0000C000)            /*!< Data packing mode */
#define  DFSDM_CHCFGR1_DATMPX                 ((uint32_t)0x00003000)            /*!< Input data multiplexer for channel y */

#define  DFSDM_CHCFGR1_CHINSEL                ((uint32_t)0x00000100)            /*!< Serial inputs selection for channel y */   
#define  DFSDM_CHCFGR1_CHEN                   ((uint32_t)0x00000080)            /*!< Channel y enable */
#define  DFSDM_CHCFGR1_CKABEN                 ((uint32_t)0x00000040)            /*!< Clock absence detector enable on channel y */
#define  DFSDM_CHCFGR1_SCDEN                  ((uint32_t)0x00000020)            /*!< Short circuit detector enable on channel y */

#define  DFSDM_CHCFGR1_SPICKSEL               ((uint32_t)0x0000000C)            /*!< SPICKSEL[1:0] SPI clock select for channel y */
#define  DFSDM_CHCFGR1_SPICKSEL_1             ((uint32_t)0x00000008)            /*!< SPI clock select for channel y, Bit 1 */
#define  DFSDM_CHCFGR1_SPICKSEL_0             ((uint32_t)0x00000004)            /*!< SPI clock select for channel y, Bit 0 */

#define  DFSDM_CHCFGR1_SITP                   ((uint32_t)0x00000003)            /*!< SITP[1:0] Serial interface type for channel y */
#define  DFSDM_CHCFGR1_SITP_1                 ((uint32_t)0x00000002)            /*!< Serial interface type for channel y, Bit 1 */
#define  DFSDM_CHCFGR1_SITP_0                 ((uint32_t)0x00000001)            /*!< Serial interface type for channel y, Bit 0 */

/***************  Bit definition for DFSDM_CHCFGR2 register  ******************/
#define  DFSDM_CHCFGR2_OFFSET                 ((uint32_t)0xFFFFFF00)            /*!< OFFSET[23:0] 24-bit calibration offset for channel y */
#define  DFSDM_CHCFGR2_DTRBS                  ((uint32_t)0x000000F8)            /*!< DTRBS[4:0] Data right bit-shift for channel y */

/******************  Bit definition for DFSDM_AWSCDR register *****************/
#define  DFSDM_AWSCDR_AWFORD                  ((uint32_t)0x00C00000)            /*!< AWFORD[1:0] Analog watchdog Sinc filter order on channel y */
#define  DFSDM_AWSCDR_AWFOSR                  ((uint32_t)0x001F0000)            /*!< AWFOSR[4:0] Analog watchdog filter oversampling ratio on cahannel y */
#define  DFSDM_AWSCDR_BKSCD                   ((uint32_t)0x0000F000)            /*!< BKSCD[3:0] Break signal assignment for short circuit detector on channel y */
#define  DFSDM_AWSCDR_SCDT                    ((uint32_t)0x000000FF)            /*!< SCDT[7:0] Short circuit detector threshold for channel y */

/****************  Bit definition for DFSDM_CHWDATR register *******************/
#define  DFSDM_AWSCDR_WDATA                   ((uint32_t)0x0000FFFF)            /*!< WDATA[15:0] Input channel y watchdog data */

/****************  Bit definition for DFSDM_CHDATINR register *****************/
#define  DFSDM_AWSCDR_INDAT0                   ((uint32_t)0x0000FFFF)            /*!< INDAT0[31:16] Input data for channel y or channel (y+1) */
#define  DFSDM_AWSCDR_INDAT1                   ((uint32_t)0xFFFF0000)            /*!< INDAT0[15:0] Input data for channel y */
   
/************************   DFSDM module registers  ****************************/

/********************  Bit definition for DFSDM_CR1 register *******************/
#define  DFSDM_CR1_AWFSEL                     ((uint32_t)0x40000000)            /*!< Analog watchdog fast mode select */
#define  DFSDM_CR1_FAST                       ((uint32_t)0x20000000)            /*!< Fast conversion mode selection */
#define  DFSDM_CR1_RCH                        ((uint32_t)0x07000000)            /*!< RCH[2:0] Regular channel selection */
#define  DFSDM_CR1_RDMAEN                     ((uint32_t)0x00200000)            /*!< DMA channel enabled to read data for the regular conversion */
#define  DFSDM_CR1_RSYNC                      ((uint32_t)0x00080000)            /*!< Launch regular conversion synchronously with DFSDMx */
#define  DFSDM_CR1_RCONT                      ((uint32_t)0x00040000)            /*!< Continuous mode selection for regular conversions */
#define  DFSDM_CR1_RSWSTART                   ((uint32_t)0x00020000)            /*!< Software start of a conversion on the regular channel */
#define  DFSDM_CR1_JEXTEN                     ((uint32_t)0x00006000)            /*!< JEXTEN[1:0] Trigger enable and trigger edge selection for injected conversions */
#define  DFSDM_CR1_JEXTSEL                    ((uint32_t)0x00000700)            /*!< JEXTSEL[2:0]Trigger signal selection for launching injected conversions */
#define  DFSDM_CR1_JDMAEN                     ((uint32_t)0x00000020)            /*!< DMA channel enabled to read data for the injected channel group */
#define  DFSDM_CR1_JSCAN                      ((uint32_t)0x00000010)            /*!< Scanning conversion in continuous mode selection for injected conversions */
#define  DFSDM_CR1_JSYNC                      ((uint32_t)0x00000008)            /*!< Launch an injected conversion synchronously with DFSDMx JSWSTART trigger  */
#define  DFSDM_CR1_JSWSTART                   ((uint32_t)0x00000002)            /*!< Start the conversion of the injected group of channels */
#define  DFSDM_CR1_DFEN                       ((uint32_t)0x00000001)            /*!< DFSDM enable */

/********************  Bit definition for DFSDM_CR2 register *******************/
#define  DFSDM_CR2_AWDCH                      ((uint32_t)0x00FF0000)            /*!< AWDCH[7:0] Analog watchdog channel selection */
#define  DFSDM_CR2_EXCH                       ((uint32_t)0x0000FF00)            /*!< EXCH[7:0] Extreme detector channel selection */
#define  DFSDM_CR2_CKABIE                     ((uint32_t)0x00000040)            /*!< Clock absence interrupt enable */
#define  DFSDM_CR2_SCDIE                      ((uint32_t)0x00000020)            /*!< Short circuit detector interrupt enable */
#define  DFSDM_CR2_AWDIE                      ((uint32_t)0x00000010)            /*!< Analog watchdog interrupt enable */
#define  DFSDM_CR2_ROVRIE                     ((uint32_t)0x00000008)            /*!< Regular data overrun interrupt enable */
#define  DFSDM_CR2_JOVRIE                     ((uint32_t)0x00000004)            /*!< Injected data overrun interrupt enable */
#define  DFSDM_CR2_REOCIE                     ((uint32_t)0x00000002)            /*!< Regular end of conversion interrupt enable */
#define  DFSDM_CR2_JEOCIE                     ((uint32_t)0x00000001)            /*!< Injected end of conversion interrupt enable */

/********************  Bit definition for DFSDM_ISR register *******************/
#define  DFSDM_ISR_SCDF                       ((uint32_t)0xFF000000)            /*!< SCDF[7:0] Short circuit detector flag */
#define  DFSDM_ISR_CKABF                      ((uint32_t)0x00FF0000)            /*!< CKABF[7:0] Clock absence flag */
#define  DFSDM_ISR_RCIP                       ((uint32_t)0x00004000)            /*!< Regular conversion in progress status */
#define  DFSDM_ISR_JCIP                       ((uint32_t)0x00002000)            /*!< Injected conversion in progress status */
#define  DFSDM_ISR_AWDF                       ((uint32_t)0x00000010)            /*!< Analog watchdog */
#define  DFSDM_ISR_ROVRF                      ((uint32_t)0x00000008)            /*!< Regular conversion overrun flag */
#define  DFSDM_ISR_JOVRF                      ((uint32_t)0x00000004)            /*!< Injected conversion overrun flag */
#define  DFSDM_ISR_REOCF                      ((uint32_t)0x00000002)            /*!< End of regular conversion flag */
#define  DFSDM_ISR_JEOCF                      ((uint32_t)0x00000001)            /*!< End of injected conversion flag */

/********************  Bit definition for DFSDM_ICR register *******************/
#define  DFSDM_ICR_CLRSCSDF                   ((uint32_t)0xFF000000)            /*!< CLRSCSDF[7:0] Clear the short circuit detector flag */
#define  DFSDM_ICR_CLRCKABF                   ((uint32_t)0x00FF0000)            /*!< CLRCKABF[7:0] Clear the clock absence flag */
#define  DFSDM_ICR_CLRROVRF                   ((uint32_t)0x00000008)            /*!< Clear the regular conversion overrun flag */
#define  DFSDM_ICR_CLRJOVRF                   ((uint32_t)0x00000004)            /*!< Clear the injected conversion overrun flag */

/*******************  Bit definition for DFSDM_JCHGR register ******************/
#define  DFSDM_JCHGR_JCHG                     ((uint32_t)0x000000FF)            /*!< JCHG[7:0] Injected channel group selection */

/********************  Bit definition for DFSDM_FCR register *******************/
#define  DFSDM_FCR_FORD                       ((uint32_t)0xE0000000)            /*!< FORD[2:0] Sinc filter order */
#define  DFSDM_FCR_FOSR                       ((uint32_t)0x03FF0000)            /*!< FOSR[9:0] Sinc filter oversampling ratio (decimation rate) */
#define  DFSDM_FCR_IOSR                       ((uint32_t)0x000000FF)            /*!< IOSR[7:0] Integrator oversampling ratio (averaging length) */

/******************  Bit definition for DFSDM_JDATAR register *****************/
#define  DFSDM_JDATAR_JDATA                   ((uint32_t)0xFFFFFF00)            /*!< JDATA[23:0] Injected group conversion data */
#define  DFSDM_JDATAR_JDATACH                 ((uint32_t)0x00000007)            /*!< JDATACH[2:0] Injected channel most recently converted */

/******************  Bit definition for DFSDM_RDATAR register *****************/
#define  DFSDM_JDATAR_RDATA                   ((uint32_t)0xFFFFFF00)            /*!< JDATA[23:0] Regular channel conversion data */
#define  DFSDM_JDATAR_RPEND                   ((uint32_t)0x00000010)            /*!< RPEND Regular channel pending data */

/******************  Bit definition for DFSDM_AWHTR register ******************/
#define  DFSDM_AWHTR_AWHT                    ((uint32_t)0xFFFFFF00)             /*!< AWHT[23:0] Analog watchdog high threshold */
#define  DFSDM_AWHTR_BKAWH                   ((uint32_t)0x0000000F)             /*!< BKAWH[3:0] Break signal assignment to analog watchdog high threshold event */

/******************  Bit definition for DFSDM_AWLTR register ******************/
#define  DFSDM_AWLTR_AWLT                    ((uint32_t)0xFFFFFF00)             /*!< AWHT[23:0] Analog watchdog low threshold */
#define  DFSDM_AWLTR_BKAWL                   ((uint32_t)0x0000000F)             /*!< BKAWL[3:0] Break signal assignment to analog watchdog low threshold event */

/******************  Bit definition for DFSDM_AWSR register ******************/
#define  DFSDM_AWSR_AWHTF                    ((uint32_t)0x0000FF00)             /*!< AWHTF[15:8] Analog watchdog high threshold error on given channels */
#define  DFSDM_AWSR_AWLTF                    ((uint32_t)0x000000FF)             /*!< AWLTF[7:0] Analog watchdog low threshold error on given channels */

/******************  Bit definition for DFSDM_AWCFR) register *****************/
#define  DFSDM_EXMAX_CLRAWHTF                ((uint32_t)0x0000FF00)             /*!< CLRAWHTF[15:8] Clear the Analog watchdog high threshold flag */
#define  DFSDM_EXMAX_CLRAWLTF                ((uint32_t)0x000000FF)             /*!< CLRAWLTF[7:0] Clear the Analog watchdog low threshold flag */

/******************  Bit definition for DFSDM_EXMAX register ******************/
#define  DFSDM_EXMAX_EXMAX                   ((uint32_t)0xFFFFFF00)             /*!< EXMAX[23:0] Extreme detector maximum value */
#define  DFSDM_EXMAX_EXMAXCH                 ((uint32_t)0x00000007)             /*!< EXMAXCH[2:0] Extreme detector maximum data channel */

/******************  Bit definition for DFSDM_EXMIN register ******************/
#define  DFSDM_EXMIN_EXMIN                   ((uint32_t)0xFFFFFF00)             /*!< EXMIN[23:0] Extreme detector minimum value */
#define  DFSDM_EXMIN_EXMINCH                 ((uint32_t)0x00000007)             /*!< EXMINCH[2:0] Extreme detector minimum data channel */

/******************  Bit definition for DFSDM_EXMIN register ******************/
#define  DFSDM_CNVTIMR_CNVCNT                ((uint32_t)0xFFFFFFF0)             /*!< CNVCNT[27:0]: 28-bit timer counting conversion time */
/******************************************************************************/
/*                                                                            */
/*                           DMA Controller (DMA)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_ISR register  ********************/
#define  DMA_ISR_GIF1                        ((uint32_t)0x00000001)        /*!< Channel 1 Global interrupt flag */
#define  DMA_ISR_TCIF1                       ((uint32_t)0x00000002)        /*!< Channel 1 Transfer Complete flag */
#define  DMA_ISR_HTIF1                       ((uint32_t)0x00000004)        /*!< Channel 1 Half Transfer flag */
#define  DMA_ISR_TEIF1                       ((uint32_t)0x00000008)        /*!< Channel 1 Transfer Error flag */
#define  DMA_ISR_GIF2                        ((uint32_t)0x00000010)        /*!< Channel 2 Global interrupt flag */
#define  DMA_ISR_TCIF2                       ((uint32_t)0x00000020)        /*!< Channel 2 Transfer Complete flag */
#define  DMA_ISR_HTIF2                       ((uint32_t)0x00000040)        /*!< Channel 2 Half Transfer flag */
#define  DMA_ISR_TEIF2                       ((uint32_t)0x00000080)        /*!< Channel 2 Transfer Error flag */
#define  DMA_ISR_GIF3                        ((uint32_t)0x00000100)        /*!< Channel 3 Global interrupt flag */
#define  DMA_ISR_TCIF3                       ((uint32_t)0x00000200)        /*!< Channel 3 Transfer Complete flag */
#define  DMA_ISR_HTIF3                       ((uint32_t)0x00000400)        /*!< Channel 3 Half Transfer flag */
#define  DMA_ISR_TEIF3                       ((uint32_t)0x00000800)        /*!< Channel 3 Transfer Error flag */
#define  DMA_ISR_GIF4                        ((uint32_t)0x00001000)        /*!< Channel 4 Global interrupt flag */
#define  DMA_ISR_TCIF4                       ((uint32_t)0x00002000)        /*!< Channel 4 Transfer Complete flag */
#define  DMA_ISR_HTIF4                       ((uint32_t)0x00004000)        /*!< Channel 4 Half Transfer flag */
#define  DMA_ISR_TEIF4                       ((uint32_t)0x00008000)        /*!< Channel 4 Transfer Error flag */
#define  DMA_ISR_GIF5                        ((uint32_t)0x00010000)        /*!< Channel 5 Global interrupt flag */
#define  DMA_ISR_TCIF5                       ((uint32_t)0x00020000)        /*!< Channel 5 Transfer Complete flag */
#define  DMA_ISR_HTIF5                       ((uint32_t)0x00040000)        /*!< Channel 5 Half Transfer flag */
#define  DMA_ISR_TEIF5                       ((uint32_t)0x00080000)        /*!< Channel 5 Transfer Error flag */
#define  DMA_ISR_GIF6                        ((uint32_t)0x00100000)        /*!< Channel 6 Global interrupt flag */
#define  DMA_ISR_TCIF6                       ((uint32_t)0x00200000)        /*!< Channel 6 Transfer Complete flag */
#define  DMA_ISR_HTIF6                       ((uint32_t)0x00400000)        /*!< Channel 6 Half Transfer flag */
#define  DMA_ISR_TEIF6                       ((uint32_t)0x00800000)        /*!< Channel 6 Transfer Error flag */
#define  DMA_ISR_GIF7                        ((uint32_t)0x01000000)        /*!< Channel 7 Global interrupt flag */
#define  DMA_ISR_TCIF7                       ((uint32_t)0x02000000)        /*!< Channel 7 Transfer Complete flag */
#define  DMA_ISR_HTIF7                       ((uint32_t)0x04000000)        /*!< Channel 7 Half Transfer flag */
#define  DMA_ISR_TEIF7                       ((uint32_t)0x08000000)        /*!< Channel 7 Transfer Error flag */

/*******************  Bit definition for DMA_IFCR register  *******************/
#define  DMA_IFCR_CGIF1                      ((uint32_t)0x00000001)        /*!< Channel 1 Global interrupt clearr */
#define  DMA_IFCR_CTCIF1                     ((uint32_t)0x00000002)        /*!< Channel 1 Transfer Complete clear */
#define  DMA_IFCR_CHTIF1                     ((uint32_t)0x00000004)        /*!< Channel 1 Half Transfer clear */
#define  DMA_IFCR_CTEIF1                     ((uint32_t)0x00000008)        /*!< Channel 1 Transfer Error clear */
#define  DMA_IFCR_CGIF2                      ((uint32_t)0x00000010)        /*!< Channel 2 Global interrupt clear */
#define  DMA_IFCR_CTCIF2                     ((uint32_t)0x00000020)        /*!< Channel 2 Transfer Complete clear */
#define  DMA_IFCR_CHTIF2                     ((uint32_t)0x00000040)        /*!< Channel 2 Half Transfer clear */
#define  DMA_IFCR_CTEIF2                     ((uint32_t)0x00000080)        /*!< Channel 2 Transfer Error clear */
#define  DMA_IFCR_CGIF3                      ((uint32_t)0x00000100)        /*!< Channel 3 Global interrupt clear */
#define  DMA_IFCR_CTCIF3                     ((uint32_t)0x00000200)        /*!< Channel 3 Transfer Complete clear */
#define  DMA_IFCR_CHTIF3                     ((uint32_t)0x00000400)        /*!< Channel 3 Half Transfer clear */
#define  DMA_IFCR_CTEIF3                     ((uint32_t)0x00000800)        /*!< Channel 3 Transfer Error clear */
#define  DMA_IFCR_CGIF4                      ((uint32_t)0x00001000)        /*!< Channel 4 Global interrupt clear */
#define  DMA_IFCR_CTCIF4                     ((uint32_t)0x00002000)        /*!< Channel 4 Transfer Complete clear */
#define  DMA_IFCR_CHTIF4                     ((uint32_t)0x00004000)        /*!< Channel 4 Half Transfer clear */
#define  DMA_IFCR_CTEIF4                     ((uint32_t)0x00008000)        /*!< Channel 4 Transfer Error clear */
#define  DMA_IFCR_CGIF5                      ((uint32_t)0x00010000)        /*!< Channel 5 Global interrupt clear */
#define  DMA_IFCR_CTCIF5                     ((uint32_t)0x00020000)        /*!< Channel 5 Transfer Complete clear */
#define  DMA_IFCR_CHTIF5                     ((uint32_t)0x00040000)        /*!< Channel 5 Half Transfer clear */
#define  DMA_IFCR_CTEIF5                     ((uint32_t)0x00080000)        /*!< Channel 5 Transfer Error clear */
#define  DMA_IFCR_CGIF6                      ((uint32_t)0x00100000)        /*!< Channel 6 Global interrupt clear */
#define  DMA_IFCR_CTCIF6                     ((uint32_t)0x00200000)        /*!< Channel 6 Transfer Complete clear */
#define  DMA_IFCR_CHTIF6                     ((uint32_t)0x00400000)        /*!< Channel 6 Half Transfer clear */
#define  DMA_IFCR_CTEIF6                     ((uint32_t)0x00800000)        /*!< Channel 6 Transfer Error clear */
#define  DMA_IFCR_CGIF7                      ((uint32_t)0x01000000)        /*!< Channel 7 Global interrupt clear */
#define  DMA_IFCR_CTCIF7                     ((uint32_t)0x02000000)        /*!< Channel 7 Transfer Complete clear */
#define  DMA_IFCR_CHTIF7                     ((uint32_t)0x04000000)        /*!< Channel 7 Half Transfer clear */
#define  DMA_IFCR_CTEIF7                     ((uint32_t)0x08000000)        /*!< Channel 7 Transfer Error clear */

/*******************  Bit definition for DMA_CCR1 register  *******************/
#define  DMA_CCR1_EN                         ((uint16_t)0x0001)            /*!< Channel enable*/
#define  DMA_CCR1_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR1_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR1_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR1_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR1_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR1_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR1_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR1_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR1_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR1_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR1_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR1_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR1_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR1_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits(Channel Priority level) */
#define  DMA_CCR1_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR1_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR1_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode */

/*******************  Bit definition for DMA_CCR2 register  *******************/
#define  DMA_CCR2_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR2_TCIE                       ((uint16_t)0x0002)            /*!< ransfer complete interrupt enable */
#define  DMA_CCR2_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR2_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR2_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR2_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR2_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR2_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR2_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR2_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR2_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR2_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR2_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR2_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR2_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR2_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR2_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR2_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode */

/*******************  Bit definition for DMA_CCR3 register  *******************/
#define  DMA_CCR3_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR3_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR3_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR3_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR3_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR3_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR3_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR3_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR3_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR3_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR3_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR3_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR3_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR3_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR3_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR3_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR3_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR3_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode */

/*!<******************  Bit definition for DMA_CCR4 register  *******************/
#define  DMA_CCR4_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR4_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR4_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR4_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR4_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR4_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR4_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR4_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR4_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR4_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR4_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR4_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR4_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR4_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR4_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR4_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR4_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR4_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode */

/******************  Bit definition for DMA_CCR5 register  *******************/
#define  DMA_CCR5_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR5_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR5_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR5_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR5_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR5_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR5_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR5_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR5_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR5_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR5_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR5_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR5_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR5_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR5_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR5_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR5_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR5_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode enable */

/*******************  Bit definition for DMA_CCR6 register  *******************/
#define  DMA_CCR6_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR6_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR6_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR6_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR6_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR6_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR6_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR6_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR6_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR6_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR6_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR6_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR6_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR6_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR6_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR6_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR6_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR6_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode */

/*******************  Bit definition for DMA_CCR7 register  *******************/
#define  DMA_CCR7_EN                         ((uint16_t)0x0001)            /*!< Channel enable */
#define  DMA_CCR7_TCIE                       ((uint16_t)0x0002)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR7_HTIE                       ((uint16_t)0x0004)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR7_TEIE                       ((uint16_t)0x0008)            /*!< Transfer error interrupt enable */
#define  DMA_CCR7_DIR                        ((uint16_t)0x0010)            /*!< Data transfer direction */
#define  DMA_CCR7_CIRC                       ((uint16_t)0x0020)            /*!< Circular mode */
#define  DMA_CCR7_PINC                       ((uint16_t)0x0040)            /*!< Peripheral increment mode */
#define  DMA_CCR7_MINC                       ((uint16_t)0x0080)            /*!< Memory increment mode */

#define  DMA_CCR7_PSIZE                      ((uint16_t)0x0300)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR7_PSIZE_0                    ((uint16_t)0x0100)            /*!< Bit 0 */
#define  DMA_CCR7_PSIZE_1                    ((uint16_t)0x0200)            /*!< Bit 1 */

#define  DMA_CCR7_MSIZE                      ((uint16_t)0x0C00)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR7_MSIZE_0                    ((uint16_t)0x0400)            /*!< Bit 0 */
#define  DMA_CCR7_MSIZE_1                    ((uint16_t)0x0800)            /*!< Bit 1 */

#define  DMA_CCR7_PL                         ((uint16_t)0x3000)            /*!< PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR7_PL_0                       ((uint16_t)0x1000)            /*!< Bit 0 */
#define  DMA_CCR7_PL_1                       ((uint16_t)0x2000)            /*!< Bit 1 */

#define  DMA_CCR7_MEM2MEM                    ((uint16_t)0x4000)            /*!< Memory to memory mode enable */

/******************  Bit definition for DMA_CNDTR1 register  ******************/
#define  DMA_CNDTR1_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR2 register  ******************/
#define  DMA_CNDTR2_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR3 register  ******************/
#define  DMA_CNDTR3_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR4 register  ******************/
#define  DMA_CNDTR4_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR5 register  ******************/
#define  DMA_CNDTR5_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR6 register  ******************/
#define  DMA_CNDTR6_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR7 register  ******************/
#define  DMA_CNDTR7_NDT                      ((uint16_t)0xFFFF)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CPAR1 register  *******************/
#define  DMA_CPAR1_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR2 register  *******************/
#define  DMA_CPAR2_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR3 register  *******************/
#define  DMA_CPAR3_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */


/******************  Bit definition for DMA_CPAR4 register  *******************/
#define  DMA_CPAR4_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR5 register  *******************/
#define  DMA_CPAR5_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR6 register  *******************/
#define  DMA_CPAR6_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */


/******************  Bit definition for DMA_CPAR7 register  *******************/
#define  DMA_CPAR7_PA                        ((uint32_t)0xFFFFFFFF)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CMAR1 register  *******************/
#define  DMA_CMAR1_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */
                                                                     
/******************  Bit definition for DMA_CMAR2 register  *******************/
#define  DMA_CMAR2_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR3 register  *******************/
#define  DMA_CMAR3_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */


/******************  Bit definition for DMA_CMAR4 register  *******************/
#define  DMA_CMAR4_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR5 register  *******************/
#define  DMA_CMAR5_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR6 register  *******************/
#define  DMA_CMAR6_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR7 register  *******************/
#define  DMA_CMAR7_MA                        ((uint32_t)0xFFFFFFFF)        /*!< Memory Address */

/*******************  Bit definition for DMA_CSELR register  *******************/
#define  DMA_CSELR_C1S                          ((uint32_t)0x0000007)          /*!< Channel 1 Selection */ 
#define  DMA_CSELR_C2S                          ((uint32_t)0x0000070)          /*!< Channel 2 Selection */ 
#define  DMA_CSELR_C3S                          ((uint32_t)0x0000700)          /*!< Channel 3 Selection */ 
#define  DMA_CSELR_C4S                          ((uint32_t)0x0007000)          /*!< Channel 4 Selection */ 
#define  DMA_CSELR_C5S                          ((uint32_t)0x0070000)          /*!< Channel 5 Selection */ 
#define  DMA_CSELR_C6S                          ((uint32_t)0x0700000)          /*!< Channel 6 Selection */ 
#define  DMA_CSELR_C7S                          ((uint32_t)0x7000000)          /*!< Channel 7 Selection */ 



/******************************************************************************/
/*                                                                            */
/*                    External Interrupt/Event Controller                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for EXTI_IMR register  *******************/
#define  EXTI_IMR_IM0                        ((uint32_t)0x00000001)        /*!< Interrupt Mask on line 0 */
#define  EXTI_IMR_IM1                        ((uint32_t)0x00000002)        /*!< Interrupt Mask on line 1 */
#define  EXTI_IMR_IM2                        ((uint32_t)0x00000004)        /*!< Interrupt Mask on line 2 */
#define  EXTI_IMR_IM3                        ((uint32_t)0x00000008)        /*!< Interrupt Mask on line 3 */
#define  EXTI_IMR_IM4                        ((uint32_t)0x00000010)        /*!< Interrupt Mask on line 4 */
#define  EXTI_IMR_IM5                        ((uint32_t)0x00000020)        /*!< Interrupt Mask on line 5 */
#define  EXTI_IMR_IM6                        ((uint32_t)0x00000040)        /*!< Interrupt Mask on line 6 */
#define  EXTI_IMR_IM7                        ((uint32_t)0x00000080)        /*!< Interrupt Mask on line 7 */
#define  EXTI_IMR_IM8                        ((uint32_t)0x00000100)        /*!< Interrupt Mask on line 8 */
#define  EXTI_IMR_IM9                        ((uint32_t)0x00000200)        /*!< Interrupt Mask on line 9 */
#define  EXTI_IMR_IM10                       ((uint32_t)0x00000400)        /*!< Interrupt Mask on line 10 */
#define  EXTI_IMR_IM11                       ((uint32_t)0x00000800)        /*!< Interrupt Mask on line 11 */
#define  EXTI_IMR_IM12                       ((uint32_t)0x00001000)        /*!< Interrupt Mask on line 12 */
#define  EXTI_IMR_IM13                       ((uint32_t)0x00002000)        /*!< Interrupt Mask on line 13 */
#define  EXTI_IMR_IM14                       ((uint32_t)0x00004000)        /*!< Interrupt Mask on line 14 */
#define  EXTI_IMR_IM15                       ((uint32_t)0x00008000)        /*!< Interrupt Mask on line 15 */
#define  EXTI_IMR_IM16                       ((uint32_t)0x00010000)        /*!< Interrupt Mask on line 16 */
#define  EXTI_IMR_IM17                       ((uint32_t)0x00020000)        /*!< Interrupt Mask on line 17 */
#define  EXTI_IMR_IM18                       ((uint32_t)0x00040000)        /*!< Interrupt Mask on line 18 */
#define  EXTI_IMR_IM19                       ((uint32_t)0x00080000)        /*!< Interrupt Mask on line 19 */
#define  EXTI_IMR_IM20                       ((uint32_t)0x00100000)        /*!< Interrupt Mask on line 20 */
#define  EXTI_IMR_IM21                       ((uint32_t)0x00200000)        /*!< Interrupt Mask on line 21 */
#define  EXTI_IMR_IM22                       ((uint32_t)0x00400000)        /*!< Interrupt Mask on line 22 */
#define  EXTI_IMR_IM23                       ((uint32_t)0x00800000)        /*!< Interrupt Mask on line 23 */
#define  EXTI_IMR_IM24                       ((uint32_t)0x01000000)        /*!< Interrupt Mask on line 24 */
#define  EXTI_IMR_IM25                       ((uint32_t)0x02000000)        /*!< Interrupt Mask on line 25 */
#define  EXTI_IMR_IM26                       ((uint32_t)0x04000000)        /*!< Interrupt Mask on line 26 */
#define  EXTI_IMR_IM27                       ((uint32_t)0x08000000)        /*!< Interrupt Mask on line 27 */
#define  EXTI_IMR_IM28                       ((uint32_t)0x10000000)        /*!< Interrupt Mask on line 28 */

/*******************  Bit definition for EXTI_EMR register  *******************/
#define  EXTI_EMR_EM0                        ((uint32_t)0x00000001)        /*!< Event Mask on line 0 */
#define  EXTI_EMR_EM1                        ((uint32_t)0x00000002)        /*!< Event Mask on line 1 */
#define  EXTI_EMR_EM2                        ((uint32_t)0x00000004)        /*!< Event Mask on line 2 */
#define  EXTI_EMR_EM3                        ((uint32_t)0x00000008)        /*!< Event Mask on line 3 */
#define  EXTI_EMR_EM4                        ((uint32_t)0x00000010)        /*!< Event Mask on line 4 */
#define  EXTI_EMR_EM5                        ((uint32_t)0x00000020)        /*!< Event Mask on line 5 */
#define  EXTI_EMR_EM6                        ((uint32_t)0x00000040)        /*!< Event Mask on line 6 */
#define  EXTI_EMR_EM7                        ((uint32_t)0x00000080)        /*!< Event Mask on line 7 */
#define  EXTI_EMR_EM8                        ((uint32_t)0x00000100)        /*!< Event Mask on line 8 */
#define  EXTI_EMR_EM9                        ((uint32_t)0x00000200)        /*!< Event Mask on line 9 */
#define  EXTI_EMR_EM10                       ((uint32_t)0x00000400)        /*!< Event Mask on line 10 */
#define  EXTI_EMR_EM11                       ((uint32_t)0x00000800)        /*!< Event Mask on line 11 */
#define  EXTI_EMR_EM12                       ((uint32_t)0x00001000)        /*!< Event Mask on line 12 */
#define  EXTI_EMR_EM13                       ((uint32_t)0x00002000)        /*!< Event Mask on line 13 */
#define  EXTI_EMR_EM14                       ((uint32_t)0x00004000)        /*!< Event Mask on line 14 */
#define  EXTI_EMR_EM15                       ((uint32_t)0x00008000)        /*!< Event Mask on line 15 */
#define  EXTI_EMR_EM16                       ((uint32_t)0x00010000)        /*!< Event Mask on line 16 */
#define  EXTI_EMR_EM17                       ((uint32_t)0x00020000)        /*!< Event Mask on line 17 */
#define  EXTI_EMR_EM18                       ((uint32_t)0x00040000)        /*!< Event Mask on line 18 */
#define  EXTI_EMR_EM19                       ((uint32_t)0x00080000)        /*!< Event Mask on line 19 */
#define  EXTI_EMR_EM20                       ((uint32_t)0x00100000)        /*!< Event Mask on line 20 */
#define  EXTI_EMR_EM21                       ((uint32_t)0x00200000)        /*!< Event Mask on line 21 */
#define  EXTI_EMR_EM22                       ((uint32_t)0x00400000)        /*!< Event Mask on line 22 */
#define  EXTI_EMR_EM23                       ((uint32_t)0x00800000)        /*!< Event Mask on line 23 */
#define  EXTI_EMR_EM24                       ((uint32_t)0x01000000)        /*!< Event Mask on line 24 */
#define  EXTI_EMR_EM25                       ((uint32_t)0x02000000)        /*!< Event Mask on line 25 */
#define  EXTI_EMR_EM26                       ((uint32_t)0x04000000)        /*!< Event Mask on line 26 */
#define  EXTI_EMR_EM27                       ((uint32_t)0x08000000)        /*!< Event Mask on line 27 */
#define  EXTI_EMR_EM28                       ((uint32_t)0x10000000)        /*!< Event Mask on line 28 */

/******************  Bit definition for EXTI_RTSR register  *******************/
#define  EXTI_RTSR_TR0                       ((uint32_t)0x00000001)        /*!< Rising trigger event configuration bit of line 0 */
#define  EXTI_RTSR_TR1                       ((uint32_t)0x00000002)        /*!< Rising trigger event configuration bit of line 1 */
#define  EXTI_RTSR_TR2                       ((uint32_t)0x00000004)        /*!< Rising trigger event configuration bit of line 2 */
#define  EXTI_RTSR_TR3                       ((uint32_t)0x00000008)        /*!< Rising trigger event configuration bit of line 3 */
#define  EXTI_RTSR_TR4                       ((uint32_t)0x00000010)        /*!< Rising trigger event configuration bit of line 4 */
#define  EXTI_RTSR_TR5                       ((uint32_t)0x00000020)        /*!< Rising trigger event configuration bit of line 5 */
#define  EXTI_RTSR_TR6                       ((uint32_t)0x00000040)        /*!< Rising trigger event configuration bit of line 6 */
#define  EXTI_RTSR_TR7                       ((uint32_t)0x00000080)        /*!< Rising trigger event configuration bit of line 7 */
#define  EXTI_RTSR_TR8                       ((uint32_t)0x00000100)        /*!< Rising trigger event configuration bit of line 8 */
#define  EXTI_RTSR_TR9                       ((uint32_t)0x00000200)        /*!< Rising trigger event configuration bit of line 9 */
#define  EXTI_RTSR_TR10                      ((uint32_t)0x00000400)        /*!< Rising trigger event configuration bit of line 10 */
#define  EXTI_RTSR_TR11                      ((uint32_t)0x00000800)        /*!< Rising trigger event configuration bit of line 11 */
#define  EXTI_RTSR_TR12                      ((uint32_t)0x00001000)        /*!< Rising trigger event configuration bit of line 12 */
#define  EXTI_RTSR_TR13                      ((uint32_t)0x00002000)        /*!< Rising trigger event configuration bit of line 13 */
#define  EXTI_RTSR_TR14                      ((uint32_t)0x00004000)        /*!< Rising trigger event configuration bit of line 14 */
#define  EXTI_RTSR_TR15                      ((uint32_t)0x00008000)        /*!< Rising trigger event configuration bit of line 15 */
#define  EXTI_RTSR_TR16                      ((uint32_t)0x00010000)        /*!< Rising trigger event configuration bit of line 16 */
#define  EXTI_RTSR_TR17                      ((uint32_t)0x00020000)        /*!< Rising trigger event configuration bit of line 17 */
#define  EXTI_RTSR_TR18                      ((uint32_t)0x00040000)        /*!< Rising trigger event configuration bit of line 18 */
#define  EXTI_RTSR_TR19                      ((uint32_t)0x00080000)        /*!< Rising trigger event configuration bit of line 19 */
#define  EXTI_RTSR_TR20                      ((uint32_t)0x00100000)        /*!< Rising trigger event configuration bit of line 20 */
#define  EXTI_RTSR_TR21                      ((uint32_t)0x00200000)        /*!< Rising trigger event configuration bit of line 21 */
#define  EXTI_RTSR_TR22                      ((uint32_t)0x00400000)        /*!< Rising trigger event configuration bit of line 22 */
#define  EXTI_RTSR_TR23                      ((uint32_t)0x00800000)        /*!< Rising trigger event configuration bit of line 23 */
#define  EXTI_RTSR_TR24                      ((uint32_t)0x01000000)        /*!< Rising trigger event configuration bit of line 24 */
#define  EXTI_RTSR_TR25                      ((uint32_t)0x02000000)        /*!< Rising trigger event configuration bit of line 25 */
#define  EXTI_RTSR_TR26                      ((uint32_t)0x04000000)        /*!< Rising trigger event configuration bit of line 26 */
#define  EXTI_RTSR_TR27                      ((uint32_t)0x08000000)        /*!< Rising trigger event configuration bit of line 27 */
#define  EXTI_RTSR_TR28                      ((uint32_t)0x10000000)        /*!< Rising trigger event configuration bit of line 28 */

/******************  Bit definition for EXTI_FTSR register  *******************/
#define  EXTI_FTSR_TR0                       ((uint32_t)0x00000001)        /*!< Falling trigger event configuration bit of line 0 */
#define  EXTI_FTSR_TR1                       ((uint32_t)0x00000002)        /*!< Falling trigger event configuration bit of line 1 */
#define  EXTI_FTSR_TR2                       ((uint32_t)0x00000004)        /*!< Falling trigger event configuration bit of line 2 */
#define  EXTI_FTSR_TR3                       ((uint32_t)0x00000008)        /*!< Falling trigger event configuration bit of line 3 */
#define  EXTI_FTSR_TR4                       ((uint32_t)0x00000010)        /*!< Falling trigger event configuration bit of line 4 */
#define  EXTI_FTSR_TR5                       ((uint32_t)0x00000020)        /*!< Falling trigger event configuration bit of line 5 */
#define  EXTI_FTSR_TR6                       ((uint32_t)0x00000040)        /*!< Falling trigger event configuration bit of line 6 */
#define  EXTI_FTSR_TR7                       ((uint32_t)0x00000080)        /*!< Falling trigger event configuration bit of line 7 */
#define  EXTI_FTSR_TR8                       ((uint32_t)0x00000100)        /*!< Falling trigger event configuration bit of line 8 */
#define  EXTI_FTSR_TR9                       ((uint32_t)0x00000200)        /*!< Falling trigger event configuration bit of line 9 */
#define  EXTI_FTSR_TR10                      ((uint32_t)0x00000400)        /*!< Falling trigger event configuration bit of line 10 */
#define  EXTI_FTSR_TR11                      ((uint32_t)0x00000800)        /*!< Falling trigger event configuration bit of line 11 */
#define  EXTI_FTSR_TR12                      ((uint32_t)0x00001000)        /*!< Falling trigger event configuration bit of line 12 */
#define  EXTI_FTSR_TR13                      ((uint32_t)0x00002000)        /*!< Falling trigger event configuration bit of line 13 */
#define  EXTI_FTSR_TR14                      ((uint32_t)0x00004000)        /*!< Falling trigger event configuration bit of line 14 */
#define  EXTI_FTSR_TR15                      ((uint32_t)0x00008000)        /*!< Falling trigger event configuration bit of line 15 */
#define  EXTI_FTSR_TR16                      ((uint32_t)0x00010000)        /*!< Falling trigger event configuration bit of line 16 */
#define  EXTI_FTSR_TR17                      ((uint32_t)0x00020000)        /*!< Falling trigger event configuration bit of line 17 */
#define  EXTI_FTSR_TR18                      ((uint32_t)0x00040000)        /*!< Falling trigger event configuration bit of line 18 */
#define  EXTI_FTSR_TR19                      ((uint32_t)0x00080000)        /*!< Falling trigger event configuration bit of line 19 */
#define  EXTI_FTSR_TR20                      ((uint32_t)0x00100000)        /*!< Falling trigger event configuration bit of line 20 */
#define  EXTI_FTSR_TR21                      ((uint32_t)0x00200000)        /*!< Falling trigger event configuration bit of line 21 */
#define  EXTI_FTSR_TR22                      ((uint32_t)0x00400000)        /*!< Falling trigger event configuration bit of line 22 */
#define  EXTI_FTSR_TR23                      ((uint32_t)0x00800000)        /*!< Falling trigger event configuration bit of line 23 */
#define  EXTI_FTSR_TR24                      ((uint32_t)0x01000000)        /*!< Falling trigger event configuration bit of line 24 */
#define  EXTI_FTSR_TR25                      ((uint32_t)0x02000000)        /*!< Falling trigger event configuration bit of line 25 */
#define  EXTI_FTSR_TR26                      ((uint32_t)0x04000000)        /*!< Falling trigger event configuration bit of line 26 */
#define  EXTI_FTSR_TR27                      ((uint32_t)0x08000000)        /*!< Falling trigger event configuration bit of line 27 */
#define  EXTI_FTSR_TR28                      ((uint32_t)0x10000000)        /*!< Falling trigger event configuration bit of line 28 */

/******************  Bit definition for EXTI_SWIER register  ******************/
#define  EXTI_SWIER_SWIER0                   ((uint32_t)0x00000001)        /*!< Software Interrupt on line 0 */
#define  EXTI_SWIER_SWIER1                   ((uint32_t)0x00000002)        /*!< Software Interrupt on line 1 */
#define  EXTI_SWIER_SWIER2                   ((uint32_t)0x00000004)        /*!< Software Interrupt on line 2 */
#define  EXTI_SWIER_SWIER3                   ((uint32_t)0x00000008)        /*!< Software Interrupt on line 3 */
#define  EXTI_SWIER_SWIER4                   ((uint32_t)0x00000010)        /*!< Software Interrupt on line 4 */
#define  EXTI_SWIER_SWIER5                   ((uint32_t)0x00000020)        /*!< Software Interrupt on line 5 */
#define  EXTI_SWIER_SWIER6                   ((uint32_t)0x00000040)        /*!< Software Interrupt on line 6 */
#define  EXTI_SWIER_SWIER7                   ((uint32_t)0x00000080)        /*!< Software Interrupt on line 7 */
#define  EXTI_SWIER_SWIER8                   ((uint32_t)0x00000100)        /*!< Software Interrupt on line 8 */
#define  EXTI_SWIER_SWIER9                   ((uint32_t)0x00000200)        /*!< Software Interrupt on line 9 */
#define  EXTI_SWIER_SWIER10                  ((uint32_t)0x00000400)        /*!< Software Interrupt on line 10 */
#define  EXTI_SWIER_SWIER11                  ((uint32_t)0x00000800)        /*!< Software Interrupt on line 11 */
#define  EXTI_SWIER_SWIER12                  ((uint32_t)0x00001000)        /*!< Software Interrupt on line 12 */
#define  EXTI_SWIER_SWIER13                  ((uint32_t)0x00002000)        /*!< Software Interrupt on line 13 */
#define  EXTI_SWIER_SWIER14                  ((uint32_t)0x00004000)        /*!< Software Interrupt on line 14 */
#define  EXTI_SWIER_SWIER15                  ((uint32_t)0x00008000)        /*!< Software Interrupt on line 15 */
#define  EXTI_SWIER_SWIER16                  ((uint32_t)0x00010000)        /*!< Software Interrupt on line 16 */
#define  EXTI_SWIER_SWIER17                  ((uint32_t)0x00020000)        /*!< Software Interrupt on line 17 */
#define  EXTI_SWIER_SWIER18                  ((uint32_t)0x00040000)        /*!< Software Interrupt on line 18 */
#define  EXTI_SWIER_SWIER19                  ((uint32_t)0x00080000)        /*!< Software Interrupt on line 19 */
#define  EXTI_SWIER_SWIER20                  ((uint32_t)0x00100000)        /*!< Software Interrupt on line 20 */
#define  EXTI_SWIER_SWIER21                  ((uint32_t)0x00200000)        /*!< Software Interrupt on line 21 */
#define  EXTI_SWIER_SWIER22                  ((uint32_t)0x00400000)        /*!< Software Interrupt on line 22 */
#define  EXTI_SWIER_SWIER23                  ((uint32_t)0x00800000)        /*!< Software Interrupt on line 23 */
#define  EXTI_SWIER_SWIER24                  ((uint32_t)0x01000000)        /*!< Software Interrupt on line 24 */
#define  EXTI_SWIER_SWIER25                  ((uint32_t)0x02000000)        /*!< Software Interrupt on line 25 */
#define  EXTI_SWIER_SWIER26                  ((uint32_t)0x04000000)        /*!< Software Interrupt on line 26 */
#define  EXTI_SWIER_SWIER27                  ((uint32_t)0x08000000)        /*!< Software Interrupt on line 27 */
#define  EXTI_SWIER_SWIER28                  ((uint32_t)0x10000000)        /*!< Software Interrupt on line 28 */

/*******************  Bit definition for EXTI_PR register  ********************/
#define  EXTI_PR_PR0                         ((uint32_t)0x00000001)        /*!< Pending bit for line 0 */
#define  EXTI_PR_PR1                         ((uint32_t)0x00000002)        /*!< Pending bit for line 1 */
#define  EXTI_PR_PR2                         ((uint32_t)0x00000004)        /*!< Pending bit for line 2 */
#define  EXTI_PR_PR3                         ((uint32_t)0x00000008)        /*!< Pending bit for line 3 */
#define  EXTI_PR_PR4                         ((uint32_t)0x00000010)        /*!< Pending bit for line 4 */
#define  EXTI_PR_PR5                         ((uint32_t)0x00000020)        /*!< Pending bit for line 5 */
#define  EXTI_PR_PR6                         ((uint32_t)0x00000040)        /*!< Pending bit for line 6 */
#define  EXTI_PR_PR7                         ((uint32_t)0x00000080)        /*!< Pending bit for line 7 */
#define  EXTI_PR_PR8                         ((uint32_t)0x00000100)        /*!< Pending bit for line 8 */
#define  EXTI_PR_PR9                         ((uint32_t)0x00000200)        /*!< Pending bit for line 9 */
#define  EXTI_PR_PR10                        ((uint32_t)0x00000400)        /*!< Pending bit for line 10 */
#define  EXTI_PR_PR11                        ((uint32_t)0x00000800)        /*!< Pending bit for line 11 */
#define  EXTI_PR_PR12                        ((uint32_t)0x00001000)        /*!< Pending bit for line 12 */
#define  EXTI_PR_PR13                        ((uint32_t)0x00002000)        /*!< Pending bit for line 13 */
#define  EXTI_PR_PR14                        ((uint32_t)0x00004000)        /*!< Pending bit for line 14 */
#define  EXTI_PR_PR15                        ((uint32_t)0x00008000)        /*!< Pending bit for line 15 */
#define  EXTI_PR_PR16                        ((uint32_t)0x00010000)        /*!< Pending bit for line 16 */
#define  EXTI_PR_PR17                        ((uint32_t)0x00020000)        /*!< Pending bit for line 17 */
#define  EXTI_PR_PR18                        ((uint32_t)0x00040000)        /*!< Pending bit for line 18 */
#define  EXTI_PR_PR19                        ((uint32_t)0x00080000)        /*!< Pending bit for line 19 */
#define  EXTI_PR_PR20                        ((uint32_t)0x00100000)        /*!< Pending bit for line 20 */
#define  EXTI_PR_PR21                        ((uint32_t)0x00200000)        /*!< Pending bit for line 21 */
#define  EXTI_PR_PR22                        ((uint32_t)0x00400000)        /*!< Pending bit for line 22 */
#define  EXTI_PR_PR23                        ((uint32_t)0x00800000)        /*!< Pending bit for line 23 */
#define  EXTI_PR_PR24                        ((uint32_t)0x01000000)        /*!< Pending bit for line 24 */
#define  EXTI_PR_PR25                        ((uint32_t)0x02000000)        /*!< Pending bit for line 25 */
#define  EXTI_PR_PR26                        ((uint32_t)0x04000000)        /*!< Pending bit for line 26 */
#define  EXTI_PR_PR27                        ((uint32_t)0x08000000)        /*!< Pending bit for line 27 */
#define  EXTI_PR_PR28                        ((uint32_t)0x10000000)        /*!< Pending bit for line 28 */


/******************************************************************************/
/*                                                                            */
/*                                    FLASH                                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bits definition for FLASH_ACR register  *****************/
#define FLASH_ACR_LATENCY                    ((uint32_t)0x00000007)
#define FLASH_ACR_LATENCY_0WS                ((uint32_t)0x00000000)
#define FLASH_ACR_LATENCY_1WS                ((uint32_t)0x00000001)
#define FLASH_ACR_LATENCY_2WS                ((uint32_t)0x00000002)
#define FLASH_ACR_LATENCY_3WS                ((uint32_t)0x00000003)
#define FLASH_ACR_LATENCY_4WS                ((uint32_t)0x00000004)


#define FLASH_ACR_PRFTEN                     ((uint32_t)0x00000100)
#define FLASH_ACR_ICEN                       ((uint32_t)0x00000200)
#define FLASH_ACR_DCEN                       ((uint32_t)0x00000400)
#define FLASH_ACR_ICRST                      ((uint32_t)0x00000800)
#define FLASH_ACR_DCRST                      ((uint32_t)0x00001000)
#define FLASH_ACR_RUNPD                      ((uint32_t)0x00002000)   /*!< Flash power down mode during run */

/*******************  Bits definition for FLASH_SR register  ******************/
#define FLASH_SR_EOP                         ((uint32_t)0x00000001)
#define FLASH_SR_OPERR                       ((uint32_t)0x00000002)
#define FLASH_SR_PROGERR                     ((uint32_t)0x00000008)
#define FLASH_SR_WRPERR                      ((uint32_t)0x00000010)
#define FLASH_SR_PGAERR                      ((uint32_t)0x00000020)
#define FLASH_SR_SIZERR                      ((uint32_t)0x00000040)
#define FLASH_SR_PGSERR                      ((uint32_t)0x00000080)
#define FLASH_SR_MISSERR                     ((uint32_t)0x00000100)
#define FLASH_SR_FASTERR                     ((uint32_t)0x00000200)
#define FLASH_SR_RDERR                       ((uint32_t)0x00004000)
#define FLASH_SR_OPTVERR                     ((uint32_t)0x00008000)
#define FLASH_SR_BSY                         ((uint32_t)0x00010000)

/*******************  Bits definition for FLASH_CR register  ******************/
#define FLASH_CR_PG                          ((uint32_t)0x00000001)
#define FLASH_CR_PER                         ((uint32_t)0x00000002)
#define FLASH_CR_MER1                        ((uint32_t)0x00000004)
#define FLASH_CR_PNB                         ((uint32_t)0x000007F8)
#define FLASH_CR_PNB_0                       ((uint32_t)0x00000008)
#define FLASH_CR_PNB_1                       ((uint32_t)0x00000010)
#define FLASH_CR_PNB_2                       ((uint32_t)0x00000020)
#define FLASH_CR_PNB_3                       ((uint32_t)0x00000040)
#define FLASH_CR_PNB_4                       ((uint32_t)0x00000080)
#define FLASH_CR_PNB_5                       ((uint32_t)0x00000100)
#define FLASH_CR_PNB_6                       ((uint32_t)0x00000200)
#define FLASH_CR_PNB_7                       ((uint32_t)0x00000400)
#define FLASH_CR_BKER                        ((uint32_t)0x00000800)
#define FLASH_CR_BIFER                       ((uint32_t)0x00001000)
#define FLASH_CR_MER2                        ((uint32_t)0x00008000)
#define FLASH_CR_STRT                        ((uint32_t)0x00010000)
#define FLASH_CR_OPTSTRT                     ((uint32_t)0x00020000)
#define FLASH_CR_FSTPG                       ((uint32_t)0x00040000)
#define FLASH_CR_VDDLOW                      ((uint32_t)0x00800000)
#define FLASH_CR_EOPIE                       ((uint32_t)0x01000000)
#define FLASH_CR_ERRIE                       ((uint32_t)0x02000000)
#define FLASH_CR_RDERRIE                     ((uint32_t)0x04000000)
#define FLASH_CR_OBLLAUNCH                   ((uint32_t)0x08000000)
#define FLASH_CR_OPTLOCK                     ((uint32_t)0x40000000)
#define FLASH_CR_LOCK                        ((uint32_t)0x80000000)


/*******************  Bits definition for FLASH_OPTCR register  ***************/
#define FLASH_OPTCR_RDP                     ((uint32_t)0x000000FF)
#define FLASH_OPTCR_RDP_0                   ((uint32_t)0x00000001)
#define FLASH_OPTCR_RDP_1                   ((uint32_t)0x00000002)
#define FLASH_OPTCR_RDP_2                   ((uint32_t)0x00000004)
#define FLASH_OPTCR_RDP_3                   ((uint32_t)0x00000008)
#define FLASH_OPTCR_RDP_4                   ((uint32_t)0x00000010)
#define FLASH_OPTCR_RDP_5                   ((uint32_t)0x00000020)
#define FLASH_OPTCR_RDP_6                   ((uint32_t)0x00000040)
#define FLASH_OPTCR_RDP_7                   ((uint32_t)0x00000080)

#define FLASH_OPTCR_BOR_LEV_0               ((uint32_t)0x00000000)
#define FLASH_OPTCR_BOR_LEV_1               ((uint32_t)0x00000010)
#define FLASH_OPTCR_BOR_LEV_2               ((uint32_t)0x00000020)
#define FLASH_OPTCR_BOR_LEV_3               ((uint32_t)0x00000030)
#define FLASH_OPTCR_BOR_LEV_4               ((uint32_t)0x00000040)
#define FLASH_OPTCR_BOR_LEV                 ((uint32_t)0x00000070)

#define FLASH_OPTCR_nRST_STOP               ((uint32_t)0x00001000)
#define FLASH_OPTCR_nRST_STDBY              ((uint32_t)0x00002000)
#define FLASH_OPTCR_nRST_SHDW               ((uint32_t)0x00004000)
#define FLASH_OPTCR_IWDG_SW                 ((uint32_t)0x00010000)
#define FLASH_OPTCR_IWDG_STOP               ((uint32_t)0x00020000)
#define FLASH_OPTCR_IWDG_STDBY              ((uint32_t)0x00040000)
#define FLASH_OPTCR_WWDG_SW                 ((uint32_t)0x00080000)
#define FLASH_OPTCR_BFB2                    ((uint32_t)0x00100000)
#define FLASH_OPTCR_DB512K                  ((uint32_t)0x00200000)
#define FLASH_OPTCR_DB256K                  ((uint32_t)0x00400000)
#define FLASH_OPTCR_nBOOT1                  ((uint32_t)0x00800000)
#define FLASH_OPTCR_SRAM2_PE                ((uint32_t)0x01000000)
#define FLASH_OPTCR_SRAM2_RST               ((uint32_t)0x02000000)

/*******************  Bits definition for FLASH_ECCR register  ***************/
#define FLASH_ECCR_ECCD                     ((uint32_t)0x80000000)
#define FLASH_ECCR_ECCC                     ((uint32_t)0x40000000)
#define FLASH_ECCR_ECCIE                    ((uint32_t)0x01000000)
#define FLASH_ECCR_SYSF_ECC                 ((uint32_t)0x00100000)
#define FLASH_ECCR_BK_ECC                   ((uint32_t)0x00080000)
#define FLASH_ECCR_ADDR_ECC                 ((uint32_t)0x000FFFFF)


/******************  Bits definition for FLASH_PCROP1SR register  **********/
#define FLASH_PCROP1SR_OFFSET_STR           ((uint32_t)0x0000FFFF)
/******************  Bits definition for FLASH_PCROP1ER register  ***********/
 #define FLASH_PCROP1ER_RDP                 ((uint32_t)0x80000000)
 #define FLASH_PCROP1ER_OFFSET_END          ((uint32_t)0x0000FFFF)
  
/******************  Bits definition for FLASH_WRPA1R register  ***************/
#define FLASH_WRPA1R_OFFSET_STR             ((uint32_t)0x000000FF)
#define FLASH_WRPA1R_OFFSET_END             ((uint32_t)0x00FF0000)

/******************  Bits definition for FLASH_WRPB1R register  ***************/
#define FLASH_WRPB1R_OFFSET_STR             ((uint32_t)0x000000FF)
#define FLASH_WRPB1R_OFFSET_END             ((uint32_t)0x00FF0000)

/******************  Bits definition for FLASH_PCROP2SR register  **********/
#define FLASH_PCROP2SR_OFFSET_STR           ((uint32_t)0x0000FFFF)
/******************  Bits definition for FLASH_PCROP2ER register  ***********/
 #define FLASH_PCROP2ER_OFFSET_END          ((uint32_t)0x0000FFFF)
  
/******************  Bits definition for FLASH_WRPA2R register  ***************/
#define FLASH_WRPA2R_OFFSET_STR             ((uint32_t)0x000000FF)
#define FLASH_WRPA2R_OFFSET_END             ((uint32_t)0x00FF0000)

/******************  Bits definition for FLASH_WRPB2R register  ***************/
#define FLASH_WRPB2R_OFFSET_STR             ((uint32_t)0x000000FF)
#define FLASH_WRPB2R_OFFSET_END             ((uint32_t)0x00FF0000)


/******************************************************************************/
/*                                                                            */
/*                          Flexible Memory Controller                        */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for FMC_BCR1 register  *******************/
#define  FMC_BCR1_MBKEN                     ((uint32_t)0x00000001)        /*!<Memory bank enable bit                 */
#define  FMC_BCR1_MUXEN                     ((uint32_t)0x00000002)        /*!<Address/data multiplexing enable bit   */

#define  FMC_BCR1_MTYP                      ((uint32_t)0x0000000C)        /*!<MTYP[1:0] bits (Memory type)           */
#define  FMC_BCR1_MTYP_0                    ((uint32_t)0x00000004)        /*!<Bit 0 */
#define  FMC_BCR1_MTYP_1                    ((uint32_t)0x00000008)        /*!<Bit 1 */

#define  FMC_BCR1_MWID                      ((uint32_t)0x00000030)        /*!<MWID[1:0] bits (Memory data bus width) */
#define  FMC_BCR1_MWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BCR1_MWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_BCR1_FACCEN                    ((uint32_t)0x00000040)        /*!<Flash access enable        */
#define  FMC_BCR1_BURSTEN                   ((uint32_t)0x00000100)        /*!<Burst enable bit           */
#define  FMC_BCR1_WAITPOL                   ((uint32_t)0x00000200)        /*!<Wait signal polarity bit   */
#define  FMC_BCR1_WRAPMOD                   ((uint32_t)0x00000400)        /*!<Wrapped burst mode support */
#define  FMC_BCR1_WAITCFG                   ((uint32_t)0x00000800)        /*!<Wait timing configuration  */
#define  FMC_BCR1_WREN                      ((uint32_t)0x00001000)        /*!<Write enable bit           */
#define  FMC_BCR1_WAITEN                    ((uint32_t)0x00002000)        /*!<Wait enable bit            */
#define  FMC_BCR1_EXTMOD                    ((uint32_t)0x00004000)        /*!<Extended mode enable       */
#define  FMC_BCR1_ASYNCWAIT                 ((uint32_t)0x00008000)        /*!<Asynchronous wait          */
#define  FMC_BCR1_CBURSTRW                  ((uint32_t)0x00080000)        /*!<Write burst enable         */
#define  FMC_BCR1_CCLKEN                    ((uint32_t)0x00100000)        /*!<Continous clock enable     */

/******************  Bit definition for FMC_BCR2 register  *******************/
#define  FMC_BCR2_MBKEN                     ((uint32_t)0x00000001)        /*!<Memory bank enable bit                 */
#define  FMC_BCR2_MUXEN                     ((uint32_t)0x00000002)        /*!<Address/data multiplexing enable bit   */

#define  FMC_BCR2_MTYP                      ((uint32_t)0x0000000C)        /*!<MTYP[1:0] bits (Memory type)           */
#define  FMC_BCR2_MTYP_0                    ((uint32_t)0x00000004)        /*!<Bit 0 */
#define  FMC_BCR2_MTYP_1                    ((uint32_t)0x00000008)        /*!<Bit 1 */

#define  FMC_BCR2_MWID                      ((uint32_t)0x00000030)        /*!<MWID[1:0] bits (Memory data bus width) */
#define  FMC_BCR2_MWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BCR2_MWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_BCR2_FACCEN                    ((uint32_t)0x00000040)        /*!<Flash access enable        */
#define  FMC_BCR2_BURSTEN                   ((uint32_t)0x00000100)        /*!<Burst enable bit           */
#define  FMC_BCR2_WAITPOL                   ((uint32_t)0x00000200)        /*!<Wait signal polarity bit   */
#define  FMC_BCR2_WRAPMOD                   ((uint32_t)0x00000400)        /*!<Wrapped burst mode support */
#define  FMC_BCR2_WAITCFG                   ((uint32_t)0x00000800)        /*!<Wait timing configuration  */
#define  FMC_BCR2_WREN                      ((uint32_t)0x00001000)        /*!<Write enable bit           */
#define  FMC_BCR2_WAITEN                    ((uint32_t)0x00002000)        /*!<Wait enable bit            */
#define  FMC_BCR2_EXTMOD                    ((uint32_t)0x00004000)        /*!<Extended mode enable       */
#define  FMC_BCR2_ASYNCWAIT                 ((uint32_t)0x00008000)        /*!<Asynchronous wait          */
#define  FMC_BCR2_CBURSTRW                  ((uint32_t)0x00080000)        /*!<Write burst enable         */

/******************  Bit definition for FMC_BCR3 register  *******************/
#define  FMC_BCR3_MBKEN                     ((uint32_t)0x00000001)        /*!<Memory bank enable bit                 */
#define  FMC_BCR3_MUXEN                     ((uint32_t)0x00000002)        /*!<Address/data multiplexing enable bit   */

#define  FMC_BCR3_MTYP                      ((uint32_t)0x0000000C)        /*!<MTYP[1:0] bits (Memory type)           */
#define  FMC_BCR3_MTYP_0                    ((uint32_t)0x00000004)        /*!<Bit 0 */
#define  FMC_BCR3_MTYP_1                    ((uint32_t)0x00000008)        /*!<Bit 1 */

#define  FMC_BCR3_MWID                      ((uint32_t)0x00000030)        /*!<MWID[1:0] bits (Memory data bus width) */
#define  FMC_BCR3_MWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BCR3_MWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_BCR3_FACCEN                    ((uint32_t)0x00000040)        /*!<Flash access enable        */
#define  FMC_BCR3_BURSTEN                   ((uint32_t)0x00000100)        /*!<Burst enable bit           */
#define  FMC_BCR3_WAITPOL                   ((uint32_t)0x00000200)        /*!<Wait signal polarity bit   */
#define  FMC_BCR3_WRAPMOD                   ((uint32_t)0x00000400)        /*!<Wrapped burst mode support */
#define  FMC_BCR3_WAITCFG                   ((uint32_t)0x00000800)        /*!<Wait timing configuration  */
#define  FMC_BCR3_WREN                      ((uint32_t)0x00001000)        /*!<Write enable bit           */
#define  FMC_BCR3_WAITEN                    ((uint32_t)0x00002000)        /*!<Wait enable bit            */
#define  FMC_BCR3_EXTMOD                    ((uint32_t)0x00004000)        /*!<Extended mode enable       */
#define  FMC_BCR3_ASYNCWAIT                 ((uint32_t)0x00008000)        /*!<Asynchronous wait          */
#define  FMC_BCR3_CBURSTRW                  ((uint32_t)0x00080000)        /*!<Write burst enable         */

/******************  Bit definition for FMC_BCR4 register  *******************/
#define  FMC_BCR4_MBKEN                     ((uint32_t)0x00000001)        /*!<Memory bank enable bit                 */
#define  FMC_BCR4_MUXEN                     ((uint32_t)0x00000002)        /*!<Address/data multiplexing enable bit   */

#define  FMC_BCR4_MTYP                      ((uint32_t)0x0000000C)        /*!<MTYP[1:0] bits (Memory type)           */
#define  FMC_BCR4_MTYP_0                    ((uint32_t)0x00000004)        /*!<Bit 0 */
#define  FMC_BCR4_MTYP_1                    ((uint32_t)0x00000008)        /*!<Bit 1 */

#define  FMC_BCR4_MWID                      ((uint32_t)0x00000030)        /*!<MWID[1:0] bits (Memory data bus width) */
#define  FMC_BCR4_MWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BCR4_MWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_BCR4_FACCEN                    ((uint32_t)0x00000040)        /*!<Flash access enable        */
#define  FMC_BCR4_BURSTEN                   ((uint32_t)0x00000100)        /*!<Burst enable bit           */
#define  FMC_BCR4_WAITPOL                   ((uint32_t)0x00000200)        /*!<Wait signal polarity bit   */
#define  FMC_BCR4_WRAPMOD                   ((uint32_t)0x00000400)        /*!<Wrapped burst mode support */
#define  FMC_BCR4_WAITCFG                   ((uint32_t)0x00000800)        /*!<Wait timing configuration  */
#define  FMC_BCR4_WREN                      ((uint32_t)0x00001000)        /*!<Write enable bit           */
#define  FMC_BCR4_WAITEN                    ((uint32_t)0x00002000)        /*!<Wait enable bit            */
#define  FMC_BCR4_EXTMOD                    ((uint32_t)0x00004000)        /*!<Extended mode enable       */
#define  FMC_BCR4_ASYNCWAIT                 ((uint32_t)0x00008000)        /*!<Asynchronous wait          */
#define  FMC_BCR4_CBURSTRW                  ((uint32_t)0x00080000)        /*!<Write burst enable         */

/******************  Bit definition for FMC_BTR1 register  ******************/
#define  FMC_BTR1_ADDSET                    ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BTR1_ADDSET_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BTR1_ADDSET_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BTR1_ADDSET_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BTR1_ADDSET_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BTR1_ADDHLD                    ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration)  */
#define  FMC_BTR1_ADDHLD_0                  ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BTR1_ADDHLD_1                  ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BTR1_ADDHLD_2                  ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BTR1_ADDHLD_3                  ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BTR1_DATAST                    ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BTR1_DATAST_0                  ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BTR1_DATAST_1                  ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BTR1_DATAST_2                  ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BTR1_DATAST_3                  ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BTR1_DATAST_4                  ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BTR1_DATAST_5                  ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BTR1_DATAST_6                  ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BTR1_DATAST_7                  ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BTR1_BUSTURN                   ((uint32_t)0x000F0000)        /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define  FMC_BTR1_BUSTURN_0                 ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_BTR1_BUSTURN_1                 ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_BTR1_BUSTURN_2                 ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_BTR1_BUSTURN_3                 ((uint32_t)0x00080000)        /*!<Bit 3 */

#define  FMC_BTR1_CLKDIV                    ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BTR1_CLKDIV_0                  ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BTR1_CLKDIV_1                  ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BTR1_CLKDIV_2                  ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BTR1_CLKDIV_3                  ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BTR1_DATLAT                    ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BTR1_DATLAT_0                  ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BTR1_DATLAT_1                  ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BTR1_DATLAT_2                  ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BTR1_DATLAT_3                  ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BTR1_ACCMOD                    ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BTR1_ACCMOD_0                  ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BTR1_ACCMOD_1                  ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BTR2 register  *******************/
#define  FMC_BTR2_ADDSET                    ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BTR2_ADDSET_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BTR2_ADDSET_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BTR2_ADDSET_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BTR2_ADDSET_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BTR2_ADDHLD                    ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BTR2_ADDHLD_0                  ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BTR2_ADDHLD_1                  ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BTR2_ADDHLD_2                  ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BTR2_ADDHLD_3                  ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BTR2_DATAST                    ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BTR2_DATAST_0                  ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BTR2_DATAST_1                  ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BTR2_DATAST_2                  ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BTR2_DATAST_3                  ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BTR2_DATAST_4                  ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BTR2_DATAST_5                  ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BTR2_DATAST_6                  ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BTR2_DATAST_7                  ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BTR2_BUSTURN                   ((uint32_t)0x000F0000)        /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define  FMC_BTR2_BUSTURN_0                 ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_BTR2_BUSTURN_1                 ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_BTR2_BUSTURN_2                 ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_BTR2_BUSTURN_3                 ((uint32_t)0x00080000)        /*!<Bit 3 */

#define  FMC_BTR2_CLKDIV                    ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BTR2_CLKDIV_0                  ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BTR2_CLKDIV_1                  ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BTR2_CLKDIV_2                  ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BTR2_CLKDIV_3                  ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BTR2_DATLAT                    ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BTR2_DATLAT_0                  ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BTR2_DATLAT_1                  ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BTR2_DATLAT_2                  ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BTR2_DATLAT_3                  ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BTR2_ACCMOD                    ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BTR2_ACCMOD_0                  ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BTR2_ACCMOD_1                  ((uint32_t)0x20000000)        /*!<Bit 1 */

/*******************  Bit definition for FMC_BTR3 register  *******************/
#define  FMC_BTR3_ADDSET                    ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BTR3_ADDSET_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BTR3_ADDSET_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BTR3_ADDSET_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BTR3_ADDSET_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BTR3_ADDHLD                    ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BTR3_ADDHLD_0                  ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BTR3_ADDHLD_1                  ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BTR3_ADDHLD_2                  ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BTR3_ADDHLD_3                  ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BTR3_DATAST                    ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BTR3_DATAST_0                  ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BTR3_DATAST_1                  ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BTR3_DATAST_2                  ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BTR3_DATAST_3                  ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BTR3_DATAST_4                  ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BTR3_DATAST_5                  ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BTR3_DATAST_6                  ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BTR3_DATAST_7                  ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BTR3_BUSTURN                   ((uint32_t)0x000F0000)        /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define  FMC_BTR3_BUSTURN_0                 ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_BTR3_BUSTURN_1                 ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_BTR3_BUSTURN_2                 ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_BTR3_BUSTURN_3                 ((uint32_t)0x00080000)        /*!<Bit 3 */

#define  FMC_BTR3_CLKDIV                    ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BTR3_CLKDIV_0                  ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BTR3_CLKDIV_1                  ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BTR3_CLKDIV_2                  ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BTR3_CLKDIV_3                  ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BTR3_DATLAT                    ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BTR3_DATLAT_0                  ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BTR3_DATLAT_1                  ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BTR3_DATLAT_2                  ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BTR3_DATLAT_3                  ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BTR3_ACCMOD                    ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BTR3_ACCMOD_0                  ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BTR3_ACCMOD_1                  ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BTR4 register  *******************/
#define  FMC_BTR4_ADDSET                    ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BTR4_ADDSET_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BTR4_ADDSET_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BTR4_ADDSET_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BTR4_ADDSET_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BTR4_ADDHLD                    ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BTR4_ADDHLD_0                  ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BTR4_ADDHLD_1                  ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BTR4_ADDHLD_2                  ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BTR4_ADDHLD_3                  ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BTR4_DATAST                    ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BTR4_DATAST_0                  ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BTR4_DATAST_1                  ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BTR4_DATAST_2                  ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BTR4_DATAST_3                  ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BTR4_DATAST_4                  ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BTR4_DATAST_5                  ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BTR4_DATAST_6                  ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BTR4_DATAST_7                  ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BTR4_BUSTURN                   ((uint32_t)0x000F0000)        /*!<BUSTURN[3:0] bits (Bus turnaround phase duration) */
#define  FMC_BTR4_BUSTURN_0                 ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_BTR4_BUSTURN_1                 ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_BTR4_BUSTURN_2                 ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_BTR4_BUSTURN_3                 ((uint32_t)0x00080000)        /*!<Bit 3 */

#define  FMC_BTR4_CLKDIV                    ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BTR4_CLKDIV_0                  ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BTR4_CLKDIV_1                  ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BTR4_CLKDIV_2                  ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BTR4_CLKDIV_3                  ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BTR4_DATLAT                    ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BTR4_DATLAT_0                  ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BTR4_DATLAT_1                  ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BTR4_DATLAT_2                  ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BTR4_DATLAT_3                  ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BTR4_ACCMOD                    ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BTR4_ACCMOD_0                  ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BTR4_ACCMOD_1                  ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BWTR1 register  ******************/
#define  FMC_BWTR1_ADDSET                   ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BWTR1_ADDSET_0                 ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BWTR1_ADDSET_1                 ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BWTR1_ADDSET_2                 ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BWTR1_ADDSET_3                 ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BWTR1_ADDHLD                   ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BWTR1_ADDHLD_0                 ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BWTR1_ADDHLD_1                 ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BWTR1_ADDHLD_2                 ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BWTR1_ADDHLD_3                 ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BWTR1_DATAST                   ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BWTR1_DATAST_0                 ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BWTR1_DATAST_1                 ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BWTR1_DATAST_2                 ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BWTR1_DATAST_3                 ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BWTR1_DATAST_4                 ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BWTR1_DATAST_5                 ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BWTR1_DATAST_6                 ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BWTR1_DATAST_7                 ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BWTR1_CLKDIV                   ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BWTR1_CLKDIV_0                 ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BWTR1_CLKDIV_1                 ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BWTR1_CLKDIV_2                 ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BWTR1_CLKDIV_3                 ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BWTR1_DATLAT                   ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BWTR1_DATLAT_0                 ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BWTR1_DATLAT_1                 ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BWTR1_DATLAT_2                 ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BWTR1_DATLAT_3                 ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BWTR1_ACCMOD                   ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BWTR1_ACCMOD_0                 ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BWTR1_ACCMOD_1                 ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BWTR2 register  ******************/
#define  FMC_BWTR2_ADDSET                   ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BWTR2_ADDSET_0                 ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BWTR2_ADDSET_1                 ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BWTR2_ADDSET_2                 ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BWTR2_ADDSET_3                 ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BWTR2_ADDHLD                   ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BWTR2_ADDHLD_0                 ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BWTR2_ADDHLD_1                 ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BWTR2_ADDHLD_2                 ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BWTR2_ADDHLD_3                 ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BWTR2_DATAST                   ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BWTR2_DATAST_0                 ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BWTR2_DATAST_1                 ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BWTR2_DATAST_2                 ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BWTR2_DATAST_3                 ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BWTR2_DATAST_4                 ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BWTR2_DATAST_5                 ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BWTR2_DATAST_6                 ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BWTR2_DATAST_7                 ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BWTR2_CLKDIV                   ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BWTR2_CLKDIV_0                 ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BWTR2_CLKDIV_1                 ((uint32_t)0x00200000)        /*!<Bit 1*/
#define  FMC_BWTR2_CLKDIV_2                 ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BWTR2_CLKDIV_3                 ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BWTR2_DATLAT                   ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BWTR2_DATLAT_0                 ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BWTR2_DATLAT_1                 ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BWTR2_DATLAT_2                 ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BWTR2_DATLAT_3                 ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BWTR2_ACCMOD                   ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BWTR2_ACCMOD_0                 ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BWTR2_ACCMOD_1                 ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BWTR3 register  ******************/
#define  FMC_BWTR3_ADDSET                   ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BWTR3_ADDSET_0                 ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BWTR3_ADDSET_1                 ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BWTR3_ADDSET_2                 ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BWTR3_ADDSET_3                 ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BWTR3_ADDHLD                   ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BWTR3_ADDHLD_0                 ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BWTR3_ADDHLD_1                 ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BWTR3_ADDHLD_2                 ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BWTR3_ADDHLD_3                 ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BWTR3_DATAST                   ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BWTR3_DATAST_0                 ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BWTR3_DATAST_1                 ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BWTR3_DATAST_2                 ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BWTR3_DATAST_3                 ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BWTR3_DATAST_4                 ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BWTR3_DATAST_5                 ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BWTR3_DATAST_6                 ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BWTR3_DATAST_7                 ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BWTR3_CLKDIV                   ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BWTR3_CLKDIV_0                 ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BWTR3_CLKDIV_1                 ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BWTR3_CLKDIV_2                 ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BWTR3_CLKDIV_3                 ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BWTR3_DATLAT                   ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BWTR3_DATLAT_0                 ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BWTR3_DATLAT_1                 ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BWTR3_DATLAT_2                 ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BWTR3_DATLAT_3                 ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BWTR3_ACCMOD                   ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BWTR3_ACCMOD_0                 ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BWTR3_ACCMOD_1                 ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_BWTR4 register  ******************/
#define  FMC_BWTR4_ADDSET                   ((uint32_t)0x0000000F)        /*!<ADDSET[3:0] bits (Address setup phase duration) */
#define  FMC_BWTR4_ADDSET_0                 ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_BWTR4_ADDSET_1                 ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_BWTR4_ADDSET_2                 ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_BWTR4_ADDSET_3                 ((uint32_t)0x00000008)        /*!<Bit 3 */

#define  FMC_BWTR4_ADDHLD                   ((uint32_t)0x000000F0)        /*!<ADDHLD[3:0] bits (Address-hold phase duration) */
#define  FMC_BWTR4_ADDHLD_0                 ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_BWTR4_ADDHLD_1                 ((uint32_t)0x00000020)        /*!<Bit 1 */
#define  FMC_BWTR4_ADDHLD_2                 ((uint32_t)0x00000040)        /*!<Bit 2 */
#define  FMC_BWTR4_ADDHLD_3                 ((uint32_t)0x00000080)        /*!<Bit 3 */

#define  FMC_BWTR4_DATAST                   ((uint32_t)0x0000FF00)        /*!<DATAST [3:0] bits (Data-phase duration) */
#define  FMC_BWTR4_DATAST_0                 ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_BWTR4_DATAST_1                 ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_BWTR4_DATAST_2                 ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_BWTR4_DATAST_3                 ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_BWTR4_DATAST_4                 ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_BWTR4_DATAST_5                 ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_BWTR4_DATAST_6                 ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_BWTR4_DATAST_7                 ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_BWTR4_CLKDIV                   ((uint32_t)0x00F00000)        /*!<CLKDIV[3:0] bits (Clock divide ratio) */
#define  FMC_BWTR4_CLKDIV_0                 ((uint32_t)0x00100000)        /*!<Bit 0 */
#define  FMC_BWTR4_CLKDIV_1                 ((uint32_t)0x00200000)        /*!<Bit 1 */
#define  FMC_BWTR4_CLKDIV_2                 ((uint32_t)0x00400000)        /*!<Bit 2 */
#define  FMC_BWTR4_CLKDIV_3                 ((uint32_t)0x00800000)        /*!<Bit 3 */

#define  FMC_BWTR4_DATLAT                   ((uint32_t)0x0F000000)        /*!<DATLA[3:0] bits (Data latency) */
#define  FMC_BWTR4_DATLAT_0                 ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_BWTR4_DATLAT_1                 ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_BWTR4_DATLAT_2                 ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_BWTR4_DATLAT_3                 ((uint32_t)0x08000000)        /*!<Bit 3 */

#define  FMC_BWTR4_ACCMOD                   ((uint32_t)0x30000000)        /*!<ACCMOD[1:0] bits (Access mode) */
#define  FMC_BWTR4_ACCMOD_0                 ((uint32_t)0x10000000)        /*!<Bit 0 */
#define  FMC_BWTR4_ACCMOD_1                 ((uint32_t)0x20000000)        /*!<Bit 1 */

/******************  Bit definition for FMC_PCR2 register  *******************/
#define  FMC_PCR2_PWAITEN                   ((uint32_t)0x00000002)        /*!<Wait feature enable bit                   */
#define  FMC_PCR2_PBKEN                     ((uint32_t)0x00000004)        /*!<PC Card/NAND Flash memory bank enable bit */
#define  FMC_PCR2_PTYP                      ((uint32_t)0x00000008)        /*!<Memory type                               */

#define  FMC_PCR2_PWID                      ((uint32_t)0x00000030)        /*!<PWID[1:0] bits (NAND Flash databus width) */
#define  FMC_PCR2_PWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_PCR2_PWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_PCR2_ECCEN                     ((uint32_t)0x00000040)        /*!<ECC computation logic enable bit          */

#define  FMC_PCR2_TCLR                      ((uint32_t)0x00001E00)        /*!<TCLR[3:0] bits (CLE to RE delay)          */
#define  FMC_PCR2_TCLR_0                    ((uint32_t)0x00000200)        /*!<Bit 0 */
#define  FMC_PCR2_TCLR_1                    ((uint32_t)0x00000400)        /*!<Bit 1 */
#define  FMC_PCR2_TCLR_2                    ((uint32_t)0x00000800)        /*!<Bit 2 */
#define  FMC_PCR2_TCLR_3                    ((uint32_t)0x00001000)        /*!<Bit 3 */

#define  FMC_PCR2_TAR                       ((uint32_t)0x0001E000)        /*!<TAR[3:0] bits (ALE to RE delay)           */
#define  FMC_PCR2_TAR_0                     ((uint32_t)0x00002000)        /*!<Bit 0 */
#define  FMC_PCR2_TAR_1                     ((uint32_t)0x00004000)        /*!<Bit 1 */
#define  FMC_PCR2_TAR_2                     ((uint32_t)0x00008000)        /*!<Bit 2 */
#define  FMC_PCR2_TAR_3                     ((uint32_t)0x00010000)        /*!<Bit 3 */

#define  FMC_PCR2_ECCPS                     ((uint32_t)0x000E0000)        /*!<ECCPS[1:0] bits (ECC page size)           */
#define  FMC_PCR2_ECCPS_0                   ((uint32_t)0x00020000)        /*!<Bit 0 */
#define  FMC_PCR2_ECCPS_1                   ((uint32_t)0x00040000)        /*!<Bit 1 */
#define  FMC_PCR2_ECCPS_2                   ((uint32_t)0x00080000)        /*!<Bit 2 */

/******************  Bit definition for FMC_PCR3 register  *******************/
#define  FMC_PCR3_PWAITEN                   ((uint32_t)0x00000002)        /*!<Wait feature enable bit                   */
#define  FMC_PCR3_PBKEN                     ((uint32_t)0x00000004)        /*!<PC Card/NAND Flash memory bank enable bit */
#define  FMC_PCR3_PTYP                      ((uint32_t)0x00000008)        /*!<Memory type                               */

#define  FMC_PCR3_PWID                      ((uint32_t)0x00000030)        /*!<PWID[1:0] bits (NAND Flash databus width) */
#define  FMC_PCR3_PWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_PCR3_PWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_PCR3_ECCEN                     ((uint32_t)0x00000040)        /*!<ECC computation logic enable bit          */

#define  FMC_PCR3_TCLR                      ((uint32_t)0x00001E00)        /*!<TCLR[3:0] bits (CLE to RE delay)          */
#define  FMC_PCR3_TCLR_0                    ((uint32_t)0x00000200)        /*!<Bit 0 */
#define  FMC_PCR3_TCLR_1                    ((uint32_t)0x00000400)        /*!<Bit 1 */
#define  FMC_PCR3_TCLR_2                    ((uint32_t)0x00000800)        /*!<Bit 2 */
#define  FMC_PCR3_TCLR_3                    ((uint32_t)0x00001000)        /*!<Bit 3 */

#define  FMC_PCR3_TAR                       ((uint32_t)0x0001E000)        /*!<TAR[3:0] bits (ALE to RE delay)           */
#define  FMC_PCR3_TAR_0                     ((uint32_t)0x00002000)        /*!<Bit 0 */
#define  FMC_PCR3_TAR_1                     ((uint32_t)0x00004000)        /*!<Bit 1 */
#define  FMC_PCR3_TAR_2                     ((uint32_t)0x00008000)        /*!<Bit 2 */
#define  FMC_PCR3_TAR_3                     ((uint32_t)0x00010000)        /*!<Bit 3 */

#define  FMC_PCR3_ECCPS                     ((uint32_t)0x000E0000)        /*!<ECCPS[2:0] bits (ECC page size)           */
#define  FMC_PCR3_ECCPS_0                   ((uint32_t)0x00020000)        /*!<Bit 0 */
#define  FMC_PCR3_ECCPS_1                   ((uint32_t)0x00040000)        /*!<Bit 1 */
#define  FMC_PCR3_ECCPS_2                   ((uint32_t)0x00080000)        /*!<Bit 2 */

/******************  Bit definition for FMC_PCR4 register  *******************/
#define  FMC_PCR4_PWAITEN                   ((uint32_t)0x00000002)        /*!<Wait feature enable bit                   */
#define  FMC_PCR4_PBKEN                     ((uint32_t)0x00000004)        /*!<PC Card/NAND Flash memory bank enable bit */
#define  FMC_PCR4_PTYP                      ((uint32_t)0x00000008)        /*!<Memory type                               */

#define  FMC_PCR4_PWID                      ((uint32_t)0x00000030)        /*!<PWID[1:0] bits (NAND Flash databus width) */
#define  FMC_PCR4_PWID_0                    ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  FMC_PCR4_PWID_1                    ((uint32_t)0x00000020)        /*!<Bit 1 */

#define  FMC_PCR4_ECCEN                     ((uint32_t)0x00000040)        /*!<ECC computation logic enable bit          */

#define  FMC_PCR4_TCLR                      ((uint32_t)0x00001E00)        /*!<TCLR[3:0] bits (CLE to RE delay)          */
#define  FMC_PCR4_TCLR_0                    ((uint32_t)0x00000200)        /*!<Bit 0 */
#define  FMC_PCR4_TCLR_1                    ((uint32_t)0x00000400)        /*!<Bit 1 */
#define  FMC_PCR4_TCLR_2                    ((uint32_t)0x00000800)        /*!<Bit 2 */
#define  FMC_PCR4_TCLR_3                    ((uint32_t)0x00001000)        /*!<Bit 3 */

#define  FMC_PCR4_TAR                       ((uint32_t)0x0001E000)        /*!<TAR[3:0] bits (ALE to RE delay)           */
#define  FMC_PCR4_TAR_0                     ((uint32_t)0x00002000)        /*!<Bit 0 */
#define  FMC_PCR4_TAR_1                     ((uint32_t)0x00004000)        /*!<Bit 1 */
#define  FMC_PCR4_TAR_2                     ((uint32_t)0x00008000)        /*!<Bit 2 */
#define  FMC_PCR4_TAR_3                     ((uint32_t)0x00010000)        /*!<Bit 3 */

#define  FMC_PCR4_ECCPS                     ((uint32_t)0x000E0000)        /*!<ECCPS[2:0] bits (ECC page size)           */
#define  FMC_PCR4_ECCPS_0                   ((uint32_t)0x00020000)        /*!<Bit 0 */
#define  FMC_PCR4_ECCPS_1                   ((uint32_t)0x00040000)        /*!<Bit 1 */
#define  FMC_PCR4_ECCPS_2                   ((uint32_t)0x00080000)        /*!<Bit 2 */

/*******************  Bit definition for FMC_SR2 register  *******************/
#define  FMC_SR2_IRS                        ((uint8_t)0x01)               /*!<Interrupt Rising Edge status                */
#define  FMC_SR2_ILS                        ((uint8_t)0x02)               /*!<Interrupt Level status                      */
#define  FMC_SR2_IFS                        ((uint8_t)0x04)               /*!<Interrupt Falling Edge status               */
#define  FMC_SR2_IREN                       ((uint8_t)0x08)               /*!<Interrupt Rising Edge detection Enable bit  */
#define  FMC_SR2_ILEN                       ((uint8_t)0x10)               /*!<Interrupt Level detection Enable bit        */
#define  FMC_SR2_IFEN                       ((uint8_t)0x20)               /*!<Interrupt Falling Edge detection Enable bit */
#define  FMC_SR2_FEMPT                      ((uint8_t)0x40)               /*!<FIFO empty                                  */

/*******************  Bit definition for FMC_SR3 register  *******************/
#define  FMC_SR3_IRS                        ((uint8_t)0x01)               /*!<Interrupt Rising Edge status                */
#define  FMC_SR3_ILS                        ((uint8_t)0x02)               /*!<Interrupt Level status                      */
#define  FMC_SR3_IFS                        ((uint8_t)0x04)               /*!<Interrupt Falling Edge status               */
#define  FMC_SR3_IREN                       ((uint8_t)0x08)               /*!<Interrupt Rising Edge detection Enable bit  */
#define  FMC_SR3_ILEN                       ((uint8_t)0x10)               /*!<Interrupt Level detection Enable bit        */
#define  FMC_SR3_IFEN                       ((uint8_t)0x20)               /*!<Interrupt Falling Edge detection Enable bit */
#define  FMC_SR3_FEMPT                      ((uint8_t)0x40)               /*!<FIFO empty                                  */

/*******************  Bit definition for FMC_SR4 register  *******************/
#define  FMC_SR4_IRS                        ((uint8_t)0x01)               /*!<Interrupt Rising Edge status                */
#define  FMC_SR4_ILS                        ((uint8_t)0x02)               /*!<Interrupt Level status                      */
#define  FMC_SR4_IFS                        ((uint8_t)0x04)               /*!<Interrupt Falling Edge status               */
#define  FMC_SR4_IREN                       ((uint8_t)0x08)               /*!<Interrupt Rising Edge detection Enable bit  */
#define  FMC_SR4_ILEN                       ((uint8_t)0x10)               /*!<Interrupt Level detection Enable bit        */
#define  FMC_SR4_IFEN                       ((uint8_t)0x20)               /*!<Interrupt Falling Edge detection Enable bit */
#define  FMC_SR4_FEMPT                      ((uint8_t)0x40)               /*!<FIFO empty                                  */

/******************  Bit definition for FMC_PMEM2 register  ******************/
#define  FMC_PMEM2_MEMSET2                  ((uint32_t)0x000000FF)        /*!<MEMSET2[7:0] bits (Common memory 2 setup time) */
#define  FMC_PMEM2_MEMSET2_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PMEM2_MEMSET2_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PMEM2_MEMSET2_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PMEM2_MEMSET2_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PMEM2_MEMSET2_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PMEM2_MEMSET2_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PMEM2_MEMSET2_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PMEM2_MEMSET2_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PMEM2_MEMWAIT2                 ((uint32_t)0x0000FF00)        /*!<MEMWAIT2[7:0] bits (Common memory 2 wait time) */
#define  FMC_PMEM2_MEMWAIT2_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PMEM2_MEMWAIT2_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PMEM2_MEMWAIT2_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PMEM2_MEMWAIT2_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PMEM2_MEMWAIT2_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PMEM2_MEMWAIT2_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PMEM2_MEMWAIT2_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PMEM2_MEMWAIT2_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PMEM2_MEMHOLD2                 ((uint32_t)0x00FF0000)        /*!<MEMHOLD2[7:0] bits (Common memory 2 hold time) */
#define  FMC_PMEM2_MEMHOLD2_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PMEM2_MEMHOLD2_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PMEM2_MEMHOLD2_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PMEM2_MEMHOLD2_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PMEM2_MEMHOLD2_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PMEM2_MEMHOLD2_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PMEM2_MEMHOLD2_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PMEM2_MEMHOLD2_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PMEM2_MEMHIZ2                  ((uint32_t)0xFF000000)        /*!<MEMHIZ2[7:0] bits (Common memory 2 databus HiZ time) */
#define  FMC_PMEM2_MEMHIZ2_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PMEM2_MEMHIZ2_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PMEM2_MEMHIZ2_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PMEM2_MEMHIZ2_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PMEM2_MEMHIZ2_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PMEM2_MEMHIZ2_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PMEM2_MEMHIZ2_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PMEM2_MEMHIZ2_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PMEM3 register  ******************/
#define  FMC_PMEM3_MEMSET3                  ((uint32_t)0x000000FF)        /*!<MEMSET3[7:0] bits (Common memory 3 setup time) */
#define  FMC_PMEM3_MEMSET3_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PMEM3_MEMSET3_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PMEM3_MEMSET3_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PMEM3_MEMSET3_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PMEM3_MEMSET3_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PMEM3_MEMSET3_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PMEM3_MEMSET3_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PMEM3_MEMSET3_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PMEM3_MEMWAIT3                 ((uint32_t)0x0000FF00)        /*!<MEMWAIT3[7:0] bits (Common memory 3 wait time) */
#define  FMC_PMEM3_MEMWAIT3_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PMEM3_MEMWAIT3_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PMEM3_MEMWAIT3_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PMEM3_MEMWAIT3_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PMEM3_MEMWAIT3_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PMEM3_MEMWAIT3_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PMEM3_MEMWAIT3_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PMEM3_MEMWAIT3_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PMEM3_MEMHOLD3                 ((uint32_t)0x00FF0000)        /*!<MEMHOLD3[7:0] bits (Common memory 3 hold time) */
#define  FMC_PMEM3_MEMHOLD3_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PMEM3_MEMHOLD3_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PMEM3_MEMHOLD3_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PMEM3_MEMHOLD3_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PMEM3_MEMHOLD3_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PMEM3_MEMHOLD3_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PMEM3_MEMHOLD3_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PMEM3_MEMHOLD3_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PMEM3_MEMHIZ3                  ((uint32_t)0xFF000000)        /*!<MEMHIZ3[7:0] bits (Common memory 3 databus HiZ time) */
#define  FMC_PMEM3_MEMHIZ3_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PMEM3_MEMHIZ3_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PMEM3_MEMHIZ3_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PMEM3_MEMHIZ3_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PMEM3_MEMHIZ3_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PMEM3_MEMHIZ3_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PMEM3_MEMHIZ3_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PMEM3_MEMHIZ3_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PMEM4 register  ******************/
#define  FMC_PMEM4_MEMSET4                  ((uint32_t)0x000000FF)        /*!<MEMSET4[7:0] bits (Common memory 4 setup time) */
#define  FMC_PMEM4_MEMSET4_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PMEM4_MEMSET4_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PMEM4_MEMSET4_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PMEM4_MEMSET4_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PMEM4_MEMSET4_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PMEM4_MEMSET4_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PMEM4_MEMSET4_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PMEM4_MEMSET4_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PMEM4_MEMWAIT4                 ((uint32_t)0x0000FF00)        /*!<MEMWAIT4[7:0] bits (Common memory 4 wait time) */
#define  FMC_PMEM4_MEMWAIT4_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PMEM4_MEMWAIT4_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PMEM4_MEMWAIT4_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PMEM4_MEMWAIT4_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PMEM4_MEMWAIT4_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PMEM4_MEMWAIT4_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PMEM4_MEMWAIT4_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PMEM4_MEMWAIT4_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PMEM4_MEMHOLD4                 ((uint32_t)0x00FF0000)        /*!<MEMHOLD4[7:0] bits (Common memory 4 hold time) */
#define  FMC_PMEM4_MEMHOLD4_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PMEM4_MEMHOLD4_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PMEM4_MEMHOLD4_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PMEM4_MEMHOLD4_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PMEM4_MEMHOLD4_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PMEM4_MEMHOLD4_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PMEM4_MEMHOLD4_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PMEM4_MEMHOLD4_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PMEM4_MEMHIZ4                  ((uint32_t)0xFF000000)        /*!<MEMHIZ4[7:0] bits (Common memory 4 databus HiZ time) */
#define  FMC_PMEM4_MEMHIZ4_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PMEM4_MEMHIZ4_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PMEM4_MEMHIZ4_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PMEM4_MEMHIZ4_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PMEM4_MEMHIZ4_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PMEM4_MEMHIZ4_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PMEM4_MEMHIZ4_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PMEM4_MEMHIZ4_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PATT2 register  ******************/
#define  FMC_PATT2_ATTSET2                  ((uint32_t)0x000000FF)        /*!<ATTSET2[7:0] bits (Attribute memory 2 setup time) */
#define  FMC_PATT2_ATTSET2_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PATT2_ATTSET2_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PATT2_ATTSET2_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PATT2_ATTSET2_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PATT2_ATTSET2_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PATT2_ATTSET2_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PATT2_ATTSET2_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PATT2_ATTSET2_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PATT2_ATTWAIT2                 ((uint32_t)0x0000FF00)        /*!<ATTWAIT2[7:0] bits (Attribute memory 2 wait time) */
#define  FMC_PATT2_ATTWAIT2_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PATT2_ATTWAIT2_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PATT2_ATTWAIT2_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PATT2_ATTWAIT2_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PATT2_ATTWAIT2_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PATT2_ATTWAIT2_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PATT2_ATTWAIT2_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PATT2_ATTWAIT2_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PATT2_ATTHOLD2                 ((uint32_t)0x00FF0000)        /*!<ATTHOLD2[7:0] bits (Attribute memory 2 hold time) */
#define  FMC_PATT2_ATTHOLD2_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PATT2_ATTHOLD2_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PATT2_ATTHOLD2_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PATT2_ATTHOLD2_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PATT2_ATTHOLD2_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PATT2_ATTHOLD2_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PATT2_ATTHOLD2_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PATT2_ATTHOLD2_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PATT2_ATTHIZ2                  ((uint32_t)0xFF000000)        /*!<ATTHIZ2[7:0] bits (Attribute memory 2 databus HiZ time) */
#define  FMC_PATT2_ATTHIZ2_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PATT2_ATTHIZ2_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PATT2_ATTHIZ2_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PATT2_ATTHIZ2_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PATT2_ATTHIZ2_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PATT2_ATTHIZ2_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PATT2_ATTHIZ2_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PATT2_ATTHIZ2_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PATT3 register  ******************/
#define  FMC_PATT3_ATTSET3                  ((uint32_t)0x000000FF)        /*!<ATTSET3[7:0] bits (Attribute memory 3 setup time) */
#define  FMC_PATT3_ATTSET3_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PATT3_ATTSET3_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PATT3_ATTSET3_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PATT3_ATTSET3_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PATT3_ATTSET3_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PATT3_ATTSET3_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PATT3_ATTSET3_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PATT3_ATTSET3_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PATT3_ATTWAIT3                 ((uint32_t)0x0000FF00)        /*!<ATTWAIT3[7:0] bits (Attribute memory 3 wait time) */
#define  FMC_PATT3_ATTWAIT3_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PATT3_ATTWAIT3_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PATT3_ATTWAIT3_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PATT3_ATTWAIT3_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PATT3_ATTWAIT3_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PATT3_ATTWAIT3_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PATT3_ATTWAIT3_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PATT3_ATTWAIT3_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PATT3_ATTHOLD3                 ((uint32_t)0x00FF0000)        /*!<ATTHOLD3[7:0] bits (Attribute memory 3 hold time) */
#define  FMC_PATT3_ATTHOLD3_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PATT3_ATTHOLD3_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PATT3_ATTHOLD3_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PATT3_ATTHOLD3_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PATT3_ATTHOLD3_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PATT3_ATTHOLD3_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PATT3_ATTHOLD3_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PATT3_ATTHOLD3_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PATT3_ATTHIZ3                  ((uint32_t)0xFF000000)        /*!<ATTHIZ3[7:0] bits (Attribute memory 3 databus HiZ time) */
#define  FMC_PATT3_ATTHIZ3_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PATT3_ATTHIZ3_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PATT3_ATTHIZ3_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PATT3_ATTHIZ3_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PATT3_ATTHIZ3_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PATT3_ATTHIZ3_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PATT3_ATTHIZ3_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PATT3_ATTHIZ3_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PATT4 register  ******************/
#define  FMC_PATT4_ATTSET4                  ((uint32_t)0x000000FF)        /*!<ATTSET4[7:0] bits (Attribute memory 4 setup time) */
#define  FMC_PATT4_ATTSET4_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PATT4_ATTSET4_1                ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PATT4_ATTSET4_2                ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PATT4_ATTSET4_3                ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PATT4_ATTSET4_4                ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PATT4_ATTSET4_5                ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PATT4_ATTSET4_6                ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PATT4_ATTSET4_7                ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PATT4_ATTWAIT4                 ((uint32_t)0x0000FF00)        /*!<ATTWAIT4[7:0] bits (Attribute memory 4 wait time) */
#define  FMC_PATT4_ATTWAIT4_0               ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PATT4_ATTWAIT4_1               ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PATT4_ATTWAIT4_2               ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PATT4_ATTWAIT4_3               ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PATT4_ATTWAIT4_4               ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PATT4_ATTWAIT4_5               ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PATT4_ATTWAIT4_6               ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PATT4_ATTWAIT4_7               ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PATT4_ATTHOLD4                 ((uint32_t)0x00FF0000)        /*!<ATTHOLD4[7:0] bits (Attribute memory 4 hold time) */
#define  FMC_PATT4_ATTHOLD4_0               ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PATT4_ATTHOLD4_1               ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PATT4_ATTHOLD4_2               ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PATT4_ATTHOLD4_3               ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PATT4_ATTHOLD4_4               ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PATT4_ATTHOLD4_5               ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PATT4_ATTHOLD4_6               ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PATT4_ATTHOLD4_7               ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PATT4_ATTHIZ4                  ((uint32_t)0xFF000000)        /*!<ATTHIZ4[7:0] bits (Attribute memory 4 databus HiZ time) */
#define  FMC_PATT4_ATTHIZ4_0                ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PATT4_ATTHIZ4_1                ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PATT4_ATTHIZ4_2                ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PATT4_ATTHIZ4_3                ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PATT4_ATTHIZ4_4                ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PATT4_ATTHIZ4_5                ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PATT4_ATTHIZ4_6                ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PATT4_ATTHIZ4_7                ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_PIO4 register  *******************/
#define  FMC_PIO4_IOSET4                    ((uint32_t)0x000000FF)        /*!<IOSET4[7:0] bits (I/O 4 setup time) */
#define  FMC_PIO4_IOSET4_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  FMC_PIO4_IOSET4_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  FMC_PIO4_IOSET4_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  FMC_PIO4_IOSET4_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  FMC_PIO4_IOSET4_4                  ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  FMC_PIO4_IOSET4_5                  ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  FMC_PIO4_IOSET4_6                  ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  FMC_PIO4_IOSET4_7                  ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  FMC_PIO4_IOWAIT4                   ((uint32_t)0x0000FF00)        /*!<IOWAIT4[7:0] bits (I/O 4 wait time) */
#define  FMC_PIO4_IOWAIT4_0                 ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  FMC_PIO4_IOWAIT4_1                 ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  FMC_PIO4_IOWAIT4_2                 ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  FMC_PIO4_IOWAIT4_3                 ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  FMC_PIO4_IOWAIT4_4                 ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  FMC_PIO4_IOWAIT4_5                 ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  FMC_PIO4_IOWAIT4_6                 ((uint32_t)0x00004000)        /*!<Bit 6 */
#define  FMC_PIO4_IOWAIT4_7                 ((uint32_t)0x00008000)        /*!<Bit 7 */

#define  FMC_PIO4_IOHOLD4                   ((uint32_t)0x00FF0000)        /*!<IOHOLD4[7:0] bits (I/O 4 hold time) */
#define  FMC_PIO4_IOHOLD4_0                 ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  FMC_PIO4_IOHOLD4_1                 ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  FMC_PIO4_IOHOLD4_2                 ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  FMC_PIO4_IOHOLD4_3                 ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  FMC_PIO4_IOHOLD4_4                 ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  FMC_PIO4_IOHOLD4_5                 ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  FMC_PIO4_IOHOLD4_6                 ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  FMC_PIO4_IOHOLD4_7                 ((uint32_t)0x00800000)        /*!<Bit 7 */

#define  FMC_PIO4_IOHIZ4                    ((uint32_t)0xFF000000)        /*!<IOHIZ4[7:0] bits (I/O 4 databus HiZ time) */
#define  FMC_PIO4_IOHIZ4_0                  ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  FMC_PIO4_IOHIZ4_1                  ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  FMC_PIO4_IOHIZ4_2                  ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  FMC_PIO4_IOHIZ4_3                  ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  FMC_PIO4_IOHIZ4_4                  ((uint32_t)0x10000000)        /*!<Bit 4 */
#define  FMC_PIO4_IOHIZ4_5                  ((uint32_t)0x20000000)        /*!<Bit 5 */
#define  FMC_PIO4_IOHIZ4_6                  ((uint32_t)0x40000000)        /*!<Bit 6 */
#define  FMC_PIO4_IOHIZ4_7                  ((uint32_t)0x80000000)        /*!<Bit 7 */

/******************  Bit definition for FMC_ECCR2 register  ******************/
#define  FMC_ECCR2_ECC2                     ((uint32_t)0xFFFFFFFF)        /*!<ECC result */

/******************  Bit definition for FMC_ECCR3 register  ******************/
#define  FMC_ECCR3_ECC3                     ((uint32_t)0xFFFFFFFF)        /*!<ECC result */


/******************************************************************************/
/*                                                                            */
/*                            General Purpose I/O                             */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0                    ((uint32_t)0x00000003)
#define GPIO_MODER_MODER0_0                  ((uint32_t)0x00000001)
#define GPIO_MODER_MODER0_1                  ((uint32_t)0x00000002)

#define GPIO_MODER_MODER1                    ((uint32_t)0x0000000C)
#define GPIO_MODER_MODER1_0                  ((uint32_t)0x00000004)
#define GPIO_MODER_MODER1_1                  ((uint32_t)0x00000008)

#define GPIO_MODER_MODER2                    ((uint32_t)0x00000030)
#define GPIO_MODER_MODER2_0                  ((uint32_t)0x00000010)
#define GPIO_MODER_MODER2_1                  ((uint32_t)0x00000020)

#define GPIO_MODER_MODER3                    ((uint32_t)0x000000C0)
#define GPIO_MODER_MODER3_0                  ((uint32_t)0x00000040)
#define GPIO_MODER_MODER3_1                  ((uint32_t)0x00000080)

#define GPIO_MODER_MODER4                    ((uint32_t)0x00000300)
#define GPIO_MODER_MODER4_0                  ((uint32_t)0x00000100)
#define GPIO_MODER_MODER4_1                  ((uint32_t)0x00000200)

#define GPIO_MODER_MODER5                    ((uint32_t)0x00000C00)
#define GPIO_MODER_MODER5_0                  ((uint32_t)0x00000400)
#define GPIO_MODER_MODER5_1                  ((uint32_t)0x00000800)

#define GPIO_MODER_MODER6                    ((uint32_t)0x00003000)
#define GPIO_MODER_MODER6_0                  ((uint32_t)0x00001000)
#define GPIO_MODER_MODER6_1                  ((uint32_t)0x00002000)

#define GPIO_MODER_MODER7                    ((uint32_t)0x0000C000)
#define GPIO_MODER_MODER7_0                  ((uint32_t)0x00004000)
#define GPIO_MODER_MODER7_1                  ((uint32_t)0x00008000)

#define GPIO_MODER_MODER8                    ((uint32_t)0x00030000)
#define GPIO_MODER_MODER8_0                  ((uint32_t)0x00010000)
#define GPIO_MODER_MODER8_1                  ((uint32_t)0x00020000)

#define GPIO_MODER_MODER9                    ((uint32_t)0x000C0000)
#define GPIO_MODER_MODER9_0                  ((uint32_t)0x00040000)
#define GPIO_MODER_MODER9_1                  ((uint32_t)0x00080000)

#define GPIO_MODER_MODER10                   ((uint32_t)0x00300000)
#define GPIO_MODER_MODER10_0                 ((uint32_t)0x00100000)
#define GPIO_MODER_MODER10_1                 ((uint32_t)0x00200000)

#define GPIO_MODER_MODER11                   ((uint32_t)0x00C00000)
#define GPIO_MODER_MODER11_0                 ((uint32_t)0x00400000)
#define GPIO_MODER_MODER11_1                 ((uint32_t)0x00800000)

#define GPIO_MODER_MODER12                   ((uint32_t)0x03000000)
#define GPIO_MODER_MODER12_0                 ((uint32_t)0x01000000)
#define GPIO_MODER_MODER12_1                 ((uint32_t)0x02000000)

#define GPIO_MODER_MODER13                   ((uint32_t)0x0C000000)
#define GPIO_MODER_MODER13_0                 ((uint32_t)0x04000000)
#define GPIO_MODER_MODER13_1                 ((uint32_t)0x08000000)

#define GPIO_MODER_MODER14                   ((uint32_t)0x30000000)
#define GPIO_MODER_MODER14_0                 ((uint32_t)0x10000000)
#define GPIO_MODER_MODER14_1                 ((uint32_t)0x20000000)

#define GPIO_MODER_MODER15                   ((uint32_t)0xC0000000)
#define GPIO_MODER_MODER15_0                 ((uint32_t)0x40000000)
#define GPIO_MODER_MODER15_1                 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_OTYPER register  ****************/
#define GPIO_OTYPER_OT_0                     ((uint32_t)0x00000001)
#define GPIO_OTYPER_OT_1                     ((uint32_t)0x00000002)
#define GPIO_OTYPER_OT_2                     ((uint32_t)0x00000004)
#define GPIO_OTYPER_OT_3                     ((uint32_t)0x00000008)
#define GPIO_OTYPER_OT_4                     ((uint32_t)0x00000010)
#define GPIO_OTYPER_OT_5                     ((uint32_t)0x00000020)
#define GPIO_OTYPER_OT_6                     ((uint32_t)0x00000040)
#define GPIO_OTYPER_OT_7                     ((uint32_t)0x00000080)
#define GPIO_OTYPER_OT_8                     ((uint32_t)0x00000100)
#define GPIO_OTYPER_OT_9                     ((uint32_t)0x00000200)
#define GPIO_OTYPER_OT_10                    ((uint32_t)0x00000400)
#define GPIO_OTYPER_OT_11                    ((uint32_t)0x00000800)
#define GPIO_OTYPER_OT_12                    ((uint32_t)0x00001000)
#define GPIO_OTYPER_OT_13                    ((uint32_t)0x00002000)
#define GPIO_OTYPER_OT_14                    ((uint32_t)0x00004000)
#define GPIO_OTYPER_OT_15                    ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0               ((uint32_t)0x00000003)
#define GPIO_OSPEEDER_OSPEEDR0_0             ((uint32_t)0x00000001)
#define GPIO_OSPEEDER_OSPEEDR0_1             ((uint32_t)0x00000002)

#define GPIO_OSPEEDER_OSPEEDR1               ((uint32_t)0x0000000C)
#define GPIO_OSPEEDER_OSPEEDR1_0             ((uint32_t)0x00000004)
#define GPIO_OSPEEDER_OSPEEDR1_1             ((uint32_t)0x00000008)

#define GPIO_OSPEEDER_OSPEEDR2               ((uint32_t)0x00000030)
#define GPIO_OSPEEDER_OSPEEDR2_0             ((uint32_t)0x00000010)
#define GPIO_OSPEEDER_OSPEEDR2_1             ((uint32_t)0x00000020)

#define GPIO_OSPEEDER_OSPEEDR3               ((uint32_t)0x000000C0)
#define GPIO_OSPEEDER_OSPEEDR3_0             ((uint32_t)0x00000040)
#define GPIO_OSPEEDER_OSPEEDR3_1             ((uint32_t)0x00000080)

#define GPIO_OSPEEDER_OSPEEDR4               ((uint32_t)0x00000300)
#define GPIO_OSPEEDER_OSPEEDR4_0             ((uint32_t)0x00000100)
#define GPIO_OSPEEDER_OSPEEDR4_1             ((uint32_t)0x00000200)

#define GPIO_OSPEEDER_OSPEEDR5               ((uint32_t)0x00000C00)
#define GPIO_OSPEEDER_OSPEEDR5_0             ((uint32_t)0x00000400)
#define GPIO_OSPEEDER_OSPEEDR5_1             ((uint32_t)0x00000800)

#define GPIO_OSPEEDER_OSPEEDR6               ((uint32_t)0x00003000)
#define GPIO_OSPEEDER_OSPEEDR6_0             ((uint32_t)0x00001000)
#define GPIO_OSPEEDER_OSPEEDR6_1             ((uint32_t)0x00002000)

#define GPIO_OSPEEDER_OSPEEDR7               ((uint32_t)0x0000C000)
#define GPIO_OSPEEDER_OSPEEDR7_0             ((uint32_t)0x00004000)
#define GPIO_OSPEEDER_OSPEEDR7_1             ((uint32_t)0x00008000)

#define GPIO_OSPEEDER_OSPEEDR8               ((uint32_t)0x00030000)
#define GPIO_OSPEEDER_OSPEEDR8_0             ((uint32_t)0x00010000)
#define GPIO_OSPEEDER_OSPEEDR8_1             ((uint32_t)0x00020000)

#define GPIO_OSPEEDER_OSPEEDR9               ((uint32_t)0x000C0000)
#define GPIO_OSPEEDER_OSPEEDR9_0             ((uint32_t)0x00040000)
#define GPIO_OSPEEDER_OSPEEDR9_1             ((uint32_t)0x00080000)

#define GPIO_OSPEEDER_OSPEEDR10              ((uint32_t)0x00300000)
#define GPIO_OSPEEDER_OSPEEDR10_0            ((uint32_t)0x00100000)
#define GPIO_OSPEEDER_OSPEEDR10_1            ((uint32_t)0x00200000)

#define GPIO_OSPEEDER_OSPEEDR11              ((uint32_t)0x00C00000)
#define GPIO_OSPEEDER_OSPEEDR11_0            ((uint32_t)0x00400000)
#define GPIO_OSPEEDER_OSPEEDR11_1            ((uint32_t)0x00800000)

#define GPIO_OSPEEDER_OSPEEDR12              ((uint32_t)0x03000000)
#define GPIO_OSPEEDER_OSPEEDR12_0            ((uint32_t)0x01000000)
#define GPIO_OSPEEDER_OSPEEDR12_1            ((uint32_t)0x02000000)

#define GPIO_OSPEEDER_OSPEEDR13              ((uint32_t)0x0C000000)
#define GPIO_OSPEEDER_OSPEEDR13_0            ((uint32_t)0x04000000)
#define GPIO_OSPEEDER_OSPEEDR13_1            ((uint32_t)0x08000000)

#define GPIO_OSPEEDER_OSPEEDR14              ((uint32_t)0x30000000)
#define GPIO_OSPEEDER_OSPEEDR14_0            ((uint32_t)0x10000000)
#define GPIO_OSPEEDER_OSPEEDR14_1            ((uint32_t)0x20000000)

#define GPIO_OSPEEDER_OSPEEDR15              ((uint32_t)0xC0000000)
#define GPIO_OSPEEDER_OSPEEDR15_0            ((uint32_t)0x40000000)
#define GPIO_OSPEEDER_OSPEEDR15_1            ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_PUPDR0                    ((uint32_t)0x00000003)
#define GPIO_PUPDR_PUPDR0_0                  ((uint32_t)0x00000001)
#define GPIO_PUPDR_PUPDR0_1                  ((uint32_t)0x00000002)

#define GPIO_PUPDR_PUPDR1                    ((uint32_t)0x0000000C)
#define GPIO_PUPDR_PUPDR1_0                  ((uint32_t)0x00000004)
#define GPIO_PUPDR_PUPDR1_1                  ((uint32_t)0x00000008)

#define GPIO_PUPDR_PUPDR2                    ((uint32_t)0x00000030)
#define GPIO_PUPDR_PUPDR2_0                  ((uint32_t)0x00000010)
#define GPIO_PUPDR_PUPDR2_1                  ((uint32_t)0x00000020)

#define GPIO_PUPDR_PUPDR3                    ((uint32_t)0x000000C0)
#define GPIO_PUPDR_PUPDR3_0                  ((uint32_t)0x00000040)
#define GPIO_PUPDR_PUPDR3_1                  ((uint32_t)0x00000080)

#define GPIO_PUPDR_PUPDR4                    ((uint32_t)0x00000300)
#define GPIO_PUPDR_PUPDR4_0                  ((uint32_t)0x00000100)
#define GPIO_PUPDR_PUPDR4_1                  ((uint32_t)0x00000200)

#define GPIO_PUPDR_PUPDR5                    ((uint32_t)0x00000C00)
#define GPIO_PUPDR_PUPDR5_0                  ((uint32_t)0x00000400)
#define GPIO_PUPDR_PUPDR5_1                  ((uint32_t)0x00000800)

#define GPIO_PUPDR_PUPDR6                    ((uint32_t)0x00003000)
#define GPIO_PUPDR_PUPDR6_0                  ((uint32_t)0x00001000)
#define GPIO_PUPDR_PUPDR6_1                  ((uint32_t)0x00002000)

#define GPIO_PUPDR_PUPDR7                    ((uint32_t)0x0000C000)
#define GPIO_PUPDR_PUPDR7_0                  ((uint32_t)0x00004000)
#define GPIO_PUPDR_PUPDR7_1                  ((uint32_t)0x00008000)

#define GPIO_PUPDR_PUPDR8                    ((uint32_t)0x00030000)
#define GPIO_PUPDR_PUPDR8_0                  ((uint32_t)0x00010000)
#define GPIO_PUPDR_PUPDR8_1                  ((uint32_t)0x00020000)

#define GPIO_PUPDR_PUPDR9                    ((uint32_t)0x000C0000)
#define GPIO_PUPDR_PUPDR9_0                  ((uint32_t)0x00040000)
#define GPIO_PUPDR_PUPDR9_1                  ((uint32_t)0x00080000)

#define GPIO_PUPDR_PUPDR10                   ((uint32_t)0x00300000)
#define GPIO_PUPDR_PUPDR10_0                 ((uint32_t)0x00100000)
#define GPIO_PUPDR_PUPDR10_1                 ((uint32_t)0x00200000)

#define GPIO_PUPDR_PUPDR11                   ((uint32_t)0x00C00000)
#define GPIO_PUPDR_PUPDR11_0                 ((uint32_t)0x00400000)
#define GPIO_PUPDR_PUPDR11_1                 ((uint32_t)0x00800000)

#define GPIO_PUPDR_PUPDR12                   ((uint32_t)0x03000000)
#define GPIO_PUPDR_PUPDR12_0                 ((uint32_t)0x01000000)
#define GPIO_PUPDR_PUPDR12_1                 ((uint32_t)0x02000000)

#define GPIO_PUPDR_PUPDR13                   ((uint32_t)0x0C000000)
#define GPIO_PUPDR_PUPDR13_0                 ((uint32_t)0x04000000)
#define GPIO_PUPDR_PUPDR13_1                 ((uint32_t)0x08000000)

#define GPIO_PUPDR_PUPDR14                   ((uint32_t)0x30000000)
#define GPIO_PUPDR_PUPDR14_0                 ((uint32_t)0x10000000)
#define GPIO_PUPDR_PUPDR14_1                 ((uint32_t)0x20000000)

#define GPIO_PUPDR_PUPDR15                   ((uint32_t)0xC0000000)
#define GPIO_PUPDR_PUPDR15_0                 ((uint32_t)0x40000000)
#define GPIO_PUPDR_PUPDR15_1                 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR_0                       ((uint32_t)0x00000001)
#define GPIO_IDR_IDR_1                       ((uint32_t)0x00000002)
#define GPIO_IDR_IDR_2                       ((uint32_t)0x00000004)
#define GPIO_IDR_IDR_3                       ((uint32_t)0x00000008)
#define GPIO_IDR_IDR_4                       ((uint32_t)0x00000010)
#define GPIO_IDR_IDR_5                       ((uint32_t)0x00000020)
#define GPIO_IDR_IDR_6                       ((uint32_t)0x00000040)
#define GPIO_IDR_IDR_7                       ((uint32_t)0x00000080)
#define GPIO_IDR_IDR_8                       ((uint32_t)0x00000100)
#define GPIO_IDR_IDR_9                       ((uint32_t)0x00000200)
#define GPIO_IDR_IDR_10                      ((uint32_t)0x00000400)
#define GPIO_IDR_IDR_11                      ((uint32_t)0x00000800)
#define GPIO_IDR_IDR_12                      ((uint32_t)0x00001000)
#define GPIO_IDR_IDR_13                      ((uint32_t)0x00002000)
#define GPIO_IDR_IDR_14                      ((uint32_t)0x00004000)
#define GPIO_IDR_IDR_15                      ((uint32_t)0x00008000)
/* Old GPIO_IDR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_IDR_0                    GPIO_IDR_IDR_0
#define GPIO_OTYPER_IDR_1                    GPIO_IDR_IDR_1
#define GPIO_OTYPER_IDR_2                    GPIO_IDR_IDR_2
#define GPIO_OTYPER_IDR_3                    GPIO_IDR_IDR_3
#define GPIO_OTYPER_IDR_4                    GPIO_IDR_IDR_4
#define GPIO_OTYPER_IDR_5                    GPIO_IDR_IDR_5
#define GPIO_OTYPER_IDR_6                    GPIO_IDR_IDR_6
#define GPIO_OTYPER_IDR_7                    GPIO_IDR_IDR_7
#define GPIO_OTYPER_IDR_8                    GPIO_IDR_IDR_8
#define GPIO_OTYPER_IDR_9                    GPIO_IDR_IDR_9
#define GPIO_OTYPER_IDR_10                   GPIO_IDR_IDR_10
#define GPIO_OTYPER_IDR_11                   GPIO_IDR_IDR_11
#define GPIO_OTYPER_IDR_12                   GPIO_IDR_IDR_12
#define GPIO_OTYPER_IDR_13                   GPIO_IDR_IDR_13
#define GPIO_OTYPER_IDR_14                   GPIO_IDR_IDR_14
#define GPIO_OTYPER_IDR_15                   GPIO_IDR_IDR_15

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR_0                       ((uint32_t)0x00000001)
#define GPIO_ODR_ODR_1                       ((uint32_t)0x00000002)
#define GPIO_ODR_ODR_2                       ((uint32_t)0x00000004)
#define GPIO_ODR_ODR_3                       ((uint32_t)0x00000008)
#define GPIO_ODR_ODR_4                       ((uint32_t)0x00000010)
#define GPIO_ODR_ODR_5                       ((uint32_t)0x00000020)
#define GPIO_ODR_ODR_6                       ((uint32_t)0x00000040)
#define GPIO_ODR_ODR_7                       ((uint32_t)0x00000080)
#define GPIO_ODR_ODR_8                       ((uint32_t)0x00000100)
#define GPIO_ODR_ODR_9                       ((uint32_t)0x00000200)
#define GPIO_ODR_ODR_10                      ((uint32_t)0x00000400)
#define GPIO_ODR_ODR_11                      ((uint32_t)0x00000800)
#define GPIO_ODR_ODR_12                      ((uint32_t)0x00001000)
#define GPIO_ODR_ODR_13                      ((uint32_t)0x00002000)
#define GPIO_ODR_ODR_14                      ((uint32_t)0x00004000)
#define GPIO_ODR_ODR_15                      ((uint32_t)0x00008000)
/* Old GPIO_ODR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_ODR_0                    GPIO_ODR_ODR_0
#define GPIO_OTYPER_ODR_1                    GPIO_ODR_ODR_1
#define GPIO_OTYPER_ODR_2                    GPIO_ODR_ODR_2
#define GPIO_OTYPER_ODR_3                    GPIO_ODR_ODR_3
#define GPIO_OTYPER_ODR_4                    GPIO_ODR_ODR_4
#define GPIO_OTYPER_ODR_5                    GPIO_ODR_ODR_5
#define GPIO_OTYPER_ODR_6                    GPIO_ODR_ODR_6
#define GPIO_OTYPER_ODR_7                    GPIO_ODR_ODR_7
#define GPIO_OTYPER_ODR_8                    GPIO_ODR_ODR_8
#define GPIO_OTYPER_ODR_9                    GPIO_ODR_ODR_9
#define GPIO_OTYPER_ODR_10                   GPIO_ODR_ODR_10
#define GPIO_OTYPER_ODR_11                   GPIO_ODR_ODR_11
#define GPIO_OTYPER_ODR_12                   GPIO_ODR_ODR_12
#define GPIO_OTYPER_ODR_13                   GPIO_ODR_ODR_13
#define GPIO_OTYPER_ODR_14                   GPIO_ODR_ODR_14
#define GPIO_OTYPER_ODR_15                   GPIO_ODR_ODR_15

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_BS_0                       ((uint32_t)0x00000001)
#define GPIO_BSRR_BS_1                       ((uint32_t)0x00000002)
#define GPIO_BSRR_BS_2                       ((uint32_t)0x00000004)
#define GPIO_BSRR_BS_3                       ((uint32_t)0x00000008)
#define GPIO_BSRR_BS_4                       ((uint32_t)0x00000010)
#define GPIO_BSRR_BS_5                       ((uint32_t)0x00000020)
#define GPIO_BSRR_BS_6                       ((uint32_t)0x00000040)
#define GPIO_BSRR_BS_7                       ((uint32_t)0x00000080)
#define GPIO_BSRR_BS_8                       ((uint32_t)0x00000100)
#define GPIO_BSRR_BS_9                       ((uint32_t)0x00000200)
#define GPIO_BSRR_BS_10                      ((uint32_t)0x00000400)
#define GPIO_BSRR_BS_11                      ((uint32_t)0x00000800)
#define GPIO_BSRR_BS_12                      ((uint32_t)0x00001000)
#define GPIO_BSRR_BS_13                      ((uint32_t)0x00002000)
#define GPIO_BSRR_BS_14                      ((uint32_t)0x00004000)
#define GPIO_BSRR_BS_15                      ((uint32_t)0x00008000)
#define GPIO_BSRR_BR_0                       ((uint32_t)0x00010000)
#define GPIO_BSRR_BR_1                       ((uint32_t)0x00020000)
#define GPIO_BSRR_BR_2                       ((uint32_t)0x00040000)
#define GPIO_BSRR_BR_3                       ((uint32_t)0x00080000)
#define GPIO_BSRR_BR_4                       ((uint32_t)0x00100000)
#define GPIO_BSRR_BR_5                       ((uint32_t)0x00200000)
#define GPIO_BSRR_BR_6                       ((uint32_t)0x00400000)
#define GPIO_BSRR_BR_7                       ((uint32_t)0x00800000)
#define GPIO_BSRR_BR_8                       ((uint32_t)0x01000000)
#define GPIO_BSRR_BR_9                       ((uint32_t)0x02000000)
#define GPIO_BSRR_BR_10                      ((uint32_t)0x04000000)
#define GPIO_BSRR_BR_11                      ((uint32_t)0x08000000)
#define GPIO_BSRR_BR_12                      ((uint32_t)0x10000000)
#define GPIO_BSRR_BR_13                      ((uint32_t)0x20000000)
#define GPIO_BSRR_BR_14                      ((uint32_t)0x40000000)
#define GPIO_BSRR_BR_15                      ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_BRR register  ******************/
#define GPIO_BRR_BR_0                       ((uint32_t)0x00000001)
#define GPIO_BRR_BR_1                       ((uint32_t)0x00000002)
#define GPIO_BRR_BR_2                       ((uint32_t)0x00000004)
#define GPIO_BRR_BR_3                       ((uint32_t)0x00000008)
#define GPIO_BRR_BR_4                       ((uint32_t)0x00000010)
#define GPIO_BRR_BR_5                       ((uint32_t)0x00000020)
#define GPIO_BRR_BR_6                       ((uint32_t)0x00000040)
#define GPIO_BRR_BR_7                       ((uint32_t)0x00000080)
#define GPIO_BRR_BR_8                       ((uint32_t)0x00000100)
#define GPIO_BRR_BR_9                       ((uint32_t)0x00000200)
#define GPIO_BRR_BR_10                      ((uint32_t)0x00000400)
#define GPIO_BRR_BR_11                      ((uint32_t)0x00000800)
#define GPIO_BRR_BR_12                      ((uint32_t)0x00001000)
#define GPIO_BRR_BR_13                      ((uint32_t)0x00002000)
#define GPIO_BRR_BR_14                      ((uint32_t)0x00004000)
#define GPIO_BRR_BR_15                      ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_RLAR register  *******************/
#define GPIO_RLA_EN_0                       ((uint32_t)0x00000001)
#define GPIO_RLA_EN_1                       ((uint32_t)0x00000002)
#define GPIO_RLA_EN_2                       ((uint32_t)0x00000004)
#define GPIO_RLA_EN_3                       ((uint32_t)0x00000008)
#define GPIO_RLA_EN_4                       ((uint32_t)0x00000010)
#define GPIO_RLA_EN_5                       ((uint32_t)0x00000020)
#define GPIO_RLA_EN_6                       ((uint32_t)0x00000040)
#define GPIO_RLA_EN_7                       ((uint32_t)0x00000080)
#define GPIO_RLA_EN_8                       ((uint32_t)0x00000100)
#define GPIO_RLA_EN_9                       ((uint32_t)0x00000200)
#define GPIO_RLA_EN_10                      ((uint32_t)0x00000400)
#define GPIO_RLA_EN_11                      ((uint32_t)0x00000800)
#define GPIO_RLA_EN_12                      ((uint32_t)0x00001000)
#define GPIO_RLA_EN_13                      ((uint32_t)0x00002000)
#define GPIO_RLA_EN_14                      ((uint32_t)0x00004000)
#define GPIO_RLA_EN_15                      ((uint32_t)0x00008000)

/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface (I2C)              */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for I2C_CR1 register  *******************/
#define  I2C_CR1_PE                          ((uint32_t)0x00000001)        /*!< Peripheral enable */
#define  I2C_CR1_TXIE                        ((uint32_t)0x00000002)        /*!< TX interrupt enable */
#define  I2C_CR1_RXIE                        ((uint32_t)0x00000004)        /*!< RX interrupt enable */
#define  I2C_CR1_ADDRIE                      ((uint32_t)0x00000008)        /*!< Address match interrupt enable */
#define  I2C_CR1_NACKIE                      ((uint32_t)0x00000010)        /*!< NACK received interrupt enable */
#define  I2C_CR1_STOPIE                      ((uint32_t)0x00000020)        /*!< STOP detection interrupt enable */
#define  I2C_CR1_TCIE                        ((uint32_t)0x00000040)        /*!< Transfer complete interrupt enable */
#define  I2C_CR1_ERRIE                       ((uint32_t)0x00000080)        /*!< Errors interrupt enable */
#define  I2C_CR1_DFN                         ((uint32_t)0x00000F00)        /*!< Digital noise filter */
#define  I2C_CR1_ANFOFF                      ((uint32_t)0x00001000)        /*!< Analog noise filter OFF */
#define  I2C_CR1_SWRST                       ((uint32_t)0x00002000)        /*!< Software reset */
#define  I2C_CR1_TXDMAEN                     ((uint32_t)0x00004000)        /*!< DMA transmission requests enable */
#define  I2C_CR1_RXDMAEN                     ((uint32_t)0x00008000)        /*!< DMA reception requests enable */
#define  I2C_CR1_SBC                         ((uint32_t)0x00010000)        /*!< Slave byte control */
#define  I2C_CR1_NOSTRETCH                   ((uint32_t)0x00020000)        /*!< Clock stretching disable */
#define  I2C_CR1_WUPEN                       ((uint32_t)0x00040000)        /*!< Wakeup from STOP enable */
#define  I2C_CR1_GCEN                        ((uint32_t)0x00080000)        /*!< General call enable */
#define  I2C_CR1_SMBHEN                      ((uint32_t)0x00100000)        /*!< SMBus host address enable */
#define  I2C_CR1_SMBDEN                      ((uint32_t)0x00200000)        /*!< SMBus device default address enable */
#define  I2C_CR1_ALERTEN                     ((uint32_t)0x00400000)        /*!< SMBus alert enable */
#define  I2C_CR1_PECEN                       ((uint32_t)0x00800000)        /*!< PEC enable */

/******************  Bit definition for I2C_CR2 register  ********************/
#define  I2C_CR2_SADD                        ((uint32_t)0x000003FF)        /*!< Slave address (master mode) */
#define  I2C_CR2_RD_WRN                      ((uint32_t)0x00000400)        /*!< Transfer direction (master mode) */
#define  I2C_CR2_ADD10                       ((uint32_t)0x00000800)        /*!< 10-bit addressing mode (master mode) */
#define  I2C_CR2_HEAD10R                     ((uint32_t)0x00001000)        /*!< 10-bit address header only read direction (master mode) */
#define  I2C_CR2_START                       ((uint32_t)0x00002000)        /*!< START generation */
#define  I2C_CR2_STOP                        ((uint32_t)0x00004000)        /*!< STOP generation (master mode) */
#define  I2C_CR2_NACK                        ((uint32_t)0x00008000)        /*!< NACK generation (slave mode) */
#define  I2C_CR2_NBYTES                      ((uint32_t)0x00FF0000)        /*!< Number of bytes */
#define  I2C_CR2_RELOAD                      ((uint32_t)0x01000000)        /*!< NBYTES reload mode */
#define  I2C_CR2_AUTOEND                     ((uint32_t)0x02000000)        /*!< Automatic end mode (master mode) */
#define  I2C_CR2_PECBYTE                     ((uint32_t)0x04000000)        /*!< Packet error checking byte */

/*******************  Bit definition for I2C_OAR1 register  ******************/
#define  I2C_OAR1_OA1                        ((uint32_t)0x000003FF)        /*!< Interface own address 1 */
#define  I2C_OAR1_OA1MODE                    ((uint32_t)0x00000400)        /*!< Own address 1 10-bit mode */
#define  I2C_OAR1_OA1EN                      ((uint32_t)0x00008000)        /*!< Own address 1 enable */

/*******************  Bit definition for I2C_OAR2 register  ******************/
#define  I2C_OAR2_OA2                        ((uint32_t)0x000000FE)        /*!< Interface own address 2 */
#define  I2C_OAR2_OA2MSK                     ((uint32_t)0x00000700)        /*!< Own address 2 masks */
#define  I2C_OAR2_OA2EN                      ((uint32_t)0x00008000)        /*!< Own address 2 enable */

/*******************  Bit definition for I2C_TIMINGR register *******************/
#define  I2C_TIMINGR_SCLL                    ((uint32_t)0x000000FF)        /*!< SCL low period (master mode) */
#define  I2C_TIMINGR_SCLH                    ((uint32_t)0x0000FF00)        /*!< SCL high period (master mode) */
#define  I2C_TIMINGR_SDADEL                  ((uint32_t)0x000F0000)        /*!< Data hold time */
#define  I2C_TIMINGR_SCLDEL                  ((uint32_t)0x00F00000)        /*!< Data setup time */
#define  I2C_TIMINGR_PRESC                   ((uint32_t)0xF0000000)        /*!< Timings prescaler */

/******************* Bit definition for I2C_TIMEOUTR register *******************/
#define  I2C_TIMEOUTR_TIMEOUTA               ((uint32_t)0x00000FFF)        /*!< Bus timeout A */
#define  I2C_TIMEOUTR_TIDLE                  ((uint32_t)0x00001000)        /*!< Idle clock timeout detection */
#define  I2C_TIMEOUTR_TIMOUTEN               ((uint32_t)0x00008000)        /*!< Clock timeout enable */
#define  I2C_TIMEOUTR_TIMEOUTB               ((uint32_t)0x0FFF0000)        /*!< Bus timeout B*/
#define  I2C_TIMEOUTR_TEXTEN                 ((uint32_t)0x80000000)        /*!< Extended clock timeout enable */

/******************  Bit definition for I2C_ISR register  *********************/
#define  I2C_ISR_TXE                         ((uint32_t)0x00000001)        /*!< Transmit data register empty */
#define  I2C_ISR_TXIS                        ((uint32_t)0x00000002)        /*!< Transmit interrupt status */
#define  I2C_ISR_RXNE                        ((uint32_t)0x00000004)        /*!< Receive data register not empty */
#define  I2C_ISR_ADDR                        ((uint32_t)0x00000008)        /*!< Address matched (slave mode)*/
#define  I2C_ISR_NACKF                       ((uint32_t)0x00000010)        /*!< NACK received flag */
#define  I2C_ISR_STOPF                       ((uint32_t)0x00000020)        /*!< STOP detection flag */
#define  I2C_ISR_TC                          ((uint32_t)0x00000040)        /*!< Transfer complete (master mode) */
#define  I2C_ISR_TCR                         ((uint32_t)0x00000080)        /*!< Transfer complete reload */
#define  I2C_ISR_BERR                        ((uint32_t)0x00000100)        /*!< Bus error */
#define  I2C_ISR_ARLO                        ((uint32_t)0x00000200)        /*!< Arbitration lost */
#define  I2C_ISR_OVR                         ((uint32_t)0x00000400)        /*!< Overrun/Underrun */
#define  I2C_ISR_PECERR                      ((uint32_t)0x00000800)        /*!< PEC error in reception */
#define  I2C_ISR_TIMEOUT                     ((uint32_t)0x00001000)        /*!< Timeout or Tlow detection flag */
#define  I2C_ISR_ALERT                       ((uint32_t)0x00002000)        /*!< SMBus alert */
#define  I2C_ISR_BUSY                        ((uint32_t)0x00008000)        /*!< Bus busy */
#define  I2C_ISR_DIR                         ((uint32_t)0x00010000)        /*!< Transfer direction (slave mode) */
#define  I2C_ISR_ADDCODE                     ((uint32_t)0x00FE0000)        /*!< Address match code (slave mode) */

/******************  Bit definition for I2C_ICR register  *********************/
#define  I2C_ICR_ADDRCF                      ((uint32_t)0x00000008)        /*!< Address matched clear flag */
#define  I2C_ICR_NACKCF                      ((uint32_t)0x00000010)        /*!< NACK clear flag */
#define  I2C_ICR_STOPCF                      ((uint32_t)0x00000020)        /*!< STOP detection clear flag */
#define  I2C_ICR_BERRCF                      ((uint32_t)0x00000100)        /*!< Bus error clear flag */
#define  I2C_ICR_ARLOCF                      ((uint32_t)0x00000200)        /*!< Arbitration lost clear flag */
#define  I2C_ICR_OVRCF                       ((uint32_t)0x00000400)        /*!< Overrun/Underrun clear flag */
#define  I2C_ICR_PECCF                       ((uint32_t)0x00000800)        /*!< PAC error clear flag */
#define  I2C_ICR_TIMOUTCF                    ((uint32_t)0x00001000)        /*!< Timeout clear flag */
#define  I2C_ICR_ALERTCF                     ((uint32_t)0x00002000)        /*!< Alert clear flag */

/******************  Bit definition for I2C_PECR register  *********************/
#define  I2C_PECR_PEC                        ((uint32_t)0x000000FF)        /*!< PEC register */

/******************  Bit definition for I2C_RXDR register  *********************/
#define  I2C_RXDR_RXDATA                     ((uint32_t)0x000000FF)        /*!< 8-bit receive data */

/******************  Bit definition for I2C_TXDR register  *********************/
#define  I2C_TXDR_TXDATA                     ((uint32_t)0x000000FF)        /*!< 8-bit transmit data */

/******************************************************************************/
/*                                                                            */
/*                           Independent WATCHDOG                             */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IWDG_KR register  ********************/
#define  IWDG_KR_KEY                         ((uint16_t)0xFFFF)            /*!<Key value (write only, read 0000h)  */

/*******************  Bit definition for IWDG_PR register  ********************/
#define  IWDG_PR_PR                          ((uint8_t)0x07)               /*!<PR[2:0] (Prescaler divider)         */
#define  IWDG_PR_PR_0                        ((uint8_t)0x01)               /*!<Bit 0 */
#define  IWDG_PR_PR_1                        ((uint8_t)0x02)               /*!<Bit 1 */
#define  IWDG_PR_PR_2                        ((uint8_t)0x04)               /*!<Bit 2 */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define  IWDG_RLR_RL                         ((uint16_t)0x0FFF)            /*!<Watchdog counter reload value        */

/*******************  Bit definition for IWDG_SR register  ********************/
#define  IWDG_SR_PVU                         ((uint8_t)0x01)               /*!< Watchdog prescaler value update */
#define  IWDG_SR_RVU                         ((uint8_t)0x02)               /*!< Watchdog counter reload value update */
#define  IWDG_SR_WVU                         ((uint8_t)0x04)               /*!< Watchdog counter window value update */

/*******************  Bit definition for IWDG_KR register  ********************/
#define  IWDG_WINR_WIN                         ((uint16_t)0x0FFF)            /*!< Watchdog counter window value */

/******************************************************************************/
/*                                                                            */
/*                                     Firewall                               */
/*                                                                            */
/******************************************************************************/

/*******Bit definition for CSSA;CSL;NVDSSA;NVDSL;VDSSA;VDSL;LSSA;LSL register */
#define  FIREWALL_CSSA_ADD                        ((uint32_t)0x00FFFF00)        /*!< Code Segment Start Address */ 
#define  FIREWALL_CSL_LENG                        ((uint32_t)0x003FFF00)        /*!< Code Segment Length        */  
#define  FIREWALL_NVDSSA_ADD                      ((uint32_t)0x00FFFF00)        /*!< Non Volatile Dat Segment Start Address */ 
#define  FIREWALL_NVDSL_LENG                      ((uint32_t)0x003FFF00)        /*!< Non Volatile Data Segment Length */ 
#define  FIREWALL_VDSSA_ADD                       ((uint32_t)0x0001FFC0)        /*!< Volatile Data Segment Start Address */ 
#define  FIREWALL_VDSL_LENG                       ((uint32_t)0x0001FFC0)        /*!< Volatile Data Segment Length */ 
#define  FIREWALL_LSSA_ADD                        ((uint32_t)0x0007FF80)        /*!< Library Segment Start Address*/ 
#define  FIREWALL_LSL_LENG                        ((uint32_t)0x0007FF80)        /*!< Library Segment Length*/ 
/**************************Bit definition for CR register *********************/
#define  FIREWALL_CR_FPA                          ((uint32_t)0x00000001)         /*!< Firewall Pre Arm*/ 
#define  FIREWALL_CR_VDS                          ((uint32_t)0x00000002)         /*!< Volatile Data Sharing*/ 
#define  FIREWALL_CR_VDE                          ((uint32_t)0x00000004)         /*!< Volatile Data Execution*/ 

/******************************************************************************/
/*                                                                            */
/*                             Power Control                                  */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for PWR_CR1 register  ********************/

#define  PWR_CR1_LPR                          ((uint32_t)0x00004000) 
#define  PWR_CR1_VOS                          ((uint32_t)0x00000600)     /*!< VOS[1:0] bits (Regulator voltage scaling output selection) */
#define  PWR_CR1_VOS_0                        ((uint32_t)0x00000200)     /*!< Bit 0 */
#define  PWR_CR1_VOS_1                        ((uint32_t)0x00000400)     /*!< Bit 1 */

#define  PWR_CR1_LPMS_DSLEEP                  ((uint32_t)0x00000002)
#define  PWR_CR1_LPMS_STDBY                   ((uint32_t)0x00000003)
#define  PWR_CR1_LPMS_SHUTD                   ((uint32_t)0x00000004)


/* Legacy define */
#define  PWR_CR_PMODE                        PWR_CR_VOS
/********************  Bit definition for PWR_CR2 register  ********************/
/*!< PVME  Peripheral Voltage Monitor Enable */
#define  PWR_CR2_PVME_0                       ((uint32_t)0x00000010)     /*!< PVM Voltage 0 */
#define  PWR_CR2_PVME_1                       ((uint32_t)0x00000020)     /*!< PVM Voltage 1 */
#define  PWR_CR2_PVME_2                       ((uint32_t)0x00000040)     /*!< PVM Voltage 2 */
#define  PWR_CR2_PVME_3                       ((uint32_t)0x00000080)     /*!< PVM Voltage 3 */
/*!< PVD level configuration */
#define  PWR_CR2_PVDE                         ((uint32_t)0x00000001)     /*!< Power Voltage Detector Enable       */
#define  PWR_CR2_PLS_LEV0                     ((uint32_t)0x00000000)     /*!< PVD level 0 */
#define  PWR_CR2_PLS_LEV1                     ((uint32_t)0x00000002)     /*!< PVD level 1 */
#define  PWR_CR2_PLS_LEV2                     ((uint32_t)0x00000004)     /*!< PVD level 2 */
#define  PWR_CR2_PLS_LEV3                     ((uint32_t)0x00000006)     /*!< PVD level 3 */
#define  PWR_CR2_PLS_LEV4                     ((uint32_t)0x00000008)     /*!< PVD level 4 */
#define  PWR_CR2_PLS_LEV5                     ((uint32_t)0x0000000A)     /*!< PVD level 5 */
#define  PWR_CR2_PLS_LEV6                     ((uint32_t)0x0000000C)     /*!< PVD level 6 */
#define  PWR_CR2_PLS_LEV7                     ((uint32_t)0x0000000E)     /*!< PVD level 7 */


/********************  Bit definition for PWR_CR3 register  ********************/
#define  PWR_CR3_RRS                         ((uint32_t)0x00000100)
/********************  Bit definition for PWR_CR4 register  ********************/

/********************  Bit definition for PWR_SR1 register  ********************/
#define   PWR_SR1_WUFI                        ((uint32_t)0x00008000)  
#define   PWR_SR1_SBF                         ((uint32_t)0x00000100) 
#define   PWR_SR1_WUF4                        ((uint32_t)0x00000010) 
#define   PWR_SR1_WUF3                        ((uint32_t)0x00000008) 
#define   PWR_SR1_WUF2                        ((uint32_t)0x00000004) 
#define   PWR_SR1_WUF1                        ((uint32_t)0x00000002) 
#define   PWR_SR1_WUF0                        ((uint32_t)0x00000001) 
/********************  Bit definition for PWR_SR2 register  ********************/

#define   PWR_SR2_PVDO3                       ((uint32_t)0x00008000) 
#define   PWR_SR2_PVDO2                       ((uint32_t)0x00004000) 
#define   PWR_SR2_PVDO1                       ((uint32_t)0x00002000) 
#define   PWR_SR2_PVDO0                       ((uint32_t)0x00001000)
#define   PWR_SR2_PVDO                        ((uint32_t)0x00000800) 
#define   PWR_SR2_VOSRDY                      ((uint32_t)0x00000400) 
#define   PWR_SR2_REGLPF                      ((uint32_t)0x00000200) 


/* Legacy define */
#define  PWR_CSR_REGRDY                      PWR_CSR_VOSRDY
/********************  Bit definition for PWR_SRR register  ********************/



/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for RCC_CR register  ********************/
#define  RCC_CR_MSION                        ((uint32_t)0x00000001)      /*!< Internal Multi Speed clock enable */
#define  RCC_CR_MSIRDY                       ((uint32_t)0x00000002)      /*!< Internal Multi Speed clock ready flag */
#define  RCC_CR_MSIPLLEN                     ((uint32_t)0x00000004)      /*!< Internal Multi Speed PLL enable */
#define  RCC_CR_MSIRGSEL                     ((uint32_t)0x00000008)      /*!< Internal Multi Speed range selection */

/*!< MSIRANGE configuration : 12 frequency ranges available */

#define  RCC_CR_MSIRANGE                     ((uint32_t)0x000000F0)       /*!< Internal Multi Speed clock Range */
#define  RCC_CR_MSIRANGE_0                   ((uint32_t)0x00000000)       /*!< Internal Multi Speed clock Range 100 KHz */
#define  RCC_CR_MSIRANGE_1                   ((uint32_t)0x00000010)       /*!< Internal Multi Speed clock Range 200 KHz */
#define  RCC_CR_MSIRANGE_2                   ((uint32_t)0x00000020)       /*!< Internal Multi Speed clock Range 400 KHz */
#define  RCC_CR_MSIRANGE_3                   ((uint32_t)0x00000030)       /*!< Internal Multi Speed clock Range 800 KHz */
#define  RCC_CR_MSIRANGE_4                   ((uint32_t)0x00000040)       /*!< Internal Multi Speed clock Range 1 MHz */
#define  RCC_CR_MSIRANGE_5                   ((uint32_t)0x00000050)       /*!< Internal Multi Speed clock Range 2 MHz */
#define  RCC_CR_MSIRANGE_6                   ((uint32_t)0x00000060)       /*!< Internal Multi Speed clock Range 4  MHz */
#define  RCC_CR_MSIRANGE_7                   ((uint32_t)0x00000070)       /*!< Internal Multi Speed clock Range 8 KHz */
#define  RCC_CR_MSIRANGE_8                   ((uint32_t)0x00000080)       /*!< Internal Multi Speed clock Range 16 MHz */
#define  RCC_CR_MSIRANGE_9                   ((uint32_t)0x00000090)       /*!< Internal Multi Speed clock Range 24 MHz */
#define  RCC_CR_MSIRANGE_10                  ((uint32_t)0x000000A0)       /*!< Internal Multi Speed clock Range 32 MHz */
#define  RCC_CR_MSIRANGE_11                  ((uint32_t)0x000000B0)       /*!< Internal Multi Speed clock Range 48  MHz */

#define  RCC_CR_HSION                        ((uint32_t)0x00000100)       /*!< Internal High Speed clock enable */
#define  RCC_CR_HSIKERON                     ((uint32_t)0x00000200)       /*!< Internal High Speed clock enable for some IPs Kernel */
#define  RCC_CR_HSIRDY                       ((uint32_t)0x00000400)       /*!< Internal High Speed clock ready flag */
#define  RCC_CR_HSIASFS                      ((uint32_t)0x00000800)       /*!< HSI Automatic Start from Stop */

#define  RCC_CR_HSEON                        ((uint32_t)0x00010000)       /*!< External High Speed clock enable */
#define  RCC_CR_HSERDY                       ((uint32_t)0x00020000)       /*!< External High Speed clock ready */
#define  RCC_CR_HSEBYP                       ((uint32_t)0x00040000)       /*!< External High Speed clock Bypass */
#define  RCC_CR_HSECSSON                     ((uint32_t)0x00080000)       /*!< HSE Clock security System enable */

#define  RCC_CR_PLLSYSON                     ((uint32_t)0x01000000)       /*!< System PLL clock enable */
#define  RCC_CR_PLLSYSRDY                    ((uint32_t)0x02000000)       /*!< System PLL clock ready */
#define  RCC_CR_PLLSAI1ON                    ((uint32_t)0x04000000)       /*!< SAI1 PLL enable */
#define  RCC_CR_PLLSAI1RDY                   ((uint32_t)0x08000000)       /*!< SAI PLL ready */
#define  RCC_CR_PLLSAI2ON                    ((uint32_t)0x10000000)       /*!< SAI2 PLL enable */
#define  RCC_CR_PLLSAI2RDY                   ((uint32_t)0x20000000)       /*!< SAI2 PLL ready */

/********************  Bit definition for RCC_ICSCR register  ***************/
/*!< MSICAL configuration */
#define  RCC_ICSCR_MSICAL                    ((uint32_t)0x000000FF)       /*!< MSICAL[7:0] bits */
#define  RCC_ICSCR_MSICAL_0                  ((uint32_t)0x00000001)       /*!<Bit 0 */
#define  RCC_ICSCR_MSICAL_1                  ((uint32_t)0x00000002)       /*!<Bit 1 */
#define  RCC_ICSCR_MSICAL_2                  ((uint32_t)0x00000004)       /*!<Bit 2 */
#define  RCC_ICSCR_MSICAL_3                  ((uint32_t)0x00000008)       /*!<Bit 3 */
#define  RCC_ICSCR_MSICAL_4                  ((uint32_t)0x00000010)       /*!<Bit 4 */
#define  RCC_ICSCR_MSICAL_5                  ((uint32_t)0x00000020)       /*!<Bit 5 */
#define  RCC_ICSCR_MSICAL_6                  ((uint32_t)0x00000040)       /*!<Bit 6 */
#define  RCC_ICSCR_MSICAL_7                  ((uint32_t)0x00000080)       /*!<Bit 7 */

/*!< MSITRIM configuration */
#define  RCC_ICSCR_MSITRIM                   ((uint32_t)0x0000FF00)       /*!< MSITRIM[7:0] bits */
#define  RCC_ICSCR_MSITRIM_0                 ((uint32_t)0x00000100)       /*!<Bit 0 */
#define  RCC_ICSCR_MSITRIM_1                 ((uint32_t)0x00000200)       /*!<Bit 1 */
#define  RCC_ICSCR_MSITRIM_2                 ((uint32_t)0x00000400)       /*!<Bit 2 */
#define  RCC_ICSCR_MSITRIM_3                 ((uint32_t)0x00000800)       /*!<Bit 3 */
#define  RCC_ICSCR_MSITRIM_4                 ((uint32_t)0x00001000)       /*!<Bit 4 */
#define  RCC_ICSCR_MSITRIM_5                 ((uint32_t)0x00002000)       /*!<Bit 5 */
#define  RCC_ICSCR_MSITRIM_6                 ((uint32_t)0x00004000)       /*!<Bit 6 */
#define  RCC_ICSCR_MSITRIM_7                 ((uint32_t)0x00008000)       /*!<Bit 7 */

/*!< HSICAL configuration */
#define  RCC_ICSCR_HSICAL                    ((uint32_t)0x00FF0000)       /*!< HSICAL[7:0] bits */
#define  RCC_ICSCR_HSICAL_0                  ((uint32_t)0x00010000)        /*!<Bit 0 */
#define  RCC_ICSCR_HSICAL_1                  ((uint32_t)0x00020000)        /*!<Bit 1 */
#define  RCC_ICSCR_HSICAL_2                  ((uint32_t)0x00040000)        /*!<Bit 2 */
#define  RCC_ICSCR_HSICAL_3                  ((uint32_t)0x00080000)        /*!<Bit 3 */
#define  RCC_ICSCR_HSICAL_4                  ((uint32_t)0x00100000)        /*!<Bit 4 */
#define  RCC_ICSCR_HSICAL_5                  ((uint32_t)0x00200000)        /*!<Bit 5 */
#define  RCC_ICSCR_HSICAL_6                  ((uint32_t)0x00400000)        /*!<Bit 6 */
#define  RCC_ICSCR_HSICAL_7                  ((uint32_t)0x00800000)        /*!<Bit 7 */

/*!< HSITRIM configuration */
#define  RCC_ICSCR_HSITRIM                   ((uint32_t)0x1F000000)       /*!< HSITRIM[7:0] bits */
#define  RCC_ICSCR_HSITRIM_0                 ((uint32_t)0x01000000)        /*!<Bit 0 */
#define  RCC_ICSCR_HSITRIM_1                 ((uint32_t)0x02000000)        /*!<Bit 1 */
#define  RCC_ICSCR_HSITRIM_2                 ((uint32_t)0x04000000)        /*!<Bit 2 */
#define  RCC_ICSCR_HSITRIM_3                 ((uint32_t)0x08000000)        /*!<Bit 3 */
#define  RCC_ICSCR_HSITRIM_4                 ((uint32_t)0x10000000)        /*!<Bit 4 */


/********************  Bit definition for RCC_PLLCFGR register  ***************/

#define  RCC_PLLCFGR_PLLSRC                    ((uint32_t)0x00000003)

#define  RCC_PLLCFGR_PLLSRC_MSI                 ((uint32_t)0x00000001)     /*!< MSI source clock selected */
#define  RCC_PLLCFGR_PLLSRC_HSI                 ((uint32_t)0x00000002)     /*!< HSI source clock selected */
#define  RCC_PLLCFGR_PLLSRC_HSE                 ((uint32_t)0x00000003)     /*!< HSE source clock selected */


#define  RCC_PLLCFGR_PLLM                      ((uint32_t)0x00000070)
#define  RCC_PLLCFGR_PLLM_0                    ((uint32_t)0x00000010)
#define  RCC_PLLCFGR_PLLM_1                    ((uint32_t)0x00000020)
#define  RCC_PLLCFGR_PLLM_2                    ((uint32_t)0x00000040)

#define  RCC_PLLCFGR_PLLN                      ((uint32_t)0x00007F00)
#define  RCC_PLLCFGR_PLLN_0                    ((uint32_t)0x00000100)
#define  RCC_PLLCFGR_PLLN_1                    ((uint32_t)0x00000200)
#define  RCC_PLLCFGR_PLLN_2                    ((uint32_t)0x00000400)
#define  RCC_PLLCFGR_PLLN_3                    ((uint32_t)0x00000800)
#define  RCC_PLLCFGR_PLLN_4                    ((uint32_t)0x00001000)
#define  RCC_PLLCFGR_PLLN_5                    ((uint32_t)0x00002000)
#define  RCC_PLLCFGR_PLLN_6                    ((uint32_t)0x00004000)

#define  RCC_PLLCFGR_PLLPEN                    ((uint32_t)0x00010000)
#define  RCC_PLLCFGR_PLLP                      ((uint32_t)0x00020000)
#define  RCC_PLLCFGR_PLLQEN                    ((uint32_t)0x00100000)

#define  RCC_PLLCFGR_PLLQ                      ((uint32_t)0x00600000)
#define  RCC_PLLCFGR_PLLQ_0                    ((uint32_t)0x00200000)
#define  RCC_PLLCFGR_PLLQ_1                    ((uint32_t)0x00400000)

#define  RCC_PLLCFGR_PLLREN                    ((uint32_t)0x01000000)
#define  RCC_PLLCFGR_PLLR                      ((uint32_t)0x06000000)
#define  RCC_PLLCFGR_PLLR_0                    ((uint32_t)0x02000000)
#define  RCC_PLLCFGR_PLLR_1                    ((uint32_t)0x04000000)


/********************  Bit definition for RCC_CFGR register  ******************/
/*!< SW configuration */
#define  RCC_CFGR_SW                          ((uint32_t)0x00000003)       /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_0                        ((uint32_t)0x00000001)       /*!<Bit 0 */
#define  RCC_CFGR_SW_1                        ((uint32_t)0x00000002)       /*!<Bit 1 */

#define  RCC_CFGR_SW_MSI                      ((uint32_t)0x00000000)       /*!< MSI selection as system clock */
#define  RCC_CFGR_SW_HSI                      ((uint32_t)0x00000001)       /*!< HSI selection as system clock */
#define  RCC_CFGR_SW_HSE                      ((uint32_t)0x00000002)       /*!< HSE selection as system clock */
#define  RCC_CFGR_SW_PLL                      ((uint32_t)0x00000003)       /*!< PLL selection as system clock */

#define  RCC_CFGR_SWS_MSI                      ((uint32_t)0x00000000)      /*!< MSI used as system clock */
#define  RCC_CFGR_SWS_HSI                      ((uint32_t)0x00000004)      /*!< HSI used as system clock */
#define  RCC_CFGR_SWS_HSE                      ((uint32_t)0x00000008)      /*!< HSE used as system clock */
#define  RCC_CFGR_SWS_PLL                      ((uint32_t)0x0000000C)      /*!< PLL used as system clock */

/*!< SWS configuration */  
#define  RCC_CFGR_SWS                         ((uint32_t)0x0000000C)       /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_0                       ((uint32_t)0x00000004)       /*!<Bit 0 */ 
#define  RCC_CFGR_SWS_1                       ((uint32_t)0x00000008)       /*!<Bit 1 */

/*!< HPRE configuration */  
#define  RCC_CFGR_HPRE                        ((uint32_t)0x000000F0)       /*!< HPRE[3:0] bits (AHB prescaler) */
#define  RCC_CFGR_HPRE_0                      ((uint32_t)0x00000010)       /*!<Bit 0 */
#define  RCC_CFGR_HPRE_1                      ((uint32_t)0x00000020)       /*!<Bit 1 */
#define  RCC_CFGR_HPRE_2                      ((uint32_t)0x00000040)       /*!<Bit 2 */
#define  RCC_CFGR_HPRE_3                      ((uint32_t)0x00000080)       /*!<Bit 3 */


#define  RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000)        /*!< SYSCLK not divided */
#define  RCC_CFGR_HPRE_DIV2                  ((uint32_t)0x00000080)        /*!< SYSCLK divided by 2 */
#define  RCC_CFGR_HPRE_DIV4                  ((uint32_t)0x00000090)        /*!< SYSCLK divided by 4 */
#define  RCC_CFGR_HPRE_DIV8                  ((uint32_t)0x000000A0)        /*!< SYSCLK divided by 8 */
#define  RCC_CFGR_HPRE_DIV16                 ((uint32_t)0x000000B0)        /*!< SYSCLK divided by 16 */
#define  RCC_CFGR_HPRE_DIV64                 ((uint32_t)0x000000C0)        /*!< SYSCLK divided by 64 */
#define  RCC_CFGR_HPRE_DIV128                ((uint32_t)0x000000D0)        /*!< SYSCLK divided by 128 */
#define  RCC_CFGR_HPRE_DIV256                ((uint32_t)0x000000E0)        /*!< SYSCLK divided by 256 */
#define  RCC_CFGR_HPRE_DIV512                ((uint32_t)0x000000F0)        /*!< SYSCLK divided by 512 */


/*!< PPRE1 configuration */
#define  RCC_CFGR_PPRE1                       ((uint32_t)0x00000700)       /*!< PRE1[2:0] bits (APB2 prescaler) */
#define  RCC_CFGR_PPRE1_0                     ((uint32_t)0x00000100)       /*!<Bit 0 */
#define  RCC_CFGR_PPRE1_1                     ((uint32_t)0x00000200)       /*!<Bit 1 */
#define  RCC_CFGR_PPRE1_2                     ((uint32_t)0x00000400)       /*!<Bit 2 */

#define  RCC_CFGR_PPRE1_DIV1                 ((uint32_t)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE1_DIV2                 ((uint32_t)0x00000400)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4                 ((uint32_t)0x00000500)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE1_DIV8                 ((uint32_t)0x00000600)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE1_DIV16                ((uint32_t)0x00000700)        /*!< HCLK divided by 16 */

/*!< PPRE2 configuration */ 
#define  RCC_CFGR_PPRE2                       ((uint32_t)0x00003800)       /*!< PRE2[2:0] bits (APB2 prescaler) */
#define  RCC_CFGR_PPRE2_0                     ((uint32_t)0x00000800)       /*!<Bit 0 */
#define  RCC_CFGR_PPRE2_1                     ((uint32_t)0x00001000)       /*!<Bit 1 */
#define  RCC_CFGR_PPRE2_2                     ((uint32_t)0x00002000)       /*!<Bit 2 */


#define  RCC_CFGR_PPRE2_DIV1                 ((uint32_t)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE2_DIV2                 ((uint32_t)0x00002000)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE2_DIV4                 ((uint32_t)0x00002800)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE2_DIV8                 ((uint32_t)0x00003000)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE2_DIV16                ((uint32_t)0x00003800)        /*!< HCLK divided by 16 */

/*!< MCOSEL configuration */ 
#define  RCC_CFGR_MCOSEL                       ((uint32_t)0x07000000)       /*!< MCOSEL [2:0] bits (Clock output selection) */
#define  RCC_CFGR_MCOSEL_0                     ((uint32_t)0x01000000)       /*!<Bit 0 */
#define  RCC_CFGR_MCOSEL_1                     ((uint32_t)0x02000000)       /*!<Bit 1 */
#define  RCC_CFGR_MCOSEL_2                     ((uint32_t)0x04000000)       /*!<Bit 2 */

#define  RCC_CFGR_MCO_PRE                    ((uint32_t)0x70000000)        /*!< MCO prescaler */
#define  RCC_CFGR_MCO_PRE_1                  ((uint32_t)0x00000000)        /*!< MCO is divided by 1 */
#define  RCC_CFGR_MCO_PRE_2                  ((uint32_t)0x10000000)        /*!< MCO is divided by 2 */
#define  RCC_CFGR_MCO_PRE_4                  ((uint32_t)0x20000000)        /*!< MCO is divided by 4 */
#define  RCC_CFGR_MCO_PRE_8                  ((uint32_t)0x30000000)        /*!< MCO is divided by 8 */
#define  RCC_CFGR_MCO_PRE_16                 ((uint32_t)0x40000000)        /*!< MCO is divided by 16 */

/********************  Bit definition for RCC_CIERR register  ******************/
#define  RCC_CIER_LSIRDYIE                    ((uint32_t)0x00000001)
#define  RCC_CIER_LSERDYIE                    ((uint32_t)0x00000002)
#define  RCC_CIER_MSIRDYIE                    ((uint32_t)0x00000004)
#define  RCC_CIER_HSIRDYIE                    ((uint32_t)0x00000008)
#define  RCC_CIER_HSERDYIE                    ((uint32_t)0x00000010)
#define  RCC_CIER_PLLRDYIE                    ((uint32_t)0x00000020)
#define  RCC_CIER_PLLSAI1RDYIE                ((uint32_t)0x00000040)
#define  RCC_CIER_PLLSAI2RDYIE                ((uint32_t)0x00000080)
#define  RCC_CIER_LSECSSIE                    ((uint32_t)0x00000200)

/********************  Bit definition for RCC_CIFR register  ******************/
#define  RCC_CIER_LSIRDYF                     ((uint32_t)0x00000001)
#define  RCC_CIER_LSERDYF                     ((uint32_t)0x00000002)
#define  RCC_CIER_MSIRDYF                     ((uint32_t)0x00000004)
#define  RCC_CIER_HSIRDYF                     ((uint32_t)0x00000008)
#define  RCC_CIER_HSERDYF                     ((uint32_t)0x00000010)
#define  RCC_CIER_PLLRDYF                     ((uint32_t)0x00000020)
#define  RCC_CIER_PLLSAI1RDYF                 ((uint32_t)0x00000040)
#define  RCC_CIER_PLLSAI2RDYF                 ((uint32_t)0x00000080)
#define  RCC_CIER_HSECSSF                     ((uint32_t)0x00000100)
#define  RCC_CIER_LSECSSF                     ((uint32_t)0x00000200)

/********************  Bit definition for RCC_CICR register  ******************/
#define  RCC_CIER_LSIRDYC                     ((uint32_t)0x00000001)
#define  RCC_CIER_LSERDYC                     ((uint32_t)0x00000002)
#define  RCC_CIER_MSIRDYC                     ((uint32_t)0x00000004)
#define  RCC_CIER_HSIRDYC                     ((uint32_t)0x00000008)
#define  RCC_CIER_HSERDYC                     ((uint32_t)0x00000010)
#define  RCC_CIER_PLLRDYC                     ((uint32_t)0x00000020)
#define  RCC_CIER_PLLSAI1RDYC                 ((uint32_t)0x00000040)
#define  RCC_CIER_PLLSAI2RDYC                 ((uint32_t)0x00000080)
#define  RCC_CIER_HSECSSC                     ((uint32_t)0x00000100)
#define  RCC_CIER_CSSC                        ((uint32_t)0x00000200)


/********************  Bit definition for RCC_AHB1RSTR register  **************/
#define  RCC_AHB1RSTR_DMA1RST                ((uint32_t)0x00000001)
#define  RCC_AHB1RSTR_DMA2RST                ((uint32_t)0x00000002)
#define  RCC_AHB1RSTR_FLITFRST               ((uint32_t)0x00000100)
#define  RCC_AHB1RSTR_CRCRST                 ((uint32_t)0x00001000)
#define  RCC_AHB1RSTR_TSCRST                 ((uint32_t)0x00010000)


/********************  Bit definition for RCC_AHB2RSTR register  **************/
#define  RCC_AHB2RSTR_GPIOARST                 ((uint32_t)0x00000001)
#define  RCC_AHB2RSTR_GPIOBRST                 ((uint32_t)0x00000002)
#define  RCC_AHB2RSTR_GPIOCRST                 ((uint32_t)0x00000004)
#define  RCC_AHB2RSTR_GPIODRST                 ((uint32_t)0x00000008)
#define  RCC_AHB2RSTR_GPIOERST                 ((uint32_t)0x00000010)
#define  RCC_AHB2RSTR_GPIOFRST                 ((uint32_t)0x00000020)
#define  RCC_AHB2RSTR_GPIOGRST                 ((uint32_t)0x00000040)
#define  RCC_AHB2RSTR_GPIOHRST                 ((uint32_t)0x00000080)
#define  RCC_AHB2RSTR_OTGFSRST                 ((uint32_t)0x00001000)
#define  RCC_AHB2RSTR_ADCRST                   ((uint32_t)0x00002000)
#define  RCC_AHB2RSTR_AESRST                   ((uint32_t)0x00010000)
#define  RCC_AHB2RSTR_RNGRST                   ((uint32_t)0x00040000)

/********************  Bit definition for RCC_AHB3RSTR register  **************/
#define  RCC_AHB3RSTR_FMCRST                 ((uint32_t)0x00000001)
#define  RCC_AHB3RSTR_QSPIRST                ((uint32_t)0x00000100)

/********************  Bit definition for RCC_APB1LRSTR register  **************/
#define  RCC_APB1LRSTR_TIM2RST                ((uint32_t)0x00000001)
#define  RCC_APB1LRSTR_TIM3RST                ((uint32_t)0x00000002)
#define  RCC_APB1LRSTR_TIM4RST                ((uint32_t)0x00000004)
#define  RCC_APB1LRSTR_TIM5RST                ((uint32_t)0x00000008)
#define  RCC_APB1LRSTR_TIM6RST                ((uint32_t)0x00000010)
#define  RCC_APB1LRSTR_TIM7RST                ((uint32_t)0x00000020)
#define  RCC_APB1LRSTR_LCDRST                 ((uint32_t)0x00000200)
#define  RCC_APB1LRSTR_SPI2RST                ((uint32_t)0x00004000)
#define  RCC_APB1LRSTR_SPI3RST                ((uint32_t)0x00008000)
#define  RCC_APB1LRSTR_USART2RST              ((uint32_t)0x00020000)
#define  RCC_APB1LRSTR_USART3RST              ((uint32_t)0x00040000)
#define  RCC_APB1LRSTR_UART4RST               ((uint32_t)0x00080000)
#define  RCC_APB1LRSTR_UART5RST               ((uint32_t)0x00100000)
#define  RCC_APB1LRSTR_I2C1RST                ((uint32_t)0x00200000)
#define  RCC_APB1LRSTR_I2C2RST                ((uint32_t)0x00400000)
#define  RCC_APB1LRSTR_I2C3RST                ((uint32_t)0x00800000)
#define  RCC_APB1LRSTR_CANRST                 ((uint32_t)0x02000000)
#define  RCC_APB1LRSTR_PWRRST                 ((uint32_t)0x10000000)
#define  RCC_APB1LRSTR_DACRST                 ((uint32_t)0x20000000)
#define  RCC_APB1LRSTR_OPAMPRST               ((uint32_t)0x40000000)
#define  RCC_APB1LRSTR_LPTIM1RST              ((uint32_t)0x80000000)

/********************  Bit definition for RCC_APB1HRSTR register  **************/
#define  RCC_APB1HRSTR_LPUARTRST               ((uint32_t)0x00000001)
#define  RCC_APB1HRSTR_SWPRSTRST               ((uint32_t)0x00000004)
#define  RCC_APB1HRSTR_LPTIM2RST               ((uint32_t)0x00000020)

/********************  Bit definition for RCC_APB2RSTR register  **************/
#define  RCC_APB2RSTR_SYSCFGRST                ((uint32_t)0x00000001)
#define  RCC_APB2RSTR_SDIORST                  ((uint32_t)0x00000400)
#define  RCC_APB2RSTR_TIM1RST                  ((uint32_t)0x00000800)
#define  RCC_APB2RSTR_SPI1RST                  ((uint32_t)0x00001000)
#define  RCC_APB2RSTR_TIM8RST                  ((uint32_t)0x00002000)
#define  RCC_APB2RSTR_USART1RST                ((uint32_t)0x00004000)
#define  RCC_APB2RSTR_TIM15RST                 ((uint32_t)0x00010000)
#define  RCC_APB2RSTR_TIM16RST                 ((uint32_t)0x00020000)
#define  RCC_APB2RSTR_TIM17RST                 ((uint32_t)0x00040000)
#define  RCC_APB2RSTR_SAI1RST                  ((uint32_t)0x00200000)
#define  RCC_APB2RSTR_SAI2RST                  ((uint32_t)0x00400000)
#define  RCC_APB2RSTR_DFSDMRST                 ((uint32_t)0x01000000)

/********************  Bit definition for RCC_AHB1ENR register  ***************/
#define  RCC_AHB1ENR_DMA1EN                ((uint32_t)0x00000001)
#define  RCC_AHB1ENR_DMA2EN                ((uint32_t)0x00000002)
#define  RCC_AHB1ENR_FLITFEN               ((uint32_t)0x00000100)
#define  RCC_AHB1ENR_CRCEN                 ((uint32_t)0x00001000)
#define  RCC_AHB1ENR_TSCEN                 ((uint32_t)0x00010000)

/********************  Bit definition for RCC_AHB2ENR register  ***************/
#define  RCC_AHB2ENR_GPIOAEN                 ((uint32_t)0x00000001)
#define  RCC_AHB2ENR_GPIOBEN                 ((uint32_t)0x00000002)
#define  RCC_AHB2ENR_GPIOCEN                 ((uint32_t)0x00000004)
#define  RCC_AHB2ENR_GPIODEN                 ((uint32_t)0x00000008)
#define  RCC_AHB2ENR_GPIOEEN                 ((uint32_t)0x00000010)
#define  RCC_AHB2ENR_GPIOFEN                 ((uint32_t)0x00000020)
#define  RCC_AHB2ENR_GPIOGEN                 ((uint32_t)0x00000040)
#define  RCC_AHB2ENR_GPIOHEN                 ((uint32_t)0x00000080)
#define  RCC_AHB2ENR_OTGFSEN                 ((uint32_t)0x00001000)
#define  RCC_AHB2ENR_ADCEN                   ((uint32_t)0x00002000)
#define  RCC_AHB2ENR_AESEN                   ((uint32_t)0x00010000)
#define  RCC_AHB2ENR_RNGEN                   ((uint32_t)0x00040000)

/********************  Bit definition for RCC_AHB3ENR register  ***************/
#define  RCC_AHB3ENR_FMCEN                   ((uint32_t)0x00000001)
#define  RCC_AHB3ENR_QSPIEN                  ((uint32_t)0x00000100)


/********************  Bit definition for RCC_APB1LENR register  ***************/
#define  RCC_APB1LENR_TIM2EN                ((uint32_t)0x00000001)
#define  RCC_APB1LENR_TIM3EN                ((uint32_t)0x00000002)
#define  RCC_APB1LENR_TIM4EN                ((uint32_t)0x00000004)
#define  RCC_APB1LENR_TIM5EN                ((uint32_t)0x00000008)
#define  RCC_APB1LENR_TIM6EN                ((uint32_t)0x00000010)
#define  RCC_APB1LENR_TIM7EN                ((uint32_t)0x00000020)
#define  RCC_APB1LENR_LCDEN                 ((uint32_t)0x00000200)
#define  RCC_APB1LENR_WWDGEN                ((uint32_t)0x00000800)
#define  RCC_APB1LENR_SPI2EN                ((uint32_t)0x00004000)
#define  RCC_APB1LENR_SPI3EN                ((uint32_t)0x00008000)
#define  RCC_APB1LENR_USART2EN              ((uint32_t)0x00020000)
#define  RCC_APB1LENR_USART3EN              ((uint32_t)0x00040000)
#define  RCC_APB1LENR_UART4EN               ((uint32_t)0x00080000)
#define  RCC_APB1LENR_UART5EN               ((uint32_t)0x00100000)
#define  RCC_APB1LENR_I2C1EN                ((uint32_t)0x00200000)
#define  RCC_APB1LENR_I2C2EN                ((uint32_t)0x00400000)
#define  RCC_APB1LENR_I2C3EN                ((uint32_t)0x00800000)
#define  RCC_APB1LENR_CANEN                 ((uint32_t)0x02000000)
#define  RCC_APB1LENR_PWREN                 ((uint32_t)0x10000000)
#define  RCC_APB1LENR_DACEN                 ((uint32_t)0x20000000)
#define  RCC_APB1LENR_OPAMPEN               ((uint32_t)0x40000000)
#define  RCC_APB1LENR_LPTIM1EN              ((uint32_t)0x80000000)
/********************  Bit definition for RCC_APB1HRSTR register  **************/
#define  RCC_APB1HENR_LPUARTEN               ((uint32_t)0x00000001)
#define  RCC_APB1HENR_SWPEN                  ((uint32_t)0x00000004)
#define  RCC_APB1HENR_LPTIM2EN               ((uint32_t)0x00000020)

/********************  Bit definition for RCC_APB2ENR register  ***************/
#define  RCC_APB2ENR_SYSCFGEN                ((uint32_t)0x00000001)
#define  RCC_APB2ENR_FIREWALLEN              ((uint32_t)0x00000100)
#define  RCC_APB2ENR_SDMMCEN                 ((uint32_t)0x00000400)
#define  RCC_APB2ENR_TIM1EN                  ((uint32_t)0x00000800)
#define  RCC_APB2ENR_SPI1EN                  ((uint32_t)0x00001000)
#define  RCC_APB2ENR_TIM8EN                  ((uint32_t)0x00002000)
#define  RCC_APB2ENR_USART1EN                ((uint32_t)0x00004000)
#define  RCC_APB2ENR_TIM15EN                 ((uint32_t)0x00010000)
#define  RCC_APB2ENR_TIM16EN                 ((uint32_t)0x00020000)
#define  RCC_APB2ENR_TIM17EN                 ((uint32_t)0x00040000)
#define  RCC_APB2ENR_SAI1EN                  ((uint32_t)0x00200000)
#define  RCC_APB2ENR_SAI2EN                  ((uint32_t)0x00400000)
#define  RCC_APB2ENR_DFSDMEN                 ((uint32_t)0x01000000)


/********************  Bit definition for RCC_AHB1LPENR register  ***************/
#define  RCC_AHB1LPENR_DMA1LPEN                ((uint32_t)0x00000001)
#define  RCC_AHB1LPENR_DMA2LPEN                ((uint32_t)0x00000002)
#define  RCC_AHB1LPENR_FLITFLPEN               ((uint32_t)0x00000100)
#define  RCC_AHB1LPENR_CRCLPEN                 ((uint32_t)0x00001000)
#define  RCC_AHB1LPENR_TSCLPEN                 ((uint32_t)0x00010000)
/********************  Bit definition for RCC_AHB2LPENR register  *************/
#define  RCC_AHB2LPENR_GPIOALPEN                 ((uint32_t)0x00000001)
#define  RCC_AHB2LPENR_GPIOBLPEN                 ((uint32_t)0x00000002)
#define  RCC_AHB2LPENR_GPIOCLPEN                 ((uint32_t)0x00000004)
#define  RCC_AHB2LPENR_GPIODLPEN                 ((uint32_t)0x00000008)
#define  RCC_AHB2LPENR_GPIOELPEN                 ((uint32_t)0x00000010)
#define  RCC_AHB2LPENR_GPIOFLPEN                 ((uint32_t)0x00000020)
#define  RCC_AHB2LPENR_GPIOGLPEN                 ((uint32_t)0x00000040)
#define  RCC_AHB2LPENR_GPIOHLPEN                 ((uint32_t)0x00000080)
#define  RCC_AHB2LPENR_OTGFSLPEN                 ((uint32_t)0x00001000)
#define  RCC_AHB2LPENR_ADCLPEN                   ((uint32_t)0x00002000)
#define  RCC_AHB2LPENR_AESLPEN                   ((uint32_t)0x00010000)
#define  RCC_AHB2LPENR_RNGLPEN                   ((uint32_t)0x00040000)
/********************  Bit definition for RCC_AHB3LPENR register  *************/
#define  RCC_AHB3LPENR_FMCLPEN                   ((uint32_t)0x00000001)
#define  RCC_AHB3LPENR_QSPILPEN                  ((uint32_t)0x00000100)

/********************  Bit definition for RCC_APB1LLPENR register  *************/
#define  RCC_APB1LLPENR_TIM2LPEN                ((uint32_t)0x00000001)
#define  RCC_APB1LLPENR_TIM3LPEN                ((uint32_t)0x00000002)
#define  RCC_APB1LLPENR_TIM4LPEN                ((uint32_t)0x00000004)
#define  RCC_APB1LLPENR_TIM5LPEN                ((uint32_t)0x00000008)
#define  RCC_APB1LLPENR_TIM6LPEN                ((uint32_t)0x00000010)
#define  RCC_APB1LLPENR_TIM7LPEN                ((uint32_t)0x00000020)
#define  RCC_APB1LLPENR_LCDLPEN                 ((uint32_t)0x00000200)
#define  RCC_APB1LLPENR_WWDGLPEN                ((uint32_t)0x00000800)
#define  RCC_APB1LLPENR_SPI2LPEN                ((uint32_t)0x00004000)
#define  RCC_APB1LLPENR_SPI3LPEN                ((uint32_t)0x00008000)
#define  RCC_APB1LLPENR_USART2LPEN              ((uint32_t)0x00020000)
#define  RCC_APB1LLPENR_USART3LPEN              ((uint32_t)0x00040000)
#define  RCC_APB1LLPENR_UART4LPEN               ((uint32_t)0x00080000)
#define  RCC_APB1LLPENR_UART5LPEN               ((uint32_t)0x00100000)
#define  RCC_APB1LLPENR_I2C1LPEN                ((uint32_t)0x00200000)
#define  RCC_APB1LLPENR_I2C2LPEN                ((uint32_t)0x00400000)
#define  RCC_APB1LLPENR_I2C3LPEN                ((uint32_t)0x00800000)
#define  RCC_APB1LLPENR_CANLPEN                 ((uint32_t)0x02000000)
#define  RCC_APB1LLPENR_PWRLPEN                 ((uint32_t)0x10000000)
#define  RCC_APB1LLPENR_DACLPEN                 ((uint32_t)0x20000000)
#define  RCC_APB1LLPENR_OPAMPLPEN               ((uint32_t)0x40000000)
#define  RCC_APB1LLPENR_LPTIM1LPEN              ((uint32_t)0x80000000)

/********************  Bit definition for RCC_APB1HLPENR register  *************/
#define  RCC_APB1HLPENR_LPUARTLPEN               ((uint32_t)0x00000001)
#define  RCC_APB1HLPENR_SWPLPEN                  ((uint32_t)0x00000004)
#define  RCC_APB1HLPENR_LPTIM2LPEN               ((uint32_t)0x00000020)
/********************  Bit definition for RCC_APB2LPENR register  *************/
#define  RCC_APB2LPENR_SYSCFGLPEN                ((uint32_t)0x00000001)
#define  RCC_APB2LPENR_SDMMCLPEN                 ((uint32_t)0x00000400)
#define  RCC_APB2LPENR_TIM1LPEN                  ((uint32_t)0x00000800)
#define  RCC_APB2LPENR_SPI1LPEN                  ((uint32_t)0x00001000)
#define  RCC_APB2LPENR_TIM8LPEN                  ((uint32_t)0x00002000)
#define  RCC_APB2LPENR_USART1LPEN                ((uint32_t)0x00004000)
#define  RCC_APB2LPENR_TIM15LPEN                 ((uint32_t)0x00010000)
#define  RCC_APB2LPENR_TIM16LPEN                 ((uint32_t)0x00020000)
#define  RCC_APB2LPENR_TIM17LPEN                 ((uint32_t)0x00040000)
#define  RCC_APB2LPENR_SAI1LPEN                  ((uint32_t)0x00200000)
#define  RCC_APB2LPENR_SAI2LPEN                  ((uint32_t)0x00400000)
#define  RCC_APB2LPENR_DFSDMLPEN                 ((uint32_t)0x01000000)
/********************  Bit definition for RCC_CCIPR register  ******************/
#define  RCC_CCIPR_USART1SEL                     ((uint32_t)0x00000003)
#define  RCC_CCIPR_USART1SEL_0                   ((uint32_t)0x00000001)
#define  RCC_CCIPR_USART1SEL_1                   ((uint32_t)0x00000002)

#define  RCC_CCIPR_USART2SEL                     ((uint32_t)0x0000000C)
#define  RCC_CCIPR_USART2SEL_0                   ((uint32_t)0x00000004)
#define  RCC_CCIPR_USART2SEL_1                   ((uint32_t)0x00000008)

#define  RCC_CCIPR_USART3SEL                     ((uint32_t)0x00000030)
#define  RCC_CCIPR_USART3SEL_0                   ((uint32_t)0x00000010)
#define  RCC_CCIPR_USART3SEL_1                   ((uint32_t)0x00000020)

#define  RCC_CCIPR_UART4SEL                      ((uint32_t)0x000000C0)
#define  RCC_CCIPR_UART4SEL_0                    ((uint32_t)0x00000040)
#define  RCC_CCIPR_UART4SEL_1                    ((uint32_t)0x00000080)

#define  RCC_CCIPR_UART5SEL                      ((uint32_t)0x00000300)
#define  RCC_CCIPR_UART5SEL_0                    ((uint32_t)0x00000100)
#define  RCC_CCIPR_UART5SEL_1                    ((uint32_t)0x00000200)

#define  RCC_CCIPR_LPUARTSEL                     ((uint32_t)0x00000C00)
#define  RCC_CCIPR_LPUARTSEL_0                   ((uint32_t)0x00000400)
#define  RCC_CCIPR_LPUARTSEL_1                   ((uint32_t)0x00000800)

#define  RCC_CCIPR_I2C1SEL                       ((uint32_t)0x00003000)
#define  RCC_CCIPR_I2C1SEL_0                     ((uint32_t)0x00001000)
#define  RCC_CCIPR_I2C1SEL_1                     ((uint32_t)0x00002000)

#define  RCC_CCIPR_I2C2SEL                       ((uint32_t)0x0000C000)
#define  RCC_CCIPR_I2C2SEL_0                     ((uint32_t)0x00004000)
#define  RCC_CCIPR_I2C2SEL_1                     ((uint32_t)0x00008000)

#define  RCC_CCIPR_I2C3SEL                       ((uint32_t)0x00030000)
#define  RCC_CCIPR_I2C3SEL_0                     ((uint32_t)0x00010000)
#define  RCC_CCIPR_I2C3SEL_1                     ((uint32_t)0x00020000)

#define  RCC_CCIPR_LPTIM1SEL                     ((uint32_t)0x000C0000)
#define  RCC_CCIPR_LPTIM1SEL_0                   ((uint32_t)0x00040000)
#define  RCC_CCIPR_LPTIM1SEL_1                   ((uint32_t)0x00080000)

#define  RCC_CCIPR_LPTIM2SEL                     ((uint32_t)0x00300000)
#define  RCC_CCIPR_LPTIM2SEL_0                   ((uint32_t)0x00100000)
#define  RCC_CCIPR_LPTIM2SEL_1                   ((uint32_t)0x00200000)

#define  RCC_CCIPR_SAI1SEL                       ((uint32_t)0x00C00000)
#define  RCC_CCIPR_SAI1SEL_0                     ((uint32_t)0x00400000)
#define  RCC_CCIPR_SAI1SEL_1                     ((uint32_t)0x00800000)

#define  RCC_CCIPR_SAI2SEL                       ((uint32_t)0x03000000)
#define  RCC_CCIPR_SAI2SEL_0                     ((uint32_t)0x01000000)
#define  RCC_CCIPR_SAI2SEL_1                     ((uint32_t)0x02000000)

#define  RCC_CCIPR_CLK48SEL                      ((uint32_t)0x0C000000)
#define  RCC_CCIPR_CLK48SEL_0                    ((uint32_t)0x04000000)
#define  RCC_CCIPR_CLK48SEL_1                    ((uint32_t)0x08000000)

#define  RCC_CCIPR_ADCSEL                        ((uint32_t)0x30000000)
#define  RCC_CCIPR_ADCSEL_0                      ((uint32_t)0x10000000)
#define  RCC_CCIPR_ADCSEL_1                      ((uint32_t)0x20000000)

#define  RCC_CCIPR_SWPSEL                        ((uint32_t)0x40000000)
#define  RCC_CCIPR_DFSDMSEL                      ((uint32_t)0x80000000)


/********************  Bit definition for RCC_BDCR register  ******************/
#define  RCC_BDCR_LSEON                       ((uint32_t)0x00000001)
#define  RCC_BDCR_LSERDY                      ((uint32_t)0x00000002)
#define  RCC_BDCR_LSEBYP                      ((uint32_t)0x00000004)
#define  RCC_BDCR_LSECSSON                    ((uint32_t)0x00000008)
#define  RCC_BDCR_LSECSSD                     ((uint32_t)0x00000010)
#define  RCC_BDCR_RTCSEL                      ((uint32_t)0x00000300)
#define  RCC_BDCR_RTCSEL_0                    ((uint32_t)0x00000100)
#define  RCC_BDCR_RTCSEL_1                    ((uint32_t)0x00000200)
#define  RCC_BDCR_RTCEN                       ((uint32_t)0x00008000)
#define  RCC_BDCR_BDRST                       ((uint32_t)0x00010000)
#define  RCC_BDCR_LSCOEN                      ((uint32_t)0x01000000)
#define  RCC_BDCR_LSCOSEL                     ((uint32_t)0x02000000)

/********************  Bit definition for RCC_CSR register  *******************/
#define  RCC_CSR_LSION                       ((uint32_t)0x00000001)
#define  RCC_CSR_LSIRDY                      ((uint32_t)0x00000002)
#define  RCC_CSR_MSIRANGE                    ((uint32_t)0x00000F00)           
#define  RCC_CSR_MSIRANGE_1                  ((uint32_t)0x00000400)             /*!< MSI frequency 1MHZ */
#define  RCC_CSR_MSIRANGE_2                  ((uint32_t)0x00000500)             /*!< MSI frequency 2MHZ */
#define  RCC_CSR_MSIRANGE_4                  ((uint32_t)0x00000600)             /*!< The default frequency 4MHZ */
#define  RCC_CSR_MSIRANGE_8                  ((uint32_t)0x00000700)             /*!< MSI frequency 8MHZ */

#define  RCC_CSR_RMVF                        ((uint32_t)0x00800000)
#define  RCC_CSR_FIREWALLRSTF                ((uint32_t)0x01000000)
#define  RCC_CSR_OBLRSTF                     ((uint32_t)0x02000000)
#define  RCC_CSR_PADRSTF                     ((uint32_t)0x04000000)
#define  RCC_CSR_BORRSTF                     ((uint32_t)0x08000000)
#define  RCC_CSR_SFTRSTF                     ((uint32_t)0x10000000)
#define  RCC_CSR_WDGRSTF                     ((uint32_t)0x20000000)
#define  RCC_CSR_WWDGRSTF                    ((uint32_t)0x40000000)
#define  RCC_CSR_LPWRRSTF                    ((uint32_t)0x80000000)


/********************  Bit definition for RCC_PLLSAI1CFGR register  ************/
#define  RCC_PLLSAI1CFGR_PLLSAI1N              ((uint32_t)0x00007F00)
#define  RCC_PLLSAI1CFGR_PLLSAI1P              ((uint32_t)0x00020000)
#define  RCC_PLLSAI1CFGR_PLLSAI1Q              ((uint32_t)0x00600000)
#define  RCC_PLLSAI1CFGR_PLLSAI1R              ((uint32_t)0x06000000)
#define  RCC_PLLSAI1CFGR_PLLSAI1PEN            ((uint32_t)0x00010000)
#define  RCC_PLLSAI1CFGR_PLLSAI1QEN            ((uint32_t)0x00100000)
#define  RCC_PLLSAI1CFGR_PLLSAI1REN            ((uint32_t)0x01000000)


/********************  Bit definition for RCC_PLLSAI2CFGR register  ************/
#define  RCC_PLLSAI2CFGR_PLLSAI2N              ((uint32_t)0x00007F00)
#define  RCC_PLLSAI1CFGR_PLLSAI2P              ((uint32_t)0x00020000)
#define  RCC_PLLSAI2CFGR_PLLSAI2Q              ((uint32_t)0x00600000)
#define  RCC_PLLSAI2CFGR_PLLSAI2R              ((uint32_t)0x06000000)
#define  RCC_PLLSAI1CFGR_PLLSAI2PEN            ((uint32_t)0x00010000)
#define  RCC_PLLSAI1CFGR_PLLSAI2QEN            ((uint32_t)0x00100000)
#define  RCC_PLLSAI1CFGR_PLLSAI2REN            ((uint32_t)0x01000000)



/******************************************************************************/
/*                                                                            */
/*                                    RNG                                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RNG_CR register  *******************/
#define RNG_CR_RNGEN                         ((uint32_t)0x00000004)
#define RNG_CR_IE                            ((uint32_t)0x00000008)

/********************  Bits definition for RNG_SR register  *******************/
#define RNG_SR_DRDY                          ((uint32_t)0x00000001)
#define RNG_SR_CECS                          ((uint32_t)0x00000002)
#define RNG_SR_SECS                          ((uint32_t)0x00000004)
#define RNG_SR_CEIS                          ((uint32_t)0x00000020)
#define RNG_SR_SEIS                          ((uint32_t)0x00000040)

/******************************************************************************/
/*                                                                            */
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RTC_TR register  *******************/
#define RTC_TR_PM                            ((uint32_t)0x00400000)
#define RTC_TR_HT                            ((uint32_t)0x00300000)
#define RTC_TR_HT_0                          ((uint32_t)0x00100000)
#define RTC_TR_HT_1                          ((uint32_t)0x00200000)
#define RTC_TR_HU                            ((uint32_t)0x000F0000)
#define RTC_TR_HU_0                          ((uint32_t)0x00010000)
#define RTC_TR_HU_1                          ((uint32_t)0x00020000)
#define RTC_TR_HU_2                          ((uint32_t)0x00040000)
#define RTC_TR_HU_3                          ((uint32_t)0x00080000)
#define RTC_TR_MNT                           ((uint32_t)0x00007000)
#define RTC_TR_MNT_0                         ((uint32_t)0x00001000)
#define RTC_TR_MNT_1                         ((uint32_t)0x00002000)
#define RTC_TR_MNT_2                         ((uint32_t)0x00004000)
#define RTC_TR_MNU                           ((uint32_t)0x00000F00)
#define RTC_TR_MNU_0                         ((uint32_t)0x00000100)
#define RTC_TR_MNU_1                         ((uint32_t)0x00000200)
#define RTC_TR_MNU_2                         ((uint32_t)0x00000400)
#define RTC_TR_MNU_3                         ((uint32_t)0x00000800)
#define RTC_TR_ST                            ((uint32_t)0x00000070)
#define RTC_TR_ST_0                          ((uint32_t)0x00000010)
#define RTC_TR_ST_1                          ((uint32_t)0x00000020)
#define RTC_TR_ST_2                          ((uint32_t)0x00000040)
#define RTC_TR_SU                            ((uint32_t)0x0000000F)
#define RTC_TR_SU_0                          ((uint32_t)0x00000001)
#define RTC_TR_SU_1                          ((uint32_t)0x00000002)
#define RTC_TR_SU_2                          ((uint32_t)0x00000004)
#define RTC_TR_SU_3                          ((uint32_t)0x00000008)

/********************  Bits definition for RTC_DR register  *******************/
#define RTC_DR_YT                            ((uint32_t)0x00F00000)
#define RTC_DR_YT_0                          ((uint32_t)0x00100000)
#define RTC_DR_YT_1                          ((uint32_t)0x00200000)
#define RTC_DR_YT_2                          ((uint32_t)0x00400000)
#define RTC_DR_YT_3                          ((uint32_t)0x00800000)
#define RTC_DR_YU                            ((uint32_t)0x000F0000)
#define RTC_DR_YU_0                          ((uint32_t)0x00010000)
#define RTC_DR_YU_1                          ((uint32_t)0x00020000)
#define RTC_DR_YU_2                          ((uint32_t)0x00040000)
#define RTC_DR_YU_3                          ((uint32_t)0x00080000)
#define RTC_DR_WDU                           ((uint32_t)0x0000E000)
#define RTC_DR_WDU_0                         ((uint32_t)0x00002000)
#define RTC_DR_WDU_1                         ((uint32_t)0x00004000)
#define RTC_DR_WDU_2                         ((uint32_t)0x00008000)
#define RTC_DR_MT                            ((uint32_t)0x00001000)
#define RTC_DR_MU                            ((uint32_t)0x00000F00)
#define RTC_DR_MU_0                          ((uint32_t)0x00000100)
#define RTC_DR_MU_1                          ((uint32_t)0x00000200)
#define RTC_DR_MU_2                          ((uint32_t)0x00000400)
#define RTC_DR_MU_3                          ((uint32_t)0x00000800)
#define RTC_DR_DT                            ((uint32_t)0x00000030)
#define RTC_DR_DT_0                          ((uint32_t)0x00000010)
#define RTC_DR_DT_1                          ((uint32_t)0x00000020)
#define RTC_DR_DU                            ((uint32_t)0x0000000F)
#define RTC_DR_DU_0                          ((uint32_t)0x00000001)
#define RTC_DR_DU_1                          ((uint32_t)0x00000002)
#define RTC_DR_DU_2                          ((uint32_t)0x00000004)
#define RTC_DR_DU_3                          ((uint32_t)0x00000008)

/********************  Bits definition for RTC_CR register  *******************/
#define RTC_CR_ITSE                          ((uint32_t)0x01000000) 
#define RTC_CR_COE                           ((uint32_t)0x00800000)
#define RTC_CR_OSEL                          ((uint32_t)0x00600000)
#define RTC_CR_OSEL_0                        ((uint32_t)0x00200000)
#define RTC_CR_OSEL_1                        ((uint32_t)0x00400000)
#define RTC_CR_POL                           ((uint32_t)0x00100000)
#define RTC_CR_COSEL                         ((uint32_t)0x00080000)
#define RTC_CR_BCK                           ((uint32_t)0x00040000)
#define RTC_CR_SUB1H                         ((uint32_t)0x00020000)
#define RTC_CR_ADD1H                         ((uint32_t)0x00010000)
#define RTC_CR_TSIE                          ((uint32_t)0x00008000)
#define RTC_CR_WUTIE                         ((uint32_t)0x00004000)
#define RTC_CR_ALRBIE                        ((uint32_t)0x00002000)
#define RTC_CR_ALRAIE                        ((uint32_t)0x00001000)
#define RTC_CR_TSE                           ((uint32_t)0x00000800)
#define RTC_CR_WUTE                          ((uint32_t)0x00000400)
#define RTC_CR_ALRBE                         ((uint32_t)0x00000200)
#define RTC_CR_ALRAE                         ((uint32_t)0x00000100)
#define RTC_CR_FMT                           ((uint32_t)0x00000040)
#define RTC_CR_BYPSHAD                       ((uint32_t)0x00000020)
#define RTC_CR_REFCKON                       ((uint32_t)0x00000010)
#define RTC_CR_TSEDGE                        ((uint32_t)0x00000008)
#define RTC_CR_WUCKSEL                       ((uint32_t)0x00000007)
#define RTC_CR_WUCKSEL_0                     ((uint32_t)0x00000001)
#define RTC_CR_WUCKSEL_1                     ((uint32_t)0x00000002)
#define RTC_CR_WUCKSEL_2                     ((uint32_t)0x00000004)

/********************  Bits definition for RTC_ISR register  ******************/
#define RTC_ISR_ITSF                         ((uint32_t)0x00020000)
#define RTC_ISR_RECALPF                      ((uint32_t)0x00010000)
#define RTC_ISR_TAMP3F                       ((uint32_t)0x00008000)
#define RTC_ISR_TAMP2F                       ((uint32_t)0x00004000)
#define RTC_ISR_TAMP1F                       ((uint32_t)0x00002000)
#define RTC_ISR_TSOVF                        ((uint32_t)0x00001000)
#define RTC_ISR_TSF                          ((uint32_t)0x00000800)
#define RTC_ISR_WUTF                         ((uint32_t)0x00000400)
#define RTC_ISR_ALRBF                        ((uint32_t)0x00000200)
#define RTC_ISR_ALRAF                        ((uint32_t)0x00000100)
#define RTC_ISR_INIT                         ((uint32_t)0x00000080)
#define RTC_ISR_INITF                        ((uint32_t)0x00000040)
#define RTC_ISR_RSF                          ((uint32_t)0x00000020)
#define RTC_ISR_INITS                        ((uint32_t)0x00000010)
#define RTC_ISR_SHPF                         ((uint32_t)0x00000008)
#define RTC_ISR_WUTWF                        ((uint32_t)0x00000004)
#define RTC_ISR_ALRBWF                       ((uint32_t)0x00000002)
#define RTC_ISR_ALRAWF                       ((uint32_t)0x00000001)

/********************  Bits definition for RTC_PRER register  *****************/
#define RTC_PRER_PREDIV_A                    ((uint32_t)0x007F0000)
#define RTC_PRER_PREDIV_S                    ((uint32_t)0x00007FFF)

/********************  Bits definition for RTC_WUTR register  *****************/
#define RTC_WUTR_WUT                         ((uint32_t)0x0000FFFF)

/********************  Bits definition for RTC_ALRMAR register  ***************/
#define RTC_ALRMAR_MSK4                      ((uint32_t)0x80000000)
#define RTC_ALRMAR_WDSEL                     ((uint32_t)0x40000000)
#define RTC_ALRMAR_DT                        ((uint32_t)0x30000000)
#define RTC_ALRMAR_DT_0                      ((uint32_t)0x10000000)
#define RTC_ALRMAR_DT_1                      ((uint32_t)0x20000000)
#define RTC_ALRMAR_DU                        ((uint32_t)0x0F000000)
#define RTC_ALRMAR_DU_0                      ((uint32_t)0x01000000)
#define RTC_ALRMAR_DU_1                      ((uint32_t)0x02000000)
#define RTC_ALRMAR_DU_2                      ((uint32_t)0x04000000)
#define RTC_ALRMAR_DU_3                      ((uint32_t)0x08000000)
#define RTC_ALRMAR_MSK3                      ((uint32_t)0x00800000)
#define RTC_ALRMAR_PM                        ((uint32_t)0x00400000)
#define RTC_ALRMAR_HT                        ((uint32_t)0x00300000)
#define RTC_ALRMAR_HT_0                      ((uint32_t)0x00100000)
#define RTC_ALRMAR_HT_1                      ((uint32_t)0x00200000)
#define RTC_ALRMAR_HU                        ((uint32_t)0x000F0000)
#define RTC_ALRMAR_HU_0                      ((uint32_t)0x00010000)
#define RTC_ALRMAR_HU_1                      ((uint32_t)0x00020000)
#define RTC_ALRMAR_HU_2                      ((uint32_t)0x00040000)
#define RTC_ALRMAR_HU_3                      ((uint32_t)0x00080000)
#define RTC_ALRMAR_MSK2                      ((uint32_t)0x00008000)
#define RTC_ALRMAR_MNT                       ((uint32_t)0x00007000)
#define RTC_ALRMAR_MNT_0                     ((uint32_t)0x00001000)
#define RTC_ALRMAR_MNT_1                     ((uint32_t)0x00002000)
#define RTC_ALRMAR_MNT_2                     ((uint32_t)0x00004000)
#define RTC_ALRMAR_MNU                       ((uint32_t)0x00000F00)
#define RTC_ALRMAR_MNU_0                     ((uint32_t)0x00000100)
#define RTC_ALRMAR_MNU_1                     ((uint32_t)0x00000200)
#define RTC_ALRMAR_MNU_2                     ((uint32_t)0x00000400)
#define RTC_ALRMAR_MNU_3                     ((uint32_t)0x00000800)
#define RTC_ALRMAR_MSK1                      ((uint32_t)0x00000080)
#define RTC_ALRMAR_ST                        ((uint32_t)0x00000070)
#define RTC_ALRMAR_ST_0                      ((uint32_t)0x00000010)
#define RTC_ALRMAR_ST_1                      ((uint32_t)0x00000020)
#define RTC_ALRMAR_ST_2                      ((uint32_t)0x00000040)
#define RTC_ALRMAR_SU                        ((uint32_t)0x0000000F)
#define RTC_ALRMAR_SU_0                      ((uint32_t)0x00000001)
#define RTC_ALRMAR_SU_1                      ((uint32_t)0x00000002)
#define RTC_ALRMAR_SU_2                      ((uint32_t)0x00000004)
#define RTC_ALRMAR_SU_3                      ((uint32_t)0x00000008)

/********************  Bits definition for RTC_ALRMBR register  ***************/
#define RTC_ALRMBR_MSK4                      ((uint32_t)0x80000000)
#define RTC_ALRMBR_WDSEL                     ((uint32_t)0x40000000)
#define RTC_ALRMBR_DT                        ((uint32_t)0x30000000)
#define RTC_ALRMBR_DT_0                      ((uint32_t)0x10000000)
#define RTC_ALRMBR_DT_1                      ((uint32_t)0x20000000)
#define RTC_ALRMBR_DU                        ((uint32_t)0x0F000000)
#define RTC_ALRMBR_DU_0                      ((uint32_t)0x01000000)
#define RTC_ALRMBR_DU_1                      ((uint32_t)0x02000000)
#define RTC_ALRMBR_DU_2                      ((uint32_t)0x04000000)
#define RTC_ALRMBR_DU_3                      ((uint32_t)0x08000000)
#define RTC_ALRMBR_MSK3                      ((uint32_t)0x00800000)
#define RTC_ALRMBR_PM                        ((uint32_t)0x00400000)
#define RTC_ALRMBR_HT                        ((uint32_t)0x00300000)
#define RTC_ALRMBR_HT_0                      ((uint32_t)0x00100000)
#define RTC_ALRMBR_HT_1                      ((uint32_t)0x00200000)
#define RTC_ALRMBR_HU                        ((uint32_t)0x000F0000)
#define RTC_ALRMBR_HU_0                      ((uint32_t)0x00010000)
#define RTC_ALRMBR_HU_1                      ((uint32_t)0x00020000)
#define RTC_ALRMBR_HU_2                      ((uint32_t)0x00040000)
#define RTC_ALRMBR_HU_3                      ((uint32_t)0x00080000)
#define RTC_ALRMBR_MSK2                      ((uint32_t)0x00008000)
#define RTC_ALRMBR_MNT                       ((uint32_t)0x00007000)
#define RTC_ALRMBR_MNT_0                     ((uint32_t)0x00001000)
#define RTC_ALRMBR_MNT_1                     ((uint32_t)0x00002000)
#define RTC_ALRMBR_MNT_2                     ((uint32_t)0x00004000)
#define RTC_ALRMBR_MNU                       ((uint32_t)0x00000F00)
#define RTC_ALRMBR_MNU_0                     ((uint32_t)0x00000100)
#define RTC_ALRMBR_MNU_1                     ((uint32_t)0x00000200)
#define RTC_ALRMBR_MNU_2                     ((uint32_t)0x00000400)
#define RTC_ALRMBR_MNU_3                     ((uint32_t)0x00000800)
#define RTC_ALRMBR_MSK1                      ((uint32_t)0x00000080)
#define RTC_ALRMBR_ST                        ((uint32_t)0x00000070)
#define RTC_ALRMBR_ST_0                      ((uint32_t)0x00000010)
#define RTC_ALRMBR_ST_1                      ((uint32_t)0x00000020)
#define RTC_ALRMBR_ST_2                      ((uint32_t)0x00000040)
#define RTC_ALRMBR_SU                        ((uint32_t)0x0000000F)
#define RTC_ALRMBR_SU_0                      ((uint32_t)0x00000001)
#define RTC_ALRMBR_SU_1                      ((uint32_t)0x00000002)
#define RTC_ALRMBR_SU_2                      ((uint32_t)0x00000004)
#define RTC_ALRMBR_SU_3                      ((uint32_t)0x00000008)

/********************  Bits definition for RTC_WPR register  ******************/
#define RTC_WPR_KEY                          ((uint32_t)0x000000FF)

/********************  Bits definition for RTC_SSR register  ******************/
#define RTC_SSR_SS                           ((uint32_t)0x0000FFFF)

/********************  Bits definition for RTC_SHIFTR register  ***************/
#define RTC_SHIFTR_SUBFS                     ((uint32_t)0x00007FFF)
#define RTC_SHIFTR_ADD1S                     ((uint32_t)0x80000000)

/********************  Bits definition for RTC_TSTR register  *****************/
#define RTC_TSTR_PM                          ((uint32_t)0x00400000)
#define RTC_TSTR_HT                          ((uint32_t)0x00300000)
#define RTC_TSTR_HT_0                        ((uint32_t)0x00100000)
#define RTC_TSTR_HT_1                        ((uint32_t)0x00200000)
#define RTC_TSTR_HU                          ((uint32_t)0x000F0000)
#define RTC_TSTR_HU_0                        ((uint32_t)0x00010000)
#define RTC_TSTR_HU_1                        ((uint32_t)0x00020000)
#define RTC_TSTR_HU_2                        ((uint32_t)0x00040000)
#define RTC_TSTR_HU_3                        ((uint32_t)0x00080000)
#define RTC_TSTR_MNT                         ((uint32_t)0x00007000)
#define RTC_TSTR_MNT_0                       ((uint32_t)0x00001000)
#define RTC_TSTR_MNT_1                       ((uint32_t)0x00002000)
#define RTC_TSTR_MNT_2                       ((uint32_t)0x00004000)
#define RTC_TSTR_MNU                         ((uint32_t)0x00000F00)
#define RTC_TSTR_MNU_0                       ((uint32_t)0x00000100)
#define RTC_TSTR_MNU_1                       ((uint32_t)0x00000200)
#define RTC_TSTR_MNU_2                       ((uint32_t)0x00000400)
#define RTC_TSTR_MNU_3                       ((uint32_t)0x00000800)
#define RTC_TSTR_ST                          ((uint32_t)0x00000070)
#define RTC_TSTR_ST_0                        ((uint32_t)0x00000010)
#define RTC_TSTR_ST_1                        ((uint32_t)0x00000020)
#define RTC_TSTR_ST_2                        ((uint32_t)0x00000040)
#define RTC_TSTR_SU                          ((uint32_t)0x0000000F)
#define RTC_TSTR_SU_0                        ((uint32_t)0x00000001)
#define RTC_TSTR_SU_1                        ((uint32_t)0x00000002)
#define RTC_TSTR_SU_2                        ((uint32_t)0x00000004)
#define RTC_TSTR_SU_3                        ((uint32_t)0x00000008)

/********************  Bits definition for RTC_TSDR register  *****************/
#define RTC_TSDR_WDU                         ((uint32_t)0x0000E000)
#define RTC_TSDR_WDU_0                       ((uint32_t)0x00002000)
#define RTC_TSDR_WDU_1                       ((uint32_t)0x00004000)
#define RTC_TSDR_WDU_2                       ((uint32_t)0x00008000)
#define RTC_TSDR_MT                          ((uint32_t)0x00001000)
#define RTC_TSDR_MU                          ((uint32_t)0x00000F00)
#define RTC_TSDR_MU_0                        ((uint32_t)0x00000100)
#define RTC_TSDR_MU_1                        ((uint32_t)0x00000200)
#define RTC_TSDR_MU_2                        ((uint32_t)0x00000400)
#define RTC_TSDR_MU_3                        ((uint32_t)0x00000800)
#define RTC_TSDR_DT                          ((uint32_t)0x00000030)
#define RTC_TSDR_DT_0                        ((uint32_t)0x00000010)
#define RTC_TSDR_DT_1                        ((uint32_t)0x00000020)
#define RTC_TSDR_DU                          ((uint32_t)0x0000000F)
#define RTC_TSDR_DU_0                        ((uint32_t)0x00000001)
#define RTC_TSDR_DU_1                        ((uint32_t)0x00000002)
#define RTC_TSDR_DU_2                        ((uint32_t)0x00000004)
#define RTC_TSDR_DU_3                        ((uint32_t)0x00000008)

/********************  Bits definition for RTC_TSSSR register  ****************/
#define RTC_TSSSR_SS                         ((uint32_t)0x0000FFFF)

/********************  Bits definition for RTC_CAL register  *****************/
#define RTC_CALR_CALP                        ((uint32_t)0x00008000)
#define RTC_CALR_CALW8                       ((uint32_t)0x00004000)
#define RTC_CALR_CALW16                      ((uint32_t)0x00002000)
#define RTC_CALR_CALM                        ((uint32_t)0x000001FF)
#define RTC_CALR_CALM_0                      ((uint32_t)0x00000001)
#define RTC_CALR_CALM_1                      ((uint32_t)0x00000002)
#define RTC_CALR_CALM_2                      ((uint32_t)0x00000004)
#define RTC_CALR_CALM_3                      ((uint32_t)0x00000008)
#define RTC_CALR_CALM_4                      ((uint32_t)0x00000010)
#define RTC_CALR_CALM_5                      ((uint32_t)0x00000020)
#define RTC_CALR_CALM_6                      ((uint32_t)0x00000040)
#define RTC_CALR_CALM_7                      ((uint32_t)0x00000080)
#define RTC_CALR_CALM_8                      ((uint32_t)0x00000100)

/********************  Bits definition for RTC_TAFCR register  ****************/
#define RTC_TAFCR_TAMP3MF                    ((uint32_t)0x01000000)
#define RTC_TAFCR_TAMP3NOERASE               ((uint32_t)0x00800000)
#define RTC_TAFCR_TAMP3IE                    ((uint32_t)0x00400000)
#define RTC_TAFCR_TAMP2MF                    ((uint32_t)0x00200000)
#define RTC_TAFCR_TAMP2NOERASE               ((uint32_t)0x00100000)
#define RTC_TAFCR_TAMP2IE                    ((uint32_t)0x00080000)
#define RTC_TAFCR_TAMP1MF                    ((uint32_t)0x00040000)
#define RTC_TAFCR_TAMP1NOERASE               ((uint32_t)0x00020000)
#define RTC_TAFCR_TAMP1IE                    ((uint32_t)0x00010000)
#define RTC_TAFCR_TAMPPUDIS                  ((uint32_t)0x00008000)
#define RTC_TAFCR_TAMPPRCH                   ((uint32_t)0x00006000)
#define RTC_TAFCR_TAMPPRCH_0                 ((uint32_t)0x00002000)
#define RTC_TAFCR_TAMPPRCH_1                 ((uint32_t)0x00004000)
#define RTC_TAFCR_TAMPFLT                    ((uint32_t)0x00001800)
#define RTC_TAFCR_TAMPFLT_0                  ((uint32_t)0x00000800)
#define RTC_TAFCR_TAMPFLT_1                  ((uint32_t)0x00001000)
#define RTC_TAFCR_TAMPFREQ                   ((uint32_t)0x00000700)
#define RTC_TAFCR_TAMPFREQ_0                 ((uint32_t)0x00000100)
#define RTC_TAFCR_TAMPFREQ_1                 ((uint32_t)0x00000200)
#define RTC_TAFCR_TAMPFREQ_2                 ((uint32_t)0x00000400)
#define RTC_TAFCR_TAMPTS                     ((uint32_t)0x00000080)
#define RTC_TAFCR_TAMP3_TRG                   ((uint32_t)0x00000040)
#define RTC_TAFCR_TAMP3E                     ((uint32_t)0x00000020)
#define RTC_TAFCR_TAMP2_TRG                   ((uint32_t)0x00000010)
#define RTC_TAFCR_TAMP2E                     ((uint32_t)0x00000008)
#define RTC_TAFCR_TAMPIE                     ((uint32_t)0x00000004)
#define RTC_TAFCR_TAMP1_TRG                   ((uint32_t)0x00000002)
#define RTC_TAFCR_TAMP1E                     ((uint32_t)0x00000001)

/********************  Bits definition for RTC_ALRMASSR register  *************/
#define RTC_ALRMASSR_MASKSS                  ((uint32_t)0x0F000000)
#define RTC_ALRMASSR_MASKSS_0                ((uint32_t)0x01000000)
#define RTC_ALRMASSR_MASKSS_1                ((uint32_t)0x02000000)
#define RTC_ALRMASSR_MASKSS_2                ((uint32_t)0x04000000)
#define RTC_ALRMASSR_MASKSS_3                ((uint32_t)0x08000000)
#define RTC_ALRMASSR_SS                      ((uint32_t)0x00007FFF)

/********************  Bits definition for RTC_ALRMBSSR register  *************/
#define RTC_ALRMBSSR_MASKSS                  ((uint32_t)0x0F000000)
#define RTC_ALRMBSSR_MASKSS_0                ((uint32_t)0x01000000)
#define RTC_ALRMBSSR_MASKSS_1                ((uint32_t)0x02000000)
#define RTC_ALRMBSSR_MASKSS_2                ((uint32_t)0x04000000)
#define RTC_ALRMBSSR_MASKSS_3                ((uint32_t)0x08000000)
#define RTC_ALRMBSSR_SS                      ((uint32_t)0x00007FFF)

/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_OR_OUT_RMP                      ((uint32_t)0x00000002)
#define RTC_OR_ALARMOUTTYPE                 ((uint32_t)0x00000001)


/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_BKP0R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP1R register  ****************/
#define RTC_BKP1R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP2R register  ****************/
#define RTC_BKP2R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP3R register  ****************/
#define RTC_BKP3R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP4R register  ****************/
#define RTC_BKP4R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP5R register  ****************/
#define RTC_BKP5R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP6R register  ****************/
#define RTC_BKP6R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP7R register  ****************/
#define RTC_BKP7R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP8R register  ****************/
#define RTC_BKP8R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP9R register  ****************/
#define RTC_BKP9R                            ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP10R register  ***************/
#define RTC_BKP10R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP11R register  ***************/
#define RTC_BKP11R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP12R register  ***************/
#define RTC_BKP12R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP13R register  ***************/
#define RTC_BKP13R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP14R register  ***************/
#define RTC_BKP14R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP15R register  ***************/
#define RTC_BKP15R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP16R register  ***************/
#define RTC_BKP16R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP17R register  ***************/
#define RTC_BKP17R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP18R register  ***************/
#define RTC_BKP18R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP19R register  ***************/
#define RTC_BKP19R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP20R register  ***************/
#define RTC_BKP20R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP21R register  ***************/
#define RTC_BKP21R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP22R register  ***************/
#define RTC_BKP22R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP23R register  ***************/
#define RTC_BKP23R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP24R register  ***************/
#define RTC_BKP24R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP25R register  ***************/
#define RTC_BKP25R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP26R register  ***************/
#define RTC_BKP26R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP27R register  ***************/
#define RTC_BKP27R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP28R register  ***************/
#define RTC_BKP28R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP29R register  ***************/
#define RTC_BKP29R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP30R register  ***************/
#define RTC_BKP30R                           ((uint32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP31R register  ***************/
#define RTC_BKP31R                           ((uint32_t)0xFFFFFFFF)

/******************************************************************************/
/*                                                                            */
/*                          Serial Audio Interface                            */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for SAI_GCR register  *******************/
#define  SAI_GCR_SYNCIN                  ((uint32_t)0x00000003)        /*!<SYNCIN[1:0] bits (Synchronization Inputs)   */
#define  SAI_GCR_SYNCIN_0                ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  SAI_GCR_SYNCIN_1                ((uint32_t)0x00000002)        /*!<Bit 1 */

#define  SAI_GCR_SYNCOUT                 ((uint32_t)0x00000030)        /*!<SYNCOUT[1:0] bits (Synchronization Outputs) */
#define  SAI_GCR_SYNCOUT_0               ((uint32_t)0x00000010)        /*!<Bit 0 */
#define  SAI_GCR_SYNCOUT_1               ((uint32_t)0x00000020)        /*!<Bit 1 */

/*******************  Bit definition for SAI_xCR1 register  *******************/
#define  SAI_xCR1_MODE                    ((uint32_t)0x00000003)        /*!<MODE[1:0] bits (Audio Block Mode)           */
#define  SAI_xCR1_MODE_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  SAI_xCR1_MODE_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */

#define  SAI_xCR1_PRTCFG                  ((uint32_t)0x0000000C)        /*!<PRTCFG[1:0] bits (Protocol Configuration)   */
#define  SAI_xCR1_PRTCFG_0                ((uint32_t)0x00000004)        /*!<Bit 0 */
#define  SAI_xCR1_PRTCFG_1                ((uint32_t)0x00000008)        /*!<Bit 1 */

#define  SAI_xCR1_DS                      ((uint32_t)0x000000E0)        /*!<DS[1:0] bits (Data Size) */
#define  SAI_xCR1_DS_0                    ((uint32_t)0x00000020)        /*!<Bit 0 */
#define  SAI_xCR1_DS_1                    ((uint32_t)0x00000040)        /*!<Bit 1 */
#define  SAI_xCR1_DS_2                    ((uint32_t)0x00000080)        /*!<Bit 2 */

#define  SAI_xCR1_LSBFIRST                ((uint32_t)0x00000100)        /*!<LSB First Configuration  */
#define  SAI_xCR1_CKSTR                   ((uint32_t)0x00000200)        /*!<ClocK STRobing edge      */

#define  SAI_xCR1_SYNCEN                  ((uint32_t)0x00000C00)        /*!<SYNCEN[1:0](SYNChronization ENable) */
#define  SAI_xCR1_SYNCEN_0                ((uint32_t)0x00000400)        /*!<Bit 0 */
#define  SAI_xCR1_SYNCEN_1                ((uint32_t)0x00000800)        /*!<Bit 1 */

#define  SAI_xCR1_MONO                    ((uint32_t)0x00001000)        /*!<Mono mode                  */
#define  SAI_xCR1_OUTDRIV                 ((uint32_t)0x00002000)        /*!<Output Drive               */
#define  SAI_xCR1_SAIEN                   ((uint32_t)0x00010000)        /*!<Audio Block enable         */
#define  SAI_xCR1_DMAEN                   ((uint32_t)0x00020000)        /*!<DMA enable                 */
#define  SAI_xCR1_NODIV                   ((uint32_t)0x00080000)        /*!<No Divider Configuration   */

#define  SAI_xCR1_MCKDIV                  ((uint32_t)0x00780000)        /*!<MCKDIV[3:0] (Master ClocK Divider)  */
#define  SAI_xCR1_MCKDIV_0                ((uint32_t)0x00080000)        /*!<Bit 0  */
#define  SAI_xCR1_MCKDIV_1                ((uint32_t)0x00100000)        /*!<Bit 1  */
#define  SAI_xCR1_MCKDIV_2                ((uint32_t)0x00200000)        /*!<Bit 2  */
#define  SAI_xCR1_MCKDIV_3                ((uint32_t)0x00400000)        /*!<Bit 3  */

/*******************  Bit definition for SAI_xCR2 register  *******************/
#define  SAI_xCR2_FTH                     ((uint32_t)0x00000003)        /*!<FTH[1:0](Fifo THreshold)  */
#define  SAI_xCR2_FTH_0                   ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  SAI_xCR2_FTH_1                   ((uint32_t)0x00000002)        /*!<Bit 1 */

#define  SAI_xCR2_FFLUSH                  ((uint32_t)0x00000008)        /*!<Fifo FLUSH                       */
#define  SAI_xCR2_TRIS                    ((uint32_t)0x00000010)        /*!<TRIState Management on data line */
#define  SAI_xCR2_MUTE                    ((uint32_t)0x00000020)        /*!<Mute mode                        */
#define  SAI_xCR2_MUTEVAL                 ((uint32_t)0x00000040)        /*!<Muate value                      */


#define  SAI_xCR2_MUTECNT                  ((uint32_t)0x00001F80)        /*!<MUTECNT[5:0] (MUTE counter) */
#define  SAI_xCR2_MUTECNT_0               ((uint32_t)0x00000080)        /*!<Bit 0 */
#define  SAI_xCR2_MUTECNT_1               ((uint32_t)0x00000100)        /*!<Bit 1 */
#define  SAI_xCR2_MUTECNT_2               ((uint32_t)0x00000200)        /*!<Bit 2 */
#define  SAI_xCR2_MUTECNT_3               ((uint32_t)0x00000400)        /*!<Bit 3 */
#define  SAI_xCR2_MUTECNT_4               ((uint32_t)0x00000800)        /*!<Bit 4 */
#define  SAI_xCR2_MUTECNT_5               ((uint32_t)0x00001000)        /*!<Bit 5 */

#define  SAI_xCR2_COMP                     ((uint32_t)0x0000C000)        /*!<COMP[1:0] (Companding mode) */
#define  SAI_xCR2_COMP_0                  ((uint32_t)0x00004000)        /*!<Bit 0 */
#define  SAI_xCR2_COMP_1                  ((uint32_t)0x00008000)        /*!<Bit 1 */


/******************  Bit definition for SAI_xFRCR register  *******************/
#define  SAI_xFRCR_FRL                    ((uint32_t)0x000000FF)        /*!<FRL[1:0](Frame length)  */
#define  SAI_xFRCR_FRL_0                  ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  SAI_xFRCR_FRL_1                  ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  SAI_xFRCR_FRL_2                  ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  SAI_xFRCR_FRL_3                  ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  SAI_xFRCR_FRL_4                  ((uint32_t)0x00000010)        /*!<Bit 4 */
#define  SAI_xFRCR_FRL_5                  ((uint32_t)0x00000020)        /*!<Bit 5 */
#define  SAI_xFRCR_FRL_6                  ((uint32_t)0x00000040)        /*!<Bit 6 */
#define  SAI_xFRCR_FRL_7                  ((uint32_t)0x00000080)        /*!<Bit 7 */

#define  SAI_xFRCR_FSALL                  ((uint32_t)0x00007F00)        /*!<FRL[1:0] (Frame synchronization active level length)  */
#define  SAI_xFRCR_FSALL_0                ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  SAI_xFRCR_FSALL_1                ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  SAI_xFRCR_FSALL_2                ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  SAI_xFRCR_FSALL_3                ((uint32_t)0x00000800)        /*!<Bit 3 */
#define  SAI_xFRCR_FSALL_4                ((uint32_t)0x00001000)        /*!<Bit 4 */
#define  SAI_xFRCR_FSALL_5                ((uint32_t)0x00002000)        /*!<Bit 5 */
#define  SAI_xFRCR_FSALL_6                ((uint32_t)0x00004000)        /*!<Bit 6 */

#define  SAI_xFRCR_FSDEF                  ((uint32_t)0x00010000)        /*!< Frame Synchronization Definition */
#define  SAI_xFRCR_FSPO                   ((uint32_t)0x00020000)        /*!<Frame Synchronization POLarity    */
#define  SAI_xFRCR_FSOFF                  ((uint32_t)0x00040000)        /*!<Frame Synchronization OFFset      */

/******************  Bit definition for SAI_xSLOTR register  *******************/
#define  SAI_xSLOTR_FBOFF                 ((uint32_t)0x0000001F)        /*!<FRL[4:0](First Bit Offset)  */
#define  SAI_xSLOTR_FBOFF_0               ((uint32_t)0x00000001)        /*!<Bit 0 */
#define  SAI_xSLOTR_FBOFF_1               ((uint32_t)0x00000002)        /*!<Bit 1 */
#define  SAI_xSLOTR_FBOFF_2               ((uint32_t)0x00000004)        /*!<Bit 2 */
#define  SAI_xSLOTR_FBOFF_3               ((uint32_t)0x00000008)        /*!<Bit 3 */
#define  SAI_xSLOTR_FBOFF_4               ((uint32_t)0x00000010)        /*!<Bit 4 */
                                     
#define  SAI_xSLOTR_SLOTSZ                ((uint32_t)0x000000C0)        /*!<SLOTSZ[1:0] (Slot size)  */
#define  SAI_xSLOTR_SLOTSZ_0              ((uint32_t)0x00000040)        /*!<Bit 0 */
#define  SAI_xSLOTR_SLOTSZ_1              ((uint32_t)0x00000080)        /*!<Bit 1 */

#define  SAI_xSLOTR_NBSLOT                ((uint32_t)0x00000F00)        /*!<NBSLOT[3:0] (Number of Slot in audio Frame)  */
#define  SAI_xSLOTR_NBSLOT_0              ((uint32_t)0x00000100)        /*!<Bit 0 */
#define  SAI_xSLOTR_NBSLOT_1              ((uint32_t)0x00000200)        /*!<Bit 1 */
#define  SAI_xSLOTR_NBSLOT_2              ((uint32_t)0x00000400)        /*!<Bit 2 */
#define  SAI_xSLOTR_NBSLOT_3              ((uint32_t)0x00000800)        /*!<Bit 3 */

#define  SAI_xSLOTR_SLOTEN                ((uint32_t)0xFFFF0000)        /*!<SLOTEN[15:0] (Slot Enable)  */

/*******************  Bit definition for SAI_xIMR register  *******************/
#define  SAI_xIMR_OVRUDRIE                ((uint32_t)0x00000001)        /*!<Overrun underrun interrupt enable                              */
#define  SAI_xIMR_MUTEDETIE               ((uint32_t)0x00000002)        /*!<Mute detection interrupt enable                                */
#define  SAI_xIMR_WCKCFGIE                ((uint32_t)0x00000004)        /*!<Wrong Clock Configuration interrupt enable                     */
#define  SAI_xIMR_FREQIE                  ((uint32_t)0x00000008)        /*!<FIFO request interrupt enable                                  */
#define  SAI_xIMR_CNRDYIE                 ((uint32_t)0x00000010)        /*!<Codec not ready interrupt enable                               */
#define  SAI_xIMR_AFSDETIE                ((uint32_t)0x00000020)        /*!<Anticipated frame synchronization detection interrupt enable   */
#define  SAI_xIMR_LFSDETIE                ((uint32_t)0x00000040)        /*!<Late frame synchronization detection interrupt enable          */

/********************  Bit definition for SAI_xSR register  *******************/
#define  SAI_xSR_OVRUDR                   ((uint32_t)0x00000001)         /*!<Overrun underrun                               */
#define  SAI_xSR_MUTEDET                  ((uint32_t)0x00000002)         /*!<Mute detection                                 */
#define  SAI_xSR_WCKCFG                   ((uint32_t)0x00000004)         /*!<Wrong Clock Configuration                      */
#define  SAI_xSR_FREQ                     ((uint32_t)0x00000008)         /*!<FIFO request                                   */
#define  SAI_xSR_CNRDY                    ((uint32_t)0x00000010)         /*!<Codec not ready                                */
#define  SAI_xSR_AFSDET                   ((uint32_t)0x00000020)         /*!<Anticipated frame synchronization detection    */
#define  SAI_xSR_LFSDET                   ((uint32_t)0x00000040)         /*!<Late frame synchronization detection           */

#define  SAI_xSR_FLVL                     ((uint32_t)0x00070000)         /*!<FLVL[2:0] (FIFO Level Threshold)               */
#define  SAI_xSR_FLVL_0                   ((uint32_t)0x00010000)         /*!<Bit 0 */
#define  SAI_xSR_FLVL_1                   ((uint32_t)0x00020000)         /*!<Bit 1 */
#define  SAI_xSR_FLVL_2                   ((uint32_t)0x00030000)         /*!<Bit 2 */

/******************  Bit definition for SAI_xCLRFR register  ******************/
#define  SAI_xCLRFR_COVRUDR               ((uint32_t)0x00000001)        /*!<Clear Overrun underrun                               */
#define  SAI_xCLRFR_CMUTEDET              ((uint32_t)0x00000002)        /*!<Clear Mute detection                                 */
#define  SAI_xCLRFR_CWCKCFG               ((uint32_t)0x00000004)        /*!<Clear Wrong Clock Configuration                      */
#define  SAI_xCLRFR_CFREQ                 ((uint32_t)0x00000008)        /*!<Clear FIFO request                                   */
#define  SAI_xCLRFR_CCNRDY                ((uint32_t)0x00000010)        /*!<Clear Codec not ready                                */
#define  SAI_xCLRFR_CAFSDET               ((uint32_t)0x00000020)        /*!<Clear Anticipated frame synchronization detection    */
#define  SAI_xCLRFR_CLFSDET               ((uint32_t)0x00000040)        /*!<Clear Late frame synchronization detection           */

/******************  Bit definition for SAI_xDR register  ******************/
#define  SAI_xDR_DATA                     ((uint32_t)0xFFFFFFFF)        


/******************************************************************************/
/*                                                                            */
/*                          LCD Controller (LCD)                              */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for LCD_CR register  *********************/
#define LCD_CR_LCDEN               ((uint32_t)0x00000001)     /*!< LCD Enable Bit */
#define LCD_CR_VSEL                ((uint32_t)0x00000002)     /*!< Voltage source selector Bit */

#define LCD_CR_DUTY                ((uint32_t)0x0000001C)     /*!< DUTY[2:0] bits (Duty selector) */
#define LCD_CR_DUTY_0              ((uint32_t)0x00000004)     /*!< Duty selector Bit 0 */
#define LCD_CR_DUTY_1              ((uint32_t)0x00000008)     /*!< Duty selector Bit 1 */
#define LCD_CR_DUTY_2              ((uint32_t)0x00000010)     /*!< Duty selector Bit 2 */

#define LCD_CR_BIAS                ((uint32_t)0x00000060)     /*!< BIAS[1:0] bits (Bias selector) */
#define LCD_CR_BIAS_0              ((uint32_t)0x00000020)     /*!< Bias selector Bit 0 */
#define LCD_CR_BIAS_1              ((uint32_t)0x00000040)     /*!< Bias selector Bit 1 */

#define LCD_CR_MUX_SEG             ((uint32_t)0x00000080)     /*!< Mux Segment Enable Bit */
#define LCD_CR_BUFEN               ((uint32_t)0x00000100)     /*!< Voltage output buffer enable */
/*******************  Bit definition for LCD_FCR register  ********************/
#define LCD_FCR_HD                 ((uint32_t)0x00000001)     /*!< High Drive Enable Bit */
#define LCD_FCR_SOFIE              ((uint32_t)0x00000002)     /*!< Start of Frame Interrupt Enable Bit */
#define LCD_FCR_UDDIE              ((uint32_t)0x00000008)     /*!< Update Display Done Interrupt Enable Bit */

#define LCD_FCR_PON                ((uint32_t)0x00000070)     /*!< PON[2:0] bits (Puls ON Duration) */
#define LCD_FCR_PON_0              ((uint32_t)0x00000010)     /*!< Bit 0 */
#define LCD_FCR_PON_1              ((uint32_t)0x00000020)     /*!< Bit 1 */
#define LCD_FCR_PON_2              ((uint32_t)0x00000040)     /*!< Bit 2 */

#define LCD_FCR_DEAD               ((uint32_t)0x00000380)     /*!< DEAD[2:0] bits (DEAD Time) */
#define LCD_FCR_DEAD_0             ((uint32_t)0x00000080)     /*!< Bit 0 */
#define LCD_FCR_DEAD_1             ((uint32_t)0x00000100)     /*!< Bit 1 */
#define LCD_FCR_DEAD_2             ((uint32_t)0x00000200)     /*!< Bit 2 */

#define LCD_FCR_CC                 ((uint32_t)0x00001C00)     /*!< CC[2:0] bits (Contrast Control) */
#define LCD_FCR_CC_0               ((uint32_t)0x00000400)     /*!< Bit 0 */
#define LCD_FCR_CC_1               ((uint32_t)0x00000800)     /*!< Bit 1 */
#define LCD_FCR_CC_2               ((uint32_t)0x00001000)     /*!< Bit 2 */

#define LCD_FCR_BLINKF             ((uint32_t)0x0000E000)     /*!< BLINKF[2:0] bits (Blink Frequency) */
#define LCD_FCR_BLINKF_0           ((uint32_t)0x00002000)     /*!< Bit 0 */
#define LCD_FCR_BLINKF_1           ((uint32_t)0x00004000)     /*!< Bit 1 */
#define LCD_FCR_BLINKF_2           ((uint32_t)0x00008000)     /*!< Bit 2 */

#define LCD_FCR_BLINK              ((uint32_t)0x00030000)     /*!< BLINK[1:0] bits (Blink Enable) */
#define LCD_FCR_BLINK_0            ((uint32_t)0x00010000)     /*!< Bit 0 */
#define LCD_FCR_BLINK_1            ((uint32_t)0x00020000)     /*!< Bit 1 */

#define LCD_FCR_DIV                ((uint32_t)0x003C0000)     /*!< DIV[3:0] bits (Divider) */
#define LCD_FCR_PS                 ((uint32_t)0x03C00000)     /*!< PS[3:0] bits (Prescaler) */

/*******************  Bit definition for LCD_SR register  *********************/
#define LCD_SR_ENS                 ((uint32_t)0x00000001)     /*!< LCD Enabled Bit */
#define LCD_SR_SOF                 ((uint32_t)0x00000002)     /*!< Start Of Frame Flag Bit */
#define LCD_SR_UDR                 ((uint32_t)0x00000004)     /*!< Update Display Request Bit */
#define LCD_SR_UDD                 ((uint32_t)0x00000008)     /*!< Update Display Done Flag Bit */
#define LCD_SR_RDY                 ((uint32_t)0x00000010)     /*!< Ready Flag Bit */
#define LCD_SR_FCRSR               ((uint32_t)0x00000020)     /*!< LCD FCR Register Synchronization Flag Bit */

/*******************  Bit definition for LCD_CLR register  ********************/
#define LCD_CLR_SOFC               ((uint32_t)0x00000002)     /*!< Start Of Frame Flag Clear Bit */
#define LCD_CLR_UDDC               ((uint32_t)0x00000008)     /*!< Update Display Done Flag Clear Bit */

/*******************  Bit definition for LCD_RAM register  ********************/
#define LCD_RAM_SEGMENT_DATA       ((uint32_t)0xFFFFFFFF)     /*!< Segment Data Bits */

/******************************************************************************/
/*                                                                            */
/*                          SD host Interface                                 */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for SDIO_POWER register  ******************/
#define  SDIO_POWER_PWRCTRL                  ((uint8_t)0x03)               /*!<PWRCTRL[1:0] bits (Power supply control bits) */
#define  SDIO_POWER_PWRCTRL_0                ((uint8_t)0x01)               /*!<Bit 0 */
#define  SDIO_POWER_PWRCTRL_1                ((uint8_t)0x02)               /*!<Bit 1 */

/******************  Bit definition for SDIO_CLKCR register  ******************/
#define  SDIO_CLKCR_CLKDIV                   ((uint16_t)0x00FF)            /*!<Clock divide factor             */
#define  SDIO_CLKCR_CLKEN                    ((uint16_t)0x0100)            /*!<Clock enable bit                */
#define  SDIO_CLKCR_PWRSAV                   ((uint16_t)0x0200)            /*!<Power saving configuration bit  */
#define  SDIO_CLKCR_BYPASS                   ((uint16_t)0x0400)            /*!<Clock divider bypass enable bit */

#define  SDIO_CLKCR_WIDBUS                   ((uint16_t)0x1800)            /*!<WIDBUS[1:0] bits (Wide bus mode enable bit) */
#define  SDIO_CLKCR_WIDBUS_0                 ((uint16_t)0x0800)            /*!<Bit 0 */
#define  SDIO_CLKCR_WIDBUS_1                 ((uint16_t)0x1000)            /*!<Bit 1 */

#define  SDIO_CLKCR_NEGEDGE                  ((uint16_t)0x2000)            /*!<SDIO_CK dephasing selection bit */
#define  SDIO_CLKCR_HWFC_EN                  ((uint16_t)0x4000)            /*!<HW Flow Control enable          */

/*******************  Bit definition for SDIO_ARG register  *******************/
#define  SDIO_ARG_CMDARG                     ((uint32_t)0xFFFFFFFF)            /*!<Command argument */

/*******************  Bit definition for SDIO_CMD register  *******************/
#define  SDIO_CMD_CMDINDEX                   ((uint16_t)0x003F)            /*!<Command Index                               */

#define  SDIO_CMD_WAITRESP                   ((uint16_t)0x00C0)            /*!<WAITRESP[1:0] bits (Wait for response bits) */
#define  SDIO_CMD_WAITRESP_0                 ((uint16_t)0x0040)            /*!< Bit 0 */
#define  SDIO_CMD_WAITRESP_1                 ((uint16_t)0x0080)            /*!< Bit 1 */

#define  SDIO_CMD_WAITINT                    ((uint16_t)0x0100)            /*!<CPSM Waits for Interrupt Request                               */
#define  SDIO_CMD_WAITPEND                   ((uint16_t)0x0200)            /*!<CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define  SDIO_CMD_CPSMEN                     ((uint16_t)0x0400)            /*!<Command path state machine (CPSM) Enable bit                   */
#define  SDIO_CMD_SDIOSUSPEND                ((uint16_t)0x0800)            /*!<SD I/O suspend command                                         */
#define  SDIO_CMD_ENCMDCOMPL                 ((uint16_t)0x1000)            /*!<Enable CMD completion                                          */
#define  SDIO_CMD_NIEN                       ((uint16_t)0x2000)            /*!<Not Interrupt Enable */
#define  SDIO_CMD_CEATACMD                   ((uint16_t)0x4000)            /*!<CE-ATA command       */

/*****************  Bit definition for SDIO_RESPCMD register  *****************/
#define  SDIO_RESPCMD_RESPCMD                ((uint8_t)0x3F)               /*!<Response command index */

/******************  Bit definition for SDIO_RESP0 register  ******************/
#define  SDIO_RESP0_CARDSTATUS0              ((uint32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP1 register  ******************/
#define  SDIO_RESP1_CARDSTATUS1              ((uint32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP2 register  ******************/
#define  SDIO_RESP2_CARDSTATUS2              ((uint32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP3 register  ******************/
#define  SDIO_RESP3_CARDSTATUS3              ((uint32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP4 register  ******************/
#define  SDIO_RESP4_CARDSTATUS4              ((uint32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_DTIMER register  *****************/
#define  SDIO_DTIMER_DATATIME                ((uint32_t)0xFFFFFFFF)        /*!<Data timeout period. */

/******************  Bit definition for SDIO_DLEN register  *******************/
#define  SDIO_DLEN_DATALENGTH                ((uint32_t)0x01FFFFFF)        /*!<Data length value    */

/******************  Bit definition for SDIO_DCTRL register  ******************/
#define  SDIO_DCTRL_DTEN                     ((uint16_t)0x0001)            /*!<Data transfer enabled bit         */
#define  SDIO_DCTRL_DTDIR                    ((uint16_t)0x0002)            /*!<Data transfer direction selection */
#define  SDIO_DCTRL_DTMODE                   ((uint16_t)0x0004)            /*!<Data transfer mode selection      */
#define  SDIO_DCTRL_DMAEN                    ((uint16_t)0x0008)            /*!<DMA enabled bit                   */

#define  SDIO_DCTRL_DBLOCKSIZE               ((uint16_t)0x00F0)            /*!<DBLOCKSIZE[3:0] bits (Data block size) */
#define  SDIO_DCTRL_DBLOCKSIZE_0             ((uint16_t)0x0010)            /*!<Bit 0 */
#define  SDIO_DCTRL_DBLOCKSIZE_1             ((uint16_t)0x0020)            /*!<Bit 1 */
#define  SDIO_DCTRL_DBLOCKSIZE_2             ((uint16_t)0x0040)            /*!<Bit 2 */
#define  SDIO_DCTRL_DBLOCKSIZE_3             ((uint16_t)0x0080)            /*!<Bit 3 */

#define  SDIO_DCTRL_RWSTART                  ((uint16_t)0x0100)            /*!<Read wait start         */
#define  SDIO_DCTRL_RWSTOP                   ((uint16_t)0x0200)            /*!<Read wait stop          */
#define  SDIO_DCTRL_RWMOD                    ((uint16_t)0x0400)            /*!<Read wait mode          */
#define  SDIO_DCTRL_SDIOEN                   ((uint16_t)0x0800)            /*!<SD I/O enable functions */

/******************  Bit definition for SDIO_DCOUNT register  *****************/
#define  SDIO_DCOUNT_DATACOUNT               ((uint32_t)0x01FFFFFF)        /*!<Data count value */

/******************  Bit definition for SDIO_STA register  ********************/
#define  SDIO_STA_CCRCFAIL                   ((uint32_t)0x00000001)        /*!<Command response received (CRC check failed)  */
#define  SDIO_STA_DCRCFAIL                   ((uint32_t)0x00000002)        /*!<Data block sent/received (CRC check failed)   */
#define  SDIO_STA_CTIMEOUT                   ((uint32_t)0x00000004)        /*!<Command response timeout                      */
#define  SDIO_STA_DTIMEOUT                   ((uint32_t)0x00000008)        /*!<Data timeout                                  */
#define  SDIO_STA_TXUNDERR                   ((uint32_t)0x00000010)        /*!<Transmit FIFO underrun error                  */
#define  SDIO_STA_RXOVERR                    ((uint32_t)0x00000020)        /*!<Received FIFO overrun error                   */
#define  SDIO_STA_CMDREND                    ((uint32_t)0x00000040)        /*!<Command response received (CRC check passed)  */
#define  SDIO_STA_CMDSENT                    ((uint32_t)0x00000080)        /*!<Command sent (no response required)           */
#define  SDIO_STA_DATAEND                    ((uint32_t)0x00000100)        /*!<Data end (data counter, SDIDCOUNT, is zero)   */
#define  SDIO_STA_STBITERR                   ((uint32_t)0x00000200)        /*!<Start bit not detected on all data signals in wide bus mode */
#define  SDIO_STA_DBCKEND                    ((uint32_t)0x00000400)        /*!<Data block sent/received (CRC check passed)   */
#define  SDIO_STA_CMDACT                     ((uint32_t)0x00000800)        /*!<Command transfer in progress                  */
#define  SDIO_STA_TXACT                      ((uint32_t)0x00001000)        /*!<Data transmit in progress                     */
#define  SDIO_STA_RXACT                      ((uint32_t)0x00002000)        /*!<Data receive in progress                      */
#define  SDIO_STA_TXFIFOHE                   ((uint32_t)0x00004000)        /*!<Transmit FIFO Half Empty: at least 8 words can be written into the FIFO */
#define  SDIO_STA_RXFIFOHF                   ((uint32_t)0x00008000)        /*!<Receive FIFO Half Full: there are at least 8 words in the FIFO */
#define  SDIO_STA_TXFIFOF                    ((uint32_t)0x00010000)        /*!<Transmit FIFO full                            */
#define  SDIO_STA_RXFIFOF                    ((uint32_t)0x00020000)        /*!<Receive FIFO full                             */
#define  SDIO_STA_TXFIFOE                    ((uint32_t)0x00040000)        /*!<Transmit FIFO empty                           */
#define  SDIO_STA_RXFIFOE                    ((uint32_t)0x00080000)        /*!<Receive FIFO empty                            */
#define  SDIO_STA_TXDAVL                     ((uint32_t)0x00100000)        /*!<Data available in transmit FIFO               */
#define  SDIO_STA_RXDAVL                     ((uint32_t)0x00200000)        /*!<Data available in receive FIFO                */
#define  SDIO_STA_SDIOIT                     ((uint32_t)0x00400000)        /*!<SDIO interrupt received                       */
#define  SDIO_STA_CEATAEND                   ((uint32_t)0x00800000)        /*!<CE-ATA command completion signal received for CMD61 */

/*******************  Bit definition for SDIO_ICR register  *******************/
#define  SDIO_ICR_CCRCFAILC                  ((uint32_t)0x00000001)        /*!<CCRCFAIL flag clear bit */
#define  SDIO_ICR_DCRCFAILC                  ((uint32_t)0x00000002)        /*!<DCRCFAIL flag clear bit */
#define  SDIO_ICR_CTIMEOUTC                  ((uint32_t)0x00000004)        /*!<CTIMEOUT flag clear bit */
#define  SDIO_ICR_DTIMEOUTC                  ((uint32_t)0x00000008)        /*!<DTIMEOUT flag clear bit */
#define  SDIO_ICR_TXUNDERRC                  ((uint32_t)0x00000010)        /*!<TXUNDERR flag clear bit */
#define  SDIO_ICR_RXOVERRC                   ((uint32_t)0x00000020)        /*!<RXOVERR flag clear bit  */
#define  SDIO_ICR_CMDRENDC                   ((uint32_t)0x00000040)        /*!<CMDREND flag clear bit  */
#define  SDIO_ICR_CMDSENTC                   ((uint32_t)0x00000080)        /*!<CMDSENT flag clear bit  */
#define  SDIO_ICR_DATAENDC                   ((uint32_t)0x00000100)        /*!<DATAEND flag clear bit  */
#define  SDIO_ICR_STBITERRC                  ((uint32_t)0x00000200)        /*!<STBITERR flag clear bit */
#define  SDIO_ICR_DBCKENDC                   ((uint32_t)0x00000400)        /*!<DBCKEND flag clear bit  */
#define  SDIO_ICR_SDIOITC                    ((uint32_t)0x00400000)        /*!<SDIOIT flag clear bit   */
#define  SDIO_ICR_CEATAENDC                  ((uint32_t)0x00800000)        /*!<CEATAEND flag clear bit */

/******************  Bit definition for SDIO_MASK register  *******************/
#define  SDIO_MASK_CCRCFAILIE                ((uint32_t)0x00000001)        /*!<Command CRC Fail Interrupt Enable          */
#define  SDIO_MASK_DCRCFAILIE                ((uint32_t)0x00000002)        /*!<Data CRC Fail Interrupt Enable             */
#define  SDIO_MASK_CTIMEOUTIE                ((uint32_t)0x00000004)        /*!<Command TimeOut Interrupt Enable           */
#define  SDIO_MASK_DTIMEOUTIE                ((uint32_t)0x00000008)        /*!<Data TimeOut Interrupt Enable              */
#define  SDIO_MASK_TXUNDERRIE                ((uint32_t)0x00000010)        /*!<Tx FIFO UnderRun Error Interrupt Enable    */
#define  SDIO_MASK_RXOVERRIE                 ((uint32_t)0x00000020)        /*!<Rx FIFO OverRun Error Interrupt Enable     */
#define  SDIO_MASK_CMDRENDIE                 ((uint32_t)0x00000040)        /*!<Command Response Received Interrupt Enable */
#define  SDIO_MASK_CMDSENTIE                 ((uint32_t)0x00000080)        /*!<Command Sent Interrupt Enable              */
#define  SDIO_MASK_DATAENDIE                 ((uint32_t)0x00000100)        /*!<Data End Interrupt Enable                  */
#define  SDIO_MASK_STBITERRIE                ((uint32_t)0x00000200)        /*!<Start Bit Error Interrupt Enable           */
#define  SDIO_MASK_DBCKENDIE                 ((uint32_t)0x00000400)        /*!<Data Block End Interrupt Enable            */
#define  SDIO_MASK_CMDACTIE                  ((uint32_t)0x00000800)        /*!<CCommand Acting Interrupt Enable           */
#define  SDIO_MASK_TXACTIE                   ((uint32_t)0x00001000)        /*!<Data Transmit Acting Interrupt Enable      */
#define  SDIO_MASK_RXACTIE                   ((uint32_t)0x00002000)        /*!<Data receive acting interrupt enabled      */
#define  SDIO_MASK_TXFIFOHEIE                ((uint32_t)0x00004000)        /*!<Tx FIFO Half Empty interrupt Enable        */
#define  SDIO_MASK_RXFIFOHFIE                ((uint32_t)0x00008000)        /*!<Rx FIFO Half Full interrupt Enable         */
#define  SDIO_MASK_TXFIFOFIE                 ((uint32_t)0x00010000)        /*!<Tx FIFO Full interrupt Enable              */
#define  SDIO_MASK_RXFIFOFIE                 ((uint32_t)0x00020000)        /*!<Rx FIFO Full interrupt Enable              */
#define  SDIO_MASK_TXFIFOEIE                 ((uint32_t)0x00040000)        /*!<Tx FIFO Empty interrupt Enable             */
#define  SDIO_MASK_RXFIFOEIE                 ((uint32_t)0x00080000)        /*!<Rx FIFO Empty interrupt Enable             */
#define  SDIO_MASK_TXDAVLIE                  ((uint32_t)0x00100000)        /*!<Data available in Tx FIFO interrupt Enable */
#define  SDIO_MASK_RXDAVLIE                  ((uint32_t)0x00200000)        /*!<Data available in Rx FIFO interrupt Enable */
#define  SDIO_MASK_SDIOITIE                  ((uint32_t)0x00400000)        /*!<SDIO Mode Interrupt Received interrupt Enable */
#define  SDIO_MASK_CEATAENDIE                ((uint32_t)0x00800000)        /*!<CE-ATA command completion signal received Interrupt Enable */

/*****************  Bit definition for SDIO_FIFOCNT register  *****************/
#define  SDIO_FIFOCNT_FIFOCOUNT              ((uint32_t)0x00FFFFFF)        /*!<Remaining number of words to be written to or read from the FIFO */

/******************  Bit definition for SDIO_FIFO register  *******************/
#define  SDIO_FIFO_FIFODATA                  ((uint32_t)0xFFFFFFFF)        /*!<Receive and transmit FIFO data */

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface (SPI)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_CPHA                        ((uint16_t)0x0001)            /*!<Clock Phase      */
#define  SPI_CR1_CPOL                        ((uint16_t)0x0002)            /*!<Clock Polarity   */
#define  SPI_CR1_MSTR                        ((uint16_t)0x0004)            /*!<Master Selection */

#define  SPI_CR1_BR                          ((uint16_t)0x0038)            /*!<BR[2:0] bits (Baud Rate Control) */
#define  SPI_CR1_BR_0                        ((uint16_t)0x0008)            /*!<Bit 0 */
#define  SPI_CR1_BR_1                        ((uint16_t)0x0010)            /*!<Bit 1 */
#define  SPI_CR1_BR_2                        ((uint16_t)0x0020)            /*!<Bit 2 */

#define  SPI_CR1_SPE                         ((uint16_t)0x0040)            /*!<SPI Enable                          */
#define  SPI_CR1_LSBFIRST                    ((uint16_t)0x0080)            /*!<Frame Format                        */
#define  SPI_CR1_SSI                         ((uint16_t)0x0100)            /*!<Internal slave select               */
#define  SPI_CR1_SSM                         ((uint16_t)0x0200)            /*!<Software slave management           */
#define  SPI_CR1_RXONLY                      ((uint16_t)0x0400)            /*!<Receive only                        */
#define  SPI_CR1_CRCL                        ((uint16_t)0x0800)            /*!< CRC Length */
#define  SPI_CR1_CRCNEXT                     ((uint16_t)0x1000)            /*!<Transmit CRC next                   */
#define  SPI_CR1_CRCEN                       ((uint16_t)0x2000)            /*!<Hardware CRC calculation enable     */
#define  SPI_CR1_BIDIOE                      ((uint16_t)0x4000)            /*!<Output enable in bidirectional mode */
#define  SPI_CR1_BIDIMODE                    ((uint16_t)0x8000)            /*!<Bidirectional data mode enable      */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define  SPI_CR2_RXDMAEN                     ((uint16_t)0x0001)            /*!< Rx Buffer DMA Enable */
#define  SPI_CR2_TXDMAEN                     ((uint16_t)0x0002)            /*!< Tx Buffer DMA Enable */
#define  SPI_CR2_SSOE                        ((uint16_t)0x0004)            /*!< SS Output Enable */
#define  SPI_CR2_NSSP                        ((uint16_t)0x0008)            /*!< NSS pulse management Enable */
#define  SPI_CR2_FRF                         ((uint16_t)0x0010)            /*!< Frame Format Enable */
#define  SPI_CR2_ERRIE                       ((uint16_t)0x0020)            /*!< Error Interrupt Enable */
#define  SPI_CR2_RXNEIE                      ((uint16_t)0x0040)            /*!< RX buffer Not Empty Interrupt Enable */
#define  SPI_CR2_TXEIE                       ((uint16_t)0x0080)            /*!< Tx buffer Empty Interrupt Enable */
#define  SPI_CR2_DS                          ((uint16_t)0x0F00)            /*!< DS[3:0] Data Size */
#define  SPI_CR2_DS_0                        ((uint16_t)0x0100)            /*!< Bit 0 */
#define  SPI_CR2_DS_1                        ((uint16_t)0x0200)            /*!< Bit 1 */
#define  SPI_CR2_DS_2                        ((uint16_t)0x0400)            /*!< Bit 2 */
#define  SPI_CR2_DS_3                        ((uint16_t)0x0800)            /*!< Bit 3 */
#define  SPI_CR2_FRXTH                       ((uint16_t)0x1000)            /*!< FIFO reception Threshold */
#define  SPI_CR2_LDMARX                      ((uint16_t)0x2000)            /*!< Last DMA transfer for reception */
#define  SPI_CR2_LDMATX                      ((uint16_t)0x4000)            /*!< Last DMA transfer for transmission */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_RXNE                         ((uint16_t)0x0001)            /*!< Receive buffer Not Empty */
#define  SPI_SR_TXE                          ((uint16_t)0x0002)            /*!< Transmit buffer Empty */
#define  SPI_SR_CHSIDE                       ((uint16_t)0x0004)            /*!< Channel side */
#define  SPI_SR_UDR                          ((uint16_t)0x0008)            /*!< Underrun flag */
#define  SPI_SR_CRCERR                       ((uint16_t)0x0010)            /*!< CRC Error flag */
#define  SPI_SR_MODF                         ((uint16_t)0x0020)            /*!< Mode fault */
#define  SPI_SR_OVR                          ((uint16_t)0x0040)            /*!< Overrun flag */
#define  SPI_SR_BSY                          ((uint16_t)0x0080)            /*!< Busy flag */
#define  SPI_SR_FRE                          ((uint16_t)0x0100)            /*!< TI frame format error */
#define  SPI_SR_FRLVL                        ((uint16_t)0x0600)            /*!< FIFO Reception Level */
#define  SPI_SR_FRLVL_0                      ((uint16_t)0x0200)            /*!< Bit 0 */
#define  SPI_SR_FRLVL_1                      ((uint16_t)0x0400)            /*!< Bit 1 */
#define  SPI_SR_FTLVL                        ((uint16_t)0x1800)            /*!< FIFO Transmission Level */
#define  SPI_SR_FTLVL_0                      ((uint16_t)0x0800)            /*!< Bit 0 */
#define  SPI_SR_FTLVL_1                      ((uint16_t)0x1000)            /*!< Bit 1 */  

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint16_t)0xFFFF)            /*!<Data Register           */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define  SPI_CRCPR_CRCPOLY                   ((uint16_t)0xFFFF)            /*!<CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define  SPI_RXCRCR_RXCRC                    ((uint16_t)0xFFFF)            /*!<Rx CRC Register         */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define  SPI_TXCRCR_TXCRC                    ((uint16_t)0xFFFF)            /*!<Tx CRC Register         */

/******************************************************************************/
/*                                                                            */
/*                                    QUADSPI                                 */
/*                                                                            */
/******************************************************************************/
/*****************  Bit definition for QUADSPI_CR register  *******************/
#define  QUADSPI_CR_EN                           ((uint32_t)0x00000001)            /*!< Enable */
#define  QUADSPI_CR_ABORT                        ((uint32_t)0x00000002)            /*!< Abort request */
#define  QUADSPI_CR_DMAEN                        ((uint32_t)0x00000004)            /*!< DMA Enable */
#define  QUADSPI_CR_TCEN                         ((uint32_t)0x00000008)            /*!< Timeout Counter Enable */
#define  QUADSPI_CR_SSHIFT                       ((uint32_t)0x00000030)            /*!< SSHIFT[1:0] Sample Shift */
#define  QUADSPI_CR_SSHIFT_0                     ((uint32_t)0x00000010)            /*!< Bit 0 */
#define  QUADSPI_CR_SSHIFT_1                     ((uint32_t)0x00000020)            /*!< Bit 1 */
#define  QUADSPI_CR_FTHRES                       ((uint32_t)0x00000F00)            /*!< FTHRES[3:0] FIFO Level */
#define  QUADSPI_CR_FTHRES_0                     ((uint32_t)0x00000100)            /*!< Bit 0 */
#define  QUADSPI_CR_FTHRES_1                     ((uint32_t)0x00000200)            /*!< Bit 1 */
#define  QUADSPI_CR_FTHRES_2                     ((uint32_t)0x00000400)            /*!< Bit 2 */
#define  QUADSPI_CR_FTHRES_3                     ((uint32_t)0x00000800)            /*!< Bit 3 */
#define  QUADSPI_CR_TEIE                         ((uint32_t)0x00010000)            /*!< Transfer Error Interrupt Enable */
#define  QUADSPI_CR_TCIE                         ((uint32_t)0x00020000)            /*!< Transfer Complete Interrupt Enable */
#define  QUADSPI_CR_FTIE                         ((uint32_t)0x00040000)            /*!< FIFO Threshold Interrupt Enable */
#define  QUADSPI_CR_SMIE                         ((uint32_t)0x00080000)            /*!< Status Match Interrupt Enable */
#define  QUADSPI_CR_TOIE                         ((uint32_t)0x00100000)            /*!< TimeOut Interrupt Enable */
#define  QUADSPI_CR_APMS                         ((uint32_t)0x00400000)            /*!< Bit 1 */
#define  QUADSPI_CR_PMM                          ((uint32_t)0x00800000)            /*!< Polling Match Mode */
#define  QUADSPI_CR_PRESCALER                    ((uint32_t)0xFF000000)            /*!< PRESCALER[7:0] Clock prescaler */
#define  QUADSPI_CR_PRESCALER_0                  ((uint32_t)0x01000000)            /*!< Bit 0 */
#define  QUADSPI_CR_PRESCALER_1                  ((uint32_t)0x02000000)            /*!< Bit 1 */
#define  QUADSPI_CR_PRESCALER_2                  ((uint32_t)0x04000000)            /*!< Bit 2 */
#define  QUADSPI_CR_PRESCALER_3                  ((uint32_t)0x08000000)            /*!< Bit 3 */
#define  QUADSPI_CR_PRESCALER_4                  ((uint32_t)0x10000000)            /*!< Bit 4 */
#define  QUADSPI_CR_PRESCALER_5                  ((uint32_t)0x20000000)            /*!< Bit 5 */
#define  QUADSPI_CR_PRESCALER_6                  ((uint32_t)0x40000000)            /*!< Bit 6 */
#define  QUADSPI_CR_PRESCALER_7                  ((uint32_t)0x80000000)            /*!< Bit 7 */

/*****************  Bit definition for QUADSPI_DCR register  ******************/
#define  QUADSPI_DCR_CKMODE                      ((uint32_t)0x00000001)            /*!< Mode 0 / Mode 3 */
#define  QUADSPI_DCR_CSHT                        ((uint32_t)0x00000700)            /*!< CSHT[2:0]: ChipSelect High Time */
#define  QUADSPI_DCR_CSHT_0                      ((uint32_t)0x00000100)            /*!< Bit 0 */
#define  QUADSPI_DCR_CSHT_1                      ((uint32_t)0x00000200)            /*!< Bit 1 */
#define  QUADSPI_DCR_CSHT_2                      ((uint32_t)0x00000400)            /*!< Bit 2 */
#define  QUADSPI_DCR_FSIZE                       ((uint32_t)0x001F0000)            /*!< FSIZE[4:0]: Flash Size */
#define  QUADSPI_DCR_FSIZE_0                     ((uint32_t)0x00010000)            /*!< Bit 0 */
#define  QUADSPI_DCR_FSIZE_1                     ((uint32_t)0x00020000)            /*!< Bit 1 */
#define  QUADSPI_DCR_FSIZE_2                     ((uint32_t)0x00040000)            /*!< Bit 2 */
#define  QUADSPI_DCR_FSIZE_3                     ((uint32_t)0x00080000)            /*!< Bit 3 */
#define  QUADSPI_DCR_FSIZE_4                     ((uint32_t)0x00100000)            /*!< Bit 4 */

/******************  Bit definition for QUADSPI_SR register  *******************/
#define  QUADSPI_SR_TEF                          ((uint32_t)0x00000001)             /*!< Transfer Error Flag */
#define  QUADSPI_SR_TCF                          ((uint32_t)0x00000002)             /*!< Transfer Complete Flag */
#define  QUADSPI_SR_FTF                          ((uint32_t)0x00000004)             /*!< FIFO Threshlod Flag */
#define  QUADSPI_SR_SMF                          ((uint32_t)0x00000008)             /*!< Status Match Flag */
#define  QUADSPI_SR_TOF                          ((uint32_t)0x00000010)             /*!< Timeout Flag */
#define  QUADSPI_SR_BUSY                         ((uint32_t)0x00000020)             /*!< Busy */
#define  QUADSPI_SR_FLEVEL                       ((uint32_t)0x00001F00)             /*!< FIFO Threshlod Flag */
#define  QUADSPI_SR_FLEVEL_0                     ((uint32_t)0x00000100)             /*!< Bit 0 */
#define  QUADSPI_SR_FLEVEL_1                     ((uint32_t)0x00000200)             /*!< Bit 1 */
#define  QUADSPI_SR_FLEVEL_2                     ((uint32_t)0x00000400)             /*!< Bit 2 */
#define  QUADSPI_SR_FLEVEL_3                     ((uint32_t)0x00000800)             /*!< Bit 3 */
#define  QUADSPI_SR_FLEVEL_4                     ((uint32_t)0x00001000)             /*!< Bit 4 */

/******************  Bit definition for QUADSPI_FCR register  ******************/
#define  QUADSPI_FCR_CTEF                        ((uint32_t)0x00000001)             /*!< Clear Transfer Error Flag */
#define  QUADSPI_FCR_CTCF                        ((uint32_t)0x00000002)             /*!< Clear Transfer Complete Flag */
#define  QUADSPI_FCR_CSMF                        ((uint32_t)0x00000008)             /*!< Clear Status Match Flag */
#define  QUADSPI_FCR_CTOF                        ((uint32_t)0x00000010)             /*!< Clear Timeout Flag */

/******************  Bit definition for QUADSPI_DLR register  ******************/
#define  QUADSPI_DLR_DL                        ((uint32_t)0xFFFFFFFF)               /*!< DL[31:0]: Data Length */

/******************  Bit definition for QUADSPI_CCR register  ******************/
#define  QUADSPI_CCR_INSTRUCTION                  ((uint32_t)0x000000FF)            /*!< INSTRUCTION[7:0]: Instruction */
#define  QUADSPI_CCR_INSTRUCTION_0                ((uint32_t)0x00000001)            /*!< Bit 0 */
#define  QUADSPI_CCR_INSTRUCTION_1                ((uint32_t)0x00000002)            /*!< Bit 1 */
#define  QUADSPI_CCR_INSTRUCTION_2                ((uint32_t)0x00000004)            /*!< Bit 2 */
#define  QUADSPI_CCR_INSTRUCTION_3                ((uint32_t)0x00000008)            /*!< Bit 3 */
#define  QUADSPI_CCR_INSTRUCTION_4                ((uint32_t)0x00000010)            /*!< Bit 4 */
#define  QUADSPI_CCR_INSTRUCTION_5                ((uint32_t)0x00000020)            /*!< Bit 5 */
#define  QUADSPI_CCR_INSTRUCTION_6                ((uint32_t)0x00000040)            /*!< Bit 6 */
#define  QUADSPI_CCR_INSTRUCTION_7                ((uint32_t)0x00000080)            /*!< Bit 7 */
#define  QUADSPI_CCR_IMODE                        ((uint32_t)0x00000300)            /*!< IMODE[1:0]: Instruction Mode */
#define  QUADSPI_CCR_IMODE_0                      ((uint32_t)0x00000100)            /*!< Bit 0 */
#define  QUADSPI_CCR_IMODE_1                      ((uint32_t)0x00000200)            /*!< Bit 1 */
#define  QUADSPI_CCR_ADMODE                       ((uint32_t)0x00000C00)            /*!< ADMODE[1:0]: Address Mode */
#define  QUADSPI_CCR_ADMODE_0                     ((uint32_t)0x00000400)            /*!< Bit 0 */
#define  QUADSPI_CCR_ADMODE_1                     ((uint32_t)0x00000800)            /*!< Bit 1 */
#define  QUADSPI_CCR_ADSIZE                       ((uint32_t)0x00003000)            /*!< ADSIZE[1:0]: Address Size */
#define  QUADSPI_CCR_ADSIZE_0                     ((uint32_t)0x00001000)            /*!< Bit 0 */
#define  QUADSPI_CCR_ADSIZE_1                     ((uint32_t)0x00002000)            /*!< Bit 1 */
#define  QUADSPI_CCR_ABMODE                       ((uint32_t)0x0000C000)            /*!< ABMODE[1:0]: Alternate Bytes Mode */
#define  QUADSPI_CCR_ABMODE_0                     ((uint32_t)0x00004000)            /*!< Bit 0 */
#define  QUADSPI_CCR_ABMODE_1                     ((uint32_t)0x00008000)            /*!< Bit 1 */
#define  QUADSPI_CCR_ABSIZE                       ((uint32_t)0x00030000)            /*!< ABSIZE[1:0]: Instruction Mode */
#define  QUADSPI_CCR_ABSIZE_0                     ((uint32_t)0x00010000)            /*!< Bit 0 */
#define  QUADSPI_CCR_ABSIZE_1                     ((uint32_t)0x00020000)            /*!< Bit 1 */
#define  QUADSPI_CCR_DCYC                         ((uint32_t)0x007C0000)            /*!< DCYC[4:0]: Dummy Cycles */
#define  QUADSPI_CCR_DCYC_0                       ((uint32_t)0x00040000)            /*!< Bit 0 */
#define  QUADSPI_CCR_DCYC_1                       ((uint32_t)0x00080000)            /*!< Bit 1 */
#define  QUADSPI_CCR_DCYC_2                       ((uint32_t)0x00100000)            /*!< Bit 2 */
#define  QUADSPI_CCR_DCYC_3                       ((uint32_t)0x00200000)            /*!< Bit 3 */
#define  QUADSPI_CCR_DCYC_4                       ((uint32_t)0x00400000)            /*!< Bit 4 */
#define  QUADSPI_CCR_DMODE                        ((uint32_t)0x03000000)            /*!< DMODE[1:0]: Data Mode */
#define  QUADSPI_CCR_DMODE_0                      ((uint32_t)0x01000000)            /*!< Bit 0 */
#define  QUADSPI_CCR_DMODE_1                      ((uint32_t)0x02000000)            /*!< Bit 1 */
#define  QUADSPI_CCR_FMODE                        ((uint32_t)0x0C000000)            /*!< FMODE[1:0]: Functional Mode */
#define  QUADSPI_CCR_FMODE_0                      ((uint32_t)0x04000000)            /*!< Bit 0 */
#define  QUADSPI_CCR_FMODE_1                      ((uint32_t)0x08000000)            /*!< Bit 1 */
#define  QUADSPI_CCR_SIOO                         ((uint32_t)0x10000000)            /*!< SIOO: Send Instruction Only Once Mode */
#define  QUADSPI_CCR_DDRM                         ((uint32_t)0x80000000)            /*!< DDRM: Double Data Rate Mode */

/******************  Bit definition for QUADSPI_AR register  *******************/
#define  QUADSPI_AR_ADDRESS                       ((uint32_t)0xFFFFFFFF)            /*!< ADDRESS[31:0]: Address */

/******************  Bit definition for QUADSPI_ABR register  ******************/
#define  QUADSPI_ABR_ALTERNATE                    ((uint32_t)0xFFFFFFFF)            /*!< ALTERNATE[31:0]: Alternate Bytes */

/******************  Bit definition for QUADSPI_DR register  *******************/
#define  QUADSPI_DR_DATA                          ((uint32_t)0xFFFFFFFF)            /*!< DATA[31:0]: Data */

/******************  Bit definition for QUADSPI_PSMKR register  ****************/
#define  QUADSPI_PSMKR_MASK                       ((uint32_t)0xFFFFFFFF)            /*!< MASK[31:0]: Status Mask */

/******************  Bit definition for QUADSPI_PSMAR register  ****************/
#define  QUADSPI_PSMAR_MATCH                      ((uint32_t)0xFFFFFFFF)            /*!< MATCH[31:0]: Status Match */

/******************  Bit definition for QUADSPI_PIR register  *****************/
#define  QUADSPI_PIR_INTERVAL                     ((uint32_t)0x0000FFFF)            /*!< INTERVAL[15:0]: Polling Interval */

/******************  Bit definition for QUADSPI_LPTR register  *****************/
#define  QUADSPI_LPTR_TIMEOUT                     ((uint32_t)0x0000FFFF)            /*!< TIMEOUT[15:0]: Timeout period */

/******************************************************************************/
/*                                                                            */
/*                                 SYSCFG                                     */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for SYSCFG_MEMRMP register  ***************/  
#define SYSCFG_MEMRMP_MEM_MODE          ((uint32_t)0x00000007) /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_MEMRMP_UFB_MODE          ((uint32_t)0x00000100) /*!< User Flash Bank mode */

/******************  Bit definition for SYSCFG_PMC register  ******************/
#define SYSCFG_PMC_FWDIS                  ((uint32_t)0x00000001) /*!< FIREWALL access enable*/
#define SYSCFG_PMC_I2C_PB6_FMP            ((uint32_t)0x00010000) /*!< I2C PB6 Fast mode plus */
#define SYSCFG_PMC_I2C_PB7_FMP            ((uint32_t)0x00020000) /*!< I2C PB7 Fast mode plus */
#define SYSCFG_PMC_I2C_PB8_FMP            ((uint32_t)0x00040000) /*!< I2C PB8 Fast mode plus */
#define SYSCFG_PMC_I2C_PB9_FMP            ((uint32_t)0x00080000) /*!< I2C PB9 Fast mode plus */
#define SYSCFG_PMC_I2C1_FMP               ((uint32_t)0x00100000) /*!< I2C1 Fast mode plus */
#define SYSCFG_PMC_I2C2_FMP               ((uint32_t)0x00200000) /*!< I2C2 Fast mode plus */
#define SYSCFG_PMC_I2C3_FMP               ((uint32_t)0x00400000) /*!< I2C3 Fast mode plus */
#define SYSCFG_PMC_FPU_IE_0               ((uint32_t)0x04000000) /*!<  Invalid operation Interrupt enable */
#define SYSCFG_PMC_FPU_IE_1               ((uint32_t)0x08000000) /*!<  Divide-by-zero Interrupt enable */
#define SYSCFG_PMC_FPU_IE_2               ((uint32_t)0x10000000) /*!<  Underflow Interrupt enable */
#define SYSCFG_PMC_FPU_IE_3               ((uint32_t)0x20000000) /*!<  Overflow Interrupt enable */
#define SYSCFG_PMC_FPU_IE_4               ((uint32_t)0x40000000) /*!<  Input denormal Interrupt enable */
#define SYSCFG_PMC_FPU_IE_5               ((uint32_t)0x80000000) /*!<  Inexact Interrupt enable (interrupt disabled at reset) */

/*****************  Bit definition for SYSCFG_EXTICR1 register  ***************/
#define SYSCFG_EXTICR1_EXTI0            ((uint16_t)0x0007) /*!<EXTI 0 configuration */
#define SYSCFG_EXTICR1_EXTI1            ((uint16_t)0x0070) /*!<EXTI 1 configuration */
#define SYSCFG_EXTICR1_EXTI2            ((uint16_t)0x0700) /*!<EXTI 2 configuration */
#define SYSCFG_EXTICR1_EXTI3            ((uint16_t)0x7000) /*!<EXTI 3 configuration */
/** 
  * @brief   EXTI0 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI0_PA         ((uint16_t)0x0000) /*!<PA[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PB         ((uint16_t)0x0001) /*!<PB[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PC         ((uint16_t)0x0002) /*!<PC[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PD         ((uint16_t)0x0003) /*!<PD[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PE         ((uint16_t)0x0004) /*!<PE[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PF         ((uint16_t)0x0005) /*!<PF[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PG         ((uint16_t)0x0006) /*!<PG[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PH         ((uint16_t)0x0007) /*!<PH[0] pin */


/** 
  * @brief   EXTI1 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint16_t)0x0000) /*!<PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint16_t)0x0010) /*!<PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC         ((uint16_t)0x0020) /*!<PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PD         ((uint16_t)0x0030) /*!<PD[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PE         ((uint16_t)0x0040) /*!<PE[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PF         ((uint16_t)0x0050) /*!<PF[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PG         ((uint16_t)0x0060) /*!<PG[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PH         ((uint16_t)0x0070) /*!<PH[1] pin */

/** 
  * @brief   EXTI2 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint16_t)0x0000) /*!<PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB         ((uint16_t)0x0100) /*!<PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC         ((uint16_t)0x0200) /*!<PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD         ((uint16_t)0x0300) /*!<PD[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PE         ((uint16_t)0x0400) /*!<PE[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PF         ((uint16_t)0x0500) /*!<PF[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PG         ((uint16_t)0x0600) /*!<PG[2] pin */


/** 
  * @brief   EXTI3 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint16_t)0x0000) /*!<PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB         ((uint16_t)0x1000) /*!<PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC         ((uint16_t)0x2000) /*!<PC[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PD         ((uint16_t)0x3000) /*!<PD[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PE         ((uint16_t)0x4000) /*!<PE[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PF         ((uint16_t)0x5000) /*!<PF[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PG         ((uint16_t)0x6000) /*!<PG[3] pin */


/*****************  Bit definition for SYSCFG_EXTICR2 register  ***************/
#define SYSCFG_EXTICR2_EXTI4            ((uint16_t)0x0007) /*!<EXTI 4 configuration */
#define SYSCFG_EXTICR2_EXTI5            ((uint16_t)0x0070) /*!<EXTI 5 configuration */
#define SYSCFG_EXTICR2_EXTI6            ((uint16_t)0x0700) /*!<EXTI 6 configuration */
#define SYSCFG_EXTICR2_EXTI7            ((uint16_t)0x7000) /*!<EXTI 7 configuration */
/** 
  * @brief   EXTI4 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI4_PA         ((uint16_t)0x0000) /*!<PA[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PB         ((uint16_t)0x0001) /*!<PB[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PC         ((uint16_t)0x0002) /*!<PC[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PD         ((uint16_t)0x0003) /*!<PD[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PE         ((uint16_t)0x0004) /*!<PE[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PF         ((uint16_t)0x0005) /*!<PF[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PG         ((uint16_t)0x0006) /*!<PG[4] pin */

/** 
  * @brief   EXTI5 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint16_t)0x0000) /*!<PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB         ((uint16_t)0x0010) /*!<PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC         ((uint16_t)0x0020) /*!<PC[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PD         ((uint16_t)0x0030) /*!<PD[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PE         ((uint16_t)0x0040) /*!<PE[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PF         ((uint16_t)0x0050) /*!<PF[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PG         ((uint16_t)0x0060) /*!<PG[5] pin */

/** 
  * @brief   EXTI6 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint16_t)0x0000) /*!<PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB         ((uint16_t)0x0100) /*!<PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC         ((uint16_t)0x0200) /*!<PC[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PD         ((uint16_t)0x0300) /*!<PD[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PE         ((uint16_t)0x0400) /*!<PE[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PF         ((uint16_t)0x0500) /*!<PF[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PG         ((uint16_t)0x0600) /*!<PG[6] pin */

/** 
  * @brief   EXTI7 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint16_t)0x0000) /*!<PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB         ((uint16_t)0x1000) /*!<PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC         ((uint16_t)0x2000) /*!<PC[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PD         ((uint16_t)0x3000) /*!<PD[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PE         ((uint16_t)0x4000) /*!<PE[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PF         ((uint16_t)0x5000) /*!<PF[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PG         ((uint16_t)0x6000) /*!<PG[7] pin */


/*****************  Bit definition for SYSCFG_EXTICR3 register  ***************/
#define SYSCFG_EXTICR3_EXTI8            ((uint16_t)0x0007) /*!<EXTI 8 configuration */
#define SYSCFG_EXTICR3_EXTI9            ((uint16_t)0x0070) /*!<EXTI 9 configuration */
#define SYSCFG_EXTICR3_EXTI10           ((uint16_t)0x0700) /*!<EXTI 10 configuration */
#define SYSCFG_EXTICR3_EXTI11           ((uint16_t)0x7000) /*!<EXTI 11 configuration */
           
/** 
  * @brief   EXTI8 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI8_PA         ((uint16_t)0x0000) /*!<PA[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PB         ((uint16_t)0x0001) /*!<PB[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PC         ((uint16_t)0x0002) /*!<PC[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PD         ((uint16_t)0x0003) /*!<PD[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PE         ((uint16_t)0x0004) /*!<PE[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PF         ((uint16_t)0x0005) /*!<PF[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PG         ((uint16_t)0x0006) /*!<PG[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PH         ((uint16_t)0x0007) /*!<PH[8] pin */

/** 
  * @brief   EXTI9 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint16_t)0x0000) /*!<PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB         ((uint16_t)0x0010) /*!<PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC         ((uint16_t)0x0020) /*!<PC[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PD         ((uint16_t)0x0030) /*!<PD[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PE         ((uint16_t)0x0040) /*!<PE[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PF         ((uint16_t)0x0050) /*!<PF[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PG         ((uint16_t)0x0060) /*!<PG[9] pin */

/** 
  * @brief   EXTI10 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint16_t)0x0000) /*!<PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB        ((uint16_t)0x0100) /*!<PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC        ((uint16_t)0x0200) /*!<PC[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PD        ((uint16_t)0x0300) /*!<PD[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PE        ((uint16_t)0x0400) /*!<PE[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PF        ((uint16_t)0x0500) /*!<PF[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PG        ((uint16_t)0x0600) /*!<PG[10] pin */

/** 
  * @brief   EXTI11 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint16_t)0x0000) /*!<PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB        ((uint16_t)0x1000) /*!<PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC        ((uint16_t)0x2000) /*!<PC[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PD        ((uint16_t)0x3000) /*!<PD[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PE        ((uint16_t)0x4000) /*!<PE[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PF        ((uint16_t)0x5000) /*!<PF[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PG        ((uint16_t)0x6000) /*!<PG[11] pin */

/*****************  Bit definition for SYSCFG_EXTICR4 register  ***************/
#define SYSCFG_EXTICR4_EXTI12           ((uint16_t)0x0007) /*!<EXTI 12 configuration */
#define SYSCFG_EXTICR4_EXTI13           ((uint16_t)0x0070) /*!<EXTI 13 configuration */
#define SYSCFG_EXTICR4_EXTI14           ((uint16_t)0x0700) /*!<EXTI 14 configuration */
#define SYSCFG_EXTICR4_EXTI15           ((uint16_t)0x7000) /*!<EXTI 15 configuration */
/** 
  * @brief   EXTI12 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI12_PA        ((uint16_t)0x0000) /*!<PA[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PB        ((uint16_t)0x0001) /*!<PB[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PC        ((uint16_t)0x0002) /*!<PC[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PD        ((uint16_t)0x0003) /*!<PD[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PE        ((uint16_t)0x0004) /*!<PE[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PF        ((uint16_t)0x0005) /*!<PF[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PG        ((uint16_t)0x0006) /*!<PG[12] pin */

/** 
  * @brief   EXTI13 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI13_PA        ((uint16_t)0x0000) /*!<PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB        ((uint16_t)0x0010) /*!<PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC        ((uint16_t)0x0020) /*!<PC[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PD        ((uint16_t)0x0030) /*!<PD[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PE        ((uint16_t)0x0040) /*!<PE[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PF        ((uint16_t)0x0050) /*!<PF[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PG        ((uint16_t)0x0060) /*!<PG[13] pin */

/** 
  * @brief   EXTI14 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI14_PA        ((uint16_t)0x0000) /*!<PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB        ((uint16_t)0x0100) /*!<PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC        ((uint16_t)0x0200) /*!<PC[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PD        ((uint16_t)0x0300) /*!<PD[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PE        ((uint16_t)0x0400) /*!<PE[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PF        ((uint16_t)0x0500) /*!<PF[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PG        ((uint16_t)0x0600) /*!<PG[14] pin */

/** 
  * @brief   EXTI15 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI15_PA        ((uint16_t)0x0000) /*!<PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB        ((uint16_t)0x1000) /*!<PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC        ((uint16_t)0x2000) /*!<PC[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PD        ((uint16_t)0x3000) /*!<PD[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PE        ((uint16_t)0x4000) /*!<PE[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PF        ((uint16_t)0x5000) /*!<PF[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PG        ((uint16_t)0x6000) /*!<PG[15] pin */

/******************  Bit definition for SYSCFG_GPC register  ****************/  
#define SYSCFG_GPC_BRER                 ((uint32_t)0x00000001) /*!< SRAM2 Erase Request */
#define SYSCFG_GPC_BREO                 ((uint32_t)0x00000002) /*!< SRAM2 Erase Ongoing */

/******************  Bit definition for SYSCFG_CBR register  ****************/  
#define SYSCFG_CBR_CLL                 ((uint32_t)0x00000001) /*!< Core Lockup Lock */
#define SYSCFG_CBR_SPL                 ((uint32_t)0x00000002) /*!< SRAM Parity Lock*/
#define SYSCFG_CBR_PVDL                ((uint32_t)0x00000004) /*!<  PVD Lock */
#define SYSCFG_CBR_ECCL                ((uint32_t)0x00000008) /*!< ECC Lock*/
#define SYSCFG_CBR_SPF                 ((uint32_t)0x00000100) /*!< SRAM Parity Flag */
/******************  Bit definition for SYSCFG_SWP register  ****************/ 
#define SYSCFG_SWP_PAGE0         ((uint32_t)0x00000001)  /*!< SRAM2 Write protection page 0 */
#define SYSCFG_SWP_PAGE1          ((uint32_t)0x00000002) /*!< SRAM2 Write protection page 1 */
#define SYSCFG_SWP_PAGE2          ((uint32_t)0x00000004) /*!< SRAM2 Write protection page 2 */
#define SYSCFG_SWP_PAGE3          ((uint32_t)0x00000008) /*!< SRAM2 Write protection page 3 */
#define SYSCFG_SWP_PAGE4          ((uint32_t)0x00000010) /*!< SRAM2 Write protection page 4 */
#define SYSCFG_SWP_PAGE5          ((uint32_t)0x00000020) /*!< SRAM2 Write protection page 5 */
#define SYSCFG_SWP_PAGE6          ((uint32_t)0x00000040) /*!< SRAM2 Write protection page 6 */
#define SYSCFG_SWP_PAGE7          ((uint32_t)0x00000080) /*!< SRAM2 Write protection page 7 */
#define SYSCFG_SWP_PAGE8          ((uint32_t)0x00000100) /*!< SRAM2 Write protection page 8 */
#define SYSCFG_SWP_PAGE9          ((uint32_t)0x00000200) /*!< SRAM2 Write protection page 9 */
#define SYSCFG_SWP_PAGE10         ((uint32_t)0x00000400) /*!< SRAM2 Write protection page 10*/
#define SYSCFG_SWP_PAGE11         ((uint32_t)0x00000800) /*!< SRAM2 Write protection page 11*/
#define SYSCFG_SWP_PAGE12         ((uint32_t)0x00001000) /*!< SRAM2 Write protection page 12*/
#define SYSCFG_SWP_PAGE13         ((uint32_t)0x00002000) /*!< SRAM2 Write protection page 13*/
#define SYSCFG_SWP_PAGE14         ((uint32_t)0x00004000) /*!< SRAM2 Write protection page 14*/
#define SYSCFG_SWP_PAGE15         ((uint32_t)0x00008000) /*!< SRAM2 Write protection page 15*/
#define SYSCFG_SWP_PAGE16         ((uint32_t)0x00010000) /*!< SRAM2 Write protection page 16*/
#define SYSCFG_SWP_PAGE17         ((uint32_t)0x00020000) /*!< SRAM2 Write protection page 17*/
#define SYSCFG_SWP_PAGE18         ((uint32_t)0x00040000) /*!< SRAM2 Write protection page 18*/
#define SYSCFG_SWP_PAGE19         ((uint32_t)0x00080000) /*!< SRAM2 Write protection page 19*/
#define SYSCFG_SWP_PAGE20         ((uint32_t)0x00100000) /*!< SRAM2 Write protection page 20*/
#define SYSCFG_SWP_PAGE21         ((uint32_t)0x00200000) /*!< SRAM2 Write protection page 21*/
#define SYSCFG_SWP_PAGE22         ((uint32_t)0x00400000) /*!< SRAM2 Write protection page 22*/
#define SYSCFG_SWP_PAGE23         ((uint32_t)0x00800000) /*!< SRAM2 Write protection page 23*/
#define SYSCFG_SWP_PAGE24         ((uint32_t)0x01000000) /*!< SRAM2 Write protection page 24*/
#define SYSCFG_SWP_PAGE25         ((uint32_t)0x02000000) /*!< SRAM2 Write protection page 25*/
#define SYSCFG_SWP_PAGE26         ((uint32_t)0x04000000) /*!< SRAM2 Write protection page 26*/
#define SYSCFG_SWP_PAGE27         ((uint32_t)0x08000000) /*!< SRAM2 Write protection page 27*/
#define SYSCFG_SWP_PAGE28         ((uint32_t)0x10000000) /*!< SRAM2 Write protection page 28*/
#define SYSCFG_SWP_PAGE29         ((uint32_t)0x20000000) /*!< SRAM2 Write protection page 29*/
#define SYSCFG_SWP_PAGE30         ((uint32_t)0x40000000) /*!< SRAM2 Write protection page 30*/
#define SYSCFG_SWP_PAGE31         ((uint32_t)0x80000000) /*!< SRAM2 Write protection page 31*/
/******************  Bit definition for SYSCFG_VCSR register  ****************/  
#define SYSCFG_VCSR_ENVR          ((uint32_t)0x00000001) /*!< Enable Voltage Reference  */
#define SYSCFG_VCSR_HIZ           ((uint32_t)0x00000002) /*!< High impedence mode for the VREF_BUF*/
#define SYSCFG_VCSR_VRS           ((uint32_t)0x00000004) /*!< Voltage Reference Scale */
#define SYSCFG_VCSR_VRR           ((uint32_t)0x00000008) /*!< Voltage Reference Ready*/
#define SYSCFG_VCSR_LP            ((uint32_t)0x00000010) /*!< Low Power mode */
#define SYSCFG_VCSR_LPS           ((uint32_t)0x00000020) /*!< Low Power Stop mode */
/******************  Bit definition for SYSCFG_VCCR register  ****************/  
#define SYSCFG_VCCR_TRIM          ((uint32_t)0x0000001F) /*!<  VREF_BUF  Trimming code  */



/******************************************************************************/
/*                                                                            */
/*                                    TIM                                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                         ((uint16_t)0x0001)            /*!<Counter enable */
#define  TIM_CR1_UDIS                        ((uint16_t)0x0002)            /*!<Update disable */
#define  TIM_CR1_URS                         ((uint16_t)0x0004)            /*!<Update request source */
#define  TIM_CR1_OPM                         ((uint16_t)0x0008)            /*!<One pulse mode */
#define  TIM_CR1_DIR                         ((uint16_t)0x0010)            /*!<Direction */

#define  TIM_CR1_CMS                         ((uint16_t)0x0060)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       ((uint16_t)0x0020)            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                       ((uint16_t)0x0040)            /*!<Bit 1 */

#define  TIM_CR1_ARPE                        ((uint16_t)0x0080)            /*!<Auto-reload preload enable */

#define  TIM_CR1_CKD                         ((uint16_t)0x0300)            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                       ((uint16_t)0x0200)            /*!<Bit 1 */

#define  TIM_CR1_UIFREMAP                    ((uint16_t)0x0800)            /*!<Update interrupt flag remap */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        ((uint32_t)0x00000001)            /*!<Capture/Compare Preloaded Control */
#define  TIM_CR2_CCUS                        ((uint32_t)0x00000004)            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint32_t)0x00000008)            /*!<Capture/Compare DMA Selection */

#define  TIM_CR2_MMS                         ((uint32_t)0x00000070)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint32_t)0x00000040)            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        ((uint32_t)0x00000080)            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint32_t)0x00000100)            /*!<Output Idle state 1 (OC1 output) */
#define  TIM_CR2_OIS1N                       ((uint32_t)0x00000200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint32_t)0x00000400)            /*!<Output Idle state 2 (OC2 output) */
#define  TIM_CR2_OIS2N                       ((uint32_t)0x00000800)            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint32_t)0x00001000)            /*!<Output Idle state 3 (OC3 output) */
#define  TIM_CR2_OIS3N                       ((uint32_t)0x00002000)            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint32_t)0x00004000)            /*!<Output Idle state 4 (OC4 output) */
#define  TIM_CR2_OIS5                        ((uint32_t)0x00010000)            /*!<Output Idle state 4 (OC4 output) */
#define  TIM_CR2_OIS6                        ((uint32_t)0x00020000)            /*!<Output Idle state 4 (OC4 output) */

#define  TIM_CR2_MMS2                        ((uint32_t)0x00F00000)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS2_0                      ((uint32_t)0x00100000)            /*!<Bit 0 */
#define  TIM_CR2_MMS2_1                      ((uint32_t)0x00200000)            /*!<Bit 1 */
#define  TIM_CR2_MMS2_2                      ((uint32_t)0x00400000)            /*!<Bit 2 */
#define  TIM_CR2_MMS2_3                      ((uint32_t)0x00800000)            /*!<Bit 2 */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define  TIM_SMCR_SMS                        ((uint32_t)0x00010007)            /*!<SMS[2:0] bits (Slave mode selection) */
#define  TIM_SMCR_SMS_0                      ((uint32_t)0x00000001)            /*!<Bit 0 */
#define  TIM_SMCR_SMS_1                      ((uint32_t)0x00000002)            /*!<Bit 1 */
#define  TIM_SMCR_SMS_2                      ((uint32_t)0x00000004)            /*!<Bit 2 */
#define  TIM_SMCR_SMS_3                      ((uint32_t)0x00010000)            /*!<Bit 3 */

#define  TIM_SMCR_OCCS                       ((uint32_t)0x00000008)            /*!< OCREF clear selection */

#define  TIM_SMCR_TS                         ((uint32_t)0x00000070)            /*!<TS[2:0] bits (Trigger selection) */
#define  TIM_SMCR_TS_0                       ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_SMCR_TS_1                       ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_SMCR_TS_2                       ((uint32_t)0x00000040)            /*!<Bit 2 */

#define  TIM_SMCR_MSM                        ((uint32_t)0x00000080)            /*!<Master/slave mode */

#define  TIM_SMCR_ETF                        ((uint32_t)0x00000F00)            /*!<ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      ((uint32_t)0x00000100)            /*!<Bit 0 */
#define  TIM_SMCR_ETF_1                      ((uint32_t)0x00000200)            /*!<Bit 1 */
#define  TIM_SMCR_ETF_2                      ((uint32_t)0x00000400)            /*!<Bit 2 */
#define  TIM_SMCR_ETF_3                      ((uint32_t)0x00000800)            /*!<Bit 3 */

#define  TIM_SMCR_ETPS                       ((uint32_t)0x00003000)            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     ((uint32_t)0x00001000)            /*!<Bit 0 */
#define  TIM_SMCR_ETPS_1                     ((uint32_t)0x00002000)            /*!<Bit 1 */

#define  TIM_SMCR_ECE                        ((uint32_t)0x00004000)            /*!<External clock enable */
#define  TIM_SMCR_ETP                        ((uint32_t)0x00008000)            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint16_t)0x0001)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint16_t)0x0002)            /*!<Capture/Compare 1 interrupt enable */
#define  TIM_DIER_CC2IE                      ((uint16_t)0x0004)            /*!<Capture/Compare 2 interrupt enable */
#define  TIM_DIER_CC3IE                      ((uint16_t)0x0008)            /*!<Capture/Compare 3 interrupt enable */
#define  TIM_DIER_CC4IE                      ((uint16_t)0x0010)            /*!<Capture/Compare 4 interrupt enable */
#define  TIM_DIER_COMIE                      ((uint16_t)0x0020)            /*!<COM interrupt enable */
#define  TIM_DIER_TIE                        ((uint16_t)0x0040)            /*!<Trigger interrupt enable */
#define  TIM_DIER_BIE                        ((uint16_t)0x0080)            /*!<Break interrupt enable */
#define  TIM_DIER_UDE                        ((uint16_t)0x0100)            /*!<Update DMA request enable */
#define  TIM_DIER_CC1DE                      ((uint16_t)0x0200)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint16_t)0x0400)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint16_t)0x0800)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint16_t)0x1000)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint16_t)0x2000)            /*!<COM DMA request enable */
#define  TIM_DIER_TDE                        ((uint16_t)0x4000)            /*!<Trigger DMA request enable */

/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          ((uint32_t)0x00000001)            /*!<Update interrupt Flag */
#define  TIM_SR_CC1IF                        ((uint32_t)0x00000002)            /*!<Capture/Compare 1 interrupt Flag */
#define  TIM_SR_CC2IF                        ((uint32_t)0x00000004)            /*!<Capture/Compare 2 interrupt Flag */
#define  TIM_SR_CC3IF                        ((uint32_t)0x00000008)            /*!<Capture/Compare 3 interrupt Flag */
#define  TIM_SR_CC4IF                        ((uint32_t)0x00000010)            /*!<Capture/Compare 4 interrupt Flag */
#define  TIM_SR_COMIF                        ((uint32_t)0x00000020)            /*!<COM interrupt Flag */
#define  TIM_SR_TIF                          ((uint32_t)0x00000040)            /*!<Trigger interrupt Flag */
#define  TIM_SR_BIF                          ((uint32_t)0x00000080)            /*!<Break interrupt Flag */
#define  TIM_SR_B2IF                         ((uint32_t)0x00000100)            /*!<Break2 interrupt Flag */
#define  TIM_SR_CC1OF                        ((uint32_t)0x00000200)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint32_t)0x00000400)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint32_t)0x00000800)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint32_t)0x00001000)            /*!<Capture/Compare 4 Overcapture Flag */
#define  TIM_SR_CC5IF                        ((uint32_t)0x00010000)            /*!<Capture/Compare 5 interrupt Flag */
#define  TIM_SR_CC6IF                        ((uint32_t)0x00020000)            /*!<Capture/Compare 6 interrupt Flag */


/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          ((uint16_t)0x0001)               /*!<Update Generation */
#define  TIM_EGR_CC1G                        ((uint16_t)0x0002)               /*!<Capture/Compare 1 Generation */
#define  TIM_EGR_CC2G                        ((uint16_t)0x0004)               /*!<Capture/Compare 2 Generation */
#define  TIM_EGR_CC3G                        ((uint16_t)0x0008)               /*!<Capture/Compare 3 Generation */
#define  TIM_EGR_CC4G                        ((uint16_t)0x0010)               /*!<Capture/Compare 4 Generation */
#define  TIM_EGR_COMG                        ((uint16_t)0x0020)               /*!<Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          ((uint16_t)0x0040)               /*!<Trigger Generation */
#define  TIM_EGR_BG                          ((uint16_t)0x0080)               /*!<Break Generation */
#define  TIM_EGR_B2G                         ((uint16_t)0x0100)               /*!<Break Generation */


/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      ((uint32_t)0x00000003)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint32_t)0x00000001)            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint32_t)0x00000002)            /*!<Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint32_t)0x00000004)            /*!<Output Compare 1 Fast enable */
#define  TIM_CCMR1_OC1PE                     ((uint32_t)0x00000008)            /*!<Output Compare 1 Preload enable */

#define  TIM_CCMR1_OC1M                      ((uint32_t)0x00010070)            /*!<OC1M[2:0] bits (Output Compare 1 Mode) */
#define  TIM_CCMR1_OC1M_0                    ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint32_t)0x00000040)            /*!<Bit 2 */
#define  TIM_CCMR1_OC1M_3                    ((uint32_t)0x00010000)            /*!<Bit 3 */

#define  TIM_CCMR1_OC1CE                     ((uint32_t)0x00000080)            /*!<Output Compare 1Clear Enable */

#define  TIM_CCMR1_CC2S                      ((uint32_t)0x00000300)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint32_t)0x00000100)            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint32_t)0x00000200)            /*!<Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint32_t)0x00000400)            /*!<Output Compare 2 Fast enable */
#define  TIM_CCMR1_OC2PE                     ((uint32_t)0x00000800)            /*!<Output Compare 2 Preload enable */

#define  TIM_CCMR1_OC2M                      ((uint32_t)0x01007000)            /*!<OC2M[2:0] bits (Output Compare 2 Mode) */
#define  TIM_CCMR1_OC2M_0                    ((uint32_t)0x00001000)            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint32_t)0x00002000)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint32_t)0x00004000)            /*!<Bit 2 */
#define  TIM_CCMR1_OC2M_3                    ((uint32_t)0x01000000)            /*!<Bit 3 */

#define  TIM_CCMR1_OC2CE                     ((uint32_t)0x00008000)            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    ((uint32_t)0x0000000C)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint32_t)0x00000004)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint32_t)0x00000008)            /*!<Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint32_t)0x000000F0)            /*!<IC1F[3:0] bits (Input Capture 1 Filter) */
#define  TIM_CCMR1_IC1F_0                    ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint32_t)0x00000040)            /*!<Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint32_t)0x00000080)            /*!<Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint32_t)0x00000C00)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define  TIM_CCMR1_IC2PSC_0                  ((uint32_t)0x00000400)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint32_t)0x00000800)            /*!<Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint32_t)0x0000F000)            /*!<IC2F[3:0] bits (Input Capture 2 Filter) */
#define  TIM_CCMR1_IC2F_0                    ((uint32_t)0x00001000)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint32_t)0x00002000)            /*!<Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint32_t)0x00004000)            /*!<Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint32_t)0x00008000)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      ((uint32_t)0x00000003)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define  TIM_CCMR2_CC3S_0                    ((uint32_t)0x00000001)            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint32_t)0x00000002)            /*!<Bit 1 */

#define  TIM_CCMR2_OC3FE                     ((uint32_t)0x00000004)            /*!<Output Compare 3 Fast enable */
#define  TIM_CCMR2_OC3PE                     ((uint32_t)0x00000008)            /*!<Output Compare 3 Preload enable */

#define  TIM_CCMR2_OC3M                      ((uint32_t)0x00000070)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint32_t)0x00000040)            /*!<Bit 2 */
#define  TIM_CCMR2_OC3M_3                    ((uint32_t)0x00010000)            /*!<Bit 3 */

#define  TIM_CCMR2_OC3CE                     ((uint32_t)0x00000080)            /*!<Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      ((uint32_t)0x00000300)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint32_t)0x00000100)            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint32_t)0x00000200)            /*!<Bit 1 */

#define  TIM_CCMR2_OC4FE                     ((uint32_t)0x00000400)            /*!<Output Compare 4 Fast enable */
#define  TIM_CCMR2_OC4PE                     ((uint32_t)0x00000800)            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      ((uint32_t)0x00007000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint32_t)0x00001000)            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint32_t)0x00002000)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint32_t)0x00004000)            /*!<Bit 2 */
#define  TIM_CCMR2_OC4M_3                    ((uint32_t)0x00100000)            /*!<Bit 3 */

#define  TIM_CCMR2_OC4CE                     ((uint32_t)0x00008000)            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    ((uint16_t)0x0000000C)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  ((uint16_t)0x00000004)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  ((uint16_t)0x00000008)            /*!<Bit 1 */

#define  TIM_CCMR2_IC3F                      ((uint16_t)0x000000F0)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    ((uint16_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3F_1                    ((uint16_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CCMR2_IC3F_2                    ((uint16_t)0x00000040)            /*!<Bit 2 */
#define  TIM_CCMR2_IC3F_3                    ((uint16_t)0x00000080)            /*!<Bit 3 */

#define  TIM_CCMR2_IC4PSC                    ((uint16_t)0x00000C00)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  ((uint16_t)0x00000400)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  ((uint16_t)0x00000800)            /*!<Bit 1 */

#define  TIM_CCMR2_IC4F                      ((uint16_t)0x0000F000)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    ((uint16_t)0x00001000)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4F_1                    ((uint16_t)0x00002000)            /*!<Bit 1 */
#define  TIM_CCMR2_IC4F_2                    ((uint16_t)0x00004000)            /*!<Bit 2 */
#define  TIM_CCMR2_IC4F_3                    ((uint16_t)0x00008000)            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint32_t)0x00000001)            /*!<Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       ((uint32_t)0x00000002)            /*!<Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NE                      ((uint32_t)0x00000004)            /*!<Capture/Compare 1 Complementary output enable */
#define  TIM_CCER_CC1NP                      ((uint32_t)0x00000008)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint32_t)0x00000010)            /*!<Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       ((uint32_t)0x00000020)            /*!<Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NE                      ((uint32_t)0x00000040)            /*!<Capture/Compare 2 Complementary output enable */
#define  TIM_CCER_CC2NP                      ((uint32_t)0x00000080)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint32_t)0x00000100)            /*!<Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       ((uint32_t)0x00000200)            /*!<Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NE                      ((uint32_t)0x00000400)            /*!<Capture/Compare 3 Complementary output enable */
#define  TIM_CCER_CC3NP                      ((uint32_t)0x00000800)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint32_t)0x00001000)            /*!<Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint32_t)0x00002000)            /*!<Capture/Compare 4 output Polarity */
#define  TIM_CCER_CC4NP                      ((uint32_t)0x00008000)            /*!<Capture/Compare 4 Complementary output Polarity */
#define  TIM_CCER_CC5E                       ((uint32_t)0x00010000)            /*!<Capture/Compare 5 output enable */
#define  TIM_CCER_CC5P                       ((uint32_t)0x00020000)            /*!<Capture/Compare 5 output Polarity */
#define  TIM_CCER_CC6E                       ((uint32_t)0x00100000)            /*!<Capture/Compare 6 output enable */
#define  TIM_CCER_CC6P                       ((uint32_t)0x00200000)            /*!<Capture/Compare 6 output Polarity */
/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         ((uint32_t)0xFFFFFFFF)            /*!<Counter Value */
#define  TIM_CNT_UIFCPY                      ((uint32_t)0x80000000)                                        /*!<Update interrupt flag copy */
/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         ((uint16_t)0xFFFF)            /*!<Prescaler Value */

/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         ((uint32_t)0xFFFFFFFF)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  ********************/
#define  TIM_RCR_REP                         ((uint8_t)0xFF)               /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       ((uint16_t)0xFFFF)            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       ((uint16_t)0xFFFF)            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       ((uint16_t)0xFFFF)            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       ((uint16_t)0xFFFF)            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIM_CCR5 register  *******************/
#define  TIM_CCR5_CCR5                       ((uint32_t)0xFFFFFFFF)        /*!<Capture/Compare 5 Value */
#define  TIM_CCR5_GC5C1                      ((uint32_t)0x20000000)        /*!<Group Channel 5 and Channel 1 */
#define  TIM_CCR5_GC5C2                      ((uint32_t)0x40000000)        /*!<Group Channel 5 and Channel 2 */
#define  TIM_CCR5_GC5C3                      ((uint32_t)0x80000000)        /*!<Group Channel 5 and Channel 3 */

/*******************  Bit definition for TIM_CCR6 register  *******************/
#define  TIM_CCR6_CCR6                       ((uint16_t)0xFFFF)            /*!<Capture/Compare 6 Value */

/*******************  Bit definition for TIM_BDTR register  *******************/
#define  TIM_BDTR_DTG                        ((uint32_t)0x000000FF)            /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      ((uint32_t)0x00000001)            /*!<Bit 0 */
#define  TIM_BDTR_DTG_1                      ((uint32_t)0x00000002)            /*!<Bit 1 */
#define  TIM_BDTR_DTG_2                      ((uint32_t)0x00000004)            /*!<Bit 2 */
#define  TIM_BDTR_DTG_3                      ((uint32_t)0x00000008)            /*!<Bit 3 */
#define  TIM_BDTR_DTG_4                      ((uint32_t)0x00000010)            /*!<Bit 4 */
#define  TIM_BDTR_DTG_5                      ((uint32_t)0x00000020)            /*!<Bit 5 */
#define  TIM_BDTR_DTG_6                      ((uint32_t)0x00000040)            /*!<Bit 6 */
#define  TIM_BDTR_DTG_7                      ((uint32_t)0x00000080)            /*!<Bit 7 */

#define  TIM_BDTR_LOCK                       ((uint32_t)0x00000300)            /*!<LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     ((uint32_t)0x00000100)            /*!<Bit 0 */
#define  TIM_BDTR_LOCK_1                     ((uint32_t)0x00000200)            /*!<Bit 1 */

#define  TIM_BDTR_OSSI                       ((uint32_t)0x00000400)            /*!<Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       ((uint32_t)0x00000800)            /*!<Off-State Selection for Run mode */
#define  TIM_BDTR_BKE                        ((uint32_t)0x00001000)            /*!<Break enable for Break1 */
#define  TIM_BDTR_BKP                        ((uint32_t)0x00002000)            /*!<Break Polarity for Break1 */
#define  TIM_BDTR_AOE                        ((uint32_t)0x00004000)            /*!<Automatic Output enable */
#define  TIM_BDTR_MOE                        ((uint32_t)0x00008000)            /*!<Main Output enable */

#define  TIM_BDTR_BKF                        ((uint32_t)0x000F0000)            /*!<Break Filter for Break1 */
#define  TIM_BDTR_BK2F                       ((uint32_t)0x00F00000)            /*!<Break Filter for Break2 */

#define  TIM_BDTR_BK2E                       ((uint32_t)0x01000000)            /*!<Break enable for Break2 */
#define  TIM_BDTR_BK2P                       ((uint32_t)0x02000000)            /*!<Break Polarity for Break2 */

/*******************  Bit definition for TIM_DCR register  ********************/
#define  TIM_DCR_DBA                         ((uint16_t)0x001F)            /*!<DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIM_DCR_DBA_1                       ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIM_DCR_DBA_2                       ((uint16_t)0x0004)            /*!<Bit 2 */
#define  TIM_DCR_DBA_3                       ((uint16_t)0x0008)            /*!<Bit 3 */
#define  TIM_DCR_DBA_4                       ((uint16_t)0x0010)            /*!<Bit 4 */

#define  TIM_DCR_DBL                         ((uint16_t)0x1F00)            /*!<DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIM_DCR_DBL_1                       ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIM_DCR_DBL_2                       ((uint16_t)0x0400)            /*!<Bit 2 */
#define  TIM_DCR_DBL_3                       ((uint16_t)0x0800)            /*!<Bit 3 */
#define  TIM_DCR_DBL_4                       ((uint16_t)0x1000)            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       ((uint16_t)0xFFFF)            /*!<DMA register for burst accesses */

/*******************  Bit definition for TIM16_OR register  *********************/
#define TIM16_OR_TI1_RMP                     ((uint16_t)0x00C0)            /*!<TI1_RMP[1:0] bits (TIM16 Input 1 remap) */
#define TIM16_OR_TI1_RMP_0                   ((uint16_t)0x0040)            /*!<Bit 0 */
#define TIM16_OR_TI1_RMP_1                   ((uint16_t)0x0080)            /*!<Bit 1 */

/*******************  Bit definition for TIM1_OR register  *********************/
#define TIM1_OR_ETR_RMP                      ((uint16_t)0x000F)            /*!<ETR_RMP[3:0] bits (TIM1 ETR remap) */
#define TIM1_OR_ETR_RMP_0                    ((uint16_t)0x0001)            /*!<Bit 0 */
#define TIM1_OR_ETR_RMP_1                    ((uint16_t)0x0002)            /*!<Bit 1 */
#define TIM1_OR_ETR_RMP_2                    ((uint16_t)0x0004)            /*!<Bit 2 */
#define TIM1_OR_ETR_RMP_3                    ((uint16_t)0x0008)            /*!<Bit 3 */

/*******************  Bit definition for TIM8_OR register  *********************/
#define TIM8_OR_ETR_RMP                      ((uint16_t)0x000F)            /*!<ETR_RMP[3:0] bits (TIM8 ETR remap) */
#define TIM8_OR_ETR_RMP_0                    ((uint16_t)0x0001)            /*!<Bit 0 */
#define TIM8_OR_ETR_RMP_1                    ((uint16_t)0x0002)            /*!<Bit 1 */
#define TIM8_OR_ETR_RMP_2                    ((uint16_t)0x0004)            /*!<Bit 2 */
#define TIM8_OR_ETR_RMP_3                    ((uint16_t)0x0008)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR3 register  *******************/
#define  TIM_CCMR3_OC5FE                     ((uint32_t)0x00000004)            /*!<Output Compare 5 Fast enable */
#define  TIM_CCMR3_OC5PE                     ((uint32_t)0x00000008)            /*!<Output Compare 5 Preload enable */

#define  TIM_CCMR3_OC5M                      ((uint32_t)0x00000070)            /*!<OC5M[2:0] bits (Output Compare 5 Mode) */
#define  TIM_CCMR3_OC5M_0                    ((uint32_t)0x00000010)            /*!<Bit 0 */
#define  TIM_CCMR3_OC5M_1                    ((uint32_t)0x00000020)            /*!<Bit 1 */
#define  TIM_CCMR3_OC5M_2                    ((uint32_t)0x00000040)            /*!<Bit 2 */
#define  TIM_CCMR3_OC5M_3                    ((uint32_t)0x00010000)            /*!<Bit 3 */

#define  TIM_CCMR3_OC5CE                     ((uint32_t)0x00000080)            /*!<Output Compare 5 Clear Enable */

#define  TIM_CCMR3_OC6FE                     ((uint32_t)0x00000400)            /*!<Output Compare 4 Fast enable */
#define  TIM_CCMR3_OC6PE                     ((uint32_t)0x00000800)            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR3_OC6M                      ((uint32_t)0x00007000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR3_OC6M_0                    ((uint32_t)0x00001000)            /*!<Bit 0 */
#define  TIM_CCMR3_OC6M_1                    ((uint32_t)0x00002000)            /*!<Bit 1 */
#define  TIM_CCMR3_OC6M_2                    ((uint32_t)0x00004000)            /*!<Bit 2 */
#define  TIM_CCMR3_OC6M_3                    ((uint32_t)0x00100000)            /*!<Bit 3 */

#define  TIM_CCMR3_OC6CE                     ((uint32_t)0x00008000)            /*!<Output Compare 4 Clear Enable */
/******************************************************************************/
/*                                                                            */
/*                         Low Power Timer (LPTTIM)                           */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for LPTIM_ISR register  *******************/
#define  LPTIM_ISR_CMPM                         ((uint32_t)0x00000001)            /*!< Compare match */
#define  LPTIM_ISR_ARRM                         ((uint32_t)0x00000002)            /*!< Autoreload match */
#define  LPTIM_ISR_EXTTRIG                      ((uint32_t)0x00000004)            /*!< External trigger edge event */
#define  LPTIM_ISR_CMPOK                        ((uint32_t)0x00000008)            /*!< Compare register update OK */
#define  LPTIM_ISR_ARROK                        ((uint32_t)0x00000010)            /*!< Autoreload register update OK */
#define  LPTIM_ISR_UP                           ((uint32_t)0x00000020)            /*!< Counter direction change down to up */
#define  LPTIM_ISR_DOWN                         ((uint32_t)0x00000040)            /*!< Counter direction change up to down */

/******************  Bit definition for LPTIM_ICR register  *******************/
#define  LPTIM_ICR_CMPMCF                       ((uint32_t)0x00000001)            /*!< Compare match Clear Flag */
#define  LPTIM_ICR_ARRMCF                       ((uint32_t)0x00000002)            /*!< Autoreload match Clear Flag */
#define  LPTIM_ICR_EXTTRIGCF                    ((uint32_t)0x00000004)            /*!< External trigger edge event Clear Flag */
#define  LPTIM_ICR_CMPOKCF                      ((uint32_t)0x00000008)            /*!< Compare register update OK Clear Flag */
#define  LPTIM_ICR_ARROKCF                      ((uint32_t)0x00000010)            /*!< Autoreload register update OK Clear Flag */
#define  LPTIM_ICR_UPCF                         ((uint32_t)0x00000020)            /*!< Counter direction change down to up Clear Flag */
#define  LPTIM_ICR_DOWNCF                       ((uint32_t)0x00000040)            /*!< Counter direction change up to down Clear Flag */

/******************  Bit definition for LPTIM_IER register ********************/
#define  LPTIM_IER_CMPMIE                       ((uint32_t)0x00000001)            /*!< Compare match Interrupt Enable */
#define  LPTIM_IER_ARRMIE                       ((uint32_t)0x00000002)            /*!< Autoreload match Interrupt Enable */
#define  LPTIM_IER_EXTTRIGIE                    ((uint32_t)0x00000004)            /*!< External trigger edge event Interrupt Enable */
#define  LPTIM_IER_CMPOKIE                      ((uint32_t)0x00000008)            /*!< Compare register update OK Interrupt Enable */
#define  LPTIM_IER_ARROKIE                      ((uint32_t)0x00000010)            /*!< Autoreload register update OK Interrupt Enable */
#define  LPTIM_IER_UPIE                         ((uint32_t)0x00000020)            /*!< Counter direction change down to up Interrupt Enable */
#define  LPTIM_IER_DOWNIE                       ((uint32_t)0x00000040)            /*!< Counter direction change up to down Interrupt Enable */

/******************  Bit definition for LPTIM_CFGR register *******************/
#define  LPTIM_CFGR_CKSEL                       ((uint32_t)0x00000001)             /*!< Clock selector */

#define  LPTIM_CFGR_CKPOL                       ((uint32_t)0x00000006)             /*!< CKPOL[1:0] bits (Clock polarity) */
#define  LPTIM_CFGR_CKPOL_0                     ((uint32_t)0x00000002)             /*!< Bit 0 */
#define  LPTIM_CFGR_CKPOL_1                     ((uint32_t)0x00000004)             /*!< Bit 1 */

#define  LPTIM_CFGR_CKFLT                       ((uint32_t)0x00000018)             /*!< CKFLT[1:0] bits (Configurable digital filter for external clock) */
#define  LPTIM_CFGR_CKFLT_0                     ((uint32_t)0x00000008)             /*!< Bit 0 */
#define  LPTIM_CFGR_CKFLT_1                     (uint32_t)0x000000010)             /*!< Bit 1 */

#define  LPTIM_CFGR_TRGFLT                      ((uint32_t)0x000000C0)             /*!< TRGFLT[1:0] bits (Configurable digital filter for trigger) */
#define  LPTIM_CFGR_TRGFLT_0                    ((uint32_t)0x00000040)             /*!< Bit 0 */
#define  LPTIM_CFGR_TRGFLT_1                    ((uint32_t)0x00000080)             /*!< Bit 1 */

#define  LPTIM_CFGR_PRESC                       ((uint32_t)0x00000E00)             /*!< PRESC[2:0] bits (Clock prescaler) */
#define  LPTIM_CFGR_PRESC_0                     ((uint32_t)0x00000200)             /*!< Bit 0 */
#define  LPTIM_CFGR_PRESC_1                     ((uint32_t)0x00000400)             /*!< Bit 1 */
#define  LPTIM_CFGR_PRESC_2                     ((uint32_t)0x00000800)             /*!< Bit 2 */

#define  LPTIM_CFGR_TRIGSEL                     ((uint32_t)0x0000E000)             /*!< TRIGSEL[2:0]] bits (Trigger selector) */
#define  LPTIM_CFGR_TRIGSEL_0                   ((uint32_t)0x00002000)             /*!< Bit 0 */
#define  LPTIM_CFGR_TRIGSEL_1                   ((uint32_t)0x00004000)             /*!< Bit 1 */
#define  LPTIM_CFGR_TRIGSEL_2                   ((uint32_t)0x00008000)             /*!< Bit 2 */

#define  LPTIM_CFGR_TRIGEN                      ((uint32_t)0x00060000)             /*!< TRIGEN[1:0] bits (Trigger enable and polarity) */
#define  LPTIM_CFGR_TRIGEN_0                    ((uint32_t)0x00020000)             /*!< Bit 0 */
#define  LPTIM_CFGR_TRIGEN_1                    ((uint32_t)0x00040000)             /*!< Bit 1 */

#define  LPTIM_CFGR_TIMOUT                      ((uint32_t)0x00080000)             /*!< Timout enable */
#define  LPTIM_CFGR_WAVE                        ((uint32_t)0x00100000)             /*!< Waveform shape */
#define  LPTIM_CFGR_WAVPOL                      ((uint32_t)0x00200000)             /*!< Waveform shape polarity */
#define  LPTIM_CFGR_PRELOAD                     ((uint32_t)0x00400000)             /*!< Reg update mode */
#define  LPTIM_CFGR_COUNTMODE                   ((uint32_t)0x00800000)             /*!< Counter mode enable */      //////////
#define  LPTIM_CFGR_ENC                         ((uint32_t)0x01000000)             /*!< Encoder mode enable */           //////////

/******************  Bit definition for LPTIM_CR register  ********************/
#define  LPTIM_CR_ENABLE                        ((uint32_t)0x00000001)             /*!< LPTIMer enable */
#define  LPTIM_CR_SNGSTRT                       ((uint32_t)0x00080002)             /*!< Timer start in single mode */
#define  LPTIM_CR_CNTSTRT                       ((uint32_t)0x00000004)             /*!< Timer start in continuous mode */

/******************  Bit definition for LPTIM_CMP register  *******************/
#define  LPTIM_CMP_CMP                          ((uint32_t)0x0000FFFF)             /*!< Compare register */

/******************  Bit definition for LPTIM_ARR register  *******************/
#define  LPTIM_ARR_ARR                          ((uint32_t)0x0000FFFF)             /*!< Auto reload register */

/******************  Bit definition for LPTIM_CNT register  *******************/
#define  LPTIM_CNT_CNT                          ((uint32_t)0x0000FFFF)             /*!< Counter register */

/******************  Bit definition for LPTIM_OR register  *******************/
#define  LPTIM_OR_OR                           ((uint32_t)0x00000003)               /*!< LPTIMER[1:0] bits (Remap selection) */
#define  LPTIM_OR_OR_0                         ((uint32_t)0x00000001)               /*!< Bit 0 */
#define  LPTIM_OR_OR_1                         ((uint32_t)0x00000002)               /*!< Bit 1 */

/******************************************************************************/
/*                                                                            */
/*                      Analog Comparators (COMP)                             */
/*                                                                            */
/******************************************************************************/
/**********************  Bit definition for COMP1_CSR register  ***************/
#define COMP1_CTRL_COMP1_EN              ((uint32_t)0x00000001) /*!< COMP1 enable */
#define COMP1_CTRL_COMP1_PWR_MODE        ((uint32_t)0x0000000C) /*!< COMP1 power mode */
#define COMP1_CTRL_COMP1_PWR_MODE_0      ((uint32_t)0x00000004) /*!< COMP1 power mode bit 0 */
#define COMP1_CTRL_COMP1_PWR_MODE_1      ((uint32_t)0x00000008) /*!< COMP1 power mode bit 1 */
#define COMP1_CTRL_COMP1_IN_SEL          ((uint32_t)0x00000070) /*!< COMP1 inverting input select */
#define COMP1_CTRL_COMP1_IN_SEL_0        ((uint32_t)0x00000010) /*!< COMP1 inverting input select bit 0 */
#define COMP1_CTRL_COMP1_IN_SEL_1        ((uint32_t)0x00000020) /*!< COMP1 inverting input select bit 1 */  
#define COMP1_CTRL_COMP1_INSEL_2         ((uint32_t)0x00000040) /*!< COMP1 inverting input select bit 2 */ 
#define COMP1_CTRL_COMP1_POLARITY        ((uint32_t)0x00008000) /*!< COMP1 output polarity */ 
#define COMP1_CTRL_COMP1_HYST            ((uint32_t)0x00030000) /*!< COMP1 hysteresis */
#define COMP1_CTRL_COMP1_HYST_0          ((uint32_t)0x00010000) /*!< COMP1 hysteresis bit 0 */
#define COMP1_CTRL_COMP1_HYST_1          ((uint32_t)0x00020000) /*!< COMP1 hysteresis bit 1 */
#define COMP1_CTRL_COMP1_BLANKING        ((uint32_t)0x001C0000) /*!< COMP1 blanking source */
#define COMP1_CTRL_COMP1_BLANKING_1      ((uint32_t)0x00040000) /*!< COMP1 blanking source bit 0 */
#define COMP1_CTRL_COMP1_BLANKING_2      ((uint32_t)0x00080000) /*!< COMP1 blanking source bit 1 */
#define COMP1_CTRL_COMP1_BLANKING_3      ((uint32_t)0x00100000) /*!< COMP1 blanking source bit 2 */ 
#define COMP1_CTRL_COMP1_SCAL_EN         ((uint32_t)0x00800000) /*!< COMP1 bandgap Scaler Enable */
#define COMP1_CTRL_COMP1_VALUE           ((uint32_t)0x40000000) /*!< COMP1 value */
#define COMP1_CTRL_COMP1_LOCK            ((uint32_t)0x80000000) /*!< COMP1 lock */

/**********************  Bit definition for COMP2_CSR register  ***************/
#define COMP2_CTRL_COMP2_EN              ((uint32_t)0x00000001) /*!< COMP2 enable */
#define COMP2_CTRL_COMP2_PWR_MODE        ((uint32_t)0x0000000C) /*!< COMP2 power mode */
#define COMP2_CTRL_COMP2_PWR_MODE_0      ((uint32_t)0x00000004) /*!< COMP2 power mode bit 0 */
#define COMP2_CTRL_COMP2_PWR_MODE_1      ((uint32_t)0x00000008) /*!< COMP2 power mode bit 1 */
#define COMP2_CTRL_COMP2_IN_SEL          ((uint32_t)0x00000070) /*!< COMP2 inverting input select */
#define COMP2_CTRL_COMP2_IN_SEL_0        ((uint32_t)0x00000010) /*!< COMP2 inverting input select bit 0 */
#define COMP2_CTRL_COMP2_IN_SEL_1        ((uint32_t)0x00000020) /*!< COMP2 inverting input select bit 1 */  
#define COMP2_CTRL_COMP2_IN_SEL_2        ((uint32_t)0x00000040) /*!< COMP2 inverting input select bit 2 */
#define COMP2_CTRL_COMP2_WNDW            ((uint32_t)0x00000200) /*!< COMP2 window mode */  
#define COMP2_CTRL_COMP2_POLARITY        ((uint32_t)0x00008000) /*!< COMP2 output polarity */ 
#define COMP2_CTRL_COMP2_HYST            ((uint32_t)0x00030000) /*!< COMP2 hysteresis */
#define COMP2_CTRL_COMP2_HYST_0          ((uint32_t)0x00010000) /*!< COMP2 hysteresis bit 0 */
#define COMP2_CTRL_COMP2_HYST_1          ((uint32_t)0x00020000) /*!< COMP2 hysteresis bit 1 */
#define COMP2_CTRL_COMP2_BLANKING        ((uint32_t)0x001C0000) /*!< COMP2 blanking source */
#define COMP2_CTRL_COMP2_BLANKING_1      ((uint32_t)0x00040000) /*!< COMP2 blanking source bit 0 */
#define COMP2_CTRL_COMP2_BLANKING_2      ((uint32_t)0x00080000) /*!< COMP2 blanking source bit 1 */
#define COMP2_CTRL_COMP2_BLANKING_3      ((uint32_t)0x00100000) /*!< COMP2 blanking source bit 2 */ 
#define COMP2_CTRL_COMP2_SCAL_EN         ((uint32_t)0x00800000) /*!< COMP2 bandgap Scaler Enable */
#define COMP2_CTRL_COMP2_VALUE           ((uint32_t)0x40000000) /*!< COMP2 value */
#define COMP2_CTRL_COMP2_LOCK            ((uint32_t)0x80000000) /*!< COMP2 lock */

/**********************  Bit definition for COMPx_CTRL register  ****************/
#define COMP_CTRL_COMPx_EN              ((uint32_t)0x00000001) /*!< COMPx enable */
#define COMP_CTRL_COMPx_PWR_MODE        ((uint32_t)0x0000000C) /*!< COMPx power mode */
#define COMP_CTRL_COMPx_PWR_MODE_0      ((uint32_t)0x00000004) /*!< COMPx power mode bit 0 */
#define COMP_CTRL_COMPx_PWR_MODE_1      ((uint32_t)0x00000008) /*!< COMPx power mode bit 1 */
#define COMP_CTRL_COMPx_IN_SEL          ((uint32_t)0x00000070) /*!< COMPx inverting input select */
#define COMP_CTRL_COMPx_IN_SEL_0        ((uint32_t)0x00000010) /*!< COMPx inverting input select bit 0 */
#define COMP_CTRL_COMPx_IN_SEL_1        ((uint32_t)0x00000020) /*!< COMPx inverting input select bit 1 */  
#define COMP_CTRL_COMPx_INSEL_2         ((uint32_t)0x00000040) /*!< COMPx inverting input select bit 2 */
#define COMP_CTRL_COMPx_WNDW            ((uint32_t)0x00000200) /*!< COMPx window mode */  
#define COMP_CTRL_COMPx_POLARITY        ((uint32_t)0x00008000) /*!< COMPx output polarity */ 
#define COMP_CTRL_COMPx_HYST            ((uint32_t)0x00030000) /*!< COMPx hysteresis */
#define COMP_CTRL_COMPx_HYST_0          ((uint32_t)0x00010000) /*!< COMPx hysteresis bit 0 */
#define COMP_CTRL_COMPx_HYST_1          ((uint32_t)0x00020000) /*!< COMPx hysteresis bit 1 */
#define COMP_CTRL_COMPx_BLANKING        ((uint32_t)0x001C0000) /*!< COMPx blanking source */
#define COMP_CTRL_COMPx_BLANKING_1      ((uint32_t)0x00040000) /*!< COMPx blanking source bit 0 */
#define COMP_CTRL_COMPx_BLANKING_2      ((uint32_t)0x00080000) /*!< COMPx blanking source bit 1 */
#define COMP_CTRL_COMPx_BLANKING_3      ((uint32_t)0x00100000) /*!< COMPx blanking source bit 2 */ 
#define COMP_CTRL_COMPx_SCAL_EN         ((uint32_t)0x00800000) /*!< COMPx bandgap Scaler Enable */
#define COMP_CTRL_COMPx_VALUE           ((uint32_t)0x40000000) /*!< COMPx value */
#define COMP_CTRL_COMPx_LOCK            ((uint32_t)0x80000000) /*!< COMPx lock */

/******************************************************************************/
/*                                                                            */
/*                         Operational Amplifier (OPAMP)                      */
/*                                                                            */
/******************************************************************************/
/*********************  Bit definition for OPAMP1_CSR register  ***************/
#define OPAMP1_CSR_OPAEN               ((uint32_t)0x00000001) /*!< Operational amplifier1 Enable */
#define OPAMP1_CSR_OPALPM              ((uint32_t)0x00000002) /*!< Operational amplifier1 Low Power Mode */

#define OPAMP1_CSR_OPAMODE             ((uint32_t)0x0000000C) /*!< Operational amplifier1 PGA mode */
#define OPAMP1_CSR_OPAMODE_0           ((uint32_t)0x00000004) /*!< Bit 0 */
#define OPAMP1_CSR_OPAMODE_1           ((uint32_t)0x00000008) /*!< Bit 1 */

#define OPAMP1_CSR_PGAGAIN             ((uint32_t)0x00000030) /*!< Operational amplifier1 Programmable amplifier gain value */
#define OPAMP1_CSR_PGAGAIN_0           ((uint32_t)0x00000010) /*!< Bit 0 */
#define OPAMP1_CSR_PGAGAIN_1           ((uint32_t)0x00000020) /*!< Bit 1 */

#define OPAMP1_CSR_VMSEL               ((uint32_t)0x00000300) /*!< Inverting input selection */
#define OPAMP1_CSR_VMSEL_0             ((uint32_t)0x00000100) /*!< Bit 0 */
#define OPAMP1_CSR_VMSEL_1             ((uint32_t)0x00000200) /*!< Bit 1 */

#define OPAMP1_CSR_VPSEL               ((uint32_t)0x00000400) /*!< Non inverted input selection */
#define OPAMP1_CSR_CALON               ((uint32_t)0x00001000) /*!< Calibration mode enable */
#define OPAMP1_CSR_CALSEL              ((uint32_t)0x00002000) /*!< Calibration selection */
#define OPAMP1_CSR_USERTRIM            ((uint32_t)0x00004000) /*!< User trimming enable */
#define OPAMP1_CSR_CALOUT              ((uint32_t)0x00008000) /*!< Operational amplifier1 calibration output */
#define OPAMP1_CSR_OPARANGE            ((uint32_t)0x80000000) /*!< Operational amplifiers power supply range for stability */

/*********************  Bit definition for OPAMP2_CSR register  ***************/
#define OPAMP2_CSR_OPAEN               ((uint32_t)0x00000001) /*!< Operational amplifier2 Enable */
#define OPAMP2_CSR_OPALPM              ((uint32_t)0x00000002) /*!< Operational amplifier2 Low Power Mode */

#define OPAMP2_CSR_OPAMODE             ((uint32_t)0x0000000C) /*!< Operational amplifier2 PGA mode */
#define OPAMP2_CSR_OPAMODE_0           ((uint32_t)0x00000004) /*!< Bit 0 */
#define OPAMP2_CSR_OPAMODE_1           ((uint32_t)0x00000008) /*!< Bit 1 */

#define OPAMP2_CSR_PGAGAIN             ((uint32_t)0x00000030) /*!< Operational amplifier2 Programmable amplifier gain value */
#define OPAMP2_CSR_PGAGAIN_0           ((uint32_t)0x00000010) /*!< Bit 0 */
#define OPAMP2_CSR_PGAGAIN_1           ((uint32_t)0x00000020) /*!< Bit 1 */

#define OPAMP2_CSR_VMSEL               ((uint32_t)0x00000300) /*!< Inverting input selection */
#define OPAMP2_CSR_VMSEL_0             ((uint32_t)0x00000100) /*!< Bit 0 */
#define OPAMP2_CSR_VMSEL_1             ((uint32_t)0x00000200) /*!< Bit 1 */

#define OPAMP2_CSR_VPSEL               ((uint32_t)0x00000400) /*!< Non inverted input selection */
#define OPAMP2_CSR_CALON               ((uint32_t)0x00001000) /*!< Calibration mode enable */
#define OPAMP2_CSR_CALSEL              ((uint32_t)0x00002000) /*!< Calibration selection */
#define OPAMP2_CSR_USERTRIM            ((uint32_t)0x00004000) /*!< User trimming enable */
#define OPAMP2_CSR_CALOUT              ((uint32_t)0x00008000) /*!< Operational amplifier2 calibration output */


/*******************  Bit definition for OPAMP1_OTR register  ******************/
#define OPAMP1_OTR_TRIMOFFSETN            ((uint32_t)0x0000001F)        /*!< Trim for NMOS differential pairs */
#define OPAMP1_OTR_TRIMOFFSETP            ((uint32_t)0x00001F00)        /*!< Trim for PMOS differential pairs */

/*******************  Bit definition for OPAMP2_OTR register  ******************/
#define OPAMP2_OTR_TRIMOFFSETN            ((uint32_t)0x0000001F)        /*!< Trim for NMOS differential pairs */
#define OPAMP2_OTR_TRIMOFFSETP            ((uint32_t)0x00001F00)        /*!< Trim for PMOS differential pairs */

/*******************  Bit definition for OPAMP1_LPOTR register  ****************/
#define OPAMP1_LPOTR_TRIMLPOFFSETN        ((uint32_t)0x0000001F)        /*!< Trim for NMOS differential pairs */
#define OPAMP1_LPOTR_TRIMLPOFFSETP        ((uint32_t)0x00001F00)        /*!< Trim for PMOS differential pairs */

/*******************  Bit definition for OPAMP2_LPOTR register  ****************/
#define OPAMP2_LPOTR_TRIMLPOFFSETN        ((uint32_t)0x0000001F)        /*!< Trim for NMOS differential pairs */
#define OPAMP2_LPOTR_TRIMLPOFFSETP        ((uint32_t)0x00001F00)        /*!< Trim for PMOS differential pairs */

/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_UE                        ((uint32_t)0x00000001)            /*!< USART Enable */
#define  USART_CR1_UESM                      ((uint32_t)0x00000002)            /*!< USART Enable in STOP Mode */
#define  USART_CR1_RE                        ((uint32_t)0x00000004)            /*!< Receiver Enable */
#define  USART_CR1_TE                        ((uint32_t)0x00000008)            /*!< Transmitter Enable */
#define  USART_CR1_IDLEIE                    ((uint32_t)0x00000010)            /*!< IDLE Interrupt Enable */
#define  USART_CR1_RXNEIE                    ((uint32_t)0x00000020)            /*!< RXNE Interrupt Enable */
#define  USART_CR1_TCIE                      ((uint32_t)0x00000040)            /*!< Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint32_t)0x00000080)            /*!< TXE Interrupt Enable */
#define  USART_CR1_PEIE                      ((uint32_t)0x00000100)            /*!< PE Interrupt Enable */
#define  USART_CR1_PS                        ((uint32_t)0x00000200)            /*!< Parity Selection */
#define  USART_CR1_PCE                       ((uint32_t)0x00000400)            /*!< Parity Control Enable */
#define  USART_CR1_WAKE                      ((uint32_t)0x00000800)            /*!< Receiver Wakeup method */
#define  USART_CR1_M                         ((uint32_t)0x10001000)            /*!< Word length */
#define  USART_CR1_M_0                       ((uint32_t)0x00001000)            /*!< Word length - Bit 0 */
#define  USART_CR1_MME                       ((uint32_t)0x00002000)            /*!< Mute Mode Enable */
#define  USART_CR1_CMIE                      ((uint32_t)0x00004000)            /*!< Character match interrupt enable */
#define  USART_CR1_OVER8                     ((uint32_t)0x00008000)            /*!< Oversampling by 8-bit or 16-bit mode */
#define  USART_CR1_DEDT                      ((uint32_t)0x001F0000)            /*!< DEDT[4:0] bits (Driver Enable Deassertion Time) */
#define  USART_CR1_DEDT_0                    ((uint32_t)0x00010000)            /*!< Bit 0 */
#define  USART_CR1_DEDT_1                    ((uint32_t)0x00020000)            /*!< Bit 1 */
#define  USART_CR1_DEDT_2                    ((uint32_t)0x00040000)            /*!< Bit 2 */
#define  USART_CR1_DEDT_3                    ((uint32_t)0x00080000)            /*!< Bit 3 */
#define  USART_CR1_DEDT_4                    ((uint32_t)0x00100000)            /*!< Bit 4 */
#define  USART_CR1_DEAT                      ((uint32_t)0x03E00000)            /*!< DEAT[4:0] bits (Driver Enable Assertion Time) */
#define  USART_CR1_DEAT_0                    ((uint32_t)0x00200000)            /*!< Bit 0 */
#define  USART_CR1_DEAT_1                    ((uint32_t)0x00400000)            /*!< Bit 1 */
#define  USART_CR1_DEAT_2                    ((uint32_t)0x00800000)            /*!< Bit 2 */
#define  USART_CR1_DEAT_3                    ((uint32_t)0x01000000)            /*!< Bit 3 */
#define  USART_CR1_DEAT_4                    ((uint32_t)0x02000000)            /*!< Bit 4 */
#define  USART_CR1_RTOIE                     ((uint32_t)0x04000000)            /*!< Receive Time Out interrupt enable */
#define  USART_CR1_EOBIE                     ((uint32_t)0x08000000)            /*!< End of Block interrupt enable */
#define  USART_CR1_M_1                       ((uint32_t)0x10000000)            /*!< Word length - Bit 1 */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADDM7                     ((uint32_t)0x00000010)            /*!< 7-bit or 4-bit Address Detection */
#define  USART_CR2_LBDL                      ((uint32_t)0x00000020)            /*!< LIN Break Detection Length */
#define  USART_CR2_LBDIE                     ((uint32_t)0x00000040)            /*!< LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint32_t)0x00000100)            /*!< Last Bit Clock pulse */
#define  USART_CR2_CPHA                      ((uint32_t)0x00000200)            /*!< Clock Phase */
#define  USART_CR2_CPOL                      ((uint32_t)0x00000400)            /*!< Clock Polarity */
#define  USART_CR2_CLKEN                     ((uint32_t)0x00000800)            /*!< Clock Enable */
#define  USART_CR2_STOP                      ((uint32_t)0x00003000)            /*!< STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint32_t)0x00001000)            /*!< Bit 0 */
#define  USART_CR2_STOP_1                    ((uint32_t)0x00002000)            /*!< Bit 1 */
#define  USART_CR2_LINEN                     ((uint32_t)0x00004000)            /*!< LIN mode enable */
#define  USART_CR2_SWAP                      ((uint32_t)0x00008000)            /*!< SWAP TX/RX pins */
#define  USART_CR2_RXINV                     ((uint32_t)0x00010000)            /*!< RX pin active level inversion */
#define  USART_CR2_TXINV                     ((uint32_t)0x00020000)            /*!< TX pin active level inversion */
#define  USART_CR2_DATAINV                   ((uint32_t)0x00040000)            /*!< Binary data inversion */
#define  USART_CR2_MSBFIRST                  ((uint32_t)0x00080000)            /*!< Most Significant Bit First */
#define  USART_CR2_ABREN                     ((uint32_t)0x00100000)            /*!< Auto Baud-Rate Enable*/
#define  USART_CR2_ABRMODE                   ((uint32_t)0x00600000)            /*!< ABRMOD[1:0] bits (Auto Baud-Rate Mode) */
#define  USART_CR2_ABRMODE_0                 ((uint32_t)0x00200000)            /*!< Bit 0 */
#define  USART_CR2_ABRMODE_1                 ((uint32_t)0x00400000)            /*!< Bit 1 */
#define  USART_CR2_RTOEN                     ((uint32_t)0x00800000)            /*!< Receiver Time-Out enable */
#define  USART_CR2_ADD                       ((uint32_t)0xFF000000)            /*!< Address of the USART node */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint32_t)0x00000001)            /*!< Error Interrupt Enable */
#define  USART_CR3_IREN                      ((uint32_t)0x00000002)            /*!< IrDA mode Enable */
#define  USART_CR3_IRLP                      ((uint32_t)0x00000004)            /*!< IrDA Low-Power */
#define  USART_CR3_HDSEL                     ((uint32_t)0x00000008)            /*!< Half-Duplex Selection */
#define  USART_CR3_NACK                      ((uint32_t)0x00000010)            /*!< SmartCard NACK enable */
#define  USART_CR3_SCEN                      ((uint32_t)0x00000020)            /*!< SmartCard mode enable */
#define  USART_CR3_DMAR                      ((uint32_t)0x00000040)            /*!< DMA Enable Receiver */
#define  USART_CR3_DMAT                      ((uint32_t)0x00000080)            /*!< DMA Enable Transmitter */
#define  USART_CR3_RTSE                      ((uint32_t)0x00000100)            /*!< RTS Enable */
#define  USART_CR3_CTSE                      ((uint32_t)0x00000200)            /*!< CTS Enable */
#define  USART_CR3_CTSIE                     ((uint32_t)0x00000400)            /*!< CTS Interrupt Enable */
#define  USART_CR3_ONEBIT                    ((uint32_t)0x00000800)            /*!< One sample bit method enable */
#define  USART_CR3_OVRDIS                    ((uint32_t)0x00001000)            /*!< Overrun Disable */
#define  USART_CR3_DDRE                      ((uint32_t)0x00002000)            /*!< DMA Disable on Reception Error */
#define  USART_CR3_DEM                       ((uint32_t)0x00004000)            /*!< Driver Enable Mode */
#define  USART_CR3_DEP                       ((uint32_t)0x00008000)            /*!< Driver Enable Polarity Selection */
#define  USART_CR3_SCARCNT                   ((uint32_t)0x000E0000)            /*!< SCARCNT[2:0] bits (SmartCard Auto-Retry Count) */
#define  USART_CR3_SCARCNT_0                 ((uint32_t)0x00020000)            /*!< Bit 0 */
#define  USART_CR3_SCARCNT_1                 ((uint32_t)0x00040000)            /*!< Bit 1 */
#define  USART_CR3_SCARCNT_2                 ((uint32_t)0x00080000)            /*!< Bit 2 */
#define  USART_CR3_WUS                       ((uint32_t)0x00300000)            /*!< WUS[1:0] bits (Wake UP Interrupt Flag Selection) */
#define  USART_CR3_WUS_0                     ((uint32_t)0x00100000)            /*!< Bit 0 */
#define  USART_CR3_WUS_1                     ((uint32_t)0x00200000)            /*!< Bit 1 */
#define  USART_CR3_WUFIE                     ((uint32_t)0x00400000)            /*!< Wake Up Interrupt Enable */
#define  USART_CR3_UCESM                     ((uint32_t)0x00800000)            /*!< LPUART/USART Clock Enable in Stop mode */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_FRACTION              ((uint16_t)0x000F)                /*!< Fraction of USARTDIV */
#define  USART_BRR_DIV_MANTISSA              ((uint16_t)0xFFF0)                /*!< Mantissa of USARTDIV */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint16_t)0x00FF)                /*!< PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_GT                       ((uint16_t)0xFF00)                /*!< GT[7:0] bits (Guard time value) */


/*******************  Bit definition for USART_RTOR register  *****************/
#define  USART_RTOR_RTO                      ((uint32_t)0x00FFFFFF)            /*!< Receiver Time Out Value */
#define  USART_RTOR_BLEN                     ((uint32_t)0xFF000000)            /*!< Block Length */

/*******************  Bit definition for USART_RQR register  ******************/
#define  USART_RQR_ABRRQ                     ((uint16_t)0x0001)                /*!< Auto-Baud Rate Request */
#define  USART_RQR_SBKRQ                     ((uint16_t)0x0002)                /*!< Send Break Request */
#define  USART_RQR_MMRQ                      ((uint16_t)0x0004)                /*!< Mute Mode Request */
#define  USART_RQR_RXFRQ                     ((uint16_t)0x0008)                /*!< Receive Data flush Request */
#define  USART_RQR_TXFRQ                     ((uint16_t)0x0010)                /*!< Transmit data flush Request */

/*******************  Bit definition for USART_ISR register  ******************/
#define  USART_ISR_PE                        ((uint32_t)0x00000001)            /*!< Parity Error */
#define  USART_ISR_FE                        ((uint32_t)0x00000002)            /*!< Framing Error */
#define  USART_ISR_NE                        ((uint32_t)0x00000004)            /*!< Noise detected Flag */
#define  USART_ISR_ORE                       ((uint32_t)0x00000008)            /*!< OverRun Error */
#define  USART_ISR_IDLE                      ((uint32_t)0x00000010)            /*!< IDLE line detected */
#define  USART_ISR_RXNE                      ((uint32_t)0x00000020)            /*!< Read Data Register Not Empty */
#define  USART_ISR_TC                        ((uint32_t)0x00000040)            /*!< Transmission Complete */
#define  USART_ISR_TXE                       ((uint32_t)0x00000080)            /*!< Transmit Data Register Empty */
#define  USART_ISR_LBD                       ((uint32_t)0x00000100)            /*!< LIN Break Detection Flag */
#define  USART_ISR_CTSIF                     ((uint32_t)0x00000200)            /*!< CTS interrupt flag */
#define  USART_ISR_CTS                       ((uint32_t)0x00000400)            /*!< CTS flag */
#define  USART_ISR_RTOF                      ((uint32_t)0x00000800)            /*!< Receiver Time Out */
#define  USART_ISR_EOBF                      ((uint32_t)0x00001000)            /*!< End Of Block Flag */
#define  USART_ISR_ABRE                      ((uint32_t)0x00004000)            /*!< Auto-Baud Rate Error */
#define  USART_ISR_ABRF                      ((uint32_t)0x00008000)            /*!< Auto-Baud Rate Flag */
#define  USART_ISR_BUSY                      ((uint32_t)0x00010000)            /*!< Busy Flag */
#define  USART_ISR_CMF                       ((uint32_t)0x00020000)            /*!< Character Match Flag */
#define  USART_ISR_SBKF                      ((uint32_t)0x00040000)            /*!< Send Break Flag */
#define  USART_ISR_RWU                       ((uint32_t)0x00080000)            /*!< Receive Wake Up from mute mode Flag */
#define  USART_ISR_WUF                       ((uint32_t)0x00100000)            /*!< Wake Up from stop mode Flag */
#define  USART_ISR_TEACK                     ((uint32_t)0x00200000)            /*!< Transmit Enable Acknowledge Flag */
#define  USART_ISR_REACK                     ((uint32_t)0x00400000)            /*!< Receive Enable Acknowledge Flag */

/*******************  Bit definition for USART_ICR register  ******************/
#define  USART_ICR_PECF                      ((uint32_t)0x00000001)            /*!< Parity Error Clear Flag */
#define  USART_ICR_FECF                      ((uint32_t)0x00000002)            /*!< Framing Error Clear Flag */
#define  USART_ICR_NCF                       ((uint32_t)0x00000004)            /*!< Noise detected Clear Flag */
#define  USART_ICR_ORECF                     ((uint32_t)0x00000008)            /*!< OverRun Error Clear Flag */
#define  USART_ICR_IDLECF                    ((uint32_t)0x00000010)            /*!< IDLE line detected Clear Flag */
#define  USART_ICR_TCCF                      ((uint32_t)0x00000040)            /*!< Transmission Complete Clear Flag */
#define  USART_ICR_LBDCF                     ((uint32_t)0x00000100)            /*!< LIN Break Detection Clear Flag */
#define  USART_ICR_CTSCF                     ((uint32_t)0x00000200)            /*!< CTS Interrupt Clear Flag */
#define  USART_ICR_RTOCF                     ((uint32_t)0x00000800)            /*!< Receiver Time Out Clear Flag */
#define  USART_ICR_EOBCF                     ((uint32_t)0x00001000)            /*!< End Of Block Clear Flag */
#define  USART_ICR_CMCF                      ((uint32_t)0x00020000)            /*!< Character Match Clear Flag */
#define  USART_ICR_WUCF                      ((uint32_t)0x00100000)            /*!< Wake Up from stop mode Clear Flag */

/*******************  Bit definition for USART_RDR register  ******************/
#define  USART_RDR_RDR                       ((uint16_t)0x01FF)                /*!< RDR[8:0] bits (Receive Data value) */

/*******************  Bit definition for USART_TDR register  ******************/
#define  USART_TDR_TDR                       ((uint16_t)0x01FF)                /*!< TDR[8:0] bits (Transmit Data value) */

/******************************************************************************/
/*                                                                            */
/*           Single Wire Protocol Master Interface (SWPMI)                    */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for SWPMI_CR register   ********************/
#define  SWPMI_CR_RXDMA                      ((uint16_t)0x0001)            /*!<Reception DMA enable                                 */
#define  SWPMI_CR_TXDMA                      ((uint16_t)0x0002)            /*!<Transmission DMA enable                              */
#define  SWPMI_CR_RXMODE                     ((uint16_t)0x0004)            /*!<Reception buffering mode                             */
#define  SWPMI_CR_TXMODE                     ((uint16_t)0x0008)            /*!<Transmission buffering mode                          */
#define  SWPMI_CR_LPBK                       ((uint16_t)0x0010)            /*!<Loopback mode enable                                 */
#define  SWPMI_CR_SWPME                      ((uint16_t)0x0020)            /*!<Single wire protocol master interface enable         */
#define  SWPMI_CR_DEACT                      ((uint16_t)0x0400)            /*!<Single wire protocol master interface deactivate     */

/*******************  Bit definition for SWPMI_BRR register  ********************/
#define  SWPMI_BRR_BR                        ((uint16_t)0x003F)            /*!<BR[5:0] bits (Bitrate prescaler) */

/*******************  Bit definition for SWPMI_TRIM register ********************/
#define  SWPMI_TRIM_TRIM                     ((uint16_t)0x00FF)            /*!<TRIM[7:0] bits (Trimming bits) */

/*******************  Bit definition for SWPMI_ISR register  ********************/
#define  SWPMI_ISR_RXBFF                     ((uint16_t)0x0001)            /*!<Receive buffer full flag        */
#define  SWPMI_ISR_TXBEF                     ((uint16_t)0x0002)            /*!<Transmit buffer empty flag      */
#define  SWPMI_ISR_RXBERF                    ((uint16_t)0x0004)            /*!<Receive CRC error flag          */
#define  SWPMI_ISR_RXOVRF                    ((uint16_t)0x0008)            /*!<Receive overrun error flag      */
#define  SWPMI_ISR_TXUNRF                    ((uint16_t)0x0010)            /*!<Transmit underrun error flag    */
#define  SWPMI_ISR_RXNE                      ((uint16_t)0x0020)            /*!<Receive data register not empty */
#define  SWPMI_ISR_TXE                       ((uint16_t)0x0040)            /*!<Transmit data register empty    */
#define  SWPMI_ISR_TCF                       ((uint16_t)0x0080)            /*!<Transfer complete flag          */
#define  SWPMI_ISR_SRF                       ((uint16_t)0x0100)            /*!<Slave resume flag               */
#define  SWPMI_ISR_SUSP                      ((uint16_t)0x0200)            /*!<SUSPEND flag                    */
#define  SWPMI_ISR_DEACTF                    ((uint16_t)0x0400)            /*!<DEACTIVATED flag                */        

/*******************  Bit definition for SWPMI_ICR register  ********************/
#define  SWPMI_ICR_CRXBFF                    ((uint16_t)0x0001)            /*!<Clear receive buffer full flag       */
#define  SWPMI_ICR_CTXBEF                    ((uint16_t)0x0002)            /*!<Clear transmit buffer empty flag     */
#define  SWPMI_ICR_CRXBERF                   ((uint16_t)0x0004)            /*!<Clear receive CRC error flag         */
#define  SWPMI_ICR_CRXOVRF                   ((uint16_t)0x0008)            /*!<Clear receive overrun error flag     */
#define  SWPMI_ICR_CTXUNRF                   ((uint16_t)0x0010)            /*!<Clear transmit underrun error flag   */
#define  SWPMI_ICR_CTCF                      ((uint16_t)0x0080)            /*!<Clear transfer complete flag         */
#define  SWPMI_ICR_CSRF                      ((uint16_t)0x0100)            /*!<Clear slave resume flag              */


/*******************  Bit definition for SWPMI_IER register  ********************/
#define  SWPMI_IER_SRIE                    ((uint16_t)0x0100)            /*!<Slave resume interrupt enable               */
#define  SWPMI_IER_TCIE                    ((uint16_t)0x0080)            /*!<Transmit complete interrupt enable          */
#define  SWPMI_IER_TIE                     ((uint16_t)0x0040)            /*!<Transmit interrupt enable                   */
#define  SWPMI_IER_RIE                     ((uint16_t)0x0020)            /*!<Receive interrupt enable                    */
#define  SWPMI_IER_TXUNRIE                 ((uint16_t)0x0010)            /*!<Transmit underrun error interrupt enable    */
#define  SWPMI_IER_RXOVRIE                 ((uint16_t)0x0008)            /*!<Receive overrun error interrupt enable      */
#define  SWPMI_IER_RXBERIE                 ((uint16_t)0x0004)            /*!<Receive CRC error interrupt enable          */
#define  SWPMI_IER_TXBEIE                  ((uint16_t)0x0002)            /*!<Transmit buffer empty interrupt enable      */
#define  SWPMI_IER_RXBFIE                  ((uint16_t)0x0001)            /*!<Receive buffer full interrupt enable        */


/*******************  Bit definition for SWPMI_RFL register  ********************/
#define  SWPMI_RFL_RFL                     ((uint16_t)0x001F)             /*!<RFL[4:0] bits (Receive Frame length) */
#define  SWPMI_RFL_RFL_0_1                 ((uint16_t)0x0003)             /*!<RFL[1:0] bits (number of relevant bytes for the last SWPMI_RDR register read.) */

/*******************  Bit definition for SWPMI_TDR register  ********************/
#define  SWPMI_TDR_TD                      ((uint32_t)0xFFFFFFFF)         /*!<Transmit Data Register         */

/*******************  Bit definition for SWPMI_RDR register  ********************/
#define  SWPMI_RDR_RD                      ((uint32_t)0xFFFFFFFF)         /*!<Recive Data Register           */


/*******************  Bit definition for SWPMI_OR register  ********************/
#define  SWPMI_OR_TBYP                     ((uint16_t)0x0001)             /*!<SWP Transceiver Bypass */
#define  SWPMI_OR_CLASS                    ((uint16_t)0x0002)             /*!<SWP CLASS selection */

/******************************************************************************/
/*                                                                            */
/*                            Window WATCHDOG                                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for WWDG_CR register  ********************/
#define  WWDG_CR_T                           ((uint8_t)0x7F)               /*!<T[6:0] bits (7-Bit counter (MSB to LSB)) */
#define  WWDG_CR_T0                          ((uint8_t)0x01)               /*!<Bit 0 */
#define  WWDG_CR_T1                          ((uint8_t)0x02)               /*!<Bit 1 */
#define  WWDG_CR_T2                          ((uint8_t)0x04)               /*!<Bit 2 */
#define  WWDG_CR_T3                          ((uint8_t)0x08)               /*!<Bit 3 */
#define  WWDG_CR_T4                          ((uint8_t)0x10)               /*!<Bit 4 */
#define  WWDG_CR_T5                          ((uint8_t)0x20)               /*!<Bit 5 */
#define  WWDG_CR_T6                          ((uint8_t)0x40)               /*!<Bit 6 */

#define  WWDG_CR_WDGA                        ((uint8_t)0x80)               /*!<Activation bit */

/*******************  Bit definition for WWDG_CFR register  *******************/
#define  WWDG_CFR_W                          ((uint16_t)0x007F)            /*!<W[6:0] bits (7-bit window value) */
#define  WWDG_CFR_W0                         ((uint16_t)0x0001)            /*!<Bit 0 */
#define  WWDG_CFR_W1                         ((uint16_t)0x0002)            /*!<Bit 1 */
#define  WWDG_CFR_W2                         ((uint16_t)0x0004)            /*!<Bit 2 */
#define  WWDG_CFR_W3                         ((uint16_t)0x0008)            /*!<Bit 3 */
#define  WWDG_CFR_W4                         ((uint16_t)0x0010)            /*!<Bit 4 */
#define  WWDG_CFR_W5                         ((uint16_t)0x0020)            /*!<Bit 5 */
#define  WWDG_CFR_W6                         ((uint16_t)0x0040)            /*!<Bit 6 */

#define  WWDG_CFR_WDGTB                      ((uint16_t)0x0180)            /*!<WDGTB[1:0] bits (Timer Base) */
#define  WWDG_CFR_WDGTB0                     ((uint16_t)0x0080)            /*!<Bit 0 */
#define  WWDG_CFR_WDGTB1                     ((uint16_t)0x0100)            /*!<Bit 1 */

#define  WWDG_CFR_EWI                        ((uint16_t)0x0200)            /*!<Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_SR register  ********************/
#define  WWDG_SR_EWIF                        ((uint8_t)0x01)               /*!<Early Wakeup Interrupt Flag */


/******************************************************************************/
/*                                                                            */
/*                                DBG                                         */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for DBGMCU_IDCODE register  *************/
#define  DBGMCU_IDCODE_DEV_ID                ((uint32_t)0x00000FFF)
#define  DBGMCU_IDCODE_REV_ID                ((uint32_t)0xFFFF0000)

/********************  Bit definition for DBGMCU_CR register  *****************/
#define  DBGMCU_CR_DBG_SLEEP                 ((uint32_t)0x00000001)
#define  DBGMCU_CR_DBG_STOP                  ((uint32_t)0x00000002)
#define  DBGMCU_CR_DBG_STANDBY               ((uint32_t)0x00000004)
#define  DBGMCU_CR_TRACE_IOEN                ((uint32_t)0x00000020)

#define  DBGMCU_CR_TRACE_MODE                ((uint32_t)0x000000C0)
#define  DBGMCU_CR_TRACE_MODE_0              ((uint32_t)0x00000040)/*!<Bit 0 */
#define  DBGMCU_CR_TRACE_MODE_1              ((uint32_t)0x00000080)/*!<Bit 1 */

/********************  Bit definition for DBGMCU_APB1L_FZ register  ************/
#define  DBGMCU_APB1L_FZ_DBG_TIM2_STOP            ((uint32_t)0x00000001)
#define  DBGMCU_APB1L_FZ_DBG_TIM3_STOP            ((uint32_t)0x00000002)
#define  DBGMCU_APB1L_FZ_DBG_TIM4_STOP            ((uint32_t)0x00000004)
#define  DBGMCU_APB1L_FZ_DBG_TIM5_STOP            ((uint32_t)0x00000008)
#define  DBGMCU_APB1L_FZ_DBG_TIM6_STOP            ((uint32_t)0x00000010)
#define  DBGMCU_APB1L_FZ_DBG_TIM7_STOP            ((uint32_t)0x00000020)
#define  DBGMCU_APB1L_FZ_DBG_RTC_STOP             ((uint32_t)0x00000400)
#define  DBGMCU_APB1L_FZ_DBG_WWDG_STOP            ((uint32_t)0x00000800)
#define  DBGMCU_APB1L_FZ_DBG_IWDG_STOP            ((uint32_t)0x00001000)
#define  DBGMCU_APB1L_FZ_DBG_I2C1_STOP            ((uint32_t)0x00200000)
#define  DBGMCU_APB1L_FZ_DBG_I2C2_STOP            ((uint32_t)0x00400000)
#define  DBGMCU_APB1L_FZ_DBG_I2C3_STOP            ((uint32_t)0x00800000)
#define  DBGMCU_APB1L_FZ_DBG_CAN_STOP             ((uint32_t)0x02000000)
/********************  Bit definition for DBGMCU_APB1H_FZ register  ************/
#define  DBGMCU_APB1H_FZ_DBG_LPTIM1_STOP          ((uint32_t)0x00000010)
#define  DBGMCU_APB1H_FZ_DBG_LPTIM2_STOP          ((uint32_t)0x00000020)
/* Old IWDGSTOP bit definition, maintained for legacy purpose */
#define  DBGMCU_APB1_FZ_DBG_IWDEG_STOP           DBGMCU_APB1L_FZ_DBG_IWDG_STOP

/********************  Bit definition for DBGMCU_APB2_FZ register  ************/
#define  DBGMCU_APB2_FZ_DBG_TIM15_STOP            ((uint32_t)0x00000001)
#define  DBGMCU_APB2_FZ_DBG_TIM16_STOP            ((uint32_t)0x00000002)
#define  DBGMCU_APB2_FZ_DBG_TIM17_STOP            ((uint32_t)0x00010004)
#define  DBGMCU_APB2_FZ_DBG_TIM1_STOP             ((uint32_t)0x00000800)
#define  DBGMCU_APB2_FZ_DBG_TIM8_STOP             ((uint32_t)0x00002000)



/**
  *
  */

 /**
  * @}
  */ 

#ifdef USE_STDPERIPH_DRIVER
  #include "stm32l4xx_conf.h"
#endif /* USE_STDPERIPH_DRIVER */

/** @addtogroup Exported_macro
  * @{
  */

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32L4xx_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
