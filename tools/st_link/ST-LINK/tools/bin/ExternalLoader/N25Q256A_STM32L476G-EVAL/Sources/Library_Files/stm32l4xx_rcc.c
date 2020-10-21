/**
  ******************************************************************************
  * @file    stm32l4xx_rcc.c
  * @author  MCD Application Team
  * @version VX.Y.Z (Start from version V1.0.0)
  * @date    DD-Month-YYYY
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Reset and clock control (RCC) peripheral:
  *           + Internal/external clocks, PLL, CSS and MCO configuration
  *           + System, AHB and APB busses clocks configuration
  *           + Peripheral clocks configuration
  *           + Interrupts and flags management
  *
 @verbatim                
 ===============================================================================
                      ##### RCC specific features #####
 ===============================================================================
    [..]  
      After reset the device is running from Multiple Speed Internal oscillator.
      (4 MHz) with Flash 0 wait state, Flash prefetch buffer, D-Cache 
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.(TBC)
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.
    [..]          
      Once the device started from reset, the user application has to:        
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings  
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (SAI, RTC, ADC, USB OTG FS/SDIO/RNG)                                
 @endverbatim    
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_rcc.h"
#include "stm32l4xx_conf.h" 
/** @addtogroup STM32L4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup RCC 
  * @brief RCC driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
/* --- CR Register ---*/
/* Alias word address of HSION bit */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x08
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))
   /* Alias word address of MSION bit */
#define MSION_BitNumber           0x00
#define CR_MSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (MSION_BitNumber * 4))
    
/* Alias word address of HSECSSON bit */
#define HSECSSON_BitNumber        0x13
#define CR_HSECSSON_BB            (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSECSSON_BitNumber * 4))

/* Alias word address of PLLON bit */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

/* Alias word address of PLLSAI1ON bit */
#define PLLSAI1ON_BitNumber         0x1A
#define CR_PLLSAI1ON_BB            (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLSAI1ON_BitNumber * 4))

/* Alias word address of PLLSAI2ON bit */
#define PLLSAI2ON_BitNumber        0x1C
#define CR_PLLSAI2ON_BB            (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLSAI2ON_BitNumber * 4))

 /* --- CFGR Register ---*/
#define CFGR_OFFSET                (RCC_OFFSET + 0x08)
#define STOPWUCK_BitNumber         0xF
#define CFGR_STOPWUCK_BB          (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (STOPWUCK_BitNumber * 4))   
    
 /* --- PLLCFGR Register ---*/ 
#define PLLCFGR_OFFSET             (RCC_OFFSET + 0x0C)
#define PLLSYSPEN_BitNumber        0x10
#define PLLCFGR_PLLSYSPEN_BB       (PERIPH_BB_BASE + (PLLCFGR_OFFSET * 32) + (PLLSYSPEN_BitNumber * 4))

  /* --- PLLSAI1CFGR Register ---*/ 
#define PLLSAI1CFGR_OFFSET          (RCC_OFFSET + 0x10)
#define PLLSAI1PEN_BitNumber        0x10
#define PLLSAI1CFGR_PLLSAI1PEN_BB   (PERIPH_BB_BASE + (PLLSAI1CFGR_OFFSET * 32) + (PLLSAI1PEN_BitNumber * 4))

  /* --- PLLSAI2CFGR Register ---*/ 
#define PLLSAI2CFGR_OFFSET          (RCC_OFFSET + 0x14)
#define PLLSAI2PEN_BitNumber         0x10
#define PLLSAI2CFGR_PLLSAI2PEN_BB   (PERIPH_BB_BASE + (PLLSAI2CFGR_OFFSET * 32) + (PLLSYSPEN_BitNumber * 4))

/* --- CCIPR Register ---*/
#define CCIPR_OFFSET                 (RCC_OFFSET + 0x88)
#define DFSDMSRC_BitNumber           0x1F
#define CCIPR_DFSDMSRC_BB            (PERIPH_BB_BASE + (CCIPR_OFFSET * 32) + (DFSDMSRC_BitNumber * 4))
#define SWPSRC_BitNumber             0x1E
#define CCIPR_SWPSRC_BB             (PERIPH_BB_BASE + (CCIPR_OFFSET * 32) + (SWPSRC_BitNumber * 4))
    
/* --- BDCR Register ---*/
/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x90)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))
/* Alias word address of LSECSSON bit */
#define LSECSSON_BitNumber        0x05
#define BDCR_LSECSSON_BB          (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (LSECSSON_BitNumber * 4))
    
/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x94)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

/* ---------------------- RCC registers bit mask ------------------------ */
/* CFGR register bit mask */
#define CFGR_MCO_RESET_MASK      ((uint32_t)0x88FFFFFF)

/* RCC Flag Mask */
#define RCC_FLAG_MASK                 ((uint8_t)0x1F)
 
/* CR register byte 3 (Bits[23:16]) base address */
#define CR_BYTE3_ADDRESS         ((uint32_t)(RCC_BASE + 0x00 + 0x02))

 /* CR register byte 2 (Bits[15:8]) base address */
#define ICSCR_BYTE2_ADDRESS         ((uint32_t)(RCC_BASE + 0x04 + 0x01))   
    
/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
// const uint32_t MSIRangeTable[12] =
// {100000, 200000, 400000, 800000, 1000000, 2000000, 4000000,
//  8000000, 16000000, 24000000, 32000000, 48000000};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCC_Private_Functions
  * @{
  */ 

/** @defgroup RCC_Group1 Internal and external clocks, PLL, CSS and MCO configuration functions
 *  @brief   Internal and external clocks, PLL, CSS and MCO configuration functions 
 *
@verbatim   
 ===================================================================================
 ##### Internal and  external clocks, PLL, CSS and MCO configuration functions #####
 ===================================================================================  
    [..]
      This section provide functions allowing to configure the internal/external clocks,
      PLLs, CSS and MCO pins.
  
      (#) HSI (high-speed internal), 16 MHz factory-trimmed RC used directly or through
          the PLL as System clock source.

      (#) LSI (low-speed internal), 32 KHz low consumption RC used as IWDG and/or RTC
          clock source.

      (#) HSE (high-speed external), 4 to 48 MHz crystal oscillator used directly or
          through the PLL as System clock source. Can be used also as RTC clock source.

      (#) LSE (low-speed external), 32 KHz oscillator used as RTC clock source.   
      
      (#) MSI (Mutiple Speed Internal), Its frequency is software trimmable from 100KHZ to 48MHZ.   
         It can be used to generate the clock for the USB OTG FS (48 MHz).

      (#) PLL (clocked by HSI , HSE or MSI), featuring three different output clocks:
        (++) The first output is used to generate the high speed system clock (up to 80MHz)
        (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
             the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
        (++) The Third output is used to generate an accurate clock to achieve 
             high-quality audio performance on SAI interface in case.

      (#) PLLSAI1 (clocked by HSI , HSE or MSI), featuring three different output clocks: 
        (++) The first output is used to generate SAR ADC1 clock.
        (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
             the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
        (++) The Third output is used to generate an accurate clock to achieve 
             high-quality audio performance on SAI interface in case.

     (#) PLLSAI2 (clocked by HSI , HSE or MSI), featuring three different output clocks: 
        (++) The first output is used to generate SAR ADC2 clock.
        (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
             the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
        (++) The Third output is used to generate an accurate clock to achieve 
             high-quality audio performance on SAI interface in case.


      (#) CSS (Clock security system), once enable and if a HSE clock failure occurs 
         (HSE used directly or through PLL as System clock source), the System clock
         is automatically switched to HSI and an interrupt is generated if enabled. 
         The interrupt is linked to the Cortex-M4 NMI (Non-Maskable Interrupt) 
         exception vector.   

      (#) MCO (microcontroller clock output), used to output MSI, HSI, LSE, HSE or PLL
          clock (through a configurable prescaler) on PA8 pin. (TBC)

 @endverbatim
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - MSI  ON and used as system clock source
  *            - HSE, HSI, PLL and PLLSAIx OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS, MCO OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks  
  *            - LSI, LSE and RTC clocks 
  * @param  None
  * @retval None
  */
void RCC_DeInit(void)
{
  /* Set MSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSION, HSIKERON, HSIASFS, HSEON, HSECSSON, PLLSYSON, PLLSAIxON bits */
  RCC->CR &= (uint32_t)0xEAF6F4F7;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000800;
 
  /* Reset PLLSAIxCFGR register */
  RCC->PLLSAI1CFGR =  0x00000800;
  RCC->PLLSAI2CFGR =  0x00000800;
  
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;
}

/**
  * @brief  Configures the External High Speed oscillator (HSE).
  * @note   After enabling the HSE (RCC_HSE_ON or RCC_HSE_Bypass), the application
  *         software should wait on HSERDY flag to be set indicating that HSE clock
  *         is stable and can be used to clock the PLL and/or system clock.
  * @note   HSE state can not be changed if it is used directly or through the
  *         PLL as system clock. In this case, you have to select another source
  *         of the system clock then change the HSE state (ex. disable it).
  * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.  
  * @note   This function reset the CSSON bit, so if the Clock security system(CSS)
  *         was previously enabled you have to enable it again after calling this
  *         function.    
  * @param  RCC_HSE: specifies the new state of the HSE.
  *          This parameter can be one of the following values:
  *            @arg RCC_HSE_OFF: turn OFF the HSE oscillator, HSERDY flag goes low after
  *                              6 HSE oscillator clock cycles.
  *            @arg RCC_HSE_ON: turn ON the HSE oscillator
  *            @arg RCC_HSE_Bypass: HSE oscillator bypassed with external clock
  * @retval None
  */
void RCC_HSEConfig(uint8_t RCC_HSE)
{
  /* Check the parameters */
  assert_param(IS_RCC_HSE(RCC_HSE));

  /* Reset HSEON and HSEBYP bits before configuring the HSE ------------------*/
  *(__IO uint8_t *) CR_BYTE3_ADDRESS = RCC_HSE_OFF;

  /* Set the new HSE configuration -------------------------------------------*/
  *(__IO uint8_t *) CR_BYTE3_ADDRESS = RCC_HSE;
}


/**
  * @brief  Waits for HSE start-up.
  * @note   This functions waits on HSERDY flag to be set and return SUCCESS if 
  *         this flag is set, otherwise returns ERROR if the timeout is reached 
  *         and this flag is not set. The timeout value is defined by the constant
  *         HSE_STARTUP_TIMEOUT in stm32l4xx.h file. You can tailor it depending
  *         on the HSE crystal used in your application. 
  * @param  None
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: HSE oscillator is stable and ready to use
  *          - ERROR: HSE oscillator not yet ready
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t startupcounter = 0;
  ErrorStatus status = ERROR;
  FlagStatus hsestatus = RESET;
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    hsestatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    startupcounter++;
  } while((startupcounter != HSE_STARTUP_TIMEOUT) && (hsestatus == RESET));

  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return (status);
}
/**
  * @brief  Adjusts the Internal Multi Speed oscillator (MSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal MSI RC.
  *         Refer to the Application Note AN3300 for more details on how to  
  *         calibrate the MSI.
  * @param  MSICalibrationValue: specifies the MSI calibration trimming value.
  *   This parameter must be a number between 0 and 0xFF.
  * @retval None
  */
void RCC_AdjustMSICalibrationValue(uint32_t MSICalibrationValue)
{
  
  /* Check the parameters */
  assert_param(IS_RCC_MSI_CALIBRATION_VALUE(MSICalibrationValue));

  *(__IO uint8_t *) ICSCR_BYTE2_ADDRESS = MSICalibrationValue;  
}

/**
  * @brief  Configures the Internal Multi Speed oscillator (MSI) clock range in run mode
  * @note     After restart from Reset , the MSI clock is around 4 MHz. 
  *           After stop the startup clock can be MSI (at any of its possible 
  *           frequencies, the one that was used before enteringstop mode) or HSI.
  *          After Standby its frequency can be selected between 4 possible values 
  *          (1, 2, 4 or 8 MHz), refer to RCC_MSIStandbyRangeConfig().
  * @note     MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready
  *          (MSIRDY=1).
  *  @note    The MSI clock range after reset can be modified on the fly.     
  * @param  RCC_MSIRange: specifies the MSI Clock range.
  *   This parameter must be one of the following values:
  *     @arg RCC_MSIRange_0: MSI clock is around 100 KHz
  *     @arg RCC_MSIRange_1: MSI clock is around 200 KHz
  *     @arg RCC_MSIRange_2: MSI clock is around 400 KHz
  *     @arg RCC_MSIRange_3: MSI clock is around 800 KHz
  *     @arg RCC_MSIRange_4: MSI clock is around 1 MHz
  *     @arg RCC_MSIRange_5: MSI clock is around 2MHz 
  *     @arg RCC_MSIRange_6: MSI clock is around 4MHz (default after Reset)
  *     @arg RCC_MSIRange_7: MSI clock is around 8  MHz
  *     @arg RCC_MSIRange_8: MSI clock is around 16 MHz 
  *     @arg RCC_MSIRange_9: MSI clock is around 24 MHz
  *     @arg RCC_MSIRange_10: MSI clock is around 32 MHz 
  *     @arg RCC_MSIRange_11: MSI clock is around 48 MHz 
  *                   
  * @retval None
  */
void RCC_MSIRangeConfig(uint32_t RCC_MSIRange)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_RCC_MSI_CLOCK_RANGE(RCC_MSIRange));
  
  /*select that MSI range is read in V12 part*/
  RCC->CR |=RCC_CR_MSIRGSEL ;
    
  tmpreg = RCC->CR;
  
  /* Clear MSIRANGE[7:4] bits */
  tmpreg &= ~RCC_CR_MSIRANGE;
  
  /* Set the MSIRANGE[7:4] bits according to RCC_MSIRange value */
  tmpreg |= (uint32_t)RCC_MSIRange;

  /* Store the new value */
  RCC->CR = tmpreg;
}

