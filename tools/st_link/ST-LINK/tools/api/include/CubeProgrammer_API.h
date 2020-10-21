/* doxygen documentation */
/*!
 * \file CubeProgrammer_API.h
 * \mainpage STM32CubeProgrammer API Documentation
 * \section intro1 Introduction
 * In addition to the graphical user interface and the commandline interface, STM32CubeProgrammer offers a C++ API
 * that you could use to develop your own application and benefit of  wide range
 * of features to program  STM32 microcontrollers memories (such as Flash, RAM, and OTP)
 * either over debug interface or bootloder interface (USB DFU, UART, I²C, SPI and CAN).
 *
 * This documentation details all types and functions API to use STM32CubeProgrammer functionalities. \n
 *
 * \section include STM32CubeProgrammer API package
 * The API package is based on the following resources that are ready for use : \n
 * <table>
 * <tr><th>File                                 <th>Description
 * <tr><td>CubeProgrammer_API.h                 <td>header file contains functions declarations and macro definitions to be used by the CubeProgrammer_API.dll.
 * <tr><td>DeviceDataStructure.h                <td>header file contains macro definitions to be shared between several functions.
 * <tr><td>CubeProgrammer_API.dll/.so/.dylib    <td>Dynamic Link Library file contains a library of functions and other informations that can be accessed by windows, linux and MacOs programs.
 * <tr><td>CubeProgrammer_API.lib               <td>import library for use with MSVC compiler.
 * </table>
 *
 * \section app Application System requirements
 * We offer various examples developed for use with Visual Studio and Qt Creator IDEs:
 *  -   Visual Studio project for windows applications under MSVC compiler.
 *  -   Qt project for Windows, Linux and MacOs applications under MinGW compiler.
 * \note The attached examples are tested on STM32F429, STM32L4R9 and STM32L452RE Nucleo Boards.
 *
 * \subsection windows Windows build settings
 * To ensure the correct environment for the provided examples, some constraints must be taken into account\n
 *
 * \subsubsection vs Visual studio
 * The code was developed with Microsoft Visual Studio Community 2017 version 15.6.6 with Windows SDK version : 10.0.16299.0
 * and it was tested also on Microsoft Visual Studio  2010 \n
 *  -   Verify that the api's lib directory is the default working directory.
 *  -   The debug command is to execute the application that is resided in the output directory.
 *  -   Insert the Additional Include Directories:\n
 * <HR>
 * \image html link1.jpg \n
 * <HR>
 *  -   Indicate the output name and path. \n
 *  -   Insert the Additional Library Directories. \n
 *  -   Insert the Additional Dependencies, The CubeProgrammer_API import library "CubeProgrammer_API.lib" must be added to Linker input\n
 * <HR>
 * \image html link2.jpg \n
 * <HR>
 * \note The provided Visual studio project is already configured like the description above and you can change it carefully according to your use case.
 * \subsubsection qt Qt creator
 * The Qt projects are developed with Qt Creator 4.7.1, Qt 5.11.2 and MinGW 32 bit compiler and also it can be compiled by MinGW x64.\n
 * <b> The shadow build option must be disable to inform Qt that the build should be located in the original Qt project directory. </b>
 * \note Verify that the following DLLs are placed to /lib in the api package directory :
 * <table>
 * <tr><td>CubeProgrammer_API.dll   <td>FileManager.dll     <td>STLinkUSBDriver.dll
 * <tr><td>HSM_P11_Lib.dll          <td>libeay32.dll        <td>libgcc_s_dw2-1.dll
 * <tr><td>stlibp11_SAM.dll         <td>libstdc++-6.dll     <td>libwinpthread-1.dll
 * <tr><td>mfc120.dll               <td>msvcp120.dll        <td>msvcr120.dll
 * <tr><td>Qt5Core.dll              <td>Qt5SerialPort.dll   <td>Qt5Xml.dll
 * </table>
 *
 * \subsection qtlinux Linux build settings
 * To correctly build the Qt project in linux platform, you should follow the next 3 steps:
 *      1. Select the build configuration in Projects field of the main view. \n
 *      2. Disable the Shadow build option. \n
 *      3. Go to build environment, add the System Environment variable LD_LIBRARY_PATH and set the path to ../../../../lib \n
 * <HR>
 * \image html LdLibraryPath.png \n
 * <HR>
 * * \note Verify that the following Shared Objects are placed to /lib in the STM32CubeProgrammer package directory :
 * <table>
 * <tr><td>libCubeProgrammer_API.so     <td>libCubeProgrammer_API.so.1      <td>libSTLinkUSBDriver.so   <td>libFileManager.so.1
 * <tr><td>libhsmp11.so                 <td>libQt5SerialPort.so.5           <td>libQtXml.so.5           <td>libQt5Core.so.5
 * <tr><td>libicudata.so.56             <td>libicui18n.so.56                <td>libicuuc.so.56          <td> -
 * </table>
 *
 * \subsection qtMacOS MacOS build settings
 * To get the default api folder that is integrated into the STM32CubeProgrammer package already installed, go to \n
 * /Application/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/contents/MacOs/api
 * \note Verify that the following Mach Objects are placed to /Application/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/contents/MacOs/bin in the STM32CubeProgrammer package directory:
 * <table>
 * <tr><td>libCubeProgrammer_API.dylib      <td>libCubeProgrammer_API.1.dylib       <td>libCubeProgrammer_API.1.0.dylib     <td>libCubeProgrammer_API.1.0.0.dylib
 * <tr><td>libFileManager.dylib             <td>libFileManager.1.dylib              <td>libFileManager.1.0.dylib            <td>libFileManager.1.0.0.dylib
 * <tr><td>libSTLinkUSBDriver.dylib         <td>libhsmp11.dylib                     <td>libstP11_SAM.dylib                  <td>libusb-1.0.0.dylib
 * <tr><td>QtSerialPort.framework           <td>QtXml.framework                     <td>QtCore.framework                    <td> -
 * </table>
 */


