/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-03     Ghazigq      the first version
 */
#ifndef _NES_PORT_H_
#define _NES_PORT_H_

#include "nes_cfg.h"

#define GAME_FILE_NAME_MAXLEN       20

typedef struct
{
    char gameName[GAME_FILE_NAME_MAXLEN];
    const unsigned char *gameFileSrc;
} nesGameFile, *pnesGameFile;

#ifndef PKG_NES_DFS
    extern const unsigned char acSuperMario[];
    extern const unsigned char acCatAndMouse[];
    extern const unsigned char acTanks[];
    extern const unsigned char acMacrossSeries[];
#endif

int nesOpenFile(const char *fileName);
int nesCloseFile(void);
int nesReadFile(void *buf, unsigned int len, unsigned short num);
#ifndef PKG_NES_DFS
    void nesListFile(void);
#endif
char *nesGetFileName(int index);

#endif /* _NES_PORT_H_ */
