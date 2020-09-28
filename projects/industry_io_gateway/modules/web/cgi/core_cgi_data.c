#include "core_cgi_data.h"
#include "plugins.h"
#include <cJSON.h>
#include <dfs_fs.h>
#include <arpa/inet.h>
#include <netdev.h>

char *core_cgi_serialize_basic_info(void)
{
    char tmp[100];

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "code", cJSON_CreateNumber(0));
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);

    rt_uint32_t total_mem = 0, used_mem = 0, max_used_mem = 0;
    rt_object_t mhob = rt_object_find("heap", RT_Object_Class_MemHeap);
    if(mhob)
    {
        struct rt_memheap *mh = (struct rt_memheap *)mhob;
        total_mem += mh->pool_size;
        used_mem += (mh->pool_size - mh->available_size);
        max_used_mem += mh->max_used_size;
    }

    mhob = rt_object_find("sdram", RT_Object_Class_MemHeap);
    if(mhob)
    {
        struct rt_memheap *mh = (struct rt_memheap *)mhob;
        total_mem += mh->pool_size;
        used_mem += (mh->pool_size - mh->available_size);
        max_used_mem += mh->max_used_size;
    }

    snprintf(tmp, sizeof(tmp), "%u", total_mem);
    cJSON_AddItemToObject(payload, "id0", cJSON_CreateString(tmp));
    snprintf(tmp, sizeof(tmp), "%u", used_mem);
    cJSON_AddItemToObject(payload, "id1", cJSON_CreateString(tmp));
    snprintf(tmp, sizeof(tmp), "%u", max_used_mem);
    cJSON_AddItemToObject(payload, "id2", cJSON_CreateString(tmp));

    const struct global_sundry *g_sundry = global_sundry_get();
    snprintf(tmp, sizeof(tmp), "%d", g_sundry->time_zone);
    cJSON_AddItemToObject(payload, "id3", cJSON_CreateString(tmp));
    time_t now_time = time(RT_NULL);
    now_time += g_sundry->time_zone * 3600;
    struct tm local_tm = {0};
    localtime_r(&now_time, &local_tm);
    local_tm.tm_year += 1900;
    local_tm.tm_mon += 1;
    snprintf(tmp, sizeof(tmp), "%d-%02d-%02d %d:%02d:%02d", local_tm.tm_year, local_tm.tm_mon, 
             local_tm.tm_mday, local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);
    cJSON_AddItemToObject(payload, "id4", cJSON_CreateString(tmp));

    cJSON_AddItemToObject(payload, "id5", cJSON_CreateString(g_sundry->operation_path));

    struct statfs buffer;
    if(dfs_statfs(NORFLASH_PATH, &buffer) != 0)
    {
        snprintf(tmp, sizeof(tmp), "%d", 0);
        cJSON_AddItemToObject(payload, "id6", cJSON_CreateString(tmp));
        cJSON_AddItemToObject(payload, "id7", cJSON_CreateString(tmp));
    }
    else
    {
        uint64_t total_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)buffer.f_blocks);
        snprintf(tmp, sizeof(tmp), "%llu", total_fbsize);
        cJSON_AddItemToObject(payload, "id6", cJSON_CreateString(tmp));

        uint64_t used_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)(buffer.f_blocks - buffer.f_bfree));
        snprintf(tmp, sizeof(tmp), "%llu", used_fbsize);
        cJSON_AddItemToObject(payload, "id7", cJSON_CreateString(tmp));
    }

    if(dfs_statfs(SD_PATH, &buffer) != 0)
    {
        snprintf(tmp, sizeof(tmp), "%d", 0);
        cJSON_AddItemToObject(payload, "id8", cJSON_CreateString(tmp));
        cJSON_AddItemToObject(payload, "id9", cJSON_CreateString(tmp));
    }
    else
    {
        uint64_t total_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)buffer.f_blocks);
        snprintf(tmp, sizeof(tmp), "%llu", total_fbsize);
        cJSON_AddItemToObject(payload, "id8", cJSON_CreateString(tmp));

        uint64_t used_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)(buffer.f_blocks - buffer.f_bfree));
        snprintf(tmp, sizeof(tmp), "%llu", used_fbsize);
        cJSON_AddItemToObject(payload, "id9", cJSON_CreateString(tmp));
    }

    if(dfs_statfs(USB_PATH, &buffer) != 0)
    {
        snprintf(tmp, sizeof(tmp), "%d", 0);
        cJSON_AddItemToObject(payload, "id10", cJSON_CreateString(tmp));
        cJSON_AddItemToObject(payload, "id11", cJSON_CreateString(tmp));
    }
    else
    {
        uint64_t total_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)buffer.f_blocks);
        snprintf(tmp, sizeof(tmp), "%llu", total_fbsize);
        cJSON_AddItemToObject(payload, "id10", cJSON_CreateString(tmp));

        uint64_t used_fbsize = ((uint64_t)buffer.f_bsize) * ((uint64_t)(buffer.f_blocks - buffer.f_bfree));
        snprintf(tmp, sizeof(tmp), "%llu", used_fbsize);
        cJSON_AddItemToObject(payload, "id11", cJSON_CreateString(tmp));
    }

    cJSON_AddItemToObject(payload, "id12", cJSON_CreateString(FIRMWARE_VERSION));
    cJSON_AddItemToObject(payload, "id13", cJSON_CreateString(g_sundry->serial_number));

    cJSON_AddItemToObject(payload, "id14", cJSON_CreateFalse());
    cJSON_AddItemToObject(payload, "id15", cJSON_CreateFalse());
    cJSON_AddItemToObject(payload, "id16", cJSON_CreateString(""));

    int internet_up = 0;
    if(netdev_default && netdev_is_internet_up(netdev_default))
        internet_up = 1;
    cJSON_AddItemToObject(payload, "id17", cJSON_CreateBool(internet_up));

    snprintf(tmp, sizeof(tmp), "%u", g_sundry->run_time);
    cJSON_AddItemToObject(payload, "id18", cJSON_CreateString(tmp));

    char *json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_data;
}

char *core_cgi_serialize_plugins_info(void)
{
    char tmp[10];

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "code", cJSON_CreateNumber(0));
    cJSON *plugins = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "plugins", plugins);

    rt_slist_t *header = plugins_get_header();
    rt_slist_t *node;
    int index = -1;
    rt_slist_for_each(node, header)
    {
        struct plugins_module *module = rt_slist_entry(node, struct plugins_module, slist);

        index++;
        cJSON *plugin_ob = cJSON_CreateObject();
        cJSON_AddItemToArray(plugins, plugin_ob);
        snprintf(tmp, sizeof(tmp), "%d", index);
        cJSON_AddItemToObject(plugin_ob, "key", cJSON_CreateString(tmp));
        cJSON_AddItemToObject(plugin_ob, "name", cJSON_CreateString(module->name));
        cJSON_AddItemToObject(plugin_ob, "author", cJSON_CreateString(module->author));
        cJSON_AddItemToObject(plugin_ob, "version", cJSON_CreateString(module->version));
        cJSON_AddItemToObject(plugin_ob, "is_sys", cJSON_CreateBool(module->is_sys));
        snprintf(tmp, sizeof(tmp), "%d", module->state);
        cJSON_AddItemToObject(plugin_ob, "state", cJSON_CreateString(tmp));
    }

    char *json_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_data;
}
