/*!
 * \file      lora-radio-test-shell.c
 *
 * \brief     lora radio shell for test implementation
 *
 * \copyright SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */
#include "lora-radio-rtos-config.h"
#include "lora-radio.h"
#include "lora-radio-timer.h"
#include "lora-radio-test-shell.h"

#define LOG_TAG "APP.LoRa.Radio.Shell"
#define LOG_LEVEL  LOG_LVL_INFO
#include "lora-radio-debug.h"

#define TX_LED_TOGGLE
#define TX_LED_ON
#define TX_LED_OFF

#define RX_LED_TOGGLE
#define RX_LED_ON
#define RX_LED_OFF

static struct rt_event radio_event;

const static uint8_t PingMsg[] = "PING";

static uint16_t BufferSize = BUFFER_SIZE;
static uint8_t Buffer[BUFFER_SIZE];

static int16_t rssi_value = -255;
static int16_t rssi_value_min = -255;
static int16_t rssi_value_max = -255;
static int32_t rssi_value_total = 0;

static int8_t snr_value = -128;
static int8_t snr_value_min = -128;
static int8_t snr_value_max = -128;
static int32_t snr_value_total = 0;

static uint32_t master_address = LORA_MASTER_DEVADDR;
static uint32_t slaver_address = LORA_SLAVER_DEVADDR;
static uint32_t payload_len = 32;

static uint32_t tx_seq_cnt = 0;
static uint16_t max_tx_nbtrials = 10;
static uint32_t rx_correct_cnt = 0;
static uint32_t rx_error_cnt = 0;
static uint32_t rx_timeout_cnt = 0;

static uint32_t tx_timestamp;
static uint32_t rx_timestamp;

static uint32_t rx_timeout = RX_TIMEOUT_VALUE;
static uint8_t lora_chip_initialized;
static bool master_flag = true;
static bool rx_only_flag = false;

static lora_radio_test_t lora_radio_test_paras = 
{
    .frequency = RF_FREQUENCY,
    .txpower   = TX_OUTPUT_POWER,
    
    // lora
    .modem     = MODEM_LORA,
    .sf        = LORA_SPREADING_FACTOR,
    .bw        = LORA_BANDWIDTH,
    .cr        = LORA_CODINGRATE,
    
     // FSK
    .fsk_bandwidth = FSK_BANDWIDTH,
};
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * lora radio test thread
 */
static rt_thread_t lora_radio_test_thread = RT_NULL; 

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
static void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
static void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
static void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
static void OnRxError( void );

/*!
 * \brief thread for ping ping
 */
static void lora_radio_test_thread_entry(void* parameter);

static void OnTxDone( void )
{
    Radio.Sleep( );
    rt_event_send(&radio_event, EV_RADIO_TX_DONE);
}

static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    rt_memcpy( Buffer, payload, BufferSize );
    rssi_value = rssi;
    snr_value = snr;
 
    // first rxdone
    if( rssi_value_max == -255 )
    {
        rssi_value_min = rssi_value_max = rssi;
    }
    if( snr_value_max == -128 )
    {
        snr_value_min = snr_value_max = snr;
    }
    
    // update
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
    rx_timestamp = TimerGetCurrentTime();
    
    rt_event_send(&radio_event, EV_RADIO_RX_DONE);
}

static void OnTxTimeout( void )
{
    Radio.Sleep( );
    rt_event_send(&radio_event, EV_RADIO_TX_TIMEOUT);
}

static void OnRxTimeout( void )
{
    Radio.Sleep( );
    rx_timestamp = TimerGetCurrentTime();
    rt_event_send(&radio_event, EV_RADIO_RX_TIMEOUT);
}

static void OnRxError( void )
{
    Radio.Sleep( );
    rt_event_send(&radio_event, EV_RADIO_RX_ERROR);
}

