/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Minimum test program for the loragw_hal 'library'

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
*/

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

/* fix an issue between POSIX and C99 */
#if __STDC_VERSION__ >= 199901L
    #define _XOPEN_SOURCE 600
#else
    #define _XOPEN_SOURCE 500
#endif

#include <stdint.h>        /* C99 types */
#include <stdbool.h>       /* bool type */
#include <stdio.h>         /* LOG_D */
#include <string.h>        /* memset */

#include <rthw.h>
#include <rtdevice.h>
#include "board.h"
#include <rtthread.h>
#include <fal.h>
#include <easyflash.h>

#include "loragw_hal.h"
#include "loragw_usr.h"
#include "lora_pkt_fwd.h"

#define DBG_LVL     DBG_INFO
#define LOG_TAG    "lfp.conf"
#include "lora_pkt_fwd_dbg.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define DEFAULT_RSSI_OFFSET -176
#define DEFAULT_NOTCH_FREQ  129000U

struct lgw_conf_usr_s lpf_conf_lgw;
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
const struct lgw_conf_usr_s lpf_conf_lgw_default =  
{
    .magic = 0xAA55,
    
    .lorawan_public = true,/* true for lorawan */

    .rxrf =
    {
        {
            .enable  = true,
            .invert_pol = false, /* false for lorawan */
            .freq_hz = LGD_RF_CHIAN0_CENTER_FREQ,
        },
        
        {
            .enable  = true,
            .invert_pol = false, /* false for lorawan */
            .freq_hz = LGD_RF_CHIAN1_CENTER_FREQ,
        },
    },
    
    .rxif = 
    {    
        .channel_if_enable =  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        .channel_if_rfchain = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 },
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
    
    .txrf =
    {
        .freq_hz = 475300000,
        .rf_power = 20,
        .datarate = 7,

         /* lora */
        .modulation = MOD_LORA,
        .invert_pol = false, /* true for lorawan downlink */
        .preamble = 8,
        .no_header = false, 
        .no_crc = false, /* true for lorawan downlink */
        .bandwidth = BW_125KHZ,
        .coderate = CR_LORA_4_5, 
        // FSK
        .f_dev = 25,//25kHz

        .tx_mode = IMMEDIATE,//TIMESTAMPED,
        
#ifdef LORA_GW_DRIVER_USING_SX1302_LSD4WN_2K830NE0
        .external_pa = true,
#endif            
    },
};

#ifdef PKG_USING_EASYFLASH  
static int lpf_get_lgw_nv_params(void *buff, int len);
static int lpf_get_srv_nv_params(void *buff, int len);
#endif

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DECLARATION ---------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */
#if 0
static void sig_handler(int sigio) {
    if (sigio == SIGQUIT) {
        quit_sig = 1;
    } else if ((sigio == SIGINT) || (sigio == SIGTERM)) {
        exit_sig = 1;
    }
}
#endif

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */
void lpf_nv_params_init(void)
{
#ifdef PKG_USING_EASYFLASH
    fal_init();
    easyflash_init();
#endif

     struct lgw_conf_usr_s lpf_conf_lgw_temp;
    /* get lpf chip Parameters */
     if ( lpf_get_lgw_nv_params(&lpf_conf_lgw_temp,LGW_CONF_USR_SIZE) == LGW_CONF_USR_SIZE && ( lpf_conf_lgw_temp.magic == 0xAA55 ))
     {
         lpf_conf_lgw = lpf_conf_lgw_temp;
         LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"Get LPF GW Chip Parameters Successed!\n");
     }
     else
     {
         LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"Get LPF GW Chip Parameters Failed,Use default Parameters!\n");
         lpf_conf_lgw = lpf_conf_lgw_default;
     }

    struct lpf_conf_srv_s lpf_conf_srv_temp;
     /* get lpf srv Parameters */
    if (lpf_get_srv_nv_params(&lpf_conf_srv_temp,LPF_CONF_SRV_SIZE) != LPF_CONF_SRV_SIZE )
    {
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"Get LPF Server Parameters Failed,Use default Parameters!\n");
        lpf_conf_srv = lpf_conf_srv_default[LPF_SUPPORTED_NETWORK_SERVER_LIERDA_UNICORE_GWMP];
    }
    else
    {
        lpf_conf_srv = lpf_conf_srv_temp;
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"Get LPF Server Parameters Successed!\n");
    }
}

int lpf_conf_lgw_nv_save(void *buff, int len)
{
#ifdef PKG_USING_EASYFLASH
    EfErrCode result = EF_NO_ERR;

    /* set and store the wlan config information to Env */
    result = ef_set_env_blob("lpf_conf_lgw", buff, len);
    if (result == EF_NO_ERR)
    {
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"LPF GW Chip Parameters(%d) Save Successed!\n", len);
        return RT_EOK;
    }
    else
    {
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"LPF GW Chip Parameters Save Fail,%d!\n", result);
        return -RT_ERROR;
    }
#else
    return -RT_ERROR;
#endif    
}

int lpf_conf_srv_nv_save(void *buff, int len)
{
#ifdef PKG_USING_EASYFLASH    
    EfErrCode result = EF_NO_ERR;
    
    /* set and store the lpf server config information to Env */
    result = ef_set_env_blob("lpf_conf_srv", buff, len);

    if (result == EF_NO_ERR)
    {
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"LPF SRV Parameters(%d) Save Successed!\n", len);
        return RT_EOK;
    }
    else
    {
        LPF_DEBUG_LOG(LPF_DBG_PARAMS_CONF, LOG_LVL_INFO,"LPF SRV Parameters Save Fail,%d!\n", result);
        return -RT_ERROR;
    }
#else    
    return -RT_ERROR;
#endif    
}

int lpf_get_lgw_nv_params(void *buff, int len)
{
    size_t saved_len = 0;
#ifdef PKG_USING_EASYFLASH
    ef_get_env_blob("lpf_conf_lgw", buff, len, &saved_len);
#endif
    return saved_len;
}

int lpf_get_srv_nv_params(void *buff, int len)
{
    size_t saved_len = 0;
#ifdef PKG_USING_EASYFLASH
    ef_get_env_blob("lpf_conf_srv", buff, len, &saved_len);
#endif
    return saved_len;
}


/* --- EOF ------------------------------------------------------------------ */
