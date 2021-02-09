/*
 * qboot_quicklz.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */

#ifndef __QBOOT_QUICKLZ_H__
#define __QBOOT_QUICKLZ_H__

#include <qboot.h>

#ifdef QBOOT_USING_QUICKLZ

#define QBOOT_QUICKLZ_BLOCK_HDR_SIZE    4

void qbt_quicklz_state_init(void);
u32 qbt_quicklz_get_block_size(const u8 *comp_datas);
u32 qbt_quicklz_decompress(u8 *out_buf, const u8 *in_buf);

#endif

#endif
