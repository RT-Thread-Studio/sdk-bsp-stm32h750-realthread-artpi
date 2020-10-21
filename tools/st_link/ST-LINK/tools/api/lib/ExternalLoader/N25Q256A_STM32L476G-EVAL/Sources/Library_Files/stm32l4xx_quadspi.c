/**
  ******************************************************************************
  * @file    <stmxx>_<ppp>.c
  * @author  MCD Application Team
  * @version VX.Y.ZRCx
  * @date    DD-Month-YYYY
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the QUADSPI peripheral:
  *           < Add below the list of function groups (one group per line, copied
  *             from the header file) 
  *             Note: do not use "Peripheral" or "PPP" word in the function group name
  *           >
  *           +
  *           +
  *           +
  *           +
  *
  *  @verbatim
  *
================================================================================
          ##### <Product specific features/integration> #####
================================================================================
           
  [..] < This section can contain: 
       (#) Description of the product specific implementation; all features
           that is specific to this IP: separate clock for RTC/LCD/IWDG/ADC,
           power domain (backup domain for the RTC)...   
       (#) IP main features, only when needed and not mandatory for all IPs,
           ex. for xWDG, GPIO, COMP...
       >  
       
  [..] < You can add as much sections as needed.>
  
  [..] < You can add as much sections as needed.>
                 
   
            ##### How to use this driver #####
================================================================================
           [..]
           < 
            (#) Add below all needed steps to configure/use the peripheral and 
                its associated resources (clock, GPIO, DMA, IT...) 
                !!!THE STEPS SHOULD FOLLOW WHAT IS WRITTEN IN THE REFERENCE MANUAL!!!
                Basically, it should be the same as we provide in the examples.
                 
            (#) Exception can be made for system IPs (there is no standard order
                of API usage) for which the description of API usage will be made
                in each function group. These IPs are: PWR, RCC and FLASH
                In this case, this section should contain the following note:
                "Refer to the description within the different function groups below"
           >
  
            (+) 
  
            (+) 
  
            (+) 
  
            (+) 
  
            (+) 
  
            (+) 
  
    @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT YYYY STMicroelectronics</center></h2>
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
#include "stm32l4xx_quadspi.h"
#include "stm32l4xx_rcc.h"
#include "stm32l4xx_conf.h" 
/** @addtogroup STM32L4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup QUADSPI
  * @brief QUADSPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define QUADSPI_CR_CLEAR_MASK                           0x00FFFFCF
#define QUADSPI_DCR_CLEAR_MASK                          0xFFE0F7FE
#define QUADSPI_CCR_CLEAR_MASK                          0x90800000
#define QUADSPI_PIR_CLEAR_MASK                          0xFFFF0000
#define QUADSPI_LPTR_CLEAR_MASK                         0xFFFF0000
#define QUADSPI_CCR_CLEAR_INSTRUCTION_MASK              0xFFFFFF00
#define QUADSPI_CCR_CLEAR_DCY_MASK                      0xFFC3FFFF
#define QUADSPI_CR_CLEAR_FIFOTHRESHOLD_MASK             0xFFFFF0FF
#define QUADSPI_CR_INTERRUPT_MASK                       0x001F0000
#define QUADSPI_SR_INTERRUPT_MASK                       0x0000001F

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* Initialization and Configuration functions *********************************/

/** @defgroup <PPP>_Private_Functions 
  * @{
  */

/** @defgroup <PPP>_Group1 Function Group1 Name
 *  @brief   Function group1 name description (copied from the header file) 
 *
@verbatim  
 ===============================================================================
     ##### < Function group1 name (copied from the header file)
 Note: do not use "Peripheral" or "PPP" word in the function group name >  #####
 ===============================================================================

       [..] < OPTIONAL:
              Add here the most important information to know about the IP features
              covered by this group of function.
        
              For system IPs, this section contains how to use this group API.
            >

@endverbatim
  * @{
  */   

/**
  * @brief  Deinitializes the QUADSPI peripheral registers to their default
  *         reset values.
  * @param  None
  * @retval None
  */
void QUADSPI_DeInit(void)
{
    /* Enable QUADSPI reset state */
    RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, ENABLE);
    /* Release QUADSPI from reset state */
    RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_QSPI, DISABLE);
}

