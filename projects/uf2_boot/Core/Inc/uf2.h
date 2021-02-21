
#ifndef __UF2_H__
#define __UF2_H__

#include <stdint.h>
#include <stdbool.h>

#define __YEAR_INT__ ((( \
  (__DATE__ [ 7u] - '0')  * 10u + \
  (__DATE__ [ 8u] - '0')) * 10u + \
  (__DATE__ [ 9u] - '0')) * 10u + \
  (__DATE__ [10u] - '0'))

#define __MONTH_INT__ ( \
    (__DATE__ [2u] == 'n' && __DATE__ [1u] == 'a') ?  1u  /*Jan*/ \
  : (__DATE__ [2u] == 'b'                        ) ?  2u  /*Feb*/ \
  : (__DATE__ [2u] == 'r' && __DATE__ [1u] == 'a') ?  3u  /*Mar*/ \
  : (__DATE__ [2u] == 'r'                        ) ?  4u  /*Apr*/ \
  : (__DATE__ [2u] == 'y'                        ) ?  5u  /*May*/ \
  : (__DATE__ [2u] == 'n'                        ) ?  6u  /*Jun*/ \
  : (__DATE__ [2u] == 'l'                        ) ?  7u  /*Jul*/ \
  : (__DATE__ [2u] == 'g'                        ) ?  8u  /*Aug*/ \
  : (__DATE__ [2u] == 'p'                        ) ?  9u  /*Sep*/ \
  : (__DATE__ [2u] == 't'                        ) ? 10u  /*Oct*/ \
  : (__DATE__ [2u] == 'v'                        ) ? 11u  /*Nov*/ \
  :                                                  12u  /*Dec*/ )

#define __DAY_INT__ ( \
   (__DATE__ [4u] == ' ' ? 0 : __DATE__ [4u] - '0') * 10u \
 + (__DATE__ [5u] - '0')                                   )

// __TIME__ expands to an eight-character string constant
// "23:59:01", or (if cannot determine time) "??:??:??" 
#define __HOUR_INT__ ( \
   (__TIME__ [0u] == '?' ? 0 : __TIME__ [0u] - '0') * 10u \
 + (__TIME__ [1u] == '?' ? 0 : __TIME__ [1u] - '0')       )

#define __MINUTE_INT__ ( \
   (__TIME__ [3u] == '?' ? 0 : __TIME__ [3u] - '0') * 10u \
 + (__TIME__ [4u] == '?' ? 0 : __TIME__ [4u] - '0')       )

#define __SECONDS_INT__ ( \
   (__TIME__ [6u] == '?' ? 0 : __TIME__ [6u] - '0') * 10u \
 + (__TIME__ [7u] == '?' ? 0 : __TIME__ [7u] - '0')       )


#define __DOSDATE__ ( \
	((__YEAR_INT__  - 1980u) << 9u) | \
	( __MONTH_INT__          << 5u) | \
	( __DAY_INT__            << 0u) )

#define __DOSTIME__ ( \
	( __HOUR_INT__    << 11u) | \
	( __MINUTE_INT__  <<  5u) | \
	( __SECONDS_INT__ <<  0u) )


#define BOARD_UF2_FAMILY_ID 0x6db66082 // 0x5ee21072UL
#define BOARD_FLASH_APP_START 0x90000000UL

//--------------------------------------------------------------------+
// USB UF2
//--------------------------------------------------------------------+

#define USB_MANUFACTURER  "STM32"
#define USB_PRODUCT       "STM32H750XBT6"

#define UF2_PRODUCT_NAME  USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID      "ART-Pi"
#define UF2_VOLUME_LABEL  "ART-Pi"
#define UF2_INDEX_URL     "https://art-pi.gitee.io/website/"

//--------------------------------------------------------------------+
// UF2 Configuration
//--------------------------------------------------------------------+

// Version is passed by makefile
#define UF2_VERSION         "001"

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+

// All entries are little endian.
#define UF2_MAGIC_START0    0x0A324655UL // "UF2\n"
#define UF2_MAGIC_START1    0x9E5D5157UL // Randomly selected
#define UF2_MAGIC_END       0x0AB16F30UL // Ditto

// If set, the block is "comment" and should not be flashed to the device
#define UF2_FLAG_NOFLASH    0x00000001
#define UF2_FLAG_FAMILYID   0x00002000

typedef struct {
    // 32 byte header
    uint32_t magicStart0;
    uint32_t magicStart1;
    uint32_t flags;
    uint32_t targetAddr;
    uint32_t payloadSize;
    uint32_t blockNo;
    uint32_t numBlocks;
    uint32_t familyID;

    // raw data;
    uint8_t data[476];

    // store magic also at the end to limit damage from partial block reads
    uint32_t magicEnd;
} UF2_Block;


void uf2_init(void);
void uf2_read_block(uint32_t block_no, uint8_t *data);
int uf2_write_block(uint32_t block_no, uint8_t *data);

#endif
