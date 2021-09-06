
#include <string.h>
#include <stdint.h>

#include "spi.h"
#include "w25q128jv.h"

static uint8_t _rwByte(uint8_t cmd)
{
    uint8_t val;
    HAL_SPI_TransmitReceive(&hspi1, &cmd, &val, 1, 1000);
    return val;
}

static void _wPin(uint8_t val)
{
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, val);
}

static uint8_t SPI_W25Q128JV_ReadStatusReg(uint8_t reg)
{
    uint8_t cmd;
    uint8_t val;

    switch (reg)
    {
    case 1:
        cmd = 0x5;
        break;

    case 2:
        cmd = 0x35;
        break;

    case 3:
        cmd = 0x15;
        break;

    default:
        cmd = 0x5;
        break;
    }

    _wPin(GPIO_PIN_RESET);
    _rwByte(cmd);
    val = _rwByte(0xff);
    _wPin(GPIO_PIN_SET);

    return val;
}

static void SPI_W25Q128JV_WriteEnable(void)
{
    uint8_t cmd = W25Q128JV_WRITE_ENABLE;

    _wPin(GPIO_PIN_RESET);
    _rwByte(cmd);
    _wPin(GPIO_PIN_SET);
}

static void SPI_W25Q128JV_WaitWriteEnd(void)
{
    while ((SPI_W25Q128JV_ReadStatusReg(1) & 0x01) == 0x01)
    {
        ;
    }
}

void SPI_W25Q128JV_Reset(void)
{
    uint8_t cmd = W25Q128JV_ENABLE_RESET;
    _wPin(GPIO_PIN_RESET);
    _rwByte(cmd);
    _wPin(GPIO_PIN_SET);

    cmd = W25Q128JV_RESET_DEVICE;
    _wPin(GPIO_PIN_RESET);
    _rwByte(cmd);
    _wPin(GPIO_PIN_SET);
}

void SPI_W25Q128JV_DeviceID(uint8_t *v)
{
    uint8_t cmd = W25Q128JV_DEVICE_ID;

    _wPin(GPIO_PIN_RESET);

    _rwByte(cmd);
    _rwByte(0);
    _rwByte(0);
    _rwByte(0);

    v[0] = _rwByte(0xff);
    v[1] = _rwByte(0xff);

    _wPin(GPIO_PIN_SET);
}

void SPI_W25Q128JV_IDNumber(uint8_t *v)
{
    uint8_t cmd = W25Q128JV_ID_NUMBER;

    _wPin(GPIO_PIN_RESET);
    _rwByte(cmd);
    for (int i = 0; i < 12; i++)
    {
        v[i] = _rwByte(0xff);
    }
    _wPin(GPIO_PIN_SET);
}

int SPI_W25Q128JV_EraseSector(uint32_t SectorAddress)
{
    uint8_t cmd[4] = {0x0};

    cmd[0] = W25Q128JV_ERASE_SECTOR;
    cmd[1] = (SectorAddress & 0xff0000) >> 16;
    cmd[2] = (SectorAddress & 0xff00) >> 8;
    cmd[3] = SectorAddress & 0xff;

    SPI_W25Q128JV_WriteEnable();

    _wPin(GPIO_PIN_RESET);

    _rwByte(cmd[0]);
    _rwByte(cmd[1]);
    _rwByte(cmd[2]);
    _rwByte(cmd[3]);

    _wPin(GPIO_PIN_SET);

    SPI_W25Q128JV_WaitWriteEnd();

    return SPI_OK;
}

static int _SPI_W25Q128JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4] = {0x0};

    cmd[0] = W25Q128JV_READ;
    cmd[1] = (ReadAddr & 0xff0000) >> 16;
    cmd[2] = (ReadAddr & 0xff00) >> 8;
    cmd[3] = ReadAddr & 0xff;

    _wPin(GPIO_PIN_RESET);

    if (Size > W25Q128JV_PAGE_SIZE)
    {
        Size = W25Q128JV_PAGE_SIZE;
    }

    _rwByte(cmd[0]);
    _rwByte(cmd[1]);
    _rwByte(cmd[2]);
    _rwByte(cmd[3]);

    for (volatile int i = 0; i < Size; i++)
    {
        pData[i] = _rwByte(0xff);
    }

    _wPin(GPIO_PIN_SET);

    SPI_W25Q128JV_WaitWriteEnd();
    return SPI_OK;
}

int SPI_W25Q128JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    int ret = SPI_OK;
    uint32_t end_addr, current_size, current_addr;
    uint8_t *read_data;

    current_size = W25Q128JV_PAGE_SIZE - (ReadAddr % W25Q128JV_PAGE_SIZE);

    if (current_size > W25Q128JV_PAGE_SIZE)
    {
        current_size = W25Q128JV_PAGE_SIZE;
    }

    current_addr = ReadAddr;
    end_addr = ReadAddr + Size;
    read_data = pData;

    do
    {
        if (_SPI_W25Q128JV_Read(read_data, current_addr, current_size) != SPI_OK)
        {
            ret = SPI_ERROR;
        }
        else
        {
            current_addr += current_size;
            read_data += current_size;
            current_size = ((current_addr + W25Q128JV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128JV_PAGE_SIZE;
        }
    } while ((current_addr < end_addr) && (ret == SPI_OK));

    return ret;
}

int SPI_W25Q128JV_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[4] = {0x0};

    cmd[0] = W25Q128JV_PAGE_PROGRAM;
     cmd[1] = (WriteAddr & 0xff0000) >> 16;
    cmd[2] = (WriteAddr & 0xff00) >> 8;
    cmd[3] = WriteAddr & 0xff;

    SPI_W25Q128JV_WriteEnable();

    _wPin(GPIO_PIN_RESET);

    if (Size > W25Q128JV_PAGE_SIZE)
    {
        Size = W25Q128JV_PAGE_SIZE;
    }

    _rwByte(cmd[0]);
    _rwByte(cmd[1]);
    _rwByte(cmd[2]);
    _rwByte(cmd[3]);

    for (volatile int i = 0; i < Size; i++)
    {
        _rwByte(pData[i]);
    }

    _wPin(GPIO_PIN_SET);

    SPI_W25Q128JV_WaitWriteEnd();

    return SPI_OK;
}

int SPI_W25Q128JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    int ret = SPI_OK;
    uint32_t end_addr, current_size, current_addr;
    uint8_t *write_data;

    current_size = W25Q128JV_PAGE_SIZE - (WriteAddr % W25Q128JV_PAGE_SIZE);

    if (current_size > W25Q128JV_PAGE_SIZE)
    {
        current_size = W25Q128JV_PAGE_SIZE;
    }

    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    write_data = pData;

    do
    {
        if (SPI_W25Q128JV_PageProgram(write_data, current_addr, current_size) != SPI_OK)
        {
            ret = SPI_ERROR;
        }
        else
        {
            current_addr += current_size;
            write_data += current_size;
            current_size = ((current_addr + W25Q128JV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128JV_PAGE_SIZE;
        }
    } while ((current_addr < end_addr) && (ret == SPI_OK));

    return ret;
}
