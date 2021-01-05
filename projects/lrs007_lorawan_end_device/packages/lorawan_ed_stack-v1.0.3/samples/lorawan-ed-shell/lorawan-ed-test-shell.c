/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *                forest-rain
 */

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <board.h>
#include "lorawan-ed-aps.h"

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LORAWAN.APP"
#include "lorawan-ed-debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define LORAWAN_MAX_BAT             254

/*!
 * CAYENNE_LPP is myDevices Application server.
 */
//#define CAYENNE_LPP
#define LPP_DATATYPE_DIGITAL_INPUT  0x00
#define LPP_DATATYPE_DIGITAL_OUTPUT 0x01
#define LPP_DATATYPE_HUMIDITY       0x68
#define LPP_DATATYPE_TEMPERATURE    0x67
#define LPP_DATATYPE_BAROMETER      0x73
#define LPP_APP_PORT 99
/*!
 * Defines the application data join duty cycle. 10s, value in [ms].
 */
#define APP_JOIN_DUTYCYCLE                         10000
/*!
 * Defines the application data transmission duty cycle. 5s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            5000
/*!
 * Defines the ping transmission duty cycle. 3s, value in [ms].
 */
#define PING_TX_DUTYCYCLE                           3000
/*!
 * Defines the application data transmission random. 5s, value in [ms].
 */
#define APP_TX_DUTYCYCLE_RANDOM                     5000

/*!
 * User application data buffer size
 */
#define LORAWAN_ED_APP_DATA_BUFF_SIZE               64

/*!
 * User application demo fport
 */
#define LORAWAN_ED_APP_DEMO_FPORT                   3

#define LED_Toggle( x )
#define LED_On( x )
#define LED_Off( x )

// lorawan application event
#define EV_LORAWAN_APL_START_JOIN_NETWORK               0x0001
#define EV_LORAWAN_APL_REJOIN_NETWORK                   0x0002
#define EV_LORAWAN_APL_PERIODIC_TX_DATA                 0x0004
#define EV_LORAWAN_APL_MAC_PROCESS_NOTIFY               0x8000

/* event control */
static struct rt_event ev_lorawan_apl;

static struct rt_thread lorwan_ed_test_shell_thread;

static rt_uint8_t rt_lorwan_ed_test_shell_thread_stack[4096];

/*!
 * User application data
 */
