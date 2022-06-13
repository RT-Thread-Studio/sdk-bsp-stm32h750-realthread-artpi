/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-01     iysheng      first version
 */
#include <stdint.h>
#include <rtconfig.h>
#include "flash_map_backend/flash_map_backend.h"
#include "mcuboot_logging.h"
#include "bootutil_priv.h"
#include <bootutil/sign_key.h>
#include "image_sign_pub.c.import"

const struct bootutil_key bootutil_keys[] = {
    [0] = {
        .key = image_sign_pub_der,
        .len = &image_sign_pub_der_len,
    }
};

const int bootutil_key_cnt = sizeof(bootutil_keys) / sizeof(bootutil_keys[0]);

typedef enum {
    IMAGE0_PRIMARY_SLOT_FA_ID = 1,
    IMAGE0_SECONDARY_SLOT_FA_ID,
} image_slot_fa_idx_E;

static struct flash_area gs_flash_area[BOOT_IMAGE_NUMBER][BOOT_NUM_SLOTS];

extern void W25QXX_Write(uint8_t *pbuf, uint32_t addr, uint16_t size);
extern void W25QXX_Read(uint8_t *pbuf, uint32_t addr, uint16_t size);
extern void W25QXX_SectorErase(uint32_t addr);

#define W25Q_FLASH_MAPPED_ADDR 0x90000000

int w25q_flash_write(uint32_t addr, const unsigned char *src, uint32_t len)
{
    W25QXX_Write(src, addr, (uint16_t)len);

    return 0;
}

int w25q_flash_read(uint32_t addr, unsigned char *dst, uint32_t len)
{
    W25QXX_Read(dst, addr, (uint16_t)len);

    return 0;
}

int flash_area_id_from_multi_image_slot(int image_index, int slot)
{
    int flash_area_id = 0;

    if (image_index == 0)
    {
        if (slot == BOOT_PRIMARY_SLOT)
        {
            flash_area_id = IMAGE0_PRIMARY_SLOT_FA_ID;
        }
        else if (slot == BOOT_SECONDARY_SLOT)
        {
            flash_area_id = IMAGE0_SECONDARY_SLOT_FA_ID;
        }
    }

    return flash_area_id;
}

int flash_area_write(struct flash_area *fa, unsigned int offset, const unsigned char *src, unsigned int len)
{
    uint32_t addr = fa->fa_off + offset;

    if (fa->fa_device_id == MCUBOOT_FLASH_DEVICE_ID)
    {
        addr -= W25Q_FLASH_MAPPED_ADDR;
    }
    w25q_flash_write(addr, src, len);

    return 0;
}

int flash_area_to_sectors(int idx, int *num, struct flash_area *sectors)
{
    int i = 0;

    if (idx == FLASH_AREA_IMAGE_PRIMARY(0))
    {
        *num = MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS;
        for (; i < MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS; i++)
        {
            sectors->fa_id = IMAGE0_PRIMARY_SLOT_FA_ID;
            sectors->fa_device_id = MCUBOOT_FLASH_DEVICE_ID;
            sectors->fa_off = i * MCUBOOT_FLASH_SECTOR_SIZE + W25Q_FLASH_MAPPED_ADDR;
            sectors->fa_size = MCUBOOT_FLASH_SECTOR_SIZE;
            sectors++;
        }
        gs_flash_area[0][0].fa_id = IMAGE0_PRIMARY_SLOT_FA_ID;
        gs_flash_area[0][0].fa_device_id = MCUBOOT_FLASH_DEVICE_ID;
        gs_flash_area[0][0].fa_off = W25Q_FLASH_MAPPED_ADDR;
        gs_flash_area[0][0].fa_size = MCUBOOT_FLASH_SECTOR_SIZE * MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS;
    }
    else if (idx == FLASH_AREA_IMAGE_SECONDARY(0))
    {
        *num = MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS;
        for (; i < MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS; i++)
        {
            sectors->fa_id = IMAGE0_SECONDARY_SLOT_FA_ID;
            sectors->fa_device_id = MCUBOOT_FLASH_DEVICE_ID;
            sectors->fa_off = (i + MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS) * MCUBOOT_FLASH_SECTOR_SIZE + W25Q_FLASH_MAPPED_ADDR;
            sectors->fa_size = MCUBOOT_FLASH_SECTOR_SIZE;
            sectors++;
        }
        gs_flash_area[0][1].fa_id = IMAGE0_SECONDARY_SLOT_FA_ID;
        gs_flash_area[0][1].fa_device_id = MCUBOOT_FLASH_DEVICE_ID;
        gs_flash_area[0][1].fa_off = W25Q_FLASH_MAPPED_ADDR + MCUBOOT_FLASH_SECTOR_SIZE * MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS;
        gs_flash_area[0][1].fa_size = MCUBOOT_FLASH_SECTOR_SIZE * MCUBOOT_IMAGE_SLOT_SECTOR_COUNTS;
    }
    else
    {
        /* TODO for scrach */
    }

    return 0;
}

int flash_area_open(int idx, const struct flash_area **fa)
{
    int ret = 0;

    if (!fa)
    {
        return -1;
    }

    if (idx == 1)
    {
        *fa = &gs_flash_area[0][0];
    }
    else if (idx == 2)
    {
        *fa = &gs_flash_area[0][1];
    }
#if MCUBOOT_SWAP_USING_SCRATCH
    else if (idx == FLASH_AREA_IMAGE_SCRATCH)
    {
        /* TODO with scratch */
    }
#endif
    else
    {
        ret = -1;
    }

    return ret;
}

int flash_area_read(const struct flash_area *fa, unsigned int offset, unsigned char *dst, unsigned int len)
{
    uint32_t addr = fa->fa_off + offset;

    if (fa->fa_device_id == MCUBOOT_FLASH_DEVICE_ID)
    {
        addr -= W25Q_FLASH_MAPPED_ADDR;
    }
    w25q_flash_read(addr, dst, len);

    return 0;
}

int flash_area_align(const struct flash_area *fa)
{
    return 4;
}

int flash_area_close(const struct flash_area *fa)
{
    return 0;
}

int flash_area_id_from_image_slot(int slot)
{
    /* TODO sth */
    return 0;
}

uint8_t flash_area_erased_val(const struct flash_area *fa)
{
    return 0xff;
}

int flash_area_erase(struct flash_area *fa, unsigned int off, unsigned int len)
{
    uint32_t addr = fa->fa_off + off;
    uint32_t sector_size = fa->fa_size;
    unsigned int sector_counts;

    if (fa->fa_device_id == MCUBOOT_FLASH_DEVICE_ID)
    {
        if (len)
        {
            addr -= W25Q_FLASH_MAPPED_ADDR;
            W25QXX_SectorErase(addr);
        }
        return 0;
    }

    return -1;
}
