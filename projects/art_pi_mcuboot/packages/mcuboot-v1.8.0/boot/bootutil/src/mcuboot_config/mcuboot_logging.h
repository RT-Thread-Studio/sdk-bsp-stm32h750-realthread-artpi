/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-01     iysheng      first version
 */
#ifndef __MCUBOOT_LOGGING_H__
#define __MCUBOOT_LOGGING_H__

#define DBG_TAG "mcuboot"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MCUBOOT_LOG_MODULE_DECLARE(...)

#define MCUBOOT_LOG_ERR(...) LOG_E(__VA_ARGS__)
#define MCUBOOT_LOG_WRN(...) LOG_W(__VA_ARGS__)
#define MCUBOOT_LOG_INF(...) LOG_I(__VA_ARGS__)
#define MCUBOOT_LOG_DBG(...) LOG_D(__VA_ARGS__)

#endif
