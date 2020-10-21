/**
  ******************************************************************************
  * @file    system_stm32l4xx.c
  * @author  MCD Application Team
  * @version VX.Y.Z
  * @date    DD-Month-YYYY
  * @brief   This file contains the system clock configuration for STM32L4xx devices.
  *
  *   ==========================================================================
  *          This file is intended for use on Orca FPGA platform. 
  *   ==========================================================================
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),
  *                      depending on the configuration made in the clock xls tool. 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the MSI (4 MHz) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32l4xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HSI still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.
  *
  * 4. The default value of HSE crystal is set to 8MHz, refer to "HSE_VALUE" define
  *    in "stm32l4xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  *
   *   ==========================================================================  
  *    On FPGA platform the HSE value is configurable from 1 to 60 MHz, its value 
  *    is "redefined" in stm32f4xx.h file as below:         
  *    #if defined  (HSE_VALUE)
  *      // Set the right HSE value as configured in the FPGA platform
  *      #undef HSE_VALUE
  *      #define HSE_VALUE    ((uint32_t)60000000) 
  *    #endif // HSE_VALUE
  *   ==========================================================================
  ****************************************************************************** 
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l4xx_system
  * @{
  */  
  
/** @addtogroup STM32L4xx_System_Private_Includes
  * @{
  */

#include "stm32l4xx.h"

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Defines
  * @{
  */
 
/* Select the System clock source then adapt accordingly the clock parameters */
//#define SYSCLK_SOURCE_HSE    /* System clock Frequency 60 Mhz */
//#define SYSCLK_SOURCE_PLL  /* System clock Frequency 80 Mhz;Normally fixed in FPGA : Check with Netlist generator*/*/
#define SYSCLK_SOURCE_HSI  /* System clock Frequency 16 Mhz */
//#define SYSCLK_SOURCE_MSI  /* System clock Frequency 48 Mhz ;Normally fixed in FPGA : Check with Netlist generator*/ 

/************************* HSI used System clock source ***********************/
#ifdef SYSCLK_SOURCE_HSI
 #define HCLK_PRESCALER_VALUE     RCC_CFGR_HPRE_DIV1
 #define PLCK2_PRESCALER_VALUE    RCC_CFGR_PPRE2_DIV1
 #define PLCK1_PRESCALER_VALUE    RCC_CFGR_PPRE1_DIV2
#define FLASH_ACR_VALUE          (FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_4WS)

 /* Note: set the right HSI_VALUE in stm32f4xx.h file */
#endif /* SYSCLK_SOURCE_HSI */ 
/******************************************************************************/
/************************* HSE used System clock source ***********************/
#ifdef SYSCLK_SOURCE_HSE
 #define HCLK_PRESCALER_VALUE     RCC_CFGR_HPRE_DIV1
 #define PLCK2_PRESCALER_VALUE    RCC_CFGR_PPRE2_DIV1
 #define PLCK1_PRESCALER_VALUE    RCC_CFGR_PPRE1_DIV2
#define FLASH_ACR_VALUE          (FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_4WS)

 /* Note: set the right HSE_VALUE in stm32f4xx.h file */
#endif /* SYSCLK_SOURCE_HSE */ 
/******************************************************************************/
/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM mounted */
/* #define DATA_IN_ExtSRAM */ 
/* #define DATA_IN_ExtSDRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
/******************************************************************************/

/************************* PLL Parameters *************************************/
#ifdef SYSCLK_SOURCE_PLL
/* PLL_VCO = (HSE_VALUE or MSI_VALUE or HSI_VALUE / PLL_M) * PLL_N */

#define PLL_M       8

#define PLL_N      20

/* SYSCLK = PLL_VCO / PLL_R */
#define PLL_R      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      4

/* SAI clock =  PLL_VCO / PLLP */
#define PLL_P      7
 #define HCLK_PRESCALER_VALUE     RCC_CFGR_HPRE_DIV1
 #define PLCK2_PRESCALER_VALUE    RCC_CFGR_PPRE2_DIV1
 #define PLCK1_PRESCALER_VALUE    RCC_CFGR_PPRE1_DIV2
 #define FLASH_ACR_VALUE          (FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN |FLASH_ACR_LATENCY_4WS)
#endif /* SYSCLK_SOURCE_PLL */ 
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Variables
  * @{
  */

#if defined(SYSCLK_SOURCE_HSE)
  uint32_t SystemCoreClock = 60000000;
#elif defined (SYSCLK_SOURCE_PLL) 
  uint32_t SystemCoreClock = 80000000;

#elif defined (SYSCLK_SOURCE_MSI) 
  uint32_t SystemCoreClock = 48000000;
#else
  uint32_t SystemCoreClock = 16000000;
#endif
  

  __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);
#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
  * @param  None
  * @retval None
  */

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;
  
  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC->CR &= (uint32_t)0xEAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000800;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */
         
  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings ----------------------------------*/
  SetSysClock();

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI_VALUE(*)
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(***) 
  *             or HSI_VALUE(*) or MSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32l4xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32l4xx.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *     
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, MSI_RANGE =0, pllvco = 0, pllr = 2, pllsource = 0, pllm = 2;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  /* Get MSI Range -----------------------------------------------------------*/
   MSI_RANGE = (1 << (((RCC->CSR & RCC_CSR_MSIRANGE)>>8 )-4 ))* 1000000   ;
   
   switch (tmp)
  {
    case 0x00:  /* MSI used as system clock source */
      SystemCoreClock = MSI_RANGE ;
      
      break;
    case 0x04:  /* HSI used as system clock  source */
      SystemCoreClock = HSI_VALUE;
      break;
      
    case 0x08:  /* HSE used as system clock  source */
     SystemCoreClock = HSE_VALUE;
      break;
      
    case 0x0C:  /* PLL used as system clock  source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLR
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
      pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM)>> 4) + 1 ;

      
       switch (pllsource)
  {
    case 0x00:  /* MSI used as PLL clock source */
      pllvco = (MSI_RANGE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
    break;
    
     case 0x01:  /* HSI used as PLL clock source */
      pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
    break;
    
     case 0x02:  /* HSE used as PLL clock source */
     pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
    break;
    
    default:
     pllvco = (MSI_RANGE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
      break;
  }
      pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >>25) + 1 ) *2;
     SystemCoreClock = pllvco/pllr;
      break;
    default:
     SystemCoreClock = (1 << ((RCC->CSR & RCC_CSR_MSIRANGE)-4 ))* 1000000   ;
      break;
  }
  
  
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
  
  

}

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors, 
  *         AHB/APBx prescalers and Flash settings
  * @Note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).   
  * @param  None
  * @retval None
  */

