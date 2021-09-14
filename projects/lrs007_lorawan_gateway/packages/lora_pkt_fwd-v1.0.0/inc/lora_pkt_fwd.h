#ifndef _LORA_PKT_FWD_H
#define _LORA_PKT_FWD_H

#include "loragw_usr.h"

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

// LoRa Packet Forward(LPF) northbound interface
typedef enum
{
    LPF_NORTHBOUND_IF_OVER_WIFI = 0,        // over wifi
    LPF_NORTHBOUND_IF_OVER_ETHERNET,        // over ethernet
    LPF_NORTHBOUND_IF_NUMS,                 // northbound if numbers
}lpf_northbound_interface_t;

// LoRa Packet Forward(LPF) supported network server
typedef enum
{
    LPF_SUPPORTED_NETWORK_SERVER_LIERDA_UNICORE_GWMP = 0,          // lierda unicore
    LPF_SUPPORTED_NETWORK_SERVER_TTN_GWMP,                         // ttn 
    LPF_SUPPORTED_NETWORK_SERVER_TENCENT_IOT_EXPLORER_GWMP,        // tencent iot-explorer
    LPF_SUPPORTED_NETWORK_SERVER_NUMS,                             // server numbers
}lpf_supported_server_t;

#pragma pack(push,1) // lign to byte
/**
@struct lpf_conf_srv_s
@brief Configuration structure for lora pkt fwd server conf
*/
struct lpf_conf_srv_s
{
    uint16_t magic;
    lpf_northbound_interface_t northboud_if;

    char server_addr[64];
    char server_port_up[8];
    char server_port_down[8];
    
    char json_conf_file[64];        /* lgw json conf file */
};
#define LPF_CONF_SRV_SIZE   sizeof(struct lpf_conf_srv_s)
#pragma pack(pop)  

extern struct lpf_conf_srv_s lpf_conf_srv;
extern const struct lpf_conf_srv_s lpf_conf_srv_default[];
extern struct lgw_conf_usr_s lpf_conf_lgw;
extern const struct lgw_conf_usr_s lpf_conf_lgw_default;

extern int lpf_init(void);
extern void lpf_nv_params_init(void);
extern int lpf_conf_lgw_nv_save(void *buff, int len);
extern int lpf_conf_srv_nv_save(void *buff, int len);

#endif

/* --- EOF ------------------------------------------------------------------ */
