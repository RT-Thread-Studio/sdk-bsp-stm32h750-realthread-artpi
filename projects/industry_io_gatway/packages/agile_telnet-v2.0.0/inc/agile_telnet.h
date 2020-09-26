#ifndef __PKG_AGILE_TELNET_H
#define __PKG_AGILE_TELNET_H
#include <rtthread.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif

struct agile_telnet
{
    uint8_t isconnected;
    rt_int32_t server_fd;
    rt_int32_t client_fd;
    int client_timeout;

    struct rt_ringbuffer *rx_rb;
    struct rt_ringbuffer *tx_rb;
};

#ifdef __cplusplus
}
#endif

#endif