/**
  * @brief  Configures the Internal Multi Speed oscillator (MSI) clock range after standby mode
  * @note     After wakeup from Standby , the MSI   frequency can be between 
  *           4 possible values (1, 2, 4 or 8 MHz).
  * @note     MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready
  *          (MSIRDY=1).
  *  @note    The MSI clock range after reset can be modified on the fly.     
  * @param  RCC_MSIRange: specifies the MSI Clock range.
  *   This parameter must be one of the following values:
  *     @arg RCC_MSIRange_4: MSI clock is around 1 MHz
  *     @arg RCC_MSIRange_5: MSI clock is around 2MHz 
  *     @arg RCC_MSIRange_6: MSI clock is around 4MHz 
  *     @arg RCC_MSIRange_7: MSI clock is around 8  MHz
  *                   
  * @retval None
  */
void RCC_MSIStandbyRangeConfig(uint32_t RCC_MSIRange)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_RCC_MSI_CLOCK_RANGE(RCC_MSIRange));
  
  /*MSISTBYRG can be writen only if MSIRGSEL = 1*/
  RCC->CR |=RCC_CR_MSIRGSEL ;
    
  tmpreg = RCC->CSR;
  
  /* Clear MSIRANGE[11:8] bits */
  tmpreg &= ~RCC_CSR_MSIRANGE;
  
  /* Set the MSIRANGE[11:8] bits according to RCC_MSIRange value */
  tmpreg |= ((uint32_t)RCC_MSIRange << 4);

  /* Store the new value */
  RCC->CSR = tmpreg;
}

/**
  * @brief  Enables or disables the Internal Multi Speed oscillator (MSI).
  * @note     The MSI is stopped by hardware when entering STOP and STANDBY modes.
  *           It is used (enabled by hardware) as system clock source after
  *           startup from Reset, wakeup from STOP and STANDBY mode, or in case
  *           of failure of the HSE used directly or indirectly as system clock
  *           (if the Clock Security System CSS is enabled).             
  * @note     MSI can not be stopped if it is used as system clock source.
  *           In this case, you have to select another source of the system
  *           clock then stop the MSI.  
  * @note     After enabling the MSI, the application software should wait on
  *           MSIRDY flag to be set indicating that MSI clock is stable and can
  *           be used as system clock source.                                       
  * @param  NewState: new state of the MSI.
  *   This parameter can be: ENABLE or DISABLE.
  * @note   When the MSI is stopped, MSIRDY flag goes low after 6 MSI oscillator
  *         clock cycles.  
  * @retval None
  */
void RCC_MSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CR_MSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  HSICalibrationValue: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  * @retval None
  */
void RCC_AdjustHSICalibrationValue(uint32_t HSICalibrationValue)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_RCC_HSI_CALIBRATION_VALUE(HSICalibrationValue));

  tmpreg = RCC->ICSCR;

  /* Clear HSITRIM[4:0] bits */
  tmpreg &= ~RCC_ICSCR_HSITRIM;

  /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value */
  tmpreg |= (uint32_t)HSICalibrationValue << 24;

  /* Store the new value */
  RCC->ICSCR = tmpreg;
}

/**
  * @brief  Enables or disables the Internal High Speed oscillator (HSI).
  * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
  *         It is used (enabled by hardware) as system clock source after startup
  *         from Reset, wakeup from STOP and STANDBY mode, or in case of failure
  *         of the HSE used directly or indirectly as system clock (if the Clock
  *         Security System CSS is enabled).             
  * @note   HSI can not be stopped if it is used as system clock source. In this case,
  *         you have to select another source of the system clock then stop the HSI.  
  * @note   After enabling the HSI, the application software should wait on HSIRDY
  *         flag to be set indicating that HSI clock is stable and can be used as
  *         system clock source.  
  * @param  NewState: new state of the HSI.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles.  
  * @retval None
  */
void RCC_HSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the External Low Speed oscillator (LSE).
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using 
  *         PWR_BackupAccessCmd(ENABLE) function before to configure the LSE
  *         (to be done once after reset).  
  * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_Bypass), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  RCC_LSE: specifies the new state of the LSE.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSE_OFF: turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg RCC_LSE_ON: turn ON the LSE oscillator
  *            @arg RCC_LSE_Bypass: LSE oscillator bypassed with external clock
  * @retval None
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* Check the parameters */
  assert_param(IS_RCC_LSE(RCC_LSE));

  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* Reset LSEON bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Reset LSEBYP bit */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
  switch (RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;
    default:
      break;
  }
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on 
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  * @note   LSI can not be disabled if the IWDG is running.  
  * @param  NewState: new state of the LSI.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles. 
  * @retval None
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the main PLL clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL is disabled.
  *  
  * @param  RCC_PLLSource: specifies the PLL entry clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_PLLSource_MSI: MSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSource_HSI: HSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSource_HSE: HSE oscillator clock selected as PLL clock entry
  * @note   This clock source (RCC_PLLSource) is common for the main PLL and PLLSAI1 & PLLSAI2 .  
  *  
  * @param  PLLM: specifies the division factor for PLL VCO input clock
  *          This parameter must be a number between 1 and 8.
  * @note   You have to set the PLLM parameter correctly to ensure that the VCO input
  *         frequency ranges from 4 to 8 MHz. It is recommended to select a frequency
  *         of 8 MHz to limit PLL jitter. (TBC)
  *  
  * @param  PLLN: specifies the multiplication factor for PLL VCO output clock
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLN parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *   
  * @param  PLLP: specifies the division factor for SAI clock.
  *          This parameter must be a number in the range {7 or 17}.
  *  
  * @param  PLLQ: specifies the division factor for OTG FS, SDIO and RNG clocks
  *          This parameter must be in the range (2 , 4 ,6 or 8)
  * @note   If the USB OTG FS is used in your application, you have to set the
  *         PLLQ parameter correctly to have 48 MHz clock for the USB. However,
  *         the SDIO and RNG need a frequency lower than or equal to 48 MHz to work
  *         correctly.
  * @param  PLLR: specifies the division factor for the main system clock.
  * @note   You have to set the PLLR parameter correctly to not execced 80MHZ.
  *          This parameter must be in the range (2 , 4 ,6 or 8)
  * @retval None
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ ,uint32_t PLLR)
{
  /* Check the parameters */
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLLM_VALUE(PLLM));
  assert_param(IS_RCC_PLLN_VALUE(PLLN));
  assert_param(IS_RCC_PLLP_VALUE(PLLP));
  assert_param(IS_RCC_PLLQ_VALUE(PLLQ));
  assert_param(IS_RCC_PLLR_VALUE(PLLR));

  RCC->PLLCFGR = ((PLLM-1) << 4) | (PLLN << 8) | ((PLLP >> 4 ) << 17) | (RCC_PLLSource) |
                 (((PLLQ /2)-1) << 21) | (((PLLR/2)-1) << 25) ;
}

