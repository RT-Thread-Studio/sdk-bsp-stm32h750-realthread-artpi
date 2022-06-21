#ifndef __PKG_AGILE_TELNET_H
#define __PKG_AGILE_TELNET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include <rtdevice.h>
#include <stdint.h>

#ifdef PKG_AGILE_TELNET_USING_AUTH
enum agile_telnet_state {
    AGILE_TELNET_STATE_USER = 0,
    AGILE_TELNET_STATE_PASSWORD,
    AGILE_TELNET_STATE_PROCESS
};
#endif

struct agile_telnet {
    uint8_t isconnected;
    int server_fd;
    int client_fd;
    int client_timeout;
    rt_device_t tlnt_dev;
    int tx_fd;

#ifdef PKG_AGILE_TELNET_USING_AUTH
    enum agile_telnet_state state;
    char username[50];
    char password[50];
#endif

    struct rt_ringbuffer *rx_rb;
    struct rt_ringbuffer *tx_rb;
};

#ifdef __cplusplus
}
#endif

#endif