static uint8_t lorawan_ed_app_data_buff[LORAWAN_ED_APP_DATA_BUFF_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* callback when LoRaWAN End-Device has received a frame*/
static void lorawan_ed_receive_message(lorawan_ed_appdata_t *lorawan_ed_app_data);
/* callback when LoRaWAN End-Device has received a linkcheck ans*/
static void lorawan_ed_receive_linkcheck(MlmeConfirm_t *mlmeConfirm);
/* callback when LoRaWAN End-Device has just joined*/
static void lorawan_ed_joined(void);

static void lorawan_ed_tx_confirm(McpsConfirm_t *mcpsConfirm);
/* callback when LoRaWAN End-Device has just switch the class*/
static void lorawan_ed_confirm_device_class(DeviceClass_t Class);

/* callback when server needs End-Device to send a frame */
static void lorawan_ed_tx_dummy(void);

/* LoRaWAN class a ping test */
static lorawan_ed_error_status_t lorawan_ed_tx_ping_packet(uint8_t len);

/* callback to get the battery level in % of full charge (254 full charge, 0 no charge)*/
static uint8_t lorawan_ed_get_battery_level(void);

/* tx timer callback function*/
static void on_tx_timer_event(void);

/* Private variables ---------------------------------------------------------*/

/*!
 * Specifies the state of the application LED
 */
static uint8_t app_led_state_on = RESET;

static TimerEvent_t TxTimer;

static uint32_t app_tx_period = APP_TX_DUTYCYCLE;
// ping statistics
static bool ping_test_flag = false;
static uint8_t payload_len = 32;
static uint32_t tx_seq_cnt = 0;
static uint16_t max_tx_nbtrials = 10;
static uint32_t rx_correct_cnt = 0;

static int16_t rssi_value = -255;
static int16_t rssi_value_min = -255;
static int16_t rssi_value_max = -255;
static int32_t rssi_value_total = 0;

static int8_t snr_value = -128;
static int8_t snr_value_min = -128;
static int8_t snr_value_max = -128;
static int32_t snr_value_total = 0;

static int8_t gw_demod_margin_value = -128;
static int8_t gw_demod_margin_value_min = -128;
static int8_t gw_demod_margin_value_max = -128;
static int32_t gw_demod_margin_value_total = 0;

static uint8_t gw_received_nb;

static uint32_t tx_timestamp;
static uint32_t rx_timestamp;

uint32_t tx_total_byte;
uint32_t rx_total_byte;

/* Public variables ---------------------------------------------------------*/

/*!
 * User application data structure
 */
lorawan_ed_appdata_t lorawan_ed_app_data = { lorawan_ed_app_data_buff,  32, LORAWAN_ED_STACK_MAC_PARAMETER_APPLICATION_FPORT };

/* LoRaWAN End-Device app callbacks structure*/
lorawan_ed_app_callback_t lorawan_ed_user_app_callback = 
{
    lorawan_ed_get_battery_level,
    NULL,//GetTemperatureLevel,
    lorawan_ed_joined,
    lorawan_ed_tx_confirm,
    lorawan_ed_receive_message,
    lorawan_ed_receive_linkcheck,
    lorawan_ed_confirm_device_class,
    lorawan_ed_tx_dummy,
#ifndef LORAWAN_ED_STACK_USING_ON_RTOS_RT_THREAD
    NULL//LoraMacProcessNotify
#endif    
};
                                             
/* Initialize the LoRaWAN End-Device Parameters */
lorawan_ed_params_t lorawan_ed_init_params_default =
{
    0,
    CLASS_A,
    LORAWAN_ED_ACTIVATION_TYPE_OTAA,
    LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED,
    1, /* ADR ON */
    5, /* DR5( SF7 BW125 ) */
    1  /* Public */
};

lorawan_ed_params_t lorawan_ed_init_params =
{
    0,/* nvm flag loaded from NVM */
    LORAWAN_ED_STACK_MAC_PARAMETER_DEVICE_TYPE,
    LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE,
    LORAWAN_ED_STACK_MAC_PARAMETER_DATA_MESSAGE_TYPE,
    LORAWAN_ED_STACK_MAC_PARAMETER_ADR,
    LORAWAN_ED_STACK_PHY_PARAMETER_DEFAULT_DATARATE,
    LORAWAN_ED_STACK_MAC_PARAMETER_LORAWAN_NETWORK_ATTRIBUTE
};
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  lorawan_ed_app_thread_entry
  * @param  void * parameter
  * @retval None
  */
void lorawan_ed_app_thread_entry(void* parameter)
{
    rt_uint32_t ev;
    
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "LORAMAC_VERSION= %02X.%02X.%02X.%02X\r\n", (uint8_t)(__LORA_MAC_VERSION >> 24), (uint8_t)(__LORA_MAC_VERSION >> 16), (uint8_t)(__LORA_MAC_VERSION >> 8), (uint8_t)__LORA_MAC_VERSION);
   
    rt_event_init(&ev_lorawan_apl, "ev_lorawan_apl", RT_IPC_FLAG_PRIO);//RT_IPC_FLAG_FIFO);

    while (1)
    {
        if (rt_event_recv(&ev_lorawan_apl, (EV_LORAWAN_APL_START_JOIN_NETWORK |
                                            EV_LORAWAN_APL_REJOIN_NETWORK |
                                            EV_LORAWAN_APL_PERIODIC_TX_DATA),
                                            ( RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR ),
                                            RT_WAITING_FOREVER, &ev) == RT_EOK)
        {

            if (ev == EV_LORAWAN_APL_START_JOIN_NETWORK)
            {
                uint32_t start_join_timestamp = app_tx_period + randr(-APP_TX_DUTYCYCLE_RANDOM, APP_TX_DUTYCYCLE_RANDOM);
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL,
                        "After Random Delay(%d ms),Start to Join Network.", start_join_timestamp);
                TimerSetValue(&TxTimer, start_join_timestamp);
                TimerStart(&TxTimer);
            }
            else if (ev == EV_LORAWAN_APL_REJOIN_NETWORK)
            {
                /* Not joined, try again later */
                lorawan_ed_start_join_network();
            }
            else if (ev == EV_LORAWAN_APL_PERIODIC_TX_DATA)
            {
                /* Triggle for next tx slot */
                TimerSetValue(&TxTimer, app_tx_period);
                TimerStart(&TxTimer);

                if (ping_test_flag)
                {
                    /* for first time of printf info */
                    if (!tx_seq_cnt)
                    {
                        rt_kprintf("Pinging with %d bytes of data for %d counters:", payload_len, max_tx_nbtrials);
                    }

                    lorawan_ed_tx_ping_packet(payload_len);
                }
                else
                {
                    lorawan_ed_send(&lorawan_ed_app_data);
                }
                tx_seq_cnt++;
            }
        }
    }
}

/* Private functions ---------------------------------------------------------*/
static void lorawan_ed_joined(void)
{
    lorawan_ed_request_device_class((DeviceClass_t)lorawan_ed_init_params.Class);
}

static void lorawan_ed_tx_confirm(McpsConfirm_t *mcpsConfirm)
{

}

static void lorawan_ed_receive_message(lorawan_ed_appdata_t *app_data)
{
#ifdef LORAWAN_ED_STACK_DEBUG_APP_DATA_CONFIG
    if(app_data->BuffSize)
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP_DATA, DBG_LVL,"[App Payload(%d)]:",app_data->BuffSize);
        for( uint8_t i = 0; i < app_data->BuffSize; i++ )
        {
            LORAWAN_ED_DEBUG_LOG_RAW(LORAWAN_ED_STACK_DEBUG_APP_DATA, " %02X",app_data->Buff[i]);
        }
        LORAWAN_ED_DEBUG_LOG_RAW(LORAWAN_ED_STACK_DEBUG_APP_DATA, "\r\n");
    }