#ifndef CUBEPROGRAMMER_API_H
#define CUBEPROGRAMMER_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "DeviceDataStructure.h"

#if (defined WIN32 || defined _WIN32 || defined WINCE)
# define CP_EXPORTS __declspec(dllexport)
#else
# define CP_EXPORTS
#endif


/* -------------------------------------------------------------------------------------------- */
/*                                      Errors Enumerations                                     */
/* -------------------------------------------------------------------------------------------- */

/**
 * \enum  cubeProgrammerVerbosityLevel
 * \brief List of verbosity levels.
 */
enum cubeProgrammerVerbosityLevel {
    /** no messages ever printed by the library */
    CUBEPROGRAMMER_VER_LEVEL_NONE   = 0,

    /** warning, error and success messages are printed (default) */
    CUBEPROGRAMMER_VER_LEVEL_ONE    = 1,

    /** error roots informational messages are printed */
    CUBEPROGRAMMER_VER_LEVEL_TWO    = 2,

    /** debug and informational messages are printed */
    CUBEPROGRAMMER_VER_LEVEL_DEBUG  = 3,

    /** no progress bar is printed in the output of the library */
    CUBEPROGRAMMER_NO_PROGRESS_BAR  = 4, /* progress bars are printed only with verbosity level one */
};


/**
 * \enum  cubeProgrammerError
 * \brief List of errors that can be occured.
 */
enum cubeProgrammerError {
    /** Success (no error) */
    CUBEPROGRAMMER_NO_ERROR = 0,

    /** Device not connected */
    CUBEPROGRAMMER_ERROR_NOT_CONNECTED = -1,

    /** Device not found */
    CUBEPROGRAMMER_ERROR_NO_DEVICE = -2,

    /** Device connection error */
    CUBEPROGRAMMER_ERROR_CONNECTION = -3,

    /** No such file  */
    CUBEPROGRAMMER_ERROR_NO_FILE = -4,

    /** Operation not supported or unimplemented on this interface */
    CUBEPROGRAMMER_ERROR_NOT_SUPPORTED = -5,

    /** Interface not supported or unimplemented on this plateform */
    CUBEPROGRAMMER_ERROR_INTERFACE_NOT_SUPPORTED = -6,

    /** Insufficient memory */
    CUBEPROGRAMMER_ERROR_NO_MEM = -7,

    /** Wrong parameters */
    CUBEPROGRAMMER_ERROR_WRONG_PARAM = -8,

    /** Memory read failure */
    CUBEPROGRAMMER_ERROR_READ_MEM = -9,

    /** Memory write failure */
    CUBEPROGRAMMER_ERROR_WRITE_MEM = -10,

    /** Memory erase failure */
    CUBEPROGRAMMER_ERROR_ERASE_MEM = -11,

    /** File format not supported for this kind of device */
    CUBEPROGRAMMER_ERROR_UNSUPPORTED_FILE_FORMAT = -12,

    /** Refresh required **/
    CUBEPROGRAMMER_ERROR_REFRESH_REQUIRED = -13,

