
#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>       /* PRIx64, PRIu64... */
#include <stdio.h>          /* fprintf, snprintf, fopen, fputs */

#include "loragw_usr.h"
#include "lora_pkt_sniffer.h"

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LPS.SHELL"
#include "lora_pkt_sniffer_dbg.h"

typedef enum
{
    /* for lora packet sniffer finish\msh */
    CMD_LPS_RAIDO_PROBE_INDEX = 0,    // lps device probe
    CMD_LPS_SUSPEND_INDEX,            // lps suspend
    CMD_LPS_RESUME_INDEX,             // lps resume
    CMD_LPS_REBOOT_INDEX,             // lps restart
    CMD_LPS_SAVE_INDEX,               // lps parameters save    
    CMD_LPS_FACTORY_INDEX,            // lps factory
    CMD_LPS_NETWORK_SERVER_INDEX,     // lps network server setup
    CMD_LPS_NORTHBOUND_IF_INDEX,      // lps northboud interface setup
    CMD_LPS_DEVEUI_INFO_INDEX,        // lps deveui info setup
    CMD_LPS_DEVADDR_INFO_INDEX,       // lps devaddr info setup
    CMD_LPS_LORA_MAC_INDEX,           // lps loramac parameters config
    CMD_LPS_LORA_PHY_INDEX,           // lps information filter
    CMD_LPS_RADIO_RX_CONFIG_INDEX,    // lps Radio rx config
} cmd_lps_shell_t;

const char* lora_packet_sniffer_help_info[] =
{
    [CMD_LPS_RAIDO_PROBE_INDEX]     = "lps probe                                        - lora packet sniffer probe",
    [CMD_LPS_SUSPEND_INDEX]         = "lps suspend                                      - lora packet sniffer suspend(pause)",
    [CMD_LPS_RESUME_INDEX]          = "lps resume                                       - lora packet sniffer resume(run)",
    [CMD_LPS_REBOOT_INDEX]          = "lps reboot                                       - lora packet sniffer reboot",
    [CMD_LPS_SAVE_INDEX]            = "lps save                                         - lora packet sniffer parameters save to nvm",
    [CMD_LPS_FACTORY_INDEX]         = "lps factory                                      - lora packet sniffer parameters restore to factory",  
    [CMD_LPS_NETWORK_SERVER_INDEX]  = "lps srv <lns> <port>                             - config network server,such as 192.168.1.10",
    [CMD_LPS_NORTHBOUND_IF_INDEX]   = "lps nif <type>                                   - config northbound interface parameters,such as uart,wifi,eth..",
    [CMD_LPS_DEVEUI_INFO_INDEX]     = "lps deveui <eui><cmd_tpye>[<appeui><appkey>]     - config dev deveui info,<cmd_tpye>:<add|del|whitelist|fcnt..>,such as [lps deveui add appeui appkey]..",
    [CMD_LPS_DEVADDR_INFO_INDEX]    = "lps devaddr <addr><cmd_tpye>[<appskey><nwkskey>] - config dev devaddr info,<cmd_tpye>:<add|del|whitelist|fcnt..>,such as [lps devaddr add nwkskey appskey]..",
    [CMD_LPS_LORA_MAC_INDEX]        = "lps mac <lorawan_public>                         - config lora mac parameters,such as lorawan public..",  
    [CMD_LPS_LORA_PHY_INDEX]        = "lps phy <crc>                                    - config phy frame paramters,such as crc filter mode..",
    [CMD_LPS_RADIO_RX_CONFIG_INDEX] = "lps rxc <cmd_tpye> <val>                         - config phy radio rx parameters,<cmd_tpye>:<rf0|rf1|auto|iq..>", 
};

extern bool lps_initialized;
/**
 * @brief  get_hex_byte
 * @param **string
 * @returns
 */
static uint8_t get_hex_byte(char **hex_string)
{
    char temp[3] = { 0 };
    char *string_ptr = *hex_string;
    uint8_t value;

    for(uint8_t i = 0; i < 2; i++)
    {
        temp[i] = *string_ptr++;
    }

    *hex_string = string_ptr;

    value = strtol(temp, 0, 16);

    return value;
}

