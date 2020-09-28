#include "common.h"
#include <fal.h>
#include <easyflash.h>
#include <webnet.h>
#include <cJSON.h>
#include <dfs_posix.h>
#include "init_module.h"
#include "fs_operation.h"
#include "plugins.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static struct global_sundry g_sundry = {0};

const struct global_sundry *global_sundry_get(void)
{
    return (const struct global_sundry *)&g_sundry;
}
RTM_EXPORT(global_sundry_get);

static int global_sundry_default_init(void)
{
    rt_memset(&g_sundry, 0, sizeof(struct global_sundry));

    char env_buf[100];
    int env_len;

    int op_path = 0;
    env_len = ef_get_env_blob("operation_path", env_buf, sizeof(env_buf), RT_NULL);
    if(env_len >= sizeof(env_buf))
        env_len = sizeof(env_buf) - 1;
    if(env_len <= 0)
        ef_set_env("operation_path", "0");
    else
    {
        env_buf[env_len] = '\0';
        op_path = atoi(env_buf);
    }
    switch(op_path)
    {
        case 1:
            g_sundry.operation_path = SD_PATH;
        break;

        case 2:
            g_sundry.operation_path = USB_PATH;
        break;

        default:
            g_sundry.operation_path = NORFLASH_PATH;
        break;
    }
    LOG_I("The operation path is \"%s\"", g_sundry.operation_path);

    char *sn = DEFAULT_SERIAL_NUMBER;
    env_len = ef_get_env_blob("serial_number", env_buf, sizeof(env_buf), RT_NULL);
    if(env_len >= sizeof(env_buf))
        env_len = sizeof(env_buf) - 1;
    if(env_len <= 0)
        ef_set_env("serial_number", DEFAULT_SERIAL_NUMBER);
    else
    {
        env_buf[env_len] = '\0';
        sn = rt_strdup(env_buf);
    }
    g_sundry.serial_number = sn;

    return RT_EOK;
}

static void run_time_calc_hook(void)
{
    static rt_tick_t prev_tick = 0;
    static rt_tick_t redress = 0;
    rt_tick_t now_tick = rt_tick_get();
    rt_tick_t diff_tick = 0;

    if(now_tick >= prev_tick)
        diff_tick = now_tick - prev_tick;
    else
        diff_tick = RT_TICK_MAX - prev_tick + now_tick;
    diff_tick += redress;

    uint32_t sec_part = diff_tick / RT_TICK_PER_SECOND;
    if(sec_part > 0)
    {
        prev_tick = now_tick;
        redress = diff_tick % RT_TICK_PER_SECOND;
        g_sundry.run_time += sec_part;
    }
}

static int system_configure_parameter(void)
{
    char fs_path[256];
    snprintf(fs_path, sizeof(fs_path), "%s/plugins", g_sundry.operation_path);
    fs_create_dir(fs_path);
    snprintf(fs_path, sizeof(fs_path), "%s/sys", g_sundry.operation_path);
    fs_create_dir(fs_path);


    int time_zone = 8;
    int webnet_port = 80;
    char *webnet_root = RT_NULL;
    uint8_t need_overwrite = 0;
    
    strcat(fs_path, "/cfg.json");
    char *cfg_buf = fs_read_file(fs_path);
    do
    {
        if(cfg_buf == RT_NULL)
        {
            need_overwrite = 1;
            break;
        }
        cJSON *root = cJSON_Parse(cfg_buf);
        rt_free(cfg_buf);
        if(root == RT_NULL)
        {
            need_overwrite = 1;
            break;
        }
        
        cJSON *set_time_zone = cJSON_GetObjectItem(root, "time_zone");
        cJSON *set_webnet_root = cJSON_GetObjectItem(root, "webnet_root");
        cJSON *set_webnet_port = cJSON_GetObjectItem(root, "webnet_port");

        if(set_time_zone && ((set_time_zone->type & 0xFF) == cJSON_String))
        {
            time_zone = atoi(set_time_zone->valuestring);
        }
        else
            need_overwrite = 1;
        
        if(set_webnet_root && ((set_webnet_root->type & 0xFF) == cJSON_String))
        {
            webnet_root = rt_strdup(set_webnet_root->valuestring);
        }
        else
            need_overwrite = 1;
        
        if(set_webnet_port && ((set_webnet_port->type & 0xFF) == cJSON_String))
        {
            int t = atoi(set_webnet_port->valuestring);
            if(t > 0)
                webnet_port = t;
            else
                need_overwrite = 1;
        }
        else
            need_overwrite = 1;

        cJSON_Delete(root);
    }while(0);

    g_sundry.time_zone = time_zone;
    webnet_set_port(webnet_port);
    if(webnet_root == RT_NULL)
        webnet_set_root(g_sundry.operation_path);
    else
        webnet_set_root(webnet_root);
    
    if(need_overwrite)
    {
        LOG_I("write cfg to sys/cfg.json.");

        char tmp[20];
        cJSON *root = cJSON_CreateObject();
        snprintf(tmp, sizeof(tmp), "%d", time_zone);
        cJSON_AddItemToObject(root, "time_zone", cJSON_CreateString(tmp));
        if(webnet_root == RT_NULL)
            cJSON_AddItemToObject(root, "webnet_root", cJSON_CreateString(g_sundry.operation_path));
        else
            cJSON_AddItemToObject(root, "webnet_root", cJSON_CreateString(webnet_root));
        snprintf(tmp, sizeof(tmp), "%d", webnet_port);
        cJSON_AddItemToObject(root, "webnet_port", cJSON_CreateString(tmp));

        cfg_buf = cJSON_PrintUnformatted(root);
        cJSON_Delete(root);

        do
        {
            if(cfg_buf == RT_NULL)
                break;
            
            FILE *fp = fopen(fs_path, "wb");
            if(fp == RT_NULL)
                break;
            fwrite(cfg_buf, rt_strlen(cfg_buf), 1, fp);
            fclose(fp);
        }while(0);

        rt_free(cfg_buf);
    }

    if(webnet_root)
        rt_free(webnet_root);

    return RT_EOK;
}

int main(void)
{
    HAL_PWR_EnableBkUpAccess();
    RT_ASSERT(fal_init() > 0);
    RT_ASSERT(easyflash_init() == EF_NO_ERR);

    global_sundry_default_init();
    rt_thread_idle_sethook(run_time_calc_hook);
    fs_init(&g_sundry);
    system_configure_parameter();
    plugins_init();
    init_module_init();

    webnet_init();
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


