#include "fs_operation.h"
#include <fal.h>
#include <dfs_fs.h>
#include <dfs_romfs.h>
#include <dfs_posix.h>
#include "drv_sdio.h"

#define DBG_TAG "fs_op"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

/* SD Card hot plug detection pin */
#define SD_CHECK_PIN GET_PIN(D, 5)

static void _sdcard_mount(struct global_sundry *g_sundry)
{
    rt_device_t device;

    device = rt_device_find("sd0");
    if (device == NULL)
    {
        mmcsd_wait_cd_changed(0);
        sdcard_change();
        mmcsd_wait_cd_changed(RT_WAITING_FOREVER);
        device = rt_device_find("sd0");
    }
    if (device != RT_NULL)
    {
        if (dfs_mount("sd0", SD_PATH, "elm", 0, 0) == RT_EOK)
        {
            LOG_I("sd card mount to '%s'", SD_PATH);
            g_sundry->sd_mount = 1;
        }
        else
        {
            LOG_W("sd card mount to '%s' failed!", SD_PATH);
        }
    }
}

static void _sdcard_unmount(struct global_sundry *g_sundry)
{
    rt_thread_mdelay(200);
    dfs_unmount(SD_PATH);
    LOG_I("Unmount \"%s\"", SD_PATH);

    g_sundry->sd_mount = 0;
    mmcsd_wait_cd_changed(0);
    sdcard_change();
    mmcsd_wait_cd_changed(RT_WAITING_FOREVER);
}

static void sd_mount(void *parameter)
{
    rt_uint8_t re_sd_check_pin = 1;
    struct global_sundry *g_sundry = parameter;
    rt_pin_mode(SD_CHECK_PIN, PIN_MODE_INPUT_PULLUP);
    rt_thread_mdelay(200);
    if(rt_pin_read(SD_CHECK_PIN))
    {
        _sdcard_mount(g_sundry);
    }
    while (1)
    {
        rt_thread_mdelay(200);
        if(!re_sd_check_pin && (re_sd_check_pin = rt_pin_read(SD_CHECK_PIN)) != 0)
        {
            _sdcard_mount(g_sundry);
        }

        if (re_sd_check_pin && (re_sd_check_pin = rt_pin_read(SD_CHECK_PIN)) == 0)
        {
            _sdcard_unmount(g_sundry);
        }
    }
}

int fs_init(struct global_sundry *g_sundry)
{
#define FS_PARTITION_NAME        "filesystem"

    if(dfs_mount(RT_NULL, "/", "rom", 0, &romfs_root) != 0)
    {
        LOG_E("romfs mount error, now retrying...");
        HAL_NVIC_SystemReset();
    }
    LOG_I("romfs mount to '/'");

    struct rt_device *mtd_dev = fal_mtd_nor_device_create(FS_PARTITION_NAME);
    RT_ASSERT(mtd_dev != RT_NULL);

    if(dfs_mount(FS_PARTITION_NAME, NORFLASH_PATH, "lfs", 0, 0) != 0)
    {
        LOG_W("norflash mount failed, now retrying...");
        dfs_mkfs("lfs", FS_PARTITION_NAME);
        rt_thread_mdelay(200);
        HAL_NVIC_SystemReset();
    }
    LOG_I("norflash mount to '%s'", NORFLASH_PATH);
    g_sundry->norflash_mount = 1;

    rt_thread_t tid = rt_thread_create("sd_mount", sd_mount, g_sundry, 2048, RT_THREAD_PRIORITY_MAX - 3, 100);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    rt_thread_mdelay(1000);

    if(!g_sundry->sd_mount)
        LOG_W("sd card mount to '%s' failed!", SD_PATH);

    return RT_EOK;
}

char *fs_read_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
    {
        LOG_E("cannot find config file: %s!", path);
        return RT_NULL;
    }

    fseek(fp, 0, SEEK_END);
    int fp_size = ftell(fp);
    rewind(fp);
    LOG_D("file size is %d", fp_size);
    if(fp_size <= 0)
    {
        fclose(fp);
        return RT_NULL;
    }

    char *buf = rt_malloc(fp_size + 1);
    if(buf == RT_NULL)
    {
        fclose(fp);
        return RT_NULL;
    }

    rt_memset(buf, 0, fp_size + 1);
    int result = fread(buf, fp_size, 1, fp);
    buf[fp_size] = 0;

    if (result != 1)
    {
        LOG_E("file read error");
        rt_free(buf);
        fclose(fp);
        return RT_NULL;
    }

    fclose(fp);
    return buf;
}
RTM_EXPORT(fs_read_file);

int fs_create_dir(const char *path)
{
    int result = RT_EOK;

    DIR *dir = opendir(path);
    if(dir == RT_NULL)
    {
        LOG_I("create %s dir.", path);
        if(mkdir(path, 0x777) != 0)
            result = -RT_ERROR;
    }
    else
        closedir(dir);

    return result;
}
RTM_EXPORT(fs_create_dir);

char* fs_normalize_path(char* fullpath)
{
    char *dst0, *dst, *src;

    src = fullpath;
    dst = fullpath;

    dst0 = dst;
    while (1)
    {
        char c = *src;

        if (c == '.')
        {
            if (!src[1]) src ++; /* '.' and ends */
            else if (src[1] == '/')
            {
                /* './' case */
                src += 2;

                while ((*src == '/') && (*src != '\0')) src ++;
                continue;
            }
            else if (src[1] == '.')
            {
                if (!src[2])
                {
                    /* '..' and ends case */
                    src += 2;
                    goto up_one;
                }
                else if (src[2] == '/')
                {
                    /* '../' case */
                    src += 3;

                    while ((*src == '/') && (*src != '\0')) src ++;
                    goto up_one;
                }
            }
        }

        /* copy up the next '/' and erase all '/' */
        while ((c = *src++) != '\0' && c != '/') *dst ++ = c;

        if (c == '/')
        {
            *dst ++ = '/';
            while (c == '/') c = *src++;

            src --;
        }
        else if (!c) break;

        continue;

up_one:
        dst --;
        if (dst < dst0) return RT_NULL;
        while (dst0 < dst && dst[-1] != '/') dst --;
    }

    *dst = '\0';

    /* remove '/' in the end of path if exist */
    dst --;
    if ((dst != fullpath) && (*dst == '/')) *dst = '\0';

    return fullpath;
}
RTM_EXPORT(fs_normalize_path);
