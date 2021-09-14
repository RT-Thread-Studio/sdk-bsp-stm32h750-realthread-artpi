/*
 * loragw_debug.h 
 */
#ifndef _LORAGW_DBG_H
#define _LORAGW_DBG_H

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

//#include "config.h"    /* library configuration options (dynamically generated) */

#include "rtconfig.h"

#ifdef RT_USING_ULOG
#include <rtdbg.h>
#include <ulog.h> 
#endif
/* -------------------------------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

/* Turn on some of these (set to non-zero) to debug LoRa GW Driver */

#ifndef LGD_DBG_USR
#define LGD_DBG_USR                          0
#endif

#ifndef LGD_DBG_LORAWAN_MAC
#define LGD_DBG_LORAWAN_MAC                  0
#endif

#ifndef LGD_DBG_HAL
#define LGD_DBG_HAL                          0
#endif

#ifndef LGD_DBG_TEMPERTURE
#define LGD_DBG_TEMPERTURE                   0
#endif

#ifndef LGD_DBG_TEMPERTURE_TST
#define LGD_DBG_TEMPERTURE_TST               0
#endif

#ifndef LGD_DBG_SX1302
#define LGD_DBG_SX1302                       0
#endif

#ifndef LGD_DBG_REG
#define LGD_DBG_REG                          0
#endif

#ifndef LGD_DBG_SPI_TST
#define LGD_DBG_SPI_TST                      0
#endif

#ifndef LGD_DBG_SPI
#define LGD_DBG_SPI                          0
#endif

#ifndef LGD_DBG_SPI_TST
#define LGD_DBG_SPI_TST                      0
#endif

#ifndef LGD_DBG_I2C
#define LGD_DBG_I2C                          0
#endif

#if defined RT_USING_ULOG
#define LGD_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_output(level, LOG_TAG, RT_TRUE, __VA_ARGS__);                    \
    }                                                                         \
}                                                                             \
while (0)


#define LGD_DEBUG_LOG_RAW(type, ...)                                   \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_raw(__VA_ARGS__);                                                \
    }                                                                         \
}                                                                             \
while (0)

#define LGD_DEBUG_LOG_HEXDUMP(type, buf, size)                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        ulog_hexdump(LOG_TAG, 16, buf, size);                                 \
    }                                                                         \
}                                                                             \
while (0)
    
#else

#define LGD_DEBUG_LOG(type, level, ...)                                \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
    {                                                                         \
        rt_kprintf(__VA_ARGS__);                                              \
        rt_kprintf("\r\n");                                                   \
    }                                                                         \
}                                                                             \
while (0)

#define LGD_DEBUG_LOG_HEXDUMP(type, buf, size) 

#endif

#else /* LGD */

#define LGD_DEBUG_LOG(type, level, ...)

#endif /* LGD */


/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS PROTOTYPES ------------------------------------------ */

/**
@brief
@param
*/
//void dbg_log_buffer_to_file(FILE * file, uint8_t * buffer, uint16_t size);

/**
@brief
@param
*/
//void dbg_log_payload_diff_to_file(FILE * file, uint8_t * buffer1, uint8_t * buffer2, uint16_t size);

/**
@brief
@param
*/
void dbg_init_random(void);

/**
@brief
@param
*/
void dbg_generate_random_payload(uint32_t pkt_cnt, uint8_t * buffer_expected, uint8_t size);

/**
@brief
@param
*/
//int dbg_check_payload(struct lgw_conf_debug_s * context, FILE * file, uint8_t * payload_received, uint8_t size, uint8_t ref_payload_idx, uint8_t sf);


/* --- EOF ------------------------------------------------------------------ */