    /** Refresh required **/
    CUBEPROGRAMMER_ERROR_NO_SECURITY = -14,

    /** Changing frequency problem **/
    CUBEPROGRAMMER_ERROR_CHANGE_FREQ = -15,


    /* NB: Remember to update CUBEPROGRAMMER_ERROR_COUNT below. */

    /** Other error */
    CUBEPROGRAMMER_ERROR_OTHER = -99,
};


/* -------------------------------------------------------------------------------------------- */
/*                         Bootloader Data Structures and Enumerations                          */
/* -------------------------------------------------------------------------------------------- */

/**
 * \enum  usartParity
 * \brief The parity bit in the data frame of the USART communication tells the receiving device if there is any error in the data bits.
 */
typedef enum usartParity
{
    EVEN = 0,               /**< Even parity bit. */
    ODD = 1,                /**< Odd parity bit. */
    NONE = 2,               /**< No check parity. */
}usartParity;


/**
 * \enum  usartFlowControl
 * \brief UART Flow Control is a method for devices to communicate with each other over UART without the risk of losing data.
 */
typedef enum usartFlowControl
{
    OFF = 0,                /**< No flow control.  */
    HARDWARE = 1,           /**< Hardware flow control : RTS/CTS.  */
    SOFTWARE = 2,           /**< Software flow control : Transmission is started and stopped by sending special characters. */
}usartFlowControl;


/**
 * \struct  dfuDeviceInfo
 * \brief   Get DFU device informations .
 */
typedef struct dfuDeviceInfo
{
    char usbIndex[10];                  /**< USB index. */
    int busNumber;                      /**< Bus number. */
    int addressNumber;                  /**< Address number. */
    char productId[100];                /**< Product number. */
    char serialNumber[100];             /**< Serial number. */
    unsigned int dfuVersion;            /**< DFU version. */
}dfuDeviceInfo;

/**
 * \struct  usartConnectParameters
 * \brief   Specify the USART connect parameters.
 */
typedef struct usartConnectParameters
{
    char portName[100];             /**< Interface identifier: COM1, COM2, /dev/ttyS0...*/
    unsigned int  baudrate;         /**< Speed transmission: 115200, 9600... */
    usartParity parity;             /**< Parity bit: value in usartParity. */
    unsigned char dataBits;         /**< Data bit: value in {6, 7, 8}. */
    float stopBits;                 /**< Stop bit: value in {1, 1.5, 2}. */
    usartFlowControl flowControl;   /**< Flow control: value in usartFlowControl. */
    unsigned char noinitBits;       /**< Set No Init bits: value in {0,1}. */

}usartConnectParameters;


/**
 * \struct  spiConnectParameters
 * \brief   Specify the SPI connect parameters.
 * \note    Recommended SPI parameters : baudrate=375, crcPol=7, direction=0, cpha=0, cpol=0, crc=0, firstBit=1, frameFormat=0, dataSize=1, mode=1, nss=1, nssPulse=1, delay=1
 */
typedef struct spiConnectParameters
{
    uint32_t baudrate ;        /**< Speed transmission 187, 375, 750, 1500, 3000, 6000, 12000 KHz. */
    uint16_t crcPol   ;        /**< crc polynom value. */
    int direction     ;        /**< 2LFullDuplex/2LRxOnly/1LRx/1LTx. */
    int cpha          ;        /**< 1Edge or 2Edge. */
    int cpol          ;        /**< LOW or HIGH. */
    int crc           ;        /**< DISABLE or ENABLE. */
    int firstBit      ;        /**< First bit: LSB or MSB. */
    int frameFormat   ;        /**< Frame format: Motorola or TI. */
    int dataSize      ;        /**< Size of frame data: 16bit or 8bit . */
    int mode          ;        /**< Operating mode: Slave or Master. */
    int nss           ;        /**< Selection: Soft or Hard. */
    int nssPulse      ;        /**< NSS pulse: No Pulse or Pulse. */
    int delay         ;        /**< Delay of few microseconds, No Delay or Delay, at least 4us delay is inserted */

} spiConnectParameters;


/**
 * \struct  canConnectParameters
 * \brief   Specify the CAN connect parameters.
 * \note    Not all configurations are supported by STM32 Bootloader, such as CAN type is STANDARD and the filter should be always activated.
 * \note    Recommended CAN parameters : br=125000, mode=0, ide=0, rtr=0, fifo=0, fm=0, fs=1, fe=1, fbn=0
 */