/**
  * @brief  Enables or disables the main PLL.
  * @note   After enabling the main PLL, the application software should wait on 
  *         PLLRDY flag to be set indicating that PLL clock is stable and can
  *         be used as system clock source.
  * @note   The main PLL can not be disabled if it is used as system clock source
  * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
  * @param  NewState: new state of the main PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLLCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables each clock output (ck_pll_P, ck_pll_Q, ck_pll_R PLL.)
  * @note   Enabling/disabling  Those Clocks can be anytime  without the need to stop the PLL, 
  *        (except the ck_pll_R of the System PLL that cannot be stopped if used as System
  *         Clock.This is mainly used to save Power.
  * @param  RCC_PLLClockOut: specifies the PLL clock to be output 
  *          This parameter can be one of the following values:
  *            @arg RCC_PLLCLk_P: This clock is used to generate an accurate clock to achieve 
  *                               high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLCLk_Q: This Clock is used to generate the clock for the USB OTG FS (48 MHz),
  *                               the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg RCC_PLLCLk_R: This Clock is used to generate the high speed system clock (up to 80MHz)
  * @param  NewState: new state of the main PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */


void RCC_PLLClockOutCmd(uint32_t RCC_PLLClockOut, FunctionalState NewState)
{
    __IO uint32_t tmp = 0;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_RCC_PLLCLOCKOUT_VALUE(RCC_PLLClockOut));

  tmp = PLLCFGR_PLLSYSPEN_BB + RCC_PLLClockOut;
  
  *(__IO uint32_t *) (tmp) = (uint32_t)NewState;   
    
}



/**
  * @brief  Configures the PLLSAI1 clock multiplication and division factors.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured in 
  *         RCC_PLLConfig function )  
  *             
  * @param  PLLSAI1N: specifies the multiplication factor for PLLSAI1 VCO output clock
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI1N parameter correctly to ensure that the VCO 
  *         output frequency is between 64 and 344 MHz.
  *
  * @param  PLLSAI1P: specifies the division factor for SAI clock
  *          This parameter must be a number in the range {7 or 17}.
  *           
  * @param  PLLSAI1Q: specifies the division factor for USB clock
  *          This parameter must be in the range (2 , 4 ,6 or 8)
  *            
  * @param  PLLSAI1R: specifies the division factor for SAR ADC clock
  *          This parameter must be in the range (2 , 4 ,6 or 8)
  *   
  * @retval None
  */
void RCC_PLLSAI1Config(uint32_t PLLSAI1N, uint32_t PLLSAI1P,uint32_t PLLSAI1Q, uint32_t PLLSAI1R)
{
  /* Check the parameters */
  assert_param(IS_RCC_PLLSAI1N_VALUE(PLLSAI1N));
  assert_param(IS_RCC_PLLSAI1P_VALUE(PLLSAI1P));
  assert_param(IS_RCC_PLLSAI1Q_VALUE(PLLSAI1Q));
  assert_param(IS_RCC_PLLSAI1R_VALUE(PLLSAI1R));
  
  RCC->PLLSAI1CFGR = (PLLSAI1N << 8) | ((PLLSAI1P >> 4 ) << 17)  | (((PLLSAI1Q /2)-1) << 21) | (((PLLSAI1R/2)-1) << 25) ;
   
   
}

/**
  * @brief  Enables or disables the PLLSAI1. 
  * 
  * @note   The PLLSAI1 is disabled by hardware when entering STOP and STANDBY modes.  
  * @param  NewState: new state of the PLLSAI1. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLLSAI1Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLLSAI1ON_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the PLLSAI2 clock multiplication and division factors.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured in 
  *         RCC_PLLConfig function )  
  *             
  * @param  PLLSAI2N: specifies the multiplication factor for PLLSAI2 VCO output clock
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI2N parameter correctly to ensure that the VCO 
  *         output frequency is between 64 and 344 MHz.
  *
  * @param  PLLSAI1P: specifies the division factor for SAI clock
  *           This parameter must be a number in the range {7 or 17}.
  *           
  * @param  PLLSAI2Q: specifies the division factor for USB clock
  *           This parameter must be in the range (2 , 4 ,6 or 8)
  *            
  * @param  PLLSAI2R: specifies the division factor for SAR ADC clock
  *          This parameter must be in the range (2 , 4 ,6 or 8)
  *   
  * @retval None
  */

/**
  * @brief  Enables or disables each clock output (ck_pll_P, ck_pll_Q, ck_pll_R PLL.)
  * @note   Enabling/disabling  Those Clocks can be anytime  without the need to stop the PLL, 
  *         This is mainly used to save Power.
  * @param  RCC_PLLSAI1ClockOut: specifies the PLL clock to be output 
  *          This parameter can be one of the following values:
  *            @arg RCC_PLLSAI1CLk_P: This clock is used to generate an accurate clock to achieve 
  *                               high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI1CLk_Q: This Clock is used to generate the clock for the USB OTG FS (48 MHz),
  *                               the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg RCC_PLLSAI1CLk_R: This Clock is used to generate the high speed system clock (up to 80MHz)
  * @param  NewState: new state of the main PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */


void RCC_PLLSAI1ClockOutCmd(uint32_t RCC_PLLSAI1ClockOut, FunctionalState NewState)
{
    __IO uint32_t tmp = 0;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_RCC_PLLSAI1CLOCKOUT_VALUE(RCC_PLLSAI1ClockOut));

  tmp = PLLSAI1CFGR_PLLSAI1PEN_BB + RCC_PLLSAI1ClockOut;
  
  *(__IO uint32_t *) (tmp) = (uint32_t)NewState;   
    
}

void RCC_PLLSAI2Config(uint32_t PLLSAI2N, uint32_t PLLSAI2P, uint32_t PLLSAI2Q, uint32_t PLLSAI2R)
{
  /* Check the parameters */
  assert_param(IS_RCC_PLLSAI2N_VALUE(PLLSAI2N));
  assert_param(IS_RCC_PLLSAI2P_VALUE(PLLSAI2P));
  assert_param(IS_RCC_PLLSAI2Q_VALUE(PLLSAI2Q));
  assert_param(IS_RCC_PLLSAI2R_VALUE(PLLSAI2R));
 
   RCC->PLLSAI2CFGR = (PLLSAI2N << 8) | ((PLLSAI2P >> 4 ) << 17)  | (((PLLSAI2Q /2)-1) << 21) | (((PLLSAI2R/2)-1) << 25) ;
}

/**
  * @brief  Enables or disables the PLLSAI2. 
  * 
  * @note   The PLLSAI2 is disabled by hardware when entering STOP and STANDBY modes.  
  * @param  NewState: new state of the PLLSAI2. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLLSAI2Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLLSAI2ON_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables each clock output (ck_pll_P, ck_pll_Q, ck_pll_R PLL.)
  * @note   Enabling/disabling  Those Clocks can be anytime  without the need to stop the PLL, 
  *         This is mainly used to save Power.
  * @param  RCC_PLLSAI2ClockOut: specifies the PLL clock to be output 
  *          This parameter can be one of the following values:
  *            @arg RCC_PLLSAI2CLk_P: This clock is used to generate an accurate clock to achieve 
  *                               high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI2CLk_Q: This Clock is used to generate the clock for the USB OTG FS (48 MHz),
  *                               the random analog generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg RCC_PLLSAI2CLk_R: This Clock is used to generate the high speed system clock (up to 80MHz)
  * @param  NewState: new state of the main PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */


void RCC_PLLSAI2ClockOutCmd(uint32_t RCC_PLLSAI2ClockOut, FunctionalState NewState)
{
    __IO uint32_t tmp = 0;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_RCC_PLLSAI2CLOCKOUT_VALUE(RCC_PLLSAI2ClockOut));

  tmp = PLLSAI2CFGR_PLLSAI2PEN_BB + RCC_PLLSAI2ClockOut;
  
  *(__IO uint32_t *) (tmp) = (uint32_t)NewState;   
    
}
/**
  * @brief  Enables or disables the Clock Security System on HSE.
  * @note   If a failure is detected on the HSE oscillator clock, this oscillator
  *         is automatically disabled and an interrupt is generated to inform the
  *         software about the failure (Clock Security System Interrupt, CSSI),
  *         allowing the MCU to perform rescue operations. The CSSI is linked to 
  *         the Cortex-M4 NMI (Non-Maskable Interrupt) exception vector.  
  * @param  NewState: new state of the Clock Security System on HSE.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSEClockSecuritySystemCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_HSECSSON_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables the Clock Security System on LSE.
  * @note   If a failure is detected on the LSE oscillator clock, this oscillator
  *         is no more delivered to the RTC but no hardware action is done
  *         on the registers. If MSI was in PLL mode, this mode is disabled.
  *         In Standby mode a wakeup is generated, in other modes an interrupt
  *         can be send to wake-up the software
  * @param  NewState: new state of the Clock Security System on LSE.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_LSEClockSecuritySystemCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_LSECSSON_BB = (uint32_t)NewState;
}

/**
  * @brief  Select the Clock source after wakeup from Stop mode.
  * @note   The selected clock is also used as emergency clock for the Clock 
  *         Security System on HSE. 
  * @note   Warning: STOPWUCK must not be modified when the Clock Security System 
  *         is enabled by HSECSSON in CR register and the system clock is HSE
  *        (SWS=”10”) or a switch on HSE is requested (SW=”10”)
  * @param  RCC_STOPWUCK_Clock: specifies the clock source after wake up from STOP mode
  *         This clock source can be the emergency clock for CSS.
  *         This parameter can be one of the following values:
  *            @arg RCC_STOPWUCK_MSI: MSI clock selected as STOPWUCK source
  *            @arg RCC_STOPWUCK_HSI: HSI clock selected as STOPWUCK source
  * @retval None
  */
void RCC_StopWakeUpClock(uint32_t RCC_STOPWUCK_Clock)
{
  /* Check the parameters */
  assert_param(IS_RCC_STOPWUP_VALUE(RCC_STOPWUCK_Clock));
  *(__IO uint32_t *) CFGR_STOPWUCK_BB = (uint32_t)RCC_STOPWUCK_Clock;
}

/**
  * @brief  Selects the clock source to output on MCO pin(PA8). 
  * @note   PA8 should be configured in alternate function mode.
  * @param  RCC_MCO1Source: specifies the clock source to output.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCOSource_SYSCLK: system  clock selected as MCO source
  *            @arg RCC_MCOSource_MSI: MSI clock selected as MCO source
  *            @arg RCC_MCOSource_HSI: HSI clock selected as MCO source
  *            @arg RCC_MCOSource_HSE: HSE clock selected as MCO source
  *            @arg RCC_MCOSource_HSE: HSE clock selected as MCO source
  *            @arg RCC_MCOSource_PLLCLK: main PLL clock selected as MCO source
  *            @arg RCC_MCOSource_LSI: LSI clock selected as MCO source
  *            @arg RCC_MCOSource_LSE: LSE clock selected as MCO source
  * @param  RCC_MC1Div: specifies the MCO prescaler.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCODiv_1: no division applied to MCO clock
  *            @arg RCC_MCODiv_2: division by 2 applied to MCO clock
  *            @arg RCC_MCODiv_4: division by 4 applied to MCO clock
  *            @arg RCC_MCODiv_8: division by 8 applied to MCO clock
  *            @arg RCC_MCODiv_16: division by 16 applied to MCO clock
  * @retval None
  */
