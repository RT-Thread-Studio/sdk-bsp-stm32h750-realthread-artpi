#ifndef __PKG_AGILE_CONSOLE_H
#define __PKG_AGILE_CONSOLE_H
#include <rtthread.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif

struct agile_console_backend
{
    void (*output)(const uint8_t *buf, int len);
    int (*read)(uint8_t *buf, int len);
    rt_slist_t slist;
};

struct agile_console
{
    struct rt_device parent;
    struct rt_ringbuffer rx_rb;
};

int agile_console_backend_register(struct agile_console_backend *backend);

#ifdef __cplusplus
}
#endif

#endif
