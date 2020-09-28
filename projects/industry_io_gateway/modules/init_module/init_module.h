#ifndef __INIT_MODULE_H
#define __INIT_MODULE_H
#include <rtthread.h>

struct init_module
{
    int (*init)(void);
    rt_slist_t slist;
};


void init_module_prev_register(struct init_module *module);
void init_module_device_register(struct init_module *module);
void init_module_component_register(struct init_module *module);
void init_module_env_register(struct init_module *module);
void init_module_app_register(struct init_module *module);

int init_module_init(void);

#endif
