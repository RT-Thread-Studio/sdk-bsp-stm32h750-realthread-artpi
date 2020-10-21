/** 
* \file main.cpp
* The examples provided by the documentation are written for documentation to take inspiration for use cases.
* This code is the main program that allows to choose and execute the preferred example and to execute some operations to the internal STM32 Flash memory. \n
*	-	Set the Loader path.
*	-	Associate display manager functions.
*	-	Call examples functions.
*	-	Close all connected ST-LINK probes. 
*.
* Go to the source code : \ref main 
\example main
* The examples provided by the documentation are written for documentation to take inspiration for use cases.
* This code is the main program that allows to choose and execute the preferred example and to execute some operations to the internal STM32 Flash memory. \n
*	-	Set the Loader path.
*	-	Associate display manager functions.
*	-	Call examples functions.
*	-	Close all connected ST-LINK probes.
*  \code{.cpp}
**/


#include <iostream>
#include <DisplayManager.h>
#include <CubeProgrammer_API.h>
#include <Example1.h>
#include <Example2.h>
#include <Example3.h>
#include <I2C_Example.h>
#include <CAN_Example.h>
#include <SPI_Example.h>
#include <UART_Example.h>
#include <USB_Example.h>

extern unsigned int verbosityLevel;

int main()
{
	int ret = 0;
    const char* loaderPath = "./.";
    displayCallBacks vsLogMsg;

    /* Set device loaders path that contains FlashLoader and ExternalLoader folders*/
	setLoadersPath(loaderPath);

    /* Set the progress bar and message display functions callbacks */
	vsLogMsg.logMessage = DisplayMessage;
	vsLogMsg.initProgressBar = InitPBar;
	vsLogMsg.loadBar = lBar;
    setDisplayCallbacks(vsLogMsg);

    /* Set DLL verbosity level */
    setVerbosityLevel(verbosityLevel = VERBOSITY_LEVEL_1);

    ret = Example1();
    //ret = Example2();
    //ret = Example3();
    //ret = I2C_Example();
    //ret = CAN_Example();
    //ret = SPI_Example();
    //ret = UART_Example();
    //ret = USB_Example();

    std::cout << "\n" << "Press enter to continue...";
	std::cin.get() ;
	return ret;
}

/** \endcode **/
