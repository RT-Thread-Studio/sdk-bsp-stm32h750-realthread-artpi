/*
 * crc32.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-02     qiyongzhong       first version
 */

#ifndef __CRC32_H__
#define __CRC32_H__

#include "typedef.h"
#include "crc_cfg.h"

#ifdef CRCLIB_USING_CRC32

#if (!defined(CRC32_USING_CONST_TABLE) || (CRC32_POLY != 0xEDB88320) || (CRC32_POLY != 0x82F63B78))
/* 
 * @brief   cyclic initialize crc table
 * @param   none
 * @retval  none
 */
void crc32_table_init(void);
#endif

/* 
 * @brief   cyclic calculation crc check value
 * @param   init_val    - initial value
 * @param   pdata       - datas pointer
 * @param   len         - datas len
 * @retval  calculated result 
 */
u32 crc32_cyc_cal(u32 init_val, u8 *pdata, u32 len);

/* 
 * @brief   calculation crc check value, initial is CRC32_INIT_VAL
 * @param   pdata       - datas pointer
 * @param   len         - datas len
 * @retval  calculated result 
 */
u32 crc32_cal(u8 *pdata, u32 len);

#endif

#endif