static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSIStatus = 0, HSEStatus = 0;
#ifndef SYSCLK_SOURCE_MSI  
#ifndef SYSCLK_SOURCE_HSI

  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
     /* Select regulator voltage output Scale 1 mode, System frequency up to 80 MHz */
    RCC->APB1LENR |= RCC_APB1LENR_PWREN;
    PWR->CR1 |= PWR_CR1_VOS_0;

    /* Set HCLK prescaler */
    RCC->CFGR |= HCLK_PRESCALER_VALUE;    

    /* Set PCLK2 prescaler */    
    RCC->CFGR |= PLCK2_PRESCALER_VALUE;

    /* Set PCLK1 prescaler */
    RCC->CFGR |= PLCK1_PRESCALER_VALUE;

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_VALUE;
    
#if defined (SYSCLK_SOURCE_HSE)   
    /* Select the HSE as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_HSE;

    /* Wait till the HSE is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_HSE);
    {
    }
#elif defined (SYSCLK_SOURCE_PLL)
    /* Configure the main PLL => NOT NEEDED on FPGA since the PLL configuration
       is fixed and can't be changed */
    /* Configure the main PLL */

//    RCC->PLLCFGR = ((PLL_M-1) << 4) | (PLL_N << 8) | ((PLL_P >> 4 ) << 17) | (RCC_PLLCFGR_PLLSRC_HSE) |
//                 (((PLL_Q /2)-1) << 21) | (((PLL_R/2)-1) << 25) ;
    
    /* Enable the main PLL */
     RCC->CR |= RCC_CR_PLLSYSON;
     
    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLSYSRDY) == 0)
    {
    }
   
    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
    
 /********************/
//      /* unlcok PLL by restting the PLLON bit */
//  RCC->CR &= ~RCC_CR_PLLON;
//  /* Check that the PLL is unlcoked = PLLRDY flag is reset */
//  while(RCC->CR & RCC_CR_PLLRDY);
//  
//  /* Configure the PLL system(PLLSYS) */
//  RCC_PLLSYSConfig(8,0,0,PLL_SYS[1],ENABLE);
//   
//  /* Enable the pll clock */
//  RCC_PLLSYSState(DISABLE,DISABLE,ENABLE);
//  /* Enable the PLL */
//  RCC->CR |= RCC_CR_PLLON;
//  /* Check that the PLLRDY flag is set */
//  while(!(RCC->CR & RCC_CR_PLLRDY));
//  
//  //  RCC_MCO1Config(RCC_MCOSource_PLL, RCC_MCODiv_1);
//  /* Select the PLL clock to switch to */
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);


  /*******************************/
  
