/*
 *  FIPS-46-3 compliant Triple-DES implementation
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
 *  DES, on which TDES is based, was originally designed by Horst Feistel
 *  at IBM in 1974, and was adopted as a standard by NIST (formerly NBS).
 *
 *  http://csrc.nist.gov/publications/fips/fips46-3/fips46-3.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_DES_C)

#include "des_alt.h"
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#define DBG_SECTION_NAME "DES_ALT"
#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#if defined(MBEDTLS_DES_ALT)

void mbedtls_des_init(mbedtls_des_context *ctx)
{
    if (ctx)
    {
        ctx->des_context = rt_hwcrypto_symmetric_create(rt_hwcrypto_dev_dufault(), HWCRYPTO_TYPE_DES);
        LOG_D("des init ctx[%08x]", ctx->des_context);
    }
    else
    {
        LOG_E("des init. but ctx is null");
    }
}

void mbedtls_des_free(mbedtls_des_context *ctx)
{
    if (ctx)
    {
        LOG_D("des free ctx[%08x]", ctx->des_context);
        rt_hwcrypto_symmetric_destroy(ctx->des_context);
    }
    else
    {
        LOG_E("des free. but ctx is null");
    }
}

void mbedtls_des3_init(mbedtls_des3_context *ctx)
{
    if (ctx)
    {
        ctx->des3_context = rt_hwcrypto_symmetric_create(rt_hwcrypto_dev_dufault(), HWCRYPTO_TYPE_3DES);
        LOG_D("3des init ctx[%08x]", ctx->des3_context);
    }
    else
    {
        LOG_E("3des init. but ctx is null");
    }
}

void mbedtls_des3_free(mbedtls_des3_context *ctx)
{
    if (ctx)
    {
        LOG_D("3des free ctx[%08x]", ctx->des3_context);
        rt_hwcrypto_symmetric_destroy(ctx->des3_context);
    }
    else
    {
        LOG_E("3des free. but ctx is null");
    }
}

void mbedtls_des_key_set_parity(unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    LOG_E("fun[%s] is run. but this fun no entity", __FUNCTION__);

    while (1)
    {

    }
}

/*
 * Check the given key's parity, returns 1 on failure, 0 on SUCCESS
 */
int mbedtls_des_key_check_key_parity(const unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    int flag = 1;
    LOG_E("fun[%s] is run. but this fun no entity", __FUNCTION__);

    if (flag)
    {
        while (1);
    }
    return flag;
}

int mbedtls_des_key_check_weak(const unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    int flag = 1;
    LOG_E("fun[%s] is run. but this fun no entity", __FUNCTION__);

    if (flag)
    {
        while (1);
    }
    return flag;
}

#if !defined(MBEDTLS_DES_SETKEY_ALT)
void mbedtls_des_setkey(uint32_t SK[32], const unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    LOG_E("fun[%s] is run. but this fun no entity", __FUNCTION__);
    while (1)
    {

    }
}
#endif /* !MBEDTLS_DES_SETKEY_ALT */

/*
 * DES key schedule (56-bit, encryption)
 */
int mbedtls_des_setkey_enc(mbedtls_des_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    if (ctx)
    {
        LOG_D("des enc setkey ctx[%08x] key:%08x len:%d",
              ctx->des_context, key, MBEDTLS_DES_KEY_SIZE);
        ctx->mode = HWCRYPTO_MODE_ENCRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des_context, key, MBEDTLS_DES_KEY_SIZE << 3) != RT_EOK)
        {
            LOG_E("des enc setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des enc setkey. but ctx is null");
    }
    return 0;
}

/*
 * DES key schedule (56-bit, decryption)
 */
int mbedtls_des_setkey_dec(mbedtls_des_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    if (ctx)
    {
        LOG_D("des dec setkey ctx[%08x] key:%08x len:%d",
              ctx->des_context, key, MBEDTLS_DES_KEY_SIZE);
        ctx->mode = HWCRYPTO_MODE_DECRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des_context, key, MBEDTLS_DES_KEY_SIZE << 3) != RT_EOK)
        {
            LOG_E("des dec setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des dec setkey. but ctx is null");
    }
    return 0;
}

