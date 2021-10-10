
#include "rtthread.h"
#include <stdlib.h>         /* atol,atoi */
#include <string.h>         
#include <stdint.h>         /* C99 types */
#include <stdbool.h>        /* bool type */
#include <stdio.h>          /* fprintf, snprintf, fopen, fputs */
#include <inttypes.h>       /* PRIx64, PRIu64... */
#include <pthread.h>        /* ptherad */

#ifdef PKG_USING_EASYFLASH
#include <easyflash.h>
#endif
#include "loragw_usr.h"
#include "lora_pkt_fwd.h"

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "lpf.shell"
#include "lora_pkt_fwd_dbg.h"

extern pthread_mutex_t mx_concent;

const char *northboud_if_string[LPF_NORTHBOUND_IF_NUMS] = {"wifi","eth"};

typedef enum
{
    /* for lora packet sniffer finish\msh */
    CMD_LPF_PROBE_INDEX = 0,          // lps device probe
    CMD_LPF_SUSPEND_INDEX,            // lps suspend
    CMD_LPF_RESUME_INDEX,             // lps resume
    CMD_LPF_CFG_SAVE_INDEX,           // lps parameters save    
    CMD_LPF_FACTORY_INDEX,            // lps factory
    CMD_LPF_SRV_CONFIG_INDEX,         // lps network server setup
    CMD_LPF_NIF_CONFIG_INDEX,         // lps northboud interface setup
    CMD_LPF_RADIO_CONFIG_INDEX,       // lps northboud interface setup
} cmd_lpf_shell_t;

const char* lpf_help_info[] =
{
    [CMD_LPF_PROBE_INDEX]           = "lpf probe                     - lora packet forward probe",
    [CMD_LPF_SUSPEND_INDEX]         = "lps suspend                   - lora packet forward suspend(pause)",
    [CMD_LPF_RESUME_INDEX]          = "lps resume                    - lora packet forward resume(run)",
    [CMD_LPF_CFG_SAVE_INDEX]        = "lpf save                      - save parameters to flash",
    [CMD_LPF_FACTORY_INDEX]         = "lpf factory                   - restore factory settings",
    [CMD_LPF_SRV_CONFIG_INDEX]      = "lpf srv <cmd_type> <val..>    - config network server,<cmd_type>:name,addr,pup,pud,addr such as 192.168.1.10..",
    [CMD_LPF_NIF_CONFIG_INDEX]      = "lpf nif <type>                - config northbound interface parameters,such as wifi,eth..",
    [CMD_LPF_RADIO_CONFIG_INDEX]    = "lpf rxc <num><val..>          - config phy radio rx parameters,<cmd_type>: <rf0><rf1><auto><iq>..",
};

