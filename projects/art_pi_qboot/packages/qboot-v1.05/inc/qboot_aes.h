/*
 * qboot_aes.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */

#ifndef __QBOOT_AES_H__
#define __QBOOT_AES_H__

#include <qboot.h>

#ifdef QBOOT_USING_AES

void qbt_aes_decrypt_init(void);
void qbt_aes_decrypt(u8 *dst_buf, const u8 *src_buf, u32 len);

#endif

#endif