void RCC_MCOConfig(uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_RCC_MCOSOURCE(RCC_MCOSource));
  assert_param(IS_RCC_MCODIV(RCC_MCODiv));  

  tmpreg = RCC->CFGR;

  /* Clear MCO[1:0] and MCOPRE[2:0] bits */
  tmpreg &= CFGR_MCO_RESET_MASK;

  /* Select MCO clock source and prescaler */
  tmpreg |= RCC_MCOSource | RCC_MCODiv;

  /* Store the new value */
  RCC->CFGR = tmpreg;  
}


/** @defgroup RCC_Group2 System AHB and APB busses clocks configuration functions
 *  @brief   System, AHB and APB busses clocks configuration functions
 *
@verbatim   
 ===============================================================================
      ##### System, AHB and APB busses clocks configuration functions #####
 ===============================================================================  
    [..]
      This section provide functions allowing to configure the System, AHB, APB1 and 
      APB2 busses clocks.
  
      (#) Several clock sources can be used to drive the System clock (SYSCLK): MSI, HSI,
          HSE and PLL.
          The AHB clock (HCLK) is derived from System clock through configurable 
          prescaler and used to clock the CPU, memory and peripherals mapped 
          on AHB bus (DMA, GPIO...). APB1 (PCLK1) and APB2 (PCLK2) clocks are derived 
          from AHB clock through configurable prescalers and used to clock 
          the peripherals mapped on these busses. You can use 
          "RCC_GetClocksFreq()" function to retrieve the frequencies of these clocks.  

      -@- All the peripheral clocks are derived from the System clock (SYSCLK) except:
        (+@) The USB full speed clock (48 MHz) which is coming from a specific output 
             of three PLLs or from MSI.
        (+@) The SDIO and RNG are using the same 48MHz clock as the USB
        (+@) The SAI clocks which are coming from a specific output of the three PLLs. 
        (+@) The USART clocks which can be derived from the system clock or HSI or LSE
        (+@) the I2C clocks which can be derived from system clock or from HSI
        (+@) The Low Power Timer clock which can be derived from system clock or HSI or LSI or LSE
        (+@) The SAR ADC clock which is coming from a specific output of two PLLs or from 
             the system clock.
        (+@) The SWP clock can be derived from HSI
        (+@) The RTC in the Vswitch domain is clocked either by the external 32 kHz 
            oscillator present in the Vswitch domain,or by the internal RC 32 kHz , or by 
           the external oscillator (4-48 MHz) divided by 32 .
      (#) The maximum frequency of the SYSCLK and HCLK is 80 MHz, PCLK2 40 MHz 
          and PCLK1 20 MHz. Depending on the device voltage range, the maximum 
          frequency should be adapted accordingly:
 +-------------------------------------------------+     
 | Latency       |    HCLK clock frequency (MHz)   |
 |               |     VDD33 range: 1.6V-3.6V      |
 |               |---------------------------------|     
 |               | VDD12 range1   | VDD12 range2   | 
 |               |  1.08V-1.32V   | 0.90 V-1.10V   | 
 |---------------|----------------|----------------|            
 |0WS(1CPU cycle)|0 < HCLK <= 16  |0 < HCLK <= 6   |
 |---------------|----------------|----------------|
 |1WS(2CPU cycle)|16 < HCLK <= 32 |6 < HCLK <= 12  |
 |---------------|----------------|----------------| 
 |2WS(3CPU cycle)|32 < HCLK <= 48 |12 < HCLK <= 18 |
 |---------------|----------------|----------------|
 |3WS(4CPU cycle)|48 < HCLK <= 64 |18 < HCLK <= 24 |
 |---------------|----------------|----------------|
 |4WS(5CPU cycle)|64 < HCLK <= 80 |18 < HCLK <= 24 |
 |---------------|----------------|----------------| 

      -@- When VOSbit (in PWR_CR register) is reset to 0 , the maximum value of HCLK is 80 MHz.
          You can use PWR_MainRegulatorModeConfig() function to set or reset this bit.

@endverbatim
  * @{
  */

/**
  * @brief  Configures the system clock (SYSCLK).
  * @note   The MSI is used (enabled by hardware) as system clock source after
  *         startup from Reset, wake-up from STOP and STANDBY mode, or in case
  *         of failure of the HSE used directly or indirectly as system clock
  *         (if the Clock Security System CSS is enabled).
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after startup delay or PLL locked). 
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready. 
  *         You can use RCC_GetSYSCLKSource() function to know which clock is
  *         currently used as system clock source. 
  * @param  RCC_SYSCLKSource: specifies the clock source used as system clock.
  *          This parameter can be one of the following values:
  *            @arg RCC_SYSCLKSource_MSI:    MSI selected as system clock source
  *            @arg RCC_SYSCLKSource_HSI:    HSI selected as system clock source
  *            @arg RCC_SYSCLKSource_HSE:    HSE selected as system clock source
  *            @arg RCC_SYSCLKSource_PLLCLK: PLL selected as system clock source
  * @retval None
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

  tmpreg = RCC->CFGR;

  /* Clear SW[1:0] bits */
  tmpreg &= ~RCC_CFGR_SW;

  /* Set SW[1:0] bits according to RCC_SYSCLKSource value */
  tmpreg |= RCC_SYSCLKSource;

  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Returns the clock source used as system clock.
  * @param  None
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following:
  *              - 0x00: MSI used as system clock
  *              - 0x04: HSI used as system clock
  *              - 0x08: HSE used as system clock
  *              - 0x0C: PLL used as system clock
  */
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & RCC_CFGR_SWS));
}

/**
  * @brief  Configures the AHB clock (HCLK).
  * @note   Depending on the device voltage range, the software has to set correctly
  *         these bits to ensure that HCLK not exceed the maximum allowed frequency
  *         (for more details refer to section above
  *           "CPU, AHB and APB busses clocks configuration functions")
  * @param  RCC_SYSCLK: defines the AHB clock divider. This clock is derived from 
  *         the system clock (SYSCLK).
  *          This parameter can be one of the following values:
  *            @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *            @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *            @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *            @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *            @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *            @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *            @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *            @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *            @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @retval None
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));

  tmpreg = RCC->CFGR;

  /* Clear HPRE[3:0] bits */
  tmpreg &= ~RCC_CFGR_HPRE;

  /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
  tmpreg |= RCC_SYSCLK;

  /* Store the new value */
  RCC->CFGR = tmpreg;
}