/*
 * Triple-DES key schedule (112-bit, encryption)
 */
int mbedtls_des3_set2key_enc(mbedtls_des3_context *ctx,
                             const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2])
{
    if (ctx)
    {
        LOG_D("des3 enc setkey ctx[%08x] key:%08x len:%d",
              ctx->des3_context, key, MBEDTLS_DES_KEY_SIZE * 2);
        ctx->mode = HWCRYPTO_MODE_ENCRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des3_context, key, (MBEDTLS_DES_KEY_SIZE * 2) << 3) != RT_EOK)
        {
            LOG_E("des3 enc setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des enc setkey. but ctx is null");
    }
    return 0;
}

/*
 * Triple-DES key schedule (112-bit, decryption)
 */
int mbedtls_des3_set2key_dec(mbedtls_des3_context *ctx,
                             const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2])
{
    if (ctx)
    {
        LOG_D("des3 dec setkey ctx[%08x] key:%08x len:%d",
              ctx->des3_context, key, MBEDTLS_DES_KEY_SIZE * 2);
        ctx->mode = HWCRYPTO_MODE_DECRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des3_context, key, (MBEDTLS_DES_KEY_SIZE * 2) << 3) != RT_EOK)
        {
            LOG_E("des3 dec setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des3 dec setkey. but ctx is null");
    }
    return 0;
}

/*
 * Triple-DES key schedule (168-bit, encryption)
 */
int mbedtls_des3_set3key_enc(mbedtls_des3_context *ctx,
                             const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3])
{
    if (ctx)
    {
        LOG_D("des3 enc setkey ctx[%08x] key:%08x len:%d",
              ctx->des3_context, key, MBEDTLS_DES_KEY_SIZE * 3);
        ctx->mode = HWCRYPTO_MODE_ENCRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des3_context, key, (MBEDTLS_DES_KEY_SIZE * 3) << 3) != RT_EOK)
        {
            LOG_E("des3 enc setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des enc setkey. but ctx is null");
    }
    return 0;
}

/*
 * Triple-DES key schedule (168-bit, decryption)
 */
int mbedtls_des3_set3key_dec(mbedtls_des3_context *ctx,
                             const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3])
{
    if (ctx)
    {
        LOG_D("des3 dec setkey ctx[%08x] key:%08x len:%d",
              ctx->des3_context, key, MBEDTLS_DES_KEY_SIZE * 2);
        ctx->mode = HWCRYPTO_MODE_DECRYPT;
        if (rt_hwcrypto_symmetric_setkey(ctx->des3_context, key, (MBEDTLS_DES_KEY_SIZE * 3) << 3) != RT_EOK)
        {
            LOG_E("des3 dec setkey err");
            return -1;
        }
    }
    else
    {
        LOG_E("des3 dec setkey. but ctx is null");
    }
    return 0;
}

/*
 * DES-ECB block encryption/decryption
 */
#if !defined(MBEDTLS_DES_CRYPT_ECB_ALT)
int mbedtls_des_crypt_ecb(mbedtls_des_context *ctx,
                          const unsigned char input[8],
                          unsigned char output[8])
{
    struct hwcrypto_symmetric *des_ctx;

    if (ctx)
    {
        des_ctx = (struct hwcrypto_symmetric *)(ctx->des_context);
        LOG_D("des crypt ecb ctx[%08x] mode:%d in:%08x out:%08x",
              ctx->des_context, mode, input, output);
        if (des_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(ctx->des_context, HWCRYPTO_TYPE_DES_ECB);
        }

        if (rt_hwcrypto_symmetric_crypt(ctx->des_context, (hwcrypto_mode)ctx->mode, 8, input, output) != RT_EOK)
        {
            LOG_E("des crypt ecb err");
            return -1;
        }
    }
    else
    {
        LOG_E("des crypt ecb. but ctx is null");
    }

    return 0;
}
#endif /* !MBEDTLS_DES_CRYPT_ECB_ALT */

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * DES-CBC buffer encryption/decryption
 */