/**
  * @brief  Fills each QUADSPI_InitStruct member with its default value.
  * @param  QUADSPI_InitStruct: pointer to a QUADSPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void QUADSPI_StructInit(QUADSPI_InitTypeDef* QUADSPI_InitStruct)
{
/*--------- Reset QUADSPI init structure parameters default values ------------*/
  /* Initialize the QUADSPI_SShift member */
  QUADSPI_InitStruct->QUADSPI_SShift = QUADSPI_SShift_NoShift ;
  /* Initialize the QUADSPI_Prescaler member */  
  QUADSPI_InitStruct->QUADSPI_Prescaler = 0 ;
  /* Initialize the QUADSPI_CKMode member */
  QUADSPI_InitStruct->QUADSPI_CKMode = QUADSPI_CKMode_Mode0 ;
  /* Initialize the QUADSPI_CSHTime member */
  QUADSPI_InitStruct->QUADSPI_CSHTime = QUADSPI_CSHTime_1Cycle ;
  /* Initialize the QUADSPI_FSize member */
  QUADSPI_InitStruct->QUADSPI_FSize = 0 ;
}

/**
  * @brief  Fills each QUADSPI_ComConfig_InitStruct member with its default value.
  * @param  QUADSPI_ComConfig_InitStruct: pointer to a QUADSPI_ComConfig_InitTypeDef structure which will be initialized.
  * @retval None
  */
void QUADSPI_ComConfig_StructInit(QUADSPI_ComConfig_InitTypeDef* QUADSPI_ComConfig_InitStruct)
{
/*--------- Reset QUADSPI ComConfig init structure parameters default values ------------*/
    
/* Set QUADSPI Communication configuration structure parameters default values */
  /* Initialize the QUADSPI_ComConfig_DDRMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode = QUADSPI_ComConfig_DDRMode_Disable ;
  /* Initialize the QUADSPI_ComConfig_SIOOMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode = QUADSPI_ComConfig_SIOOMode_Disable ;
  /* Initialize the QUADSPI_ComConfig_FMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode = QUADSPI_ComConfig_FMode_Indirect_Write ;
  /* Initialize the QUADSPI_ComConfig_DMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode = QUADSPI_ComConfig_DMode_NoData ;
  /* Initialize the QUADSPI_ComConfig_DummyCycles member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles = 0 ;
  /* Initialize the QUADSPI_ComConfig_ABSize member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize = QUADSPI_ComConfig_ABSize_8bit ;
  /* Initialize the QUADSPI_ComConfig_ABMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode = QUADSPI_ComConfig_ABMode_NoAlternateByte ;
  /* Initialize the QUADSPI_ComConfig_ADSize member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize = QUADSPI_ComConfig_ADSize_8bit ;
  /* Initialize the QUADSPI_ComConfig_ADMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode = QUADSPI_ComConfig_ADMode_NoAddress ;
  /* Initialize the QUADSPI_ComConfig_IMode member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode = QUADSPI_ComConfig_IMode_NoInstruction ;
  /* Initialize the QUADSPI_ComConfig_Ins member */
  QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins = 0 ;  
}

/**
  * @brief  Initializes the QUADSPI peripheral according to the specified 
  *         parameters in the QUADSPI_InitStruct.
  * @param  QUADSPI_InitStruct: pointer to a QUADSPI_InitTypeDef structure that
  *         contains the configuration information for the specified QUADSPI peripheral.
  * @retval None
  */
