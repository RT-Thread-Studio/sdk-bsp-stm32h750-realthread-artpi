/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2019 Semtech

Description:
    platform specific functions to init sx130x spi、gpio...
 

License: Revised BSD License, see LICENSE.TXT file include in the project
*/


#ifndef _LORA_GW_DRIVER_BOARD_H
#define _LORA_GW_DRIVER_BOARD_H

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>        /* C99 types*/

//#include "config.h"    /* library configuration options (dynamically generated) */

/* -------------------------------------------------------------------------- */
/* --- PUBLIC CONSTANTS ----------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS PROTOTYPES ------------------------------------------ */

/**
@brief LoRa concentrator SPI init
@param None
@return status of spi init operation (rt_err_t)
*/
int lgw_spi_init(void);

/**
@brief LoRa concentrator IO init
@param None
@return None
*/
void lgw_io_init(void);

/**
@brief LoRa concentrator I2C init
@param None
@return status of spi init operation (rt_err_t)
*/
int lgw_i2c_init(void);

#endif

/* --- EOF ------------------------------------------------------------------ */
