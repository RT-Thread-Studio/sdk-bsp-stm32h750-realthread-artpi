/*
 * qboot_gzip.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-08     qiyongzhong       first version
 * 2020-09-18     qiyongzhong       add deinit function
 */
#ifndef __QBOOT_GZIP_H__
#define __QBOOT_GZIP_H__

#include <qboot.h>

#ifdef QBOOT_USING_GZIP
     
void qbt_gzip_init(void);
void qbt_gzip_set_in(const u8 *in_buf, u32 in_size);
int qbt_gzip_decompress(u8 *out_buf, u32 out_buf_size);
void qbt_gzip_deinit(void);

#endif

#endif

