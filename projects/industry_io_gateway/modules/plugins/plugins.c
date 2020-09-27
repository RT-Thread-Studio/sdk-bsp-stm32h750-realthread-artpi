#include "plugins.h"
#include <dfs_posix.h>
#include <dlfcn.h>
#include "fs_operation.h"

#define DBG_TAG "plugins"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

typedef int(*modregister)(const char *path, void *dlmodule, uint8_t is_sys);

static rt_slist_t plugins_header = RT_SLIST_OBJECT_INIT(plugins_header);

rt_slist_t *plugins_get_header(void)
{
    return &plugins_header;
}

struct plugins_module *plugins_find_module_by_name(const char *name)
{
    if(name == RT_NULL)
        return RT_NULL;
    
    rt_slist_t *node;
    rt_slist_for_each(node, &plugins_header)
    {
        struct plugins_module *module = rt_slist_entry(node, struct plugins_module, slist);
        if(strcmp(module->name, name) == 0)
            return module;
    }

    return RT_NULL;
}

static char *plugins_get_dir_path(struct plugins_module *modulem, char *dir_path)
{
    int path_len = strlen(modulem->operation_path) + strlen("/") + strlen(dir_path);
    char *path = rt_malloc(path_len + 1);
    rt_memset(path, 0, path_len + 1);
    strcat(path, modulem->operation_path);
    strcat(path, "/");
    strcat(path, dir_path);
    path[path_len] = '\0';
    fs_create_dir(path);

    return path;
}

void plugins_register(struct plugins_module *module, const char *path, void *dlmodule, uint8_t is_sys)
{
    if((module->name == RT_NULL) || (module->version == RT_NULL) || (module->author == RT_NULL))
        return;

    module->dlmodule = dlmodule;
    if(is_sys)
	{
        const struct global_sundry *g_sundry = global_sundry_get();
		int path_len = strlen(g_sundry->operation_path) + strlen("/sys/") + strlen(module->name);
		char *path_tmp = rt_malloc(path_len + 1);
		rt_memset(path_tmp, 0, path_len + 1);
		strcat(path_tmp, g_sundry->operation_path);
		strcat(path_tmp, "/sys/");
		strcat(path_tmp, module->name);
		path_tmp[path_len] = '\0';
        fs_create_dir(path_tmp);
		module->operation_path = path_tmp;
	}
	else
		module->operation_path = rt_strdup(path);
    module->etc_path = plugins_get_dir_path(module, "etc");
    module->web_path = plugins_get_dir_path(module, "web");
    int path_len = strlen(module->etc_path) + strlen("/cfg.json");
    char *path_tmp = rt_malloc(path_len + 1);
    rt_memset(path_tmp, 0, path_len + 1);
    strcat(path_tmp, module->etc_path);
    strcat(path_tmp, "/cfg.json");
    path_tmp[path_len] = '\0';
    module->cfg_file_path = path_tmp;
    module->is_sys = is_sys;
    module->state = PLUGINS_STATE_CLOSED;
    rt_slist_init(&(module->slist));

    rt_base_t level;

    level = rt_hw_interrupt_disable();

    rt_slist_append(&plugins_header, &(module->slist));

    rt_hw_interrupt_enable(level);
}
RTM_EXPORT(plugins_register);

static int system_plugins_init(void)
{
    struct dirent *dirent = RT_NULL;
    DIR *dir = RT_NULL;
    char path[256];
    dir = opendir(SYS_PLUGINS_PATH);
    if(dir)
    {
        do
        {
            dirent = readdir(dir);
            if(dirent == RT_NULL)
                break;
            if(dirent->d_type != DT_REG)
                continue;
            snprintf(path, sizeof(path), "%s/%s", SYS_PLUGINS_PATH, dirent->d_name);
            char *ptr = strstr(dirent->d_name, ".so");
            if(ptr == RT_NULL)
                continue;
            if(strlen(ptr) != 3)
                continue;
            
            void *module = dlopen(path, RTLD_LAZY);
            if(module == RT_NULL)
                continue;
            modregister fn = dlsym(module, "fregister");
            if(fn == RT_NULL)
            {
                dlclose(module);
                continue;
            }

            fn(SYS_PLUGINS_PATH, module, 1);
        }while(dirent != RT_NULL);

        closedir(dir);
    }

    return RT_EOK;
}

static int user_plugins_init(void)
{
    const struct global_sundry *g_sundry = global_sundry_get();
    struct dirent *parent_dirent = RT_NULL;
    DIR *parent_dir = RT_NULL;
    char path[256];
    snprintf(path, sizeof(path), "%s/plugins", g_sundry->operation_path);
    parent_dir = opendir(path);
    if(parent_dir)
    {
        do
        {
            snprintf(path, sizeof(path), "%s/plugins", g_sundry->operation_path);
            parent_dirent = readdir(parent_dir);
            if(parent_dirent == RT_NULL)
                break;
            if(parent_dirent->d_type != DT_DIR)
                continue;
            strcat(path, "/");
            strcat(path, parent_dirent->d_name);
            struct dirent *child_dirent = RT_NULL;
            DIR *child_dir = RT_NULL;
            child_dir = opendir(path);
            if(child_dir)
            {
                do
                {
                    child_dirent = readdir(child_dir);
                    if(child_dirent == RT_NULL)
                        break;
                    if(child_dirent->d_type != DT_REG)
                        continue;
                    int pos = strlen(path);
                    strcat(path, "/");
                    strcat(path, child_dirent->d_name);
                    char *ptr = strstr(child_dirent->d_name, ".so");
                    if(ptr == RT_NULL)
                        continue;
                    if(strlen(ptr) != 3)
                        continue;
                    
                    void *module = dlopen(path, RTLD_LAZY);
                    if(module == RT_NULL)
                        continue;
                    modregister fn = dlsym(module, "fregister");
                    if(fn == RT_NULL)
                    {
                        dlclose(module);
                        continue;
                    }

                    path[pos] = '\0';
                    fn(path, module, 0);
                }while(child_dirent != RT_NULL);

                closedir(child_dir);
            }

        }while(parent_dirent != RT_NULL);

        closedir(parent_dir);
    }

    return RT_EOK;
}

static int list_plugins(void)
{
    rt_slist_t *node;
    rt_slist_for_each(node, &plugins_header)
    {
        if(node == plugins_header.next)
        {
            rt_kprintf("|name|operation_path|version|author|is_sys|state|\r\n");
            rt_kprintf("|---|---|---|---|---|---|\r\n");
        }

        struct plugins_module *module = rt_slist_entry(node, struct plugins_module, slist);
        rt_kprintf("|%s|%s|%s|%s|%d|%d|\r\n", module->name, module->operation_path, module->version,
                   module->author, module->is_sys, module->state);
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(list_plugins, list plugins);

int plugins_init(void)
{
    system_plugins_init();
    user_plugins_init();

    return RT_EOK;
}
