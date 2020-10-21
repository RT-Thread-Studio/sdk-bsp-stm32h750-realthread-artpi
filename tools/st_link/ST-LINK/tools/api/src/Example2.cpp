/** 
* \file Example2.cpp
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each ST-LINK probes and execute the following operations: \n
* 			-	File programming + verification step.
* 			-	Read 256 bytes from the address 0x08000000.
* 			-	Run application. \n
*.
* Go to the source code : \ref STLINK_Example2 
* \example STLINK_Example2
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each St-LINK probes and execute the following operations: \n
* 			-	File programming + verification step.
* 			-	Read 256 bytes from the address 0x08000000.
* 			-	Run application. \n
*  \code{.cpp}
**/


#include <Example2.h>
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>

int Example2(void) {

	logMessage(Title, "\n+++ Example 2 +++\n");

	debugConnectParameters *stLinkList;
	debugConnectParameters debugParameters;
	generalInf* genInfo;

	int getStlinkListNb = getStLinkList(&stLinkList, 0);

	if (getStlinkListNb == 0)
	{
		logMessage(Error, "No STLINK available\n");
		return 0;
	}
	else {
		logMessage(Title, "\n-------- Connected ST-LINK Probes List --------");
		for (int i = 0; i < getStlinkListNb; i++)
		{
			logMessage(Normal, "\nST-LINK Probe %d :\n", i);
			logMessage(Info, "   ST-LINK SN   : %s \n", stLinkList[i].serialNumber);
			logMessage(Info, "   ST-LINK FW   : %s \n", stLinkList[i].firmwareVersion);
		}
		logMessage(Title, "-----------------------------------------------\n\n");
	}

	for (int index = 0; index < getStlinkListNb; index++) {

		logMessage(Title, "\n--------------------- ");
		logMessage(Title, "\n ST-LINK Probe : %d ", index);
		logMessage(Title, "\n--------------------- \n\n");

		debugParameters = stLinkList[index];
		debugParameters.connectionMode = UNDER_RESET_MODE;
		debugParameters.shared = 0;

		/* Target connect */
		int connectStlinkFlag = connectStLink(debugParameters);
		if (connectStlinkFlag != 0) {
			logMessage(Error, "Establishing connection with the device failed\n");
			disconnect();
			continue;
		}
		else {
			logMessage(GreenInfo, "\n--- Device %d Connected --- \n", index);
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
			continue;
		}

		/* Reading 256 bytes from 0x08000000 */
		unsigned char* dataStruct = 0;
		int size = 256;
		int startAddress = 0x08000000;

		int readMemoryFlag = readMemory(startAddress, &dataStruct, size);
		if (readMemoryFlag != 0)
		{
			disconnect();
			continue;
		}

		logMessage(Normal, "\nReading 32-bit memory content\n");
		logMessage(Normal, "  Size          : %d Bytes\n", size);
		logMessage(Normal, "  Address:      : 0x%08X\n", startAddress);

		int i = 0, col = 0;

		while ((i<size))
		{
			col = 0;
			logMessage(Normal, "\n0x%08X :", startAddress+i);
			while ((col<4) && (i<size))
			{
				logMessage(Info, " %08X ", *(unsigned int *)(dataStruct + i));
				col++;
				i += 4;
			}

		}
		logMessage(Normal, "\n");

		/* Run application */
		int executeFlag = execute(0x08000000);
		if (executeFlag != 0)
		{
			disconnect();
			continue;
		}
		/* The system will lose the connection with bootloader when it is in running mode */


		/* Process successfully Done */
		disconnect();
	}

	deleteInterfaceList();
	return 1;
}

/** \endcode **/