/**
  * @brief  Configures the Low Speed APB clock (PCLK1).
  * @param  RCC_HCLK: defines the APB1 clock divider. This clock is derived from 
  *         the AHB clock (HCLK).
  *          This parameter can be one of the following values:
  *            @arg RCC_HCLK_Div1:  APB1 clock = HCLK
  *            @arg RCC_HCLK_Div2:  APB1 clock = HCLK/2
  *            @arg RCC_HCLK_Div4:  APB1 clock = HCLK/4
  *            @arg RCC_HCLK_Div8:  APB1 clock = HCLK/8
  *            @arg RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @retval None
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE1[2:0] bits */
  tmpreg &= ~RCC_CFGR_PPRE1;

  /* Set PPRE1[2:0] bits according to RCC_HCLK value */
  tmpreg |= RCC_HCLK;

  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Configures the High Speed APB clock (PCLK2).
  * @param  RCC_HCLK: defines the APB2 clock divider. This clock is derived from 
  *         the AHB clock (HCLK).
  *          This parameter can be one of the following values:
  *            @arg RCC_HCLK_Div1:  APB2 clock = HCLK
  *            @arg RCC_HCLK_Div2:  APB2 clock = HCLK/2
  *            @arg RCC_HCLK_Div4:  APB2 clock = HCLK/4
  *            @arg RCC_HCLK_Div8:  APB2 clock = HCLK/8
  *            @arg RCC_HCLK_Div16: APB2 clock = HCLK/16
  * @retval None
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE2[2:0] bits */
  tmpreg &= ~RCC_CFGR_PPRE2;

  /* Set PPRE2[2:0] bits according to RCC_HCLK value */
  tmpreg |= RCC_HCLK << 3;

  /* Store the new value */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  Returns the frequencies of different on chip clocks;  System, AHB, APB2 
  *   and APB1 busses clocks,  USART1/2/3/4/5 /6 clocks, I2C1/2 clocks.
  * @note   The system frequency computed by this function is not the real 
  *         frequency in the chip. It is calculated based on the predefined 
  *         constant and the selected clock source:
  * @note     If SYSCLK source is MSI, function returns values based on MSI_VALUE(*)
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(**)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(***)
  * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(***) 
  *           or HSI_VALUE(**) or MSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note     (*) MSI_VALUE is a constant defined in stm32l4xx.h file (default value
  *               4 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSI_VALUE is a constant defined in stm32l4xx.h file (default value
  *               16 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (***) HSE_VALUE is a constant defined in stm32l4xx.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *                
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *   
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *          the clocks frequencies.
  *     
  * @note   This function can be used by the user application to compute the 
  *         baudrate for the communication peripherals or configure other parameters.
  * @note   Each time SYSCLK, HCLK, PCLK1 and/or PCLK2 clock changes, this function
  *         must be called to update the structure's field. Otherwise, any
  *         configuration based on this function will be incorrect.
  *    
  * @retval None
  */
// void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
// {
//   uint32_t tmp = 0, msirange = 0, presc = 0, pllvco = 0, pllr = 2, pllsource = 0, pllm = 2;

//   /* Get SYSCLK source -------------------------------------------------------*/
//   tmp = RCC->CFGR & RCC_CFGR_SWS;
//   
//   msirange=(RCC->CR & RCC_CR_MSIRANGE)>> 4 ; 
//   /*MSI frequency range in HZ*/
//   msirange = MSIRangeTable[msirange];
//   
//   switch (tmp)
//   {
//   case 0x00:  /* MSI used as system clock source */
//     RCC_Clocks->SYSCLK_Frequency = msirange;
//     break;
//   case 0x04:  /* HSI used as system clock  source */
//     RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
//     break;
//     
//   case 0x08:  /* HSE used as system clock  source */
//     RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
//     break;
//     
//   case 0x0C:  /* PLL used as system clock  source */
//     
//     /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
//     SYSCLK = PLL_VCO / PLLR
//     */    
//     pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
//     pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM)>> 4) + 1 ;
//     
//     
//     switch (pllsource)
//     {
//     case 0x01:  /* MSI used as PLL clock source */
//       pllvco = (msirange / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
//       break;
//       
//     case 0x02:  /* HSI used as PLL clock source */
//       pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
//       break;
//       
//     case 0x03:  /* HSE used as PLL clock source */
//       pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
//       break;
//       
//     default:
//       pllvco = (msirange / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
//       break;
//     }
//     pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >>25) + 1 ) *2;
//     RCC_Clocks->SYSCLK_Frequency = pllvco/pllr;
//     break;
//   default:
//     RCC_Clocks->SYSCLK_Frequency = msirange;
//     break;
//   }
//   /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/
//     
//   /* Get HCLK prescaler */
//   tmp = RCC->CFGR & RCC_CFGR_HPRE;
//   tmp = tmp >> 4;
//   presc = APBAHBPrescTable[tmp];
//   /* HCLK clock frequency */
//   RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

//   /* Get PCLK1 prescaler */
//   tmp = RCC->CFGR & RCC_CFGR_PPRE1;
//   tmp = tmp >> 8;
//   presc = APBAHBPrescTable[tmp];
//   /* PCLK1 clock frequency */
//   RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

//   /* Get PCLK2 prescaler */
//   tmp = RCC->CFGR & RCC_CFGR_PPRE2;
//   tmp = tmp >> 11;
//   presc = APBAHBPrescTable[tmp];
//   /* PCLK2 clock frequency */
//   RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
//   
//   /* USART1CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_USART1SEL) == 0x0)
//   {
//     /* USART Clock is PCLK */
//     RCC_Clocks->USART1CLK_Frequency = RCC_Clocks->PCLK2_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART1SEL) ==  RCC_CCIPR_USART1SEL_0)
//   {
//     /* USART Clock is System Clock */
//     RCC_Clocks->USART1CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART1SEL) ==  RCC_CCIPR_USART1SEL_1)
//   {
//     /* USART Clock is HSI Osc. */
//     RCC_Clocks->USART1CLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART1SEL) ==  RCC_CCIPR_USART1SEL)
//   {
//     /* USART Clock is LSE Osc. */
//     RCC_Clocks->USART1CLK_Frequency = LSE_VALUE;
//   }
//   
//   /* USART2CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_USART2SEL) == 0x0)
//   {
//     /* USART Clock is PCLK */
//     RCC_Clocks->USART2CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART2SEL) ==  RCC_CCIPR_USART2SEL_0)
//   {
//     /* USART Clock is System Clock */
//     RCC_Clocks->USART2CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART2SEL) ==  RCC_CCIPR_USART2SEL_1)
//   {
//     /* USART Clock is HSI Osc. */
//     RCC_Clocks->USART2CLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART2SEL) ==  RCC_CCIPR_USART2SEL)
//   {
//     /* USART Clock is LSE Osc. */
//     RCC_Clocks->USART2CLK_Frequency = LSE_VALUE;
//   }
//   
//   /* USART3CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_USART3SEL) == 0x0)
//   {
//     /* USART Clock is PCLK */
//     RCC_Clocks->USART3CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART3SEL) ==  RCC_CCIPR_USART3SEL_0)
//   {
//     /* USART Clock is System Clock */
//     RCC_Clocks->USART3CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART3SEL) ==  RCC_CCIPR_USART3SEL_1)
//   {
//     /* USART Clock is HSI Osc. */
//     RCC_Clocks->USART3CLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_USART3SEL) ==  RCC_CCIPR_USART3SEL)
//   {
//     /* USART Clock is LSE Osc. */
//     RCC_Clocks->USART3CLK_Frequency = LSE_VALUE;
//   }
//   
//   /* UART4CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_UART4SEL) == 0x0)
//   {
//     /* UART Clock is PCLK */
//     RCC_Clocks->UART4CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART4SEL) ==  RCC_CCIPR_UART4SEL_0)
//   {
//     /* UART Clock is System Clock */
//     RCC_Clocks->UART4CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART4SEL) ==  RCC_CCIPR_UART4SEL_1)
//   {
//     /* UART Clock is HSI Osc. */
//     RCC_Clocks->UART4CLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART4SEL) ==  RCC_CCIPR_UART4SEL)
//   {
//     /* UART Clock is LSE Osc. */
//     RCC_Clocks->UART4CLK_Frequency = LSE_VALUE;
//   }
//   
//   /* UART5CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_UART5SEL) == 0x0)
//   {
//     /* UART Clock is PCLK */
//     RCC_Clocks->UART5CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART5SEL) ==  RCC_CCIPR_UART5SEL_0)
//   {
//     /* UART Clock is System Clock */
//     RCC_Clocks->UART5CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART5SEL) ==  RCC_CCIPR_UART5SEL_1)
//   {
//     /* UART Clock is HSI Osc. */
//     RCC_Clocks->UART5CLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_UART5SEL) ==  RCC_CCIPR_UART5SEL)
//   {
//     /* UART Clock is LSE Osc. */
//     RCC_Clocks->UART5CLK_Frequency = LSE_VALUE;
//   }
//   
//   /* LPUARTCLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_LPUARTSEL) == 0x0)
//   {
//     /* LPUART Clock is PCLK */
//     RCC_Clocks->LPUARTCLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_LPUARTSEL) ==  RCC_CCIPR_LPUARTSEL_0)
//   {
//     /* LPUART Clock is System Clock */
//     RCC_Clocks->LPUARTCLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_LPUARTSEL) ==  RCC_CCIPR_LPUARTSEL_1)
//   {
//     /* LPUART Clock is HSI Osc. */
//     RCC_Clocks->LPUARTCLK_Frequency = HSI_VALUE;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_LPUARTSEL) ==  RCC_CCIPR_LPUARTSEL)
//   {
//     /* LPUART Clock is LSE Osc. */
//     RCC_Clocks->LPUARTCLK_Frequency = LSE_VALUE;
//   }
//   
//   /* I2C1 CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_I2C1SEL) == 0x0)
//   {
//     /* I2C1 Clock is PCLK */
//     RCC_Clocks->I2C1CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C1SEL) ==  RCC_CCIPR_I2C1SEL_0)
//   {
//     /* I2C1 Clock is System Clock */
//     RCC_Clocks->I2C1CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C1SEL) ==  RCC_CCIPR_I2C1SEL_1)
//   {
//     /* I2C1 Clock is HSI Osc. */
//     RCC_Clocks->I2C1CLK_Frequency = HSI_VALUE;
//   }
//   
//   /* I2C2 CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_I2C2SEL) == 0x0)
//   {
//     /* I2C2 Clock is PCLK */
//     RCC_Clocks->I2C2CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C2SEL) ==  RCC_CCIPR_I2C2SEL_0)
//   {
//     /* I2C2 Clock is System Clock */
//     RCC_Clocks->I2C2CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C2SEL) ==  RCC_CCIPR_I2C2SEL_1)
//   {
//     /* I2C2 Clock is HSI Osc. */
//     RCC_Clocks->I2C2CLK_Frequency = HSI_VALUE;
//   }
//  
//   /* I2C3 CLK clock frequency */
//   if((RCC->CCIPR &  RCC_CCIPR_I2C3SEL) == 0x0)
//   {
//     /* I2C3 Clock is PCLK */
//     RCC_Clocks->I2C3CLK_Frequency = RCC_Clocks->PCLK1_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C3SEL) ==  RCC_CCIPR_I2C3SEL_0)
//   {
//     /* I2C3 Clock is System Clock */
//     RCC_Clocks->I2C3CLK_Frequency = RCC_Clocks->SYSCLK_Frequency;
//   }
//   else if((RCC->CCIPR &  RCC_CCIPR_I2C3SEL) ==  RCC_CCIPR_I2C3SEL_1)
//   {
//     /* I2C3 Clock is HSI Osc. */
//     RCC_Clocks->I2C3CLK_Frequency = HSI_VALUE;
//   }
//  
//   
// }

/**
  * @}
  */

/** @defgroup RCC_Group3 Peripheral clocks configuration functions
 *  @brief   Peripheral clocks configuration functions 
 *
@verbatim   
 ===============================================================================
              ##### Peripheral clocks configuration functions #####
 ===============================================================================  
    [..] This section provide functions allowing to configure the Peripheral clocks. 
  
      (#) The RTC clock which is derived from the LSI, LSE or HSE clock divided 
          by 2 to 31.
     
      (#) After restart from Reset or wakeup from STANDBY, all peripherals are off
          except internal SRAM, Flash and JTAG. Before to start using a peripheral 
          you have to enable its interface clock. You can do this using 
          RCC_AHB1PeriphClockCmd(), RCC_AHB2PeriphClockCmd(),RCC_AHB3PeriphClockCmd(),
          RCC_APB1LPeriphClockCmd(), RCC_APB1HPeriphClockCmd()
          and RCC_APB2PeriphClockCmd() functions.

      (#) To reset the peripherals configuration (to the default state after device reset)
          you can use RCC_AHB1PeriphResetCmd(), RCC_AHB2PeriphResetCmd(),
          RCC_AHB3PeriphResetCmd(), RCC_APB1LPeriphResetCmd(), RCC_APB1HPeriphResetCmd()
          and RCC_APB2PeriphResetCmd() functions.
     
      (#) To further reduce power consumption in SLEEP mode the peripheral clocks 
          can be disabled prior to executing the WFI or WFE instructions. 
          You can do this using RCC_AHB1PeriphClockLPModeCmd(), RCC_AHB2PeriphClockLPModeCmd(),
          RCC_AHB3PeriphClockLPModeCmd(), RCC_APB2PeriphClockLPModeCmd(),
          RCC_APB1LPeriphClockLPModeCmd(), RCC_APB1HPeriphClockLPModeCmd() functions.  

@endverbatim
  * @{
  */

/**
  * @brief  Configures the RTC clock (RTCCLK).
  * @note   As the RTC clock configuration bits are in the Backup domain and write
  *         access is denied to this domain after reset, you have to enable write
  *         access using PWR_BackupAccessCmd(ENABLE) function before to configure
  *         the RTC clock source (to be done once after reset).    
  * @note   Once the RTC clock is configured it can't be changed unless the  
  *         Backup domain is reset using RCC_BackupResetCmd() function, or by
  *         a Power On Reset (POR).
  *    
  * @param  RCC_RTCCLKSource: specifies the RTC clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
  *            @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
  *            @arg RCC_RTCCLKSource_HSE: HSE clock divided by 32.
  *  
  * @note   If the LSE or LSI is used as RTC clock source, the RTC continues to
  *         work in STOP and STANDBY modes, and can be used as wakeup source.
  *         However, when the HSE clock is used as RTC clock source, the RTC
  *         cannot be used in STOP and STANDBY modes.    
  * @note   The maximum input clock frequency for RTC is 1MHz (when using HSE as
  *         RTC clock source).
  *  
  * @retval None
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{

  /* Check the parameters */
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));


  RCC->BDCR &= ~RCC_BDCR_RTCSEL;
  
  /* Select the RTC clock source */
  RCC->BDCR |= RCC_RTCCLKSource;
  
}