void QUADSPI_Init(QUADSPI_InitTypeDef* QUADSPI_InitStruct)
{
  uint32_t tmpreg = 0;
  
  /* Check the QUADSPI parameters */
  assert_param(IS_QUADSPI_SSHIFT(QUADSPI_InitStruct->QUADSPI_SShift));
  assert_param(IS_QUADSPI_PRESCALER(QUADSPI_InitStruct->QUADSPI_Prescaler));
  assert_param(IS_QUADSPI_CKMODE(QUADSPI_InitStruct->QUADSPI_CKMode));
  assert_param(IS_QUADSPI_CSHTIME(QUADSPI_InitStruct->QUADSPI_CSHTime));
  assert_param(IS_QUADSPI_FSIZE(QUADSPI_InitStruct->QUADSPI_FSize));
  
  /*------------------------ QUADSPI CR Configuration ------------------------*/
  /* Get the QUADSPI CR1 value */
  tmpreg = QUADSPI->CR;
  /* Clear PRESCALER and SSHIFT bits */
  tmpreg &= QUADSPI_CR_CLEAR_MASK;
  /* Configure QUADSPI: Prescaler and Sample Shift */
  tmpreg |= (uint32_t)(((QUADSPI_InitStruct->QUADSPI_Prescaler)<<24)
                        |(QUADSPI_InitStruct->QUADSPI_SShift));  
  /* Write to QUADSPI CR */
  QUADSPI->CR = tmpreg;
  
  /*------------------------ QUADSPI DCR Configuration ------------------------*/
  /* Get the QUADSPI DCR value */
  tmpreg = QUADSPI->DCR;
  /* Clear FSIZE, CSHT and CKMODE bits */
  tmpreg &= QUADSPI_DCR_CLEAR_MASK;
  /* Configure QUADSPI: Flash Size, Chip Select High Time and Clock Mode */
  tmpreg |= (uint32_t)(((QUADSPI_InitStruct->QUADSPI_FSize)<<16)
                        |(QUADSPI_InitStruct->QUADSPI_CSHTime)
                        |(QUADSPI_InitStruct->QUADSPI_CKMode));  
  /* Write to QUADSPI DCR */
  QUADSPI->DCR = tmpreg;  
}

/**
  * @brief  Initializes the QUADSPI CCR according to the specified 
  *         parameters in the QUADSPI_ComConfig_InitStruct.
  * @param  QUADSPI_ComConfig_InitStruct: pointer to a QUADSPI_ComConfig_InitTypeDef structure that
  *         contains the communiction configuration informations about QUADSPI peripheral.
  * @retval None
  */
void QUADSPI_ComConfig_Init(QUADSPI_ComConfig_InitTypeDef* QUADSPI_ComConfig_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the QUADSPI Communication Control parameters */
  assert_param(IS_QUADSPI_FMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode));
  assert_param(IS_QUADSPI_DDRMODE     (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode));
  assert_param(IS_QUADSPI_SIOOMODE    (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode));
  assert_param(IS_QUADSPI_DMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode));
  assert_param(IS_QUADSPI_DCY         (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles));
  assert_param(IS_QUADSPI_ABSIZE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize));
  assert_param(IS_QUADSPI_ABMODE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode));
  assert_param(IS_QUADSPI_ADSIZE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize));
  assert_param(IS_QUADSPI_ADMODE      (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode));
  assert_param(IS_QUADSPI_IMODE       (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode));
  assert_param(IS_QUADSPI_INSTRUCTION (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins));
  
  /*------------------------ QUADSPI CCR Configuration ------------------------*/
  /* Get the QUADSPI CCR value */
  tmpreg = QUADSPI->CCR;
  /* Clear FMODE Mode bits */
  tmpreg &= QUADSPI_CCR_CLEAR_MASK;
  /* Configure QUADSPI: CCR Configuration */
  tmpreg |=  (uint32_t)( (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_FMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DDRMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_SIOOMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABSize)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ABMode)                                                                       
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADSize)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_ADMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_IMode)
                       | (QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_Ins)
                       |((QUADSPI_ComConfig_InitStruct->QUADSPI_ComConfig_DummyCycles)<<18));    
  /* Write to QUADSPI DCR */
  QUADSPI->CCR = tmpreg;      
}

/**
  * @brief  Enables or disables QUADSPI peripheral.
  * @param  NewState: new state of the QUADSPI peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QUADSPI_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable QUADSPI peripheral */
    QUADSPI->CR |= QUADSPI_CR_EN;
  }
  else
  {
    /* Disable QUADSPI peripheral */
    QUADSPI->CR &= ~ QUADSPI_CR_EN;
  }
}

