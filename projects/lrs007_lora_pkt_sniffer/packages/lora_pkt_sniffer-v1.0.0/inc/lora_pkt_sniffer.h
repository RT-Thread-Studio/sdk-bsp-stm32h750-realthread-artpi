/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef __LORA_PACKET_SNIFFER_H__
#define __LORA_PACKET_SNIFFER_H__

#define DEFAULT_SERVER      192.168.1.104
#define DEFAULT_PORT_UP     1700
#define DEFAULT_PORT_DW     1700

#define LPS_FETCH_RX_BUFFER_INTERVAL_MS 100 

// Lora Packet Sniffer(LPS) northbound interface
typedef enum
{
    LPS_NORTHBOUND_IF_OVER_SERIAL = 0x00,   // directly parse and display over serial
    LPS_NORTHBOUND_IF_OVER_WIFI,            // wireshark over wifi
    LPS_NORTHBOUND_IF_OVER_ETHERNET,        // wireshark over ethernet
    LPS_NORTHBOUND_IF_NUMS,                 // northbound if numbers
}lps_northbound_interface_t;

typedef enum
{
    LPS_LORA_PACKET_CRC_OK_ONLY      = 0x00, // for lorawan uplink
    LPS_LORA_PACKET_NOCRC_ONLY       = 0x01, // for lorawan downlink
    LPS_LORA_PACKET_CRC_OK_AND_NOCRC = 0x02, // for lorawan uplink and downlink
    LPS_LORA_PACKET_CRC_ALL_CASES    = 0x03, // for lorawan uplink and downlink and CRC bad
}lps_lora_packet_crc_filter_t;

// DevAddr Whitelist
typedef enum
{
    LPS_DEVADDR_WHITE_LIST_DISABLE = 0, 
    LPS_DEVADDR_WHITE_LIST_ENABLE,         
}lps_addr_whilte_list_t;


/**
@struct lora_packet_sniffer_conf_s
@brief Configuration structure for sniffer
*/
#pragma pack(push,1) // lign to byte

struct lps_conf_sys_s 
{
    uint16_t magic;
    /* 0 - crc on only, 1 - crc off only ,2 -  crc on and off 3 all */
    lps_lora_packet_crc_filter_t crc_filter_mode;  
    lps_northbound_interface_t northboud_if;
        /* for lorawan downlink */
    uint8_t rx_invert_pol;
    
    uint32_t devaddr_white_list;
    uint8_t deveui_white_list[8];
};
#define LPS_CONF_SYS_SIZE sizeof(struct lps_conf_sys_s)
    
/**
@struct lps_conf_srv_s
@brief Configuration structure for network server info
*/
struct lps_conf_srv_s
{
    uint16_t magic;
    char server_addr[64];
    char server_port_up[8];
    char server_port_down[8];
};
#define LPS_CONF_SRV_SIZE sizeof(struct lps_conf_srv_s)
    
struct lps_msg_s
{
    rt_uint8_t *data_ptr;
    rt_uint8_t data_size; 
};

#pragma pack(pop)    

extern struct lps_conf_sys_s lps_conf_sys;
extern struct lgw_conf_usr_s lps_conf_lgw;
extern struct lps_conf_srv_s lps_conf_srv;

extern const struct lps_conf_sys_s lps_conf_sys_default;
extern const struct lgw_conf_usr_s lps_conf_lgw_default;
extern const struct lps_conf_srv_s lps_conf_srv_default;

extern const char *northboud_if_string[];
extern const char *crc_filter_strings[];

extern rt_int32_t lps_rx_wait_time;

extern struct rt_semaphore lps_rx_sem;
extern struct rt_messagequeue lps_udp_up_mq;
extern int lps_udp_up_thread_init(void);

int16_t lps_nvm_init(void);
int lps_conf_sys_save(void *buff, int len);
int lps_conf_lgw_save(void *buff, int len);
int lps_conf_srv_save(void *buff, int len);
int lps_init(void);

#endif