static void send_ping_packet(uint32_t src_addr,uint32_t dst_addr,uint8_t len)
{
    tx_seq_cnt++;
                            
    tx_timestamp = TimerGetCurrentTime();
    
    // Send the next PING frame
    uint8_t index = 0;
    
    // header 
    Buffer[index++] = 0x00; // echo cmd
    
    Buffer[index++] = src_addr & 0xFF;
    Buffer[index++] = src_addr >> 8;
    Buffer[index++] = src_addr >> 16;
    Buffer[index++] = src_addr >> 24;

    Buffer[index++] = dst_addr & 0xFF;
    Buffer[index++] = dst_addr >> 8;
    Buffer[index++] = dst_addr >> 16;
    Buffer[index++] = dst_addr >> 24;
 
    Buffer[index++] = tx_seq_cnt & 0xFF;
    Buffer[index++] = tx_seq_cnt >> 8;
    Buffer[index++] = tx_seq_cnt >> 16;
    Buffer[index++] = tx_seq_cnt >> 24;    
    
    // data
    Buffer[index++] = 'P';
    Buffer[index++] = 'I';
    Buffer[index++] = 'N';
    Buffer[index++] = 'G';
    
    // 00,01,02...
    for( uint8_t i = 0; i < len - index ; i++)
    {
        Buffer[index + i] = i;
    }
    rt_thread_mdelay(1);
    Radio.Send( Buffer, len );
}

static bool lora_radio_test_init(void)
{
    if( lora_chip_initialized == false )
    {
        // Target board initialization

        if( lora_radio_test_thread == RT_NULL )
        {
            rt_event_init(&radio_event, "ev_lora_test", RT_IPC_FLAG_FIFO);

            lora_radio_test_thread = rt_thread_create("lora-radio-test",
                                                        lora_radio_test_thread_entry, 
                                                        RT_NULL,
                                                        8096, 
                                                        2, 
                                                        10);
            if (lora_radio_test_thread != RT_NULL)
            {
                rt_thread_startup(lora_radio_test_thread);
            }
            else
                rt_kprintf("lora radio test thread create failed!\n");
        }
        
        /* Radio initialization */
        RadioEvents.TxDone = OnTxDone;
        RadioEvents.RxDone = OnRxDone;
        RadioEvents.TxTimeout = OnTxTimeout;
        RadioEvents.RxTimeout = OnRxTimeout;
        RadioEvents.RxError = OnRxError;

        if(Radio.Init(&RadioEvents))
        {
            /* setup private syncword for p2p test */
            Radio.SetPublicNetwork( false );
            
            lora_chip_initialized = true;
        }
        else
        {
            rt_kprintf("lora radio Init failed!\n");
            
            return false;
        }
    }
    return true;
}

static void radio_rx(void)
{
    rt_uint32_t timeout = 0; 
    if( master_flag == true )
    {
        timeout = rx_timeout;
    }
    Radio.Rx( timeout );
}