typedef struct canConnectParameters
{
    int br     ;                /**< Baudrate and speed transmission 125KHz, 250KHz, 500KHz... */
    int mode   ;                /**< CAN mode: NORMAL, LOOPBACK..., */
    int ide    ;                /**< CAN type: STANDARD or EXTENDED. */
    int rtr    ;                /**< Frame format: DATA or REMOTE. */
    int fifo   ;                /**< Memory of received messages: FIFO0 or FIFO1. */
    int fm     ;                /**< Filter mode: MASK or LIST. */
    int fs     ;                /**< Filter scale: 16 or 32. */
    int fe     ;                /**< Filter activation: DISABLE or ENABLE. */
    char fbn   ;                /**< Filter bank number: 0 to 13. */

} canConnectParameters;


/**
 * \struct  i2cConnectParameters
 * \brief   Specify the I2C connect parameters.
 * \warning The Bootloader Slave address varies depending on the device (see AN2606).
 * \note    Not all configurations are supported by STM32 Bootloader, such as address in 7 bits form, analog filter: ENABLE, digital filter: DISABLE.
 * \note    Recommended I2C parameters : add=0x??, br=400, sm=1, am=0, af=1, df=0, dnf=0, rt=0, ft=0
 */
typedef struct i2cConnectParameters
{
    int add      ;               /**< Device address in hex format. */
    int br       ;               /**< Baudrate and speed transmission : 100 or 400 KHz. */
    int sm       ;               /**< Speed Mode: STANDARD or FAST. */
    int am       ;               /**< Address Mode: 7 or 10 bits. */
    int af       ;               /**< Analog filter: DISABLE or ENABLE. */
    int df       ;               /**< Digital filter: DISABLE or ENABLE. */
    char dnf     ;               /**< Digital noise filter: 0 to 15. */
    int rt       ;               /**< Rise time: 0-1000 for STANDARD speed mode and  0-300 for FAST. */
    int ft       ;               /**< Fall time: 0-300 for STANDARD speed mode and  0-300 for FAST. */

} i2cConnectParameters;


/* -------------------------------------------------------------------------------------------- */
/*                           STLINK Data Structures and Enumerations                            */
/* -------------------------------------------------------------------------------------------- */

/**
 * \enum  debugResetMode
 * \brief Choose the way to apply a system reset.
 */
typedef enum debugResetMode
{
    SOFTWARE_RESET,         /**< Apply a reset by the software. */
    HARDWARE_RESET,         /**< Apply a reset by the hardware. */
    CORE_RESET              /**< Apply a reset by the internal core peripheral. */
}debugResetMode;


/**
 * \enum  debugConnectMode
 * \brief Choose the appropriate mode for connection.
 */
typedef enum debugConnectMode
{
    NORMAL_MODE,            /**< Connect with normal mode, the target is reset then halted while the type of reset is selected using the [debugResetMode]. */
    HOTPLUG_MODE,           /**< Connect with hotplug mode,  this option allows the user to connect to the target without halt or reset. */
    UNDER_RESET_MODE,       /**< Connect with under reset mode, option allows the user to connect to the target using a reset vector catch before executing any instruction. */
    PRE_RESET_MODE          /**< Connect with pre reset mode. */
}debugConnectMode;


/**
 * \enum  debugPort
 * \brief Select the debug port interface for connection.
 */
typedef enum debugPort
{
    JTAG = 0,               /**< JTAG debug port. */
    SWD = 1,                /**< SWD debug port. */
}debugPort;


/**
 * \struct  frequencies
 * \brief   Get supported frequencies for JTAG and SWD ineterfaces.
 */
typedef struct frequencies
{
    unsigned int jtagFreq[12];          /**<  JTAG frequency. */
    unsigned int jtagFreqNumber;        /**<  Get JTAG supported frequencies. */
    unsigned int swdFreq[12];           /**<  SWD frequency. */
    unsigned int swdFreqNumber;         /**<  Get SWD supported frequencies. */
}frequencies;


/**
 * \struct  debugConnectParameters
 * \brief   Get device characterization and specify connection parameters through ST-LINK interface.
 */
