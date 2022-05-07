/*!
 * \file      lgd-test-shell.c
 *
 * \brief     lgd(lora gw driver) shell for test implementation
 *
 * \copyright SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */
#include <stdlib.h>
#include <inttypes.h>       /* PRIx64, PRIu64... */
#include <stdio.h>          /* fprintf, snprintf, fopen, fputs */

#include "rtthread.h"
#include "loragw_usr.h"
#include "lgd_tester.h"

#define LOG_TAG "lora.gw.shell"
#include "loragw_dbg.h"

#define TX_LED_TOGGLE
#define TX_LED_ON
#define TX_LED_OFF

#define RX_LED_TOGGLE
#define RX_LED_ON
#define RX_LED_OFF

#define LGD_TIMESTAMP_TO_MS(x) ((x) * 1000 / RT_TICK_PER_SECOND)
#define LGD_TEST_FETCH_RX_BUFFER_INTERVAL 50 //50ms 1tick @ 1ms

#define DST_ADDRESS_OFFSET 5

const static uint8_t PingMsg[] = "PING";

static float rssic_value = -255;
static float rssis_value = -255;

static float snr_value_avg = -128;
static float snr_value_min = -128;
static float snr_value_max = -128;

static uint32_t master_address = LORA_MASTER_DEVADDR;
static uint32_t slaver_address = LORA_SLAVER_DEVADDR;
static uint32_t payload_len = 32;

static uint32_t tx_seq_cnt = 0;
static uint16_t max_tx_nbtrials = 10;
static uint32_t rx_correct_cnt = 0;
static uint32_t rx_error_cnt = 0;
static uint32_t rx_timeout_cnt = 0;

static uint32_t rx_timestamp;
static uint32_t tx_timestamp;

static int32_t lgd_tx_frequency_offset = TX_RX_FREQUENCE_OFFSET;

static bool lgd_ack_response_flag = false;
static bool lgd_tst_initialized = false;
static bool lgd_rx_only_flag = false;

static uint8_t lgd_test_mode = LGD_TEST_AS_CONCENTRATOR_MODE;

static char *bandwidth_string[3] = {"125", "250", "500"};

/* --- PUBLIC VARIABLES ---------------------------------------------------- */
static struct lgw_conf_usr_s lgd_conf_lgw =
{
    .lorawan_public = false,/* true for lorawan */

    .rxrf =
    {
        {
            .enable  = true,
            .freq_hz = LGD_RF_CHIAN0_CENTER_FREQ,
            .invert_pol = false, /* false for lorawan(uplink)*/
        },

        {
            .enable  = true,
            .freq_hz = LGD_RF_CHIAN1_CENTER_FREQ,
            .invert_pol = false, /* false for lorawan(uplink)*/
        },
    },

    .rxif =
    {
        .channel_if_enable =  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        .channel_if_rfchain = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },

        .channel_if8_datarate  = DR_LORA_SF7, /* only for IF8 */
        .channel_if8_bandwidth = BW_250KHZ, /* only for IF8 */

        .channel_if_freq =
        {
            -300000, /* for IF0 */
                -100000, /* for IF1 */
                100000,  /* for IF2 */
                300000,  /* for IF3 */
                -300000, /* for IF4 */
                -100000, /* for IF5 */
                100000,  /* for IF6 */
                300000,  /* for IF7 */
                500000   /* for IF8 */
            },
    },

    .txrf =
    {
        .freq_hz = 475300000, /* 470~510 MHz */
        .rf_power = 22,/* 8~22dBm */
        .datarate = 7, /* SF7~SF12 */

        .modulation = MOD_LORA,/* lora */
        .invert_pol = false, /* true for lorawan downlink */
        .preamble = 8,
        .no_header = false,
        .no_crc = false, /* true for lorawan downlink */
        .bandwidth = BW_125KHZ,
        .coderate = CR_LORA_4_5,

        .tx_mode = IMMEDIATE, /* or TIMESTAMPED */
    },
};

static int32_t lgd_tst_sem_main_wait_time = RT_WAITING_FOREVER;
static int32_t lgd_tst_sem_rx_wait_time = RT_WAITING_FOREVER;

static struct rt_thread lgd_tst_main_thread;
static rt_uint32_t lgd_tst_main_stack[8192];

static rt_thread_t lgd_tst_rx_thread;

static struct rt_semaphore lgd_tst_sem_main;
static struct rt_semaphore lgd_tst_sem_rx;

static struct lgw_pkt_rx_s lgd_rxpkt[16]; /* array containing up to 16 inbound packets metadata */
static struct lgw_pkt_rx_s *lgd_rxpkt_ptr; /* pointer on a RX packet */
static struct lgw_payload_usr_s lgd_app_data;

