#ifndef __WIFI_H
#define __WIFI_H

#define WIFI_DEVICE_NAME "w0"

int wifi_init(void);
int wifi_is_ready(void);
char *wifi_get_ip(void);
int wifi_connect(char *conn_str);
char *wifi_status_get(void);

#endif /*__WIFI_H*/
