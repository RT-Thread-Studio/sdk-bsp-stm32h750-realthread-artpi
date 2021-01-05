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

#include "LoRaMac.h"
#include "lorawan-ed-aps.h"
#include "lorawan-ed-certificate.h"

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LORAWAN.APS"
#include "lorawan-ed-debug.h"

#ifdef PKG_USING_FAL
#include <fal.h>
#endif
#ifdef PKG_USING_EASYFLASH
#include <easyflash.h>
#endif
/*!
 *  Select either Device_Time_req or Beacon_Time_Req following LoRaWAN version 
 *  - Device_Time_req   Available for V1.0.3 or later                          
 *  - Beacon_time_Req   Available for V1.0.2 and before                        
 */
#define LORAWAN_ED_STACK_USING_DEVICE_TIMING

/*!
 * Join requests trials duty cycle.
 */
#define OVER_THE_AIR_ACTIVATION_DUTYCYCLE           10000  // 10 [s] value in ms

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )

#include "LoRaMacTest.h"

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        true
#endif

#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
/*!
 * Default ping slots periodicity
 *
 * \remark periodicity is equal to 2^LORAWAN_DEFAULT_PING_SLOT_PERIODICITY seconds
 *         example: 2^3 = 8 seconds. The end-device will open an Rx slot every 8 seconds.
 */
#define LORAWAN_ED_STACK_DEFAULT_PING_SLOT_PERIODICITY       0   
#endif

static lorawan_ed_device_id_info_t lorawan_ed_device_id =
{
   .DevEui = LORAWAN_DEVICE_EUI,
   .JoinEui = LORAWAN_JOIN_EUI,

   .AppKey = LORAWAN_APP_KEY,
#ifdef LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X
   .NwkKey = LORAWAN_NWK_KEY,
#else
   /* 1.0.x NwkKey must be equal to AppKey */
   .NwkKey = LORAWAN_APP_KEY,
#endif

#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
   .DevAddr = LORAWAN_DEVICE_ADDRESS,
   .AppSKey = LORAWAN_APP_S_KEY,
   .NwkSEncKey = LORAWAN_NWK_S_ENC_KEY,
#ifdef LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X
   .FNwkSIntKey = LORAWAN_F_NWK_S_INT_KEY,
   .SNwkSIntKey = LORAWAN_S_NWK_S_INT_KEY,
#endif /* LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X */
#endif
};

#ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
static MlmeReqJoin_t JoinParameters;
#endif

#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
static lorawan_ed_error_status_t lorawan_ed_beacon_req( void);
static lorawan_ed_error_status_t lorawan_ed_ping_slot_req( void);

#if defined( LORAWAN_ED_STACK_USING_DEVICE_TIMING )
static lorawan_ed_error_status_t lorawan_ed_device_time_req(void);
#else
static lorawan_ed_error_status_t lorawan_ed_beacon_time_req(void);
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TIMING */
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */

/*!
 * Defines the LoRaWAN End-Device parameters at Initlize
 */

static LoRaMacPrimitives_t LoRaMacPrimitives;
static LoRaMacCallback_t LoRaMacCallbacks;
static MibRequestConfirm_t mibReq;
static lorawan_ed_app_callback_t *lorawan_ed_app_callback;
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA
static uint8_t join_request_trials = 0;
static uint8_t join_request_trials_max = 20;
#endif

/* application interface */
extern lorawan_ed_appdata_t lorawan_ed_app_data;
extern lorawan_ed_params_t lorawan_ed_init_params;

/*!
 * MAC event info status strings.
 */
const char* loramac_event_info_status_strings[] =
{ 
    "MAC EVENT OK",                            // LORAMAC_EVENT_INFO_STATUS_OK
    "MAC EVENT Error",                         // LORAMAC_EVENT_INFO_STATUS_ERROR
    "MAC EVENT Tx timeout",                    // LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT
    "MAC EVENT Rx1 timeout",                   // LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT
    "MAC EVENT Rx2 timeout",                   // LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT
    "MAC EVENT Rx1 error",                     // LORAMAC_EVENT_INFO_STATUS_RX1_ERROR
    "MAC EVENT Rx2 error",                     // LORAMAC_EVENT_INFO_STATUS_RX2_ERROR
    "MAC EVENT Join failed",                   // LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL
    "MAC EVENT Downlink repeated",             // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED
    "MAC EVENT Tx DR payload size error",      // LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR
    "MAC EVENT Downlink too many frames loss", // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS
    "MAC EVENT Address fail",                  // LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL
    "MAC EVENT MIC fail",                      // LORAMAC_EVENT_INFO_STATUS_MIC_FAIL
    "MAC EVENT Multicast fail",                // LORAMAC_EVENT_INFO_STATUS_MULTICAST_FAIL
    "MAC EVENT Beacon locked",                 // LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED
    "MAC EVENT Beacon lost",                   // LORAMAC_EVENT_INFO_STATUS_BEACON_LOST
    "MAC EVENT Beacon not found"               // LORAMAC_EVENT_INFO_STATUS_BEACON_NOT_FOUND
};


static void uplink_frame_info_trace(McpsConfirm_t *mcpsConfirm);
static void downlink_frame_info_trace(McpsIndication_t *mcpsIndication);
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
static void beacon_Info_trace(MlmeIndication_t *mlmeIndication);
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] McpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "###### ===== McpsConfirm STATUS:%s ===== ######", loramac_event_info_status_strings[mcpsConfirm->Status] );
  
    if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
    {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                               
                break;
            }
            case MCPS_PROPRIETARY:
            {
                break;
            }
            default:
                break;
        }
    }

