/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-19     liuduanfei   the first version
 */

#ifndef _FT6236_H_
#define _FT6236_H_

#include "touch.h"

int rt_hw_ft6236_init(const char *name, struct rt_touch_config *cfg,  rt_base_t pin);

#endif /* _FT6236_H_ */