/**
  * @brief  Enables or disables the RTC clock.
  * @note   This function must be used only after the RTC clock source was selected
  *         using the RCC_RTCCLKConfig function.
  * @param  NewState: new state of the RTC clock. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @brief  Forces or releases the Backup domain reset.
  * @note   This function resets the RTC peripheral (including the backup registers)
  *         and the RTC clock source selection in RCC_CSR register.
  * @note   The BKPSRAM is not affected by this reset.    
  * @param  NewState: new state of the Backup domain reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

/**
  * @brief  Configures the DFSDM clock source (DFSDMCLK).
  * @note   This function must be called before enabling the DFSDM APB clock.
  * @param  RCC_DFSDMCLKSource: specifies the DFSDM clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_DFSDMCLKSource_APB: APB clock used as DFSDM clock source.
  *            @arg RCC_DFSDMCLKSource_SYS: System clock used as DFSDM clock source.
  *                                        
  * @retval None
  */
void RCC_DFSDMCLKConfig(uint32_t RCC_DFSDMCLKSource)
{
  /* Check the parameters */
  assert_param(IS_RCC_DFSDMCLK_SOURCE(RCC_DFSDMCLKSource));
  *(__IO uint32_t *) CCIPR_DFSDMSRC_BB = RCC_DFSDMCLKSource;
}

/**
  * @brief  Configures the I2C clock (I2CCLK).
  * @param  RCC_I2CCLK: defines the I2C clock source. This clock is derived 
  *         from the HSI, PCLK or System clock.
  *          This parameter can be one of the following values:
  *             @arg RCC_I2CxCLK_PCLK: I2Cx clock = APB Clock (PCLK)  
  *             @arg RCC_I2CxCLK_HSI: I2Cx clock = HSI
  *             @arg RCC_I2CxCLK_SYSCLK: I2Cx clock = System Clock
  * @note  x can be 1, 2 or 3  
  * @retval None
  */
