/** 
* \file I2C_Example.cpp
* This example allows to connect to the target through I2C Bootloader interface and execute some operations to the internal STM32 Flash memory. \n
*		- Connect to the device via I2C interface by indicating the slave address.
*		- File programming + verification step.
*		- Read 64 bytes from 0x08000000.
*		- Option bytes programming : BOR level.
*		- Display option bytes.
*.
* Go to the source code : \ref I2C_Bootloader
* \example I2C_Bootloader
* This example allows to connect to the target through I2C Bootloader interface and execute some operations to the internal STM32 Flash memory. \n
*		- Connect to the device via I2C interface by indicating the slave address.
*		- File programming + verification step.
*		- Read 64 bytes from 0x08000000.
*		- Option bytes programming : BOR level.
*		- Display option bytes.
*  \code{.cpp}
**/

#include <I2C_Example.h>
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>

#define STM32L45xxx 0x4A    // I2C Bootloader address for STM32L45xxx device
#define STM32F42xxx 0x38
#define STM32F72xxx 0x49
#define STM32H74xxx 0x4E	//...


int I2C_Example(void) {

	logMessage(Title, "\n+++ I2C Bootloader Example +++\n\n");

	generalInf* genInfo;
	i2cConnectParameters i2cParam;

	i2cParam.add	= STM32L45xxx ;	//Bootloader address for STM32L452RE, an address must be insered here, otherwise the connection can not be established.
	i2cParam.br		= 400;
	i2cParam.sm		= 1;
	i2cParam.am		= 0;
	i2cParam.af		= 1;
	i2cParam.df		= 0;
	i2cParam.dnf	= 0;
	i2cParam.rt		= 0;
	i2cParam.ft		= 0;

	/* Target connect */
	int i2cConnectFlag = connectI2cBootloader(i2cParam);
	if (i2cConnectFlag != 0) {
		logMessage(Error, "Establishing connection with the device failed");
		disconnect();
		return 0;
	}
	else {
		logMessage(GreenInfo, "\n--- Device Connected --- \n");
	}

	/* Display device informations */
	genInfo = getDeviceGeneralInf();
	logMessage(Normal, "\nDevice name : %s ", genInfo->name);
	logMessage(Normal, "\nDevice type : %s ", genInfo->type);
	logMessage(Normal, "\nDevice CPU  : %s \n", genInfo->cpu);

	/* Download File + verification */
#ifdef _WIN32
	const wchar_t* filePath = L"../test file/data.hex";
#else
	const wchar_t* filePath = L"../api/test file/data.hex";
#endif

	unsigned int isVerify = 1; //add verification step
	unsigned int isSkipErase = 0; // no skip erase
	int downloadFileFlag = downloadFile(filePath, 0x08000000, isSkipErase, isVerify, L"");
	if (downloadFileFlag != 0)
	{
		disconnect();
		return 0;
	}


	/* Reading 64 bytes from 0x08000000 */
	int size = 64;
	int startAddress = 0x08000000;
	unsigned char* dataStruct = 0;

	int readMemoryFlag = readMemory(startAddress, &dataStruct, size);
	if (readMemoryFlag != 0)
	{
		disconnect();
		return 0;
	}
	logMessage(Normal, "\nReading 32-bit memory content\n");
	logMessage(Normal, "  Size          : %d Bytes\n", size);
	logMessage(Normal, "  Address:      : 0x%08X\n", startAddress);

	int i = 0, col = 0;

	while ((i<size))
	{
		col = 0;
		logMessage(Normal, "\n0x%08X :", startAddress + i);
		while ((col<4) && (i<size))
		{
			logMessage(Info, " %08X ", *(unsigned int *)(dataStruct + i));
			col++;
			i += 4;
		}
	}
	logMessage(Normal, "\n");

	/* Option bytes programming : BOR level  */
	int sendOptionBytesCmdFlag = sendOptionBytesCmd((char*)"-ob BOR_LEV=1");
	if (sendOptionBytesCmdFlag != 0)
	{
		disconnect();
		return 0;
	}
	
	/* Display option bytes */
	peripheral_C* ob = initOptionBytesInterface();
	if (ob == 0)
	{
		disconnect();
		return 0;
	}

	for (unsigned int i = 0; i < ob->banksNbr; i++)
	{
		logMessage(Normal, "OPTION BYTES BANK: %d\n", i);
		for (unsigned int j = 0; j < ob->banks[i]->categoriesNbr; j++)
		{
			logMessage(Title, "\t%s\n", ob->banks[i]->categories[j]);
			for (unsigned int k = 0; k < ob->banks[i]->categories[j]->bitsNbr; k++)
			{
				if (ob->banks[i]->categories[j]->bits[k]->access == 0 || ob->banks[i]->categories[j]->bits[k]->access == 2) {
					logMessage(Normal, "\t\t%s", ob->banks[i]->categories[j]->bits[k]->name);
					logMessage(Info, ": 0x%x\n", ob->banks[i]->categories[j]->bits[k]->bitValue);
				}
			}
		}
	}

	/* Process successfully Done */
	disconnect();
	return 1;
}

/** \endcode **/