static void lgd_tst_main_thread_entry(void* parameter);
static void lgd_tst_rx_thread_entry(void* parameter);

static int lgd_tst_lgw_init(uint8_t test_mode)
{
    int status;

    /* Stop the gateway */
    status = lgw_stop();

    if (status != LGW_HAL_SUCCESS)
    {
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "ERROR: failed to stop the gateway\n");
    }

    /* lora gw chip initialization */
    lgw_init(&lgd_conf_lgw);

    lgd_tst_sem_rx_wait_time = rt_tick_from_millisecond(LGD_TEST_FETCH_RX_BUFFER_INTERVAL);

    /* connect, configure and start the LoRa concentrator */
    status = lgw_start();

    if (status == LGW_HAL_SUCCESS)
    {
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "*** LoRa GW Driver Test started ***\n");
    }
    else
    {
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "*** Impossible to start LoRa GW Driver Test ***\n");

        return -RT_ERROR;
    }

    if(lgd_tst_rx_thread == RT_NULL)
    {
        lgd_tst_rx_thread = rt_thread_create("lgd-rx",
                                             lgd_tst_rx_thread_entry,
                                             RT_NULL,
                                             8192,
                                             6,
                                             10);

        if (lgd_tst_rx_thread != RT_NULL)
        {
            rt_thread_startup(lgd_tst_rx_thread);
        }
        else
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "lora gw test rx thread create failed!\n");
            return -RT_ERROR;
        }
    }

    if(test_mode == LGD_TEST_AS_CONCENTRATOR_MODE)
    {
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Concentrator Mode,Stay to Rx Continuous with start-freq=%d, end-freq=%d, Multi-SF=7~12, BW=%d, Public_Network:%d, IQ_Inversion:%d", lgd_conf_lgw.rxrf[0].freq_hz - 300000,
                      lgd_conf_lgw.rxrf[1].freq_hz + 300000,
                      125,
                      lgd_conf_lgw.lorawan_public,
                      lgd_conf_lgw.rxrf[0].invert_pol);
    }
    return RT_EOK;
}

bool lgd_tst_main_thread_init(void)
{
    if( lgd_tst_main_thread.entry == RT_NULL )
    {
        rt_err_t result;
        result = rt_sem_init(&lgd_tst_sem_main, "lgd_s_m", 0, RT_IPC_FLAG_FIFO);

        if (result != RT_EOK)
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "init lora-gw-main semaphore failed.\n");
        }

        result = rt_sem_init(&lgd_tst_sem_rx, "lgd_s_r", 0, RT_IPC_FLAG_FIFO);

        if (result != RT_EOK)
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "init lora-gw-rx semaphore failed.\n");
        }

        result = rt_thread_init(&lgd_tst_main_thread,
                                "lgd-shell",
                                lgd_tst_main_thread_entry,
                                RT_NULL,
                                &lgd_tst_main_stack[0],
                                sizeof(lgd_tst_main_stack),
                                7,
                                10);

        if (result == RT_EOK)
        {
            rt_thread_startup(&lgd_tst_main_thread);
        }
        else
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "lora gw test main thread create failed!\n");
            return false;
        }

        // wait for first lgw chip init
        rt_sem_take(&lgd_tst_sem_main, RT_WAITING_FOREVER);
    }

    return true;
}
//INIT_APP_EXPORT(lgd_tst_main_thread_init);

static void send_ping_packet(uint32_t src_addr, uint32_t dst_addr, uint8_t len)
{
    int tx_result;
    // Send the next PING frame
    uint8_t index = 0;

    tx_seq_cnt++;

    tx_timestamp = rt_tick_get();

    // header
    lgd_app_data.payload[index++] = 0x00; // echo cmd

    lgd_app_data.payload[index++] = src_addr & 0xFF;
    lgd_app_data.payload[index++] = src_addr >> 8;
    lgd_app_data.payload[index++] = src_addr >> 16;
    lgd_app_data.payload[index++] = src_addr >> 24;

    lgd_app_data.payload[index++] = dst_addr & 0xFF;
    lgd_app_data.payload[index++] = dst_addr >> 8;
    lgd_app_data.payload[index++] = dst_addr >> 16;
    lgd_app_data.payload[index++] = dst_addr >> 24;

    lgd_app_data.payload[index++] = tx_seq_cnt & 0xFF;
    lgd_app_data.payload[index++] = tx_seq_cnt >> 8;
    lgd_app_data.payload[index++] = tx_seq_cnt >> 16;
    lgd_app_data.payload[index++] = tx_seq_cnt >> 24;

    /* user data */
    lgd_app_data.payload[index++] = 'P';
    lgd_app_data.payload[index++] = 'I';
    lgd_app_data.payload[index++] = 'N';
    lgd_app_data.payload[index++] = 'G';

    /* 00,01,02...*/
    for( uint8_t i = 0; i < len - index ; i++)
    {
        lgd_app_data.payload[index + i] = i;
    }

    lgd_app_data.size = len;

    rt_thread_mdelay(1);

    tx_result = lgw_tx( &lgd_conf_lgw.txrf, &lgd_app_data );

    if( tx_result != LGW_HAL_SUCCESS )
    {
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "lgd ping(send) fail");
    }
}

