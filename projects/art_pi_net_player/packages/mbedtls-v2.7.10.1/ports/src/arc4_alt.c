/*
 *  An implementation of the ARCFOUR algorithm
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
 *  The ARCFOUR algorithm was publicly disclosed on 94/09.
 *
 *  http://groups.google.com/group/sci.crypt/msg/10a300c9d21afca0
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ARC4_C)

#include "arc4_alt.h"
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#define DBG_SECTION_NAME "RC4_ALT"
#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#if defined(MBEDTLS_ARC4_ALT)

void mbedtls_arc4_init(mbedtls_arc4_context *ctx)
{
    if (ctx)
    {
        *ctx = rt_hwcrypto_symmetric_create(rt_hwcrypto_dev_default(), HWCRYPTO_TYPE_RC4);
        LOG_D("rc4 init ctx[%08x]", *ctx);
    }
    else
    {
        LOG_E("rc4 init. but ctx is null");
    }
}

void mbedtls_arc4_free(mbedtls_arc4_context *ctx)
{
    if (ctx)
    {
        LOG_D("rc4 free ctx[%08x]", *ctx);
        rt_hwcrypto_symmetric_destroy(*ctx);
    }
    else
    {
        LOG_E("rc4 free. but ctx is null");
    }
}

/*
 * ARC4 key schedule
 */
void mbedtls_arc4_setup(mbedtls_arc4_context *ctx, const unsigned char *key,
                        unsigned int keylen)
{
    if (ctx)
    {
        LOG_D("rc4 setup ctx[%08x] key:%08x keylen:%d",
              *ctx, key, keylen);
        if (rt_hwcrypto_symmetric_setkey(*ctx, key, keylen << 3) != RT_EOK)
        {
            LOG_E("rc4 setup err");
            return;
        }
    }
    else
    {
        LOG_E("rc4 setup. but ctx is null");
    }
}

/*
 * ARC4 cipher function
 */
int mbedtls_arc4_crypt(mbedtls_arc4_context *ctx, size_t length, const unsigned char *input,
                       unsigned char *output)
{
    struct hwcrypto_symmetric *rc4_ctx;

    if (ctx)
    {
        rc4_ctx = (struct hwcrypto_symmetric *)(*ctx);
        LOG_D("rc4 crypt ctx[%08x] len:%d in:%08x out:%08x",
              *ctx, length, input, output);
        if (rc4_ctx->flags & SYMMTRIC_MODIFY_KEY)
        {
            rt_hwcrypto_symmetric_set_type(*ctx, HWCRYPTO_TYPE_RC4);
        }

        if (rt_hwcrypto_symmetric_crypt(*ctx, HWCRYPTO_MODE_ENCRYPT,
                                        length, input, output) != RT_EOK)
        {
            LOG_E("rc4 crypt err");
            return -1;
        }
    }
    else
    {
        LOG_E("rc4 crypt. but ctx is null");
    }

    return 0;
}

#endif /* MBEDTLS_SELF_TEST */
#endif /* MBEDTLS_ARC4_C */
