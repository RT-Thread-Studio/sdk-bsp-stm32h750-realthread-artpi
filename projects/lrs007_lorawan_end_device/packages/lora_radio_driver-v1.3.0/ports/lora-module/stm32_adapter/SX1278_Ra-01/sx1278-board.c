/*!
 * \file      sx1278-board.c
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
#include "lora-radio-rtos-config.h"
#include "lora-radio.h"
#include "sx127x\sx127x.h"
#include "sx127x-board.h"

#define LOG_TAG "LoRa.Board.Ra-01(SX1278)"
#define LOG_LEVEL  LOG_LVL_DBG 
#include "lora-radio-debug.h"
/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
/*!
 * \brief DIO 0 IRQ callback
 */
void SX127xOnDio0IrqEvent( void *args );

/*!
 * \brief DIO 1 IRQ callback
 */
void SX127xOnDio1IrqEvent( void *args );

/*!
 * \brief DIO 2 IRQ callback
 */
void SX127xOnDio2IrqEvent( void *args );

/*!
 * \brief DIO 3 IRQ callback
 */
void SX127xOnDio3IrqEvent( void *args );

/*!
 * \brief DIO 4 IRQ callback
 */
void SX127xOnDio4IrqEvent( void *args );

/*!
 * \brief DIO 5 IRQ callback
 */
void SX127xOnDio5IrqEvent( void *args );
#endif

/*!
 * Debug GPIO pins objects
 */
#if defined( USE_RADIO_DEBUG )
Gpio_t DbgPinTx;
Gpio_t DbgPinRx;
#endif

#ifdef LORA_RADIO_GPIO_SETUP_BY_PIN_NAME
#if ( RT_VER_NUM <= 0x40004 )
int stm32_pin_get(char *pin_name)
{
    /* eg: pin_name : "PA.4"  ( GPIOA, GPIO_PIN_4 )--> drv_gpio.c pin */
    char pin_index = strtol(&pin_name[3],0,10);
    
    if(pin_name[1] < 'A' || pin_name[1] > 'Z')
    {
        return -1;
    }

    return (16 * (pin_name[1]-'A') + pin_index);
}
#endif 
#endif /* LORA_RADIO_GPIO_SETUP_BY_PIN_NAME */

void SX127xIoInit( void )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    // RT-Thread
    rt_pin_mode(LORA_RADIO_NSS_PIN, PIN_MODE_OUTPUT);
    
    rt_pin_mode(LORA_RADIO_DIO0_PIN, PIN_MODE_INPUT_PULLDOWN);
#ifdef LORA_RADIO_DIO1_PIN     
    rt_pin_mode(LORA_RADIO_DIO1_PIN, PIN_MODE_INPUT_PULLDOWN);
#endif    
#ifdef LORA_RADIO_DIO2_PIN      
    rt_pin_mode(LORA_RADIO_DIO2_PIN, PIN_MODE_INPUT_PULLDOWN);
#endif    
#else

#endif
}

void SX127xIoIrqInit( DioIrqHandler **irqHandlers )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    #ifdef LORA_RADIO_DIO0_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO0_PIN, PIN_IRQ_MODE_RISING,SX127xOnDio0IrqEvent,(void*)"rf-dio0");
    rt_pin_irq_enable(LORA_RADIO_DIO0_PIN, PIN_IRQ_ENABLE);    
    #endif
    #ifdef LORA_RADIO_DIO1_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO1_PIN, PIN_IRQ_MODE_RISING,SX127xOnDio1IrqEvent,(void*)"rf-dio1");
    rt_pin_irq_enable(LORA_RADIO_DIO1_PIN, PIN_IRQ_ENABLE);    
    #endif
    #ifdef LORA_RADIO_DIO2_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO2_PIN, PIN_IRQ_MODE_RISING,SX127xOnDio2IrqEvent,(void*)"rf-dio2");
    rt_pin_irq_enable(LORA_RADIO_DIO2_PIN, PIN_IRQ_ENABLE);    
    #endif
#else
    #ifdef LORA_RADIO_DIO0_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO0_PIN, PIN_IRQ_MODE_RISING,irqHandlers[0],(void*)"rf-dio0");
    rt_pin_irq_enable(LORA_RADIO_DIO0_PIN, PIN_IRQ_ENABLE);    
    #endif
    #ifdef LORA_RADIO_DIO1_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO1_PIN, PIN_IRQ_MODE_RISING,irqHandlers[1],(void*)"rf-dio1");
    rt_pin_irq_enable(LORA_RADIO_DIO1_PIN, PIN_IRQ_ENABLE);    
    #endif
    #ifdef LORA_RADIO_DIO2_PIN
    rt_pin_attach_irq(LORA_RADIO_DIO2_PIN, PIN_IRQ_MODE_RISING,irqHandlers[2],(void*)"rf-dio2");
    rt_pin_irq_enable(LORA_RADIO_DIO2_PIN, PIN_IRQ_ENABLE);    
    #endif
#endif    
}

void SX127xIoDeInit( void )
{

}

void SX127xIoDbgInit( void )
{

}


void SX127xReset( void )
{
    // Set RESET pin to 0
    rt_pin_mode(LORA_RADIO_RESET_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LORA_RADIO_RESET_PIN, PIN_LOW);
    
    // Wait 1 ms
    SX127X_DELAY_MS( 1 );

    // Configure RESET as input
    rt_pin_mode(LORA_RADIO_RESET_PIN, PIN_MODE_INPUT);

    // Wait 6 ms
    SX127X_DELAY_MS( 6 );
}

void SX127xSetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX127xAntSwInit( );
        }
        else
        {
            SX127xAntSwDeInit( );
        }
    }
}

void SX127xAntSwInit( void )
{
}

void SX127xAntSwDeInit( void )
{
}

/* TX and RX ANT switch */

void SX127xSetAntSw( uint8_t opMode )
{
}

#if defined( USE_RADIO_DEBUG )
void SX127xDbgPinTxWrite( uint8_t state )
{
    GpioWrite( &DbgPinTx, state );
}

void SX127xDbgPinRxWrite( uint8_t state )
{
    GpioWrite( &DbgPinRx, state );
}
#endif

uint8_t SX127xGetPaSelect( int8_t power )
{
    return RF_PACONFIG_PASELECT_PABOOST;
}

bool SX127xCheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported,todo depend on board
    return true; 
}


