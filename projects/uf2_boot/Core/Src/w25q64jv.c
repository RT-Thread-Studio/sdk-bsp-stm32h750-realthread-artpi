

#include <string.h>
#include <stdint.h>

#include "quadspi.h"
#include "w25q64jv.h"

static int QSPI_W25Q64JV_AutoPollingMemReady(uint32_t timeout);
static int QSPI_W25Q64JV_WriteEnable(void);

static QSPI_CommandTypeDef sCmd;
static QSPI_AutoPollingTypeDef sConf;
static QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

static int QSPI_W25Q64JV_AutoPollingMemReady(uint32_t timeout)
{
    memset(&sCmd, 0, sizeof(sCmd));
    memset(&sConf, 0, sizeof(sConf));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_STATUS_REG1;

    sCmd.DataMode = QSPI_DATA_1_LINE;
    sCmd.DummyCycles = 0;

    sConf.Match = 0x00;
    sConf.Mask = 0x01;
    sConf.MatchMode = QSPI_MATCH_MODE_AND;
    sConf.StatusBytesSize = 1;
    sConf.Interval = 0x10;
    sConf.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &sCmd, &sConf, timeout) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

static int QSPI_W25Q64JV_WriteEnable(void)
{
    memset(&sCmd, 0, sizeof(sCmd));
    memset(&sConf, 0, sizeof(sConf));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_WRITE_ENABLE;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_STATUS_REG1;

    sCmd.DataMode = QSPI_DATA_1_LINE;
    sCmd.DummyCycles = 0;
    sCmd.NbData = 0;

    sConf.Match = 0x02;
    sConf.Mask = 0x02;
    sConf.MatchMode = QSPI_MATCH_MODE_AND;
    sConf.StatusBytesSize = 1;
    sConf.Interval = 0x10;
    sConf.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &sCmd, &sConf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_Reset(void)
{
    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_ENABLE_RESET;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_RESET_DEVICE;
    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_EraseSector(uint32_t SectorAddress)
{
    if (QSPI_W25Q64JV_WriteEnable() != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_ERASE_SECTOR;

    sCmd.AddressMode = QSPI_ADDRESS_1_LINE;
    sCmd.Address = SectorAddress;
    sCmd.AddressSize = QSPI_ADDRESS_24_BITS;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_INPUT_FAST_READ;

    sCmd.AddressMode = QSPI_ADDRESS_4_LINES;
    sCmd.Address = ReadAddr;
    sCmd.AddressSize = QSPI_ADDRESS_24_BITS;

    sCmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    sCmd.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    sCmd.AlternateBytes = 0;

    sCmd.DataMode = QSPI_DATA_4_LINES;
    sCmd.DummyCycles = 4;
    sCmd.NbData = Size;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_WriteEnable() != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_PAGE_PROGRAM;

    sCmd.AddressMode = QSPI_ADDRESS_1_LINE;
    sCmd.Address = WriteAddr;
    sCmd.AddressSize = QSPI_ADDRESS_24_BITS;

    sCmd.DataMode = QSPI_DATA_1_LINE;
    sCmd.DummyCycles = 0;
    sCmd.NbData = Size;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    int ret = QSPI_OK;
    uint32_t end_addr, current_size, current_addr;
    uint8_t *write_data;

    /* Calculation of the size between the write address and the end of the page */
    current_size = W25Q64JV_PAGE_SIZE - (WriteAddr % W25Q64JV_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
        current_size = Size;
    }

    /* Initialize the address variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    write_data = pData;

    /* Perform the write page by page */
    do
    {
        /* Issue page program command */
        if (QSPI_W25Q64JV_PageProgram(write_data, current_addr, current_size) != QSPI_OK)
        {
            ret = QSPI_ERROR;
        }
        else
        {
            /* Update the address and size variables for next page programming */
            current_addr += current_size;
            write_data += current_size;
            current_size = ((current_addr + W25Q64JV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q64JV_PAGE_SIZE;
        }
    } while ((current_addr < end_addr) && (ret == QSPI_OK));

    /* Return BSP status */
    return ret;
}

int QSPI_W25Q64JV_EnableMemoryMappedMode(void)
{
    memset(&sCmd, 0, sizeof(sCmd));
    memset(&s_mem_mapped_cfg, 0, sizeof(s_mem_mapped_cfg));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_INPUT_FAST_READ;

    sCmd.AddressMode = QSPI_ADDRESS_4_LINES;
    sCmd.Address = 0;
    sCmd.AddressSize = QSPI_ADDRESS_24_BITS;

    sCmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    sCmd.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
    sCmd.AlternateBytes    = 0;

    sCmd.DataMode = QSPI_DATA_4_LINES;
    sCmd.DummyCycles = 4;
    sCmd.NbData = 0;

    s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

    if (HAL_QSPI_MemoryMapped(&hqspi, &sCmd, &s_mem_mapped_cfg) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_DeviceID(uint8_t *v)
{
    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_DEVICE_ID;

    sCmd.AddressMode = QSPI_ADDRESS_1_LINE;
    sCmd.Address = 0;
    sCmd.AddressSize = QSPI_ADDRESS_24_BITS;

    sCmd.DataMode = QSPI_DATA_1_LINE;
    sCmd.DummyCycles = 0;
    sCmd.NbData = 2;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)v, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_IDNumber(uint8_t *v)
{
    memset(&sCmd, 0, sizeof(sCmd));

    sCmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCmd.Instruction = W25Q64JV_ID_NUMBER;

    sCmd.DataMode = QSPI_DATA_1_LINE;
    sCmd.DummyCycles = 0;
    sCmd.NbData = 12;

    if (HAL_QSPI_Command(&hqspi, &sCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)v, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}