void RCC_I2CCLKConfig(uint32_t RCC_I2CCLK)
{ 
  uint32_t tmp = 0,tmp_CCIPR;
  
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Check the parameters */
  assert_param(IS_RCC_I2CCLK(RCC_I2CCLK));

  tmp = (RCC_I2CCLK >> 28);
  
  /* Check which I2C Instance is called */
  switch (tmp)
  {
    case 0x01:
      /* Clear I2C1_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_I2C1SEL;
      break;
    case 0x02:
      /* Clear I2C2_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_I2C2SEL;
      break;
    case 0x03:
      /* Clear I2C3_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_I2C3SEL;
      break;
  default:
    break;
  }
  /* Update New Value */
  tmp_CCIPR |= 0x0003F000 & RCC_I2CCLK;
  /* Set I2CxSEL bits according to RCC_I2CCLK value */
  RCC->CCIPR = tmp_CCIPR;
}

/**
  * @brief  Configures the USART clock (USARTCLK).
  * @param  RCC_USARTCLK: defines the USART clock source. This clock is derived 
  *         from the HSI, PCLK, LSE or System clock.
  *          This parameter can be one of the following values:
  *             @arg RCC_PPPCLK_PCLK: USART clock = APB Clock (PCLK)
  *             @arg RCC_PPPCLK_SYSCLK: USART clock = System Clock
  *             @arg RCC_PPPCLK_LSE: USART clock = LSE Clock
  *             @arg RCC_PPPCLK_HSI: USART clock = HSI Clock
  * @note  PPP can be USART1, USART2, USART3, UART4, UART5 or LPUART  
  * @retval None
  */
void RCC_USARTCLKConfig(uint32_t RCC_USARTCLK)
{ 
  uint32_t tmp = 0,tmp_CCIPR;
  
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Check the parameters */
  assert_param(IS_RCC_USARTCLK(RCC_USARTCLK));

  tmp = (RCC_USARTCLK >> 28);

  /* Check which USART instance is Called */
  switch (tmp)
  {
    case 0x01:
      /* Clear USART1_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_USART1SEL;
      break;
    case 0x02:
      /* Clear USART2_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_USART2SEL;
      break;
    case 0x03:
      /* Clear USART3_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_USART3SEL;
      break;
    case 0x04:
      /* Clear UART4_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_UART4SEL;
      break;
    case 0x05:
      /* Clear UART5_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_UART5SEL;
      break;
    case 0x06:
      /* Clear LPUART_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_LPUARTSEL;
      break;
  default:
    break;
  }
  /* Update New Value */
  tmp_CCIPR |= 0x00000FFF & RCC_USARTCLK;
  /* Set USARTxSEL bits according to RCC_USARTCLK value */
  RCC->CCIPR = tmp_CCIPR;
}

/**
  * @brief  Configures the ADC clock source.
  * @param  RCC_ADCCLKSource: defines the ADC clock source. This clock is derived 
  *         from the PLL_SAI1, PLL_SAI2 or System clock.
  *         This parameter can be one of the following values:
  *             @arg RCC_ADCCLKSource_PLLSAI1: ADC clock = PLL_SAI1  
  *             @arg RCC_ADCCLKSource_PLLSAI2: ADC clock = PLL_SAI1
  *             @arg RCC_ADCCLKSource_SYSCLK:  ADC clock = System Clock
  * @retval None
  */
void RCC_ADCCLKConfig(uint32_t RCC_ADCCLKSource)
{ 
  uint32_t tmp_CCIPR =0 ;
  
   /* Check the parameters */
  assert_param(IS_RCC_ADC_CLOCK(RCC_ADCCLKSource));
  
  /* Gets CCIPR register value */
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Clear ADC_SEL Bits */
  tmp_CCIPR &= ~RCC_CCIPR_ADCSEL;

  /* Update New Value */
  tmp_CCIPR |= RCC_ADCCLKSource;
  
  /* Set ADCSEL bits according to RCC_ADCCLKSource value */ 
  RCC->CCIPR = tmp_CCIPR;
}

/**
  * @brief  Configures the LPTIM clock source.
  * @param  RCC_LPTIMxCLKSource: defines the LPTIM clock source. This clock is
  *         derived from the APB, LSI, HSI or LSE.
  *         This parameter can be one of the following values:
  *             @arg RCC_LPTIMxCLKSource_APB: LPTIMx clock = APB Clock  
  *             @arg RCC_LPTIMxCLKSource_LSI: LPTIMx clock = LSI clock
  *             @arg RCC_LPTIMxCLKSource_HSI: LPTIMx clock = HSI Clock
  *             @arg RCC_LPTIMxCLKSource_LSE: LPTIMx clock = LSE Clock 
  * @note  x can be 1 or 2 to select the LPTIM peripheral.  
  * @retval None
  */
void RCC_LPTIMCLKConfig(uint32_t RCC_LPTIMxCLKSource)
{ 
  uint32_t tmp = 0, tmp_CCIPR;
  
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Check the parameters */
  assert_param(IS_RCC_LPTIM(RCC_LPTIMxCLKSource));

  tmp = (RCC_LPTIMxCLKSource >> 28);
  
  /* Check which LPTIM Instance is called */
  switch (tmp)
  {
    case 0x01:
      /* Clear LPTIM1_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_LPTIM1SEL;
      break;
    case 0x02:
      /* Clear LPTIM2_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_LPTIM2SEL;
      break;
    default:
      break;
  }
  /* Update New Value */
  tmp_CCIPR |= (0x003C0000 & RCC_LPTIMxCLKSource);
  
  /* Set LPTIMxSEL bits according to RCC_LPTIMxCLKSource value */
  RCC->CCIPR = tmp_CCIPR;
}

void RCC_SWPCLKConfig(uint32_t RCC_SWPCLKSource);

/**
  * @brief  Configures the SWP clock (SWPCLK).
  * @param  RCC_SWPCLK: defines the SWP clock source. This clock is derived 
  *         from the PCLK or System clock.
  *          This parameter can be one of the following values:
  *             @arg RCC_SWPCLK_APB: SWP clock = APB Clock (PCLK)  
  *             @arg RCC_SWPCLK_HSI: SWP clock = HSI
  * @retval None
  */
void RCC_SWPCLKConfig(uint32_t RCC_SWPCLKSource)
{ 

   /* Check the parameters */
  assert_param(IS_RCC_SWPCLK_SOURCE(RCC_SWPCLKSource));
  *(__IO uint32_t *) CCIPR_SWPSRC_BB = RCC_SWPCLKSource;
  
}


/**
  * @brief  Configures the SAI clock (SAICLKSource).
  * @param  RCC_SAICLKSource: defines the SAI clock source. This clock is derived 
  *         from the PLLSAIx, system PLL or external clock (through a dedicated PIN)
  *          This parameter can be one of the following values:
  *             @arg RCC_SAI1CLK_PLLSAI1: SAIx clock = PLLSAI1  
  *             @arg RCC_SAI1CLK_PLLSAI2: SAIx clock = PLLSAI2  
  *             @arg RCC_SAI1CLK_PLLSYS: SAIx clock  = PLLSYS  
  *             @arg RCC_SAI1CLK_PIN: SAIx clock = External Clock  
  *             @arg RCC_SAI2CLK_PLLSAI1: SAIx clock = PLLSAI1  
  *             @arg RCC_SAI2CLK_PLLSAI2: SAIx clock = PLLSAI2  
  *             @arg RCC_SAI2CLK_PLLSYS: SAIx clock  = PLLSYS  
  *             @arg RCC_SAI2CLK_PIN: SAIx clock = External Clock
  * @note  x can be 1or 2   
  * @retval None
  */
void RCC_SAICLKConfig(uint32_t RCC_SAICLKSource)
{ 
  uint32_t tmp = 0,tmp_CCIPR;
  
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Check the parameters */
  assert_param(IS_RCC_SAICLK(RCC_SAICLKSource));

  tmp = (RCC_SAICLKSource >> 28);
  
  /* Check which SAI Instance is called */
  switch (tmp)
  {
    case 0x01:
      /* Clear SAI1_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_SAI1SEL;
      break;
    case 0x02:
      /* Clear SAI2_SEL Bits */
      tmp_CCIPR &= ~RCC_CCIPR_SAI2SEL;
      break;
  
  default:
    break;
  }
  /* Update New Value */
  tmp_CCIPR |= 0x03C00000 & RCC_SAICLKSource;
  /* Set SAIxSEL bits according to RCC_SAICLKSource value */
  RCC->CCIPR = tmp_CCIPR;
}


/**
  * @brief  48 MHz clock source selection
  * @param  RCC_48CLKSource: defines the 48MHz clock source. This clock is derived 
  *         from the PLL_SAI1, PLL_SAI2 or System clock.
  *         This parameter can be one of the following values:
  *             @arg RCC_CLK48Source_PLLSAI1: CLK48 clock = PLL_SAI1 Clock 
  *             @arg RCC_CLK48Source_PLLSYS: CLK48 clock = PLL_SYS Clock
  *             @arg RCC_CLK48Source_MSICLK: CLK48 clock = MSI Clock
  * @retval None
  */
void RCC_CLK48Config(uint32_t RCC_CLK48Source)
{ 
  
  uint32_t tmp_CCIPR =0 ;
  
  /* Check the parameters */
  assert_param(IS_RCC_CLK48(RCC_CLK48Source));
  
  /* Gets CCIPR register value */
  tmp_CCIPR = RCC->CCIPR ;
  
  /* Clear CLK48_SEL Bits */
  tmp_CCIPR &= ~RCC_CCIPR_CLK48SEL;

  /* Update New Value */
  tmp_CCIPR |= RCC_CLK48Source;
  
  /* Set CLK48SEL bits according to RCC_CLK48Source value */ 
  RCC->CCIPR = tmp_CCIPR;
}


/**
  * @brief  Enables or disables the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.   
  * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_AHB1Periph_DMA1         DMA1 clock    
  *            @arg RCC_AHB1Periph_DMA2         DMA2 clock   
  *            @arg RCC_AHB1Periph_TSC          TSC clock   
  *            @arg RCC_AHB1Periph_FLASH        Flash clock 
  *            @arg RCC_AHB1Periph_CRC          CRC clock    
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB1ENR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
  }
}

/**
  * @brief  Enables or disables the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_AHB2Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB2Periph_GPIOB:       GPIOB clock 
  *            @arg RCC_AHB2Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB2Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB2Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB2Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB2Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB2Periph_GPIOH:       GPIOH clock
  *            @arg RCC_AHB2Periph_OTGFS:       USB OTG Full Speed clock
  *            @arg RCC_AHB2Periph_ADC:         ADCs clock
  *            @arg RCC_AHB2Periph_AES:         AES clock
  *            @arg RCC_AHB2Periph_RNG:         RNG clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB2_PERIPH(RCC_AHB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB2ENR |= RCC_AHB2Periph;
  }
  else
  {
    RCC->AHB2ENR &= ~RCC_AHB2Periph;
  }
}

/**
  * @brief  Enables or disables the AHB3 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to gates its clock.
  *          This parameter must be: RCC_AHB3Periph_FMC or RCC_AHB3Periph_QSPI 
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB3ENR |= RCC_AHB3Periph;
  }
  else
  {
    RCC->AHB3ENR &= ~RCC_AHB3Periph;
  }
}

/**
  * @brief  Enables or disables the Low Speed APB (APB1L) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @param  RCC_APB1LPeriph: specifies the APB1L peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1LPeriph_TIM2:   TIM2 clock
  *            @arg RCC_APB1LPeriph_TIM3:   TIM3 clock
  *            @arg RCC_APB1LPeriph_TIM4:   TIM4 clock
  *            @arg RCC_APB1LPeriph_TIM5:   TIM5 clock
  *            @arg RCC_APB1LPeriph_TIM6:   TIM6 clock
  *            @arg RCC_APB1LPeriph_TIM7:   TIM7 clock
  *            @arg RCC_APB1LPeriph_LCD:    LCD clock
  *            @arg RCC_APB1LPeriph_WWDG:   WWDG clock
  *            @arg RCC_APB1LPeriph_SPI2:   SPI2 clock
  *            @arg RCC_APB1LPeriph_SPI3:   SPI3 clock
  *            @arg RCC_APB1LPeriph_USART2: USART2 clock
  *            @arg RCC_APB1LPeriph_USART3: USART3 clock
  *            @arg RCC_APB1LPeriph_UART4:  UART4 clock
  *            @arg RCC_APB1LPeriph_UART5:  UART5 clock
  *            @arg RCC_APB1LPeriph_I2C1:   I2C1 clock
  *            @arg RCC_APB1LPeriph_I2C2:   I2C2 clock
  *            @arg RCC_APB1LPeriph_I2C3:   I2C3 clock
  *            @arg RCC_APB1LPeriph_CAN:    CAN clock
  *            @arg RCC_APB1LPeriph_PWR:    PWR clock
  *            @arg RCC_APB1LPeriph_DAC:    DAC clock
  *            @arg RCC_APB1LPeriph_OPAMP:  OPAMP clock
  *            @arg RCC_APB1LPeriph_LPTIM1: LPTIM1 clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1LPeriphClockCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1L_PERIPH(RCC_APB1LPeriph));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB1LENR |= RCC_APB1LPeriph;
  }
  else
  {
    RCC->APB1LENR &= ~RCC_APB1LPeriph;
  }
}

/**
  * @brief  Enables or disables the Low Speed APB (APB1H) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @param  RCC_APB1HPeriph: specifies the APB1H peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1HPeriph_LPUART: LPUART clock
  *            @arg RCC_APB1HPeriph_SWP:    SWP clock
  *            @arg RCC_APB1HPeriph_LPTIM2: LPTIM2 clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1HPeriphClockCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1H_PERIPH(RCC_APB1HPeriph));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB1HENR |= RCC_APB1HPeriph;
  }
  else
  {
    RCC->APB1HENR &= ~RCC_APB1HPeriph;
  }
}

/**
  * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_SYSCFG:    SYSCFG clock
  *            @arg RCC_APB2Periph_COMP:      COMP clock
  *            @arg RCC_APB2Periph_FIREWALL:  FIREWALL clock
  *            @arg RCC_APB2Periph_SDIO:      SDIO clock
  *            @arg RCC_APB2Periph_TIM1:      TIM1 clock
  *            @arg RCC_APB2Periph_SPI1:      SPI1 clock
  *            @arg RCC_APB2Periph_TIM8:      TIM8 clock
  *            @arg RCC_APB2Periph_USART1:    USART1clock
  *            @arg RCC_APB2Periph_TIM15:     TIM15 clock
  *            @arg RCC_APB2Periph_TIM16:     TIM16 clock
  *            @arg RCC_APB2Periph_TIM17:     TIM17 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI1 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI2 clock
  *            @arg RCC_APB2Periph_DFSDM:     DFSDM clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  Forces or releases AHB1 peripheral reset.
  * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to reset.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_AHB1Periph_DMA1         DMA1 clock    
  *            @arg RCC_AHB1Periph_DMA2         DMA2 clock   
  *            @arg RCC_AHB1Periph_TSC          TSC clock   
  *            @arg RCC_AHB1Periph_FLASH        Flash clock 
  *            @arg RCC_AHB1Periph_CRC          CRC clock    
  *                  
  * @param  NewState: new state of the specified peripheral reset.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB1RSTR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1RSTR &= ~RCC_AHB1Periph;
  }
}

/**
  * @brief  Forces or releases AHB2 peripheral reset.
  * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to reset.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_AHB2Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB2Periph_GPIOB:       GPIOB clock 
  *            @arg RCC_AHB2Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB2Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB2Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB2Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB2Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB2Periph_GPIOH:       GPIOH clock
  *            @arg RCC_AHB2Periph_OTGFS:       USB OTG Full Speed clock
  *            @arg RCC_AHB2Periph_ADC:         ADCs clock
  *            @arg RCC_AHB2Periph_AES:         AES clock
  *            @arg RCC_AHB2Periph_RNG:         RNG clock
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB2_PERIPH(RCC_AHB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB2RSTR |= RCC_AHB2Periph;
  }
  else
  {
    RCC->AHB2RSTR &= ~RCC_AHB2Periph;
  }
}

/**
  * @brief  Forces or releases AHB3 peripheral reset.
  * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to reset.
  *          This parameter must be: RCC_AHB3Periph_FMC or RCC_AHB3Periph_QSPI  
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB3RSTR |= RCC_AHB3Periph;
  }
  else
  {
    RCC->AHB3RSTR &= ~RCC_AHB3Periph;
  }
}

/**
  * @brief  Forces or releases Low Speed APB (APB1L) peripheral reset.
  * @param  RCC_APB1LPeriph: specifies the APB1 peripheral to reset.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1LPeriph_TIM2:   TIM2 clock
  *            @arg RCC_APB1LPeriph_TIM3:   TIM3 clock
  *            @arg RCC_APB1LPeriph_TIM4:   TIM4 clock
  *            @arg RCC_APB1LPeriph_TIM5:   TIM5 clock
  *            @arg RCC_APB1LPeriph_TIM6:   TIM6 clock
  *            @arg RCC_APB1LPeriph_TIM7:   TIM7 clock
  *            @arg RCC_APB1LPeriph_LCD:    LCD clock
  *            @arg RCC_APB1LPeriph_SPI2:   SPI2 clock
  *            @arg RCC_APB1LPeriph_SPI3:   SPI3 clock
  *            @arg RCC_APB1LPeriph_USART2: USART2 clock
  *            @arg RCC_APB1LPeriph_USART3: USART3 clock
  *            @arg RCC_APB1LPeriph_UART4:  UART4 clock
  *            @arg RCC_APB1LPeriph_UART5:  UART5 clock
  *            @arg RCC_APB1LPeriph_I2C1:   I2C1 clock
  *            @arg RCC_APB1LPeriph_I2C2:   I2C2 clock
  *            @arg RCC_APB1LPeriph_I2C3:   I2C3 clock
  *            @arg RCC_APB1LPeriph_CAN:    CAN clock
  *            @arg RCC_APB1LPeriph_PWR:    PWR clock
  *            @arg RCC_APB1LPeriph_DAC:    DAC clock
  *            @arg RCC_APB1LPeriph_OPAMP:  OPAMP clock
  *            @arg RCC_APB1LPeriph_LPTIM1: LPTIM1 clock  
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1LPeriphResetCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1L_PERIPH(RCC_APB1LPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1LRSTR |= RCC_APB1LPeriph;
  }
  else
  {
    RCC->APB1LRSTR &= ~RCC_APB1LPeriph;
  }
}

/**
  * @brief  Forces or releases Low Speed APB (APB1H) peripheral reset.
  * @param  RCC_APB1HPeriph: specifies the APB1 peripheral to reset.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1HPeriph_LPUART: LPUART clock
  *            @arg RCC_APB1HPeriph_SWP:    SWP clock
  *            @arg RCC_APB1HPeriph_LPTIM2: LPTIM2 clock  
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1HPeriphResetCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1H_PERIPH(RCC_APB1HPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1HRSTR |= RCC_APB1HPeriph;
  }
  else
  {
    RCC->APB1HRSTR &= ~RCC_APB1HPeriph;
  }
}
/**
  * @brief  Forces or releases High Speed APB (APB2) peripheral reset.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_SYSCFG:    SYSCFG clock
  *            @arg RCC_APB2Periph_COMP:      COMP clock
  *            @arg RCC_APB2Periph_SDIO:      SDIO clock
  *            @arg RCC_APB2Periph_TIM1:      TIM1 clock
  *            @arg RCC_APB2Periph_SPI1:      SPI1 clock
  *            @arg RCC_APB2Periph_TIM8:      TIM8 clock
  *            @arg RCC_APB2Periph_USART1:    USART1clock
  *            @arg RCC_APB2Periph_TIM15:     TIM15 clock
  *            @arg RCC_APB2Periph_TIM16:     TIM16 clock
  *            @arg RCC_APB2Periph_TIM17:     TIM17 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI1 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI2 clock
  *            @arg RCC_APB2Periph_DFSDM:     DFSDM clock 
  * @param  NewState: new state of the specified peripheral reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_RESET_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  Enables or disables the AHB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_AHB1Periph: specifies the AHB1 peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *            @arg RCC_AHB1Periph_DMA1         DMA1 clock    
  *            @arg RCC_AHB1Periph_DMA2         DMA2 clock   
  *            @arg RCC_AHB1Periph_TSC          TSC clock   
  *            @arg RCC_AHB1Periph_FLASH        Flash clock
  *            @arg RCC_AHB1Periph_SRAM1        SRAM1 clock
  *            @arg RCC_AHB1Periph_CRC          CRC clock  
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB1_LPMODE_PERIPH(RCC_AHB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB1LPENR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1LPENR &= ~RCC_AHB1Periph;
  }
}

/**
  * @brief  Enables or disables the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *           power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_AHB2Periph: specifies the AHB2 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_AHB2Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB2Periph_GPIOB:       GPIOB clock 
  *            @arg RCC_AHB2Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB2Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB2Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB2Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB2Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB2Periph_GPIOH:       GPIOH clock
  *            @arg RCC_AHB2Periph_SRAM2:       SRAM2 clock
  *            @arg RCC_AHB2Periph_OTGFS:       USB OTG Full Speed clock
  *            @arg RCC_AHB2Periph_ADC:         ADCs clock
  *            @arg RCC_AHB2Periph_AES:         AES clock
  *            @arg RCC_AHB2Periph_RNG:         RNG clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB2_LPMODE_PERIPH(RCC_AHB2Periph)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB2LPENR |= RCC_AHB2Periph;
  }
  else
  {
    RCC->AHB2LPENR &= ~RCC_AHB2Periph;
  }
}

/**
  * @brief  Enables or disables the AHB3 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_AHB3Periph: specifies the AHB3 peripheral to gates its clock.
  *          This parameter must be: RCC_AHB3Periph_FSMC or RCC_AHB3Periph_QSPI
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB3LPENR |= RCC_AHB3Periph;
  }
  else
  {
    RCC->AHB3LPENR &= ~RCC_AHB3Periph;
  }
}

/**
  * @brief  Enables or disables the APB1L peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_APB1LPeriph: specifies the APB1 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1LPeriph_TIM2:   TIM3 clock  
  *            @arg RCC_APB1LPeriph_TIM3:   TIM3 clock
  *            @arg RCC_APB1LPeriph_TIM4:   TIM4 clock
  *            @arg RCC_APB1LPeriph_TIM5:   TIM5 clock
  *            @arg RCC_APB1LPeriph_TIM6:   TIM6 clock
  *            @arg RCC_APB1LPeriph_TIM7:   TIM7 clock
  *            @arg RCC_APB1LPeriph_LCD:    LCD clock
  *            @arg RCC_APB1LPeriph_WWDG:   WWDG clock
  *            @arg RCC_APB1LPeriph_SPI2:   SPI2 clock
  *            @arg RCC_APB1LPeriph_SPI3:   SPI3 clock
  *            @arg RCC_APB1LPeriph_USART2: USART2 clock
  *            @arg RCC_APB1LPeriph_USART3: USART3 clock
  *            @arg RCC_APB1LPeriph_UART4:  UART4 clock
  *            @arg RCC_APB1LPeriph_UART5:  UART5 clock
  *            @arg RCC_APB1LPeriph_I2C1:   I2C1 clock
  *            @arg RCC_APB1LPeriph_I2C2:   I2C2 clock
  *            @arg RCC_APB1LPeriph_I2C3:   I2C3 clock
  *            @arg RCC_APB1LPeriph_CAN:    CAN clock
  *            @arg RCC_APB1LPeriph_LPTIM1: LPTIM1 clock
  *            @arg RCC_APB1LPeriph_PWR:    PWR clock
  *            @arg RCC_APB1LPeriph_DAC:    DAC clock
  *            @arg RCC_APB1LPeriph_OPAMP:  OPAMP clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1LPeriphClockLPModeCmd(uint32_t RCC_APB1LPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1L_LP_PERIPH(RCC_APB1LPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1LLPENR |= RCC_APB1LPeriph;
  }
  else
  {
    RCC->APB1LLPENR &= ~RCC_APB1LPeriph;
  }
}

/**
  * @brief  Enables or disables the APB1H peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_APB1HPeriph: specifies the APB1 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1HPeriph_LPUART: LPUART clock
  *            @arg RCC_APB1HPeriph_SWP:    SWP clock
  *            @arg RCC_APB1HPeriph_LPTIM2: LPTIM2 clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1HPeriphClockLPModeCmd(uint32_t RCC_APB1HPeriph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB1H_PERIPH(RCC_APB1HPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1HLPENR |= RCC_APB1HPeriph;
  }
  else
  {
    RCC->APB1HLPENR &= ~RCC_APB1HPeriph;
  }
}
/**
  * @brief  Enables or disables the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB2Periph_SYSCFG:    SYSCFG clock
  *            @arg RCC_APB2Periph_COMP:      COMP clock
  *            @arg RCC_APB2Periph_FIREWALL:  FIREWALL clock
  *            @arg RCC_APB2Periph_SDIO:      SDIO clock
  *            @arg RCC_APB2Periph_TIM1:      TIM1 clock
  *            @arg RCC_APB2Periph_SPI1:      SPI1 clock
  *            @arg RCC_APB2Periph_TIM8:      TIM8 clock
  *            @arg RCC_APB2Periph_USART1:    USART1clock
  *            @arg RCC_APB2Periph_TIM15:     TIM15 clock
  *            @arg RCC_APB2Periph_TIM16:     TIM16 clock
  *            @arg RCC_APB2Periph_TIM17:     TIM17 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI1 clock
  *            @arg RCC_APB2Periph_SAI1:      SAI2 clock
  *            @arg RCC_APB2Periph_DFSDM:     DFSDM clock    
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2LPENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2LPENR &= ~RCC_APB2Periph;
  }
}

/**
  * @}
  */

/** @defgroup RCC_Group4 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions 
 *
@verbatim   
 ===============================================================================
                ##### Interrupts and flags management functions #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified RCC interrupts.
  * @param  RCC_IT: specifies the RCC interrupt sources to be enabled or disabled.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_MSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLSAI1RDY: PLLSAI1 ready interrupt
  *            @arg RCC_IT_PLLSAI2RDY: PLLSAI2 ready interrupt
  *            @arg RCC_IT_LSECSS: Clock security system interrupt 
  * @param  NewState: new state of the specified RCC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_ITConfig(uint32_t RCC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
   
   RCC->CIER |= RCC_IT;
  }
  else
  {
    RCC->CIER &= ~RCC_IT;
  }
}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  RCC_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg RCC_FLAG_MSIRDY: MSI oscillator clock ready
  *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *            @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *            @arg RCC_FLAG_PLLRDY: main PLL clock ready
  *            @arg RCC_FLAG_PLLSAI2RDY: PLLSAI2 clock ready 
  *            @arg RCC_FLAG_PLLSAI1RDY: PLLSAI1 clock ready 
  *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *            @arg RCC_FLAG_PORRST: POR/PDR or BOR reset
  *            @arg RCC_FLAG_OBLRST: OBLRST reset
  *            @arg RCC_FLAG_PINRST: Pin reset
  *            @arg RCC_FLAG_FIREWALLRST: FIREWALL reset
  *            @arg RCC_FLAG_RMVF: Remove  reset Flag
  *            @arg RCC_FLAG_SFTRST: Software reset
  *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *            @arg RCC_FLAG_LPWRRST: Low Power reset
  * @retval The new state of RCC_FLAG (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* Get the RCC register index */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* The flag to check is in CR register */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* The flag to check is in BDCR register */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* The flag to check is in CSR register */
  {
    statusreg = RCC->CSR;
  }

  /* Get the flag position */
  tmp = RCC_FLAG & RCC_FLAG_MASK;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief  Clears the RCC reset flags.
  *         The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_BORRST,  RCC_FLAG_SFTRST,
  *         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST,RCC_FLAG_OBLRST,
  *         RCC_FLAG_FIREWALLRST.
  * @param  None
  * @retval None
  */
void RCC_ClearFlag(void)
{
  /* Set RMVF bit to clear the reset flags */
  RCC->CSR |= RCC_CSR_RMVF;
}

/**
  * @brief  Checks whether the specified RCC interrupt has occurred or not.
  * @param  RCC_IT: specifies the RCC interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_MSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLSAI1RDY: PLLSAI1 ready interrupt
  *            @arg RCC_IT_PLLSAI2RDY: PLLSAI2 ready interrupt
  *            @arg RCC_IT_HSECSS: HSE Clock Security interrupt
  *            @arg RCC_IT_LSECSS: Clock security system interrupt 
  * @retval The new state of RCC_IT (SET or RESET).
  */
ITStatus RCC_GetITStatus(uint32_t RCC_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* Check the status of the specified RCC interrupt */
  if ((RCC->CIFR & RCC_IT) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the RCC_IT status */
  return  bitstatus;
}

/**
  * @brief  Clears the RCC's interrupt pending bits.
  * @param  RCC_IT: specifies the interrupt pending bit to clear.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_MSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLSAI1RDY: PLLSAI1 ready interrupt
  *            @arg RCC_IT_PLLSAI2RDY: PLLSAI2 ready interrupt
  *            @arg RCC_IT_HSECSS: HSE Clock Security interrupt
  *            @arg RCC_IT_LSECSS: Clock security system interrupt 
  * @retval None
  */
void RCC_ClearITPendingBit(uint32_t RCC_IT)
{
  /* Check the parameters */
   assert_param(IS_RCC_CLEAR_IT(RCC_IT));

    RCC->CICR = RCC_IT;
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

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