/**
  * @brief  Configure the QUADSPI Automatic Polling Mode.
  * @param  QUADSPI_Match: Value to be compared with the masked status register to get a match. 
  *          This parameter can be any value between 0x00000000 and 0xFFFFFFFF.
  * @param  QUADSPI_Mask: Mask to be applied to the status bytes received in polling mode.. 
  *          This parameter can be any value between 0x00000000 and 0xFFFFFFFF.
  * @param  QUADSPI_Match_Mode: indicates which method should be used for determining a “match” during
  *         automatic polling mode. 
  *          This parameter can be any value of :
  *            @arg QUADSPI_PMM_AND: AND match mode- SMF is set if all the unmasked bits received from the flash match
  *                 the corresponding bits in the match register
  *            @arg QUADSPI_PMM_OR: OR match mode- SMF is set if any one of the unmasked bits received from the flash
                    matches its corresponding bit in the match register.
  * @note   This function is used only in Automatic Polling Mode
  * @retval None
  */
void QUADSPI_AutoPollingMode_Config(uint32_t QUADSPI_Match, uint32_t QUADSPI_Mask , uint32_t QUADSPI_Match_Mode)
{
  /* Check the parameters */
  assert_param(IS_QUADSPI_PMM(QUADSPI_Match_Mode));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Set the Match Register */
    QUADSPI->PSMAR = QUADSPI_Match ;

    /* Set the Mask Register */
    QUADSPI->PSMKR = QUADSPI_Mask ;
    
    /* Set the Polling Match Mode */
    if(QUADSPI_Match_Mode)
    /* OR Match Mode */
    {
      /* Set the PMM bit */
      QUADSPI->CR |= QUADSPI_CR_PMM;
    }
    else
    /* AND Match Mode */
    {
      /* Reset the PMM bit */
      QUADSPI->CR &= ~ QUADSPI_CR_PMM;
    }
  }
}

/**
  * @brief  Sets the number of CLK cycle between two read during automatic polling phases.
  * @param  QUADSPI_Interval: The number of CLK cycle between two read during automatic polling phases. 
  *          This parameter can be any value of between 0x0000 and 0xFFFF
  * @note   This function is used only in Automatic Polling Mode  
  * @retval None
  */
void QUADSPI_AutoPollingMode_SetInterval(uint32_t QUADSPI_Interval)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_QUADSPI_PIR(QUADSPI_Interval));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Read the PIR Register */
    tmpreg = QUADSPI->PIR ;
    /* Clear Polling interval Bits */
    tmpreg &= QUADSPI_PIR_CLEAR_MASK ;
    /* Set the QUADSPI Polling Interval Bits */
    tmpreg |= QUADSPI_Interval;
    /* Write the PIR Register */
    QUADSPI->PIR = tmpreg;
  }
}

/**
  * @brief  Sets the value of the Timeout in Memory Mapped mode
  * @param  QUADSPI_Timeout: This field indicates how many CLK cycles QuadSPI waits after the 
  *         FIFO becomes full until it raises nCS, putting the flash memory 
  *         in a lowerconsumption state. 
  *         This parameter can be any value of between 0x0000 and 0xFFFF
  * @note   This function is used only in Memory Mapped Mode  
  * @retval None
  */
void QUADSPI_MemoryMappedMode_SetTimeout(uint32_t QUADSPI_Timeout)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_QUADSPI_TIMEOUT(QUADSPI_Timeout));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Read the LPTR Register */
    tmpreg = QUADSPI->LPTR ;
    /* Clear Timeout Bits */
    tmpreg &= QUADSPI_LPTR_CLEAR_MASK ;
    /* Set Timeout Bits */
    tmpreg |= QUADSPI_Timeout;
    /* Write the LPTR Register */
    QUADSPI->LPTR = tmpreg;
  }
}

/**
  * @brief  Sets the value of the Address
  * @param  QUADSPI_Address: Address to be send to the external flash memory.  
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  * @note   This function is used only in Indirect Mode  
  * @retval None
  */
void QUADSPI_SetAddress(uint32_t QUADSPI_Address)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the AR Register */
    QUADSPI->AR = QUADSPI_Address;
  }
}

/**
  * @brief  Sets the value of the Amternate Bytes
  * @param  QUADSPI_AlternateByte: Optional data to be send to the external QUADSPI device right after the address. 
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  * @note   This function is used only in Indirect Mode  
  * @retval None
  */
void QUADSPI_SetAlternateByte(uint32_t QUADSPI_AlternateByte)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the ABR Register */
    QUADSPI->ABR = QUADSPI_AlternateByte;
  }
}

