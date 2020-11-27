/*
 *  FIPS-180-2 compliant SHA-384/512 implementation
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
 *  The SHA-512 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA512_C)

#include "sha512_alt.h"
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#define DBG_SECTION_NAME "SHA512_ALT"
#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#if defined(MBEDTLS_SHA512_ALT)

void mbedtls_sha512_init(mbedtls_sha512_context *ctx)
{
    if (ctx)
    {
        *ctx = rt_hwcrypto_hash_create(rt_hwcrypto_dev_dufault(), HWCRYPTO_TYPE_SHA2);
        LOG_D("sha2 init ctx[%08x]", *ctx);
    }
    else
    {
        LOG_E("sha2 init. but ctx is null");
    }
}

void mbedtls_sha512_free(mbedtls_sha512_context *ctx)
{
    if (ctx)
    {
        LOG_D("sha2 free ctx[%08x]", *ctx);
        rt_hwcrypto_hash_destroy(*ctx);
    }
    else
    {
        LOG_E("sha2 free. but ctx is null");
    }
}

void mbedtls_sha512_clone(mbedtls_sha512_context *dst,
                          const mbedtls_sha512_context *src)
{
    if (dst && src)
    {
        LOG_D("sha2 clone des[%08x] src[%08x]", *dst, *src);
        rt_hwcrypto_hash_cpy(*dst, *src);
    }
    else
    {
        LOG_E("sha2 clone. but dst or src is null");
    }
}

/*
 * SHA-512 context setup
 */
int mbedtls_sha512_starts_ret(mbedtls_sha512_context *ctx, int is384)
{
    if (ctx)
    {
        LOG_D("sha2-%s starts ctx[%08x]", is384 ? "384" : "512", *ctx);
        if (is384)
        {
            rt_hwcrypto_hash_set_type(*ctx, HWCRYPTO_TYPE_SHA384);
        }
        else
        {
            rt_hwcrypto_hash_set_type(*ctx, HWCRYPTO_TYPE_SHA512);
        }
        rt_hwcrypto_hash_reset(*ctx);
    }
    else
    {
        LOG_E("sha2 starts. but ctx is null");
    }

    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha512_starts(mbedtls_sha512_context *ctx,
                           int is384)
{
    mbedtls_sha512_starts_ret(ctx, is384);
}
#endif

#if !defined(MBEDTLS_SHA512_PROCESS_ALT)
int mbedtls_internal_sha512_process(mbedtls_sha512_context *ctx,
                                    const unsigned char data[128])
{
    return mbedtls_sha512_update_ret(ctx, data, 64);
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha512_process(mbedtls_sha512_context *ctx,
                            const unsigned char data[128])
{
    mbedtls_internal_sha512_process(ctx, data);
}
#endif
#endif /* !MBEDTLS_SHA512_PROCESS_ALT */

/*
 * SHA-512 process buffer
 */
int mbedtls_sha512_update_ret(mbedtls_sha512_context *ctx, const unsigned char *input,
                              size_t ilen)
{
    if (ctx)
    {
        LOG_D("sha2 update ctx[%08x] len:%d in:%08x", *ctx, ilen, input);
        rt_hwcrypto_hash_update(*ctx, input, ilen);
    }
    else
    {
        LOG_E("sha2 update. but ctx is null");
    }

    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha512_update(mbedtls_sha512_context *ctx,
                           const unsigned char *input,
                           size_t ilen)
{
    mbedtls_sha512_update_ret(ctx, input, ilen);
}
#endif

/*
 * SHA-512 final digest
 */
int mbedtls_sha512_finish_ret(mbedtls_sha512_context *ctx, unsigned char output[64])
{
    if (ctx)
    {
        LOG_D("sha2 finish ctx[%08x] out:%08x", *ctx, output);
        rt_hwcrypto_hash_finish(*ctx, output, 32);
    }
    else
    {
        LOG_E("sha2 finish. but ctx is null");
    }

    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha512_finish(mbedtls_sha512_context *ctx,
                           unsigned char output[64])
{
    mbedtls_sha512_finish_ret(ctx, output);
}
#endif

#endif /* MBEDTLS_SELF_TEST */
#endif /* MBEDTLS_SHA512_C */
