
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "main.h"
#include "uf2.h"
#include "flash.h"

#define STATIC_ASSERT(_exp) _Static_assert(_exp, "static assert failed")

typedef struct
{
    uint8_t JumpInstruction[3];
    uint8_t OEMInfo[8];
    uint16_t SectorSize;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FATCopies;
    uint16_t RootDirectoryEntries;
    uint16_t TotalSectors16;
    uint8_t MediaDescriptor;
    uint16_t SectorsPerFAT;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t TotalSectors32;
    uint8_t PhysicalDriveNum;
    uint8_t Reserved;
    uint8_t ExtendedBootSig;
    uint32_t VolumeSerialNumber;
    uint8_t VolumeLabel[11];
    uint8_t FilesystemIdentifier[8];
} __attribute__((packed)) FAT_BootBlock;

typedef struct
{
    char name[8];
    char ext[3];
    uint8_t attrs;
    uint8_t reserved;
    uint8_t createTimeFine;
    uint16_t createTime;
    uint16_t createDate;
    uint16_t lastAccessDate;
    uint16_t highStartCluster;
    uint16_t updateTime;
    uint16_t updateDate;
    uint16_t startCluster;
    uint32_t size;
} __attribute__((packed)) DirEntry;

// #define READ_UF2

#ifndef READ_UF2
#define CODE_BIN_SIZE (8 * 1024 * 1024)
#define DATA_BIN_SIZE (16 * 1024 * 1024)
#define CODE_FILE_SIZE CODE_BIN_SIZE
#define DATA_FILE_SIZE DATA_BIN_SIZE

#else
#define CODE_UF2_SIZE (16 * 1024 * 1024)
#define DATA_UF2_SIZE (32 * 1024 * 1024)
#define CODE_FILE_SIZE CODE_UF2_SIZE
#define DATA_FILE_SIZE DATA_UF2_SIZE

#endif

#define PAYLOAD_SIZE (256)

#define VFS_SIZE (128 * 1024 * 1024)

#define SECTOR_SIZE (0x200)
#define SECTOR_PER_CLUSTER (0x4)
#define RESERVED_SECTOR (0x4)
#define FAT_COPIES (0x2)
#define ROOT_DIR_ENTRIES (0x200)
#define SECTOR_PER_FAT (((VFS_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER)) / (SECTOR_SIZE / 2)) + \
                        ((VFS_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER)) % (SECTOR_SIZE / 2) ? 1 : 0))

#define START_FAT (RESERVED_SECTOR)
#define FAT_SECTORS (SECTOR_PER_FAT * 2)

#define START_ROOT (START_FAT + FAT_SECTORS)
#define ROOT_SECTORS (ROOT_DIR_ENTRIES * sizeof(DirEntry) / SECTOR_SIZE)

#define START_DATA (START_ROOT + ROOT_SECTORS)
#define DATA_SECTORS (VFS_SIZE / SECTOR_SIZE - START_DATA)

STATIC_ASSERT(SECTOR_SIZE == 0x200);
STATIC_ASSERT(SECTOR_PER_CLUSTER == 0x4 || SECTOR_PER_CLUSTER == 0x8 || SECTOR_PER_CLUSTER == 0x10);
STATIC_ASSERT(FAT_COPIES == 0x2);
STATIC_ASSERT(ROOT_DIR_ENTRIES == 0x200);
STATIC_ASSERT(VFS_SIZE <= (65536 * SECTOR_SIZE * SECTOR_PER_CLUSTER));

