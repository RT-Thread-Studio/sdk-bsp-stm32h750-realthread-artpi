 /*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: LoRaMac classA device certificate test implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis, Gregory Cristian and Wael Guibene
*/

/* Includes ------------------------------------------------------------------*/

#include "lora-radio-timer.h"
#include "LoRaMac.h"
#include "LoRaMacTest.h"
#include "lorawan-ed-aps.h"
#include "lorawan-ed-certificate.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct ComplianceTest_s
{
    bool Running;
    uint8_t State;
    uint8_t IsTxConfirmed;
    uint8_t DataBufferSize;
    uint8_t DataBuffer[242];
    uint16_t DownLinkCounter;
    bool LinkCheck;
    uint8_t DemodMargin;
    uint8_t NbGateways;
}ComplianceTest_t;

/* Private define ------------------------------------------------------------*/
#define CERTIFICATE_TEST_TX_DUTYCYCLE 5000
/* Private variables ---------------------------------------------------------*/

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t certificate_tx_next_packet_timer;
static ComplianceTest_t certificate_param;
static uint8_t IsTxConfirmed;
static bool AdrEnableInit;

/* Private functions ---------------------------------------------------------*/

static void on_certificate_tx_next_packet_timerEvent( void );
static bool lorawan_ed_certificate_tx( void );

/* Exported functions definition---------------------------------------------------------*/
bool lorawan_ed_certificate_running(void)
{
    return certificate_param.Running;
}

void lorawan_ed_certificate_downlink_increment( void )
{
    certificate_param.DownLinkCounter++;
}

void lorawan_ed_certificate_linkcheck(MlmeConfirm_t *mlmeConfirm)
{
    certificate_param.LinkCheck = true;
    certificate_param.DemodMargin = mlmeConfirm->DemodMargin;
    certificate_param.NbGateways = mlmeConfirm->NbGateways;
}

static bool lorawan_ed_certificate_tx( void )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;

    if( certificate_param.LinkCheck == true )
    {
        certificate_param.LinkCheck = false;
        certificate_param.DataBufferSize = 3;
        certificate_param.DataBuffer[0] = 5;
        certificate_param.DataBuffer[1] = certificate_param.DemodMargin;
        certificate_param.DataBuffer[2] = certificate_param.NbGateways;
        certificate_param.State = 1;
    }
    else
    {
        switch( certificate_param.State )
        {
            case 4:
                certificate_param.State = 1;
                break;
            case 1:
                certificate_param.DataBufferSize = 2;
                certificate_param.DataBuffer[0] = certificate_param.DownLinkCounter >> 8;
                certificate_param.DataBuffer[1] = certificate_param.DownLinkCounter;
                break;
        }
    }

    if( LoRaMacQueryTxPossible( certificate_param.DataBufferSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = DR_0;
    }
    else
    {
        if( IsTxConfirmed == LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = LORAWAN_ED_CETIFICATE_TEST_FPORT;
            mcpsReq.Req.Unconfirmed.fBufferSize = certificate_param.DataBufferSize;
            mcpsReq.Req.Unconfirmed.fBuffer = &(certificate_param.DataBuffer);
            mcpsReq.Req.Unconfirmed.Datarate = DR_0;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = LORAWAN_ED_CETIFICATE_TEST_FPORT;
            mcpsReq.Req.Confirmed.fBufferSize = certificate_param.DataBufferSize;
            mcpsReq.Req.Confirmed.fBuffer = &(certificate_param.DataBuffer);
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = DR_0;
        }
    }

    /*cerification test on-going*/
    TimerStart( &certificate_tx_next_packet_timer );

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}

