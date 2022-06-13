/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-01     iysheng      first version
 */

#ifndef __MCUBOOT_ASSERT_H__
#define __MCUBOOT_ASSERT_H__

#include <rtdebug.h>

#define ASSERT(x)     RT_ASSERT(x)
#define assert(x)     ASSERT(x)
#endif
