/*
 * qboot.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */
     
#ifndef __QBOOT_H__
#define __QBOOT_H__
     
#include <typedef.h>
#include <rtconfig.h>
#include <fal_cfg.h>

//#define QBOOT_USING_PRODUCT_CODE
//#define QBOOT_USING_AES
//#define QBOOT_USING_GZIP
//#define QBOOT_USING_QUICKLZ
//#define QBOOT_USING_FASTLZ
//#define QBOOT_USING_SHELL
//#define QBOOT_USING_SYSWATCH
//#define QBOOT_USING_OTA_DOWNLOAD
//#define QBOOT_USING_PRODUCT_INFO
//#define QBOOT_USING_STATUS_LED
//#define QBOOT_USING_FACTORY_KEY
#define QBOOT_USING_APP_CHECK

#ifdef QBOOT_USING_STATUS_LED
#ifndef QBOOT_STATUS_LED_PIN
#define QBOOT_STATUS_LED_PIN            0
#endif
#ifndef QBOOT_STATUS_LED_LEVEL
#define QBOOT_STATUS_LED_LEVEL          1 //led on level, 0--low, 1--high
#endif
#endif

#ifdef QBOOT_USING_FACTORY_KEY
#ifndef QBOOT_FACTORY_KEY_PIN
#define QBOOT_FACTORY_KEY_PIN           -1
#endif
#ifndef QBOOT_FACTORY_KEY_LEVEL
#define QBOOT_FACTORY_KEY_LEVEL         0 //key press level, 0--low, 1--high
#endif
#ifndef QBOOT_FACTORY_KEY_CHK_TMO
#define QBOOT_FACTORY_KEY_CHK_TMO       10
#endif
#endif

#ifdef QBOOT_USING_SHELL
#ifndef QBOOT_SHELL_KEY_CHK_TMO
#define QBOOT_SHELL_KEY_CHK_TMO         5
#endif
#endif

#ifdef  RT_APP_PART_ADDR
#define QBOOT_APP_ADDR                  RT_APP_PART_ADDR
#else
#define QBOOT_APP_ADDR                  0x08020000
#endif

#ifdef QBOOT_USING_PRODUCT_CODE
#ifndef QBOOT_PRODUCT_CODE
#define QBOOT_PRODUCT_CODE              "00010203040506070809"
#endif
#endif

#ifndef QBOOT_APP_PART_NAME
#define QBOOT_APP_PART_NAME             "app"
#endif

#ifndef QBOOT_DOWNLOAD_PART_NAME
#define QBOOT_DOWNLOAD_PART_NAME        "download"
#endif

#ifndef QBOOT_FACTORY_PART_NAME
#define QBOOT_FACTORY_PART_NAME         "factory"
#endif

#ifdef QBOOT_USING_AES
#ifndef QBOOT_AES_IV
#define QBOOT_AES_IV                    "0123456789ABCDEF"
#endif
#ifndef QBOOT_AES_KEY
#define QBOOT_AES_KEY                   "0123456789ABCDEF0123456789ABCDEF"
#endif
#endif

#ifdef QBOOT_USING_PRODUCT_INFO
#ifndef QBOOT_PRODUCT_NAME
#define QBOOT_PRODUCT_NAME              "Qboot test device"
#endif
#ifndef QBOOT_PRODUCT_VER
#define QBOOT_PRODUCT_VER               "v1.00 2020.07.27"
#endif
#ifndef QBOOT_PRODUCT_MCU
#define QBOOT_PRODUCT_MCU               "stm32l4r5zi"
#endif
#endif

#ifndef QBOOT_THREAD_STACK_SIZE
#define QBOOT_THREAD_STACK_SIZE         4096
#endif

#ifndef QBOOT_THREAD_PRIO
#define QBOOT_THREAD_PRIO               5
#endif

#endif

