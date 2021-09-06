/*
 * crc_cfg.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-02     qiyongzhong       first version
 */

#ifndef __CRC_CFG_H__
#define __CRC_CFG_H__

#include <rtconfig.h>

//#define CRCLIB_USING_CRC8
//#define CRCLIB_USING_CRC16
//#define CRCLIB_USING_CRC32

#ifdef CRCLIB_USING_CRC8
//#define CRC8_USING_CONST_TABLE //using const table in flash memory, nodefined using table in ram
#ifndef CRC8_POLY
#define CRC8_POLY       0x8C //crc8 polynome, Poly = x8+x5+x4+1
//#define CRC8_POLY       0xD9 //crc8 polynome, Poly = x8+x7+x4+x3+x+1
#endif
#ifndef CRC8_INIT_VAL
#define CRC8_INIT_VAL   0xFF
#endif
#endif

#ifdef CRCLIB_USING_CRC16
//#define CRC16_USING_CONST_TABLE //using const table in flash memory, nodefined using table in ram
#ifndef CRC16_POLY
#define CRC16_POLY      0xA001 //crc16 polynome, Poly = x16+x15+x2+1 (IBM,SDLC)
//#define CRC16_POLY      0x8408 //crc16 polynome, Poly = x16+x12+x5+1 (CCITT,ISO,HDLC,ITUX25,PPP-FCS)
#endif
#ifndef CRC16_INIT_VAL
#define CRC16_INIT_VAL  0xFFFF
#endif
#endif

#ifdef CRCLIB_USING_CRC32
//#define CRC32_USING_CONST_TABLE //using const table in flash memory, nodefined using table in ram
#ifndef CRC32_POLY
#define CRC32_POLY      0xEDB88320 //crc32 polynome, Poly = x32+x26+x23+...+x2+x+1 (ZIP,RAR,IEEE,LAN/FDDI,PPP-FCS)
//#define CRC32_POLY      0x82F63B78 //crc32 polynome, Poly = x32+x28+x27+...+x8+x6+1 (SCTP)
#endif
#ifndef CRC32_INIT_VAL
#define CRC32_INIT_VAL  0xFFFFFFFF
#endif
#endif

#endif

