/** 
* \file USB_Example.cpp
* This example allows to connect to the target through USB Bootloader interface and execute some operations to the internal STM32 Flash memory. \n
*		- Connect to the device via USB interface.
*		- File programming + verification step.
*		- Read 64 bytes from 0x08000000.
*		- Option bytes programming : BOR level.
*		- Display option bytes.
*.
* Go to the source code : \ref USB_Bootloader
\example USB_Bootloader
* This example allows to connect to the target through USB Bootloader interface and execute some operations to the internal STM32 Flash memory. \n
*		- Connect to the device via USB interface.
*		- File programming + verification step.
*		- Read 64 bytes from 0x08000000.
*		- Option bytes programming : BOR level.
*		- Display option bytes.
*  \code{.cpp}
**/


#include <USB_Example.h>
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>

int USB_Example(void) {

	logMessage(Title, "\n+++ USB Bootloader Example +++\n\n");

	generalInf* genInfo;
	dfuDeviceInfo *dfuList;

	int getDfuListNb = getDfuDeviceList(&dfuList);

	if (getDfuListNb == 0)
	{
		logMessage(Error, "No USB DFU available\n");
		return 0;
	}
	else {
		logMessage(Title, "\n------------- USB DFU List --------------\n");
		for (int i = 0; i < getDfuListNb; i++)
		{
			logMessage(Normal, "USB Port %d \n",i);
			logMessage(Info, "	USB index   : %s \n", dfuList[i].usbIndex);
			logMessage(Info, "	USB SN      : %s \n", dfuList[i].serialNumber);
			logMessage(Info, "	DFU version : 0x%02X ", dfuList[i].dfuVersion);
		}
		logMessage(Title, "\n-----------------------------------------\n\n");
	}

	/* Target connect, choose the adequate USB port by indicating its index that is already mentioned in USB DFU List above */
	int usbConnectFlag = connectDfuBootloader(dfuList[0].usbIndex); 
	if (usbConnectFlag != 0) 
	{
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

	/* Option bytes programming : BOR level */
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
	deleteInterfaceList();
	return 1;
}

/** \endcode **/
