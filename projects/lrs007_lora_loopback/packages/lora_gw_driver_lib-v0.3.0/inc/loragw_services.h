/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __LORAGW_SERVICE_H__
#define __LORAGW_SERVICE_H__

#include "loragw_hal.h"

#define END_DEVICE_FCNT_MODIFY_IGNORE  0xFFFFFFFF

#define HEX16(X)  X[0],X[1],X[2],X[3],X[4],X[5],X[6],X[7],X[8],X[9],X[10],X[11],X[12],X[13],X[14],X[15]
#define HEX8(X)   X[0],X[1],X[2],X[3],X[4],X[5],X[6],X[7]

typedef struct dev_id_info_tag
{
    uint8_t data[8];
    uint8_t size;
}dev_id_info_t;

/**
@brief parse lora/lorawan frame and pack to pcap frame
@param pkt_data pointer to an array of struct that will receive the packet metadata and payload pointers
@return LGW_HAL_ERROR id the operation failed, else the lorawan packets retrieved
*/
int lgw_service_lora_frame_format(struct lgw_pkt_rx_s *packet);

/**
@brief register a lorawan end device
@param  pointer to deveui,appeui,appkey for otaa, devaddr,appskey,nwkskey for abp or otaa
@return LGW_HAL_ERROR id the operation failed, else the lorawan packets retrieved
*/
int lgw_service_lorawan_end_device_register(dev_id_info_t *dev_id, uint8_t *attribute1, uint8_t *attribute2);
  
/**
@brief delete a lorawan end device
@param pointer to deveui(otaa) or devaddr (abp\otaa)
@return LGW_HAL_ERROR id the operation failed, else the lorawan packets retrieved
*/  
int lgw_service_lorawan_end_device_unregister(dev_id_info_t *dev_id);
        
/**
@brief list all registered lorawan end device
@param none
@return LGW_HAL_ERROR id the operation failed, else the lorawan packets retrieved
*/  
int lgw_service_lorawan_end_devices_info_list(void);   

/**
@brief modify lorawan end device frame counter(uplink \ downlink)
@param none
@return LGW_HAL_ERROR id the operation failed, else the lorawan packets retrieved
*/  
int lgw_service_lorawan_end_device_fcnt_modify(dev_id_info_t *dev_id,uint32_t uplink_counter,uint32_t downlink_counter);
   
#endif
