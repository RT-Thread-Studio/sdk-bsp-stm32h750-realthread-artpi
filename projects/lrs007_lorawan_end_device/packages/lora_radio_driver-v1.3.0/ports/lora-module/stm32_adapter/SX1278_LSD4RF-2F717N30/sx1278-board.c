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

#define LOG_TAG "LoRa.Board.LSD4RF-2F717N30(SX1278)"
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
    rt_pin_mode(LORA_RADIO_NSS_PIN, PIN_MODE_OUTPUT);
    
    rt_pin_mode(LORA_RADIO_DIO0_PIN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(LORA_RADIO_DIO1_PIN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(LORA_RADIO_DIO2_PIN, PIN_MODE_INPUT_PULLDOWN);
#else
//    GpioInit( &SX127x.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );

//    GpioInit( &SX127x.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX127x.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX127x.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX127x.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX127x.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX127x.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );

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
//    GpioSetInterrupt( &SX127x.DIO0, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[0] );
//    GpioSetInterrupt( &SX127x.DIO1, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[1] );
//    GpioSetInterrupt( &SX127x.DIO2, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[2] );
//    GpioSetInterrupt( &SX127x.DIO3, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[3] );
//    GpioSetInterrupt( &SX127x.DIO4, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[4] );
//    GpioSetInterrupt( &SX127x.DIO5, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[5] );  
#endif /*end of USING_LORA_RADIO_DRIVER_RTOS_SUPPORT */     
}

void SX127xIoDeInit( void )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
//    rt_pin_mode(LORA_RADIO_DIO0_PIN, PIN_MODE_INPUT);
//    rt_pin_mode(LORA_RADIO_DIO1_PIN, PIN_MODE_INPUT);
//    rt_pin_mode(LORA_RADIO_DIO2_PIN, PIN_MODE_INPUT);
#else
//    GpioInit( &SX127x.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 1 );

//    GpioInit( &SX127x.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX127x.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX127x.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX127x.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX127x.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX127x.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
#endif /*end of USING_LORA_RADIO_DRIVER_RTOS_SUPPORT */
}

void SX127xIoDbgInit( void )
{
#if defined( USE_RADIO_DEBUG )
    GpioInit( &DbgPinTx, RADIO_DBG_PIN_TX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioInit( &DbgPinRx, RADIO_DBG_PIN_RX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
#endif
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
////    GpioInit( &AntSwitchRx, RADIO_ANT_SWITCH_RX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
////    GpioInit( &AntSwitchTxBoost, RADIO_ANT_SWITCH_TX_BOOST, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
////    GpioInit( &AntSwitchTxRfo, RADIO_ANT_SWITCH_TX_RFO, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );

    rt_pin_mode(LORA_RADIO_RFSW1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LORA_RADIO_RFSW2_PIN, PIN_MODE_OUTPUT);
    
    rt_pin_write(LORA_RADIO_RFSW1_PIN, PIN_LOW);
    rt_pin_write(LORA_RADIO_RFSW2_PIN, PIN_LOW);
}

void SX127xAntSwDeInit( void )
{
////    GpioInit( &AntSwitchRx, RADIO_ANT_SWITCH_RX, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
////    GpioInit( &AntSwitchTxBoost, RADIO_ANT_SWITCH_TX_BOOST, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
////    GpioInit( &AntSwitchTxRfo, RADIO_ANT_SWITCH_TX_RFO, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );

    rt_pin_write(LORA_RADIO_RFSW1_PIN, PIN_LOW);
    rt_pin_write(LORA_RADIO_RFSW2_PIN, PIN_LOW);
}

/* TX/RX ANT Swich */
void SX127xSetAntSw( uint8_t opMode )
{
    uint8_t paConfig =  SX127xRead( REG_PACONFIG );
    switch( opMode )
    {
    case RFLR_OPMODE_TRANSMITTER:
        if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
        {
            ///GpioWrite( &AntSwitchTxBoost, 1 );
            rt_pin_write(LORA_RADIO_RFSW1_PIN, PIN_HIGH);
            rt_pin_write(LORA_RADIO_RFSW2_PIN, PIN_LOW);
        }
        break;
    case RFLR_OPMODE_RECEIVER:
    case RFLR_OPMODE_RECEIVER_SINGLE:
    case RFLR_OPMODE_CAD:
    default:
        ////GpioWrite( &AntSwitchRx, 1 );
        rt_pin_write(LORA_RADIO_RFSW1_PIN, PIN_LOW);
        rt_pin_write(LORA_RADIO_RFSW2_PIN, PIN_HIGH);
        break;
    }
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