void lorawan_ed_certificate_rx( McpsIndication_t *mcpsIndication, MlmeReqJoin_t* JoinParameters)
{
    if( certificate_param.Running == false )
    {
        // Check compliance test enable command (i)
        if( ( mcpsIndication->BufferSize == 4 ) &&
          ( mcpsIndication->Buffer[0] == 0x01 ) &&
          ( mcpsIndication->Buffer[1] == 0x01 ) &&
          ( mcpsIndication->Buffer[2] == 0x01 ) &&
          ( mcpsIndication->Buffer[3] == 0x01 ) )
        {
            MibRequestConfirm_t mibReq;
            IsTxConfirmed = LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED;
            certificate_param.DataBufferSize = 2;
            certificate_param.DownLinkCounter = 0;
            certificate_param.LinkCheck = false;
            certificate_param.DemodMargin = 0;
            certificate_param.NbGateways = 0;
            certificate_param.Running = true;
            certificate_param.State = 1;

            mibReq.Type = MIB_ADR;

            LoRaMacMibGetRequestConfirm( &mibReq );
            AdrEnableInit=mibReq.Param.AdrEnable;

            mibReq.Type = MIB_ADR;
            mibReq.Param.AdrEnable = true;
            LoRaMacMibSetRequestConfirm( &mibReq );

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
            LoRaMacTestSetDutyCycleOn( false );
#endif

            TimerInit( &certificate_tx_next_packet_timer, on_certificate_tx_next_packet_timerEvent );
            TimerSetValue( &certificate_tx_next_packet_timer, CERTIFICATE_TEST_TX_DUTYCYCLE); 

            /*confirm test mode activation */
            lorawan_ed_certificate_tx( );
        }
    }
    else
    {
        certificate_param.State = mcpsIndication->Buffer[0];
        switch( certificate_param.State )
        {
            case 0: // Check compliance test disable command (ii)
            {
                certificate_param.DownLinkCounter = 0;
                certificate_param.Running = false;

                MibRequestConfirm_t mibReq;
                mibReq.Type = MIB_ADR;
                mibReq.Param.AdrEnable = AdrEnableInit;
                LoRaMacMibSetRequestConfirm( &mibReq );
#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
                LoRaMacTestSetDutyCycleOn( true );
#endif
                break;
            }
            case 1: // (iii, iv)
                certificate_param.DataBufferSize = 2;
                break;
            case 2: // Enable confirmed messages (v)
                IsTxConfirmed = LORAWAN_ED_DATA_MESSAGE_TYPE_CONFIRMED;
                certificate_param.State = 1;
              break;
            case 3:  // Disable confirmed messages (vi)
                IsTxConfirmed = LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED;
                certificate_param.State = 1;
              break;
            case 4: // (vii)
              certificate_param.DataBufferSize = mcpsIndication->BufferSize;

              certificate_param.DataBuffer[0] = 4;
              for( uint8_t i = 1; i < certificate_param.DataBufferSize; i++ )
              {
                  certificate_param.DataBuffer[i] = mcpsIndication->Buffer[i] + 1;
              }
              break;
            case 5: // (viii)
            {
                MlmeReq_t mlmeReq;
                mlmeReq.Type = MLME_LINK_CHECK;
                LoRaMacMlmeRequest( &mlmeReq );
                break;
            }      
            case 6: // (ix)
            {
                MlmeReq_t mlmeReq;

                // Disable TestMode and revert back to normal operation

                certificate_param.DownLinkCounter = 0;
                certificate_param.Running = false;

                MibRequestConfirm_t mibReq;
                mibReq.Type = MIB_ADR;
                mibReq.Param.AdrEnable = AdrEnableInit;
                LoRaMacMibSetRequestConfirm( &mibReq );

                mlmeReq.Type = MLME_JOIN;
                mlmeReq.Req.Join = *JoinParameters;

                LoRaMacMlmeRequest( &mlmeReq );
                break;
            }

            case 7: // (x)
            {
              if( mcpsIndication->BufferSize == 3 )
              {
                  MlmeReq_t mlmeReq;
                  mlmeReq.Type = MLME_TXCW;
                  mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                  LoRaMacMlmeRequest( &mlmeReq );
              }
              else if( mcpsIndication->BufferSize == 7 )
              {
                  MlmeReq_t mlmeReq;
                  mlmeReq.Type = MLME_TXCW_1;
                  mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                  mlmeReq.Req.TxCw.Frequency = ( uint32_t )( ( mcpsIndication->Buffer[3] << 16 ) | ( mcpsIndication->Buffer[4] << 8 ) | mcpsIndication->Buffer[5] ) * 100;
                  mlmeReq.Req.TxCw.Power = mcpsIndication->Buffer[6];
                  LoRaMacMlmeRequest( &mlmeReq );
              }
              certificate_param.State = 1;
              break;
            }

            case 8: // Send DeviceTimeReq
            {
                MlmeReq_t mlmeReq;

                mlmeReq.Type = MLME_DEVICE_TIME;

                LoRaMacMlmeRequest( &mlmeReq );
                break;
            }
#ifdef LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B
            case 9: // Switch end device Class
            {
                MibRequestConfirm_t mibReq;

                mibReq.Type = MIB_DEVICE_CLASS;
                // CLASS_A = 0, CLASS_B = 1, CLASS_C = 2
                mibReq.Param.Class = ( DeviceClass_t )mcpsIndication->Buffer[1];
                LoRaMacMibSetRequestConfirm( &mibReq );
                break;
            }
            case 10: // Send PingSlotInfoReq
            {
                MlmeReq_t mlmeReq;

                mlmeReq.Type = MLME_PING_SLOT_INFO;

                mlmeReq.Req.PingSlotInfo.PingSlot.Value = mcpsIndication->Buffer[1];

                LoRaMacMlmeRequest( &mlmeReq );
                break;
            }
            case 11: // Send BeaconTimingReq
            {
                MlmeReq_t mlmeReq;

                mlmeReq.Type = MLME_BEACON_TIMING;

                LoRaMacMlmeRequest( &mlmeReq );
                break;
            }
#endif /* LORAWAN_ED_STACK_USING_DEVICE_TYPE_CLASS_B */
            default:                  
                break;
        }
    }
  
    if ( certificate_param.Running == false )
    {
        /*cerification test stops*/
        TimerStop( &certificate_tx_next_packet_timer );
    }
}

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
static void on_certificate_tx_next_packet_timerEvent( void /** context */)
{
    lorawan_ed_certificate_tx( );
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