/**
  * @brief  Sets the FIFO Threshold
  * @param  QUADSPI_FIFOThres: Defines, in indirect mode, the threshold number 
  *           of bytes in the FIFO which will cause the FIFO Threshold Flag 
  *           FTF to be set.
  *         This parameter can be any value of between 0x00 and 0x0F
  * @retval None
  */
void QUADSPI_SetFIFOThreshold(uint32_t QUADSPI_FIFOThreshold)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_QUADSPI_FIFOTHRESHOLD(QUADSPI_FIFOThreshold));

  /* Read the CR Register */
  tmpreg = QUADSPI->CR ;
  /* Clear FIFO Threshold Bits */
  tmpreg &= QUADSPI_CR_CLEAR_FIFOTHRESHOLD_MASK ;
  /* Set FIFO Threshold Bits */
  tmpreg |= (QUADSPI_FIFOThreshold << 8);
  /* Write the CR Register */
  QUADSPI->CR = tmpreg;
}

/**
  * @brief  Sets number of Bytes to be transferred 
  * @param  QUADSPI_DataLength: Number of data to be retrieved (value+1) 
  *         in indirect and status-polling modes. A value no greater than 3 
  *         (indicating 4 bytes) should be used for status-polling mode. 
  *         All 1s in indirect mode means undefined length, where QuadSPI will 
  *         continue until the end of memory, as defined by FSIZE
  *         This parameter can be any value of between 0x00000000 and 0xFFFFFFFF
  *             0x0000_0000: 1 byte is to be transferred
  *             0x0000_0001: 2 bytes are to be transferred
  *             0x0000_0002: 3 bytes are to be transferred
  *             0x0000_0003: 4 bytes are to be transferred
  *             ...
  *             0xFFFF_FFFD: 4,294,967,294 (4G-2) bytes are to be transferred
  *             0xFFFF_FFFE: 4,294,967,295 (4G-1) bytes are to be transferred
  *             0xFFFF_FFFF: undefined length -- all bytes until the end of flash memory (as defined
  *             by FSIZE) are to be transferred
  * @note   This function is not used in Memory Mapped Mode.
  * @retval None
  */
void QUADSPI_SetDataLength(uint32_t QUADSPI_DataLength)
{
  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    /* Write the DLR Register */
    QUADSPI->DLR = QUADSPI_DataLength;
  }
}

/**
  * @brief  Enables or disables The Timeout Counter.
  * @param  NewState: new state of the Timeout Counter. 
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function is used only in Memory Mapped Mode.
  * @retval None
  */
void QUADSPI_TimeoutCounterCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    if (NewState != DISABLE)
    {
      /* Enable Timeout Counter */
      QUADSPI->CR |= QUADSPI_CR_TCEN;
    }
    else
    {
      /* Disable Timeout Counter */
      QUADSPI->CR &= ~ QUADSPI_CR_TCEN;
    }
  }
}

/**
  * @brief  Enables or disables Automatic Polling Mode Stop when a match occurs.
  * @param  NewState: new state of the Automatic Polling Mode Stop. 
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function is used only in Automatic Polling Mode.    
  * @retval None
  */
void QUADSPI_AutoPollingModeStopCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (!(QUADSPI->SR & QUADSPI_SR_BUSY))
  /* Device is not Busy */
  {
    if (NewState != DISABLE)
    {
      /* Enable Automatic Polling Mode Stop */
      QUADSPI->CR |= QUADSPI_CR_APMS;
    }
    else
    {
      /* Disable Automatic Polling Mode Stop */
      QUADSPI->CR &= ~ QUADSPI_CR_APMS;
    }
  }
}

/**
  * @brief  Abort the on-going command sequence.
  * @param  None
  * @retval None
  */
void QUADSPI_AbortRequest(void)
{
  /* Enable the ABORT request bit in CR */
  QUADSPI->CR |= QUADSPI_CR_ABORT;
}

/* Data transfers functions ***************************************************/