#if LORAWAN_ED_STACK_DEBUG_APS == 1
    uplink_frame_info_trace(mcpsConfirm);
#endif    

    lorawan_ed_app_callback->lorawan_ed_tx_confirm(mcpsConfirm);
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== McpsInd STATUS:%s ===== ######", loramac_event_info_status_strings[mcpsIndication->Status] );
    lorawan_ed_appdata_t app_data_temp = { 0 };

    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            break;
        }
        case MCPS_CONFIRMED:
        {
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        case MCPS_MULTICAST:
        {
            break;
        }
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    if( mcpsIndication->FramePending == true )
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"Frame Pending is true\r\n"); 

        // The server signals that it has pending data to be sent.
        // We schedule an uplink as soon as possible to flush the server.
        lorawan_ed_app_callback->lorawan_ed_tx_dummy( );
     }
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot
#ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
    if (lorawan_ed_certificate_running() == true )
    {
        lorawan_ed_certificate_downlink_increment( );
    }
#endif

#if LORAWAN_ED_STACK_DEBUG_APS == 1
    downlink_frame_info_trace(mcpsIndication);
#endif

    if( mcpsIndication->RxData == true )
    {
        switch( mcpsIndication->Port )
        {
#ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
           case LORAWAN_ED_CETIFICATE_TEST_FPORT:
               lorawan_ed_certificate_rx( mcpsIndication, &JoinParameters );
               return;
#endif
           default:
           {
               app_data_temp.Port = mcpsIndication->Port;
               app_data_temp.BuffSize = mcpsIndication->BufferSize;
               app_data_temp.Buff = mcpsIndication->Buffer;
               break;
           }
        }
    }
    
    app_data_temp.Rssi = mcpsIndication->Rssi;
    app_data_temp.Snr = mcpsIndication->Snr;
    app_data_temp.DownLinkCounter = mcpsIndication->DownLinkCounter;
    app_data_temp.AckReceived = mcpsIndication->AckReceived;
    lorawan_ed_app_callback->lorawan_ed_receive_message( &app_data_temp );
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] MlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
    MibRequestConfirm_t mibReq;
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */

    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== MlmeConfirm STATUS:%s ===== ######\r\n", loramac_event_info_status_strings[mlmeConfirm->Status] );
    
    switch( mlmeConfirm->MlmeRequest )
    {
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA
        case MLME_JOIN:
        {
            if (mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK)
            {
                /* Status is OK, node has joined the network */
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APP, DBG_LVL, "###### ===== Joined ==== ######");
                lorawan_ed_app_callback->lorawan_ed_joined();
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
                if( lorawan_ed_init_params.->Class == CLASS_B )
                {
#if defined( LORAWAN_ED_STACK_USING_DEVICE_TIMING )
                    lorawan_ed_device_time_req();
#else
                    lorawan_ed_beacon_time_req();
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TIMING */
                }
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
            }
            else
            {
                // Join was not successful. Try to join again
                lorawan_ed_start_join_network();
            }
            break;
        }
#endif /* LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA */
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
#ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
                // Check DemodMargin
                // Check NbGateways
                if (lorawan_ed_certificate_running() == true )
                {
                     lorawan_ed_certificate_linkcheck(mlmeConfirm);
                }
#endif
            }
            lorawan_ed_app_callback->lorawan_ed_receive_linkcheck(mlmeConfirm);
            break;
        }
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
        case MLME_BEACON_ACQUISITION:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                /* Beacon has been acquired */
                /* REquest Server for Ping Slot */
                lorawan_ed_ping_slot_req( );
            }
            else
            {
                /* Beacon not acquired */
                /* Search again */
                /* we can check if the MAC has received a time reference for the beacon*/
                /* in this case do either a Device_Time_Req  or a Beacon_Timing_req*/
                lorawan_ed_beacon_req( );
            }
            break;
        }
        case MLME_PING_SLOT_INFO:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
               /* class B is now ativated*/
                mibReq.Type = MIB_DEVICE_CLASS;
                mibReq.Param.Class = CLASS_B;
                LoRaMacMibSetRequestConfirm( &mibReq );
                
#if defined( REGION_AU915 ) || defined( REGION_US915 )
                mibReq.Type = MIB_PING_SLOT_DATARATE;
                mibReq.Param.PingSlotDatarate = DR_8;
                LoRaMacMibSetRequestConfirm( &mibReq );
#endif
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"\r\n#= Switch to Class B done. =#r\n" );
                
                /*notify upper layer*/
                lorawan_ed_app_callback->lorawan_ed_confirm_device_class(CLASS_B);
            }
            else
            {
                lorawan_ed_ping_slot_req( );
            }
            break;
        }
#if defined( LORAWAN_ED_STACK_USING_DEVICE_TIMING )        
        case MLME_DEVICE_TIME:
        {        
            if( mlmeConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK )
            {
                lorawan_ed_device_time_req();
            }  
        }              
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TIMING */
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
        default:
            break;
    }
}

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] MlmeIndication - Pointer to the indication structure.
 */