#endif

    switch (app_data->Port)
    {
        case LORAWAN_ED_APP_DEMO_FPORT:
        {
            /* this port switches the class */
            if (app_data->BuffSize == 1)
            {
                switch (app_data->Buff[0])
                {
                    case 0:
                    {
                        lorawan_ed_request_device_class(CLASS_A);
                        break;
                    }
                    case 1:
                    {
                        lorawan_ed_request_device_class(CLASS_B);
                        break;
                    }
                    case 2:
                    {
                        lorawan_ed_request_device_class(CLASS_C);
                        break;
                    }
                    default:
                    break;
                }
            }
            break;
        }
        case LORAWAN_ED_STACK_MAC_PARAMETER_APPLICATION_FPORT:
        {
            if ( app_data->BuffSize == 1 )
            {
                app_led_state_on = app_data->Buff[0] & 0x01;
                if (app_led_state_on == RESET)
                {
                    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "LED OFF\r\n", app_data->Port);
                    LED_Off(LED_BLUE) ;
                }
                else
                {
                    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "LED ON\r\n", app_data->Port);
                    LED_On(LED_BLUE) ;
                }
            }
            break;
        }
        case LPP_APP_PORT:
        {
            app_led_state_on = (app_data->Buff[2] == 100) ?  0x01 : 0x00;
            if (app_led_state_on == RESET)
            {
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "LED OFF\r\n", app_data->Port);

                LED_Off(LED_BLUE) ;
            }
            else
            {
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "LED ON\r\n", app_data->Port);
                LED_On(LED_BLUE) ;
            }
            break;
        }
        default:
            /* receive MAC ACK only */
            if( ping_test_flag )
            {
                rx_correct_cnt++;

                rssi_value = app_data->Rssi;
                snr_value = app_data->Snr;

                /* only used for first rx */
                if( rssi_value_max == -255 )
                {
                   rssi_value_min = rssi_value_max = app_data->Rssi;
                }
                if( snr_value_max == -128 )
                {
                   snr_value_min = snr_value_max = app_data->Snr;
                }

                /* update max and min */
                if( rssi_value < rssi_value_min )
                {
                   rssi_value_min = rssi_value;
                }
                else if( rssi_value > rssi_value_max )
                {
                   rssi_value_max = rssi_value;
                }

                if( snr_value < snr_value_min )
                {
                   snr_value_min = snr_value;
                }
                else if( snr_value > rssi_value_max )
                {
                   snr_value_max = snr_value;
                }

                rssi_value_total += rssi_value;
                snr_value_total += snr_value;

                rx_total_byte += app_data->BuffSize + LORA_MAC_FRMPAYLOAD_OVERHEAD;

                rt_kprintf("Reply from Server:seqno=%d, dn_fcnt=%d, bytes=%d, Total time=%d ms, ed_rssi=%d, ed_snr=%d, gw_demod_margin=%d, gw_received_nb=%d\r\n",
                            tx_seq_cnt, app_data->DownLinkCounter, app_data->BuffSize + LORA_MAC_FRMPAYLOAD_OVERHEAD,( rx_timestamp - tx_timestamp ), app_data->Rssi, app_data->Snr, gw_demod_margin_value, gw_received_nb);
            }
            break;
    }
}

static void lorawan_ed_receive_linkcheck(MlmeConfirm_t *mlmeConfirm)
{ 
    if( ping_test_flag )
    {
        rx_timestamp = TimerGetCurrentTime();

        if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
        {
            gw_demod_margin_value = mlmeConfirm->DemodMargin;
            gw_received_nb = mlmeConfirm->NbGateways;

            /* only used for first rx */
            if( gw_demod_margin_value_max == -128 )
            {
               gw_demod_margin_value_min = gw_demod_margin_value_max = mlmeConfirm->DemodMargin;
            }
            /* update max and min */
            if( gw_demod_margin_value < gw_demod_margin_value_min )
            {
               gw_demod_margin_value_min = gw_demod_margin_value;
            }
            else if( rssi_value > gw_demod_margin_value_max )
            {
               gw_demod_margin_value_max = gw_demod_margin_value;
            }
            gw_demod_margin_value_total += gw_demod_margin_value;
        }
        else
        {
            rt_kprintf("Request Server timed out: seqno=%d, time=%d ms\r\n", tx_seq_cnt, ( rx_timestamp - tx_timestamp ) );
        }
    }
}

