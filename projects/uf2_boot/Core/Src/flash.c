
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "flash.h"
#include "w25q64jv.h"
#include "w25q128jv.h"

static uint8_t flash_page_bitmap[W25Q64JV_FLASH_SIZE / W25Q64JV_PAGE_SIZE / 8];

static void clr_bitmap(uint32_t bitNo);
static void set_bitmap(uint32_t bitNo);
static int get_bitmap(uint32_t bitNo);

void flash_init(void)
{
    QSPI_W25Q64JV_Reset();
    SPI_W25Q128JV_Reset();
    for (int i = 0; i < (W25Q64JV_FLASH_SIZE / W25Q64JV_PAGE_SIZE / 8); i++)
    {
        set_bitmap(i);
    }

    //for (int i = 0; i < 32; i++)
    //{
    //    printf("Get bitNo(#%d)(%d) \r\n", i, get_bitmap(i));
    //    printf("Clear bitNo(#%d) \r\n", i);
    //    clr_bitmap(i);
    //    printf("Get bitNo(#%d)(%d) \r\n\r\n", i, get_bitmap(i));
    //}
}

void flash_read(uint32_t ReadAddr, uint8_t *pData, uint32_t Size)
{
    if ((ReadAddr & 0x90000000) == 0x90000000)
    {
        ReadAddr -= 0x90000000;
        QSPI_W25Q64JV_Read(pData, ReadAddr, Size);
    }
    else if ((ReadAddr & 0xA0000000) == 0xA0000000)
    {
        ReadAddr -= 0xA0000000;
        SPI_W25Q128JV_Read(pData, ReadAddr, Size);
    }
}

void flash_write(uint32_t WriteAddr, uint8_t *pData, uint32_t Size)
{
    uint32_t sector_num;
    uint32_t page_num;

    if ((WriteAddr & 0x90000000) == 0x90000000)
    {
        WriteAddr -= 0x90000000;
        page_num = WriteAddr / W25Q64JV_PAGE_SIZE;
        if (get_bitmap(page_num)) // 检测page是否pageprogram.
        {
            sector_num = WriteAddr / W25Q64JV_SECTOR_SIZE;                       // 计算出page所在sector的首地址
            page_num = sector_num * (W25Q64JV_SECTOR_SIZE / W25Q64JV_PAGE_SIZE); // 被擦除的sector对应的page首地址
            QSPI_W25Q64JV_EraseSector(sector_num * W25Q64JV_SECTOR_SIZE);
            for (uint32_t i = page_num; i < (page_num + W25Q64JV_SECTOR_SIZE / W25Q64JV_PAGE_SIZE); i++)
            {
                clr_bitmap(i); // 清除被擦除page的位图
            }
        }
        QSPI_W25Q64JV_Write(pData, WriteAddr, Size);
        page_num = WriteAddr / W25Q64JV_PAGE_SIZE;
        set_bitmap(page_num); // 设置page位图,表示此page已经被使用.
    }
    else if ((WriteAddr & 0xA0000000) == 0xA0000000)
    {
        WriteAddr -= 0xA0000000;
        page_num = WriteAddr / W25Q128JV_PAGE_SIZE;
        if (get_bitmap(page_num)) // 检测page是否pageprogram.
        {
            sector_num = WriteAddr / W25Q128JV_SECTOR_SIZE;                        // 计算出page所在sector的首地址
            page_num = sector_num * (W25Q128JV_SECTOR_SIZE / W25Q128JV_PAGE_SIZE); // 被擦除的sector对应的page首地址
            SPI_W25Q128JV_EraseSector(sector_num * W25Q128JV_SECTOR_SIZE);
            for (uint32_t i = page_num; i < (page_num + W25Q128JV_SECTOR_SIZE / W25Q128JV_PAGE_SIZE); i++)
            {
                clr_bitmap(i); // 清除被擦除page的位图
            }
        }
        SPI_W25Q128JV_Write(pData, WriteAddr, Size);
        page_num = WriteAddr / W25Q128JV_PAGE_SIZE;
        set_bitmap(page_num); // 设置page位图,表示此page已经被使用.
    }
}

static void clr_bitmap(uint32_t bitNo)
{
    uint32_t byte_num;
    uint32_t bit_num;
    uint8_t v, mask;

    byte_num = bitNo / 8;
    bit_num = bitNo % 8;
    mask = ~(0x1 << bit_num);
    v = flash_page_bitmap[byte_num];
    v &= mask;
    flash_page_bitmap[byte_num] = v;
}

static void set_bitmap(uint32_t bitNo)
{
    uint32_t byte_num;
    uint32_t bit_num;
    uint8_t v, mask;

    byte_num = bitNo / 8;
    bit_num = bitNo % 8;
    mask = (0x1 << bit_num);
    v = flash_page_bitmap[byte_num];
    v |= mask;
    flash_page_bitmap[byte_num] = v;
}

static int get_bitmap(uint32_t bitNo)
{
    uint32_t byte_num;
    uint32_t bit_num;
    uint8_t v;

    byte_num = bitNo / 8;
    bit_num = bitNo % 8;

    v = flash_page_bitmap[byte_num];
    v >>= bit_num;

    return v & 0x1;
}
