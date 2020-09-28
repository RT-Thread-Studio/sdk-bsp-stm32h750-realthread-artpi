#ifndef __PLUGINS_H
#define __PLUGINS_H
#include "common.h"

enum plugins_state
{
    PLUGINS_STATE_CLOSED = 0,
    PLUGINS_STATE_CLOSING,
    PLUGINS_STATE_STARTING,
    PLUGINS_STATE_RUNNING
};

struct plugins_module
{
    void *dlmodule;
    const char *name;
    const char *version;
    const char *author;
    const char *operation_path;
    const char *etc_path;
    const char *web_path;
    const char *cfg_file_path;
    uint8_t is_sys;
    uint8_t state;
    rt_slist_t slist;
};

int plugins_init(void);
rt_slist_t *plugins_get_header(void);
void plugins_register(struct plugins_module *module, const char *path, void *dlmodule, uint8_t is_sys);
struct plugins_module *plugins_find_module_by_name(const char *name);

#endif
