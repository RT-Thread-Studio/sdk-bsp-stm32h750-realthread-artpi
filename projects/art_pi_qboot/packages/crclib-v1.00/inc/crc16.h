/*
 * crc16.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-02     qiyongzhong       first version
 */

#ifndef __CRC16_H__
#define __CRC16_H__

#include "typedef.h"
#include "crc_cfg.h"

#ifdef CRCLIB_USING_CRC16

#if (!defined(CRC16_USING_CONST_TABLE) || (CRC16_POLY != 0xA001) || (CRC16_POLY != 0x8408))
/* 
 * @brief   cyclic initialize crc table
 * @param   none
 * @retval  none
 */
void crc16_table_init(void);
#endif

/* 
 * @brief   cyclic calculation crc check value
 * @param   init_val    - initial value
 * @param   pdata       - datas pointer
 * @param   len         - datas len
 * @retval  calculated result 
 */
u16 crc16_cyc_cal(u16 init_val, u8 *pdata, u32 len);

/* 
 * @brief   calculation crc check value, initial is 0xFFFF
 * @param   pdata       - datas pointer
 * @param   len         - datas len
 * @retval  calculated result 
 */
u16 crc16_cal(u8 *pdata, u32 len);

#endif

#endif

