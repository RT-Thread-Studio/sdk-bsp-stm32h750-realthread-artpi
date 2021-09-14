
#include "board.h"
#include <rtthread.h>
#include "loragw_hal.h"
#include "loragw_usr.h"
#include "lora_pkt_sniffer.h"

#include <easyflash.h>
#include <fal.h>

#define DBG_LVL             DBG_INFO
#define LOG_TAG             "LPS.NVM"
#include "lora_pkt_sniffer_dbg.h"

static int lps_conf_sys_info_get(void *buff, int len);
static int lps_conf_lgw_info_get(void *buff, int len);
static int lps_conf_net_info_get(void *buff, int len);

#ifdef PKG_USING_EASYFLASH

int16_t lps_nvm_init(void)
{
    fal_init();
    easyflash_init();

    struct lps_conf_sys_s lps_conf_sys_temp;
    struct lgw_conf_usr_s lps_conf_lgw_temp;
    struct lps_conf_srv_s lps_conf_srv_temp;
    
    /* get lps sys config Parameters */
    if (lps_conf_sys_info_get(&lps_conf_sys_temp.magic, LPS_CONF_SYS_SIZE) == LPS_CONF_SYS_SIZE && ( lps_conf_sys_temp.magic == 0xAA55 ))
    {
        lps_conf_sys = lps_conf_sys_temp;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS System Parameters(NV) Successed!\n");
    }
    else
    {
        lps_conf_sys = lps_conf_sys_default;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS System Parameters(NV) Failed,Use Default Parameters!\n");
    }

    /* get lps lgw config Parameters */
    if (lps_conf_lgw_info_get(&lps_conf_lgw_temp.magic, LGW_CONF_USR_SIZE) != LGW_CONF_USR_SIZE)
    {
        lps_conf_lgw = lps_conf_lgw_default;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS LoRa GW Chip Parameters(NV) Failed,Use Default Parameters!\n");
    }
    else
    {
        lps_conf_lgw = lps_conf_lgw_temp;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS LoRa GW Chip Parameters(NV) Successed!\n");
    }

    /* get lps net config Parameters */
    if (lps_conf_net_info_get(&lps_conf_srv_temp.magic, LPS_CONF_SRV_SIZE) != LPS_CONF_SRV_SIZE)
    {
        lps_conf_srv = lps_conf_srv_default;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS Network Server Parameters(NV) Failed,Use Default Parameters!\n");
    }
    else
    {
        lps_conf_srv = lps_conf_srv_temp;
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "Get LPS Network Server Parameters(NV) Successed!\n");
    }

    return 1;
}

int lps_conf_sys_save(void *buff, int len)
{
    EfErrCode result = EF_NO_ERR;

    /* set and store the sys config information to Env */
    result = ef_set_env_blob("sys_conf_info", buff, len);

    if (result == EF_NO_ERR)
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS System Parameters(%d) Save Successed!\n", len);
    }
    else
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS System Parameters Save Fail,%d!\n", result);
    }

    return result;
}

int lps_conf_lgw_save(void *buff, int len)
{
    EfErrCode result = EF_NO_ERR;

    /* set and store the wlan config information to Env */
    result = ef_set_env_blob("lgw_conf_info", buff, len);

    if (result == EF_NO_ERR)
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS lgw Parameters(%d) Save Successed!\n", len);
    }
    else
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS lgw Parameters Save Fail,%d!\n", result);
    }

    return result;
}

int lps_conf_srv_save(void *buff, int len)
{
    EfErrCode result = EF_NO_ERR;

    /* set and store the wlan config information to Env */
    result = ef_set_env_blob("net_conf_info", buff, len);

    if (result == EF_NO_ERR)
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS Network Server Parameters(%d) Save Successed!\n", len);
    }
    else
    {
        LPS_DEBUG_LOG(LPS_DBG_NVM, DBG_LVL, "LPS Network Server Parameters Save Fail,%d!\n", result);
    }

    return len;
}

int lps_conf_sys_info_get(void *buff, int len)
{
    size_t saved_len;

    ef_get_env_blob("sys_conf_info", buff, len, &saved_len);

    if (saved_len == 0)
    {
        return 0;
    }

    return len;
}

int lps_conf_lgw_info_get(void *buff, int len)
{
    size_t saved_len;

    ef_get_env_blob("lgw_conf_info", buff, len, &saved_len);

    if (saved_len == 0)
    {
        return 0;
    }

    return len;
}

int lps_conf_net_info_get(void *buff, int len)
{
    size_t saved_len;

    ef_get_env_blob("net_conf_info", buff, len, &saved_len);

    if (saved_len == 0)
    {
        return 0;
    }

    return len;
}

#endif