static void rx_frame_process(struct lgw_pkt_rx_s *pkt)
{
    if( lgd_test_mode == LGD_TEST_AS_DEVICE_MODE )
    {
        if( pkt->size > 0 )
        {
            if( rt_strncmp( ( const char* )pkt->payload + MAC_HEADER_OVERHEAD, ( const char* )PingMsg, 4 ) == 0 )
            {
                /* Indicates on a LED that the received frame is a PING ACK packet */
                RX_LED_TOGGLE;
                rx_correct_cnt++;

                uint32_t slaver_addr = 0;
                slaver_addr = pkt->payload[5];
                slaver_addr |= pkt->payload[6] << 8;
                slaver_addr |= pkt->payload[7] << 16;
                slaver_addr |= pkt->payload[8] << 24;

                uint32_t received_seqno = 0;
                received_seqno = pkt->payload[9];
                received_seqno |= pkt->payload[10] << 8;
                received_seqno |= pkt->payload[11] << 16;
                received_seqno |= pkt->payload[12] << 24;

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Reply from [0x%X]:seqno=%d, bytes=%d,total time=%d ms,rssic=%+6.1f,rssis=%+6.1f,snr=%+d", slaver_addr,
                              received_seqno,
                              pkt->size,
                              LGD_TIMESTAMP_TO_MS( rx_timestamp - tx_timestamp ),
                              rssic_value, rssis_value, snr_value_avg );

                if( tx_seq_cnt )
                {
                    lgd_ack_response_flag = true;
                    /* trigger to send the next PING frame imeadiately */
                    rt_sem_release(&lgd_tst_sem_main);
                }

            }
            else /* valid reception but neither a PING ACK */
            {
                // continuous rx
            }

            LGD_DEBUG_LOG_HEXDUMP(LGD_DBG_SHELL_TEST,pkt->payload, pkt->size);
        }
        else
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "RX ERR:BufferSize = 0");
            rx_error_cnt++;
        }
    }
    else
    {
        /* Concentrator mode */
        if( pkt->size > 0 )
        {
            uint32_t dst_address = pkt->payload[DST_ADDRESS_OFFSET] | \
                                   ( pkt->payload[DST_ADDRESS_OFFSET + 1] << 8 ) | \
                                   ( pkt->payload[DST_ADDRESS_OFFSET + 2] << 16 ) | \
                                   ( pkt->payload[DST_ADDRESS_OFFSET + 3] << 24);

            if( lgd_rx_only_flag == true )
            {
                /* Indicates on a LED that the received frame */
                RX_LED_TOGGLE;

                uint32_t src_addr, dst_addr = 0;
                src_addr = pkt->payload[1];
                src_addr |= pkt->payload[2] << 8;
                src_addr |= pkt->payload[3] << 16;
                src_addr |= pkt->payload[4] << 24;

                dst_addr = pkt->payload[5];
                dst_addr |= pkt->payload[6] << 8;
                dst_addr |= pkt->payload[7] << 16;
                dst_addr |= pkt->payload[8] << 24;

                rx_correct_cnt++;

                /* RX continuous */

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Received: Totals=%d,bytes=%d,timestamp=%d,rssic=%+6.1f,rssis=%+6.1f,snr=%+d\n", rx_correct_cnt, pkt->size, rx_timestamp, rssic_value, rssis_value, snr_value_avg );
            }
            else if( ( dst_address == slaver_address || dst_address == 0xFFFFFFFF ) && \
                     ( rt_strncmp( ( const char* )pkt->payload + MAC_HEADER_OVERHEAD, ( const char* )PingMsg, 4 ) == 0 ))
            {
                /* Indicates on a LED that the received frame is a PING */
                RX_LED_TOGGLE;
                /* echo the receive packet to master */
                {
                    /* wait for master going to be ready to rx */
                    rt_thread_mdelay(5);
                    {
                        /* send back received packet with the same freq and datarate immediately */
                        lgd_conf_lgw.txrf.freq_hz = pkt->freq_hz + lgd_tx_frequency_offset;;
                        lgd_conf_lgw.txrf.datarate = pkt->datarate;
                        lgd_conf_lgw.txrf.tx_mode = IMMEDIATE;

                        lgd_app_data.size = pkt->size;
                        rt_memcpy(lgd_app_data.payload, pkt->payload, lgd_app_data.size);

                        int tx_result = lgw_tx( &lgd_conf_lgw.txrf, &lgd_app_data );

                        if( tx_result != LGW_HAL_SUCCESS )
                        {
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "lgd send back fail");
                        }
                    }
                }
            }
            else
            {
                /* RX continuous */
            }

            LGD_DEBUG_LOG_HEXDUMP(LGD_DBG_SHELL_TEST,pkt->payload, pkt->size);
        }
    }
}

