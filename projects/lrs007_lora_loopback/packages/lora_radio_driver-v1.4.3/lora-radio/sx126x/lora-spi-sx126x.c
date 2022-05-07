/*!
 * \file      lora-spi-SX126x.c
 *
 * \brief     spi driver implementation for SX126X
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */
 
#include "sx126x-board.h"

#define LOG_TAG "LoRa.SX126X.SPI"
#define LOG_LEVEL  LOG_LVL_DBG 
#include "lora-radio-debug.h"

void SX126xWakeup( void )
{
#ifdef RT_USING_SPI
    uint8_t msg[2] = { RADIO_GET_STATUS, 0x00 };
    
    rt_spi_transfer(SX126x.spi,msg,RT_NULL,2);
    
    // Wait for chip to be ready.
    SX126xWaitOnBusy( );
    
    // Update operating mode context variable
    SX126xSetOperatingMode( MODE_STDBY_RC );
#else
#endif
    
}

void SX126xWriteCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{    
#ifdef RT_USING_SPI
    SX126xCheckDeviceReady( );

    rt_spi_send_then_send(SX126x.spi,&command,1,buffer,size);
    
    if( command != RADIO_SET_SLEEP )
    {
        SX126xWaitOnBusy( );
    }
#else
#endif    
}

uint8_t SX126xReadCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
#ifdef RT_USING_SPI
    uint8_t status = 0;
    uint8_t buffer_temp[16] = {0}; // command size is 2 size
    
    SX126xCheckDeviceReady( );
    
    rt_spi_send_then_recv(SX126x.spi,&command,1,buffer_temp,size + 1);
    
    status = buffer_temp[0];
    
    rt_memcpy(buffer,buffer_temp+1,size);
    
    SX126xWaitOnBusy( );
    
    return status;
#else
#endif    
}

void SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
#ifdef RT_USING_SPI
    uint8_t msg[3] = {0};
    
    msg[0] = RADIO_WRITE_REGISTER;
    msg[1] = ( address & 0xFF00 ) >> 8;
    msg[2] = address & 0x00FF;
    
    SX126xCheckDeviceReady( );
    
    rt_spi_send_then_send(SX126x.spi,msg,3,buffer,size);

    SX126xWaitOnBusy( );
#else
#endif    
}

void SX126xWriteRegister( uint16_t address, uint8_t value )
{
    SX126xWriteRegisters( address, &value, 1 );
}

void SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{ 
#ifdef RT_USING_SPI
    uint8_t msg[4] = {0};
    
    msg[0] = RADIO_READ_REGISTER;
    msg[1] = ( address & 0xFF00 ) >> 8;
    msg[2] = address & 0x00FF;
    msg[3] = 0;
    
    SX126xCheckDeviceReady( );

    rt_spi_send_then_recv(SX126x.spi,msg,4,buffer,size);

    SX126xWaitOnBusy( );
#else
#endif
}

uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    SX126xReadRegisters( address, &data, 1 );
    return data;
}

void SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
#ifdef RT_USING_SPI

    uint8_t msg[2] = {0};
    
    msg[0] = RADIO_WRITE_BUFFER;
    msg[1] = offset;
    
    SX126xCheckDeviceReady( );
    
    rt_spi_send_then_send(SX126x.spi,msg,2,buffer,size);
    
    SX126xWaitOnBusy( );  

#else    

#endif
}

void SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
#ifdef RT_USING_SPI
    uint8_t msg[3] = {0};
    
    msg[0] = RADIO_READ_BUFFER;
    msg[1] = offset;
    msg[2] = 0;
    
    SX126xCheckDeviceReady( );

    rt_spi_send_then_recv(SX126x.spi,msg,3,buffer,size);

    SX126xWaitOnBusy( );
#else    

#endif
    
}
