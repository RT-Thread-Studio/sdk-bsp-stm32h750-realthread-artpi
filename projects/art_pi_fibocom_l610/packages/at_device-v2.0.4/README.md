# AT device #

[Chinese](./README_ZH.md) | English

## 1. Introduction ##


The AT device software package is composed of the transplantation files and sample codes of the RT-Thread AT component for different AT devices. Currently supported AT devices are: ESP8266, ESP32, M26, MC20, RW007, MW31, SIM800C, W60X, SIM76XX, A9/ A9G, BC26, AIR720, ME3616, M6315, BC28, EC200X, M5311, L610 series devices, etc. At present, the above devices have completed the transplantation of the `AT socket` function, and the device implements the standard socket programming interface through AT commands to complete the socket communication For detailed function introduction, please refer to ["RT-Thread Programming Guide"](https://www.rt-thread.org/document/site/programming-manual/at/at/) AT command chapter.

### 1.1. Directory structure ###

| Name | Description |
| ---- | ---- |
| src | AT device implementation source code directory |
| inc | AT device header file directory |
| sample | Sample file directory for different devices |
| class | Migration and adaptation catalog of AT components for different devices |
| class/esp8266 | ESP8266 device's migration directory for AT components, realizing AT Socket function |
| class/esp32 | ESP32 device's migration directory for AT components to realize AT Socket function |
| class/rw007 | RW007 device is a migration directory for AT components, realizing AT Socket function |
| class/sim800c | SIM800C device for AT component transplantation directory, realize AT Socket function |
| class/sim76xx | SIM76XX device's migration directory for AT components, realizing AT Socket function |
| class/m26 | M26/MC20 device's migration directory for AT components, realizing AT Socket function |
| class/ec20 | EC20 device's migration directory for AT components, realizing AT Socket function |
| class/mw31 | Migration directory for AT components of MW31 equipment, realizing AT Socket function |
| class/w60x | W60X device's migration directory for AT components, realizing AT Socket function |
| class/a9g | A9G device's migration directory for AT components, realizing AT Socket function |
| class/bc26 | The migration directory of bc26 device aiming at AT components to realize AT Socket function |
| class/air720 | Air720 device is aimed at AT component transplantation catalog, realizes AT Socket function |
| class/me3616 | The me3616 device is aimed at the transplantation directory of AT components, and realizes the AT Socket function |
| class/m6315 | The migration directory of m6315 equipment for AT components, realizes the AT Socket function |
| class/bc28 | The migration directory of bc28 equipment for AT components, realizes AT Socket function |
| class/ec200x | EC200T, EC200S equipment for AT component migration catalog, realize AT Socket function |
| class/n21 | N21 device's transplantation directory for AT components to realize AT Socket function |
| class/n58 | N58 device's migration directory for AT components, realizing AT Socket function |
| class/m5311 | M5311 device is aimed at AT component transplantation catalog, realizes AT Socket function |
| class/l610 | A migration directory for AT components of L610 equipment, realizing AT Socket function |
### 1.2 License ###

The at_device package complies with the LGPLv2.1 license, see the `LICENSE` file for details.

### 1.3 Dependency ###

- RT_Thread 4.0.2+
- RT_Thread AT component 1.3.0+
- RT_Thread SAL component
- RT-Thread netdev component

## 2. How to Obtain ##

The AT device software package is a transplantation of the AT component and AT socket function. You need to enable the AT component library and AT socket function to obtain the AT device software package.

**Version number description**

At present, the AT device software package has been released in multiple versions, and the option configuration method between each version and its corresponding system version are different. The following mainly lists the currently available software package version information:

- **V1.2.0**: applicable to RT-Thread version less than V3.1.3, AT component version equal to V1.0.0;
- **V1.3.0**: applicable to RT-Thread version less than V3.1.3, AT component version equal to V1.1.0;
- **V1.4.0**: applicable to RT-Thread version less than V3.1.3 or equal to V4.0.0, AT component version equal to V1.2.0;
- **V1.5.0**: Suitable for RT-Thread version less than V3.1.3 or equal to V4.0.0, AT component version equal to V1.2.0;
- **V1.6.0**: applicable to RT-Thread version equal to V3.1.3 or equal to V4.0.1, AT component version equal to V1.2.0;
- **V2.0.0/V2.0.1**: applicable to RT-Thread version greater than V4.0.1 or greater than 3.1.3, AT component version equal to V1.3.0;
- **latest**: Only applicable to RT-Thread version greater than V4.0.1 or greater than 3.1.3, AT component version equal to V1.3.0;

> The above version judgment is automatically completed in menuconfig. When the at_device software package selects the version, it will give the best version support according to the current system environment. The version introduction is used as a reference for the operating environment.

For different version numbers, the option configuration in ENV is also different, mainly divided into the following parts:

**V1.X.X version configuration options introduction**

Open the AT device software package. This version only supports **Enable one AT device at the same time**. The configuration options are as follows:

    RT-Thread online packages --->
         IoT-internet of things --->
            -*- AT DEVICE: RT-Thread AT component porting or samples for different device
            [] Enable at device init by thread
                  AT socket device modules (Not selected, please select) --->
                  Version (V1.6.0) --->

- **Enable at device init by thread**: Configure whether to enable the device network initialization to be completed by creating a thread;

- **AT socket device modules**: AT device selection, currently supports RW007, ESP8266, M26/MC20, EC20, SIM800C, SIM76XX, A9/A9G, BC26, air720, ME3616, M6315, BC28, EC200X, M5311 and other devices;

- **Version**: download the software package version;

**V2.X.X (latest) version configuration options introduction**

Open the AT device software package, this version supports **open multiple AT devices at the same time** configuration options are as follows:

```c
RT-Thread online packages --->
     IoT-internet of things --->
        -*- AT DEVICE: RT-Thread AT component porting or samples for different device
        [*] Quectel M26/MC20 --->
          [*] Enable initialize by thread
          [*] Enable sample
          (-1) Power pin
          (-1) Power status pin
          (uart3) AT client device name
          (512) The maximum length of receive line buffer
        [] Quectel EC20 --->
        [] Espressif ESP32 --->
        [*] Espressif ESP8266 --->
          [*] Enable initialize by thread
          [*] Enable sample
          (realthread) WIFI ssid
          (12345678) WIFI password
          (uart2) AT client device name
          (512) The maximum length of receive line buffer
        [] Realthread RW007 --->
        [] SIMCom SIM800C --->
        [] SIMCom SIM76XX --->
        [] Notion MW31 --->
        [] WinnerMicro W60X --->
        [] AiThink A9/A9G --->
        [] Quectel BC26 --->
        [] Luat air720 --->
        [] GOSUNCN ME3616 --->
        [] ChinaMobile M6315 --->
        [] Quectel BC28 --->
        [] Quectel ec200x --->
        [] Neoway N21 --->
        [] Neoway N58 --->
        [] ChinaMobile M5311 --->
        Version (latest) --->
```

- **Quectel M26/MC20**: enable M20/MC20 (2G module) device support;
  - **Enable initialize by thread**: enable the use of threads to initialize the device (non-blocking mode initialization);
  - **Enable sample**: Open the sample code, the sample code has the registration of the sample device;
  - **Power pin**: Configure the power-on pin of the sample device;
  - **Power status pin**: Configure the power status pin of the sample device;
  - **AT client device name**: Configure the serial device name used by the sample device;
  - **The maximum length of receive line buffer**: Configure the maximum data length received in one line of the sample device;
- **Quectel EC20**: enable EC20 (4G module) device support;
- **Espressif ESP8266**: enable ESP8266 (WIFI module) device support;
    - **Enable initialize by thread**: enable the use of threads to initialize the device (non-blocking mode initialization);
    - **Enable sample**: Open the sample code, the sample code has the registration of the sample device;
    - **WIFI ssid**: Configure the WIFI user name connected to the sample device;
    - **WIFI password**: Configure the WIFI password connected to the sample device;
    - **AT client device name**: Configure the serial device name used by the sample device;
    - **The maximum length of receive line buffer**: Configure the maximum length of data received in one line of the sample device;
- **Espressif ESP32**: enable ESP32 (WIFI module) device support;
- **Realthread RW007**: enable RW007 (WIFI module) device support;
- **SIMCom SIM800C**: enable SIM800C (2G module) device support;
- **SIMCom SIM76XX**: enable SIM76XX (4G module) device support;
- **Notion MW31**: enable MW31 (WIFI module) device support;
- **WinnerMicro W60X**: enable W60X (WIFI module) device support;
- **AiThink A9/A9G**: enable A9/A9G (2G module) device support;
- **Quectel BC26**: enable BC26 (NB-IOT module) device support;
- **Luat Air720**: enable air720 (4g module) device support;
- **GOSUNCN ME3616**: enable ME3616 (NB-IOT module) device support;
- **ChinaMobile M6315**: enable M6315 (2G module) device support;
- **Quectel BC28**: enable BC28 (NB-IoT module) device support;
- **Quectel EC200X**: enable EC200T, EC200S (4G module) device support;
- **Neoway N21**: enable N21 (NB-IoT module) device support;
- **Neoway N58**: enable N58 (4G module) device support;
- **ChinaMobile M5311**: enable M5311 (NB-IoT module) device support;
- **Version**: download the software package version;

The above configuration options take 2G module and WIFI module options as examples to introduce the configuration method of the AT device software package of the `V2.X.X` version. The following points are worth noting:

- `V2.X.X` version supports multiple AT devices to be turned on at the same time. You can view the information of the turned on devices through the `ifocnfig` command in FinSH;
- Devices of `V2.X.X` version need to be registered before they can be used. Currently, device registration is completed in the samples directory file, and users can also customize device registration at the application layer.
- Pin options such as `Power pin` and `Power status pin` are configured according to the specific device hardware connection. If the hardware power-on function is not used, it can be configured as `-1`;
- An AT device corresponds to a serial port name, and the `AT client device name` configured for each device should be different.

**Introduction to AT component related configuration options**

After the AT device software package is selected and the related device support is enabled, the client function of the AT component will be selected by default. The following are the AT component configuration options.

```c
RT-Thread Components --->
    Network --->
        AT commands --->
    [] Enable debug log output
    [] Enable AT commands server
    -*- Enable AT commands client
    (1) The maximum number of supported clients
    -*- Enable BSD Socket API support by AT commnads
    [*] Enable CLI(Command-Line Interface) for AT commands
    [] Enable print RAW format AT command communication data
    (128) The maximum lenght of AT Commonds buffe
```

Among them, the configuration options related to the AT device software package:

- **The maximum number of supported clients**: The maximum number of supported AT clients (multiple devices selected in the AT device software package need to be configured with the corresponding value);
- **Enable BSD Socket API support by AT commnads**: enable AT Socket function support, select the AT device software package and select this option by default;
- **The maximum lenght of AT Commonds buffe**: The maximum supported length of sending command data.

## 3. Matters needing attention ##

- The module adapted to the AT device software package does not support serving as a TCP Server to complete server-related operations (such as accept, etc.);
- The default device type of the AT device software package is not selected, and the device model needs to be specified when using it;
- The `latest` version supports the access of multiple selected AT devices to realize the AT Socket function. The `V1.X.X` version only supports the access of a single AT device.
- At present, multiple versions of the AT device software package are mainly used to adapt to the changes of AT components and systems. It is recommended to use the latest version of the RT-Thread system and select the `latest` version in the menuconfig option;
- Please refer to the description in `at_sample_xxx.c`, some functions need to increase the setting value of `AT_CMD_MAX_LEN`, `RT_SERIAL_RB_BUFSZ`.

## 4. Related documents

- [AT component programming guide](https://www.rt-thread.org/document/site/programming-manual/at/at/)
- [Apply AT components to connect to ESP8266 module](https://www.rt-thread.org/document/site/application-note/components/at/an0014-at-client/)

## 5. Contact

* Maintenance: RT-Thread development team and community developers
* Homepage: https://github.com/RT-Thread-packages/at_device


