/*
Description:
    lora packet sniffer

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: forest-rain
*/
/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>        /* C99 types */
#include <stdbool.h>       /* bool type */
#include <stdio.h>         /* rt_kprintf */
#include <string.h>        /* memset */

#include <rthw.h>
#include <rtdevice.h>
#include "board.h"
#include <rtthread.h>

#ifdef LORA_PKT_SNIFFER_USING_WIRESHARK_MODE
    #include <sys/socket.h> /* BSD socket */
#endif

#include "loragw_hal.h"
#include "loragw_usr.h"
#include "lora_pkt_sniffer.h"

#include "fal.h"
#ifdef PKG_USING_EASYFLASH
    #include "easyflash.h"
#endif

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LPS"
#include "lora_pkt_sniffer_dbg.h"
/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define STRINGIFY(x)    #x
#define STR(x)          STRINGIFY(x)

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

/* --- PRIVATE VARIABLES ---------------------------------------------------- */
struct lgw_pkt_rx_s rxpkt[4]; /* array containing up to 4 inbound packets metadata */
struct lgw_pkt_rx_s *rx_pkt_ptr; /* pointer on a RX packet */

struct rt_semaphore lps_rx_sem;
struct rt_thread lps_rx_thread;
rt_uint32_t lps_rx_stack[8192];

bool lps_initialized = false;

rt_int32_t lps_rx_wait_time = LPS_FETCH_RX_BUFFER_INTERVAL_MS; // 100ms

/* --- PRIVATE VARIABLES ---------------------------------------------------- */

struct lgw_conf_rxrf_s rfconf;
struct lgw_conf_rxif_s ifconf;

struct lgw_conf_usr_s lps_conf_lgw;
const struct lgw_conf_usr_s lps_conf_lgw_default =
{
    .magic = 0xAA55,
    .lorawan_public = true,/* true for lorawan */

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
        .channel_if_enable =  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        .channel_if_rfchain = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },

        .channel_if_freq =
        {
            -300000,
            -100000,
            100000,
            300000,
            -300000,
            -100000,
            100000,
            300000,
        },
    },
};

struct lps_conf_sys_s lps_conf_sys;
const struct lps_conf_sys_s lps_conf_sys_default =
{
    .magic               = 0xAA55,
    .northboud_if        = LPS_NORTHBOUND_IF_OVER_SERIAL,//LW_SNIFFER_DISPLAY_WIRESHARK,LW_SNIFFER_DISPLAY_DIRECT,LW_SNIFFER_DISPLAY_WIRESHARK_ETHERNET

    .crc_filter_mode     = LPS_LORA_PACKET_CRC_OK_AND_NOCRC,//LPS_LORA_PACKET_CRC_OK_ONLY,
    .rx_invert_pol       = false,
};

struct lps_conf_srv_s lps_conf_srv;
const struct lps_conf_srv_s lps_conf_srv_default =
{
    .magic = 0xAA55,
    .server_addr = STR(DEFAULT_SERVER),
    .server_port_up = STR(DEFAULT_PORT_UP),
    .server_port_down = STR(DEFAULT_PORT_DW),
};

const char *crc_filter_strings[4] = {"ONLY_CRC", "NO_CRC", "CRC & NO_CRC", "NO_CHECK"};
const char *northboud_if_string[LPS_NORTHBOUND_IF_NUMS] = {"uart","wifi","eth",};
const char *protocol_parser_terminal_string[2] = {"serial", "wireshark"};
const char *ed_direction_string[2] = {"ED Uplink", "ED Downlink"};
static uint8_t crc_bad_counter = 0;
/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DECLARATION ---------------------------------------- */
static void lps_rx_thread_entry(void* parameter);
/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

