/**
 * @file    agile_console.h
 * @brief   Agile Console 软件包头文件
 * @author  马龙伟 (2544047213@qq.com)
 * @version 1.1.0
 * @date    2022-01-22
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Ma Longwei.
 * All rights reserved.</center></h2>
 *
 */

#ifndef __PKG_AGILE_CONSOLE_H
#define __PKG_AGILE_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include <rtdevice.h>
#include <stdint.h>

/** @defgroup AGILE_CONSOLE_Exported_Types Agile Console Exported Types
 * @{
 */

/**
 * @brief   Agile Console 后端接口结构体
 */
struct agile_console_backend {
    void (*output)(rt_device_t dev, const uint8_t *buf, int len); /**< 向后端输出数据接口 */
    int (*read)(rt_device_t dev, uint8_t *buf, int len);          /**< 从后端读取数据接口 */
    int (*control)(rt_device_t dev, int cmd, void *arg);          /**< 对后端进行设置接口 */
    rt_slist_t slist;                                             /**< 单向链表节点 */
};

/**
 * @brief   Agile Console 结构体
 */
struct agile_console {
    struct rt_device parent;       /**< 设备对象 */
    struct rt_ringbuffer rx_rb;    /**< 接收 ringbuffer */
    struct rt_semaphore rx_notice; /**< 接收信号量 */
    uint8_t rx_init_ok;            /**< 接收初始化完成标志 */
};

/**
 * @}
 */

/** @addtogroup AGILE_CONSOLE_Exported_Functions
 * @{
 */
int agile_console_backend_register(struct agile_console_backend *backend);
void agile_console_wakeup(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __PKG_AGILE_CONSOLE_H */
