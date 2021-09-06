/*
 * qboot_quicklz.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-06     qiyongzhong       first version
 */

#include <qboot_quicklz.h>

#ifdef QBOOT_USING_QUICKLZ

#include <quicklz.h>

static qlz_state_decompress qbt_quicklz_state;

void qbt_quicklz_state_init(void)
{
    memset((u8 *)&qbt_quicklz_state, 0, sizeof(qbt_quicklz_state));
}

u32 qbt_quicklz_get_block_size(const u8 *comp_datas)
{
    u32 block_size = 0;
    for(int i = 0; i < QBOOT_QUICKLZ_BLOCK_HDR_SIZE; i++)
    {
        block_size <<= 8;
        block_size += comp_datas[i];
    }
    return(block_size);
}

u32 qbt_quicklz_decompress(u8 *out_buf, const u8 *in_buf)
{
    return(qlz_decompress(in_buf, out_buf, &qbt_quicklz_state));
}

#endif