static void lora_radio_test_thread_entry(void* parameter)  
{
    rt_uint32_t ev = 0;

    while( 1 )                          
    {
        if (rt_event_recv(&radio_event, EV_RADIO_ALL,
                                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                        RT_WAITING_FOREVER, &ev) == RT_EOK)
        {
            switch( ev )
            {
                case EV_RADIO_INIT:
                    
                    Radio.SetChannel( lora_radio_test_paras.frequency );

                    if( lora_radio_test_paras.modem == MODEM_LORA )
                    {
                        Radio.SetTxConfig( MODEM_LORA, lora_radio_test_paras.txpower, 0, lora_radio_test_paras.bw,
                                                       lora_radio_test_paras.sf, lora_radio_test_paras.cr,
                                                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE,
                                                       true, 0, 0, LORA_IQ_INVERSION_ON_DISABLE, 3000 );

                        Radio.SetRxConfig( MODEM_LORA, lora_radio_test_paras.bw, lora_radio_test_paras.sf,
                                                       lora_radio_test_paras.cr, 0, LORA_PREAMBLE_LENGTH,
                                                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE,
                                                       0, true, 0, 0, LORA_IQ_INVERSION_ON_DISABLE, true );                                                                              
                    }
                    else
                    {
                        Radio.SetTxConfig( MODEM_FSK, lora_radio_test_paras.txpower, FSK_FDEV, 0,
                                                      FSK_DATARATE, 0,
                                                      FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                                      true, 0, 0, 0, 3000 );

                        Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                                      0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                                      0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                                      0, 0,false, true );
                    }

                    /* initlize the statistics parameters */
                    rssi_value = -255;
                    rssi_value_min = -255;
                    rssi_value_max = -255;
                    rssi_value_total = 0;
                    snr_value = -128;
                    snr_value_min = -128;
                    snr_value_max = -128;
                    snr_value_total = 0;
                
                    tx_seq_cnt = 0;
                    rx_timeout_cnt = 0;
                    rx_error_cnt = 0;
                    rx_correct_cnt = 0;

                    if( master_flag == 0 )
                    {
                        if( rx_only_flag == false )
                        {
                            rt_kprintf("Slaver Address(SA):[0x%X]\n",slaver_address);
                        }
                        rt_kprintf("Stay to Rx Continuous with freq=%d, SF=%d, CR=%d, BW=%d\n", lora_radio_test_paras.frequency, lora_radio_test_paras.sf, lora_radio_test_paras.cr, lora_radio_test_paras.bw);
                        
                        Radio.Rx( 0 );
                    }
                    else
                    {
                        rt_event_send(&radio_event, EV_RADIO_TX_START);
                    }
                    break;
                  
                case EV_RADIO_RX_DONE:
                    if( master_flag == true )
                    {
                        if( BufferSize > 0 )
                        {
                            if( rt_strncmp( ( const char* )Buffer + MAC_HEADER_OVERHEAD, ( const char* )PingMsg, 4 ) == 0 )
                            {
                                /* Indicates on a LED that the received frame is a PING ACK packet */
                                RX_LED_TOGGLE;
                                rx_correct_cnt++;

                                uint32_t slaver_addr = 0;
                                slaver_addr = Buffer[5];
                                slaver_addr |= Buffer[6] << 8;
                                slaver_addr |= Buffer[7] << 16;
                                slaver_addr |= Buffer[8] << 24;

                                uint32_t received_seqno = 0;
                                received_seqno = Buffer[9];
                                received_seqno |= Buffer[10] << 8;
                                received_seqno |= Buffer[11] << 16;
                                received_seqno |= Buffer[12] << 24;
                                
                               rt_kprintf("Reply from [0x%X]:seqno=%d, bytes=%d,total time=%d ms,rssi=%d,snr=%d\n",slaver_addr, received_seqno, BufferSize,( rx_timestamp - tx_timestamp ),rssi_value,snr_value );
                             
                               /* Send the next PING frame */
                               rt_event_send(&radio_event, EV_RADIO_TX_START);
                               break;
                            }
                            else /* valid reception but neither a PING ACK */
                            {   
                                Radio.Rx( RX_TIMEOUT_VALUE );
                            }
                        }
                        else
                        {
                            rt_kprintf("RX ERR:BufferSize = 0");
                        }
                    }
                    else
                    {
                        if( BufferSize > 0 )
                        {
                            #define DST_ADDRESS_OFFSET 5
                            uint32_t dst_address = Buffer[DST_ADDRESS_OFFSET] | \
                                                 ( Buffer[DST_ADDRESS_OFFSET+1] << 8 ) |\
                                                 ( Buffer[DST_ADDRESS_OFFSET+2] << 16 )|\
                                                 ( Buffer[DST_ADDRESS_OFFSET+3] << 24); 

                            if( rx_only_flag == true )
                            {
                                /* Indicates on a LED that the received frame */
                                RX_LED_TOGGLE;
                            
                                uint32_t src_addr,dst_addr = 0;
                                src_addr = Buffer[1];
                                src_addr |= Buffer[2] << 8;
                                src_addr |= Buffer[3] << 16;
                                src_addr |= Buffer[4] << 24;

                                dst_addr = Buffer[5];
                                dst_addr |= Buffer[6] << 8;
                                dst_addr |= Buffer[7] << 16;
                                dst_addr |= Buffer[8] << 24;
                                
                                rx_correct_cnt++;
                                
                                /* RX continuous */
                                Radio.Rx( 0 ); 
              
                                rt_kprintf("Received: Totals=%d,bytes=%d,timestamp=%d ms,rssi=%d,snr=%d\n",rx_correct_cnt, BufferSize,rx_timestamp,rssi_value,snr_value );

#ifdef RT_USING_ULOG
                                ulog_hexdump(LOG_TAG,16,Buffer,BufferSize);
#endif
                            }   
                            else if( ( dst_address == slaver_address || dst_address == 0xFFFFFFFF ) && \
                                ( rt_strncmp( ( const char* )Buffer + MAC_HEADER_OVERHEAD, ( const char* )PingMsg, 4 ) == 0 ))
                            {
                                /* Indicates on a LED that the received frame is a PING */
                                RX_LED_TOGGLE;
                                /* echo the receive packet to master */
                                {
                                    rt_thread_mdelay(1);
                                    Radio.Send( Buffer, BufferSize );
                                }
                            }                            
                            else /* valid reception but not a PING as expected */
                            {    
                                Radio.Rx( 0 ); 
                            }
                        }
                    }
                    break;
            case EV_RADIO_TX_DONE:
                /* Indicates on a LED that we have sent a PING [Master] */
                /* Indicates on a LED that we have sent a PING ACK [Slave] */
                TX_LED_TOGGLE;
                radio_rx();

                break;
            case EV_RADIO_RX_TIMEOUT:
                rx_timeout_cnt++;
                rt_kprintf("Request [SA=0x%X] timed out: seqno=%d, time=%d ms\n", slaver_address, tx_seq_cnt, ( rx_timestamp - tx_timestamp ) );
             case EV_RADIO_RX_ERROR:
             case EV_RADIO_TX_START:
                    if( master_flag == true )
                    {
                        /* tx_seq_cnt start from 0 */
                        if( tx_seq_cnt < max_tx_nbtrials ) 
                        {
                            /* for first time to printf some info */
                            if( !tx_seq_cnt ) 
                            {
                                uint32_t packet_toa = Radio.TimeOnAir(lora_radio_test_paras.modem,lora_radio_test_paras.bw,lora_radio_test_paras.sf,lora_radio_test_paras.cr,LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE,payload_len,true);
                                rt_kprintf("Master Address(MA):[0x%X]\n",master_address);
                                rt_kprintf("Pinging [SA=0x%X] with %d bytes(ToA=%d ms) of data for %d counters:\n", slaver_address, payload_len, packet_toa, max_tx_nbtrials);
                                rt_kprintf("With radio parameters: freq=%d, TxPower=%d, SF=%d, CR=%d, BW=%d\n", lora_radio_test_paras.frequency, lora_radio_test_paras.txpower, lora_radio_test_paras.sf, lora_radio_test_paras.cr, lora_radio_test_paras.bw);
                            }
    
                            send_ping_packet(master_address,slaver_address,payload_len);
                        }
                        else
                        {
                            uint16_t per = 100 - ( (float) rx_correct_cnt / tx_seq_cnt ) * 100;
                            uint32_t tx_total_byte = tx_seq_cnt * ( payload_len + MAC_HEADER_OVERHEAD );
                            uint32_t tx_total_kbyte_integer = tx_total_byte >> 10;   // / 1024
                            uint32_t tx_total_kbyte_decimal = tx_total_byte & 0x3FF; // % 1024
                            
                            uint32_t rx_total_byte = rx_correct_cnt * ( payload_len + MAC_HEADER_OVERHEAD );
                            uint32_t rx_total_kbyte_integer = rx_total_byte >> 10;   // / 1024
                            uint32_t rx_total_kbyte_decimal = rx_total_byte & 0x3FF; // % 1024
                            int32_t avg_rssi = -255;
                            int32_t avg_snr = -128;
                            if( rx_correct_cnt )
                            {
                                avg_rssi = rssi_value_total / (int32_t)rx_correct_cnt;
                                avg_snr = snr_value_total / (int32_t)rx_correct_cnt;
                            }
                            /* wait for PHY log output done */
                            rt_thread_mdelay(10);
                            rt_kprintf("\r\n====== LoRa Ping statistics for [MA=0x%X <-> SA=0x%X] with [TxPower=%d,SF=%d] ======\n",master_address, slaver_address, lora_radio_test_paras.txpower, lora_radio_test_paras.sf);
                            rt_kprintf("-> Tx pakcets: sent = %d, tx_total = %d.%d KByte\n",tx_seq_cnt, tx_total_kbyte_integer, tx_total_kbyte_decimal);
                            rt_kprintf("-> Rx pakcets: received = %d, lost = %d, per = %d%, rx_total = %d.%d KByte\n",rx_correct_cnt, rx_timeout_cnt + rx_error_cnt, per,rx_total_kbyte_integer,rx_total_kbyte_decimal);
                            rt_kprintf("--> Rx rssi: max_rssi = %d, min_rssi = %d, avg_rssi = %d\n",rssi_value_max,rssi_value_min,avg_rssi);
                            rt_kprintf("--> Rx snr: max_snr  = %d, min_snr  = %d, avg_snr  = %d\n",snr_value_max,snr_value_min,avg_snr);
                            rt_kprintf("====== LoRa Ping Test Finished ======\r\n");
                        }
                    }
                    else
                    {
                        Radio.Rx( 0 );
                    }
                    break;
                case EV_RADIO_TX_TIMEOUT:
                    radio_rx();
                    break;  
            }
        }
    } 
}

