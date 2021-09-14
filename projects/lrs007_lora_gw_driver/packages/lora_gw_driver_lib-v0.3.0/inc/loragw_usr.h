/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __LORAGW_USR_H__
#define __LORAGW_USR_H__

#include "loragw_hal.h"
#include "loragw_services.h"

/* return status code */
#define LGD_HAL_SUCCESS     0
#define LGD_HAL_ERROR       -1


#ifdef LORA_GW_DRIVER_USING_SX1302_LSD4WN_2K830NE0
#ifndef LGD_RF_CHIAN0_CENTER_FREQ
#define LGD_RF_CHIAN0_CENTER_FREQ 868.4e6
#endif 

#ifndef LGD_RF_CHIAN1_CENTER_FREQ
#define LGD_RF_CHIAN1_CENTER_FREQ 869.2e6
#endif 
#else
#ifndef LGD_RF_CHIAN0_CENTER_FREQ
#define LGD_RF_CHIAN0_CENTER_FREQ 475.6e6
#endif 

#ifndef LGD_RF_CHIAN1_CENTER_FREQ
#define LGD_RF_CHIAN1_CENTER_FREQ 476.4e6
#endif 
#endif

#ifndef LGD_RF_CHIAN0_CENTER_FREQ_OFFSET
#define LGD_RF_CHIAN0_CENTER_FREQ_OFFSET 300000//300KHz
#endif 

#ifndef LGD_RF_CHIAN1_CENTER_FREQ_OFFSET
#define LGD_RF_CHIAN1_CENTER_FREQ_OFFSET 1100000//1.1MHz
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
    
/**
@struct lgw_conf_txrf_usr_t
@brief Configuration structure for a RF chain
*/
typedef struct lgw_conf_txrf_usr_s {
    uint32_t    freq_hz;        /*!> center frequency of TX */
    uint32_t    count_us;       /*!> timestamp or delay in microseconds for TX trigger */
    uint8_t     tx_mode;        /*!> select on what event/time the TX is triggered */
    int8_t      rf_power;       /*!> TX power, in dBm */
    uint8_t     modulation;     /*!> modulation to use for the packet */
    uint16_t    preamble;       /*!> set the preamble length, 0 for default */
    int8_t      freq_offset;    /*!> frequency offset from Radio Tx frequency (CW mode) */
    uint8_t     bandwidth;      /*!> modulation bandwidth (LoRa only) */
    uint8_t     coderate;       /*!> error-correcting code of the packet (LoRa only) */
    uint32_t    datarate;       /*!> TX datarate (baudrate for FSK, SF for LoRa) */
    uint8_t     f_dev;          /*!> frequency deviation, in kHz (FSK only) */
    bool        invert_pol;     /*!> invert signal polarity, for orthogonal downlinks (LoRa only) */
    bool        no_crc;         /*!> if true, do not send a CRC in the packet */
    bool        no_header;      /*!> if true, enable implicit header mode (LoRa), fixed length (FSK) */
    bool        external_pa;    /*!> if true, enable external pa */
}lgw_conf_txrf_usr_t;

/**
@struct lgw_conf_rxrf_usr_s
@brief Configuration structure for a RF chain
*/
struct lgw_conf_rxrf_usr_s {
    bool        enable;             /*!> enable or disable that RF chain */
    uint32_t    freq_hz;            /*!> center frequency of the radio in Hz */  
    bool        invert_pol;        /*!> invert signal polarity, for orthogonal uplinks (LoRa only) */     
};

/**
@struct lgw_conf_rxif_usr_s
@brief Configuration structure for an IF chain
*/
struct lgw_conf_rxif_usr_s {
    uint8_t  channel_if_enable[LGW_IF_CHAIN_NB];  /*!> channel if enable */ 
    uint8_t  channel_if_rfchain[LGW_IF_CHAIN_NB]; /*!> channel if frequence in Hz */
    int32_t channel_if_freq[LGW_IF_CHAIN_NB];     /*!> channel if rfchain in 0~10 */
    uint32_t channel_if8_datarate;                /*!> RX datarate, 0 for default */
    uint8_t  channel_if8_bandwidth;               /*!> RX bandwidth, 0 for default */
};

typedef struct lgw_payload_usr_s {
    uint16_t    size;           /*!> payload size in bytes */
    uint8_t     payload[256];   /*!> buffer containing the payload */
}lgw_payload_usr_t;

typedef struct lgw_conf_usr_s 
{
    uint16_t magic;

    bool lorawan_public; /*!> Enable ONLY for *public* networks using the LoRa MAC protocol */
    
    /* rx */
    struct lgw_conf_rxrf_usr_s rxrf[LGW_RF_CHAIN_NB];
    struct lgw_conf_rxif_usr_s rxif;
    
    /* tx */
    struct lgw_conf_txrf_usr_s txrf;
    
}lgw_conf_usr_t;
#define LGW_CONF_USR_SIZE  sizeof(struct lgw_conf_usr_s)
    
/**
@brief lora gw driver init
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_init(lgw_conf_usr_t *conf);

/**
@brief Schedule a packet to be send immediately or after a delay depending on tx_mode
@param pkt_data structure containing the data and metadata for the packet to send
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
extern int lgw_tx(struct lgw_conf_txrf_usr_s *txrf, lgw_payload_usr_t *payload);

/**
@brief Return time on air of given packet, in milliseconds
@param txrf is a pointer to the txrf structure
@param len is payload length 
@return the packet time on air in milliseconds
*/
extern uint32_t lgw_get_toa_ms(struct lgw_conf_txrf_usr_s *txrf, uint16_t len);

/**
@brief Get the SX1302 chip version
@param ver  pointerto the memory holding the sx1302 chip version
@return LGW_REG_SUCCESS if no error, LGW_REG_ERROR otherwise
*/
int lgw_get_ver(int32_t* ver);
   
#endif