int lpf(int argc, char ** argv)
{
    size_t i = 0;

    if (argc < 2)
    {
        /* parameter error */
        LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "Usage:\n");
        for (i = 0; i < sizeof(lpf_help_info) / sizeof(char*); i++)
        {
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "%s\n", lpf_help_info[i]);
        }
        LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"\n");
    }
    else
    {
        const char *cmd = argv[1];

        if (!rt_strcmp(cmd, "probe"))
        {
            int32_t sx1302_chip_version = 0;
            int status = lgw_get_ver(&sx1302_chip_version);

            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"SX1302 version: 0x%02X\n", sx1302_chip_version);

            if( status == LGW_HAL_SUCCESS && sx1302_chip_version == 0x10 )
            {
                LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"SX1302 Access Successed\n");
            }
            else
            {
                LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"SX1302 Access Failed\n");
            }
            
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "SX1302 version: 0x%02X", sx1302_chip_version);
            
            uint64_t gw_eui;
            char gweui_string[17] = {0};
            status = lgw_get_eui(&gw_eui);

            snprintf(gweui_string, sizeof gweui_string, "%016"PRIX64, gw_eui);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "SX1302 EUI:%s",gweui_string);
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
                    lpf_conf_lgw.rxrf[0].enable   = enable;

                    lpf_conf_lgw.rxrf[0].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "rf1")) /* radio1 center frequence */
                {
                    lpf_conf_lgw.rxrf[1].enable   = enable;

                    lpf_conf_lgw.rxrf[1].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "auto")) /* setup start freq, 200k space, lgw auto setup radio0 and radio1 chanenl freq. */
                {
                    lpf_conf_lgw.rxrf[0].enable   = enable;
                    lpf_conf_lgw.rxrf[1].enable   = enable;

                    /* eg:set start 475.3 -> radio0 = 475.6 ,radio1 = 476.4 */
                    lpf_conf_lgw.rxrf[0].freq_hz = freq + LGD_RF_CHIAN0_CENTER_FREQ_OFFSET;
                    lpf_conf_lgw.rxrf[1].freq_hz = freq + LGD_RF_CHIAN1_CENTER_FREQ_OFFSET;
                }
            }

            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "-items-                     -{cmd}-   -Value-");
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " radio-chain-0 center freq   {rf0}:   %d,%d", lpf_conf_lgw.rxrf[0].freq_hz,lpf_conf_lgw.rxrf[0].enable);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " radio-chain-1 center freq   {rf1}:   %d,%d", lpf_conf_lgw.rxrf[1].freq_hz,lpf_conf_lgw.rxrf[1].enable);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " radio-chain-0-1 start freq  {auto}:  %d,%d", (lpf_conf_lgw.rxrf[0].freq_hz - LGD_RF_CHIAN0_CENTER_FREQ_OFFSET),lpf_conf_lgw.rxrf[0].enable);

            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_DBG, "== Channels Frequence Table: ==\r\n");
            for(uint8_t i = 0; i < 8; i++)
            {
                uint32_t ch_freq = lpf_conf_lgw.rxrf[lpf_conf_lgw.rxif.channel_if_rfchain[i]].freq_hz + lpf_conf_lgw.rxif.channel_if_freq[i];
                LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "CH[%i]:%d,rfchain:%d,%s\r\n", i, ch_freq, lpf_conf_lgw.rxif.channel_if_rfchain[i], lpf_conf_lgw.rxif.channel_if_enable[i] ? "enable" : "disable");
            }
        }
        else if (!rt_strcmp(cmd, "nif"))
        {
            const char *type = argv[2];
            if(argc >= 3)
            {
                if (!rt_strcmp(type, "eth"))
                {
                    lpf_conf_srv.northboud_if = LPF_NORTHBOUND_IF_OVER_ETHERNET;
                }
                else  if (!rt_strcmp(type, "wifi"))
                {
                    lpf_conf_srv.northboud_if = LPF_NORTHBOUND_IF_OVER_WIFI;
                }
            }
            else
            {
                LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "-items-         -{cmd}-   -Value-     -help-");
                LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " Northbound IF   {nif}     %s(%d):     wifi,eth", northboud_if_string[lpf_conf_srv.northboud_if], lpf_conf_srv.northboud_if);
            }
        }
        else if (!rt_strcmp(cmd, "srv"))
        {
            if(argc >= 4)
            {
                if (!rt_strcmp(argv[2], "name"))
                {
                    if (!rt_strcmp(argv[3], "unicore"))
                    {
                        lpf_conf_srv = lpf_conf_srv_default[LPF_SUPPORTED_NETWORK_SERVER_LIERDA_UNICORE_GWMP];
                    }
                    else if (!rt_strcmp(argv[3], "ttn"))
                    {
                        lpf_conf_srv = lpf_conf_srv_default[LPF_SUPPORTED_NETWORK_SERVER_TTN_GWMP];
                    }
                }                 
                if (!rt_strcmp(argv[2], "addr"))
                {
                    /* Server Address(IP address or domain name) */
                    strcpy(lpf_conf_srv.server_addr, argv[3]);
                }
                else if (!rt_strcmp(argv[2], "pup"))
                {
                    strcpy(lpf_conf_srv.server_port_up, argv[3]);
                }
                else if (!rt_strcmp(argv[2], "pdn"))
                {
                    strcpy(lpf_conf_srv.server_port_down, argv[3]);
                }
                else if (!rt_strcmp(argv[2], "json"))
                {
                    strcpy(lpf_conf_srv.json_conf_file,argv[3]);
                }
            }
            
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "-items-          -{cmd}-   -Value-");
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " server address   {addr}    %s", lpf_conf_srv.server_addr);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " port up          {pup}     %s", lpf_conf_srv.server_port_up);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " port down        {pdn}     %s", lpf_conf_srv.server_port_down);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, " server json file {json}    %s\r\n", lpf_conf_srv.json_conf_file );
        }
        else if(!rt_strcmp(cmd, "suspend"))
        {
            pthread_mutex_lock(&mx_concent);

            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "lpf rx suspend");
        }
        else if(!rt_strcmp(cmd, "resume"))
        {
            pthread_mutex_unlock(&mx_concent);
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO, "lpf rx resume");
        }
        else if(!rt_strcmp(cmd,"save"))
        {
#ifdef PKG_USING_EASYFLASH 
            /* save lora pkt fwd lgw configuration to flash */
            lpf_conf_lgw_nv_save(&lpf_conf_lgw.magic, LGW_CONF_USR_SIZE);
            /* save lora pkt fwd server configuration to flash */
            lpf_conf_srv_nv_save(&lpf_conf_srv.magic,LPF_CONF_SRV_SIZE);
            
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"save done\r\n");
#else
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"Not Support to Save\r\n");
#endif            
        }
        else if(!rt_strcmp(cmd,"factory"))
        {
            lpf_conf_lgw = lpf_conf_lgw_default;
            lpf_conf_srv = lpf_conf_srv_default[LPF_SUPPORTED_NETWORK_SERVER_LIERDA_UNICORE_GWMP];
#ifdef PKG_USING_EASYFLASH  
            /* save lora pkt fwd lgw configuration to flash */
            lpf_conf_lgw_nv_save(&lpf_conf_lgw.magic,LGW_CONF_USR_SIZE);
            /* save lora pkt fwd server configuration to flash */
            lpf_conf_srv_nv_save(&lpf_conf_srv.magic,LPF_CONF_SRV_SIZE);
#endif            
            LPF_DEBUG_LOG(LPF_DBG_SHELL, LOG_LVL_INFO,"factory done\r\n");
        }
    }
    return 1;
}

MSH_CMD_EXPORT(lpf, lora pakcet forward shell);