/* for finish\msh */
#define CMD_LORA_CHIP_PROBE_INDEX        0 // LoRa Chip probe
#define CMD_LORA_CHIP_CONFIG_INDEX       1 // tx cw
#define CMD_TX_CW_INDEX                  2 // tx cw
#define CMD_PING_INDEX                   3 // ping-pong
#define CMD_RX_PACKET_INDEX              4 // rx packet only

const char* lora_help_info[] = 
{
    [CMD_LORA_CHIP_PROBE_INDEX]       = "lora probe             - lora radio probe",
    [CMD_LORA_CHIP_CONFIG_INDEX]      = "lora config<modem>     - lora radio config parameters", 
    [CMD_TX_CW_INDEX]                 = "lora cw <freq>,<power> - tx carrier wave",
    [CMD_PING_INDEX]                  = "lora ping <para1>      - ping <-m: master,-s: slaver>",   
    [CMD_RX_PACKET_INDEX]             = "lora rx <timeout>      - rx data only(sniffer)",
};

/* LoRa Radio Test Shell */
static int lora(int argc, char *argv[])
{
    size_t i = 0;
    
    if (argc < 2)
    {   
        /* parameter error */
        rt_kprintf("Usage:\n");
        for (i = 0; i < sizeof(lora_help_info) / sizeof(char*); i++) 
        {
            rt_kprintf("%s\n", lora_help_info[i]);
        }
        rt_kprintf("\n");
    } 
    else 
    {
        const char *cmd = argv[1];

        if( lora_radio_test_init() == false )
        {
            rt_kprintf("LoRa Chip Init Failed\n");
            return 0;
        }

        if (!rt_strcmp(cmd, "probe")) 
        {   
            rt_kprintf("LoRa Chip start to test\n");

            if( Radio.Check() )
            {
                rt_kprintf("LoRa Chip Probe ok!\n");
            }
            else
            {
                rt_kprintf("LoRa Chip Probe failed!\n!");
            }
        }
        else if (!rt_strcmp(cmd, "cw")) 
        {
            uint8_t timeout = 0;
            if (argc >= 3) 
            {
                lora_radio_test_paras.frequency = atol(argv[2]);
            }
            if (argc >= 4) 
            {
                lora_radio_test_paras.txpower = atol(argv[3]);
            }
            if (argc >= 5) 
            {
                timeout = atol(argv[4]);
            }
            Radio.SetTxContinuousWave( lora_radio_test_paras.frequency, lora_radio_test_paras.txpower, timeout);
        }
        else if (!rt_strcmp(cmd, "ping")) 
        {       
            /* default for slaver */
            master_flag  = false;  
            rx_only_flag = false;
            
            if (argc >= 3) 
            {   
                 const char *cmd1 = argv[2];
                 if (!rt_strcmp(cmd1, "-m")) 
                 {
                     master_flag = true;
                }
                else // -s
                {
                    master_flag = false;
                }
                 
                if( argc >=4 )
                 {
                    /* max_tx_nbtrials for setup tx counter */
                    max_tx_nbtrials = atol(argv[3]);
                 }
            } 

            rt_event_send(&radio_event, EV_RADIO_INIT);
        }
        else if( !rt_strcmp(cmd, "rx"))
        {   
            /* eg: lora rx 1 0 */
            master_flag = false;
            rx_only_flag = true;
            
            if (argc >= 3) 
            {
                rx_only_flag = atol(argv[2]);
            }
            if (argc >= 4) 
            {
                rx_timeout = atol(argv[3]);
            }
            
            rt_event_send(&radio_event, EV_RADIO_INIT);
        }
        else if (!rt_strcmp(cmd, "config"))
        {
            /* config radio paramters, such as frequency,txPower,sf,bw...*/
            if (argc >= 3) 
            {
                lora_radio_test_paras.frequency = atol(argv[2]);
            }
            if (argc >= 4) 
            {
                lora_radio_test_paras.txpower = atol(argv[3]);
            }
            if (argc >= 5) 
            {
                lora_radio_test_paras.sf = atol(argv[4]);
            }
            if (argc >= 6) 
            {
                lora_radio_test_paras.bw = atol(argv[5]);
            }
            
            rt_kprintf("Frequency: %d\n",lora_radio_test_paras.frequency);
        
            rt_kprintf("TxPower  : %d\n",lora_radio_test_paras.txpower);

            rt_kprintf("SF: %d\n",lora_radio_test_paras.sf);
            
            rt_kprintf("BW: %d\n",lora_radio_test_paras.bw);
        }
    }
    return 1;
}
MSH_CMD_EXPORT(lora, lora radio test shell);