int mbedtls_des_crypt_cbc(mbedtls_des_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[8],
                          const unsigned char *input,
                          unsigned char *output)
{
    struct hwcrypto_symmetric *des_ctx;

    if (ctx)
    {
        des_ctx = (struct hwcrypto_symmetric *)(ctx->des_context);
        LOG_D("des crypt cbc ctx[%08x] mode:%d len:%d iv:%08x in:%08x out:%08x",
              ctx->des_context, mode, length, iv, input, output);
        if (des_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(ctx->des_context, HWCRYPTO_TYPE_DES_CBC);
        }

        rt_hwcrypto_symmetric_setiv(ctx->des_context, iv, 8);
        if (rt_hwcrypto_symmetric_crypt(ctx->des_context,
                                        mode == MBEDTLS_DES_ENCRYPT ? HWCRYPTO_MODE_ENCRYPT : HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("des crypt cbc err");
            return -1;
        }
        rt_hwcrypto_symmetric_getiv(ctx->des_context, iv, 8);
    }
    else
    {
        LOG_E("des crypt cbc. but ctx is null");
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

/*
 * 3DES-ECB block encryption/decryption
 */
#if !defined(MBEDTLS_DES3_CRYPT_ECB_ALT)
int mbedtls_des3_crypt_ecb(mbedtls_des3_context *ctx,
                           const unsigned char input[8],
                           unsigned char output[8])
{
    struct hwcrypto_symmetric *des3_ctx;

    if (ctx)
    {
        des3_ctx = (struct hwcrypto_symmetric *)(ctx->des3_context);
        LOG_D("3des crypt ecb ctx[%08x] mode:%d in:%08x out:%08x",
              ctx->des3_context, mode, input, output);
        if (des3_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(ctx->des3_context, HWCRYPTO_TYPE_3DES_ECB);
        }

        if (rt_hwcrypto_symmetric_crypt(ctx->des3_context, (hwcrypto_mode)ctx->mode, 8, input, output) != RT_EOK)
        {
            LOG_E("3des crypt ecb err");
            return -1;
        }
    }
    else
    {
        LOG_E("3des crypt ecb. but ctx is null");
    }

    return 0;
}
#endif /* !MBEDTLS_DES3_CRYPT_ECB_ALT */

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * 3DES-CBC buffer encryption/decryption
 */
int mbedtls_des3_crypt_cbc(mbedtls_des3_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[8],
                           const unsigned char *input,
                           unsigned char *output)
{
    struct hwcrypto_symmetric *des3_ctx;

    if (ctx)
    {
        des3_ctx = (struct hwcrypto_symmetric *)(ctx->des3_context);
        LOG_D("3des crypt cbc ctx[%08x] mode:%d len:%d iv:%08x in:%08x out:%08x",
              ctx->des3_context, mode, length, iv, input, output);
        if (des3_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(ctx->des3_context, HWCRYPTO_TYPE_3DES_CBC);
        }

        rt_hwcrypto_symmetric_setiv(ctx->des3_context, iv, 8);
        if (rt_hwcrypto_symmetric_crypt(ctx->des3_context,
                                        mode == MBEDTLS_DES_ENCRYPT ? HWCRYPTO_MODE_ENCRYPT : HWCRYPTO_MODE_DECRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("3des crypt cbc err");
            return -1;
        }
        rt_hwcrypto_symmetric_getiv(ctx->des3_context, iv, 8);
    }
    else
    {
        LOG_E("3des crypt cbc. but ctx is null");
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#endif /* MBEDTLS_SELF_TEST */
#endif /* MBEDTLS_DES_C */
