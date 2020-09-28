#include "init_module.h"
#include <rthw.h>

static rt_slist_t init_module_prev_header      = RT_SLIST_OBJECT_INIT(init_module_prev_header);
static rt_slist_t init_module_device_header    = RT_SLIST_OBJECT_INIT(init_module_device_header);
static rt_slist_t init_module_component_header = RT_SLIST_OBJECT_INIT(init_module_component_header);
static rt_slist_t init_module_env_header       = RT_SLIST_OBJECT_INIT(init_module_env_header);
static rt_slist_t init_module_app_header       = RT_SLIST_OBJECT_INIT(init_module_app_header);

static rt_slist_t init_module_header    = RT_SLIST_OBJECT_INIT(init_module_header);

static void init_module_register(rt_slist_t *l, struct init_module *module)
{
    rt_base_t level;

    rt_slist_init(&(module->slist));

    level = rt_hw_interrupt_disable();

    rt_slist_append(l, &(module->slist));

    rt_hw_interrupt_enable(level);
}

void init_module_prev_register(struct init_module *module)
{
    init_module_register(&init_module_prev_header, module);
}
RTM_EXPORT(init_module_prev_register);

void init_module_device_register(struct init_module *module)
{
    init_module_register(&init_module_device_header, module);
}
RTM_EXPORT(init_module_device_register);

void init_module_component_register(struct init_module *module)
{
    init_module_register(&init_module_component_header, module);
}
RTM_EXPORT(init_module_component_register);

void init_module_env_register(struct init_module *module)
{
    init_module_register(&init_module_env_header, module);
}
RTM_EXPORT(init_module_env_register);

void init_module_app_register(struct init_module *module)
{
    init_module_register(&init_module_app_header, module);
}
RTM_EXPORT(init_module_app_register);

int init_module_init(void)
{
    rt_slist_tail(&init_module_header)->next = init_module_prev_header.next;
    rt_slist_tail(&init_module_header)->next = init_module_device_header.next;
    rt_slist_tail(&init_module_header)->next = init_module_component_header.next;
    rt_slist_tail(&init_module_header)->next = init_module_env_header.next;
    rt_slist_tail(&init_module_header)->next = init_module_app_header.next;

    rt_slist_t *node;
    rt_slist_for_each(node, &init_module_header)
    {
        struct init_module *module = rt_slist_entry(node, struct init_module, slist);
        module->init();
    }

    return RT_EOK;
}