static void MlmeIndication( MlmeIndication_t *MlmeIndication )
{
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
    MibRequestConfirm_t mibReq;
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */

    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== MLMEInd STATUS:%s ===== ######\r\n", loramac_event_info_status_strings[MlmeIndication->Status] );

    switch( MlmeIndication->MlmeIndication )
    {
        case MLME_SCHEDULE_UPLINK:
        {
            // The MAC signals that we shall provide an uplink as soon as possible
            lorawan_ed_app_callback->lorawan_ed_tx_dummy( );
            break;
        }
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
        case MLME_BEACON_LOST:
        {
            // Switch to class A again
            mibReq.Type = MIB_DEVICE_CLASS;
            mibReq.Param.Class = CLASS_A;
            LoRaMacMibSetRequestConfirm( &mibReq );
            
            LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"\r\n#= Switch to Class A done. =# BEACON LOST\r\n" );

            lorawan_ed_beacon_req();
            break;
        }
        case MLME_BEACON:
        {
            if( MlmeIndication->Status == LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED )
            {
                beacon_Info_trace(MlmeIndication);
            }
            else
            {
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "BEACON NOT RECEIVED\r\n");
            }
            break;
        }
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
        default:
            break;
    }
}

/**
 *  lorawan set deveui
 */
uint8_t lorawan_ed_set_deveui(uint8_t *deveui)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    memcpy1(lorawan_ed_device_id.DevEui, deveui, 8);

    mibReq.Type = MIB_DEV_EUI;
    mibReq.Param.DevEui = deveui;
    status = LoRaMacMibGetRequestConfirm( &mibReq );

    if( status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}
/**
 *  lorawan get deveui
 */
uint8_t* lorawan_ed_get_deveui(void)
{
    return lorawan_ed_device_id.DevEui;
}

/**
 *  lorawan set join
 */
uint8_t lorawan_ed_set_joineui(uint8_t *joineui)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    memcpy1(lorawan_ed_device_id.JoinEui, joineui, 8);

    mibReq.Type = MIB_DEV_EUI;
    mibReq.Param.DevEui = joineui;
    status = LoRaMacMibSetRequestConfirm( &mibReq );

    if(status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}

/**
 *  lorawan get joineui
 */
uint8_t* lorawan_ed_get_joineui(void)
{
    return lorawan_ed_device_id.JoinEui;
}

/**
 *  lorawan set appkey
 */
uint8_t lorawan_ed_set_appkey(uint8_t *appkey)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    memcpy1(lorawan_ed_device_id.AppKey, appkey, 16);

    mibReq.Type = MIB_APP_KEY;
    mibReq.Param.DevEui = appkey;
    status = LoRaMacMibSetRequestConfirm( &mibReq );

    if(status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}

/**
 *  lorawan get appkey
 */
uint8_t* lorawan_ed_get_appkey(void)
{
    return lorawan_ed_device_id.AppKey;
}



/**
 *  lorawan set devaddr
 */
uint8_t lorawan_ed_set_devaddr(uint8_t *devaddr)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    lorawan_ed_device_id.DevAddr = devaddr[0] << 24 | devaddr[1] << 16 | devaddr[2] << 8 | devaddr[3];
    mibReq.Type = MIB_DEV_ADDR;
    mibReq.Param.DevAddr = lorawan_ed_device_id.DevAddr;
    status = LoRaMacMibSetRequestConfirm( &mibReq );

    if(status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}

/**
 *  lorawan get devaddr
 */
uint32_t lorawan_ed_get_devaddr(void)
{
    return lorawan_ed_device_id.DevAddr;
}

/**
 *  lorawan set appskey
 */
uint8_t lorawan_ed_set_appskey(uint8_t *appskey)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    memcpy1(lorawan_ed_device_id.AppSKey, appskey, 16);

    mibReq.Type = MIB_APP_S_KEY;
    mibReq.Param.DevEui = appskey;
    status = LoRaMacMibSetRequestConfirm( &mibReq );

    if(status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}

/**
 *  lorawan get appskey
 */
uint8_t* lorawan_ed_get_appskey(void)
{
    return lorawan_ed_device_id.AppSKey;
}

/**
 *  lorawan set nwkskey
 */
uint8_t lorawan_ed_set_nwkskey(uint8_t *nwkskey)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    memcpy1(lorawan_ed_device_id.NwkSEncKey, nwkskey, 16);

    mibReq.Type = MIB_NWK_S_ENC_KEY;
    mibReq.Param.DevEui = nwkskey;
    status = LoRaMacMibSetRequestConfirm( &mibReq );

    if(status == LORAMAC_STATUS_OK )
    {
        return RT_EOK;
    }
    return RT_ERROR;
}

/**
 *  lorawan get nwkskey
 */
uint8_t* lorawan_ed_get_nwkskey(void)
{
    return lorawan_ed_device_id.NwkSEncKey;
}

#ifdef PKG_USING_EASYFLASH
void lorawan_ed_save_dev_info(void)
{
    EfErrCode result = EF_NO_ERR;

    /* set and store the device information to Env */
    lorawan_ed_device_id.nvm_flag = 0xAABB;
    result = ef_set_env_blob("ed_dev", &lorawan_ed_device_id.nvm_flag, LORAWAN_ED_DEV_ID_INFO_SIZE );

    if (result == EF_NO_ERR)
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Device Info(%d) Save Successed!\n", LORAWAN_ED_DEV_ID_INFO_SIZE);
    }
    else
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Device Info Save Fail,%d!\n", result);
    }
}

int lorawan_ed_get_nvm_dev_info(void)
{
    size_t saved_len;
    lorawan_ed_device_id_info_t temp = { 0 };

    ef_get_env_blob("ed_dev", &temp, LORAWAN_ED_DEV_ID_INFO_SIZE, &saved_len);
    if( temp.nvm_flag == 0xAABB )
    {
        lorawan_ed_device_id = temp;
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Loaded NVM Device Info OK\n");
    }
    else
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Loaded NVM Device Info Nothing(%x)\n",temp.nvm_flag);

        return 0;
    }

    return saved_len;
}

