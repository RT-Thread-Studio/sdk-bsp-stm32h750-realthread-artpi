#ifndef __COMMON_H
#define __COMMON_H
#include <rtthread.h>
#include <rtdevice.h>
#include <drv_common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIRMWARE_VERSION                    "v1.0.0"
#define DEFAULT_SERIAL_NUMBER               "111"
#define SYS_PLUGINS_PATH                    "/plugins"

#define NORFLASH_PATH                       "/norflash"
#define SD_PATH                             "/sdcard"
#define USB_PATH                            "/usb"

struct global_sundry
{
    uint8_t norflash_mount;
    uint8_t sd_mount;
    uint8_t usb_mount;
    const char *operation_path;
    const char *serial_number;
    int time_zone;
    uint32_t run_time;
};

const struct global_sundry *global_sundry_get(void);

#endif