static void lgd_tst_rx_thread_entry(void* parameter)
{
    int nb_pkt;

    while( 1 )
    {
        /* fetch N packets */
        nb_pkt = lgw_receive(ARRAY_SIZE(lgd_rxpkt), lgd_rxpkt);

        if ( nb_pkt == 0 || lgd_tst_sem_rx_wait_time == RT_WAITING_FOREVER )
        {
            rt_sem_take(&lgd_tst_sem_rx, lgd_tst_sem_rx_wait_time);
        }
        else
        {
            rx_timestamp = rt_tick_get();

            /* display received packets */
            for(uint8_t i = 0; i < nb_pkt; ++i)
            {
                lgd_rxpkt_ptr = &lgd_rxpkt[i];

                if (lgd_rxpkt_ptr->status != STAT_CRC_BAD)
                {
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " tstamp   : %010u", lgd_rxpkt_ptr->count_us);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " size     :%3u", lgd_rxpkt_ptr->size);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " if_chain :%2d", lgd_rxpkt_ptr->if_chain);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " frequency: %d", lgd_rxpkt_ptr->freq_hz);

                    switch (lgd_rxpkt_ptr-> modulation)
                    {
                        case MOD_LORA:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " LoRa");
                            break;

                        case MOD_FSK:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " FSK");
                            break;

                        default:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " modulation?");
                    }

                    switch (lgd_rxpkt_ptr->datarate)
                    {
                        case DR_LORA_SF7:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF7");
                            break;

                        case DR_LORA_SF8:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF8");
                            break;

                        case DR_LORA_SF9:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF9");
                            break;

                        case DR_LORA_SF10:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF10");
                            break;

                        case DR_LORA_SF11:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF11");
                            break;

                        case DR_LORA_SF12:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " SF12");
                            break;

                        default:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " datarate?");
                    }

                    switch (lgd_rxpkt_ptr->coderate)
                    {
                        case CR_LORA_4_5:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " CR1(4/5)");
                            break;

                        case CR_LORA_4_6:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " CR2(2/3)");
                            break;

                        case CR_LORA_4_7:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " CR3(4/7)");
                            break;

                        case CR_LORA_4_8:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " CR4(1/2)");
                            break;

                        default:
                            LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " coderate?");
                    }

                    rssic_value = lgd_rxpkt_ptr->rssic;
                    rssis_value = lgd_rxpkt_ptr->rssis;

                    snr_value_max = lgd_rxpkt_ptr->snr_max;
                    snr_value_min = lgd_rxpkt_ptr->snr_min;
                    snr_value_avg = lgd_rxpkt_ptr->snr;

                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " rssi:%+6.1f (avg:%.1f) snr :%+5.1f (min:%+5.1f, max:%+5.1f)", lgd_rxpkt_ptr->rssis, lgd_rxpkt_ptr->rssic, lgd_rxpkt_ptr->snr, lgd_rxpkt_ptr->snr_min, lgd_rxpkt_ptr->snr_max);

                    rx_frame_process(lgd_rxpkt_ptr);
                }
                else
                {
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " \n\nCRC error, damaged packet");
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " if_chain:%2d", lgd_rxpkt_ptr->if_chain);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " tstamp:%010u", lgd_rxpkt_ptr->count_us);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_RX_INFO, LOG_LVL_DBG, " size:%3u\n", lgd_rxpkt_ptr->size);
                }
            }
        }
    }
}

