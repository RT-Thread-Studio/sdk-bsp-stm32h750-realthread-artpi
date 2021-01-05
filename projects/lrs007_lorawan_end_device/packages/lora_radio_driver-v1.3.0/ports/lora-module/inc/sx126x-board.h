/*!
 * \file      sx126x-board.h
 *
 * \brief     Target board SX126x driver implementation
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
 */
#ifndef __SX126x_BOARD_H__
#define __SX126x_BOARD_H__

#include "lora-radio-rtos-config.h"
#include <stdint.h>
#include <stdbool.h>
#include "sx126x/sx126x.h"

#ifdef LORA_RADIO_GPIO_SETUP_BY_PIN_NAME
#if ( RT_VER_NUM <= 0x40002 )
    #define LORA_RADIO_NSS_PIN       stm32_pin_get(LORA_RADIO_NSS_PIN_NAME)
    #define LORA_RADIO_BUSY_PIN      stm32_pin_get(LORA_RADIO_BUSY_PIN_NAME)
    #define LORA_RADIO_DIO1_PIN      stm32_pin_get(LORA_RADIO_DIO1_PIN_NAME)
    #define LORA_RADIO_RESET_PIN     stm32_pin_get(LORA_RADIO_RESET_PIN_NAME)
    #if defined( LORA_RADIO_DIO2_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      stm32_pin_get(LORA_RADIO_DIO2_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_RFSW1_PIN_NAME ) && defined ( LORA_RADIO_RFSW2_PIN_NAME )  
    #define LORA_RADIO_RFSW1_PIN     stm32_pin_get(LORA_RADIO_RFSW1_PIN_NAME)
    #define LORA_RADIO_RFSW2_PIN     stm32_pin_get(LORA_RADIO_RFSW2_PIN_NAME)
    #endif
#else
    #define LORA_RADIO_NSS_PIN       rt_pin_get(LORA_RADIO_NSS_PIN_NAME)
    #define LORA_RADIO_BUSY_PIN      rt_pin_get(LORA_RADIO_BUSY_PIN_NAME)
    #define LORA_RADIO_DIO1_PIN      rt_pin_get(LORA_RADIO_DIO1_PIN_NAME)
    #define LORA_RADIO_RESET_PIN     rt_pin_get(LORA_RADIO_RESET_PIN_NAME)
    #if defined( LORA_RADIO_DIO2_PIN_NAME ) 
    #define LORA_RADIO_DIO2_PIN      rt_pin_get(LORA_RADIO_DIO2_PIN_NAME)
    #endif
    #if defined( LORA_RADIO_RFSW1_PIN_NAME ) && defined ( LORA_RADIO_RFSW2_PIN_NAME )  
    #define LORA_RADIO_RFSW1_PIN     rt_pin_get(LORA_RADIO_RFSW1_PIN_NAME)
    #define LORA_RADIO_RFSW2_PIN     rt_pin_get(LORA_RADIO_RFSW2_PIN_NAME)
    #endif
#endif/* ( RT_VER_NUM <= 0x40002) */

#else
    /* if not use env\menuconfig,define Radio GPIO directly.*/
    #ifndef LORA_RADIO_NSS_PIN
    #define LORA_RADIO_NSS_PIN    GET_PIN(A,15)
    #endif
    #ifndef LORA_RADIO_RESET_PIN
    #define LORA_RADIO_RESET_PIN  GET_PIN(A,7)
    #endif
    #ifndef LORA_RADIO_DIO1_PIN
    #define LORA_RADIO_DIO1_PIN   GET_PIN(B,1)
    #endif
    #ifndef LORA_RADIO_BUSY_PIN
    #define LORA_RADIO_BUSY_PIN   GET_PIN(B,2)
    #endif
    #ifndef LORA_RADIO_RFSW1_PIN
    #define LORA_RADIO_RFSW1_PIN   GET_PIN(B,0)
    #endif
    #ifndef LORA_RADIO_RFSW2_PIN
    #define LORA_RADIO_RFSW2_PIN   GET_PIN(C,5)
    #endif
#endif // end of LORA_RADIO_GPIO_SETUP_BY_PIN_NAME

/*!
 * Defines the time required for the TCXO to wakeup [ms].
 */
#define BOARD_TCXO_WAKEUP_TIME                          2

#define SX126X_DELAY_MS( ms )                           rt_thread_mdelay(ms) 
#define SX126X_BLOCK_DELAY_1MS()                        rt_hw_us_delay(999)

/*!
 * \brief Initializes the radio I/Os pins interface
 */
void SX126xIoInit( void );

/*!
 * \brief Initializes DIO IRQ handlers
 *
 * \param [IN] irqHandlers Array containing the IRQ callback functions
 */
void SX126xIoIrqInit( DioIrqHandler dioIrq );

/*!
 * \brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU low power modes
 */
void SX126xIoDeInit( void );

/*!
 * \brief Initializes the TCXO power pin.
 */
void SX126xIoTcxoInit( void );

/*!
 * \brief Initializes the radio debug pins.
 */
void SX126xIoDbgInit( void );

/*!
 * \brief HW Reset of the radio
 */
void SX126xReset( void );

/*!
 * \brief Blocking loop to wait while the Busy pin in high
 */
void SX126xWaitOnBusy( void );

/*!
 * \brief Initializes the RF Switch I/Os pins interface
 */
void SX126xAntSwOn( void );

/*!
 * \brief De-initializes the RF Switch I/Os pins interface
 *
 * \remark Needed to decrease the power consumption in MCU low power modes
 */
void SX126xAntSwOff( void );

/*!
 * \brief Set the RF Switch I/Os pins interface
 */
void SX126xSetAntSw( RadioOperatingModes_t mode );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX126xCheckRfFrequency( uint32_t frequency );

/*!
 * \brief Gets the Defines the time required for the TCXO to wakeup [ms].
 *
 * \retval time Board TCXO wakeup time in ms.
 */
uint32_t SX126xGetBoardTcxoWakeupTime( void );

/*!
 * \brief Writes new Tx debug pin state
 *
 * \param [IN] state Debug pin state
 */
void SX126xDbgPinTxWrite( uint8_t state );

/*!
 * \brief Writes new Rx debug pin state
 *
 * \param [IN] state Debug pin state
 */
void SX126xDbgPinRxWrite( uint8_t state );


#endif // __SX126x_BOARD_H__
