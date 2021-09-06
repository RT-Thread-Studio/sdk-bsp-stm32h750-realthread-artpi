/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-03     Ghazigq       the first version
 */
#include <rtthread.h>
#include <nes_file_port.h>

#define DBG_TAG     "nes.file"
#define DBG_LVL     DBG_INFO
#include <rtdbg.h>

#ifndef PKG_NES_DFS

static int file_index = 0;
static const nesGameFile *file_addr = 0;

const nesGameFile gameFileList[4] =
{
    {"SuperMario",      acSuperMario},
    {"CatAndMouse",     acCatAndMouse},
    {"Tanks",           acTanks},
    {"MacrossSeries",   acMacrossSeries},
};

int nesOpenFile(const char *fileName)
{
    int i = 0;

    file_index = 0;
    for (i = 0; i < sizeof(gameFileList) / sizeof(gameFileList[0]); i ++)
    {
        if (strcmp(gameFileList[i].gameName, fileName) == 0)
        {
            file_addr = &gameFileList[i];
            return 0;
        }
    }
    LOG_W("file open failed!");
    return -1;
}

int nesReadFile(void *buf, unsigned int len, unsigned short num)
{
    volatile char *p = buf;

    if (!file_addr || !file_addr->gameFileSrc)
        return -1;

    for (int i = 0; i < num; i++)
    {
        memcpy((char *)p, file_addr->gameFileSrc + file_index, len);
        file_index += len;
        p += len;
    }

    return 0;
}

int nesCloseFile(void)
{
    file_addr = NULL;
    file_index = 0;
    return 0;
}

char *nesGetFileName(int index)
{
    if (index < 0 || index >= sizeof(gameFileList) / sizeof(gameFileList[0]))
        return NULL;
    else
        return (char *)gameFileList[index].gameName;
}

void nesListFile(void)
{
    for (int i = 0; i < sizeof(gameFileList) / sizeof(gameFileList[0]); i ++)
        printf("%d\t%s\n", i, gameFileList[i].gameName);
}

#else

#include <dfs_posix.h>

static int fd = -1;

int nesOpenFile(const char *fileName)
{
    fd = open(fileName, O_RDONLY);
    if (fd < 0)
    {
        printf("file open failed!");
        return -1;
    }
    else
        return 0;
}

int nesReadFile(void *buf, unsigned int len, unsigned short num)
{
    char *p = buf;

    if (fd < 0)
        return -1;

    for (int i = 0; i < num; i++)
    {
        read(fd, p, len);
        p += len;
    }

    return 0;
}

int nesCloseFile(void)
{
    if (fd >= 0)
        close(fd);
    fd = -1;
    return 0;
}

char *nesGetFileName(int index)
{
    //TODO:
    return NULL;
}

#endif /* RT_USING_DFS */


#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) && !defined(PKG_NES_DFS)

static void nes(int argc, char **argv)
{
    if ((argc < 2) | (argc > 3))
        goto error;
    else
    {
        const char *operator = argv[1];
        if (!strcmp(operator, "list"))
        {
            if (argc == 2)
            {
                nesListFile();
                return;
            }
            else
                goto error;
        }
        else
            goto error;
    }

error:
    printf("nes list                - list all game's name\n");
}
MSH_CMD_EXPORT(nes, start a nes game);

#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) && !defined(PKG_NES_DFS) */