void lorawan_ed_save_cfg(void)
{
    EfErrCode result = EF_NO_ERR;

    /* set and store the config information to Env */
    lorawan_ed_init_params.nvm_flag = 0xAABB;
    result = ef_set_env_blob("ed_cfg", &lorawan_ed_init_params.nvm_flag, LORAWAN_ED_APS_CFG_SIZE);

    if (result == EF_NO_ERR)
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Config Parameters(%d) Save Successed!\n", LORAWAN_ED_APS_CFG_SIZE);
    }
    else
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Config Parameters Save Fail,%d!\n", result);
    }
}

int lorawan_ed_get_nvm_cfg(void)
{
    size_t saved_len = 0;
    lorawan_ed_params_t temp = { 0 };

    ef_get_env_blob("ed_cfg", &temp, LORAWAN_ED_APS_CFG_SIZE, &saved_len);
    if( temp.nvm_flag == 0xAABB )
    {
        lorawan_ed_init_params = temp;
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Loaded NVM Config Parameters OK\n");
    }
    else
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Loaded NVM Config Parameters Nothing(%X)\n",temp.nvm_flag);

        return 0;
    }

    return saved_len;
}

#endif

/**
 *  lorawan end-device stack Init
 */
void lorawan_ed_stack_init(lorawan_ed_app_callback_t *callbacks)
{
#ifdef PKG_USING_EASYFLASH
    fal_init();
    easyflash_init();

    if(lorawan_ed_get_nvm_dev_info() == 0)
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "NVM Device Information is null\n");
    }

    if( lorawan_ed_get_nvm_cfg() == 0 )
    {
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "NVM Configuration is null\n");
    }
#endif
    /* init the main call backs*/
    lorawan_ed_app_callback = callbacks;

    LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;
    LoRaMacPrimitives.MacMcpsIndication = McpsIndication;
    LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;
    LoRaMacPrimitives.MacMlmeIndication = MlmeIndication;
    LoRaMacCallbacks.GetBatteryLevel = lorawan_ed_app_callback->BoardGetBatteryLevel;
    LoRaMacCallbacks.GetTemperatureLevel = lorawan_ed_app_callback->BoardGetTemperatureLevel;
#ifndef LORAWAN_ED_STACK_USING_ON_RTOS_RT_THREAD
    LoRaMacCallbacks.MacProcessNotify = lorawan_ed_app_callback->MacProcessNotify;
#endif
    
#if defined( REGION_AS923 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_AS923 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: AS923"); 
#elif defined( REGION_AU915 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_AU915 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: AU915"); 
#elif defined( REGION_CN470 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_CN470 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: CN470"); 
#elif defined( REGION_CN470S )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_CN470S );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: CN470S"); 
#elif defined( REGION_CN779 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_CN779 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: CN779"); 
#elif defined( REGION_EU433 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_EU433 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: EU433"); 
#elif defined( REGION_IN865 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_IN865 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: IN865"); 
#elif defined( REGION_EU868 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_EU868 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: EU868"); 
#elif defined( REGION_KR920 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_KR920 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: KR920"); 
#elif defined( REGION_US915 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_US915 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: US915"); 
#elif defined( REGION_RU864 )
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_RU864 );
    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "REGION: RU864"); 
#else
    #error "Please define a region in the compiler options."
#endif

#if defined( HYBRID )
#if defined( REGION_US915 ) || defined( REGION_AU915 )
    uint16_t channelMask[] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000};
    mibReq.Type = MIB_CHANNELS_MASK;
    mibReq.Param.ChannelsMask = channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );
    mibReq.Type = MIB_CHANNELS_DEFAULT_MASK;
    mibReq.Param.ChannelsDefaultMask = channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );
#endif
#endif

#if (( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK0 ) || \
     ( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK1 ) || \
     ( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK2 ) || \
     ( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK3 ) || \
     ( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK4 ) || \
     ( defined LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK5 ) )

    uint16_t channelMask[6] = { 0 };
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK0
    char* channelmask0_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK0;
    channelMask[0] = get_hex_byte(&channelmask0_str) << 8;
    channelMask[0] |= get_hex_byte(&channelmask0_str);
#endif
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK1
    char* channelmask1_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK1;
    channelMask[1] = get_hex_byte(&channelmask1_str) << 8;
    channelMask[1] |= get_hex_byte(&channelmask1_str);
#endif
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK2
    char* channelmask2_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK2;
    channelMask[2] = get_hex_byte(&channelmask2_str) << 8;
    channelMask[2] |= get_hex_byte(&channelmask2_str);
#endif
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK3
    char* channelmask3_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK3;
    channelMask[3] = get_hex_byte(&channelmask3_str) << 8;
    channelMask[3] |= get_hex_byte(&channelmask3_str);
#endif
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK4
    char* channelmask4_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK4;
    channelMask[4] = get_hex_byte(&channelmask4_str) << 8;
    channelMask[4] |= get_hex_byte(&channelmask4_str);
#endif
#ifdef LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK5
    char* channelmask5_str = LORAWAN_ED_STACK_PHY_PARAMETER_CHANNEL_MASK5;
    channelMask[5] = get_hex_byte(&channelmask5_str) << 8;
    channelMask[5] |= get_hex_byte(&channelmask5_str);
