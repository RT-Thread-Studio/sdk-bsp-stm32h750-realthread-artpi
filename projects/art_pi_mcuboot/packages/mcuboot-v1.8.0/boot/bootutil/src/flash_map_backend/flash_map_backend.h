/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-01     iysheng      first version
 */
#ifndef __FLASH_MAP_BACKEND_H__
#define __FLASH_MAP_BACKEND_H__

#include <stdint.h>
#include <mcuboot_config/mcuboot_config.h>

struct flash_area {
    uint8_t  fa_id;         /** The slot/scratch identification */
    uint8_t  fa_device_id;  /** The device id (usually there's only one) */
    uint16_t pad16;
    uint32_t fa_off;        /** The flash offset from the beginning */
    uint32_t fa_size;       /** The size of this sector */
};

int flash_area_to_sectors(int idx, int *num, struct flash_area *sectors);
int flash_area_id_from_multi_image_slot(int image_index, int slot);
int flash_area_write(struct flash_area *fa, unsigned int offset, const unsigned char *src, unsigned int len);
int flash_area_open(int idx, const struct flash_area **fa);
int flash_area_read(const struct flash_area *fa, unsigned int offset, unsigned char *dst, unsigned int len);
int flash_area_align(const struct flash_area *fa);
int flash_area_close(const struct flash_area *fa);
int flash_area_id_from_image_slot(int slot);
uint8_t flash_area_erased_val(const struct flash_area *fa);
int flash_area_erase(struct flash_area *fa, unsigned int off, unsigned int len);
#endif
