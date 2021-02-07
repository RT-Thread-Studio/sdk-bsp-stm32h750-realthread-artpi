/*
 * qboot.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 * 2020-08-31     qiyongzhong       fix qbt_jump_to_app type from static to weak
 * 2020-09-01     qiyongzhong       add app verify when checking firmware
 * 2020-09-18     qiyongzhong       fix bug of gzip decompression
 * 2020-09-22     qiyongzhong       add erase firmware function, update version to v1.04
 * 2020-10-05     qiyongzhong       fix to support stm32h7xx, update version to v1.05
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <fal.h>
#include <qboot.h>
#include <qboot_aes.h>
#include <qboot_gzip.h>
#include <qboot_fastlz.h>
#include <qboot_quicklz.h>

#ifdef QBOOT_USING_STATUS_LED
#include <qled.h>
#endif

//#define QBOOT_DEBUG
#define QBOOT_USING_LOG
#define DBG_TAG "Qboot"

#ifdef QBOOT_DEBUG
#define DBG_LVL DBG_LOG
#else
#define DBG_LVL DBG_INFO
#endif

#ifdef QBOOT_USING_LOG
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#ifndef DBG_COLOR
#define DBG_COLOR
#endif
#endif

#include <rtdbg.h>

#define QBOOT_VER_MSG                   "V1.0.5 2020.10.05"
#define QBOOT_SHELL_PROMPT              "Qboot>"

#define QBOOT_BUF_SIZE                  4096//must is 4096
#if (defined(QBOOT_USING_QUICKLZ) || defined(QBOOT_USING_FASTLZ))
#define QBOOT_CMPRS_READ_SIZE           1024 //it can is 512, 1024, 2048, 4096,
#define QBOOT_CMPRS_BUF_SIZE            (QBOOT_BUF_SIZE + QBOOT_CMPRS_READ_SIZE)
#else
#define QBOOT_CMPRS_READ_SIZE           QBOOT_BUF_SIZE
#define QBOOT_CMPRS_BUF_SIZE            QBOOT_BUF_SIZE
#endif


#define QBOOT_ALGO_CRYPT_NONE           0
#define QBOOT_ALGO_CRYPT_XOR            1
#define QBOOT_ALGO_CRYPT_AES            2
#define QBOOT_ALGO_CRYPT_MASK           0x0F

#define QBOOT_ALGO_CMPRS_NONE           (0 << 8)
#define QBOOT_ALGO_CMPRS_GZIP           (1 << 8)
#define QBOOT_ALGO_CMPRS_QUICKLZ        (2 << 8)
#define QBOOT_ALGO_CMPRS_FASTLZ         (3 << 8)
#define QBOOT_ALGO_CMPRS_MASK           (0x0F << 8)

#define QBOOT_ALGO2_VERIFY_NONE         0
#define QBOOT_ALGO2_VERIFY_CRC          1
#define QBOOT_ALGO2_VERIFY_MASK         0x0F

typedef struct {
    u8  type[4];
    u16 algo;
    u16 algo2;
    u32 time_stamp;
    u8  part_name[16];
    u8  fw_ver[24];
    u8  prod_code[24];
    u32 pkg_crc;
    u32 raw_crc;
    u32 raw_size;
    u32 pkg_size;
    u32 hdr_crc;
}fw_info_t;

static fw_info_t fw_info;
static u8 cmprs_buf[QBOOT_CMPRS_BUF_SIZE];
#if (defined(QBOOT_USING_AES) || defined(QBOOT_USING_GZIP) || defined(QBOOT_USING_QUICKLZ) || defined(QBOOT_USING_FASTLZ))
static u8 crypt_buf[QBOOT_BUF_SIZE];
#else
const u8 *crypt_buf = NULL;
#endif

#ifdef QBOOT_USING_GZIP
#define GZIP_REMAIN_BUF_SIZE 32
static int gzip_remain_len = 0;
static u8 gzip_remain_buf[GZIP_REMAIN_BUF_SIZE];
#endif

static bool qbt_part_is_exist(const char *part_name)
{
    return(fal_partition_find(part_name) != NULL);
}

static bool qbt_fw_info_read(const char *part_name, fw_info_t *fw_info, bool from_tail)
{
    fal_partition_t part = fal_partition_find(part_name);
    u32 addr = from_tail ? (part->len - sizeof(fw_info_t)) : 0;
    if (fal_partition_read(part, addr, (u8 *)fw_info, sizeof(fw_info_t)) < 0)
    {
        return(false);
    }
    return(true);
}

static bool qbt_fw_info_write(const char *part_name, fw_info_t *fw_info, bool to_tail)
{
    fal_partition_t part = fal_partition_find(part_name);
    u32 addr = to_tail ? (part->len - sizeof(fw_info_t)) : 0;
    if (fal_partition_write(part, addr, (u8 *)fw_info, sizeof(fw_info_t)) < 0)
    {
        return(false);
    }
    return(true);
}

static bool qbt_fw_info_check(fw_info_t *fw_info)
{
    if (strcmp((const char *)(fw_info->type), "RBL") != 0)
    {
        return(false);
    }
    
    return (crc32_cal((u8 *)fw_info, (sizeof(fw_info_t) - sizeof(u32))) == fw_info->hdr_crc);
}

static bool qbt_fw_crc_check(const char *part_name, u32 addr, u32 size, u32 crc)
{
    u32 pos = 0;
    u32 crc32 = 0xFFFFFFFF;
    fal_partition_t part = fal_partition_find(part_name);
    
    while (pos < size)
    {
        int read_len = QBOOT_BUF_SIZE;
        int gzip_remain_len = size - pos;
        if (read_len > gzip_remain_len)
        {
            read_len = gzip_remain_len;
        }
        if (fal_partition_read(part, addr + pos, cmprs_buf, read_len) < 0)
        {
            LOG_E("Qboot read firmware datas fail. part = %s, addr = %08X, length = %d", part_name, pos, read_len);
            return(false);
        }
        crc32 = crc32_cyc_cal(crc32, cmprs_buf, read_len);
        pos += read_len;
    }
    crc32 ^= 0xFFFFFFFF;

    if (crc32 != crc)
    {
        LOG_E("Qboot verify CRC32 error, cal.crc: %08X != body.crc: %08X", crc32, crc);
        return(false);
    }

    return(true);
}

static bool qbt_release_sign_check(const char *part_name, fw_info_t *fw_info)
{
    u32 release_sign = 0;
    fal_partition_t part = fal_partition_find(part_name);
    u32 pos = (((sizeof(fw_info_t) + fw_info->pkg_size) + 0x1F) & ~0x1F);

    if (fal_partition_read(part, pos, (u8 *)&release_sign, sizeof(u32)) < 0)
    {
        LOG_E("Qboot read release sign fail from %s partition.", part_name);
        return(false);
    }

    return(release_sign == 0);
}

static bool qbt_release_sign_write(const char *part_name, fw_info_t *fw_info)
{
    u32 release_sign = 0;
    fal_partition_t part = fal_partition_find(part_name);
    u32 pos = (((sizeof(fw_info_t) + fw_info->pkg_size) + 0x1F) & ~0x1F);

    if (fal_partition_write(part, pos, (u8 *)&release_sign, sizeof(u32)) < 0)
    {
        LOG_E("Qboot write release sign fail from %s partition.", part_name);
        return(false);
    }

    return(true);
}

static bool qbt_fw_decrypt_init(int crypt_type)
{
    switch (crypt_type)
    {
    case QBOOT_ALGO_CRYPT_NONE:
        break;

    #ifdef QBOOT_USING_AES
    case QBOOT_ALGO_CRYPT_AES:
        qbt_aes_decrypt_init();
        break;
    #endif

    default:
        return(false);
    }
    
    return(true);
}

static bool qbt_fw_decompress_init(int cmprs_type)
{
    switch (cmprs_type)
    {
    case QBOOT_ALGO_CMPRS_NONE:
        break;
        
    #ifdef QBOOT_USING_GZIP
    case QBOOT_ALGO_CMPRS_GZIP:
        gzip_remain_len = 0;
        qbt_gzip_init();
        break;
    #endif
    
    #ifdef QBOOT_USING_QUICKLZ
    case QBOOT_ALGO_CMPRS_QUICKLZ:
        qbt_quicklz_state_init();
        break;
    #endif

    #ifdef QBOOT_USING_FASTLZ
    case QBOOT_ALGO_CMPRS_FASTLZ:
        break;
    #endif
    
    default:
        return(false);
    }
    
    return(true);
}

static bool qbt_fw_decompress_deinit(int cmprs_type)
{
    switch (cmprs_type)
    {
    case QBOOT_ALGO_CMPRS_NONE:
        break;
        
    #ifdef QBOOT_USING_GZIP
    case QBOOT_ALGO_CMPRS_GZIP:
        qbt_gzip_deinit();
        break;
    #endif
    
    #ifdef QBOOT_USING_QUICKLZ
    case QBOOT_ALGO_CMPRS_QUICKLZ:
        break;
    #endif

    #ifdef QBOOT_USING_FASTLZ
    case QBOOT_ALGO_CMPRS_FASTLZ:
        break;
    #endif
    
    default:
        return(false);
    }
    
    return(true);
}

static bool qbt_fw_pkg_read(const fal_partition_t part, u32 pos, u8 *buf, u32 read_len, u8 *crypt_buf, int crypt_type)
{
    switch(crypt_type)
    {
    case QBOOT_ALGO_CRYPT_NONE:
        if (fal_partition_read(part, pos, buf, read_len) < 0)
        {
            return(false);
        }
        break;
    
    #ifdef QBOOT_USING_AES    
    case QBOOT_ALGO_CRYPT_AES:
        if (fal_partition_read(part, pos, crypt_buf, read_len) < 0)
        {
           return(false);
        }
        qbt_aes_decrypt(buf, crypt_buf, read_len);
        break;
    #endif

    default:
        return(false);
    }

    return(true);
}

static int qbt_dest_part_write(fal_partition_t part, u32 pos, u8 *decmprs_buf, u8 *cmprs_buf, u32 *p_cmprs_len, int cmprs_type)
{
    int write_len = 0;
    int cmprs_len = 0;
    int decomp_len = 0;
    int block_size = 0;
    
    cmprs_len = *p_cmprs_len;
    
    switch(cmprs_type)
    {
    case QBOOT_ALGO_CMPRS_NONE:
        if (fal_partition_write(part, pos, cmprs_buf, cmprs_len) < 0)
        {
            return(-1);
        }
        write_len = cmprs_len;
        cmprs_len = 0;
        break;
        
    #ifdef QBOOT_USING_GZIP
    case QBOOT_ALGO_CMPRS_GZIP:
        qbt_gzip_set_in(cmprs_buf, cmprs_len);
        while(1)
        {
            bool is_end;

            memcpy(decmprs_buf, gzip_remain_buf, gzip_remain_len);
            decomp_len = qbt_gzip_decompress(decmprs_buf + gzip_remain_len, QBOOT_BUF_SIZE - gzip_remain_len);
            if (decomp_len < 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            is_end = (decomp_len < (QBOOT_BUF_SIZE - gzip_remain_len));
            decomp_len += gzip_remain_len;
            gzip_remain_len = decomp_len % GZIP_REMAIN_BUF_SIZE;
            decomp_len -= gzip_remain_len;
            memcpy(gzip_remain_buf, decmprs_buf + decomp_len, gzip_remain_len);
            if (decomp_len > 0)
            {
                if (fal_partition_write(part, pos, decmprs_buf, decomp_len) < 0)
                {
                    write_len = -1;
                    cmprs_len = 0;
                    break;
                }
            }
            pos += decomp_len;
            write_len += decomp_len;
            if (is_end && (cmprs_len < QBOOT_CMPRS_READ_SIZE) && (gzip_remain_len > 0))//last package and remain > 0
            {
                memset(gzip_remain_buf + gzip_remain_len, 0xFF, GZIP_REMAIN_BUF_SIZE - gzip_remain_len);
                fal_partition_write(part, pos, gzip_remain_buf, GZIP_REMAIN_BUF_SIZE);
                write_len += GZIP_REMAIN_BUF_SIZE;
            }
            if (is_end)
            {
                cmprs_len = 0;
                break;
            }
        }
        break;
    #endif
    
    #ifdef QBOOT_USING_QUICKLZ    
    case QBOOT_ALGO_CMPRS_QUICKLZ:
        while(1)
        {
            if (cmprs_len < QBOOT_QUICKLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            block_size = qbt_quicklz_get_block_size(cmprs_buf);
            if (block_size <= 0)
            {
                break;
            }
            if (cmprs_len < block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            decomp_len = qbt_quicklz_decompress(decmprs_buf, cmprs_buf + QBOOT_QUICKLZ_BLOCK_HDR_SIZE);
            if (decomp_len <= 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            if (fal_partition_write(part, pos, decmprs_buf, decomp_len) < 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            pos += decomp_len;
            write_len += decomp_len;
            cmprs_len -= (block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE);
            memcpy(cmprs_buf, cmprs_buf + (block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE), cmprs_len);
        }
        break;
    #endif

    #ifdef QBOOT_USING_FASTLZ
    case QBOOT_ALGO_CMPRS_FASTLZ:
        while(1)
        {
            if (cmprs_len < QBOOT_FASTLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            block_size = qbt_fastlz_get_block_size(cmprs_buf);
            if (block_size <= 0)
            {
                break;
            }
            if (cmprs_len < block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            decomp_len = qbt_fastlz_decompress(decmprs_buf, QBOOT_BUF_SIZE, cmprs_buf + QBOOT_FASTLZ_BLOCK_HDR_SIZE, block_size);
            if (decomp_len <= 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            if (fal_partition_write(part, pos, decmprs_buf, decomp_len) < 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            pos += decomp_len;
            write_len += decomp_len;
            cmprs_len -= (block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE);
            memcpy(cmprs_buf, cmprs_buf + (block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE), cmprs_len);
        }
        break;
    #endif
        
    default:
        write_len = -1;
        cmprs_len = 0;
        break;
    }  

    *p_cmprs_len = cmprs_len;
    
    return(write_len);
}

#ifdef QBOOT_USING_APP_CHECK
static int qbt_app_crc_cal(u32 *p_crc32, u32 max_cal_len, u8 *decmprs_buf, u8 *cmprs_buf, u32 *p_cmprs_len, int cmprs_type)
{
    int write_len = 0;
    int cmprs_len = 0;
    int decomp_len = 0;
    int block_size = 0;
    
    cmprs_len = *p_cmprs_len;
    
    switch(cmprs_type)
    {
    case QBOOT_ALGO_CMPRS_NONE:
        if (cmprs_len > max_cal_len)
        {
            cmprs_len = max_cal_len;
        }
        *p_crc32 = crc32_cyc_cal(*p_crc32, cmprs_buf, cmprs_len);
        write_len = cmprs_len;
        cmprs_len = 0;
        break;
        
    #ifdef QBOOT_USING_GZIP
    case QBOOT_ALGO_CMPRS_GZIP:
        qbt_gzip_set_in(cmprs_buf, cmprs_len);
        while(1)
        {
            bool is_end;

            memcpy(decmprs_buf, gzip_remain_buf, gzip_remain_len);
            decomp_len = qbt_gzip_decompress(decmprs_buf + gzip_remain_len, QBOOT_BUF_SIZE - gzip_remain_len);
            if (decomp_len < 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            is_end = (decomp_len < (QBOOT_BUF_SIZE - gzip_remain_len));
            decomp_len += gzip_remain_len;
            gzip_remain_len = decomp_len % GZIP_REMAIN_BUF_SIZE;
            decomp_len -= gzip_remain_len;
            memcpy(gzip_remain_buf, decmprs_buf + decomp_len, gzip_remain_len);
            if (decomp_len > 0)
            {
                if (decomp_len > max_cal_len)
                {
                    decomp_len = max_cal_len;
                }
                *p_crc32 = crc32_cyc_cal(*p_crc32, decmprs_buf, decomp_len);
            }
            write_len += decomp_len;
            if (is_end && (cmprs_len < QBOOT_CMPRS_READ_SIZE) && (gzip_remain_len > 0))//last package and remain > 0
            {
                if (gzip_remain_len > max_cal_len)
                {
                    gzip_remain_len = max_cal_len;
                }
                *p_crc32 = crc32_cyc_cal(*p_crc32, gzip_remain_buf, gzip_remain_len);
                write_len += gzip_remain_len;
            }
            if (is_end)
            {
                cmprs_len = 0;
                break;
            }
        }
        break;
    #endif
    
    #ifdef QBOOT_USING_QUICKLZ    
    case QBOOT_ALGO_CMPRS_QUICKLZ:
        while(1)
        {
            if (cmprs_len < QBOOT_QUICKLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            block_size = qbt_quicklz_get_block_size(cmprs_buf);
            if (block_size <= 0)
            {
                break;
            }
            if (cmprs_len < block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            decomp_len = qbt_quicklz_decompress(decmprs_buf, cmprs_buf + QBOOT_QUICKLZ_BLOCK_HDR_SIZE);
            if (decomp_len <= 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            if (decomp_len > max_cal_len)
            {
                decomp_len = max_cal_len;
            }
            *p_crc32 = crc32_cyc_cal(*p_crc32, decmprs_buf, decomp_len);
            write_len += decomp_len;
            cmprs_len -= (block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE);
            memcpy(cmprs_buf, cmprs_buf + (block_size + QBOOT_QUICKLZ_BLOCK_HDR_SIZE), cmprs_len);
        }
        break;
    #endif

    #ifdef QBOOT_USING_FASTLZ
    case QBOOT_ALGO_CMPRS_FASTLZ:
        while(1)
        {
            if (cmprs_len < QBOOT_FASTLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            block_size = qbt_fastlz_get_block_size(cmprs_buf);
            if (block_size <= 0)
            {
                break;
            }
            if (cmprs_len < block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE)
            {
                break;
            }
            decomp_len = qbt_fastlz_decompress(decmprs_buf, QBOOT_BUF_SIZE, cmprs_buf + QBOOT_FASTLZ_BLOCK_HDR_SIZE, block_size);
            if (decomp_len <= 0)
            {
                write_len = -1;
                cmprs_len = 0;
                break;
            }
            if (decomp_len > max_cal_len)
            {
                decomp_len = max_cal_len;
            }
            *p_crc32 = crc32_cyc_cal(*p_crc32, decmprs_buf, decomp_len);
            write_len += decomp_len;
            cmprs_len -= (block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE);
            memcpy(cmprs_buf, cmprs_buf + (block_size + QBOOT_FASTLZ_BLOCK_HDR_SIZE), cmprs_len);
        }
        break;
    #endif
        
    default:
        write_len = -1;
        cmprs_len = 0;
        break;
    }  

    *p_cmprs_len = cmprs_len;
    
    return(write_len);
}

static bool qbt_app_crc_check(const char *fw_part_name, fw_info_t *fw_info)
{
    u32 crc32 = 0xFFFFFFFF;
    u32 cmprs_len = 0;
    u32 app_cal_pos = 0;
    u32 src_read_pos = sizeof(fw_info_t);
    fal_partition_t src_part = fal_partition_find(fw_part_name);
    int crypt_type = (fw_info->algo & QBOOT_ALGO_CRYPT_MASK);
    int cmprs_type = (fw_info->algo & QBOOT_ALGO_CMPRS_MASK);


    if ( ! qbt_fw_decrypt_init(crypt_type))
    {
        LOG_E("Qboot app crc check fail. nonsupport encrypt type.");
        return(false);
    }

    if ( ! qbt_fw_decompress_init(cmprs_type))
    {
        LOG_E("Qboot app crc check fail. nonsupport compress type.");
        return(false);
    }

    while(app_cal_pos < fw_info->raw_size)
    {
        int cal_len = 0;
        int read_len = QBOOT_CMPRS_READ_SIZE;
        int remain_len = (fw_info->pkg_size + sizeof(fw_info_t) - src_read_pos);
        if (read_len > remain_len)
        {
            read_len = remain_len;
        }
        if ( ! qbt_fw_pkg_read(src_part, src_read_pos, cmprs_buf + cmprs_len, read_len, crypt_buf, crypt_type))
        {
            qbt_fw_decompress_deinit(cmprs_type);
            LOG_E("Qboot app crc check fail. read package error, part = %s, addr = %08X, length = %d", fw_part_name, src_read_pos, read_len);
            return(false);
        }
        src_read_pos += read_len;
        cmprs_len += read_len;

        remain_len = fw_info->raw_size - app_cal_pos;
        cal_len = qbt_app_crc_cal(&crc32, remain_len, crypt_buf, cmprs_buf, &cmprs_len, cmprs_type);
        if (cal_len < 0)
        {
            qbt_fw_decompress_deinit(cmprs_type);
            LOG_E("Qboot app crc check fail. decompress error.");
            return(false);
        }
        app_cal_pos += cal_len;
    }
    
    qbt_fw_decompress_deinit(cmprs_type);
    crc32 ^= 0xFFFFFFFF;
    if (crc32 != fw_info->raw_crc)
    {
        LOG_E("Qboot app crc check fail. cal.crc: %08X != raw.crc: %08X", crc32, fw_info->raw_crc);
        return(false);
    }
    
    return(true);
}
#endif

static bool qbt_fw_release(const char *dst_part_name, const char *src_part_name, fw_info_t *fw_info)
{
    u32 cmprs_len = 0;
    u32 dst_write_pos = 0;
    u32 src_read_pos = sizeof(fw_info_t);
    fal_partition_t src_part = fal_partition_find(src_part_name);
    fal_partition_t dst_part = fal_partition_find(dst_part_name);
    int crypt_type = (fw_info->algo & QBOOT_ALGO_CRYPT_MASK);
    int cmprs_type = (fw_info->algo & QBOOT_ALGO_CMPRS_MASK);

    if ( ! qbt_fw_decrypt_init(crypt_type))
    {
        LOG_E("Qboot release firmware fail. nonsupport encrypt type.");
        return(false);
    }

    if ( ! qbt_fw_decompress_init(cmprs_type))
    {
        LOG_E("Qboot release firmware fail. nonsupport compress type.");
        return(false);
    }

    rt_kprintf("Start erase partition %s ...\n", dst_part_name);
    if ((fal_partition_erase(dst_part, 0, fw_info->raw_size) < 0) 
        || (fal_partition_erase(dst_part, dst_part->len - sizeof(fw_info_t), sizeof(fw_info_t)) < 0))
    {
        qbt_fw_decompress_deinit(cmprs_type);
        LOG_E("Qboot release firmware fail. erase %s error.", dst_part_name);
        return(false);
    }

    rt_kprintf("Start release firmware to %s ...     ", dst_part_name);
    while(dst_write_pos < fw_info->raw_size)
    {
        int write_len = 0;
        int read_len = QBOOT_CMPRS_READ_SIZE;
        int remain_len = (fw_info->pkg_size + sizeof(fw_info_t) - src_read_pos);
        if (read_len > remain_len)
        {
            read_len = remain_len;
        }
        if ( ! qbt_fw_pkg_read(src_part, src_read_pos, cmprs_buf + cmprs_len, read_len, crypt_buf, crypt_type))
        {
            qbt_fw_decompress_deinit(cmprs_type);
            LOG_E("Qboot release firmware fail. read package error, part = %s, addr = %08X, length = %d", src_part_name, src_read_pos, read_len);
            return(false);
        }
        src_read_pos += read_len;
        cmprs_len += read_len;
        
        write_len = qbt_dest_part_write(dst_part, dst_write_pos, crypt_buf, cmprs_buf, &cmprs_len, cmprs_type);
        if (write_len < 0)
        {
            qbt_fw_decompress_deinit(cmprs_type);
            LOG_E("Qboot release firmware fail. write destination error, part = %s, addr = %08X", dst_part_name, dst_write_pos);
            return(false);
        }
        dst_write_pos += write_len;

        rt_kprintf("\b\b\b%02d%%", (dst_write_pos * 100 / fw_info->raw_size));
    }
    rt_kprintf("\n");
    
    qbt_fw_decompress_deinit(cmprs_type);
    if ( ! qbt_fw_info_write(dst_part_name, fw_info, true))
    {
        LOG_E("Qboot release firmware fail. write firmware to %s fail.", dst_part_name);
        return(false);
    }
    
    return(true);
}

static bool qbt_dest_part_verify(const char *part_name)
{
    if ( ! qbt_fw_info_read(part_name, &fw_info, true))
    {
        LOG_E("Qboot verify fail, read firmware from %s partition", part_name);
        return(false);
    }
    
    if ( ! qbt_fw_info_check(&fw_info))
    {
        LOG_E("Qboot verify fail. firmware infomation check fail.");
        return(false);
    }

    switch (fw_info.algo2 & QBOOT_ALGO2_VERIFY_MASK)
    {
    case QBOOT_ALGO2_VERIFY_CRC :
        if ( ! qbt_fw_crc_check(part_name, 0, fw_info.raw_size, fw_info.raw_crc))
        {
            return(false);
        }
        break;
        
    default:
        break;
    }

    return(true);
}

static bool qbt_fw_check(const char *fw_part_name, fw_info_t *fw_info, bool output_log)
{
    if ( ! qbt_part_is_exist(fw_part_name))
    {
        if (output_log) LOG_E("Qboot firmware check fail. partition %d is not exist.", fw_part_name);
        return(false);
    }

    if ( ! qbt_fw_info_read(fw_part_name, fw_info, false))
    {
        if (output_log) LOG_E("Qboot firmware check fail. read firmware infomation fail.");
        return(false);
    }

    if ( ! qbt_fw_info_check(fw_info))
    {
        if (output_log) LOG_E("Qboot firmware check fail. firmware infomation check fail.");
        return(false);
    }

    if ( ! qbt_fw_crc_check(fw_part_name, sizeof(fw_info_t), fw_info->pkg_size, fw_info->pkg_crc))
    {
        if (output_log) LOG_E("Qboot firmware check fail. firmware body check fail.");
        return(false);
    }
    
    #ifdef QBOOT_USING_APP_CHECK
    if ((fw_info->algo2 & QBOOT_ALGO2_VERIFY_MASK) == QBOOT_ALGO2_VERIFY_CRC)
    {
        if ( ! qbt_app_crc_check(fw_part_name, fw_info))
        {
            if (output_log) LOG_E("Qboot firmware check fail. firmware app check fail.");
            return(false);
        }
    }
    #endif

    if (output_log) LOG_D("Qboot firmware check success.");
    
    return(true);
}

static bool qbt_fw_update(const char *dst_part_name, const char *src_part_name, fw_info_t *fw_info)
{
    bool rst;
    
    if ( ! qbt_part_is_exist(dst_part_name))
    {
        LOG_E("Qboot firmware update fail. destination partition %d is not exist.", dst_part_name);
        return(false);
    }

    #ifdef QBOOT_USING_STATUS_LED
    qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 50);
    #endif
    
    rst = qbt_fw_release(dst_part_name, src_part_name, fw_info);

    #ifdef QBOOT_USING_STATUS_LED
    qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 450);
    #endif

    if ( ! rst)
    {
        LOG_E("Qboot firmware update fail. firmware release fail.");
        return(false);
    }

    if ( ! qbt_dest_part_verify(dst_part_name))
    {
        LOG_E("Qboot firmware update fail. destination partition verify fail.");
        return(false);
    }

    LOG_I("Qboot firmware update success.");
    return(true);    
}

RT_WEAK void qbt_jump_to_app(void)
{
    typedef void (*app_func_t)(void);
    u32 app_addr = QBOOT_APP_ADDR;
    u32 stk_addr = *((__IO uint32_t *)app_addr);
    app_func_t app_func = (app_func_t)(*((__IO uint32_t *)(app_addr + 4)));

    if ((((u32)app_func & 0xff000000) != 0x08000000) || ((stk_addr & 0x2ff00000) != 0x20000000))
    {
        LOG_E("No legitimate application.");
        return;
    }

    rt_kprintf("Jump to application running ... \n");
    rt_thread_mdelay(200);
    
    __disable_irq();
    HAL_DeInit();

    for(int i=0; i<128; i++)
    {
        HAL_NVIC_DisableIRQ(i);
        HAL_NVIC_ClearPendingIRQ(i);
    }
    
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    
    HAL_RCC_DeInit();
    
    __set_CONTROL(0);
    __set_MSP(stk_addr);
    
    app_func();//Jump to application running
    
    LOG_E("Qboot jump to application fail.");
}

#ifdef QBOOT_USING_STATUS_LED
static void qbt_status_led_init(void)
{
    qled_add(QBOOT_STATUS_LED_PIN, QBOOT_STATUS_LED_LEVEL);
    qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 450);
}
#endif

#ifdef QBOOT_USING_FACTORY_KEY
static bool qbt_factory_key_check(void)
{
    bool rst = true;
    
    #if (QBOOT_FACTORY_KEY_LEVEL)//press level is high
    rt_pin_mode(QBOOT_FACTORY_KEY_PIN, PIN_MODE_INPUT_PULLDOWN);
    #else
    rt_pin_mode(QBOOT_FACTORY_KEY_PIN, PIN_MODE_INPUT_PULLUP);
    #endif

    rt_thread_mdelay(500);

    #ifdef QBOOT_USING_STATUS_LED
    qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 200);
    #endif
    
    for ( int i = 0; i < ((QBOOT_FACTORY_KEY_CHK_TMO * 10) + 1); i++)
    {
        rt_thread_mdelay(100);
        if (rt_pin_read(QBOOT_FACTORY_KEY_PIN) != QBOOT_FACTORY_KEY_LEVEL)
        {
            rst = false;
            break;
        }
    }

    #ifdef QBOOT_USING_STATUS_LED
    qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 450);
    #endif
    
    return(rst);
}
#endif

#ifdef QBOOT_USING_SHELL
static rt_sem_t qbt_shell_sem = NULL;
static rt_device_t qbt_shell_dev = NULL;

static void qbt_close_sys_shell(void)
{
    rt_thread_t thread = rt_thread_find(FINSH_THREAD_NAME);
    if (thread == NULL)
    {
        return;
    }
    if (rt_object_is_systemobject((rt_object_t)thread))
    {
        rt_thread_detach(thread);
    }
    else
    {
        rt_thread_delete(thread);
    }
}

static void qbt_open_sys_shell(void)
{
    rt_thread_t thread = rt_thread_find(FINSH_THREAD_NAME);
    if (thread == NULL)
    {
        #ifdef QBOOT_USING_STATUS_LED
        qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 950);
        #endif
        finsh_set_prompt(QBOOT_SHELL_PROMPT);
        extern int finsh_system_init(void);
        finsh_system_init();
    }
}

static rt_err_t qbt_shell_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(qbt_shell_sem);
    return(RT_EOK);
}

static bool qbt_shell_init(const char *shell_dev_name)
{
    rt_device_t dev = rt_device_find(shell_dev_name);
    if (dev == NULL)
    {
        LOG_E("Qboot shell initialize fail. no find device: %s.", shell_dev_name);
        return(false);
    }

    if (qbt_shell_sem == NULL)
    {
        qbt_shell_sem = rt_sem_create("qboot_shell", 0, RT_IPC_FLAG_FIFO);
        if (qbt_shell_sem == NULL)
        {
            LOG_E("Qboot shell initialize fail. sem create fail.");
            return(false);
        }
    }

    if (dev == qbt_shell_dev)
    {
        return(true);
    }
    
    if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM) != RT_EOK)
    {
        LOG_E("Qboot shell initialize fail. device %s open failed.", shell_dev_name);
        return(false);

    }
    
    if (qbt_shell_dev != RT_NULL)
    {
        rt_device_close(qbt_shell_dev);
        rt_device_set_rx_indicate(qbt_shell_dev, NULL);
    }

    qbt_shell_dev = dev;
    rt_device_set_rx_indicate(dev, qbt_shell_rx_ind);
    
    LOG_D("shell device %s open success.", shell_dev_name);
        
    return(true);
}

static bool qbt_shell_key_check(void)
{
    char ch;
    rt_tick_t tick_start = rt_tick_get();
    rt_tick_t tmo = rt_tick_from_millisecond(QBOOT_SHELL_KEY_CHK_TMO * 1000);

    while(rt_tick_get() - tick_start < tmo)
    {
        if (rt_device_read(qbt_shell_dev, -1, &ch, 1) > 0)
        {    
            if (ch == 0x0d)
            {
                return(true);
            }
            continue;
        }
        rt_sem_take(qbt_shell_sem, 100);
    }
    
    return(false);
}

static bool qbt_startup_shell(bool wait_press_key)
{
    if ( ! qbt_shell_init(RT_CONSOLE_DEVICE_NAME))
    {
        LOG_E("Qboot initialize shell fail.");
        return(false);
    }
    
    if (wait_press_key)
    {
        bool rst;
        rt_kprintf("Press [Enter] key into shell in %d s : ", QBOOT_SHELL_KEY_CHK_TMO);
        rst = qbt_shell_key_check();
        rt_kprintf("\n");
        if ( ! rst)
        {
            return(false);
        }
    }

    qbt_open_sys_shell();
    
    return(true);
}
#endif

static void qbt_show_msg(void)
{
    //rt_kprintf("\033[2J\033[0;0H"); //clear screen
    rt_kprintf("\n");
    rt_kprintf("Qboot startup ... \n");
    rt_kprintf("Qboot version   : %s \n", QBOOT_VER_MSG);
    
    #ifdef QBOOT_USING_PRODUCT_INFO
    rt_kprintf("Product name    : %s \n", QBOOT_PRODUCT_NAME);
    rt_kprintf("Product version : %s \n", QBOOT_PRODUCT_VER);
    rt_kprintf("Product mcu     : %s \n", QBOOT_PRODUCT_MCU);
    #endif
}

static bool qbt_app_resume_from(const char *src_part_name)
{
    if ( ! qbt_fw_check(src_part_name, &fw_info, true))
    {
        return(false);
    }

    #ifdef QBOOT_USING_PRODUCT_CODE
    if ( strcmp(fw_info.prod_code, QBOOT_PRODUCT_CODE) != 0)
    {
        LOG_E("Qboot resume fail from %s.", src_part_name);
        LOG_E("The product code error. ");
        return(false);
    }
    #endif
    
    if (strcmp(fw_info.part_name, QBOOT_APP_PART_NAME) != 0)
    {
        LOG_E("Qboot resume fail from %s.", src_part_name);
        LOG_E("The firmware of %s partition is not application. fw_info.part_name(%s) != %s", src_part_name, fw_info.part_name, QBOOT_APP_PART_NAME);
        return(false);
    }
    
    if ( ! qbt_fw_update(QBOOT_APP_PART_NAME, src_part_name, &fw_info))
    {   
        return(false);
    }
    
    LOG_I("Qboot resume success from %s.", src_part_name);
    return(true);
}

static bool qbt_release_from_part(const char *part_name, bool check_sign)
{
    if ( ! qbt_fw_check(part_name, &fw_info, true))
    {
        return(false);
    }

    #ifdef QBOOT_USING_PRODUCT_CODE
    if ( strcmp(fw_info.prod_code, QBOOT_PRODUCT_CODE) != 0)
    {
        LOG_E("The product code error.");
        return(false);
    }
    #endif
    
    if (check_sign)
    {
        if (qbt_release_sign_check(part_name, &fw_info))//not need release
        {
            return(true);
        }
    }
    
    if ( ! qbt_fw_update(fw_info.part_name, part_name, &fw_info))
    {
        return(false);
    }

    if ( ! qbt_release_sign_check(part_name, &fw_info))
    {
        qbt_release_sign_write(part_name, &fw_info);
    }
    
    LOG_I("Release firmware success from %s to %s.", part_name, fw_info.part_name);
    return(true);
}

static void qbt_thread_entry(void *params)
{
    #define QBOOT_REBOOT_DELAY_MS       5000
    
    #ifdef QBOOT_USING_SHELL
    qbt_close_sys_shell();
    #endif
    
    qbt_show_msg();
    
    #ifdef QBOOT_USING_STATUS_LED
    qbt_status_led_init();
    #endif
    
    if (fal_init() <= 0)
    {
        LOG_E("Qboot initialize fal fail.");
        
        #ifdef QBOOT_USING_SHELL
        if (qbt_startup_shell(false))
        {
            return;
        }
        #endif
        
        LOG_I("Qboot will reboot after %d ms.", QBOOT_REBOOT_DELAY_MS);
        rt_thread_mdelay(QBOOT_REBOOT_DELAY_MS);
        rt_hw_cpu_reset();
    }

    #ifdef QBOOT_USING_FACTORY_KEY
    if (qbt_factory_key_check())
    {
        if (qbt_app_resume_from(QBOOT_FACTORY_PART_NAME))
        {
            qbt_jump_to_app();
        }
    }
    #endif

    #ifdef QBOOT_USING_SHELL
    if (qbt_startup_shell(true))
    {
        return;
    }
    #endif

    qbt_release_from_part(QBOOT_DOWNLOAD_PART_NAME, true);
    qbt_jump_to_app();
    
    LOG_I("Try resume application from %s", QBOOT_DOWNLOAD_PART_NAME);
    if (qbt_app_resume_from(QBOOT_DOWNLOAD_PART_NAME))
    {
        qbt_jump_to_app();
    }

    LOG_I("Try resume application from %s", QBOOT_FACTORY_PART_NAME);
    if (qbt_app_resume_from(QBOOT_FACTORY_PART_NAME))
    {
        qbt_jump_to_app();
    }

    #ifdef QBOOT_USING_SHELL
    if (qbt_startup_shell(false))
    {
        return;
    }
    #endif
    
    LOG_I("Qboot will reboot after %d ms.", QBOOT_REBOOT_DELAY_MS);
    rt_thread_mdelay(QBOOT_REBOOT_DELAY_MS);
    rt_hw_cpu_reset();
}

static void qbt_startup(void)
{
    rt_thread_t tid = rt_thread_create("Qboot", qbt_thread_entry, NULL, QBOOT_THREAD_STACK_SIZE, QBOOT_THREAD_PRIO, 20);
    if (tid == NULL)
    {
        LOG_E("Qboot thread create fail.");
        return;
    }
    
    rt_thread_startup(tid);
}
INIT_APP_EXPORT(qbt_startup);

#ifdef QBOOT_USING_SHELL
static bool qbt_fw_clone(const char *dst_part_name, const char *src_part_name, u32 fw_pkg_size)
{
    u32 pos = 0;
    fal_partition_t src_part = fal_partition_find(src_part_name);
    fal_partition_t dst_part = fal_partition_find(dst_part_name);

    rt_kprintf("Erasing %s partition ... \n", dst_part_name);
    if (fal_partition_erase(dst_part, 0, fw_pkg_size) < 0)
    {
        LOG_E("Qboot clone firmware fail. erase %s error.", dst_part_name);
        return(false);
    }
    
    rt_kprintf("Cloning firmware from %s to %s ...    ", src_part_name, dst_part_name);
    while (pos < fw_pkg_size)
    {
        int read_len = QBOOT_BUF_SIZE;
        int remain_len = fw_pkg_size - pos;
        if (read_len > remain_len)
        {
            read_len = remain_len;
        }
        if (fal_partition_read(src_part, pos, cmprs_buf, read_len) < 0)
        {
            LOG_E("Qboot clone firmware fail. read error, part = %s, addr = %08X, length = %d", src_part_name, pos, read_len);
            return(false);
        }
        if (fal_partition_write(dst_part, pos, cmprs_buf, read_len) < 0)
        {
            LOG_E("Qboot clone firmware fail. write error, part = %s, addr = %08X, length = %d", dst_part_name, pos, read_len);
            return(false);
        }
        pos += read_len;
        rt_kprintf("\b\b\b%02d%%", (pos * 100 / fw_pkg_size));
    }
    rt_kprintf("\n");
    
    return(true);
}
static void qbt_fw_info_show(const char *part_name)
{
    char str[20];
    
    if ( ! qbt_fw_check(part_name, &fw_info, false))
    {
        return;
    }
        
    rt_memset(str, 0x0, sizeof(str));
    switch(fw_info.algo & QBOOT_ALGO_CRYPT_MASK)
    {
    case QBOOT_ALGO_CRYPT_NONE:
        strcpy(str, "NONE");
        break;
    case QBOOT_ALGO_CRYPT_XOR:
        strcpy(str, "XOR");
        break;
    case QBOOT_ALGO_CRYPT_AES:
        strcpy(str, "AES");
        break;
    default:
        strcpy(str, "UNKNOW");
        break;
    }
    switch(fw_info.algo & QBOOT_ALGO_CMPRS_MASK)
    {
    case QBOOT_ALGO_CMPRS_NONE:
        strcpy(str + strlen(str), " && NONE");
        break;
    case QBOOT_ALGO_CMPRS_GZIP:
        strcpy(str + strlen(str), " && GZIP");
        break;
    case QBOOT_ALGO_CMPRS_QUICKLZ:
        strcpy(str + strlen(str), " && QUCKLZ");
        break;
    case QBOOT_ALGO_CMPRS_FASTLZ:
        strcpy(str + strlen(str), " && FASTLZ");
        break;
    default:
        strcpy(str + strlen(str), " && UNKNOW");
        break;
    }
    rt_kprintf("==== Firmware infomation of %s partition ====\n", part_name);
    rt_kprintf("| Product code          | %*.s |\n", 20, fw_info.prod_code);
    rt_kprintf("| Algorithm mode        | %*.s |\n", 20, str);
    rt_kprintf("| Destition partition   | %*.s |\n", 20, fw_info.part_name);
    rt_kprintf("| Version               | %*.s |\n", 20, fw_info.fw_ver);
    rt_kprintf("| Package size          | %20d |\n", fw_info.pkg_size);
    rt_kprintf("| Raw code size         | %20d |\n", fw_info.raw_size);
    rt_kprintf("| Package crc           | %20X |\n", fw_info.pkg_crc);    
    rt_kprintf("| Raw code verify       | %20X |\n", fw_info.raw_crc);
    rt_kprintf("| Header crc            | %20X |\n", fw_info.hdr_crc);
    rt_kprintf("| Build timestamp       | %20d |\n", fw_info.time_stamp);
    rt_kprintf("\n");
}
static bool qbt_fw_delete(const char *part_name, u32 part_size)
{
    fal_partition_t dst_part = fal_partition_find(part_name);
    
    rt_kprintf("Erasing %s partition ... \n", part_name);
    if (fal_partition_erase(dst_part, 0, part_size) < 0)
    {
        LOG_E("Qboot delete firmware fail. erase %s error.", part_name);
        return(false);
    }
    
    rt_kprintf("Qboot delete firmware success.\n");
    
    return(true);
}
static void qbt_shell_cmd(rt_uint8_t argc, char **argv)
{
    const char *cmd_info[] = {
        "Usage:\n",
        "qboot probe                    - probe firmware packages\n",
        "qboot resume src_part          - resume application from src partiton\n",
        "qboot clone src_part dst_part  - clone src partition to dst partiton\n",
        "qboot release part             - release firmware from partiton\n",
        "qboot verify part              - verify released code of partition\n",
        "qboot del part                 - delete firmware of partiton\n",
        "qboot jump                     - jump to application\n",
        "\n"
        };
        
    if (argc < 2)
    {
        for (int i = 0; i < sizeof(cmd_info)/sizeof(char *); i++)
        {
            rt_kprintf(cmd_info[i]);
        }
        return;
    }  
    
    if (strcmp(argv[1], "probe") == 0)
    {
        qbt_fw_info_show(QBOOT_DOWNLOAD_PART_NAME);
        qbt_fw_info_show(QBOOT_FACTORY_PART_NAME);
        return;
    }
    
    if (strcmp(argv[1], "resume") == 0)
    {
        if (argc < 3)
        {
            rt_kprintf(cmd_info[2]);
            return;
        }
        qbt_app_resume_from(argv[2]);
            
        #ifdef QBOOT_USING_STATUS_LED
        qled_set_blink(QBOOT_STATUS_LED_PIN, 50, 950);
        #endif
        
        return;
    }
    
    if (strcmp(argv[1], "clone") == 0)
    {
        char *src, *dst;
        if (argc < 4)
        {
            rt_kprintf(cmd_info[3]);
            return;
        }
        src = argv[2];
        dst = argv[3];
        if ( ! qbt_part_is_exist(dst))
        {
            rt_kprintf("Desttition %s partition is not exist.\n", dst);
            return;
        }
        if ( ! qbt_fw_check(src, &fw_info, true))
        {
            rt_kprintf("Soure %s partition firmware error.\n", src);
            return;
        }
        if (qbt_fw_clone(dst, src, sizeof(fw_info_t) + fw_info.pkg_size))
        {
            rt_kprintf("Clone firmware success from %s to %s.\n", src, dst);
        }
        return;
    }

    if (strcmp(argv[1], "release") == 0)
    {
        char *part_name;
        if (argc < 3)
        {
            rt_kprintf(cmd_info[4]);
            return;
        }
        part_name = argv[2];
        if (qbt_release_from_part(part_name, false))
        {
            rt_kprintf("Release firmware success from %s partition.\n", part_name);
        }
        else
        {
            rt_kprintf("Release firmware fail from %s partition.\n", part_name);
        }
        return;
    }
    
    if (strcmp(argv[1], "verify") == 0)
    {
        char *part_name;
        if (argc < 3)
        {
            rt_kprintf(cmd_info[5]);
            return;
        }
        part_name = argv[2];
        if ( ! qbt_part_is_exist(part_name))
        {
            rt_kprintf("%s partition is not exist.\n", part_name);
            return;
        }
        if (qbt_dest_part_verify(part_name))
        {
            rt_kprintf("%s partition code verify ok.\n", part_name);
        }
        else
        {
            rt_kprintf("%s partition code verify error.\n", part_name);
        }
        return;
    }
    
    if (strcmp(argv[1], "del") == 0)
    {
        char *part_name;
        
        if (argc < 3)
        {
            rt_kprintf(cmd_info[6]);
            return;
        }

        part_name = argv[2];
        if ( ! qbt_fw_check(part_name, &fw_info, false))
        {
            rt_kprintf("%s partition without firmware.\n", part_name);
            return;
        }
            
        qbt_fw_delete(part_name, fw_info.pkg_size);
        
        return;
    }
    
    if (strcmp(argv[1], "jump") == 0)
    {
        qbt_jump_to_app();
        return;
    }
    
    rt_kprintf("No supported command.\n");
}
MSH_CMD_EXPORT_ALIAS(qbt_shell_cmd, qboot, Quick bootloader test commands);
#endif