typedef struct debugConnectParameters
{
    debugPort dbgPort;                  /**< Select the type of debug interface #debugPort. */
    int index;                          /**< Select one of the debug ports connected. */
    char serialNumber[33];              /**< ST-LINK serial number. */
    char firmwareVersion[20];           /**< Firmware version. */
    char targetVoltage[5];              /**< Operate voltage. */
    int accessPortNumber;               /**< Number of available access port. */
    int accessPort;                     /**< Select access port controller. */
    debugConnectMode connectionMode;    /**< Select the debug CONNECT mode #debugConnectMode. */
    debugResetMode resetMode;           /**< Select the debug RESET mode #debugResetMode. */
    int isOldFirmware;                  /**< Check Old ST-LINK firmware version. */
    frequencies freq;                   /**< Supported frequencies #frequencies. */
    int frequency;                      /**< Select specific frequency. */
    int isBridge;                       /**< Indicates if it's Bridge device or not. */
    int shared;                         /**< Select connection type, if it's shared, use ST-LINK Server. */
}debugConnectParameters;


/* -------------------------------------------------------------------------------------------- */
/*                          General Data Structures and Enumerations                            */
/* -------------------------------------------------------------------------------------------- */

/**
 * \enum  targetInterfaceType
 * \brief Indicates the supported interfaces.
 */
typedef enum targetInterfaceType
{
    STLINK_INTERFACE = 0,           /**< STLINK used as connection interface. */
    USART_INTERFACE  = 1,           /**< USART used as connection interface. */
    USB_INTERFACE    = 2,           /**< USB DFU used as connection interface. */
    SPI_INTERFACE    = 3,           /**< SPI used as connection interface. */
    I2C_INTERFACE    = 4,           /**< I2C used as connection interface. */
    CAN_INTERFACE    = 5            /**< CAN used as connection interface. */
}targetInterfaceType;


/**
 * \struct  displayCallBacks
 * \brief   Functions must be implemented to personalize the display of messages.
 */
typedef struct displayCallBacks
{
    void (*initProgressBar)();                              /**< Add a progress bar. */
    void (*logMessage)(int msgType,  const wchar_t* str);   /**< Display internal messages according to verbosity level. */
    void (*loadBar)(int x, int n);                          /**< Display the loading of read/write process. */
} displayCallBacks;


typedef struct segmentData_C
{
    int address;                    /**< Segment start address. */
    int size;                       /**< Memory segment size. */
    unsigned char* data;            /**< Memory segment data. */
}segmentData_C ;


/**
 * \struct  FileData_C
 * \brief   Get file required informations.
 */
typedef struct fileData_C
{
    int Type;                       /**< File extension type. */
    int segmentsNbr;                /**< Number of required segments. */
    segmentData_C* segments;        /**< Segments description. */
}fileData_C ;


/**
 * \struct  GeneralInf
 * \brief   Get device general informations.
 */
typedef struct generalInf
{
    unsigned short deviceId;       /**< Device ID. */
    int  flashSize;                 /**< Flash memory size. */
    char type[4];                   /**< Device MCU or MPU. */
    char cpu[20];                   /**< Cortex CPU. */
    char name[100];                 /**< Device name. */
    char series[100];               /**< Device serie. */
    char description[100];          /**< Take notice. */
}generalInf ;


/* -------------------------------------------------------------------------------------------- */
/*                              Loaders Data Structures                                         */
/* -------------------------------------------------------------------------------------------- */


/**
 * \struct  deviceSector
 * \brief   Get device sectors basic informations.
 */
typedef struct deviceSector
{
  uint32_t		sectorNum;                  /**< Number of Sectors. */
  uint32_t		sectorSize;                 /**< Sector Size in BYTEs. */
}deviceSector;


/**
 * \struct  externalLoader
 * \brief   Get external Loader parameters to launch the process of programming an external flash memory.
 */
typedef struct externalLoader{
    char             filePath[200];         /**< FlashLoader file path. */
    char             deviceName[100];		/**< Device Name and Description. */
    int              deviceType;		    /**< Device Type: ONCHIP, EXT8BIT, EXT16BIT, ... */
    uint32_t         deviceStartAddress;	/**< Default Device Start Address. */
    uint32_t         deviceSize;			/**< Total Size of Device. */
    uint32_t         pageSize;				/**< Programming Page Size. */
//  unsigned char  	 EraseValue;            /**< Content of Erased Memory. */
    uint32_t         sectorsTypeNbr;        /**< Type number. */
    deviceSector*	 sectors;               /**< Device sectors. */
}externalLoader;


/**
 * \struct  externalStorageInfo
 * \brief   Get external storage informations useful for external Loader.
 */
typedef struct externalStorageInfo
{
    unsigned int externalLoaderNbr;
    externalLoader* externalLoader;
}externalStorageInfo;


/* -------------------------------------------------------------------------------------------- */
/*                              STLINK functions                                                */
/* -------------------------------------------------------------------------------------------- */