int lps_init(void)
{
    if( lps_rx_thread.entry == RT_NULL )
    {
        rt_err_t result;

        #if defined LORA_PKT_SNIFFER_USING_WIRESHARK_MODE
        if( ( lps_conf_sys.northboud_if == LPS_NORTHBOUND_IF_OVER_WIFI ) || \
            ( lps_conf_sys.northboud_if == LPS_NORTHBOUND_IF_OVER_ETHERNET ))
        {
            lps_udp_up_thread_init();
        }
        #endif

        /* sem init to 0 */
        result = rt_sem_init(&lps_rx_sem, "lps_rx_sem", 0, RT_IPC_FLAG_FIFO);

        if (result != RT_EOK)
        {
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "init static semaphore failed.\n");
        }

        result = rt_thread_init(&lps_rx_thread,
                                "lps-rx",
                                lps_rx_thread_entry,
                                RT_NULL,
                                &lps_rx_stack[0],
                                sizeof(lps_rx_stack),
                                6,
                                10);

        if (result == RT_EOK)
        {
            rt_thread_startup(&lps_rx_thread);
        }
        else
        {
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "init static thread failed.\n");
            return false;
        }

        /* wait for lgw_init done */
        rt_sem_take(&lps_rx_sem, RT_WAITING_FOREVER);
    }

    return true;
}

static bool lps_crc_filter_check(struct lgw_pkt_rx_s *rx_pkt_ptr)
{
    return (( lps_conf_sys.crc_filter_mode == LPS_LORA_PACKET_CRC_OK_ONLY && rx_pkt_ptr->status == STAT_CRC_OK ) || \
    ( lps_conf_sys.crc_filter_mode == LPS_LORA_PACKET_NOCRC_ONLY && rx_pkt_ptr->status == STAT_NO_CRC )  || \
    ( lps_conf_sys.crc_filter_mode == LPS_LORA_PACKET_CRC_OK_AND_NOCRC && ( rx_pkt_ptr->status == STAT_NO_CRC || rx_pkt_ptr->status == STAT_CRC_OK )) || \
    ( lps_conf_sys.crc_filter_mode == LPS_LORA_PACKET_CRC_ALL_CASES ));
}