/**
  * @brief  Transmits a 8bit Data through the QUADSPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void QUADSPI_SendData8(uint8_t Data)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI; 
  quadspibase += 0x20;
  
  *(__IO uint8_t *) quadspibase = Data;
}

/**
  * @brief  Transmits a 16bit Data through the QUADSPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void QUADSPI_SendData16(uint16_t Data)
{
  uint32_t quadspibase = 0;

  quadspibase = (uint32_t)QUADSPI; 
  quadspibase += 0x20;
  
  *(__IO uint16_t *) quadspibase = Data;
}

/**
  * @brief  Transmits a 32bit Data through the QUADSPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void QUADSPI_SendData32(uint32_t Data)
{
  QUADSPI->DR = Data;
}

/**
  * @brief  Returns the most recent received 8bit data by the QUADSPI peripheral. 
  * @retval The value of the received data.
  */
uint8_t QUADSPI_ReceiveData8(void)
{
  uint32_t quadspibase = 0;
  
  quadspibase = (uint32_t)QUADSPI; 
  quadspibase += 0x20;
  
  return *(__IO uint8_t *) quadspibase;
}

/**
  * @brief  Returns the most recent received 16bit data by the QUADSPI peripheral. 
  * @retval The value of the received data.
  */
uint16_t QUADSPI_ReceiveData16(void)
{
  uint32_t quadspibase = 0;
  
  quadspibase = (uint32_t)QUADSPI; 
  quadspibase += 0x20;
  
  return *(__IO uint16_t *) quadspibase;
}

/**
  * @brief  Returns the most recent received 32bit data by the QUADSPI peripheral. 
  * @retval The value of the received data.
  */
uint32_t QUADSPI_ReceiveData32(void)
{
  return QUADSPI->DR;
}

/* DMA transfers management functions *****************************************/

/**
  * @brief  Enables or disables DMA for Indirect Mode.
  * @param  NewState: new state of the Timeout Counter. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QUADSPI_DMACmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable DMA */
    QUADSPI->CR |= QUADSPI_CR_DMAEN;
  }
  else
  {
    /* Disable DMA */
    QUADSPI->CR &= ~ QUADSPI_CR_DMAEN;
  }
}

/* Interrupts and flags management functions **********************************/

/**
  * @brief  Enables or disables the specified QUADSPI interrupts.
  * @param  QUADSPI_IT: specifies the QUADSPI interrupt source to be enabled or disabled. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_IT_TO: Timeout interrupt
  *            @arg QUADSPI_IT_SM: Status Match interrupt
  *            @arg QUADSPI_IT_FT: FIFO Threshold
  *            @arg QUADSPI_IT_TC: Transfer Complete
  *            @arg QUADSPI_IT_TE: Transfer Error      
  * @param  NewState: new state of the specified QUADSPI interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QUADSPI_ITConfig(uint32_t QUADSPI_IT, FunctionalState NewState)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_QUADSPI_CONFIG_IT(QUADSPI_IT));

  /* Read the CR Register */
  tmpreg = QUADSPI->CR ;
  
  if (NewState != DISABLE)
  {
    /* Enable the selected QUADSPI interrupt */
    tmpreg |= QUADSPI_IT;
  }
  else
  {
    /* Disable the selected QUADSPI interrupt */
    tmpreg &= ~QUADSPI_IT;
  }
  /* Write the CR Register */
  QUADSPI->CR = tmpreg ;  
}

/**
  * @brief  Returns the current QUADSPI FIFO filled level.
  * @retval Number of valid bytes which are being held in the FIFO.
  *         0x00 : FIFO is empty
  *         0x1F : FIFO is full    
  */
uint32_t QUADSPI_GetFIFOLevel(void)
{
  /* Get the QUADSPI FIFO level bits */
  return ((QUADSPI->SR & QUADSPI_SR_FLEVEL)>> 8);
}

/**
  * @brief  Returns the QUADSPI functional mode.  
  * @param  None 
  * @retval QUADSPI Functional Mode .The returned value can be one of the following:
  *              - 0x00000000: QUADSPI_FMode_Indirect_Write
  *              - 0x04000000: QUADSPI_FMode_Indirect_Read
  *              - 0x08000000: QUADSPI_FMode_AutoPolling
  *              - 0x0C000000: QUADSPI_FMode_MemoryMapped
  */
uint32_t QUADSPI_GetFMode(void)
{
  /* Return the QUADSPI_FMode */
  return  (QUADSPI->CCR & QUADSPI_CCR_FMODE);
}

