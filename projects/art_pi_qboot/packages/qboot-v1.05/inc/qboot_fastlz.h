/*
 * qboot_fastlz.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */
#ifndef __QBOOT_FASTLZ_H__
#define __QBOOT_FASTLZ_H__

#include <qboot.h>

#ifdef QBOOT_USING_FASTLZ
     
#define QBOOT_FASTLZ_BLOCK_HDR_SIZE    4
     
u32 qbt_fastlz_get_block_size(const u8 *comp_datas);
u32 qbt_fastlz_decompress(u8 *out_buf, u32 out_buf_size, const u8 *in_buf, u32 block_size);

#endif

#endif