static bool lps_devaddr_white_list_check(struct lgw_pkt_rx_s *rx_pkt_ptr)
{
    uint32_t devaddr_temp = rx_pkt_ptr->payload[1] | (rx_pkt_ptr->payload[2]<<8) | ((uint32_t)rx_pkt_ptr->payload[3]<<16) | ((uint32_t)rx_pkt_ptr->payload[4]<<24);
    
    if((lps_conf_sys.devaddr_white_list == LPS_DEVADDR_WHITE_LIST_DISABLE) || \
       (lps_conf_sys.devaddr_white_list == devaddr_temp))
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool lps_deveui_white_list_check(struct lgw_pkt_rx_s *rx_pkt_ptr)
{
    uint8_t deveui_default[8] = { 0 };
    uint8_t deveui_temp[8] = { 0 };
    
    for(uint8_t i = 0; i < 8; i++)
    {
        deveui_temp[i] = rx_pkt_ptr->payload[16-i];
    }
    
    if ((rt_memcmp((char *)lps_conf_sys.deveui_white_list, deveui_default, 8) == 0) || \
        (rt_memcmp((char *)lps_conf_sys.deveui_white_list, deveui_temp, 8) == 0))  
    {
        return true;
    }            
    else
    {
        return false;
    }
}

static bool lps_dev_id_filter(struct lgw_pkt_rx_s *rx_pkt_ptr)
{
    uint8_t mtype = ( rx_pkt_ptr->payload[0] & 0xE0 ) >> 5;
    if( mtype == 0x00 ) // join -request 
    {
        return lps_deveui_white_list_check(rx_pkt_ptr);
    }
    else if( mtype >= 0x02 && mtype <= 0x05 ) // up\downlink data unconfirmed\confirmed
    {
        return lps_devaddr_white_list_check(rx_pkt_ptr);
    }
    else // join accept,other..
        return true;
}

static void lps_rx_thread_entry(void* parameter)
{
    int8_t status = 0;
    uint32_t loop_cnt = 0;
    int nb_pkt;

    while(1)
    {
        if( lps_initialized == false )
        {
            /* default for concentrator mode */
            lps_init();

            //--------------------------------------------------------------------
            // display lw-sniffer's parameters
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "Northbound IF: %s", northboud_if_string[lps_conf_sys.northboud_if]);
            // crc filter mode
            // - 0 CRC only
            // - 1 no crc
            // - 2 CRC and no CRC
            // - 3 CRC all and bad
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "CRC Filter Mode: %s", crc_filter_strings[lps_conf_sys.crc_filter_mode]);
            uint8_t deveui_temp[8] = {0};
            if(rt_memcmp(lps_conf_sys.deveui_white_list,deveui_temp,8) != 0)
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "DevEUI White List : DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X",
                              HEX8(lps_conf_sys.deveui_white_list));
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "DevEUI White List : Disable");
            }
            if( lps_conf_sys.devaddr_white_list != LPS_DEVADDR_WHITE_LIST_DISABLE )
            {
                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "DevAddr White List : %08X", lps_conf_sys.devaddr_white_list);
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "DevAddr White List : Disable", lps_conf_sys.devaddr_white_list);
            }
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "LoRaWAN Public : %s", lps_conf_lgw.lorawan_public ? "Enable":"Disable");
            LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "Sniffer Direction : %s", ed_direction_string[lps_conf_lgw.rxrf[0].invert_pol]);
            //LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "Rx IQ Invert : %d", lps_conf_sys.rx_invert_pol);

            /* Stop the gateway */
            status = lgw_stop();

            if (status != 0)
            {
                LPS_DEBUG_LOG(LPS_DBG_SNF, LOG_LVL_INFO, "ERROR: failed to stop the gateway\n");
            }

            /* initlize and start sx1302*/
            lgw_init(&lps_conf_lgw);

            /* lgw_init init done */
            rt_sem_release(&lps_rx_sem);

            /* connect, configure and start the LoRa concentrator */
            status = lgw_start();

            if (status == LGW_HAL_SUCCESS)
            {
                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "*** LoRa Packet Sniffer started ***\n");
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "*** Impossible to start LoRa Packet Sniffer ***\n");

                return;
            }

            /* sniffer time */
            lps_rx_wait_time = rt_tick_from_millisecond(LPS_FETCH_RX_BUFFER_INTERVAL_MS);

            lps_initialized = true;
        }
        else
        {
            loop_cnt++;

            /* fetch N packets */
            nb_pkt = lgw_receive(ARRAY_SIZE(rxpkt), rxpkt);

            if ( nb_pkt == 0 )
            {
                rt_sem_take(&lps_rx_sem, lps_rx_wait_time);
            }
            else
            {
                /* display received packets */
                for(uint8_t i = 0; i < nb_pkt; ++i)
                {
                    rx_pkt_ptr = &rxpkt[i];
                    LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "---\nRcv pkt #%d >>\n", i + 1); // 190509 + /n
  
                    if( lps_crc_filter_check(rx_pkt_ptr) )
                    { 
                        if( lps_dev_id_filter(rx_pkt_ptr) )
                        {    
                            if (rx_pkt_ptr->status == STAT_CRC_OK || rx_pkt_ptr->status == STAT_NO_CRC )
                            {
                                //LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " frequency:%d", lps_conf_sys.channel_freq[rx_pkt_ptr->if_chain-1]);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " tstamp   : %010u", rx_pkt_ptr->count_us);// 190509 + /n
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " size     :%3u", rx_pkt_ptr->size);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " if_chain :%2d", rx_pkt_ptr->if_chain);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " frequency: %d", rx_pkt_ptr->freq_hz);
                                //LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " datarate:%d", rx_pkt_ptr->datarate);

                                switch (rx_pkt_ptr-> modulation)
                                {
                                    case MOD_LORA:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " LoRa");
                                        break;

                                    case MOD_FSK:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " FSK");
                                        break;

                                    default:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " modulation?");
                                }

                                switch (rx_pkt_ptr->datarate)
                                {
                                    case DR_LORA_SF7:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF7");
                                        break;

                                    case DR_LORA_SF8:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF8");
                                        break;

                                    case DR_LORA_SF9:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF9");
                                        break;

                                    case DR_LORA_SF10:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF10");
                                        break;

                                    case DR_LORA_SF11:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF11");
                                        break;

                                    case DR_LORA_SF12:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " SF12");
                                        break;

                                    default:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " datarate?");
                                }

                                switch (rx_pkt_ptr->coderate)
                                {
                                    case CR_LORA_4_5:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " CR1(4/5)");
                                        break;

                                    case CR_LORA_4_6:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " CR2(2/3)");
                                        break;

                                    case CR_LORA_4_7:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " CR3(4/7)");
                                        break;

                                    case CR_LORA_4_8:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " CR4(1/2)");
                                        break;

                                    default:
                                        LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " coderate?");
                                }

                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " rssi:%+6.1f (avg:%.1f) snr :%+5.1f (min:%+5.1f, max:%+5.1f)", rx_pkt_ptr->rssis, rx_pkt_ptr->rssic, rx_pkt_ptr->snr, rx_pkt_ptr->snr_min, rx_pkt_ptr->snr_max);

                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " payload:");
                            }
                            else if (rx_pkt_ptr->status == STAT_CRC_BAD)
                            {
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " if_chain:%2d", rx_pkt_ptr->if_chain);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " tstamp:%010u", rx_pkt_ptr->count_us);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " size:%3u\n", rx_pkt_ptr->size);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " CRC error, damaged packet,crc_bad_counter = %d\n\n", crc_bad_counter);
                            }
                            else
                            {
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " if_chain:%2d", rx_pkt_ptr->if_chain);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " tstamp:%010u", rx_pkt_ptr->count_us);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " size:%3u\n", rx_pkt_ptr->size);
                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, " invalid status ?!?\n\n");
                            }

                            // clear if not STAT_CRC_BAD
                            if(rx_pkt_ptr->status != STAT_CRC_BAD)
                            {
                                crc_bad_counter = 0;
                            }

                            if( rx_pkt_ptr->size < 230 ) // 230 for lorawan max payload.
                            {
                                LPS_DEBUG_LOG_HEXDUMP(LPS_DBG_SNF, rx_pkt_ptr->payload, rx_pkt_ptr->size);

                                #ifdef LORA_GW_DRIVER_USING_LORA_FRAME_FORMAT_SERVICE
                                {
                                    if( lgw_service_lora_frame_format(rx_pkt_ptr) == LGW_HAL_SUCCESS )
                                    {
                                        #if defined LORA_PKT_SNIFFER_USING_WIRESHARK_MODE
                                        if( ( lps_conf_sys.northboud_if == LPS_NORTHBOUND_IF_OVER_WIFI ) || ( lps_conf_sys.northboud_if == LPS_NORTHBOUND_IF_OVER_ETHERNET ))
                                        {
                                            struct lps_msg_s msg_ptr;
                                            msg_ptr.data_ptr = rx_pkt_ptr->payload;
                                            msg_ptr.data_size = rx_pkt_ptr->size;

                                            rt_err_t result = rt_mq_send(&lps_udp_up_mq, (void*)&msg_ptr, sizeof(struct lps_msg_s));

                                            if (result != RT_EOK)
                                            {
                                                LPS_DEBUG_LOG(LPS_DBG_SNF, DBG_LVL, "rt_mq_send ERR\n");
                                            }
                                        }
                                        #endif
                                    }
                                }
                                #endif
                            }
                        }
                    }
                }
            }
        }
    }
}
/* --- EOF ------------------------------------------------------------------ */
