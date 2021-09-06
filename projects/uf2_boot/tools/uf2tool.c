
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
Microchip (Atmel) SAMD21 - 0x68ed2b88
Microchip (Atmel) SAML21 - 0x1851780a
Microchip (Atmel) SAMD51 - 0x55114460
Nordic NRF52840 - 0xada52840
ST STM32F0xx - 0x647824b6
ST STM32F103 - 0x5ee21072
ST STM32F2xx - 0x5d1a0a2e
ST STM32F3xx - 0x6b846188
ST STM32F401 - 0x57755a57
ST STM32F407 - 0x6d0922fa
ST STM32F407VG - 0x8fb060fe
ST STM32F7xx - 0x53b80f00
ST STM32G0xx - 0x300f5633
ST STM32G4xx - 0x4c71240a
ST STM32H7xx - 0x6db66082
ST STM32L0xx - 0x202e3a91
ST STM32L1xx - 0x1e1f432d
ST STM32L4xx - 0x00ff6919
ST STM32L5xx - 0x04240bdf
ST STM32WBxx - 0x70d16653
ST STM32WLxx - 0x21460ff0
Microchip (Atmel) ATmega32 - 0x16573617
Cypress FX2 - 0x5a18069b
ESP8266 - 0x7eab61ed
ESP32 - 0x1c5f21b0
ESP32-S2 - 0xbfdd4eee
ESP32-C3 - 0xd42ba06c
ESP32-S3 - 0xc47e5767
NXP i.MX RT10XX - 0x4fb2d5bd
NXP LPC55xx - 0x2abc77ec
GD32F350 - 0x31d228c6
Raspberry Pi RP2040 - 0xe48bff56
*/

// All entries are little endian.

#define UF2_MAGIC_START0 0x0A324655UL // "UF2\n"
#define UF2_MAGIC_START1 0x9E5D5157UL // Randomly selected
#define UF2_MAGIC_END 0x0AB16F30UL    // Ditto

// If set, the block is "comment" and should not be flashed to the device
#define UF2_FLAG_NOFLASH 0x00000001
#define UF2_FLAG_FAMILYID 0x00002000

#define BOARD_UF2_FAMILY_ID 0x6db66082

#define APP_START_ADDRESS 0x90000000
#define DAT_START_ADDRESS 0xA0000000

typedef struct
{
    // 32 byte header
    uint32_t magicStart0;
    uint32_t magicStart1;
    uint32_t flags;
    uint32_t targetAddr;
    uint32_t payloadSize;
    uint32_t blockNo;
    uint32_t numBlocks;
    uint32_t reserved;

    // raw data;
    uint8_t data[476];

    // store magic also at the end to limit damage from partial block reads
    uint32_t magicEnd;
} UF2_Block;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "USAGE: %s c|d file.bin [file.uf2]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[2], "rb");
    if (!f)
    {
        fprintf(stderr, "No such file: %s\n", argv[2]);
        return 1;
    }

    fseek(f, 0L, SEEK_END);
    uint32_t sz = ftell(f);
    fseek(f, 0L, SEEK_SET);

    const char *outname = argc > 3 ? argv[3] : "flash.uf2";

    FILE *fout = fopen(outname, "wb");

    UF2_Block bl;
    memset(&bl, 0, sizeof(bl));

    bl.magicStart0 = UF2_MAGIC_START0;
    bl.magicStart1 = UF2_MAGIC_START1;
    bl.flags = UF2_FLAG_FAMILYID;
    bl.magicEnd = UF2_MAGIC_END;
    bl.targetAddr = (!strncmp("c", argv[1], 1)) ? APP_START_ADDRESS : DAT_START_ADDRESS;
    bl.numBlocks = (sz + 255) / 256;
    bl.reserved = BOARD_UF2_FAMILY_ID;
    bl.payloadSize = 256;
    int numbl = 0;
    while (fread(bl.data, 1, bl.payloadSize, f))
    {
        bl.blockNo = numbl++;
        fwrite(&bl, 1, sizeof(bl), fout);
        bl.targetAddr += bl.payloadSize;
        // clear for next iteration, in case we get a short read
        memset(bl.data, 0, sizeof(bl.data));
    }
    fclose(fout);
    fclose(f);
    printf("Target Addr 0x%x \n", (!strncmp("c", argv[1], 1)) ? APP_START_ADDRESS : DAT_START_ADDRESS);
    printf("Wrote %d blocks to %s \n", numbl, outname);
    return 0;
}
