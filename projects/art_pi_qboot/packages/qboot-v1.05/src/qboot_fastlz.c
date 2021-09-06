/*
 * qboot_fastlz.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */
 
#include <qboot_fastlz.h>
     
#ifdef QBOOT_USING_FASTLZ

#include <fastlz.h>
     
u32 qbt_fastlz_get_block_size(const u8 *comp_datas)
{
    u32 block_size = 0;
    for(int i = 0; i < QBOOT_FASTLZ_BLOCK_HDR_SIZE; i++)
    {
         block_size <<= 8;
         block_size += comp_datas[i];
    }
    return(block_size);
}

u32 qbt_fastlz_decompress(u8 *out_buf, u32 out_buf_size, const u8 *in_buf, u32 block_size)
{
    return(fastlz_decompress(in_buf, block_size, out_buf, out_buf_size));
}

#endif

