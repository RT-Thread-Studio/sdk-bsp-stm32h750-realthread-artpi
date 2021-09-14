/*!
 * \file      lora-radio.c for sx127x
 *
 * \brief     LoRa Radio interface
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

#include <stdlib.h>
#include "lora-radio.h"
#include "sx127x\sx127x.h"
#include "sx127x-board.h"

#ifdef RT_USING_PM
#include "drivers/pm.h"
#ifndef PM_RADIO_ID
#define PM_RADIO_ID  ( PM_MODULE_MAX_ID - 3 )
#endif
#endif

#define LOG_TAG "PHY.LoRa.SX127X"
#define LOG_LEVEL  LOG_LVL_DBG
#include "lora-radio-debug.h"

#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD

#define EV_LORA_RADIO_IRQ_MASK         0x0007 // DIO0 | DIO1 | DIO2 depend on board

#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
static struct rt_event lora_radio_event;
static struct rt_thread lora_radio_thread;
static rt_uint8_t rt_lora_radio_thread_stack[4096];
                            
extern struct rt_spi_device *lora_radio_spi_init(const char *bus_name, const char *lora_device_name, rt_uint8_t param);

#endif

#endif // end of LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD

static bool SX127xRadioInit( RadioEvents_t *events );

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX127xRadioInit,
    SX127xGetStatus,
    SX127xSetModem,
    SX127xSetChannel,
    SX127xIsChannelFree,
    SX127xRandom,
    SX127xSetRxConfig,
    SX127xSetTxConfig,
    SX127xCheckRfFrequency,
    SX127xGetTimeOnAir,
    SX127xSend,
    SX127xSetSleep,
    SX127xSetStby,
    SX127xSetRx,
    SX127xStartCad,
    SX127xSetTxContinuousWave,
    SX127xReadRssi,
    SX127xWrite,
    SX127xRead,
    //SX127xWriteBuffer,
    //SX127xReadBuffer,
    SX127xSetMaxPayloadLength,
    SX127xSetPublicNetwork,
    SX127xGetWakeupTime,
    NULL, // void ( *IrqProcess )( void )
    SX127xCheck,
    //SX126x Only
    NULL, // void ( *RxBoosted )( uint32_t timeout ) 
    NULL, // void ( *SetRxDutyCycle )( uint32_t rxTime, uint32_t sleepTime ) 
};

static bool lora_radio_init = false;

#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD

static uint8_t get_irq_index(uint32_t ev)
{
    uint32_t i = 0;
    for(i = 0; i < 32; i++)
    {
        if(ev & 0x01)
        {
            break;
        }
        ev >>= 1;
    }
    return i;
}

#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
/**
  * @brief  lora_radio_thread_entry
  * @param  None
  * @retval None
  */
void lora_radio_thread_entry(void* parameter)
{
    rt_uint32_t ev;
    
    while(1)
    {
        if (rt_event_recv(&lora_radio_event, EV_LORA_RADIO_IRQ_MASK,
                                (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
                                RT_WAITING_FOREVER, &ev) == RT_EOK)
        {
#ifdef RT_USING_PM
            rt_pm_module_request(PM_RADIO_ID, PM_SLEEP_MODE_NONE);
            RadioIrqProcess(get_irq_index(ev));
            rt_pm_module_release(PM_RADIO_ID, PM_SLEEP_MODE_NONE);
#else
            RadioIrqProcess(get_irq_index(ev));
#endif
        }
    }
}
#endif

bool SX127xRadioInit( RadioEvents_t *events )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    if( lora_radio_init == false )
    {
        // Initialize spi bus
        SX127x.spi = lora_radio_spi_init(LORA_RADIO0_SPI_BUS_NAME, LORA_RADIO0_DEVICE_NAME, RT_NULL);
        if (SX127x.spi == RT_NULL)
        {
            LORA_RADIO_DEBUG_LOG(LR_DBG_INTERFACE, LOG_LEVEL, "SX127x SPI Init Failed\n");
            return false;
        }

        LORA_RADIO_DEBUG_LOG(LR_DBG_INTERFACE, LOG_LEVEL, "SX127x SPI Init Succeed\n");

        rt_event_init(&lora_radio_event, "ev_lora_phy", RT_IPC_FLAG_PRIO);//RT_IPC_FLAG_FIFO);

        rt_thread_init(&lora_radio_thread,               	  
                       "lora-phy",                     	  
                       lora_radio_thread_entry,          	  
                       RT_NULL,                    	  
                       &rt_lora_radio_thread_stack[0],       
                       sizeof(rt_lora_radio_thread_stack),  
                       1,   // highest priority                       	  
                       20);                           
                                   
        rt_thread_startup(&lora_radio_thread);   
                       
       lora_radio_init = true;
   }
#endif /* LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD */ 
   
   SX127xIoInit();   
   SX127xInit(events);
   
   return true;
}

void SX127xOnDio0IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ0_FIRED);
#endif
    }
void SX127xOnDio1IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ1_FIRED);
#endif    
}
void SX127xOnDio2IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ2_FIRED);
#endif  
}
void SX127xOnDio3IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ3_FIRED);
#endif    
}
void SX127xOnDio4IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ4_FIRED);
#endif
}
void SX127xOnDio5IrqEvent( void *args )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_IRQ5_FIRED);
#endif
}
void SX127xOnTimeoutIrqEvent( void )
{
#ifdef LORA_RADIO_DRIVER_USING_ON_RTOS_RT_THREAD
    rt_event_send(&lora_radio_event, EV_LORA_RADIO_TIMEOUT_FIRED);
#endif
}

#else
void RadioInit( RadioEvents_t *events )
{
    SX127xInit(events);
}
#endif