int lps(int argc, char **argv)
{
    size_t i = 0;

    if (argc < 2)
    {
        /* parameter error */
        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "Usage:\n");

        for (i = 0; i < sizeof(lora_packet_sniffer_help_info) / sizeof(char*); i++)
        {
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "%s\n", lora_packet_sniffer_help_info[i]);
        }

        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "\n");
    }
    else
    {
        const char *cmd = argv[1];

        /* creat lora packet sniffer test thread */
        if( lps_init() == false )
        {
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "LoRa Pkt Sniffer Init Failed");
            return 0;
        }

        if (!rt_strcmp(cmd, "probe"))
        {
            int32_t sx1302_chip_version = 0;
            int status = lgw_get_ver(&sx1302_chip_version);

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "SX1302 version: 0x%02X", sx1302_chip_version);

            if( status == LGW_HAL_SUCCESS && sx1302_chip_version == 0x10 )
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, DBG_LVL, "SX1302 Access Successed");
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, DBG_LVL, "SX1302 Access Failed");
            }
            
            // get sx1302 eui
            uint64_t gw_eui;
            char gweui_string[17] = {0};
            status = lgw_get_eui(&gw_eui);

            snprintf(gweui_string, sizeof gweui_string, "%016"PRIX64, gw_eui);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "SX1302 EUI:%s",gweui_string);
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
                    lps_conf_lgw.rxrf[0].enable   = enable;

                    lps_conf_lgw.rxrf[0].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "rf1")) /* radio1 center frequence */
                {
                    lps_conf_lgw.rxrf[1].enable   = enable;

                    lps_conf_lgw.rxrf[1].freq_hz  = freq;
                }
                else if (!rt_strcmp(cmd1, "auto")) /* setup start freq, 200k space, lgw auto setup radio0 and radio1 chanenl freq. */
                {
                    lps_conf_lgw.rxrf[0].enable   = enable;
                    lps_conf_lgw.rxrf[1].enable   = enable;

                    /* eg:set start 475.3 -> radio0 = 475.6 ,radio1 = 476.4 */
                    lps_conf_lgw.rxrf[0].freq_hz = freq + LGD_RF_CHIAN0_CENTER_FREQ_OFFSET;
                    lps_conf_lgw.rxrf[1].freq_hz = freq + LGD_RF_CHIAN1_CENTER_FREQ_OFFSET;
                }
                else if (!rt_strcmp(cmd1, "iq"))
                {
                    if (argc >= 4)
                    {
                        lps_conf_lgw.rxrf[0].invert_pol = atoi(argv[3]);
                    }
                }
            }

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-                     -{cmd}-   -Value-");
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " radio-chain-0 center freq   {rf0}:   %d,%d", lps_conf_lgw.rxrf[0].freq_hz, lps_conf_lgw.rxrf[0].enable);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " radio-chain-1 center freq   {rf1}:   %d,%d", lps_conf_lgw.rxrf[1].freq_hz, lps_conf_lgw.rxrf[1].enable);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " radio-chain-0-1 start freq  {auto}:  %d,%d", (lps_conf_lgw.rxrf[0].freq_hz - LGD_RF_CHIAN0_CENTER_FREQ_OFFSET), lps_conf_lgw.rxrf[0].enable);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " rx iq invert                {iq}:    %d", lps_conf_lgw.rxrf[0].invert_pol);

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_DBG, "== Channels Frequence Table: ==\r\n");

            for(uint8_t i = 0; i < 8; i++)
            {
                uint32_t ch_freq = lps_conf_lgw.rxrf[lps_conf_lgw.rxif.channel_if_rfchain[i]].freq_hz + lps_conf_lgw.rxif.channel_if_freq[i];
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_DBG, "CH[%i]:%d,rfchain:%d,%s\r\n", i, ch_freq, lps_conf_lgw.rxif.channel_if_rfchain[i], lps_conf_lgw.rxif.channel_if_enable[i] ? "enable" : "disable");
            }
        }
        else if (!rt_strcmp(cmd, "nif"))
        {
            if(argc >= 3)
            {
                const char *northboud_if_type = argv[2];

                if (!rt_strcmp(northboud_if_type, "eth"))
                {
                    lps_conf_sys.northboud_if = LPS_NORTHBOUND_IF_OVER_ETHERNET;
                }
                else  if (!rt_strcmp(northboud_if_type, "wifi"))
                {
                    lps_conf_sys.northboud_if = LPS_NORTHBOUND_IF_OVER_WIFI;
                }
                else  if (!rt_strcmp(northboud_if_type, "uart"))
                {
                    lps_conf_sys.northboud_if = LPS_NORTHBOUND_IF_OVER_SERIAL;
                }
                else  if (!rt_strcmp(northboud_if_type, "?"))
                {
                    LPS_DEBUG_LOG(LPS_DBG_SHELL, DBG_LVL, "nif:");

                    for (i = 0; i < LPS_NORTHBOUND_IF_NUMS; i++)
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, DBG_LVL, "  %d - %s", i, northboud_if_string[i]);
                    }
                }

                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "Northbound IF:%s(%d)", northboud_if_string[lps_conf_sys.northboud_if], lps_conf_sys.northboud_if);
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-         -{cmd}-   -Value-     -help-");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " Northbound IF   {nif}     %s(%d):    uart,wifi,eth", northboud_if_string[lps_conf_sys.northboud_if], lps_conf_sys.northboud_if);
            }
        }
        else if (!rt_strcmp(cmd, "srv"))
        {
            if(argc >= 4)
            {
                if (!rt_strcmp(argv[2], "addr"))
                {
                    /* Server Address(IP address or domain name) */
                    strcpy(lps_conf_srv.server_addr, argv[3]);
                }
                else if (!rt_strcmp(argv[2], "pup"))
                {
                    strcpy(lps_conf_srv.server_port_up, argv[3]);
                }
                else if (!rt_strcmp(argv[2], "pdn"))
                {
                    strcpy(lps_conf_srv.server_port_down, argv[3]);
                }
            }

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-         -{cmd}-   -Value-");
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " server address  {addr}    %s", lps_conf_srv.server_addr);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " port up         {pup}     %s", lps_conf_srv.server_port_up);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " port down       {pdn}     %s", lps_conf_srv.server_port_down);
        }
        else if(!rt_strcmp(cmd, "deveui")) // otaa
        {
            // eg: lps deveui 1122334455667788 add [0011223344556677] [00112233445566778899aabbccddeeff]
            if(argc >= 4)
            {
                dev_id_info_t deveui = { .size = 8 };
                uint8_t appeui[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x80 };
                uint8_t appkey[16] = { 0 };
                int status;
                
                /* deveui */
                for(uint8_t i = 0; i < 8; i++)
                {
                    deveui.data[i] = get_hex_byte(&argv[2]);
                }

                // init default value for appkey[16] = deveui[8] << 64| deveui[8]
                for(uint8_t i = 0; i < 16; i++)
                {
                    appkey[i] = deveui.data[i % 8];
                }

                if (!rt_strcmp(argv[3], "add"))
                {
                    if(argc >= 5)
                    {
                        /* appeui */
                        for(uint8_t i = 0; i < 8; i++)
                        {
                            appeui[i] = get_hex_byte(&argv[4]);
                        }

                        if(argc >= 6)
                        {
                            /* appkey */
                            for(uint8_t i = 0; i < 16; i++)
                            {
                                appkey[i] = get_hex_byte(&argv[5]);
                            }
                        }
                    }

                    /* regiter a lorawan otaa end-device,if up freq = downlink freq,eg: EU868..*/
                    status = lgw_service_lorawan_end_device_register(&deveui, appeui, appkey);

                    if(status == LGW_HAL_SUCCESS )
                    {                    
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X Register Success",
                                      HEX8(deveui.data));
                   
                    }
                    else
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X Register Fail",
                                      HEX8(deveui.data));
                    }
                }
                else if (!rt_strcmp(argv[3], "del"))
                {
                    status = lgw_service_lorawan_end_device_unregister(&deveui);
                    if(status == LGW_HAL_SUCCESS )
                    {                    
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X Delete Success",
                                      HEX8(deveui.data));
                   
                    }
                    else
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X Delete Fail",
                                      HEX8(deveui.data));
                    }
                }
                else if (!rt_strcmp(argv[3], "fcnt"))
                {
                    uint32_t uplink_counter = END_DEVICE_FCNT_MODIFY_IGNORE;
                    uint32_t downlink_counter = END_DEVICE_FCNT_MODIFY_IGNORE;

                    if(argc >= 5)
                    {
                        uplink_counter = atol(argv[4]);

                        if(argc >= 5)
                        {
                            downlink_counter = atol(argv[5]);
                        }
                    }

                    lgw_service_lorawan_end_device_fcnt_modify(&deveui, uplink_counter, downlink_counter);
                    LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X,Uplink Counter:%d, downlink_counter:%d",HEX8(deveui.data),uplink_counter,downlink_counter);
                }
                else if (!rt_strcmp(argv[3], "whitelist"))
                {
                    rt_memset(lps_conf_sys.deveui_white_list,0,8);
                    /* deveui filter */
                    for(uint8_t i = 0; i < 8; i++)
                    {
                        lps_conf_sys.deveui_white_list[i] = deveui.data[i];
                    }
                    
                    LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "DevEUI White List(only for join request):DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X",
                                  HEX8(lps_conf_sys.deveui_white_list));
                }
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-           -{cmd1}-    -{cmd2}-     -parameter discription-");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " add a deveui      {deveui}    {add}        [appeui],[appkey]");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " del a deveui      {deveui}    {del}        ");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " set up|dn counter {deveui}    {fcnt}       uplink_fcnt,downlink_fcnt");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " set whitelist     {deveui}    {whitelist}  ");

                lgw_service_lorawan_end_devices_info_list();
            }
        }
        else if(!rt_strcmp(cmd, "devaddr")) // abp or otaa
        {
            // eg: lps devaddr 11223344 add [00112233445566778899aabbccddeeff]
            if(argc >= 4)
            {
                dev_id_info_t devaddr = { .size = 4 };
                uint8_t appskey[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
                uint8_t nwkskey[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
                int status;
                
                /* devaddr */
                for(uint8_t i = 0; i < 4; i++)
                {
                    devaddr.data[i] = get_hex_byte(&argv[2]);
                }

                if (!rt_strcmp(argv[3], "add"))
                {
                    if(argc >= 5)
                    {
                        /* appskey */
                        for(uint8_t i = 0; i < 16; i++)
                        {
                            appskey[i] = get_hex_byte(&argv[4]);
                        }

                        if(argc >= 5)
                        {
                            /* nwkskey */
                            for(uint8_t i = 0; i < 16; i++)
                            {
                                nwkskey[i] = get_hex_byte(&argv[5]);
                            }
                        }
                    }

                    /* regiter a lorawan a end-device by devaddr  */
                    status = lgw_service_lorawan_end_device_register(&devaddr, appskey, nwkskey);

                    if(status == LGW_HAL_SUCCESS )
                    {                       
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevAddr=%02X%02X%02X%02X Register Success",
                                      HEX8(devaddr.data));
                   
                    }
                    else
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevAddr=%02X%02X%02X%02X Register Fail",
                                      HEX8(devaddr.data));
                    }
                }
                else if (!rt_strcmp(argv[3], "del"))
                {
                    status = lgw_service_lorawan_end_device_unregister(&devaddr);
                    if(status == LGW_HAL_SUCCESS )
                    {                       
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevAddr=%02X%02X%02X%02X Register Success",
                                      HEX8(devaddr.data));
                   
                    }
                    else
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevAddr=%02X%02X%02X%02X Register Fail",
                                      HEX8(devaddr.data));
                    }
                }
                else if (!rt_strcmp(argv[3], "whitelist"))
                {
                    lps_conf_sys.devaddr_white_list = 0;
                    
                    /* devaddr filter */
                    for(uint8_t i = 0; i < 4; i++)
                    {
                        lps_conf_sys.devaddr_white_list |= devaddr.data[i]<<((3-i)<<3);
                    }
                    
                    if( lps_conf_sys.devaddr_white_list != LPS_DEVADDR_WHITE_LIST_DISABLE )
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "Devaddr White List:DevAddr=%08X",lps_conf_sys.devaddr_white_list);
                    }
                    else
                    {
                        LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "Devaddr White List:Disable");
                    }
                }
                else if (!rt_strcmp(argv[3], "fcnt"))
                {
                    uint32_t uplink_counter = END_DEVICE_FCNT_MODIFY_IGNORE;
                    uint32_t downlink_counter = END_DEVICE_FCNT_MODIFY_IGNORE;

                    if(argc >= 5)
                    {
                        uplink_counter = atol(argv[4]);

                        if(argc >= 5)
                        {
                            downlink_counter = atol(argv[5]);
                        }
                    }

                    lgw_service_lorawan_end_device_fcnt_modify(&devaddr, uplink_counter, downlink_counter);
                    
                    LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "End Device:DevAddr=0x%02X%02X%02X%02X,Uplink Counter:%d, downlink_counter:%d",HEX8(devaddr.data),uplink_counter,downlink_counter);
                }
            }
            else
            {
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-           -{cmd1}-    -{cmd2}-     -parameter discription-");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " add a devaddr     {devaddr}   {add}        [appskey],[nwkskey]");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " del a devaddr     {devaddr}   {del}        ");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " set up|dn counter {devaddr}   {fcnt}       uplink_fcnt,downlink_fcnt");
                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " set whitelist     {devaddr}   {whitelist}  ");

                lgw_service_lorawan_end_devices_info_list();
            }
        }
        else if (!rt_strcmp(cmd, "mac"))
        {
            const char *cmd1 = argv[2];

            if (!rt_strcmp(cmd1, "lorawan"))
            {
                if (argc >= 4)
                {
                    lps_conf_lgw.lorawan_public = atoi(argv[3]);
                }

                LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "Public Network: %d", lps_conf_lgw.lorawan_public);
            }

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-         -{cmd}-     -Value-");
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " Public Network  {lorawan}:  %d(%s)", lps_conf_lgw.lorawan_public,lps_conf_lgw.lorawan_public?"public":"private");
        }
        else if(!rt_strcmp(cmd, "phy"))
        {
            if (!rt_strcmp(argv[2], "crc"))
            {
                lps_conf_sys.crc_filter_mode = (lps_lora_packet_crc_filter_t)atoi(argv[3]);
            }
            
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "-items-         -{cmd}-   -Value-");
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, " CRC Filter Mode {crc}     %d(%s)",lps_conf_sys.crc_filter_mode,crc_filter_strings[lps_conf_sys.crc_filter_mode]);
        }
        else if(!rt_strcmp(cmd, "suspend"))
        {
            lps_rx_wait_time = RT_WAITING_FOREVER;
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "lps rx suspend");
        }
        else if(!rt_strcmp(cmd, "resume"))
        {
            lps_rx_wait_time = rt_tick_from_millisecond(LPS_FETCH_RX_BUFFER_INTERVAL_MS);
            rt_sem_release(&lps_rx_sem);
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "lps rx resume");
        }
        else if (!rt_strcmp(cmd, "reboot"))
        {
            /* reinit lps parameters */
            lps_rx_wait_time = rt_tick_from_millisecond(LPS_FETCH_RX_BUFFER_INTERVAL_MS);
            rt_sem_release(&lps_rx_sem);
            lps_initialized = false;
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "reboot..\r\n");
        }
        else if(!rt_strcmp(cmd, "save"))
        {
            #ifdef PKG_USING_EASYFLASH
            /* save lps sys configuaration to flash */
            lps_conf_sys_save(&lps_conf_sys.magic, LPS_CONF_SYS_SIZE);
            /* save lps rf configuration to flash */
            lps_conf_lgw_save(&lps_conf_lgw.magic, LGW_CONF_USR_SIZE);
            /* save lps network server configuration to flash */
            lps_conf_srv_save(&lps_conf_srv.magic, LPS_CONF_SRV_SIZE);

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "save\r\n");
            #else
            LPS_DEBUG_LOG(LPS_DBG_SHELL, DBG_LVL, "Not Supported\r\n");
            #endif
        }
        else if(!rt_strcmp(cmd,"factory"))
        {
            lps_conf_lgw = lps_conf_lgw_default;
            lps_conf_srv = lps_conf_srv_default;
            #ifdef PKG_USING_EASYFLASH
            /* save lps sys configuaration to flash */
            lps_conf_sys_save(&lps_conf_sys.magic, LPS_CONF_SYS_SIZE);
            /* save lps rf configuration to flash */
            lps_conf_lgw_save(&lps_conf_lgw.magic, LGW_CONF_USR_SIZE);
            /* save lps network server configuration to flash */
            lps_conf_srv_save(&lps_conf_srv.magic, LPS_CONF_SRV_SIZE);

            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO, "save\r\n");
            #endif
            LPS_DEBUG_LOG(LPS_DBG_SHELL, LOG_LVL_INFO,"factory done\r\n");
        }
    }

    return 1;
}

MSH_CMD_EXPORT(lps, lora packet sniffer shell);