#endif 
    
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
  
  #endif 
  
  #if defined (SYSCLK_SOURCE_HSI)  
  /* Enable HSI */
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
 
  /* Wait till HSI is ready and if Time out is reached exit */
  do
  {
    HSIStatus = RCC->CR & RCC_CR_HSIRDY;
    StartUpCounter++;
  } while((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
  {
    HSIStatus = (uint32_t)0x01;
  }
  else
  {
    HSIStatus = (uint32_t)0x00;
  }

  if (HSIStatus == (uint32_t)0x01)
  {
     /* Select regulator voltage output Scale 1 mode, System frequency up to 80 MHz */
    RCC->APB1LENR |= RCC_APB1LENR_PWREN;
    PWR->CR1 |= PWR_CR1_VOS_0;

    /* Set HCLK prescaler */
    RCC->CFGR |= HCLK_PRESCALER_VALUE;    

    /* Set PCLK2 prescaler */    
    RCC->CFGR |= PLCK2_PRESCALER_VALUE;

    /* Set PCLK1 prescaler */
    RCC->CFGR |= PLCK1_PRESCALER_VALUE;

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_VALUE;
    
    /* Select the HSI as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    /* Wait till the HSI is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_HSI);
    {
    } 
  }
  else
  { /* If HSIfails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
    #endif 
  
#endif /* SYSCLK_SOURCE_MSI */
}


/**
  * @brief  Setup the external memory controller. Called in startup_stm32f4xx.s 
  *          before jump to __main
  * @param  None
  * @retval None
  */ 
#ifdef DATA_IN_ExtSRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external SRAM mounted on STM324xG_EVAL/STM324x7I boards
  *         This SRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+--------------+
 +                       SRAM pins assignment                               +
 +-------------------+--------------------+------------------+--------------+
 | PD0  <-> FMC_D2  | PE0  <-> FMC_NBL0 | PF0  <-> FMC_A0 | PG0 <-> FMC_A10 | 
 | PD1  <-> FMC_D3  | PE1  <-> FMC_NBL1 | PF1  <-> FMC_A1 | PG1 <-> FMC_A11 | 
 | PD4  <-> FMC_NOE | PE3  <-> FMC_A19  | PF2  <-> FMC_A2 | PG2 <-> FMC_A12 | 
 | PD5  <-> FMC_NWE | PE4  <-> FMC_A20  | PF3  <-> FMC_A3 | PG3 <-> FMC_A13 | 
 | PD8  <-> FMC_D13 | PE7  <-> FMC_D4   | PF4  <-> FMC_A4 | PG4 <-> FMC_A14 | 
 | PD9  <-> FMC_D14 | PE8  <-> FMC_D5   | PF5  <-> FMC_A5 | PG5 <-> FMC_A15 | 
 | PD10 <-> FMC_D15 | PE9  <-> FMC_D6   | PF12 <-> FMC_A6 | PG9 <-> FMC_NE2 | 
 | PD11 <-> FMC_A16 | PE10 <-> FMC_D7   | PF13 <-> FMC_A7 |-----------------+
 | PD12 <-> FMC_A17 | PE11 <-> FMC_D8   | PF14 <-> FMC_A8 | 
 | PD13 <-> FMC_A18 | PE12 <-> FMC_D9   | PF15 <-> FMC_A9 | 
 | PD14 <-> FMC_D0  | PE13 <-> FMC_D10  |-----------------+
 | PD15 <-> FMC_D1  | PE14 <-> FMC_D11  |
 |                  | PE15 <-> FMC_D12  |
 +------------------+------------------+
*/
   /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
  RCC->AHB1ENR   = 0x00000078;
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x00CCC0CC;
  GPIOD->AFR[1]  = 0xCCCCCCCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAA0A8A;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0FCF;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA828A;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCC0000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA000AAA;
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFF000FFF;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;
  GPIOG->AFR[1]  = 0x000000C0;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0x00080AAA;
  /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0x000C0FFF;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;
  
/*-- FMC Configuration ------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR         = 0x00000001;

  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001011;
  FMC_Bank1->BTCR[3]  = 0x00110212;
  FMC_Bank1E->BWTR[2] = 0x0FFFFFFF;
/*
  Bank1_SRAM2 is configured as follow:

  NORSRAMTimingStructure.FMC_AddressSetupTime = 2;
  NORSRAMTimingStructure.FMC_AddressHoldTime = 1;
  NORSRAMTimingStructure.FMC_DataSetupTime = 2;
  NORSRAMTimingStructure.FMC_BusTurnAroundDuration = 1;
  NORSRAMTimingStructure.FMC_CLKDivision = 2;
  NORSRAMTimingStructure.FMC_DataLatency = 2;
  NORSRAMTimingStructure.FMC_AccessMode = FMC_AccessMode_A;

  FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM2;
  FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
  FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_PSRAM;
  FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_MemoryDataWidth_16b;
  FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
  FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;  
  FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
  FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
  FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
  FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
  FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
  FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
  FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
  FMC_NORSRAMInitStructure.FMC_ContinousClock = FMC_CClock_SyncOnly;
  FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &NORSRAMTimingStructure;
  FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &NORSRAMTimingStructure;
*/
  
}
#endif /* DATA_IN_ExtSRAM */
  
#ifdef DATA_IN_ExtSDRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external SDRAM mounted on STM324x9I_EVAL board
  *         This SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  register uint32_t tmpreg = 0;
  register uint32_t index;

  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->AHB1ENR |= 0x000001FC;
  
  /* Connect PCx pins to FMC Alternate function */
  GPIOC->AFR[0]  = 0x0000000c;
  GPIOC->AFR[1]  = 0x00007700;
  /* Configure PCx pins in Alternate function mode */  
  GPIOC->MODER   = 0x00a00002;
  /* Configure PCx pins speed to 50 MHz */  
  GPIOC->OSPEEDR = 0x00a00002;
  /* Configure PCx pins Output type to push-pull */  
  GPIOC->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PCx pins */ 
  GPIOC->PUPDR   = 0x00500000;
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x000000CC;
  GPIOD->AFR[1]  = 0xCC000CCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xA02A000A;
  /* Configure PDx pins speed to 50 MHz */  
  GPIOD->OSPEEDR = 0xA02A000A;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00000CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA800A;
  /* Configure PEx pins speed to 50 MHz */ 
  GPIOE->OSPEEDR = 0xAAAA800A;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0xcccccccc;
  GPIOF->AFR[1]  = 0xcccccccc;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0xcccccccc;
  GPIOG->AFR[1]  = 0xcccccccc;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xaaaaaaaa;
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0xaaaaaaaa;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x00000000;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00000000;
  
/*-- FMC Configuration ------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001;
  
  /* Configure and enable SDRAM bank1 */
  FMC_Bank5_6->SDCR[0] = 0x000029D0;
  FMC_Bank5_6->SDTR[0] = 0x0332B7B1;      
  
  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x000000F9;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while(tmpreg != 0x0);
  
  /* Delay */
  for (index = 0; index<1000; index++);
  
  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x0000001A;              
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while(tmpreg != 0x0);
  
  /* Auto refresh command */
  FMC_Bank5_6->SDCMR = 0x000000FB;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while(tmpreg != 0x0);
 
  /* MRD register program */
  FMC_Bank5_6->SDCMR = 0x000460E0;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while(tmpreg != 0x0); 
  
  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000050C<<1));
  
  /* Disable write protection */
  FMC_Bank5_6->SDRTR = 0x00000064;
  tmpreg = FMC_Bank5_6->SDCR[0]; 
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);
  
#ifdef SDRAM_MEM_SWAP
  /* Swap FMC SDRAM banks and NAND2/PCCARD banks for code execution */
  SYSCFG->MEMRMP = (uint32_t)0x400; 
#endif /* SDRAM_MEM_SWAP */ 


/*
  Bank1_SDRAM is configured as follow:

  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = 2;      
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 12;  
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = 8;        
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = 12;         
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = 3;      
  FMC_SDRAMTimingInitStructure.FMC_RPDelay = 4;                
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay = 4;               

  FMC_SDRAMInitStructure.FMC_Bank = SDRAM_BANK;
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b;
  FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_11b;
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_3; 
  FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_enable;
  FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2;
  FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_disable;
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1;
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
*/
  
}
#endif /* DATA_IN_ExtSDRAM */


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