static FAT_BootBlock const BootBlock = {
    .JumpInstruction = {0xeb, 0x3c, 0x90},
    .OEMInfo = "UF2 UF2 ",
    .SectorSize = SECTOR_SIZE,
    .SectorsPerCluster = SECTOR_PER_CLUSTER,
    .ReservedSectors = RESERVED_SECTOR,
    .FATCopies = FAT_COPIES,
    .RootDirectoryEntries = ROOT_DIR_ENTRIES,
    .TotalSectors16 = 0x0,
    .MediaDescriptor = 0xf8,
    .SectorsPerFAT = SECTOR_PER_FAT,
    .SectorsPerTrack = 1,
    .Heads = 1,
    .TotalSectors32 = (VFS_SIZE / SECTOR_SIZE),
    .PhysicalDriveNum = 0x80, // to match MediaDescriptor of 0xF8
    .ExtendedBootSig = 0x29,
    .VolumeSerialNumber = 0xbb8d45e2,
    .VolumeLabel = UF2_VOLUME_LABEL,
    .FilesystemIdentifier = "FAT16   ",
};

typedef struct
{
    char const name[11];
    char const *content;
    uint32_t start_cluster;
    uint32_t clusters;
} TextFile;

const char infoUf2File[] =
    "UF2-BOOT Version: " UF2_VERSION "\r\n"
    "Build: " __DATE__ " " __TIME__ "\r\n"
    "Model: " UF2_PRODUCT_NAME "\r\n"
    "Board-ID: " UF2_BOARD_ID "\r\n";

const char indexFile[] =
    "<!doctype html>\n"
    "<html>"
    "<body>"
    "<script>\n"
    "location.replace(\"" UF2_INDEX_URL "\");\n"
    "</script>"
    "</body>"
    "</html>\n";

static TextFile info[] = {
    {.name = "INFO_UF2TXT", .content = infoUf2File, .start_cluster = 0, .clusters = 1},
    {.name = "INDEX   HTM", .content = indexFile, .start_cluster = 0, .clusters = 1},
#ifndef READ_UF2
    // .uf2 must be the last element and its content must be NULL
    {.name = "CODE    BIN", .content = NULL, .start_cluster = 0, .clusters = (CODE_FILE_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER))},
    {.name = "DATA    BIN", .content = NULL, .start_cluster = 0, .clusters = (DATA_FILE_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER))},
#else
    {.name = "CODE    UF2", .content = NULL, .start_cluster = 0, .clusters = (CODE_FILE_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER))},
    {.name = "DATA    UF2", .content = NULL, .start_cluster = 0, .clusters = (DATA_FILE_SIZE / (SECTOR_SIZE * SECTOR_PER_CLUSTER))},
#endif
};

enum
{
    CODE_BLK = 2,
    DATA_BLK,
};

static uint32_t flash_addr_map(int code)
{
    switch (code)
    {
    case CODE_BLK:
        return 0x90000000UL;
    case DATA_BLK:
        return 0xA0000000UL;
    default:
        return 0x0;
    }
}

static void padded_memcpy(char *dst, char const *src, int len)
{
    for (int i = 0; i < len; ++i)
    {
        if (*src)
        {
            *dst = *src++;
        }
        else
        {
            *dst = ' ';
        }
        dst++;
    }
}

static inline bool is_uf2_block(UF2_Block const *bl)
{
    return (bl->magicStart0 == UF2_MAGIC_START0) &&
           (bl->magicStart1 == UF2_MAGIC_START1) &&
           (bl->magicEnd == UF2_MAGIC_END) &&
           (bl->flags & UF2_FLAG_FAMILYID) &&
           !(bl->flags & UF2_FLAG_NOFLASH);
}

/*------------------------------------------------------------------*/
/* Init UF2
 *------------------------------------------------------------------*/
void uf2_init(void)
{
    printf("SECTOR_PER_FAT(0x%x) \r\n", SECTOR_PER_FAT);
    printf("START_FAT(0x%x) \r\n", START_FAT);
    printf("FAT_SECTORS(0x%x) \r\n", FAT_SECTORS);
    printf("START_ROOT(0x%x) \r\n", START_ROOT);
    printf("ROOT_SECTORS(0x%x) \r\n", ROOT_SECTORS);
    printf("START_DATA(0x%x) \r\n", START_DATA);
    printf("DATA_SECTORS(0x%x) \r\n", DATA_SECTORS);

    // init info
    static uint32_t cluster = 2; // 簇号0,1为系统保留.
    for (int i = 0; i < (sizeof(info) / sizeof(TextFile)); i++)
    {
        info[i].start_cluster = cluster;
        cluster += info[i].clusters;
        printf("info[%d].start_cluster(0x%x) \r\n", i, info[i].start_cluster);
        printf("info[%d].clusters(0x%x) \r\n", i, info[i].clusters);
    }
}

