#include "stm32f10x.h"
#include "stm3210e_eval_fsmc_nor.h"

#define StartAddresse 0x64000000

/**
  * Description :
  * Initilize the MCU Clock, the GPIO Pins corresponding to the
  * device and initilize the FSMC with the chosen configuration 
  * Inputs    :
  *      None
  * outputs   :
  *      R0             : "1" 			: Operation succeeded
  * 			  "0" 			: Operation failure
  * Note: Mandatory for all types of device 
  */
int Init (void)
{  
  /* Set MCU Clock */
  SystemInit();
	
  /* Enable the FSMC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
  /* Init the External SRAM */
  NOR_Init();
	
  return 1;
}

/**
  * Description :
  * Read data from the device 
  * Inputs    :
  *      Address       : Write location
  *      Size          : Length in bytes  
  *      buffer       : Address where to get the data to write
  * outputs   :
  *      R0             : "1" 			: Operation succeeded
  * 			  "0" 			: Operation failure
  * Note: Mandatory for all types except SRAM and PSRAM	
  */
int Read (uint32_t Address, uint32_t Size, uint16_t* Buffer)
{    
  uint32_t InternalAddr = Address - StartAddresse;
  uint32_t ReadedData = 0;
  uint32_t Counter = 0;   
  uint16_t TmpBuffer = 0x00000000;
     
  if (InternalAddr%2 != 0)
  {
    NOR_ReadBuffer(&TmpBuffer, (InternalAddr - InternalAddr%2), 1);
       
    for (Counter =0; (Counter<(InternalAddr%2))&&(Counter<Size); Counter++)
      *((uint8_t*)Buffer+Counter) = *((uint8_t*)(&TmpBuffer)+InternalAddr%4+Counter);
       
    ReadedData += Counter;
  }
      
  if (Size-ReadedData >= 2)
  {
    NOR_ReadBuffer((uint16_t*)((uint8_t*)Buffer+ReadedData), InternalAddr+ReadedData ,(Size-ReadedData)/2);
    ReadedData += (((Size-ReadedData)/2)*2);
  }
      
  if (ReadedData < Size)
  {  
    NOR_ReadBuffer(&TmpBuffer, InternalAddr+ReadedData ,1);
        
    for (Counter =0; Counter<(Size-ReadedData); Counter++)
      *((uint8_t*)Buffer+ReadedData+Counter) = *((uint8_t*)(&TmpBuffer)+Counter);
  }

  return 1;
} 

/**
  * Description :
  * Write data from the device 
  * Inputs    :
  *      Address       : Write location
  *      Size          : Length in bytes  
  *      buffer        : Address where to get the data to write
  * outputs   :
  *      R0             : "1" 			: Operation succeeded
  * 			  "0" 			: Operation failure
  * Note: Mandatory for all types except SRAM and PSRAM	
  */
int Write (uint32_t Address, uint32_t Size, uint16_t* Buffer)
{  
  uint32_t InternalAddr = Address - StartAddresse;
  uint32_t WritedData = 0;
  uint32_t Counter = 0; 
  uint16_t TmpBuffer = 0x00000000;
        
  if (InternalAddr%2 != 0)
  {
    NOR_ReadBuffer (&TmpBuffer, (InternalAddr - InternalAddr%2),1);
         
    for (Counter =0; (Counter<(2-InternalAddr%2))&&(Counter<Size); Counter++)
      *((uint8_t*)(&TmpBuffer)+InternalAddr%2+Counter) = * ((uint8_t*)Buffer+Counter);
         
    if (NOR_WriteBuffer (&TmpBuffer, (InternalAddr - InternalAddr%2),1) != 0)
      return 0;
         
    WritedData += Counter;
  }
        
  if (Size-WritedData >= 2)
  {
    if (NOR_WriteBuffer ((uint16_t*)((uint8_t*)Buffer+WritedData), InternalAddr+WritedData, ((Size-WritedData)/2))!=0)
      return 0;

    WritedData += (((Size-WritedData)/2)*2);
  }
        
  if (WritedData < Size)
  {
    NOR_ReadBuffer (&TmpBuffer, InternalAddr+WritedData,1);

    for (Counter =0; Counter<(Size-WritedData); Counter++)
      *((uint8_t*)(&TmpBuffer)+Counter) = *((uint8_t*)Buffer+WritedData+Counter);
         
    if (NOR_WriteBuffer (&TmpBuffer, InternalAddr+WritedData,1)!=0)
      return 0;
  }

  return 1;
} 

/**
  * Description :
  * Erase a full sector in the device
  * Inputs    :
  *     None
  * outputs   :
  *     R0             : "1" : Operation succeeded
  * 			 "0" : Operation failure
  * Note: Not Mandatory for SRAM PSRAM and NOR_FLASH
  */
int MassErase (void)
{  
  if (NOR_EraseChip()==0)
    return 1;
  else
    return 0;
}

/**
  * Description :
  * Erase a full sector in the device
  * Inputs    :
  *      SectrorAddress	: Start of sector
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : "1" : Operation succeeded
  * 			 "0" : Operation failure
  * Note: Not Mandatory for SRAM PSRAM and NOR_FLASH
  */
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{      
  uint32_t BlockAddr;
  EraseStartAddress = EraseStartAddress -  EraseStartAddress%0x20000;
  while (EraseEndAddress>=EraseStartAddress)
  {
    BlockAddr = EraseStartAddress - StartAddresse;
    if (NOR_EraseBlock(BlockAddr)!=0)
      return 0;
    EraseStartAddress+=0x20000;
  }
  return 1;	
}


/**
  * Description :
  * Calculates checksum value of the memory zone
  * Inputs    :
  *      StartAddress  : Flash start address
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
  uint32_t Val;
  uint16_t value;
	
  StartAddress-=StartAddress%4;
  Size += (Size%4==0)?0:4-(Size%4);
  
  for(cnt=0; cnt<Size ; cnt+=4)
  {
    NOR_ReadBuffer(&value, StartAddress - StartAddresse,1);
    Val = value;
    NOR_ReadBuffer(&value, StartAddress - StartAddresse + 2,1);
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
    NOR_ReadBuffer(&TmpBuffer, (InternalAddr - InternalAddr%2),1);
        
    if ((uint8_t)(TmpBuffer>>8) != (*(uint8_t*)RAMBufferAddr))
      return ((checksum<<32) + MemoryAddr);
    VerifiedData++;
  }
      
  while ((Size-VerifiedData)>1)
  {
    NOR_ReadBuffer(&TmpBuffer, InternalAddr+VerifiedData,1);
      
    if ((TmpBuffer&0x00FF) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
        
    VerifiedData++;
        
    if ((uint8_t)(TmpBuffer>>8) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
        
    VerifiedData++;
  }
    
  if ((Size-VerifiedData) != 0)
  {
    NOR_ReadBuffer(&TmpBuffer, InternalAddr+VerifiedData,1);
        
    if ((uint8_t)(TmpBuffer&0x00FF) != (*((uint8_t*)RAMBufferAddr+VerifiedData)))
      return ((checksum<<32) + MemoryAddr+VerifiedData);
  }

  return (checksum<<32);
}