/*! \addtogroup STLINK
 * STLINK module groups debug ports JTAG/SWD functions together.
 *  @{
 */

/**
 * \brief This routine allows to get ST-LINK conneted probe(s).
 * \param stLinkList  : Filled with the connected ST-LINK list and its default configurations.
 * \param shared      : Enable shared mode allowing connection of two or more instances to the same ST-LINK probe.
 * \return Number of the ST-LINK probes already exists.
 * \warning The Share option is useful only with ST-LINK Server.
 * \note  At the end of usage, #deleteInterfaceList must have been called.
 */
int getStLinkList(debugConnectParameters** stLinkList, int shared);


/**
 * \brief This routine allows to start connection to device through SWD or JTAG interfaces.
 * \param debugParameters : Indicates customized configuration for ST-LINK connection,
 * It is recommended to check [debugConnectParameters] fields before connection.
 * \return 0 if the connection successfully established, otherwise an error occurred.
 */
int connectStLink(debugConnectParameters debugParameters);


/**
 * \brief This routine used to apply a target reset.
 * \note  Reset operation is only available with JTAG/SWD debug interface.
 * \param rstMode : Indicates the reset type Soft/Hard/Core #debugResetMode. \n
 * \return 0 if the reset operation finished successfully, otherwise an error occurred.
 */
int reset(debugResetMode rstMode) ;

/*! @} */


/* -------------------------------------------------------------------------------------------- */
/*                              Bootloader functions                                            */
/* -------------------------------------------------------------------------------------------- */

/*! \addtogroup Bootloader
 * Bootloader module is a way to group Serial interfaces USB/UART/SPI/I2C/CAN functions together.
 *  @{
 */

/**
 * \brief This routine allows to get connected serial ports.
 * \param usartList : Receive serial ports list and its default configurations.
 * \return Number of serial ports already connected.
 * \note  At the end of usage, #deleteInterfaceList must have been called.
 */
int getUsartList(usartConnectParameters** usartList);


/**
 * \brief This routine allows to start connection to device through USART interface.
 * \param usartParameters : Indicates customized configuration for USART connection.
 * \return 0 if the connection successfully established, otherwise an error occurred.
 */
int connectUsartBootloader(usartConnectParameters usartParameters);


/**
 * \brief This routine allows to send a single byte through the USART interface.
 * \param byte : The data to be written
 * \return 0 if the sending operation correctly achieved, otherwise an error occurred.
 */
int sendByteUart(int byte);


/**
 * \brief This routine allows to get connected DFU devices.
 * \param dfuList : Receive DFU devices list and its default configurations.
 * \return Number of DFU devices already connected.
 * \note  At the end of usage, #deleteInterfaceList must have been called.
 */
int getDfuDeviceList(dfuDeviceInfo** dfuList);


/**
 * \brief This routine allows to start connection to device through USB DFU interface.
 * \param usbIndex : Indicates the index of DFU ports already connected.
 * \return 0 if the connection successfully established, otherwise an error occurred.
 */
int connectDfuBootloader(char* usbIndex);


/**
 * \brief This routine allows to start connection to device through SPI interface.
 * \param spiParameters : Indicates customized configuration for  SPI connection
 * \return 0 if the connection successfully established, otherwise an error occurred.
 */
int connectSpiBootloader(spiConnectParameters spiParameters);


/**
 * \brief This routine allows to start connection to device through CAN interface.
 * \param canParameters : Indicates customized configuration for  CAN connection
 * \return 0 if the connection successfully established, otherwise an error occurred.
 * \warning To have CAN full support, you must have St-Link firmware version at least v3JxMxB2.
 */
int connectCanBootloader(canConnectParameters canParameters);


/**
 * \brief This routine allows to start connection to device through I2C interface.
 * \param i2cParameters : Indicates customized configuration for  I2C connection
 * \return 0 if the connection successfully established, otherwise an error occurred.
 */
int connectI2cBootloader(i2cConnectParameters i2cParameters);

/*! @} */


/* -------------------------------------------------------------------------------------------- */
/*                              General purposes functions                                      */
/* -------------------------------------------------------------------------------------------- */

/*! \addtogroup General
 * General module groups general purposes functions used by any interface.
 *  @{
 */

/**
 * \brief This routine allows to choose your custom display.
 * \param c : Fill the struct to customize the display tool.
 * \note This function must be called first of all to ensure the display management.
 */
void setDisplayCallbacks(displayCallBacks c);