/*------------------------------------------------------------------*/
/* Read UF2
 *------------------------------------------------------------------*/
void uf2_read_block(uint32_t block_no, uint8_t *data)
{
    //printf("block_no(#0x%x) \r\n", block_no);
    memset(data, 0, 0x200);
    //HAL_Delay(10);
    uint32_t sectionIdx = block_no;

    if (block_no == 0)
    {
        // Requested boot block
        memcpy(data, &BootBlock, sizeof(BootBlock));
        data[510] = 0x55; // Always at offsets 510/511, even when BPB_SECTOR_SIZE is larger
        data[511] = 0xaa; // Always at offsets 510/511, even when BPB_SECTOR_SIZE is larger
    }

    if (START_FAT <= block_no && block_no < (START_FAT + FAT_SECTORS)) // FAT Tab
    {
        sectionIdx -= START_FAT;

        if (sectionIdx >= SECTOR_PER_FAT)
        {
            sectionIdx -= SECTOR_PER_FAT;
        }

        if (sectionIdx == 0)
        {
            // first FAT entry must match BPB MediaDescriptor
            data[0] = 0xf8;
            data[1] = 0xff;
            data[2] = 0xff;
            data[3] = 0xff;

            // WARNING -- code presumes only one NULL .content for .UF2 file
            //            and all non-NULL .content fit in one sector
            //            and requires it be the last element of the array
        }

        for (uint32_t i = 0; i < (SECTOR_SIZE / sizeof(uint16_t)); i++)
        {
            // Generate the FAT chain for the firmware "file"
            uint32_t v = (sectionIdx * (SECTOR_SIZE / (sizeof(uint16_t)))) + i;
            uint16_t *p = (uint16_t *)data;

            for (int j = 0; j < (sizeof(info) / sizeof(TextFile)); j++)
            {
                uint32_t sc = info[j].start_cluster;
                uint32_t c = info[j].start_cluster + info[j].clusters;

                if (sc <= v && v < c)
                {
                    p[i] = v == (c - 1) ? 0xffff : v + 1;
                    break;
                }
            }
        }
    }

    if (START_ROOT <= block_no && block_no < (START_ROOT + ROOT_SECTORS))
    {
        // Requested root directory sector

        sectionIdx -= START_ROOT;

        DirEntry *d = (void *)data;
        int remainingEntries = (SECTOR_SIZE / sizeof(DirEntry));
        if (sectionIdx == 0)
        {
            // volume label first
            // volume label is first directory entry
            padded_memcpy(d->name, (char const *)BootBlock.VolumeLabel, 11);
            d->attrs = 0x28;
            d++;
            remainingEntries--;
        }

        for (uint32_t i = SECTOR_SIZE / sizeof(DirEntry) * sectionIdx;
             remainingEntries > 0 && i < (sizeof(info) / sizeof(TextFile));
             i++, d++)
        {
            // WARNING -- code presumes all but last file take exactly one sector

            TextFile *inf = &info[i];
            padded_memcpy(d->name, inf->name, 11);
            d->createTimeFine = __SECONDS_INT__ % 2 * 100;
            d->createTime = __DOSTIME__;
            d->createDate = __DOSDATE__;
            d->lastAccessDate = __DOSDATE__;
            d->highStartCluster = inf->start_cluster >> 16;
            d->updateTime = __DOSTIME__;
            d->updateDate = __DOSDATE__;
            d->startCluster = inf->start_cluster & 0xFFFF;
            d->size = (inf->content ? strlen(inf->content) : (inf->clusters * SECTOR_PER_CLUSTER * SECTOR_SIZE));
        }
    }

    if (START_DATA <= block_no && block_no < (START_DATA + DATA_SECTORS))
    {
        sectionIdx -= START_DATA;
        //printf("sectionIdx(0x%x) \r\n", sectionIdx);
        for (int i = 0; i < sizeof(info) / sizeof(TextFile); i++)
        {
            uint32_t sc = info[i].start_cluster - 2;

            if (sc * SECTOR_PER_CLUSTER <= sectionIdx &&
                sectionIdx < (sc + info[i].clusters) * SECTOR_PER_CLUSTER)
            {
                uint32_t idx = sectionIdx - sc * SECTOR_PER_CLUSTER;
                if (info[i].content)
                {
                    //printf("idx(0x%x) \r\n", idx);
                    uint32_t sec = strlen(info[i].content) / SECTOR_SIZE + strlen(info[i].content) % SECTOR_SIZE ? 1 : 0;
                    if (idx < sec)
                    {
                        memcpy(data, info[i].content + (idx % SECTOR_PER_CLUSTER) * SECTOR_SIZE, SECTOR_SIZE); // 注意处理最后一块的多余字节
                    }
                }
                else
                {
                    //printf("idx(0x%x) \r\n", idx);
#ifndef READ_UF2
                    uint32_t addr = flash_addr_map(i) + (idx * SECTOR_SIZE);
                    if (addr < (flash_addr_map(i) + (info[i].clusters * SECTOR_PER_CLUSTER * SECTOR_SIZE))) // TODO abstract this out
                    {                                                                                       // 注意addr起始地址
                        // printf("addr(0x%x) \r\n", addr);
                        flash_read(addr, data, SECTOR_SIZE);
                    }
#else
                    uint32_t addr = flash_addr_map(i) + (idx * PAYLOAD_SIZE);
                    if (addr < (flash_addr_map(i) + (info[i].clusters * SECTOR_PER_CLUSTER * SECTOR_SIZE / 2))) // TODO abstract this out
                    {                                                                                           // 注意addr起始地址
                        // printf("addr(0x%x) \r\n", addr);
                        UF2_Block *bl = (void *)data;
                        bl->magicStart0 = UF2_MAGIC_START0;
                        bl->magicStart1 = UF2_MAGIC_START1;
                        bl->magicEnd = UF2_MAGIC_END;
                        bl->blockNo = idx;
                        bl->numBlocks = (info[i].clusters * SECTOR_PER_CLUSTER / 2) / PAYLOAD_SIZE;
                        bl->targetAddr = addr;
                        bl->payloadSize = PAYLOAD_SIZE;
                        bl->flags = UF2_FLAG_FAMILYID;
                        bl->familyID = BOARD_UF2_FAMILY_ID;

                        flash_read(addr, bl->data, bl->payloadSize);
                    }
#endif
                }

                break;
            }
        }
    }
}

/*------------------------------------------------------------------*/
/* Write UF2
 *------------------------------------------------------------------*/

/**
 * Write an uf2 block wrapped by 512 sector.
 * @return number of bytes processed, only 3 following values
 *  -1 : if not an uf2 block
 * 512 : write is successful (BPB_SECTOR_SIZE == 512)
 *   0 : is busy with flashing, tinyusb stack will call write_block again with the same parameters later on
 */
int uf2_write_block(uint32_t block_no, uint8_t *data)
{
    (void)block_no;
    UF2_Block *bl = (void *)data;

    if (!is_uf2_block(bl))
        return -1;

    if (bl->familyID == BOARD_UF2_FAMILY_ID)
    {
        // generic family ID
        flash_write(bl->targetAddr, bl->data, bl->payloadSize);
        // printf("block_no(#0x%x) \r\n", block_no);
    }
    else
    {
        // TODO family matches VID/PID
        return -1;
    }

    //------------- Update written blocks -------------//

    return SECTOR_SIZE;
}