static void on_tx_timer_event(void)
{
    TimerStop(&TxTimer);
    
    if ( lorawan_ed_join_status() == LORAWAN_ED_NOT_JOIN_NETWORK )
    {        
        if( app_tx_period )
        {
            rt_event_send(&ev_lorawan_apl, EV_LORAWAN_APL_REJOIN_NETWORK);
        }
    }
    else
    { 
        /* tx_seq_cnt start from 0 */
        if( tx_seq_cnt < max_tx_nbtrials || max_tx_nbtrials == 0 )
        {            
            if( app_tx_period )
            {
                rt_event_send(&ev_lorawan_apl, EV_LORAWAN_APL_PERIODIC_TX_DATA);
            }
        }
        else
        {
            if( ping_test_flag )
            {
                uint16_t per = 100 - ( (float) rx_correct_cnt / tx_seq_cnt ) * 100;
                tx_total_byte = tx_seq_cnt * ( payload_len + LORA_MAC_FRMPAYLOAD_OVERHEAD );
                uint32_t tx_total_kbyte_integer = tx_total_byte >> 10;   // / 1024
                uint32_t tx_total_kbyte_decimal = tx_total_byte & 0x3FF; // % 1024

                uint32_t rx_total_kbyte_integer = rx_total_byte >> 10;   // / 1024
                uint32_t rx_total_kbyte_decimal = rx_total_byte & 0x3FF; // % 1024
                int32_t avg_ed_rssi = -255;
                int32_t avg_ed_snr = -128;
                int32_t avg_gw_demod_margin = -128;

                if( rx_correct_cnt )
                {
                   avg_ed_rssi = rssi_value_total / (int32_t)rx_correct_cnt;
                   avg_ed_snr = snr_value_total / (int32_t)rx_correct_cnt;
                   avg_gw_demod_margin = gw_demod_margin_value_total / (int32_t)rx_correct_cnt;
                }
                rt_kprintf("\r\n====== LoRaWAN End-Device Ping statistics: ======\r\n");
                rt_kprintf("-> Tx pakcets: sent = %d, tx_total = %d.%d KByte\r\n",tx_seq_cnt, tx_total_kbyte_integer, tx_total_kbyte_decimal);
                rt_kprintf("-> Rx pakcets: received = %d, lost = %d, per = %d%, rx_total = %d.%d KByte\r\n",rx_correct_cnt, tx_seq_cnt - rx_correct_cnt, per,rx_total_kbyte_integer,rx_total_kbyte_decimal);
                rt_kprintf("--> End-Device Rx rssi: max_rssi = %d, min_rssi = %d, avg_rssi = %d\r\n",rssi_value_max,rssi_value_min,avg_ed_rssi);
                rt_kprintf("--> End-Device Rx snr : max_snr  = %d, min_snr  = %d, avg_snr  = %d\r\n",snr_value_max,snr_value_min,avg_ed_snr);
                rt_kprintf("--> Gateway RX DemodMargin : max_margin = %d, min_margin = %d, avg_margin = %d\r\n",gw_demod_margin_value_max,gw_demod_margin_value_min,avg_gw_demod_margin);
                rt_kprintf("--> Gateway RX Numbers : %d\r\n",gw_received_nb);
                rt_kprintf("====== LoRaWAN End-Device Ping Test Finished ======\r\n");
            }
        }
    }        
}

static void lorawan_ed_tx_dummy(void)
{
    lorawan_ed_app_data.BuffSize = 0;
    lorawan_ed_app_data.Port = LORAWAN_ED_STACK_MAC_PARAMETER_APPLICATION_FPORT;

    lorawan_ed_send(&lorawan_ed_app_data);
}

static void lorawan_ed_confirm_device_class(DeviceClass_t Class)
{
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "Switch to Class %c Done\r\n", "ABC"[Class]);
    /*Optionnal*/
    /*informs the server that switch has occurred ASAP,mainly for Class B*/
    lorawan_ed_tx_dummy();
}

static lorawan_ed_error_status_t lorawan_ed_tx_ping_packet(uint8_t len)
{
    uint8_t index = 0;

    tx_timestamp = TimerGetCurrentTime();

    lorawan_ed_app_data.BuffSize = len;
    lorawan_ed_app_data.Port = LORAWAN_ED_STACK_MAC_PARAMETER_APPLICATION_FPORT;

    MlmeReq_t mlmeReq;
    mlmeReq.Type = MLME_LINK_CHECK;

    if( LoRaMacMlmeRequest( &mlmeReq ) != LORAMAC_STATUS_OK )
    {
        return LORAWAN_ED_STATUS_ERROR;
    }
     
    // data
    lorawan_ed_app_data.Buff[index++] = 'P';
    lorawan_ed_app_data.Buff[index++] = 'I';
    lorawan_ed_app_data.Buff[index++] = 'N';
    lorawan_ed_app_data.Buff[index++] = 'G';
    
    // 00,01,02...
    for( uint8_t i = 0; i < len - index ; i++)
    {
        lorawan_ed_app_data.Buff[index + i] = i;
    }
    
    if( lorawan_ed_send(&lorawan_ed_app_data) != true )
    {
        return LORAWAN_ED_STATUS_ERROR;
    }
    
    return LORAWAN_ED_STATUS_SUCCESS;
}

/**
  * @brief This function return the battery level
  * @param none
  * @retval the battery level  1 (very low) to 254 (fully charged)
  */