#endif

    mibReq.Type = MIB_CHANNELS_MASK;
    mibReq.Param.ChannelsMask = channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );
    mibReq.Type = MIB_CHANNELS_DEFAULT_MASK;
    mibReq.Param.ChannelsDefaultMask = channelMask;
    LoRaMacMibSetRequestConfirm( &mibReq );
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA
    if ( lorawan_ed_init_params.ActivationType == LORAWAN_ED_ACTIVATION_TYPE_OTAA )
    {
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_DEVEUI
        char* deveui_str = LORAWAN_ED_STACK_MAC_PARAMETER_DEVEUI;
        for(uint8_t i = 0; i < 8;i++)
        {
            lorawan_ed_device_id.DevEui[i] = get_hex_byte(&deveui_str);
        }
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_JOINEUI
        char* joineui_str = LORAWAN_ED_STACK_MAC_PARAMETER_JOINEUI;
        for(uint8_t i = 0; i < 8;i++)
        {
            lorawan_ed_device_id.JoinEui[i] = get_hex_byte(&joineui_str);
        }
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_APPKEY
        char* appkey_str = LORAWAN_ED_STACK_MAC_PARAMETER_APPKEY;
        for(uint8_t i = 0; i < 16;i++)
        {
            lorawan_ed_device_id.AppKey[i] = get_hex_byte(&appkey_str);
#ifdef LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_0_X
            /* 1.0.x NwkKey must be equal to AppKey */
            lorawan_ed_device_id.NwkKey[i] = lorawan_ed_device_id.AppKey[i];
#endif
        }
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_NWKKEY
        /* 1.1.x */
        char* nwkkey_str = LORAWAN_ED_STACK_MAC_PARAMETER_NWKKEY;
        for(uint8_t i = 0; i < 16;i++)
        {
            lorawan_ed_device_id.NwkKey[i] = get_hex_byte(&nwkkey_str);
        }
#endif
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Activation Type: OTAA");
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-DevEui:  %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
                HEX8(lorawan_ed_device_id.DevEui));
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-JoinEui: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
                HEX8(lorawan_ed_device_id.JoinEui));
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-AppKey:  %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X", HEX16(lorawan_ed_device_id.AppKey));
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-NwkKey:  %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X", HEX16(lorawan_ed_device_id.NwkKey));

        mibReq.Type = MIB_DEV_EUI;
        mibReq.Param.DevEui = lorawan_ed_device_id.DevEui;
        LoRaMacMibSetRequestConfirm(&mibReq);

        mibReq.Type = MIB_JOIN_EUI;
        mibReq.Param.JoinEui = lorawan_ed_device_id.JoinEui;
        LoRaMacMibSetRequestConfirm(&mibReq);

        mibReq.Type = MIB_APP_KEY;
        mibReq.Param.AppKey = lorawan_ed_device_id.AppKey;
        LoRaMacMibSetRequestConfirm(&mibReq);

        mibReq.Type = MIB_NWK_KEY;
        mibReq.Param.NwkKey = lorawan_ed_device_id.NwkKey;
        LoRaMacMibSetRequestConfirm( &mibReq );
    }
#endif /* LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA */

#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
    if ( lorawan_ed_init_params.ActivationType == LORAWAN_ED_ACTIVATION_TYPE_ABP )
    {
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_DEVADDR
        char* devaddr_str = LORAWAN_ED_STACK_MAC_PARAMETER_DEVADDR;

        lorawan_ed_device_id.DevAddr = get_hex_byte(&devaddr_str) << 24;
        lorawan_ed_device_id.DevAddr |= get_hex_byte(&devaddr_str)<< 16;
        lorawan_ed_device_id.DevAddr |= get_hex_byte(&devaddr_str)<< 8;
        lorawan_ed_device_id.DevAddr |= get_hex_byte(&devaddr_str);
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_APPSKEY
        char* appskey_str = LORAWAN_ED_STACK_MAC_PARAMETER_APPSKEY;
        for(uint8_t i = 0; i < 16; i++)
        {
            lorawan_ed_device_id.AppSKey[i] = get_hex_byte(&appskey_str);
        }
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_NWKSENCKEY
        char* nwksenckey_str = LORAWAN_ED_STACK_MAC_PARAMETER_NWKSENCKEY;
        for(uint8_t i = 0; i < 16; i++)
        {
            lorawan_ed_device_id.NwkSEncKey[i] = get_hex_byte(&nwksenckey_str);
        }
#endif

#if LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_FNWKSINTKEY
        char* fnwksintkey_str = LORAWAN_ED_STACK_MAC_PARAMETER_FNWKSINTKEY;
        for(uint8_t i = 0; i < 16; i++)
        {
            lorawan_ed_device_id.FNwkSIntKey[i] = get_hex_byte(&fnwksintkey_str);
        }
#endif

#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_SNWKSINTKEY
        char* snwksintkey_str = LORAWAN_ED_STACK_MAC_PARAMETER_SNWKSINTKEY;
        for(uint8_t i = 0; i < 16; i++)
        {
            lorawan_ed_device_id.SNwkSIntKey[i] = get_hex_byte(&snwksintkey_str);
        }
#endif
#endif /* LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X */

        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Activation Type: ABP");
        //LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,  "-DevEui= %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n", HEX8(devEui));
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-DevAddr: %08X", lorawan_ed_device_id.DevAddr);
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-AppSKey: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", HEX16(lorawan_ed_device_id.AppSKey));
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-NwkSEncKey: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", HEX16(lorawan_ed_device_id.NwkSEncKey));
#if LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-FNwkSIntKey: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", HEX16(lorawan_ed_device_id.FNwkSIntKey));
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "-SNwkSIntKey: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", HEX16(lorawan_ed_device_id.SNwkSIntKey));
#endif /* LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X */

        mibReq.Type = MIB_NET_ID;
        mibReq.Param.NetID = LORAWAN_NETWORK_ID;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_DEV_ADDR;
        mibReq.Param.DevAddr = lorawan_ed_device_id.DevAddr;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_S_NWK_S_INT_KEY;
        mibReq.Param.SNwkSIntKey = lorawan_ed_device_id.NwkSEncKey;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_NWK_S_ENC_KEY;
        mibReq.Param.NwkSEncKey = lorawan_ed_device_id.NwkSEncKey;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_APP_S_KEY;
        mibReq.Param.AppSKey = lorawan_ed_device_id.AppSKey;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_NETWORK_ACTIVATION;
        mibReq.Param.NetworkActivation = ACTIVATION_TYPE_ABP;
        LoRaMacMibSetRequestConfirm( &mibReq );

