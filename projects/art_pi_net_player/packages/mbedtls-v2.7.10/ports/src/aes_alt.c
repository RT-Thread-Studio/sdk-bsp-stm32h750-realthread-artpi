/*
 *  FIPS-197 compliant AES implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_C)

#include <string.h>
#include "aes_alt.h"
#include <rtthread.h>
#include <rtdevice.h>

#define DBG_SECTION_NAME "AES_ALT"
#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#if defined(MBEDTLS_AES_ALT)

void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
    if (ctx)
    {
        *ctx = rt_hwcrypto_symmetric_create(rt_hwcrypto_dev_dufault(), HWCRYPTO_TYPE_AES);
        LOG_D("aes init ctx[%08x]", *ctx);
    }
    else
    {
        LOG_E("aes init. but ctx is null");
    }
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
    if (ctx)
    {
        LOG_D("aes free ctx[%08x]", *ctx);
        rt_hwcrypto_symmetric_destroy(*ctx);
    }
    else
    {
        LOG_E("aes free. but ctx is null");
    }
}

/*
 * AES key schedule (encryption)
 */
#if !defined(MBEDTLS_AES_SETKEY_ENC_ALT)
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    if (ctx)
    {
        LOG_D("aes enc setkey ctx[%08x] key:%08x keybits:%d",
              *ctx, key, keybits);
        if (rt_hwcrypto_symmetric_setkey(*ctx, key, keybits) != RT_EOK)
        {
            LOG_E("aes enc setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("aes enc setkey. but ctx is null");
    }
    return 0;
}
#endif /* !MBEDTLS_AES_SETKEY_ENC_ALT */

/*
 * AES key schedule (decryption)
 */
#if !defined(MBEDTLS_AES_SETKEY_DEC_ALT)
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    if (ctx)
    {
        LOG_D("aes dec setkey ctx[%08x] key:%08x keybits:%d",
              *ctx, key, keybits);
        if (rt_hwcrypto_symmetric_setkey(*ctx, key, keybits) != RT_EOK)
        {
            LOG_E("aes dec setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("aes dec setkey. but ctx is null");
    }
    return 0;
}
#endif /* !MBEDTLS_AES_SETKEY_DEC_ALT */

/*
 * AES-ECB block encryption
 */
#if !defined(MBEDTLS_AES_ENCRYPT_ALT)
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, input, output);
}
#endif /* !MBEDTLS_AES_ENCRYPT_ALT */

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_aes_encrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
    mbedtls_internal_aes_encrypt(ctx, input, output);
}
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

/*
 * AES-ECB block decryption
 */
