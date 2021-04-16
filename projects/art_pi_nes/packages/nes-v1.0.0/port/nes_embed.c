/*===================================================================*/
#include <InfoNES.h>
#include <InfoNES_Mapper.h>
#include <InfoNES_pAPU.h>
#include <InfoNES_System.h>
#include <K6502.h>
/*                                                                   */
/*  nes_port.cpp : port File                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
//User include file
#include <nes_cfg.h>
#include <nes_file_port.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

// Palette data
WORD NesPalette[ 64 ] =
{
    0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
    0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
    0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
    0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
    0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
    0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
    0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
    0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000

};

//User

void nesmain(const char *name)
{
    if (0 != InfoNES_Load(name))
    {
        InfoNES_ReleaseRom();
        return;
    }

    InfoNES_Main();
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
int InfoNES_Menu()
{
    /*
     *  Menu screen
     *
     *  Return values
     *     0 : Normally
     *    -1 : Exit InfoNES
     */

    if (PAD_PUSH(PAD_System, PAD_SYS_QUIT))
        return -1;

    // Nothing to do here
    return 0;
}


/*===================================================================*/
/*                                                                   */
/*               InfoNES_ReadRom() : Read ROM image file             */
/*                                                                   */
/*===================================================================*/
int InfoNES_ReadRom(const char *pszFileName)
{
    /*
     *  Read ROM image file
     *
     *  Parameters
     *    const char *pszFileName          (Read)
     *
     *  Return values
     *     0 : Normally
     *    -1 : Error
     */
#ifdef PKG_NES_DOUBLE_FRAMEBUFFER
    DoubleFrame[0] = malloc(NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(WORD));
    if (DoubleFrame[0] == NULL)
        return -1;
    DoubleFrame[1] = malloc(NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(WORD));
    if (DoubleFrame[1] == NULL)
        return -1;
#else
    WorkFrame = malloc(NES_DISP_WIDTH * NES_DISP_HEIGHT * sizeof(WORD));
    if (WorkFrame == NULL)
        return -1;
#endif

    SRAM = malloc(SRAM_SIZE * sizeof(BYTE));
    if (SRAM == NULL)
        return -1;

    PPURAM = malloc(PPURAM_SIZE * sizeof(BYTE));
    if (PPURAM == NULL)
        return -1;

    SPRRAM = malloc(SPRRAM_SIZE * sizeof(BYTE));
    if (SPRRAM == NULL)
        return -1;

    /* Open ROM file */
    if (nesOpenFile(pszFileName) < 0)
        return -1;

    /* Read ROM Header */
    nesReadFile(&NesHeader, sizeof NesHeader, 1);

    if (memcmp(NesHeader.byID, "NES\x1a", 4) != 0)
    {
        nesCloseFile();
        return -1;
    }

    /* Clear SRAM */
    memset(SRAM, 0, SRAM_SIZE);

    /* If trainer presents Read Triner at 0x7000-0x71ff */
    if (NesHeader.byInfo1 & 4)
        nesReadFile(&SRAM[ 0x1000 ], 512, 1);

    /* Allocate Memory for ROM Image */
    ROM = (BYTE *)malloc(NesHeader.byRomSize * 0x4000);

    if (ROM == NULL)
        return -1;
    /* Read ROM Image */
    nesReadFile(ROM, 0x4000, NesHeader.byRomSize);

    VROM = NULL;
    if (NesHeader.byVRomSize > 0)
    {
        /* Allocate Memory for VROM Image */
        VROM = (BYTE *)malloc(NesHeader.byVRomSize * 0x2000);

        if (VROM == NULL)
            return -1;
        /* Read VROM Image */
        nesReadFile(VROM, 0x2000, NesHeader.byVRomSize);
    }

    /* File close */
    nesCloseFile();
    /* Successful */
    return 0;
}

/*===================================================================*/
/*                                                                   */
/*           InfoNES_ReleaseRom() : Release a memory for ROM         */
/*                                                                   */
/*===================================================================*/
void InfoNES_ReleaseRom()
{
    /*
     *  Release a memory for ROM
     *
     */
#ifdef PKG_NES_DOUBLE_FRAMEBUFFER
    if (DoubleFrame[0])
    {
        free(DoubleFrame[0]);
        DoubleFrame[0] = NULL;
    }

    if (DoubleFrame[1])
    {
        free(DoubleFrame[1]);
        DoubleFrame[1] = NULL;
    }
    WorkFrame = NULL;
    WorkFrameIdx = 0;
#else
    if (WorkFrame)
    {
        free(WorkFrame);
        WorkFrame = NULL;
    }
#endif

    if (SRAM)
    {
        free(SRAM);
        SRAM = NULL;
    }

    if (PPURAM)
    {
        free(PPURAM);
        PPURAM = NULL;
    }

    if (SPRRAM)
    {
        free(SPRRAM);
        SPRRAM = NULL;
    }

    if (ROM)
    {
        free(ROM);
        ROM = NULL;
    }

    if (VROM)
    {
        free(VROM);
        VROM = NULL;
    }
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemoryCopy() : memcpy                         */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemoryCopy(void *dest, const void *src, int count)
{
    /*
     *  memcpy
     *
     *  Parameters
     *    void *dest                       (Write)
     *      Points to the starting address of the copied block's destination
     *
     *    const void *src                  (Read)
     *      Points to the starting address of the block of memory to copy
     *
     *    int count                        (Read)
     *      Specifies the size, in bytes, of the block of memory to copy
     *
     *  Return values
     *    Pointer of destination
     */

    memcpy(dest, src, count);
    return dest;
}


/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemorySet() : memset                          */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemorySet(void *dest, int c, int count)
{
    /*
     *  memset
     *
     *  Parameters
     *    void *dest                       (Write)
     *      Points to the starting address of the block of memory to fill
     *
     *    int c                            (Read)
     *      Specifies the byte value with which to fill the memory block
     *
     *    int count                        (Read)
     *      Specifies the size, in bytes, of the block of memory to fill
     *
     *  Return values
     *    Pointer of destination
     */

    memset(dest, c, count);
    return dest;
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_Wait() : Wait Emulation if required            */
/*                                                                   */
/*===================================================================*/
void InfoNES_Wait()
{
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_MessageBox() : Print System Message            */
/*                                                                   */
/*===================================================================*/
void InfoNES_MessageBox(char *pszMsg, ...)
{
    va_list args;
    va_start(args, pszMsg);
    printf(pszMsg, args);
    va_end(args);
}