static uint8_t lorawan_ed_get_battery_level(void)
{
  uint8_t batteryLevel = 0;
  /* todo */
  return batteryLevel;
}

#ifdef PKG_USING_EASYFLASH
#ifndef ART_PI_USING_OTA_LIB
static int rt_flash_init(void)
{
#include <spi_flash.h>
#include <drv_spi.h>
    extern rt_spi_flash_device_t rt_sfud_flash_probe(const char *spi_flash_dev_name, const char *spi_dev_name);
    extern int fal_init(void);

    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);

    /* initialize SPI Flash device */
    rt_sfud_flash_probe("norflash0", "spi10");

    fal_init();

    return 0;
}
INIT_ENV_EXPORT(rt_flash_init);
#endif /* ART_PI_USING_OTA_LIB */
#endif

int lorawan_test_shell_init(void)
{
    if( lorwan_ed_test_shell_thread.entry == RT_NULL)
    {
        rt_err_t result = RT_EOK;

        TimerInit(&TxTimer, on_tx_timer_event);

        /* Configure the LoRaWAN End-Device Stack*/
        lorawan_ed_stack_init(&lorawan_ed_user_app_callback);

        result = rt_thread_init(&lorwan_ed_test_shell_thread,
                       "lw-test-shell",
                       lorawan_ed_app_thread_entry,
                       RT_NULL,
                       &rt_lorwan_ed_test_shell_thread_stack[0],
                       sizeof(rt_lorwan_ed_test_shell_thread_stack),
                       4,
                       20);
        if( result == RT_EOK )
        {
            rt_thread_startup(&lorwan_ed_test_shell_thread);
        }
        else
        {
            rt_kprintf("lorwan_ed_test_shell_thread creat fail!\r\n");
        }
    }
    return 0;
}
INIT_APP_EXPORT(lorawan_test_shell_init);


// for finish\msh
typedef enum
{
    CMD_LORAWAN_ED_STACK_DEVEUI_INDEX = 0,
    CMD_LORAWAN_ED_STACK_JOINEUI_INDEX,
    CMD_LORAWAN_ED_STACK_APPKEY_INDEX,
    CMD_LORAWAN_ED_STACK_DEVADDR_INDEX,
    CMD_LORAWAN_ED_STACK_APPSKEY_INDEX,
    CMD_LORAWAN_ED_STACK_NWKSENCKEY_INDEX,
    CMD_LORAWAN_ED_STACK_CLASS_INDEX,
    CMD_LORAWAN_ED_STACK_CONFIRM_INDEX,
    CMD_LORAWAN_ED_STACK_ACTIVATION_INDEX,
    CMD_LORAWAN_ED_STACK_ADR_INDEX,
    CMD_LORAWAN_ED_STACK_JOIN_INDEX,    // LoRaWAN End-device Join
    CMD_LORAWAN_ED_STACK_PING_INDEX,    // LoRaWAN End-device ping test
    CMD_LORAWAN_ED_STACK_TX_INDEX,      // LoRaWAN End-device TX data
    CMD_LORAWAN_ED_STACK_SAVE_INDEX,    // LoRaWAN End-device save
    CMD_LORAWAN_ED_STACK_FACTORY_INDEX, // LoRaWAN End-device factory
}lorawan_shell_index_t;

const char* lorawan_help_info[] = 
{
    [CMD_LORAWAN_ED_STACK_DEVEUI_INDEX]      = "lorawan deveui <hex0..7>                    - set/get DevEui(8 Bytes)",
    [CMD_LORAWAN_ED_STACK_JOINEUI_INDEX]     = "lorawan joineui<hex0..7>                    - set/get JoinEui(8 Bytes)",
    [CMD_LORAWAN_ED_STACK_APPKEY_INDEX]      = "lorawan appkey <hex0..15>                   - set/get AppKey(16 Bytes)",
    [CMD_LORAWAN_ED_STACK_DEVADDR_INDEX]     = "lorawan devaddr <hex0..3>                   - set/get DevAddr(4 Bytes)",
    [CMD_LORAWAN_ED_STACK_APPSKEY_INDEX]     = "lorawan appskey <hex0..15>                  - set/get AppSKey(16 Bytes)",
    [CMD_LORAWAN_ED_STACK_NWKSENCKEY_INDEX]  = "lorawan nwkskey <hex0..15>                  - set/get NwkSEncKey(16 Bytes)",
    [CMD_LORAWAN_ED_STACK_CLASS_INDEX]       = "lorawan class <0/1/2>                       - set/get Class Type: A,B,C",
    [CMD_LORAWAN_ED_STACK_CONFIRM_INDEX]     = "lorawan confirm <0/1>                       - set/get Data Message Type: Unconfirm,Confirm",
    [CMD_LORAWAN_ED_STACK_ACTIVATION_INDEX]  = "lorawan activation <0/1>                    - set/get Activation Type: OTAA,ABP",
    [CMD_LORAWAN_ED_STACK_ADR_INDEX]         = "lorawan adr <0/1>                           - set/get ADR: disable,enable",
    [CMD_LORAWAN_ED_STACK_JOIN_INDEX]        = "lorawan join <nbtrial> <interval>           - join network:nbtrial-max join num",
    [CMD_LORAWAN_ED_STACK_PING_INDEX]        = "lorawan ping <nbtrial> <interval>           - ping network:nbtrial-max ping num",
    [CMD_LORAWAN_ED_STACK_TX_INDEX]          = "lorawan tx <mode> <cfm> <port> <len> <data> - tx data: mode:0-stop,1-once,2~1500-cnt,>1500-period,cfm:0/1",
#ifdef PKG_USING_EASYFLASH
    [CMD_LORAWAN_ED_STACK_SAVE_INDEX]        = "lorawan save <cfg/dev>                      - save config or device info",
    [CMD_LORAWAN_ED_STACK_FACTORY_INDEX]     = "lorawan factory                             - recover to factory setup",
#endif
};