#if LORAWAN_ED_STACK_USING_LORAWAN_SPECIFICATION_V1_1_X
        mibReq.Type = MIB_F_NWK_S_INT_KEY;
        mibReq.Param.FNwkSIntKey = lorawan_ed_device_id.FNwkSIntKey;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_S_NWK_S_INT_KEY;
        mibReq.Param.SNwkSIntKey = lorawan_ed_device_id.SNwkSIntKey;
        LoRaMacMibSetRequestConfirm( &mibReq );

        // Enable legacy mode to operate according to LoRaWAN Spec. 1.1.0
        Version_t abpLrWanVersion;

        abpLrWanVersion.Fields.Major = 1;
        abpLrWanVersion.Fields.Minor = 1;
        abpLrWanVersion.Fields.Revision = 0;
        abpLrWanVersion.Fields.Rfu = 0;

        mibReq.Type = MIB_ABP_LORAWAN_VERSION;
        mibReq.Param.AbpLrWanVersion = abpLrWanVersion;
        LoRaMacMibSetRequestConfirm( &mibReq );
#else
        // Enable legacy mode to operate according to LoRaWAN Spec. 1.0.3
        Version_t abpLrWanVersion;

        abpLrWanVersion.Fields.Major = 1;
        abpLrWanVersion.Fields.Minor = 0;
        abpLrWanVersion.Fields.Revision = 3;
        abpLrWanVersion.Fields.Rfu = 0;

        mibReq.Type = MIB_ABP_LORAWAN_VERSION;
        mibReq.Param.AbpLrWanVersion = abpLrWanVersion;
        LoRaMacMibSetRequestConfirm( &mibReq );
#endif
    }
#endif /* LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP */

    mibReq.Type = MIB_PUBLIC_NETWORK;
    mibReq.Param.EnablePublicNetwork = lorawan_ed_init_params.EnablePublicNetwork;
    LoRaMacMibSetRequestConfirm( &mibReq );
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Public Network:   %s", mibReq.Param.EnablePublicNetwork?"Enable":"Disable");

    mibReq.Type = MIB_DEVICE_CLASS;
    mibReq.Param.Class= lorawan_ed_init_params.Class;
    LoRaMacMibSetRequestConfirm( &mibReq );
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Class Type:       %c","ABC"[mibReq.Param.Class]); 
                      
    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = lorawan_ed_init_params.AdrEnable;
    LoRaMacMibSetRequestConfirm( &mibReq );
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "ADR:              %s", mibReq.Param.AdrEnable?"Enable":"Disable"); 
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Datarate:         DR_%d", lorawan_ed_init_params.TxDatarate);

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
    LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif
      
    mibReq.Type = MIB_SYSTEM_MAX_RX_ERROR;
    mibReq.Param.SystemMaxRxError = LORAWAN_ED_STACK_MAC_PARAMETER_SYSTEM_MAX_RX_ERROR;
    LoRaMacMibSetRequestConfirm( &mibReq );
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "SystemMaxRxError: %d ppm", mibReq.Param.SystemMaxRxError);

    LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Channels Mask: %04X-%04X-%04X-%04X-%04X-%04X\r\n",
            channelMask[0], channelMask[1], channelMask[2], channelMask[3], channelMask[4], channelMask[5]);

    /*set Mac statein Idle*/
    LoRaMacStart( );

#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
    if ( lorawan_ed_init_params.ActivationType == LORAWAN_ED_ACTIVATION_TYPE_ABP )
    {
        lorawan_ed_app_callback->lorawan_ed_joined();
    }
#endif /* LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP */
}

