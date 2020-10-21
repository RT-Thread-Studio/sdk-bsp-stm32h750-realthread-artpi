/** 
* \file Example3.cpp
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each ST-LINK probes and execute the following operations: \n
*		-	File programming + verification step.
*		-	Set RDP level.
*		-	Display option bytes. 
*		-	Disable readout protection.
*		-	Display option bytes.
*		-	System Reset. \n
*.
* Go to the source code : \ref STLINK_Example3 
* \example STLINK_Example3
* This example allows to execute some operations to the internal STM32 Flash memory. \n
* 1.	Detect connected ST-LINK and display them. \n
* 2.	Loop on each St-LINK probes and execute the following operations: \n
*		-	File programming + verification step.
*		-	Set RDP level.
*		-	Display option bytes. 
*		-	Disable readout protection.
*		-	Display option bytes.
*		-	System Reset. \n
*  \code{.cpp}
**/


#include <Example3.h>
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>

int Example3(void) {

	logMessage(Title, "\n+++ Example 3 +++\n");

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
		int connectStlinkFlag = connectStLink(debugParameters); //try to connect
		if (connectStlinkFlag != 0) {
			logMessage(Error, "Establishing connection with the device failed\n");
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
		logMessage(Normal, "\n");

		/* Download file + verification */
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

		/* Set rdp option byte */
		int sendOptionBytesCmdFlag = sendOptionBytesCmd((char*)"-ob rdp=0xbb");
		if (sendOptionBytesCmdFlag != 0)
		{
			disconnect();
			continue;
		}

		/* Display option bytes */
		peripheral_C* ob;
		ob = initOptionBytesInterface();
		if (ob == 0)
		{
			disconnect();
			continue;
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

		/* Disable readout protection */
		int readUnprotectFlag = readUnprotect();
		if (readUnprotectFlag != 0)
		{
			disconnect();
			continue;
		}

		/* Display option bytes */
		ob = initOptionBytesInterface();
		if (ob == 0)
		{
			disconnect();
			continue;
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
