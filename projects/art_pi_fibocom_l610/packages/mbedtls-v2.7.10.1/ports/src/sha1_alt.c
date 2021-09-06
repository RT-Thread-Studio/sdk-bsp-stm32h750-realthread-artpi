/*
 *  FIPS-180-1 compliant SHA-1 implementation
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
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_C)

#include "sha1_alt.h"
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#define DBG_SECTION_NAME "SHA1_ALT"
#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#if defined(MBEDTLS_SHA1_ALT)

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    if (ctx)
    {
        *ctx = rt_hwcrypto_hash_create(rt_hwcrypto_dev_default(), HWCRYPTO_TYPE_SHA1);
        LOG_D("sha1 init ctx[%08x]", *ctx);
    }
    else
    {
        LOG_E("sha1 init. but ctx is null");
    }
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx)
    {
        LOG_D("sha1 free ctx[%08x]", *ctx);
        rt_hwcrypto_hash_destroy(*ctx);
    }
    else
    {
        LOG_E("sha1 free. but ctx is null");
    }
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src)
{
    if (dst && src)
    {
        LOG_D("sha1 clone des[%08x] src[%08x]", *dst, *src);
        rt_hwcrypto_hash_cpy(*dst, *src);
    }
    else
    {
        LOG_E("sha1 clone. but dst or src is null");
    }
}

/*
 * SHA-1 context setup
 */
int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx)
{
    if (ctx)
    {
        LOG_D("sha1 starts ctx[%08x]", *ctx);
        rt_hwcrypto_hash_reset(*ctx);
    }
    else
    {
        LOG_E("sha1 starts. but ctx is null");
    }
    return (0);
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha1_starts(mbedtls_sha1_context *ctx)
{
    mbedtls_sha1_starts_ret(ctx);
}
#endif


#if !defined(MBEDTLS_SHA1_PROCESS_ALT)
int mbedtls_internal_sha1_process(mbedtls_sha1_context *ctx,
                                  const unsigned char data[64])
{
    return mbedtls_sha1_update_ret(ctx, data, 64);
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha1_process(mbedtls_sha1_context *ctx,
                          const unsigned char data[64])
{
    mbedtls_internal_sha1_process(ctx, data);
}
#endif
#endif /* !MBEDTLS_SHA1_PROCESS_ALT */

/*
 * SHA-1 process buffer
 */
int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx)
    {
        LOG_D("sha1 update ctx[%08x] len:%d in:%08x", *ctx, ilen, input);
        rt_hwcrypto_hash_update(*ctx, input, ilen);
    }
    else
    {
        LOG_E("sha1 update. but ctx is null");
    }

    return (0);
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha1_update(mbedtls_sha1_context *ctx,
                         const unsigned char *input,
                         size_t ilen)
{
    mbedtls_sha1_update_ret(ctx, input, ilen);
}
#endif

/*
 * SHA-1 final digest
 */
int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    if (ctx)
    {
        LOG_D("sha1 finish ctx[%08x] out:%08x", *ctx, output);
        rt_hwcrypto_hash_finish(*ctx, output, 32);
    }
    else
    {
        LOG_E("sha1 finish. but ctx is null");
    }

    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha1_finish(mbedtls_sha1_context *ctx,
                         unsigned char output[20])
{
    mbedtls_sha1_finish_ret(ctx, output);
}
#endif

#endif /* MBEDTLS_SHA1_ALT */
#endif /* MBEDTLS_SHA1_C */