LoRaMacStatus_t lorawan_ed_start_join_network( void )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
#ifdef LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_OTAA
    if ( lorawan_ed_init_params.ActivationType == LORAWAN_ED_ACTIVATION_TYPE_OTAA )
    {
        static uint32_t join_request_trials_max_base = 0;
        static uint32_t join_request_start_timestamp = 0;
        static int8_t join_request_datarata = -1;

        MlmeReq_t mlmeReq;

        /* first time using the user define datarate */
        if(join_request_datarata == -1)
        {
            join_request_datarata = lorawan_ed_init_params.TxDatarate;
        }

        GetPhyParams_t getPhy;
        PhyParam_t phyParam;

        join_request_trials++;
        if( ( join_request_trials - 1) % 2 )
        {
            /* every twice lower tx datarate */
            getPhy.Attribute = PHY_NEXT_LOWER_TX_DR;
            getPhy.Datarate = join_request_datarata;
            phyParam = RegionGetPhyParam( LORAMAC_REGION_CN470S, &getPhy );
            join_request_datarata = phyParam.Value;
        }
    
        mlmeReq.Type = MLME_JOIN;
        mlmeReq.Req.Join.Datarate = join_request_datarata;
    #ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
        JoinParameters = mlmeReq.Req.Join;
    #endif
    
        if( join_request_start_timestamp == 0 )
        {
            join_request_start_timestamp = TimerGetCurrentTime();
        }

        if( join_request_trials <= ( join_request_trials_max_base + join_request_trials_max ) )
        {
            LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== Join-Request @ (%d / %d) with DR=%d ==== ######", join_request_trials, (join_request_trials_max_base + join_request_trials_max), mlmeReq.Req.Join.Datarate);
            status = LoRaMacMlmeRequest( &mlmeReq );
            LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== Join-Request: %s ==== ######",loramac_status_strings[status] );
            if( status == LORAMAC_STATUS_OK )
            {
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== Joining ==== ######" );
            }
            else
            {
                if( status == LORAMAC_STATUS_DUTYCYCLE_RESTRICTED )
                {
                    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Next Tx in  : %lu [ms]\n", mlmeReq.ReqReturn.DutyCycleWaitTime );
                }
            }
        }
        else
        {
            LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"###### ===== Join Failed(%d),Cost time:%d sec, Please Check DEVEUI-JOINEUI-APPKEY... ==== ######\n",(join_request_trials - 1),( TimerGetCurrentTime() - join_request_start_timestamp )/1000);
            join_request_trials_max_base = join_request_trials - 1;
        }
    }
#endif

#if ( defined LORAWAN_ED_STACK_MAC_PARAMETER_ACTIVATION_TYPE_ABP ) || (defined LORAWAN_ED_STACK_USING_ACTIVATION_TYPE_ABP)
    if ( lorawan_ed_init_params.ActivationType == LORAWAN_ED_ACTIVATION_TYPE_ABP )
    {
        lorawan_ed_app_callback->lorawan_ed_joined();
    }
#endif
    return status;
}

lorawan_ed_join_status_t lorawan_ed_join_status( void )
{
    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_NETWORK_ACTIVATION;

    LoRaMacMibGetRequestConfirm( &mibReq );

    if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        return LORAWAN_ED_NOT_JOIN_NETWORK;
    }
    else
    {
        return LORAWAN_ED_JOINED_NETWORK;
    }
}

bool lorawan_ed_send(lorawan_ed_appdata_t* AppData)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
  
#ifdef LORAWAN_ED_STACK_CETIFICATE_TEST_ENABLE
    /*if certification test are on going, application data is not sent*/
    if (lorawan_ed_certificate_running() == true)
    {
      return false;
    }
#endif
    
    if( LoRaMacQueryTxPossible( AppData->BuffSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = lorawan_ed_init_params.TxDatarate;
    }
    else
    {
        if( lorawan_ed_init_params.DataMessageType == LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = AppData->Port;
            mcpsReq.Req.Unconfirmed.fBufferSize = AppData->BuffSize;
            mcpsReq.Req.Unconfirmed.fBuffer = AppData->Buff;
            mcpsReq.Req.Unconfirmed.Datarate = lorawan_ed_init_params.TxDatarate;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = AppData->Port;
            mcpsReq.Req.Confirmed.fBufferSize = AppData->BuffSize;
            mcpsReq.Req.Confirmed.fBuffer = AppData->Buff;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = lorawan_ed_init_params.TxDatarate;
        }
    }
    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}  

#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
#if defined( LORAWAN_ED_STACK_USING_DEVICE_TIMING )
static lorawan_ed_error_status_t lorawan_ed_device_time_req( void)
{
  MlmeReq_t mlmeReq;

  mlmeReq.Type = MLME_DEVICE_TIME;

  if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
  {
    return LORAWAN_ED_STATUS_SUCCESS;
  }
  else
  {
    return LORAWAN_ED_STATUS_ERROR;
  }
}
#else
static lorawan_ed_error_status_t lorawan_ed_beacon_time_req( void)
{
  MlmeReq_t mlmeReq;

  mlmeReq.Type = MLME_BEACON_TIMING;

  if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
  {
    return LORAWAN_ED_STATUS_SUCCESS;
  }
  else
  {
    return LORAWAN_ED_STATUS_ERROR;
  }
}
#endif

static lorawan_ed_error_status_t lorawan_ed_beacon_req( void)
{
  MlmeReq_t mlmeReq;

  mlmeReq.Type = MLME_BEACON_ACQUISITION;

  if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
  {
    return LORAWAN_ED_STATUS_SUCCESS;
  }
  else
  {
    return LORAWAN_ED_STATUS_ERROR;
  }
}

static lorawan_ed_error_status_t lorawan_ed_ping_slot_req( void)
{

  MlmeReq_t mlmeReq;

  mlmeReq.Type = MLME_LINK_CHECK;
  LoRaMacMlmeRequest( &mlmeReq );

  mlmeReq.Type = MLME_PING_SLOT_INFO;
  mlmeReq.Req.PingSlotInfo.PingSlot.Fields.Periodicity = LORAWAN_ED_STACK_DEFAULT_PING_SLOT_PERIODICITY;
  mlmeReq.Req.PingSlotInfo.PingSlot.Fields.RFU = 0;

  if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
  {
      return LORAWAN_ED_STATUS_SUCCESS;
  }
  else
  {
     return LORAWAN_ED_STATUS_ERROR;
  }
}
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */

