/*!
 * \file      lora-spi-sx127x.h
 *
 * \brief     SX127x spi interface
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */
#ifndef __LORA_SPI_SX127x_H__
#define __LORA_SPI_SX127x_H__

/*!
 * \brief Writes the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [IN]: data New register value
 */
void SX127xWrite( uint16_t addr, uint8_t data );

/*!
 * \brief Reads the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \retval data Register value
 */
uint8_t SX127xRead( uint16_t addr );

/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
void SX127xWriteBuffer( uint16_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
void SX127xReadBuffer( uint16_t addr, uint8_t *buffer, uint8_t size );



#endif // __LORA_SPI_SX127x_H__