static void lgd_tst_main_thread_entry(void* parameter)
{
    while( 1 )
    {
        if( lgd_tst_initialized == false )
        {
            /* init lgd */
            if(lgd_tst_lgw_init(lgd_test_mode) == RT_EOK)
            {
                if(lgd_test_mode <= LGD_TEST_AS_DEVICE_MODE)
                {
                    /* start lgw rx thread */
                    rt_sem_release(&lgd_tst_sem_rx);
                }

                /* first init\reboot done */
                rt_sem_release(&lgd_tst_sem_main);
            }
            lgd_tst_initialized = true;
        }

        if(lgd_test_mode == LGD_TEST_AS_DEVICE_MODE)
        {
            /* tx_seq_cnt start from 0 */
            if( tx_seq_cnt < max_tx_nbtrials )
            {
                /* for first ping to printf some info */
                if( !tx_seq_cnt )
                {
                    uint32_t packet_toa = lgw_get_toa_ms(&lgd_conf_lgw.txrf, payload_len);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Mater Address(MA):[0x%X]", master_address);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Pinging [SA=0x%X] with %d bytes(ToA=%d ms) of data for %d counters:", slaver_address, payload_len, packet_toa, max_tx_nbtrials);
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "With radio parameters: freq=%d, TxPower=%d, SF=%d, BW=%s(%d), CR=%d, Public_Network:%d, IQ_Inversion:%d", lgd_conf_lgw.txrf.freq_hz, lgd_conf_lgw.txrf.rf_power,
                                  lgd_conf_lgw.txrf.datarate, bandwidth_string[lgd_conf_lgw.txrf.bandwidth - 4], lgd_conf_lgw.txrf.bandwidth, lgd_conf_lgw.txrf.coderate,
                                  lgd_conf_lgw.lorawan_public, lgd_conf_lgw.txrf.invert_pol);
                }

                lgd_ack_response_flag = false;
                send_ping_packet(master_address, slaver_address, payload_len);

                /* periodic send by lgd_tst_sem_main_wait_time or immediate send by rxdone */
                lgd_tst_sem_main.value = 0;
                rt_sem_take(&lgd_tst_sem_main, lgd_tst_sem_main_wait_time);

                if( lgd_ack_response_flag == false )
                {
                    rx_timeout_cnt++;
                    rx_timestamp = rt_tick_get();
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Request [SA=0x%X] timed out: seqno=%d, time=%d ms", slaver_address, tx_seq_cnt, LGD_TIMESTAMP_TO_MS( rx_timestamp - tx_timestamp ));
                }
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

                /* wait for PHY log output done */
                rt_thread_mdelay(10);

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "\r\n====== LoRa GW Driver Ping statistics for [MA=0x%X <-> SA=0x%X] with [TxPower=%d,SF=%d] ======", master_address, slaver_address, lgd_conf_lgw.txrf.rf_power, lgd_conf_lgw.txrf.datarate);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "-> Tx pakcets: sent = %d, tx_total = %d.%d KByte", tx_seq_cnt, tx_total_kbyte_integer, tx_total_kbyte_decimal);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "-> Rx pakcets: received = %d, lost = %d, per = %d %%, rx_total = %d.%d KByte", rx_correct_cnt, rx_timeout_cnt, per, rx_total_kbyte_integer, rx_total_kbyte_decimal);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "--> Rx rssi: rssic = %+6.1f, rssis = %+6.1f", rssic_value, rssis_value);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "--> Rx snr: max_snr  = %+d, min_snr  = %+d, avg_snr  = %+d", snr_value_max, snr_value_min, snr_value_avg);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "====== LoRa GW Driver Ping Test Finished ======\r\n");

                /* initlize the statistics parameters */
                rssis_value = -128;
                rssic_value = -128;

                snr_value_avg = -128;
                snr_value_min = -128;
                snr_value_max = -128;

                tx_seq_cnt = 0;
                rx_timeout_cnt = 0;
                rx_error_cnt = 0;
                rx_correct_cnt = 0;

                rt_sem_take(&lgd_tst_sem_main, RT_WAITING_FOREVER);
            }
        }
        else if( lgd_test_mode == LGD_TEST_AS_CW_MODE )
        {
            lgd_conf_lgw.txrf.modulation = MOD_CW;
            lgd_conf_lgw.txrf.freq_offset = 0;
            lgd_conf_lgw.txrf.f_dev = 0;
            lgd_conf_lgw.txrf.tx_mode = IMMEDIATE;

            lgd_app_data.size = 1;
            lgw_tx( &lgd_conf_lgw.txrf, &lgd_app_data );

            rt_sem_take(&lgd_tst_sem_main, RT_WAITING_FOREVER);
        }
        else
        {
            /* concentrator mode£¬pend here */
            rt_sem_take(&lgd_tst_sem_main, RT_WAITING_FOREVER);
        }
    }
}