/**
 * \brief This routine allows to choose the verbosity level for display.
 * \param level : Indicates the verbosity number 0, 1 or 3.
 */
void setVerbosityLevel(int level);


/**
 * \brief This routine allows to check connection status [maintained or lost].
 * \return 1 if the device is already connected, otherwise the connection to device is lost.
 */
int checkDeviceConnection();


/**
 * \brief This routine allows to get general device informations.
 * \return Structure #GeneralInf in which the informations are stored.
 */
generalInf* getDeviceGeneralInf();


/**
 * \brief This routine allows to receive memory data on the used interface with the configration already initialized.
 * \param address   : The address to start reading from.
 * \param data      : Pointer to the data buffer.
 * \param size      : It indicates the size for read data.
 * \return 0 if the reading operation correctly finished, otherwise an error occurred.
 * \warning Unlike ST-LINK interface, the Bootloader interface can access only to some specific memory regions.
 */
int readMemory(unsigned int address, unsigned char** data, unsigned int size);


/**
 * \brief This routine allows to write memory data on the user interface with the configration already initialized.
 * \param address   : The address to start writing from.
 * \param data      : Pointer to the data buffer.
 * \param size      : It indicates the size for write data.
 * \return 0 if the writing operation correctly finished, otherwise an error occurred.
 * \warning Unlike ST-LINK interface, the Bootloader interface can access only to some specific memory regions.
 */
int writeMemory(unsigned int address, char* data, unsigned int size);


/**
 * \brief This routine allows to download data from a file to the memory.
 * File formats that are supported : hex, bin, srec
 * \param filePath  : Indicates the full path of the considered file.
 * \param address   : The address to start downloading from.
 * \param skipErase : In case to win in term time and if we have a blank device, we can skip erasing memory before programming [skipErase=0].
 * \param verify    : To add verification step after downloading.
 * \param binPath   : Path of the binary file.
 * \return 0 if the downloading operation correctly finished, otherwise an error occurred.
 */
int downloadFile(const wchar_t* filePath, unsigned int address, unsigned int skipErase, unsigned int verify, const wchar_t* binPath);


/**
 * \brief This routine allows to run the application.
 * \param address : The address to start executing from.
 * In most cases, the program will run from the Flash memory starting from 0x08000000.
 * \return 0 if the execution correctly started, otherwise an error occurred.
 */
int execute(unsigned int address);


/*!
 * \brief This routine allows to erase the whole Flash memory.
 * \return 0 if the operation finished successfully, otherwise an error was occurred.
 * \note Depending on the device, this routine can take a particular period of time.
 */
int massErase();


/**
 * \brief This routine allows to erase specific sectors of the Flash memory.
 * \param sectors   : Indicates the indexs of the specific sectors to be erased.
 * \param sectorNbr : The number of chosen sectors.
 * \return 0 if the operation finished successfully, otherwise an error occurred.
 * \note Each circuit has a specific number of Flash memory sectors.
 */
int sectorErase(unsigned int sectors[],unsigned int sectorNbr);


/**
 * \brief This routine allows to disable the readout protection.
 * If the memory is not protected, a message appears to indicate that the device is not
 * under Readout protection and the command has no effects.
 * \return 0 if the disabling correctly accomplished, otherwise an error occurred.
 * \note Depending on the device used, this routine take a specific time.
 */
int readUnprotect();


/**
 * \brief This routine allows to know the interface what is in use.
 * \return The target interface type #targetInterfaceType, otherwise -1.
 */
int  getTargetInterfaceType();


/**
 * \brief This routine allows to drop the current read/write operation.
 * \return 0 if there is no call for stop operation, otherwise 1.
 */
volatile int* getCancelPointer();


/**
 * \brief This routine allows to open and get data from any supported file extension.
 * \param filePath : Indicates the full path of the considered file.
 * \return Pointer to #fileData_C if the file has hex, bin, srec or elf as extension, otherwise a null pointer to indicate that the file type is not supported.
 */
void *fileOpen(const wchar_t* filePath);


/**
 * \brief This routine allows to clean up the handled file data.
 * \param data
 */
void freeFileData(fileData_C* data);


/**
 * \brief This routine allows to verfiy if the indicated file data is identical to Flash memory content.
 * \param fileData : Input file name.
 * \param address  : The address to start verifying from, it's considered only if the file has .bin or .binary as extension.
 * \return 0 if the file data matching Flash memory content, otherwise an error occurred or the data is mismatched.
 */
int verify(fileData_C* fileData, unsigned int address);


