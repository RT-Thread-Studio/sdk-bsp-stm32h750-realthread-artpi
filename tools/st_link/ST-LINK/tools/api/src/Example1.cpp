/** 
* \file Example1.cpp
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each ST-LINK probes and execute the following operations: \n
* 			-	Mass erase.
* 			-	Single word edition (-w32).
* 			-	Read 64 bytes from 0x08000000.
* 			-	Sector erase.
* 			-	Read 64 bytes from 0x08000000 .
*			-	System Reset. \n
*.
*Go to the source code : \ref STLINK_Example1 
* \example STLINK_Example1
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each St-LINK probes and execute the following operations: \n
* 			-	Mass erase.
* 			-	Single word edition (-w32).
* 			-	Read 64 bytes from 0x08000000.
* 			-	Sector erase.
* 			-	Read 64 bytes from 0x08000000 .
*			-	System Reset. \n
* \code{.cpp}
**/

#include <stdio.h> 
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>

int Example1(void) {

    logMessage(Title, "\n+++ Example 1 +++\n\n");

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
			logMessage(GreenInfo, "\n--- Device %d Connected --- \n",index);
		}

		/* Display device informations */
		genInfo = getDeviceGeneralInf();
		logMessage(Normal, "\nDevice name : %s ", genInfo->name);
		logMessage(Normal, "\nDevice type : %s ", genInfo->type);
		logMessage(Normal, "\nDevice CPU  : %s \n", genInfo->cpu);

		/* Apply mass Erase */
		int massEraseFlag = massErase();
		if (massEraseFlag != 0)
		{
			disconnect();
			continue;
		}			

		/* Single word edition */
		int size = 4;
		int startAddress = 0x08000000;
		char data[8] = { (char)0xAA, (char)0xAA, (char)0xBB, (char)0xBB, (char)0xCC, (char)0xCC, (char)0xDD, (char)0xDD };
		int writeMemoryFlag = writeMemory(startAddress, data, size);
		if (writeMemoryFlag != 0)
		{
			disconnect();
			continue;
		}			

		/* Reading 64 bytes from 0x08000000 */
		unsigned char* dataStruct = 0;
		size = 64;

		int readMemoryFlag = readMemory(startAddress, &dataStruct, size);
		if (readMemoryFlag != 0)
		{
			disconnect();
			continue;
		}

		logMessage(Normal, "\nReading 32-bit memory content\n");
		logMessage(Normal, "  Size          : %d Bytes\n", size);
		logMessage(Normal, "  Address:      : 0x%08X\n", startAddress);

		int i = 0, col=0;

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

		/* Sector Erase */
		unsigned int sectors[4] = { 0, 1, 2, 3 };  // we suppose that we have 4 sectors
		int sectorEraseFlag = sectorErase(sectors, 1); // we will erase just the first sector
		if (sectorEraseFlag != 0)
		{
			disconnect();
			continue;
		}

		/* Reading 64 bytes from 0x08000000 */
		i = 0, col = 0;
		size = 64;
		dataStruct = 0;

		readMemoryFlag = readMemory(startAddress, &dataStruct, size);
		if (readMemoryFlag != 0)
		{
			disconnect();
			continue;
		}

		logMessage(Normal, "\nReading 32-bit memory content\n");
		logMessage(Normal, "  Size          : %d Bytes\n", size);
		logMessage(Normal, "  Address:      : 0x%08X\n", startAddress);

		while ((i<size))
		{
			col = 0;
			logMessage(Normal, "\n0x%08X :",  startAddress + i);
			while ((col<4) && (i<size))
			{
				logMessage(Info, " %08X ", *(unsigned int *)(dataStruct + i));
				col++;
				i += 4;
			}
		}
		logMessage(Normal, "\n");

		/* Apply a System Reset */
		int resetFlag = reset(SOFTWARE_RESET);
		if (resetFlag != 0)
		{
			logMessage(Error, "\nUnable to reset MCU!\n");
			disconnect();
			continue;
		}
		else
			logMessage(GreenInfo, "\nSystem Reset is performed\n");

		/* Process successfully Done */
		disconnect();
	}

	deleteInterfaceList();
	return 1;

 }

 /** \endcode **/ 