/**
  * @brief  Checks whether the specified QUADSPI flag is set or not.  
  * @param  QUADSPI_FLAG: specifies the QUADSPI flag to check. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_FLAG_TO: Timeout interrupt flag
  *            @arg QUADSPI_FLAG_SM: Status Match interrupt flag
  *            @arg QUADSPI_FLAG_FT: FIFO Threshold flag
  *            @arg QUADSPI_FLAG_TC: Transfer Complete flag
  *            @arg QUADSPI_FLAG_TE: Transfer Error flag
  *            @arg QUADSPI_FLAG_BUSY: Busy flag      
  * @retval The new state of QUADSPI_FLAG (SET or RESET).
  */
FlagStatus QUADSPI_GetFlagStatus(uint32_t QUADSPI_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_QUADSPI_GET_FLAG(QUADSPI_FLAG));

  /* Check the status of the specified QUADSPI flag */
  if (QUADSPI->SR & QUADSPI_FLAG)
  {
    /* QUADSPI_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* QUADSPI_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the QUADSPI_FLAG status */
  return  bitstatus;
}

/**
  * @brief  Clears the QUADSPI flag.
  * @param  QUADSPI_FLAG: specifies the QUADSPI flag to clear. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_FCLEAR_TO: Timeout interrupt flag
  *            @arg QUADSPI_FCLEAR_SM: Status Match interrupt flag
  *            @arg QUADSPI_FCLEAR_TC: Transfer Complete flag
  *            @arg QUADSPI_FCLEAR_TE: Transfer Error flag
  * @retval None
  */
void QUADSPI_ClearFlag(uint32_t QUADSPI_FLAG)
{
  /* Check the parameters */
  assert_param(IS_QUADSPI_CLEAR_FLAG(QUADSPI_FLAG));

  /* Clear the selected QUADSPI flags */
  QUADSPI->FCR = QUADSPI_FLAG;
}

/**
  * @brief  Checks whether the specified QUADSPI interrupt has occurred or not.
  * @param  QUADSPI_IT: specifies the QUADSPI interrupt source to check. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_IT_TO: Timeout interrupt 
  *            @arg QUADSPI_IT_SM: Status Match interrupt
  *            @arg QUADSPI_IT_FT: FIFO Threshold
  *            @arg QUADSPI_IT_TC: Transfer Complete
  *            @arg QUADSPI_IT_TE: Transfer Error    
  * @retval The new state of QUADSPI_IT (SET or RESET).
  */
ITStatus QUADSPI_GetITStatus(uint32_t QUADSPI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpcreg = 0,tmpsreg = 0; 

  /* Check the parameters */
  assert_param(IS_QUADSPI_GET_IT(QUADSPI_IT));

  /* Read the QUADSPI CR */
  tmpcreg = QUADSPI->CR ;
  
  tmpcreg &= QUADSPI_CR_INTERRUPT_MASK ;
  
  /* Shift the Masked Control Register to be aligned with Masked Status Register */
  tmpcreg = (tmpcreg >> 16);
  
  /* Read the QUADSPI SR */
  tmpsreg = QUADSPI->SR ;
  
  tmpsreg &= QUADSPI_SR_INTERRUPT_MASK;

  /* Check the status of the specified QUADSPI interrupt */
  if (tmpcreg & tmpsreg)
  {
    /* QUADSPI_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* QUADSPI_IT is reset */
    bitstatus = RESET;
  }
  /* Return the QUADSPI_IT status */
  return bitstatus;
}

/**
  * @brief  Clears the QUADSPI's interrupt pending bits.
  * @param  QUADSPI_IT: specifies the QUADSPI pending bit to clear. 
  *          This parameter can be one of the following values:
  *            @arg QUADSPI_IT_TO: Timeout interrupt 
  *            @arg QUADSPI_IT_SM: Status Match interrupt
  *            @arg QUADSPI_IT_TC: Transfer Complete
  *            @arg QUADSPI_IT_TE: Transfer Error 
  * @retval None
  */
void QUADSPI_ClearITPendingBit(uint32_t QUADSPI_IT)
{
  uint32_t itmask = 0;
  /* Check the parameters */
  assert_param(IS_QUADSPI_GET_IT(QUADSPI_IT)); 
  
  itmask = QUADSPI_IT >> 0x10;
  QUADSPI->FCR = (uint32_t)itmask;
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