#if !defined(MBEDTLS_AES_DECRYPT_ALT)
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_DECRYPT, input, output);
}
#endif /* !MBEDTLS_AES_DECRYPT_ALT */

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_aes_decrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
    mbedtls_internal_aes_decrypt(ctx, input, output);
}
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    struct hwcrypto_symmetric *aes_ctx;

    if (ctx)
    {
        aes_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("aes crypt ecb ctx[%08x] mode:%d in:%08x out:%08x",
              *ctx, mode, input, output);
        if (aes_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_AES_ECB);
        }

        if (rt_hwcrypto_symmetric_crypt(*ctx,
                                        mode == MBEDTLS_AES_ENCRYPT ? HWCRYPTO_MODE_ENCRYPT : HWCRYPTO_MODE_DECRYPT,
                                        16, input, output) != RT_EOK)
        {
            LOG_E("aes crypt ecb err");
            return -1;
        }
    }
    else
    {
        LOG_E("aes crypt ecb. but ctx is null");
    }

    return 0;
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    struct hwcrypto_symmetric *aes_ctx;

    if (ctx)
    {
        aes_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("aes crypt cbc ctx[%08x] mode:%d len:%d iv:%08x in:%08x out:%08x",
              *ctx, mode, length, iv, input, output);
        if (aes_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_AES_CBC);
        }

        rt_hwcrypto_symmetric_setiv(*ctx, iv, 16);
        if (rt_hwcrypto_symmetric_crypt(*ctx,
                                        mode == MBEDTLS_AES_ENCRYPT ? HWCRYPTO_MODE_ENCRYPT : HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("aes crypt cbc err");
            return -1;
        }
        rt_hwcrypto_symmetric_getiv(*ctx, iv, 16);
    }
    else
    {
        LOG_E("aes crypt cbc. but ctx is null");
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    struct hwcrypto_symmetric *aes_ctx;

    if (ctx)
    {
        aes_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("aes crypt cfb128 ctx[%08x] mode:%d len:%d iv_off:%d "
              "iv:%08x in:%08x out:%08x",
              *ctx, mode, length, iv_off ? *iv_off : -1, iv, input, output);
        if (aes_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_AES_CFB);
        }

        rt_hwcrypto_symmetric_setiv(*ctx, iv, 16);
        rt_hwcrypto_symmetric_set_ivoff(*ctx, (rt_int32_t)(*iv_off));
        if (rt_hwcrypto_symmetric_crypt(*ctx,
                                        mode == MBEDTLS_AES_ENCRYPT ? HWCRYPTO_MODE_ENCRYPT : HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("aes crypt cbc err");
            return -1;
        }
        rt_hwcrypto_symmetric_get_ivoff(*ctx, (rt_int32_t *)iv_off);
        rt_hwcrypto_symmetric_getiv(*ctx, iv, 16);
    }
    else
    {
        LOG_E("aes crypt cbc. but ctx is null");
    }

    return 0;
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    int flag = 1;
    LOG_E("fun[%s] is run. but this fun no entity", __FUNCTION__);

    if (flag)
    {
        while (1);
    }
    return flag;
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int mbedtls_aes_crypt_ofb(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *iv_off,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    struct hwcrypto_symmetric *aes_ctx;

    if (ctx)
    {
        aes_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("aes crypt ofb ctx[%08x] len:%d iv_off:%d iv:%08x in:%08x out:%08x",
              *ctx, length, iv_off ? *iv_off : -1, iv, input, output);
        if (aes_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_AES_OFB);
        }

        rt_hwcrypto_symmetric_setiv(*ctx, iv, 16);
        rt_hwcrypto_symmetric_set_ivoff(*ctx, (rt_int32_t)(*iv_off));
        if (rt_hwcrypto_symmetric_crypt(*ctx, HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("aes crypt ofb err");
            return -1;
        }
        rt_hwcrypto_symmetric_get_ivoff(*ctx, (rt_int32_t *)iv_off);
        rt_hwcrypto_symmetric_getiv(*ctx, iv, 16);
    }
    else
    {
        LOG_E("aes crypt ofb. but ctx is null");
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    struct hwcrypto_symmetric *aes_ctx;

    if (ctx)
    {
        aes_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("aes crypt ctr ctx[%08x] off:%d cnt:%08x blk:%08x in:%08x out:%08x",
              *ctx, nc_off ? *nc_off : -1, nonce_counter, stream_block, input, output);
        if (aes_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_AES_CTR);
        }
        if (*nc_off == 0)
        {
            rt_hwcrypto_symmetric_setiv(*ctx, nonce_counter, 16);
        }
        rt_hwcrypto_symmetric_set_ivoff(*ctx, (rt_int32_t)*nc_off);
        if (rt_hwcrypto_symmetric_crypt(*ctx, HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("aes crypt ctr err");
            return -1;
        }
        rt_hwcrypto_symmetric_get_ivoff(*ctx, (rt_int32_t *)nc_off);
        if (stream_block)
        {
            rt_hwcrypto_symmetric_getiv(*ctx, stream_block, 16);
        }
    }
    else
    {
        LOG_E("aes crypt ctr. but ctx is null");
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_SELF_TEST */
#endif /* MBEDTLS_AES_C */
