/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-01     iysheng      first version
 */
#ifndef __SYS_FLASH_H__
#define __SYS_FLASH_H__

#include <stdint.h>

#define FLASH_AREA_IMAGE_SCRATCH	0
#define FLASH_AREA_IMAGE_PRIMARY(id) ((id == 0) ? 1 : 255)
#define FLASH_AREA_IMAGE_SECONDARY(id) ((id == 0) ? 2 : 255)

#endif
