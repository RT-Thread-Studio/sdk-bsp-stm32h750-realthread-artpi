#ifndef __WIFI_H
#define __WIFI_H

int wifi_init(void);
int wifi_is_ready(void);
char* wifi_get_ip(void);
int wifi_connect(char *conn_str);

#endif  /*__WIFI_H*/
 