/* for lgd(lora gw driver)finish\msh */
typedef enum
{
    CMD_LGD_CHIP_PROBE_INDEX = 0,         // LoRa Chip probe
    CMD_LGD_TXC_CONFIG_INDEX,             // txc
    CMD_LGD_RXC_CONFIG_INDEX,             // rxc
    CMD_LGD_MAC_CONFIG_INDEX,             // mac
    CMD_LGD_TX_CW_INDEX,                  // tx cw
    CMD_LGD_PING_INDEX,                   // ping-pong
    CMD_LGD_RX_PACKET_INDEX,              // rx packet only
    CMD_LGD_RESTART_INDEX,                // restart
} lora_gw_shell_cmd_t;

const char* lgd_help_info[] =
{
    [CMD_LGD_CHIP_PROBE_INDEX]            = "lgd probe                - lora gw driver probe",
    [CMD_LGD_TXC_CONFIG_INDEX]            = "lgd txc <para> <val>     - config tx parameters,<para>:<freq><power><sf><bw><iq>..",
    [CMD_LGD_RXC_CONFIG_INDEX]            = "lgd rxc <para> <val>     - config rx parameters,<para>:<rf0><rf1><auto><iq>..",
    [CMD_LGD_MAC_CONFIG_INDEX]            = "lgd mac <lorawan_public> - config <lorawan_public>..",
    [CMD_LGD_TX_CW_INDEX]                 = "lgd cw <freq> <power>    - send a tx carrier wave",
    [CMD_LGD_PING_INDEX]                  = "lgd ping <nb> <len>      - ping with nbtrials and payload len(master)",
    [CMD_LGD_RX_PACKET_INDEX]             = "lgd rx <only>            - rx data, 0 - ping(slaver), 1 - only(sniffer)",
    [CMD_LGD_RESTART_INDEX]               = "lgd reboot               - restart sx130x",
};