lorawan_ed_error_status_t lorawan_ed_request_device_class( DeviceClass_t newClass )
{
    lorawan_ed_error_status_t Errorstatus = LORAWAN_ED_STATUS_SUCCESS;
    MibRequestConfirm_t mibReq;
    DeviceClass_t currentClass;

    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm( &mibReq );

    currentClass = mibReq.Param.Class;
    /*attempt to swicth only if class update*/
    if (currentClass != newClass)
    {
        switch (newClass)
        {
            case CLASS_A:
            {
                mibReq.Param.Class = CLASS_A;
                if( LoRaMacMibSetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
                {
                    /*switch is instantanuous*/
                    lorawan_ed_app_callback->lorawan_ed_confirm_device_class(CLASS_A);
                }
                else
                {
                    Errorstatus = LORAWAN_ED_STATUS_ERROR;
                }
                break;
            }
            case CLASS_B:
            {
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
                if (currentClass != CLASS_A)
                {
                    Errorstatus = LORAWAN_ED_STATUS_ERROR;
                }
                /*switch is not instantanuous*/
                Errorstatus = lorawan_ed_beacon_req( );
#else
                LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,
                        "warning: LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B has not been defined at compilation\r\n");
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
                break;
            }
            case CLASS_C:
            {
                if (currentClass != CLASS_A)
                {
                    Errorstatus = LORAWAN_ED_STATUS_ERROR;
                }
                /*switch is instantanuous*/
                mibReq.Param.Class = CLASS_C;
                if( LoRaMacMibSetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
                {
                    lorawan_ed_app_callback->lorawan_ed_confirm_device_class(CLASS_C);
                }
                else
                {
                    Errorstatus = LORAWAN_ED_STATUS_ERROR;
                }
                break;
            }
            default:
            break;
        } 
    }
    return Errorstatus;
}

static void uplink_frame_info_trace(McpsConfirm_t *mcpsConfirm)
{
    MibRequestConfirm_t mibGet;
    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm( &mibReq );
  
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"[CFM_STAT] UplinkCounter = %d, CH = %d, DR = %d, Class = %c, Port = %d, Data Size = %d, PWR = %d, ToA = %d", \
                             mcpsConfirm->UpLinkCounter, \
                             mcpsConfirm->Channel, \
                             mcpsConfirm->Datarate, \
                             "ABC"[mibReq.Param.Class], \
                             lorawan_ed_app_data.Port, \
                             lorawan_ed_app_data.BuffSize, \
                             mcpsConfirm->TxPower,\
                             mcpsConfirm->TxTimeOnAir);
    
    mibGet.Type  = MIB_CHANNELS_MASK;
    if( LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK )
    {
#if defined( REGION_AS923 ) || defined( REGION_CN779 ) || \
    defined( REGION_EU868 ) || defined( REGION_IN865 ) || \
    defined( REGION_KR920 ) || defined( REGION_EU433 ) || \
    defined( REGION_RU864 )

        //for( uint8_t i = 0; i < 1; i++)
        LORAWAN_ED_DEBUG_LOG(LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Channel Mask:%04X-%04X-%04X-%04X\r\n", mibGet.Param.ChannelsMask[0],
                                           mibGet.Param.ChannelsMask[1],
                                           mibGet.Param.ChannelsMask[2],
                                           mibGet.Param.ChannelsMask[3]);
        
#elif defined( REGION_AU915 ) || defined( REGION_US915 ) || defined( REGION_CN470 ) || defined( REGION_CN470S )

        //for( uint8_t i = 0; i < 5; i++)
        LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL, "Channel Mask:%04X-%04X-%04X-%04X-%04X-%04X\r\n", mibGet.Param.ChannelsMask[0],
                                           mibGet.Param.ChannelsMask[1],
                                           mibGet.Param.ChannelsMask[2],
                                           mibGet.Param.ChannelsMask[3],
                                           mibGet.Param.ChannelsMask[4],
                                           mibGet.Param.ChannelsMask[5] );
#else

#error "Please define a region in the compiler options."

#endif
    }
} 

static void downlink_frame_info_trace(McpsIndication_t *mcpsIndication)
{
    const char *slotStrings[] = { "1", "2", "C", "Ping-Slot", "Multicast Ping-Slot" };
  
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"[IND_STAT] DonwlinkCounter = %lu, DR = %d, Rx%s, Port = %d, Data size = %d, Rssi = %d, Snr = %d, FramePending = %d, Multicast = %d\r\n", \
                             mcpsIndication->DownLinkCounter, \
                             mcpsIndication->RxDatarate, \
                             slotStrings[mcpsIndication->RxSlot], \
                             mcpsIndication->Port, \
                             mcpsIndication->BufferSize, \
                             mcpsIndication->Rssi, \
                             mcpsIndication->Snr,  \
                             mcpsIndication->FramePending, \
                             mcpsIndication->Multicast);
}  

#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
static void beacon_Info_trace(MlmeIndication_t *mlmeIndication)
{
    int32_t snr = 0;
    if( mlmeIndication->BeaconInfo.Snr & 0x80 ) // The SNR sign bit is 1
    {
        // Invert and divide by 4
        snr = ( ( ~mlmeIndication->BeaconInfo.Snr + 1 ) & 0xFF ) >> 2;
        snr = -snr;
    }
    else
    {
        // Divide by 4
        snr = ( mlmeIndication->BeaconInfo.Snr & 0xFF ) >> 2;
    }  
    
    LORAWAN_ED_DEBUG_LOG( LORAWAN_ED_STACK_DEBUG_APS, DBG_LVL,"\r\n#= BEACON %lu =#, GW desc %d, rssi %d, snr %ld\r\n", \
                             mlmeIndication->BeaconInfo.Time, \
                             mlmeIndication->BeaconInfo.GwSpecific.InfoDesc, \
                             mlmeIndication->BeaconInfo.Rssi, \
                             snr );
}
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
