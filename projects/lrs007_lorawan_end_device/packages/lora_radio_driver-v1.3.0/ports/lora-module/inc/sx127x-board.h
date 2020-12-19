/*!
 * \file      sx127x-board.h
 *
 * \brief     Target board SX127x driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Forest-Rain
 */
#ifndef __SX127x_BOARD_H__
#define __SX127x_BOARD_H__

#include "lora-radio-rtos-config.h"
#include <stdint.h>
#include <stdbool.h>
#include "SX127x/SX127x.h"

#ifdef LORA_RADIO_GPIO_SETUP_BY_PIN_NAME
#if ( RT_VER_NUM <= 0x40002 )
    #define LORA_RADIO_NSS_PIN       stm32_pin_get(LORA_RADIO_NSS_PIN_NAME)
    #define LORA_RADIO_RESET_PIN     stm32_pin_get(LORA_RADIO_RESET_PIN_NAME)
    #define LORA_RADIO_DIO0_PIN      stm32_pin_get(LORA_RADIO_DIO0_PIN_NAME)
    #if defined( LORA_RADIO_DIO1_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      stm32_pin_get(LORA_RADIO_DIO1_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO2_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      stm32_pin_get(LORA_RADIO_DIO2_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO3_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      stm32_pin_get(LORA_RADIO_DIO3_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO4_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      stm32_pin_get(LORA_RADIO_DIO4_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO5_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      stm32_pin_get(LORA_RADIO_DIO5_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_RFSW1_PIN_NAME ) && defined ( LORA_RADIO_RFSW2_PIN_NAME )  
    #define LORA_RADIO_RFSW1_PIN     stm32_pin_get(LORA_RADIO_RFSW1_PIN_NAME)
    #define LORA_RADIO_RFSW2_PIN     stm32_pin_get(LORA_RADIO_RFSW2_PIN_NAME)
    #endif
#else
    #define LORA_RADIO_NSS_PIN       rt_pin_get(LORA_RADIO_NSS_PIN_NAME)
    #define LORA_RADIO_RESET_PIN     rt_pin_get(LORA_RADIO_RESET_PIN_NAME)
    #define LORA_RADIO_DIO0_PIN      rt_pin_get(LORA_RADIO_DIO0_PIN_NAME)
    #if defined( LORA_RADIO_DIO1_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      rt_pin_get(LORA_RADIO_DIO1_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO2_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      rt_pin_get(LORA_RADIO_DIO2_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO3_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      rt_pin_get(LORA_RADIO_DIO3_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO4_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      rt_pin_get(LORA_RADIO_DIO4_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_DIO5_PIN_NAME ) 
    #define LORA_RADIO_DIO1_PIN      rt_pin_get(LORA_RADIO_DIO5_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_RFSW1_PIN_NAME ) && defined ( LORA_RADIO_RFSW2_PIN_NAME )  
    #define LORA_RADIO_RFSW1_PIN     rt_pin_get(LORA_RADIO_RFSW1_PIN_NAME)
    #define LORA_RADIO_RFSW2_PIN     rt_pin_get(LORA_RADIO_RFSW2_PIN_NAME)
    #endif
#endif /* ( RT_VER_NUM <= 0x40002) */

#else
    /* if not use env\menuconfig,define Radio GPIO directly.*/
    #ifndef LORA_RADIO_NSS_PIN
    #define LORA_RADIO_NSS_PIN    GET_PIN(A,15)
    #endif
    #ifndef LORA_RADIO_RESET_PIN
    #define LORA_RADIO_RESET_PIN  GET_PIN(A,7)
    #endif
    #ifndef LORA_RADIO_DIO0_PIN
    #define LORA_RADIO_DIO0_PIN   GET_PIN(B,1)
    #endif


#endif // end of LORA_RADIO_GPIO_SETUP_BY_PIN_NAME

/*!
 * \brief delayms for radio access
 */
#define SX127X_DELAY_MS(ms)              rt_thread_mdelay(ms) 

/*!
 * \brief Initializes the radio I/Os pins interface
 */
void SX127xIoInit( void );

/*!
 * \brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU low power modes
 */
void SX127xIoDeInit( void );

/*!
 * \brief Initializes the TCXO power pin.
 */
void SX127xIoTcxoInit( void );

/*!
 * \brief Initializes the radio debug pins.
 */
void SX127xIoDbgInit( void );

/*!
 * \brief Resets the radio
 */
void SX127xReset( void );

/*!
 * \brief Gets the board PA selection configuration
 *
 * \param [IN] power Selects the right PA according to the wanted power.
 * \retval PaSelect RegPaConfig PaSelect value
 */
uint8_t SX127xGetPaSelect( int8_t power );

/*!
 * \brief Set the RF Switch I/Os pins in low power mode
 *
 * \param [IN] status enable or disable
 */
void SX127xSetAntSwLowPower( bool status );

/*!
 * \brief Initializes the RF Switch I/Os pins interface
 */
void SX127xAntSwInit( void );

/*!
 * \brief De-initializes the RF Switch I/Os pins interface
 *
 * \remark Needed to decrease the power consumption in MCU low power modes
 */
void SX127xAntSwDeInit( void );

/*!
 * \brief Controls the antenna switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
void SX127xSetAntSw( uint8_t opMode );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX127xCheckRfFrequency( uint32_t frequency );

/*!
 * \brief Enables/disables the TCXO if available on board design.
 *
 * \param [IN] state TCXO enabled when true and disabled when false.
 */
void SX127xSetBoardTcxo( uint8_t state );

/*!
 * \brief Gets the Defines the time required for the TCXO to wakeup [ms].
 *
 * \retval time Board TCXO wakeup time in ms.
 */
uint32_t SX127xGetBoardTcxoWakeupTime( void );

/*!
 * \brief Writes new Tx debug pin state
 *
 * \param [IN] state Debug pin state
 */
void SX127xDbgPinTxWrite( uint8_t state );

/*!
 * \brief Writes new Rx debug pin state
 *
 * \param [IN] state Debug pin state
 */
void SX127xDbgPinRxWrite( uint8_t state );

#ifdef LORA_RADIO_GPIO_SETUP_BY_PIN_NAME
// todo PR to drv_gpio.c
int stm32_pin_get(char *pin_name);
#endif

/*!
 * Radio hardware and global parameters
 */
extern SX127x_t SX127x;

#endif // __SX127x_BOARD_H__
