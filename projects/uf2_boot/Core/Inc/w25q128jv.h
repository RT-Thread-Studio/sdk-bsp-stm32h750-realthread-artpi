
#ifndef __W25Q128JV_H__
#define __W25Q128JV_H__

#include <stdint.h>

#define W25Q128JV_FLASH_SIZE (8 * 1024 * 1024)
#define W25Q128JV_BLOCK_SIZE (0x10000)
#define W25Q128JV_SECTOR_SIZE (0x1000)
#define W25Q128JV_PAGE_SIZE (0x100)

#define SPI_ERROR (-1)
#define SPI_OK (0)

#define W25Q128JV_WRITE_ENABLE (0x06)
#define W25Q128JV_READ (0x03)
#define W25Q128JV_PAGE_PROGRAM (0x02)
#define W25Q128JV_STATUS_REG1 (0x05)
#define W25Q128JV_ENABLE_RESET (0x66)
#define W25Q128JV_RESET_DEVICE (0x99)
#define W25Q128JV_DEVICE_ID (0x90)
#define W25Q128JV_ID_NUMBER (0x4b)
#define W25Q128JV_ERASE_SECTOR (0x20)

extern void SPI_W25Q128JV_Reset(void);
extern void SPI_W25Q128JV_DeviceID(uint8_t *v);
extern void SPI_W25Q128JV_IDNumber(uint8_t *v);
extern int SPI_W25Q128JV_EraseSector(uint32_t SectorAddress);
extern int SPI_W25Q128JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
extern int SPI_W25Q128JV_PageProgram(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
extern int SPI_W25Q128JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);

#endif
