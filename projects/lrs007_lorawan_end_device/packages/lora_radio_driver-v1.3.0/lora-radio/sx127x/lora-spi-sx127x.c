/*!
 * \file      lora-radio-spi-sx127x.c
 *
 * \brief     sx127x spi driver implementation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */
#include "sx127x\sx127x.h"
#include "sx127x-board.h"

void SX127xWriteBuffer( uint16_t addr, uint8_t *buffer, uint8_t size )
{   
#ifdef RT_USING_SPI
    uint8_t msg[1] = {0};
    
    msg[0] = (addr | 0x80);

    rt_spi_send_then_send(SX127x.spi,msg,1,buffer,size);
#else
//    uint8_t i;

//    //NSS = 0;
//    GpioWrite( &SX127x.Spi.Nss, 0 );

//    SpiInOut( &SX127x.Spi, addr | 0x80 );
//    for( i = 0; i < size; i++ )
//    {
//        SpiInOut( &SX127x.Spi, buffer[i] );
//    }

//    //NSS = 1;
//    GpioWrite( &SX127x.Spi.Nss, 1 );
#endif
}

void SX127xReadBuffer( uint16_t addr, uint8_t *buffer, uint8_t size )
{
#ifdef RT_USING_SPI
    uint8_t msg[1] = {0};
    
    msg[0] = (addr & 0x7F);

    rt_spi_send_then_recv(SX127x.spi,msg,1,buffer,size);
#else
//    uint8_t i;

//    //NSS = 0;
//    GpioWrite( &SX127x.Spi.Nss, 0 );

//    SpiInOut( &SX127x.Spi, addr & 0x7F );

//    for( i = 0; i < size; i++ )
//    {
//        buffer[i] = SpiInOut( &SX127x.Spi, 0 );
//    }

//    //NSS = 1;
//    GpioWrite( &SX127x.Spi.Nss, 1 );
#endif
}
