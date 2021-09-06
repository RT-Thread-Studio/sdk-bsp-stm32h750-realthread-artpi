
#ifndef __FLASH_H__
#define __FLASH_H__

extern void flash_init(void);
extern void flash_read(uint32_t ReadAddr, uint8_t *pData, uint32_t Size);
extern void flash_write(uint32_t WriteAddr, uint8_t *pData, uint32_t Size);

#endif