/**
 * \brief This routine allows to save the data file content to another file.
 * \param fileData  : Input file name.
 * \param sFileName : Output file name.
 * \return 0 if the output file was created successfully, otherwise an error occurred.
 */
int saveFileToFile(fileData_C *fileData, const wchar_t* sFileName);


/**
 * \brief This routine allows to save Flash memory content to file.
 * \param address   : The address to start saving from.
 * \param size      : Data size to be saved.
 * \param sFileName : Indicates the file name.
 * \return 0 if the data copy was acheived successfully, otherwise an error occurred.
 * \note The file name must finish with an extension ".hex", ".bin" or ".srec"
 */
int saveMemoryToFile(int address, int size, const wchar_t* sFileName);


/**
 * \brief This routine allows to clean up and disconnect the current connected target.
 * \note This routine disconnect the target and delete the loaded Flash Loaders.
 */
void disconnect();


/**
 * \brief This routine allows to clear the list of each created interface.
 * \note The list is filled by #getStlinkList, #getDfuDeviceList or #getUsartList.
 */
void deleteInterfaceList();


/**
 * \brief This routine allows to enter and make an automatic process for memory management through JTAG/SWD, UART, DFU, SPI, CAN and I²C interfaces.
 * \param filePath      : Indicates the full file path.
 * \param address       : The address to start downloading from.
 * \param skipErase     : If we have a blank device, we can skip erasing memory before programming [skipErase=0].
 * \param verify        : Add verification step after downloading.
 * \param isMassErase   : Erase the whole Flash memory.
 * \param obCommand     : Indicates the option bytes commands to be loaded "-ob [optionbyte=value] [optionbyte=value]..."
 * \param run           : Start the application.
 * \warning Connection to target must be established before performing automatic mode.
 */
void automaticMode(const wchar_t* filePath, unsigned int address, unsigned int skipErase, unsigned int verify, int isMassErase, char* obCommand, int run);


/*! @} */


/* -------------------------------------------------------------------------------------------- */
/*                                  Option Bytes functions                                      */
/* -------------------------------------------------------------------------------------------- */

/*! \addtogroup OB
 * OB module groups option bytes functions used by any interface.
 *  @{
 */


/**
 * \brief This routine allows program the given Option Byte.
 * The option bytes are configured by the end user depending on the application requirements.
 * \param command : Indicates the command to execute.
 * \return 0 if the programming Option Byte correctly executed, otherwise an error occurred.
 * \note The command must written as: -ob [optionbyte=value] [optionbyte=value] ...
 * \code
 * int ob = sendOptionBytesCmd("–ob rdp=0x0 BOR_LEV=0");
 * \endcode
 */
int sendOptionBytesCmd(char* command);


/**
 * \brief This routine allows to get option bytes values of the connected target.
 * \return Structure #Peripheral_C in which the option bytes descriptions are stored.
 */
peripheral_C* initOptionBytesInterface();

 /*! @} */


/* -------------------------------------------------------------------------------------------- */
/*                                  Loaders functions                                           */
/* -------------------------------------------------------------------------------------------- */


/*! \addtogroup Loaders
 * Loaders module groups loaders functions.
 *  @{
 */


/**
 * \brief This routine allows to specify the location of Flash Loader.
 * \param path : Indicates the full path of the considered folder.
 */
void setLoadersPath(const char* path);


/**
 * \brief This routine allows to specify the path of the external Loaders to be loaded.
 * \param path : Indicates the full path of the folder containing external Loaders.
 * \param externalLoaderInfo : Structure in which the external Loaders informations are stored.
 */
void setExternalLoaderPath(const char* path, externalLoader** externalLoaderInfo);


/**
 * \brief This routine allows to get available external Loaders in th mentioned path.
 * \param path : Indicates the full path of the external Loader file ready for loading.
 * \param externalStorageNfo : Structure in which we get storage information.
 * \return 1 if the External loaders cannot be loaded from the path, otherwise 0.
 * \warning All external Loader files should have the extension "stldr".
 */
int  getExternalLoaders(const char* path, externalStorageInfo** externalStorageNfo);


/**
 * \brief This routine allows to unload an external Loaders.
 * \param path : Indicates the full path of the external Loader file ready for unloading.
 */
void removeExternalLoader(const char* path);


/**
 * \brief This routine allows to delete all target Flash Loaders.
 */
void deleteLoaders();

/*! @} */



#ifdef __cplusplus
}
#endif



#endif // CUBEPROGRAMMER_API_H