static void lorawan_shell_usage(void)
{
    size_t i = 0;
    /* parameter error */
    rt_kprintf("Usage:\r\n");
    for (i = 0; i < sizeof(lorawan_help_info) / sizeof(char*); i++) 
    {
        rt_kprintf("%s\r\n", lorawan_help_info[i]);
    }
    rt_kprintf("\r\n");
}

/* LoRaWAN End-Device shell function */
static int lorawan(int argc, char *argv[])
{        
    if (argc < 2)
    {   
        lorawan_shell_usage();
    } 
    else 
    {
       const char *cmd = argv[1];
       MibRequestConfirm_t mibReq;

       /* SET \ GET */

        if (!rt_strcmp("deveui", cmd))
        {
            if( argc > 2 )
            {
                uint8_t deveui[8] = { 0 };
                for(uint8_t i = 0;i < 8;i++)
                {
                    deveui[i] = get_hex_byte(&argv[2]);
                }

                rt_kprintf("DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X", HEX8(deveui));

                if( lorawan_ed_set_deveui(deveui) == RT_EOK )
                {
                    rt_kprintf(" Set OK\r\n");
                }
                else
                {
                    rt_kprintf(" Set Failed\r\n");
                }
            }
            else
            {
                rt_kprintf("DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X\r\n", HEX8(lorawan_ed_get_deveui()));
            }
        }
        else if (!rt_strcmp("joineui", cmd))
        {
            uint8_t joineui[8] = { 0 };
            if( argc > 2 )
            {
                for(uint8_t i = 0;i < 8;i++)
                {
                    joineui[i] = get_hex_byte(&argv[2]);
                }

                rt_kprintf("JoinEUI=%02X%02X%02X%02X%02X%02X%02X%02X", HEX8(joineui));

                if( lorawan_ed_set_joineui(joineui) == RT_EOK )
                {
                    rt_kprintf(" Set OK\r\n", HEX8(joineui));
                }
                else
                {
                    rt_kprintf(" Set Failed\r\n", HEX8(joineui));
                }
            }
            else
            {
                rt_kprintf("JoinEUI=%02X%02X%02X%02X%02X%02X%02X%02X\r\n", HEX8(lorawan_ed_get_joineui()));
            }
        }
        else if (!rt_strcmp("appkey", cmd))
        {
            if( argc > 2 )
            {
                uint8_t appkey[16] = { 0 };

                for(uint8_t i = 0;i < 16;i++)
                {
                    appkey[i] = get_hex_byte(&argv[2]);
                }
                rt_kprintf("AppKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", HEX16(appkey));

                if( lorawan_ed_set_appkey(appkey) == RT_EOK )
                {
                    rt_kprintf(" Set OK\r\n");
                }
                else
                {
                    rt_kprintf(" Set Failed\r\n");
                }
            }
            else
            {
                rt_kprintf("AppKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", HEX16(lorawan_ed_get_appkey()));
            }
        }
        else if (!rt_strcmp("devaddr", cmd))
        {
            if( argc > 2 )
            {
#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
               uint8_t devaddr[4] = { 0 };

               for(uint8_t i = 0;i < 4; i++)
               {
                   devaddr[i] = get_hex_byte(&argv[2]);
               }

               rt_kprintf("DevAddr=%08X", devaddr[0] << 24 | devaddr[1] << 16 | devaddr[2] << 8 | devaddr[3]);

               if (lorawan_ed_set_devaddr(devaddr) == RT_EOK)
               {
                   rt_kprintf(" Set OK\r\n");
               }
               else
               {
                   rt_kprintf(" Set Fail\r\n");
               }
#else
                rt_kprintf("Error,Enable ABP function Please.\r\n");
#endif
            }
            else
            {
                uint32_t devaddr = lorawan_ed_get_devaddr();
                rt_kprintf("DevAddr=%08X\r\n", devaddr);
            }
        }
        else if (!rt_strcmp("appskey", cmd))
        {
            if (argc > 2)
            {
#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
                uint8_t appskey[16] = { 0 };

                for (uint8_t i = 0; i < 16; i++)
                {
                    appskey[i] = get_hex_byte(&argv[2]);
                }
                rt_kprintf("AppSKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",HEX16(appskey));
                if (lorawan_ed_set_appskey(appskey) == RT_EOK)
                {
                    rt_kprintf(" Set OK\r\n");
                }
                else
                {
                    rt_kprintf(" Set Failed\r\n");
                }
#else
                rt_kprintf("Error,Enable ABP function Please.\r\n");
#endif
            }
            else
            {
                rt_kprintf("AppSKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
                        HEX16(lorawan_ed_get_appskey()));
            }
        }
        else if (!rt_strcmp("nwkskey", cmd))
        {
            if (argc > 2)
            {
#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
                uint8_t nwkskey[16] = { 0 };

                for (uint8_t i = 0; i < 16; i++)
                {
                    nwkskey[i] = get_hex_byte(&argv[2]);
                }
                rt_kprintf("AppSKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",HEX16(nwkskey));
                if (lorawan_ed_set_nwkskey(nwkskey) == RT_EOK)
                {
                    rt_kprintf(" Set OK\r\n");
                }
                else
                {
                    rt_kprintf(" Set Failed\r\n");
                }
#else
                rt_kprintf("Error,Enable ABP function Please.\r\n");
#endif
            }
            else
            {
                rt_kprintf("AppSKey=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
                        HEX16(lorawan_ed_get_nwkskey()));
            }
        }
        else if (!rt_strcmp("class", cmd))
        {
            mibReq.Type = MIB_DEVICE_CLASS;
            if (argc > 2)
            {
                DeviceClass_t class_type = (DeviceClass_t) atol(argv[2]);

                if (class_type <= CLASS_C)
                {
                    lorawan_ed_init_params.Class = class_type;

                    mibReq.Param.Class = class_type;
                    LoRaMacMibSetRequestConfirm(&mibReq);
                }
                else
                {
                    rt_kprintf("Class Type:%d Is NOT Supported", class_type);
                }
            }
            else
            {
                LoRaMacMibGetRequestConfirm(&mibReq);
            }
            rt_kprintf("Class Type: %c, %c\r\n", "ABC"[lorawan_ed_init_params.Class],"ABC"[(mibReq.Param.Class & 0x03)]);
        }
        else if (!rt_strcmp("nbtrials", cmd))
        {
            mibReq.Type = MIB_CHANNELS_NB_TRANS;

            if( argc > 2 )
            {
                uint8_t nbtrials = atoi(argv[2]);

                if( nbtrials <= 15 )
                {
                    mibReq.Param.ChannelsNbTrans = nbtrials;
                    LoRaMacMibSetRequestConfirm( &mibReq );
                }
                else
                {
                    rt_kprintf("Nbtrials:%d Is NOT Supported",nbtrials);
                }
            }
            else
            {
                LoRaMacMibGetRequestConfirm( &mibReq );
            }
            rt_kprintf("nbtrials: %d\r\n", mibReq.Param.ChannelsNbTrans);
        }
        else if (!rt_strcmp("confirm", cmd))
        {
            if( argc > 2 )
            {
                lorawan_ed_init_params.DataMessageType = atoi(argv[2]);
            }

            rt_kprintf("Data Message Type: %s\r\n", lorawan_ed_init_params.DataMessageType?"Confirm":"UnConfirm");
        }
        else if (!rt_strcmp("activation", cmd))
        {
            if( argc > 2 )
            {
                lorawan_ed_init_params.ActivationType = atoi(argv[2]);
            }

            rt_kprintf("Activation Type: %s\r\n", lorawan_ed_init_params.ActivationType?"ABP":"OTAA");
        }
        else if (!rt_strcmp("adr", cmd))
        {
            if( argc > 2 )
            {
                lorawan_ed_init_params.AdrEnable = atoi(argv[2]);
            }

            rt_kprintf("ADR: %s\r\n", lorawan_ed_init_params.AdrEnable?"Enable":"Disable");
        }
        /* JOIN */
       else if (!rt_strcmp(cmd, "join"))
        {
            /* otaa */
            lorawan_ed_init_params.ActivationType = LORAWAN_ED_ACTIVATION_TYPE_OTAA;
            app_tx_period = APP_JOIN_DUTYCYCLE;

            if (argc > 2)
            {
                /*
                 * nbtrials
                 *  0   - stop join
                 *  > 0 - join max nbtrials
                 * */
                max_tx_nbtrials = atoi(argv[2]);

                if (argc > 3)
                {
                    uint16_t period = atol(argv[3]);
                    /* min join period */
                    if (period < 8)
                    {
                        rt_kprintf("Join Period Error!\r\n");
                    }
                    else
                    {
                        app_tx_period = period;
                    }
                }
            }

            if ( max_tx_nbtrials )
            {
                rt_event_send(&ev_lorawan_apl, EV_LORAWAN_APL_START_JOIN_NETWORK);
            }
            else
            {
                /* stop periodic join */
                app_tx_period = 0;

                TimerStop(&TxTimer);

                rt_kprintf("Stop Join\r\n");
            }
        }
        else if (!rt_strcmp(cmd, "ping")) /* Ping test */
        {
            ping_test_flag = true;
            tx_seq_cnt = 0;
            rx_correct_cnt = 0;
            max_tx_nbtrials = 10;
            app_tx_period = PING_TX_DUTYCYCLE; // 3s

            // initlize default value
            rssi_value = -255;
            rssi_value_min = -255;
            rssi_value_max = -255;
            rssi_value_total = 0;
            snr_value = -128;
            snr_value_min = -128;
            snr_value_max = -128;
            snr_value_total = 0;
            gw_demod_margin_value = -128;
            gw_demod_margin_value_min = -128;
            gw_demod_margin_value_max = -128;
            gw_demod_margin_value_total = 0;

            if ( lorawan_ed_join_status() == LORAWAN_ED_NOT_JOIN_NETWORK )
            {
                rt_kprintf("==== Please Join Network first ====\r\n");

                return 1;
            }

            if (argc > 2)
            {
                max_tx_nbtrials = atoi(argv[2]);

                if (argc > 3)
                {
                    app_tx_period = atoi(argv[3]);
                }
            }
            on_tx_timer_event();
        }
        else if (!rt_strcmp(cmd, "tx")) /* TX test */
        {
            /* tx_mode define:
             * - =0,     stop periodic tx
             * - =1,     tx immediately,tx counter = 1
             * - 2~1500, periodic tx with max counter, period is 10 sec,tx counters = tx_mode
             * - £¾1500, periodic tx forever,period = tx_mode, unit is ms
             */
            uint32_t tx_mode = 0;

            /* tx_mode */
            if (argc > 2)
            {
                tx_mode = atol(argv[2]);

                /* data message type
                 * 0 - unconfirm
                 * 1 - confirm
                 */
                if (argc > 3)
                {
                    lorawan_ed_init_params.DataMessageType = atoi(argv[3]);

                    /* fport 1~223 */
                    if (argc > 4)
                    {
                        uint8_t fport = atoi(argv[4]);
                        if ((fport > 0) && (fport < 224))
                        {
                            lorawan_ed_app_data.Port = atoi(argv[4]);
                        }
                        else
                        {
                            LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL,
                                    "==== fport=%d is not in range 1~223 ====", fport);
                        }

                        /* data len */
                       if (argc > 5)
                       {
                           uint16_t i = 0;

                           lorawan_ed_app_data.BuffSize = atoi(argv[5]);

                           /* user data */
                           for (i = 0; i < lorawan_ed_app_data.BuffSize; i++)
                           {
                               lorawan_ed_app_data.Buff[i] = get_hex_byte(&argv[i + 6]);
                           }
                       }
                    }
                }

            }

            if ( tx_mode )
            {
                tx_seq_cnt = 0;
                ping_test_flag = false;

                /* tx immediately,only once */
                if (tx_mode == 1)
                {
                    if (lorawan_ed_join_status() == LORAWAN_ED_JOINED_NETWORK)
                    {
                        lorawan_ed_send(&lorawan_ed_app_data);
                    }
                    else
                    {
                        rt_kprintf("==== Please Join Network first ====\r\n");
                    }
                }
                else
                {
                    /* periodic tx */
                    if ( tx_mode >= 1500 )
                    {
                        app_tx_period = tx_mode; /* tx period */
                        max_tx_nbtrials = 0;
                    }
                    else
                    {
                        app_tx_period = APP_TX_DUTYCYCLE;
                        max_tx_nbtrials = app_tx_period;
                    }

                    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL,
                            "\r\n==== Started Periodic Report Data-Message Timer,Report Period: %d ms, %s, fport: %d====",\
                            app_tx_period,(lorawan_ed_init_params.DataMessageType?"Confirm":"UnConfirm"),lorawan_ed_app_data.Port);
                    on_tx_timer_event();
                }
            }
            else
            {
                /* stop periodic tx */
                TimerStop(&TxTimer);
                app_tx_period = 0;

                rt_kprintf("Stop Periodic Tx\r\n");
            }
        }
#ifdef PKG_USING_EASYFLASH
        else if(!rt_strcmp(cmd, "save")) /* save to flash */
        {
            /* device id info */
            if (!rt_strcmp(argv[2], "dev"))
            {
                lorawan_ed_save_dev_info();
                rt_kprintf("save dev done\r\n");
            }
            else // if (!rt_strcmp(argv[2], "cfg"))
            {
                lorawan_ed_save_cfg();
                rt_kprintf("save cfg done\r\n");
            }
        }
        else if(!rt_strcmp(cmd, "factory"))
        {
            /* clear cfg parameters */
            lorawan_ed_init_params = lorawan_ed_init_params_default;
            lorawan_ed_save_cfg();
            rt_kprintf("save cfg done\r\n");
        }
#endif /* PKG_USING_EASYFLASH */
        else
        {
            rt_kprintf("Parameter Error\r\n");
        }
    }
    return 1;
}
MSH_CMD_EXPORT(lorawan, lorawan end-device test);
