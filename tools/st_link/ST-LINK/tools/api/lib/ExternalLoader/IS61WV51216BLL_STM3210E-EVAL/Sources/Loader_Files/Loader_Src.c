#include "stm32f10x.h"
#include "stm3210e_eval_fsmc_sram.h"

#define StartAddresse 0x68000000


/**
  * Description :
  * Initilize the MCU Clock, the GPIO Pins corresponding to the
  * device and initilize the FSMC with the chosen configuration
  * Inputs    :
  *      None
  * outputs   :
  *      R0       : "1" 	: Operation succeeded	
  * 		    "0" 	: Operation failure
  * Info :
  * Align and memory size (32/16/8 bits) is handled in this function 
  * Note : Mandatory for all types except SRAM and PSRAM	
  */
int Init (void)
{  
  /* Set MCU Clock */
  SystemInit();
	
  /* Enable the FSMC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
  /* Init the External SRAM */
  SRAM_Init();
  
  return 1;
}

/**
  * Description :
  * Read data to the device
  * Inputs    :
  *      Address  : Write location
  *      Size     : Length in bytes  
  *      buffer   : Address where to get the data to write
  * outputs   :
  *      R0       : "1" 	: Operation succeeded	
  * 		    "0" 	: Operation failure
  * Info :
  * Align and memory size (32/16/8 bits) is handled in this function 
  * Note : Mandatory for all types except SRAM and PSRAM	
  */
int Read (uint32_t Address, uint32_t Size, uint32_t Buffer)
{ 
  uint32_t InternalAddr = Address - StartAddresse;
  uint32_t ReadData = 0;    
  uint16_t TmpBuffer = 0x0000;
        
  if (InternalAddr%2 != 0)
  {
    SRAM_ReadBuffer(&TmpBuffer, (InternalAddr - InternalAddr%2),1);
    *((uint8_t*)Buffer) = (uint8_t)(TmpBuffer>>8);
    ReadData++;
  }
        
  if (Size-ReadData >= 2)
  {
    SRAM_ReadBuffer((uint16_t*)((uint8_t*)Buffer+ReadData), InternalAddr+ReadData ,(Size-ReadData)/2);
    ReadData += (((Size-ReadData)/2)*2);
  }
        
  if (ReadData < Size)
  {  
    SRAM_ReadBuffer(&TmpBuffer, InternalAddr+ReadData ,1);
    *((uint8_t*)Buffer+ReadData) = (uint8_t)(TmpBuffer&0x00FF);
  }

  return 1;
} 

	
/**
  * Description :
  * Write data to the device
  * Inputs    :
  *      Address  : Write location
  *      Size     : Length in bytes  
  *      buffer   : Address where to get the data to write
  * outputs   :
  *      R0       : "1" 	: Operation succeeded	
  * 		    "0" 	: Operation failure
  * Info :
  * Align and memory size (32/16/8 bits) is handled in this function 
  * Note : Mandatory for all types except SRAM and PSRAM	
  */
int Write (uint32_t Address, uint32_t Size, uint32_t Buffer)
{
  uint32_t InternalAddr = Address - StartAddresse;
  uint32_t WritenData = 0;
  uint16_t TmpBuffer = 0x0000;
       
  if (InternalAddr%2 != 0)
  {
    SRAM_ReadBuffer (&TmpBuffer, (InternalAddr - InternalAddr%2),1);
    *((uint8_t*)(&TmpBuffer)+1) = *(uint8_t*)(Buffer);
    SRAM_WriteBuffer (&TmpBuffer, (InternalAddr - InternalAddr%2),1);
    WritenData++;
  }
        
  if (Size-WritenData >= 2)
  {
    SRAM_WriteBuffer ((uint16_t*)((uint8_t*)Buffer+WritenData), InternalAddr+WritenData, ((Size-WritenData)/2));
    WritenData += (((Size-WritenData)/2)*2);
  }
        
  if (WritenData < Size)
  {
    SRAM_ReadBuffer (&TmpBuffer, InternalAddr+WritenData,1);
    *((uint8_t*)(&TmpBuffer)) = *(uint8_t*)((uint8_t*)Buffer+WritenData);
    SRAM_WriteBuffer (&TmpBuffer, InternalAddr+WritenData,1);
  }

  return 1;
} 


/**
  * Description :
  * Calculates checksum value of the memory zone
  * Inputs    :
  *      StartAddress  : memory start address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Checksum value
  * Note: Optional for all types of device
  */
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
  uint8_t missalignementAddress = StartAddress%4;
  uint8_t missalignementSize = Size ;
	int cnt;
	
  StartAddress-=StartAddress%4;
  Size += (Size%4==0)?0:4-(Size%4);
  
  for(cnt=0; cnt<Size ; cnt+=4)
  {
    uint32_t Val;
    uint16_t value;
    SRAM_ReadBuffer(&value, StartAddress - StartAddresse,1);
    Val = value;
    SRAM_ReadBuffer(&value, StartAddress - StartAddresse + 2,1);
    Val+= value<<16;
    if(missalignementAddress)
    {
      switch (missalignementAddress)
      {
        case 1:
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=1;
          break;
        case 2:
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=2;
          break;
        case 3:   
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=3;
          break;
      }  
    }
    else if((Size-missalignementSize)%4 && (Size-cnt) <=4)
    {
      switch (Size-missalignementSize)
      {
        case 1:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          missalignementSize-=1;
          break;
        case 2:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          missalignementSize-=2;
          break;
        case 3:   
          InitVal += (uint8_t) Val;
          missalignementSize-=3;
          break;
      } 
    }
    else
    {
      InitVal += (uint8_t) Val;
      InitVal += (uint8_t) (Val>>8 & 0xff);
      InitVal += (uint8_t) (Val>>16 & 0xff);
      InitVal += (uint8_t) (Val>>24 & 0xff);
    }
    StartAddress+=4;
  }
  
  return (InitVal);
}


/**
  * Description :
  * Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * Inputs    :
  *      FlashAddr     : Flash address
  *      RAMBufferAddr : RAM buffer address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Operation failed (address of failure)
  *     R1             : Checksum value
  * Note: Optional for all types of device
  */
uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
  uint32_t InitVal = 0;
  uint32_t InternalAddr = MemoryAddr - StartAddresse;
  uint32_t VerifiedData = 0;
  uint16_t TmpBuffer = 0x0000;
	uint64_t checksum;
  Size*=4;
  
  checksum = CheckSum((uint32_t)MemoryAddr + (missalignement & 0xf), Size - ((missalignement >> 16) & 0xF), InitVal);  
  
  if (InternalAddr%2 != 0)
  {
    SRAM_ReadBuffer(&TmpBuffer, (InternalAddr - InternalAddr%2),1);
      
    if ((uint8_t)(TmpBuffer>>8) != (*(uint8_t*)RAMBufferAddr))
      return ((checksum<<32) + MemoryAddr);
    VerifiedData++;    
  }
     
  while ((Size-VerifiedData)>1)
  {
    SRAM_ReadBuffer(&TmpBuffer, InternalAddr+VerifiedData,1);
      
    if ((TmpBuffer&0x00FF) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
       
    VerifiedData++;
        
    if ((uint8_t)(TmpBuffer>>8) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
       
    VerifiedData++;
  }
      
  if ((Size-VerifiedData) != 0)
  {
    SRAM_ReadBuffer(&TmpBuffer, InternalAddr+VerifiedData,1);
       
    if ((uint8_t)(TmpBuffer&0x00FF) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
  }

  return (checksum<<32);
}