/* LoRa GW Driver Test Shell */
static int lgd(int argc, char *argv[])
{
    size_t i = 0;

    if (argc < 2)
    {
        /* parameter error */
        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Usage:\n");

        for (i = 0; i < sizeof(lgd_help_info) / sizeof(char*); i++)
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "%s\n", lgd_help_info[i]);
        }

        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "\n");
    }
    else
    {
        const char *cmd = argv[1];

        /* creat lgd test thread init */
        if( lgd_tst_main_thread_init() == false )
        {
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "LoRa GW Driver Test Init Failed");
            return 0;
        }

        /* if lora concentrator mode is working, release and stop it forever */
        {
            rt_sem_release(&lgd_tst_sem_rx);
            lgd_tst_sem_rx_wait_time = RT_WAITING_FOREVER;
        }

        if (!rt_strcmp(cmd, "probe"))
        {
            int32_t sx1302_chip_version = 0;
            int status = lgw_get_ver(&sx1302_chip_version);

            if( status == LGW_HAL_SUCCESS && sx1302_chip_version == 0x10 )
            {
                rt_kprintf("SX1302 Access Successed\r\n");
            }
            else
            {
                rt_kprintf("SX1302 Access Failed\r\n");
            }

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "SX1302 version: 0x%02X", sx1302_chip_version);

            // get sx1302 eui
            uint64_t gw_eui;
            char gweui_string[17] = {0};
            status = lgw_get_eui(&gw_eui);

            snprintf(gweui_string, sizeof gweui_string, "%016"PRIX64, gw_eui);
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "SX1302 EUI:%s",gweui_string);
        }
        else if (!rt_strcmp(cmd, "cw"))
        {
            if (argc >= 3)
            {
                lgd_conf_lgw.txrf.freq_hz = atol(argv[2]);
            }

            if (argc >= 4)
            {
                lgd_conf_lgw.txrf.rf_power = atol(argv[3]);
            }

            /* hardware reset sx130x to stop cw*/
            lgw_hw_reset();
            /* as cw mode */
            lgd_tst_initialized = false;
            lgd_test_mode = LGD_TEST_AS_CW_MODE;
            rt_sem_release(&lgd_tst_sem_main);

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Switch to CW Mode");
        }
        else if (!rt_strcmp(cmd, "ping"))
        {
            if( argc >= 3 )
            {
                /* max_tx_nbtrials for setup tx counter */
                max_tx_nbtrials = atol(argv[2]);
            }

            if( argc >= 4 )
            {
                /* payload_len for setup tx payload length */
                payload_len = atoi(argv[3]);

                if( payload_len < MIN_TETS_APP_DATA_SIZE )
                {
                    LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Waring: This size will not supported [lgd ping]");
                }
            }

            /* as device mode */
            lgd_rx_only_flag = false;

            if( lgd_test_mode != LGD_TEST_AS_DEVICE_MODE )
            {
                // reinit sx130x
                lgd_tst_initialized = false;
                lgd_test_mode = LGD_TEST_AS_DEVICE_MODE;
            }
            else
            {
                /* start to rx */
                lgd_tst_sem_rx_wait_time = rt_tick_from_millisecond(LGD_TEST_FETCH_RX_BUFFER_INTERVAL);
                rt_sem_release(&lgd_tst_sem_rx);
            }

            /* send wait for ack timeout = 2 * toa + process time */
            lgd_tst_sem_main_wait_time = rt_tick_from_millisecond(2 * lgw_get_toa_ms(&lgd_conf_lgw.txrf, payload_len) + ( LGD_TEST_FETCH_RX_BUFFER_INTERVAL * 3) );
            
            /* start to tx */
            rt_sem_release(&lgd_tst_sem_main);

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Switch to Devcice Mode");
        }
        else if( !rt_strcmp(cmd, "rx"))
        {
            /* as concentrator */
            lgd_tst_initialized = false;
            lgd_test_mode = LGD_TEST_AS_CONCENTRATOR_MODE;

            if (argc >= 3)
            {
                /* if lgd_rx_only_flag = 1, not ack otherwise send back */
                lgd_rx_only_flag = atol(argv[2]);
            }

            rt_sem_release(&lgd_tst_sem_main);

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Switch to Concentrator Mode");
        }
        else if (!rt_strcmp(cmd, "rxc"))
        {
            const char *cmd1 = argv[2];
            uint32_t freq  = atol(argv[3]);

            uint8_t enable = 1;/* default enable */

            if( argc >= 5 )
            {
                enable = atol(argv[4]);
            }

            if( argc >= 3 )
            {
                if (!rt_strcmp(cmd1, "rf0")) /* radio0 center frequence */
                {
                    lgd_conf_lgw.rxrf[0].enable   = enable;

                    lgd_conf_lgw.rxrf[0].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "rf1")) /* radio1 center frequence */
                {
                    lgd_conf_lgw.rxrf[1].enable   = enable;

                    lgd_conf_lgw.rxrf[1].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "auto")) /* setup start freq, 200k space, lgw auto setup radio0 and radio1 chanenl freq. */
                {
                    lgd_conf_lgw.rxrf[0].enable   = enable;
                    lgd_conf_lgw.rxrf[1].enable   = enable;

                    /* eg:set start 475.3 -> radio0 = 475.6 ,radio1 = 476.4 */
                    lgd_conf_lgw.rxrf[0].freq_hz = freq + LGD_RF_CHIAN0_CENTER_FREQ_OFFSET;
                    lgd_conf_lgw.rxrf[1].freq_hz = freq + LGD_RF_CHIAN1_CENTER_FREQ_OFFSET;
                }
                else if (!rt_strcmp(cmd1, "iq"))
                {
                    if (argc >= 4)
                    {
                        lgd_conf_lgw.rxrf[0].invert_pol = atoi(argv[3]);
                    }
                }
            }

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "-items-                     -{cmd}-   -Value-");
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " radio-chain-0 center freq   {rf0}:   %d,%d", lgd_conf_lgw.rxrf[0].freq_hz,lgd_conf_lgw.rxrf[0].enable);
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " radio-chain-1 center freq   {rf1}:   %d,%d", lgd_conf_lgw.rxrf[1].freq_hz,lgd_conf_lgw.rxrf[1].enable);
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " radio-chain-0-1 start freq  {auto}:  %d,%d", (lgd_conf_lgw.rxrf[0].freq_hz - LGD_RF_CHIAN0_CENTER_FREQ_OFFSET),lgd_conf_lgw.rxrf[0].enable);
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " rx iq invert                {iq}:    %d", lgd_conf_lgw.rxrf[0].invert_pol);

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_DBG, "== Channels Frequence Table: ==\r\n");
            for(uint8_t i = 0; i < 8; i++)
            {
                uint32_t ch_freq = lgd_conf_lgw.rxrf[lgd_conf_lgw.rxif.channel_if_rfchain[i]].freq_hz + lgd_conf_lgw.rxif.channel_if_freq[i];
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "CH[%i]:%d,rfchain:%d,%s\r\n", i, ch_freq, lgd_conf_lgw.rxif.channel_if_rfchain[i], lgd_conf_lgw.rxif.channel_if_enable[i] ? "enable" : "disable");

                if( i == 0 )
                {
                    // defaultly, update tx freq equal to first rx channel frequence plus tx freq offset
                    lgd_conf_lgw.txrf.freq_hz = ch_freq + lgd_tx_frequency_offset;
                }
            }
        }
        else if (!rt_strcmp(cmd, "txc"))
        {
            const char *cmd1 = argv[2];

            /* config radio paramters, such as frequency,txPower,sf,bw...*/
            if (!rt_strcmp(cmd1, "freq"))
            {
                if (argc >= 4)
                {
                    lgd_conf_lgw.txrf.freq_hz = atol(argv[3]);
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Tx Frequency: %d", lgd_conf_lgw.txrf.freq_hz);
            }
            else if (!rt_strcmp(cmd1, "foffset"))
            {
                if (argc >= 4)
                {
                    lgd_tx_frequency_offset = atol(argv[3]);
                }

                lgd_conf_lgw.txrf.freq_hz = lgd_conf_lgw.rxrf[0].freq_hz + lgd_tx_frequency_offset;
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Tx Frequency Offset: %d", lgd_tx_frequency_offset);
            }
            else if (!rt_strcmp(cmd1, "power"))
            {
                if (argc >= 4)
                {
                    lgd_conf_lgw.txrf.rf_power = atoi(argv[3]);
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "TxPower  : %d", lgd_conf_lgw.txrf.rf_power);
            }
            else if (!rt_strcmp(cmd1, "sf"))
            {
                if (argc >= 4)
                {
                    if (!rt_strcmp(argv[3], "?"))
                    {
                        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "SF:7|8|9|10|11|12");
                    }
                    else
                    {
                        lgd_conf_lgw.txrf.datarate = atoi(argv[3]);
                    }
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "SF: %d", lgd_conf_lgw.txrf.datarate);
            }
            else if (!rt_strcmp(cmd1, "bw"))
            {
                uint32_t bandwidth;

                if (argc >= 4)
                {
                    if (!rt_strcmp(argv[3], "?"))
                    {
                        LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "LoRa BW:125kHz|250kHz|500kHz");
                    }
                    else
                    {
                        bandwidth = atoi(argv[3]);
                    }
                }

                if( bandwidth == 125 )
                {
                    lgd_conf_lgw.txrf.bandwidth = BW_125KHZ;
                }
                else if( bandwidth == 250 )
                {
                    lgd_conf_lgw.txrf.bandwidth = BW_250KHZ;
                }
                else if( bandwidth == 500 )
                {
                    lgd_conf_lgw.txrf.bandwidth = BW_500KHZ;
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "LoRa BW: %s(%d)", bandwidth_string[lgd_conf_lgw.txrf.bandwidth - BW_125KHZ], lgd_conf_lgw.txrf.bandwidth);
            }
            else if (!rt_strcmp(cmd1, "iq"))
            {
                if (argc >= 4)
                {
                    lgd_conf_lgw.txrf.invert_pol = atoi(argv[3]);
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "IQ Inversion: %d", lgd_conf_lgw.txrf.invert_pol);
            }

            if (argc < 3)
            {
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "-items-         -{cmd}-   -Value-");
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " TX Freq         {freq}:   %d", lgd_conf_lgw.txrf.freq_hz);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " TX Freq Offset  {foffset}:%d", lgd_tx_frequency_offset);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " Tx Power        {power}:  %d", lgd_conf_lgw.txrf.rf_power);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " SF              {sf}:     %d", lgd_conf_lgw.txrf.datarate);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " BW              {bw}:     %s(%d)", bandwidth_string[lgd_conf_lgw.txrf.bandwidth - 4], lgd_conf_lgw.txrf.bandwidth);
                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " Tx IQ Inversion {iq}:     %d", lgd_conf_lgw.txrf.invert_pol);

            }
        }
        else if (!rt_strcmp(cmd, "mac"))
        {
            const char *cmd1 = argv[2];

            if (!rt_strcmp(cmd1, "lorawan"))
            {
                if (argc >= 4)
                {
                    lgd_conf_lgw.lorawan_public = atoi(argv[3]);
                }

                LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "Public Network: %d", lgd_conf_lgw.lorawan_public);
            }

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, "-items-         -{cmd}-     -Value-");
            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " Public Network  {lorawan}:  %d(%s)", lgd_conf_lgw.lorawan_public,lgd_conf_lgw.lorawan_public?"public":"private");
        }
        else if (!rt_strcmp(cmd, "reboot"))
        {
            lgd_tst_initialized = false;
            rt_sem_release(&lgd_tst_sem_main);

            LGD_DEBUG_LOG(LGD_DBG_SHELL_TEST, LOG_LVL_INFO, " Restart SX130x..");
        }
    }

    return 1;
}
MSH_CMD_EXPORT(lgd, lora gw driver test shell);

