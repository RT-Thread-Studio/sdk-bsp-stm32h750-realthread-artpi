/*
 * qboot_gzip.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-08     qiyongzhong       first version
 * 2020-09-18     qiyongzhong       add deinit function
 */

#include <qboot_gzip.h>

#ifdef QBOOT_USING_GZIP

#include <zlib.h>

static z_stream qbt_strm;

void qbt_gzip_init(void)
{
    memset((u8 *)&qbt_strm, 0, sizeof(qbt_strm));
    inflateInit2(&qbt_strm, 47);
}

void qbt_gzip_set_in(const u8 *in_buf, u32 in_size)
{
    qbt_strm.next_in = in_buf;
    qbt_strm.avail_in = in_size;
}

int qbt_gzip_decompress(u8 *out_buf, u32 out_buf_size)
{
    int ret;
    
    qbt_strm.next_out = out_buf;
    qbt_strm.avail_out = out_buf_size;

    ret = inflate(&qbt_strm, Z_NO_FLUSH);
    switch (ret)
    {
    case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
        inflateEnd(&qbt_strm);
        return(ret);
    }

    return(out_buf_size - qbt_strm.avail_out);
}

void qbt_gzip_deinit(void)
{
    inflateEnd(&qbt_strm);
}
#endif

