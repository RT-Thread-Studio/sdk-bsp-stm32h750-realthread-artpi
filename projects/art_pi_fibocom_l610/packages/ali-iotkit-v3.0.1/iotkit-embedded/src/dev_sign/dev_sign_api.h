#ifndef _DEV_SIGN_H_
#define _DEV_SIGN_H_

#include "dev_sign_internal.h"

typedef struct {
    char hostname[DEV_SIGN_HOSTNAME_MAXLEN];
    uint16_t port;
    char clientid[DEV_SIGN_CLIENT_ID_MAXLEN];
    char username[DEV_SIGN_USERNAME_MAXLEN];
    char password[DEV_SIGN_PASSWORD_MAXLEN];
} iotx_sign_mqtt_t;

int32_t IOT_Sign_MQTT(iotx_mqtt_region_types_t region, iotx_dev_meta_info_t *meta, iotx_sign_mqtt_t *signout);

#endif


