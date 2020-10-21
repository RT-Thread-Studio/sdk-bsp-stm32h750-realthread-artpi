#include "Dev_Inf.h"

/* This structure containes information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo  =  {
#endif
   "M25P64_STM3210E-EVAL", 	 	 // Device Name + version number
   SPI_FLASH,                  					 // Device Type
   0x00000000,                						 // Device Start Address
   0x00800000,                 						 // Device Size in Bytes (8MBytes/64Mbits)
   0x00000100,                 						 // Programming Page Size 16Bytes
   0xFF,                       						 // Initial Content of Erased Memory
// Specify Size and Address of Sectors (view example below)
   0x00000080, 0x00010000,     				 // Sector Num : 128 ,Sector Size: 64KBytes 
   0x00000000, 0x00000000,
}; 

/*  								Sector coding example
	A device with succives 16 Sectors of 1KBytes, 128 Sectors of 16 KBytes, 
	8 Sectors of 2KBytes and 16384 Sectors of 8KBytes
	
	0x00000010, 0x00000400,     							// 16 Sectors of 1KBytes
	0x00000080, 0x00004000,     							// 128 Sectors of 16 KBytes
	0x00000008, 0x00000800,     							// 8 Sectors of 2KBytes
	0x00004000, 0x00002000,     							// 16384 Sectors of 8KBytes
	0x00000000, 0x00000000,								// end
  */
