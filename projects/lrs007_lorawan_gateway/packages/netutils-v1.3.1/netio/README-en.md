# NetIO: Network throughput test tool

## 1 Introduction

[NetIO](http://www.nwlab.net/art/netio/netio.html) A tool for network performance testing on OS/2 2.x, Windows, Linux and Unix. It uses TCP/UDP to test network net throughput using data packets of different sizes.

RT-Thread currently supports NetIO TCP server.

## 2. Use

### 2.1 Start NetIO server

You need to use Finsh/MSH commands on RT-Thread to start the NetIO server. The general effect is as follows:

```
msh />netio_init
NetIO server start successfully
msh />
```

### 2.2 Install NetIO-GUI test software

The installation file is located in `/tools/netio-gui_v1.0.4_portable.exe`, this is a green software, the installation is actually a process of unzipping, just unzip it to a new folder.

### 2.3 Perform NetIO testing

Open the newly installed `NetIO-GUI` software and configure it as follows:

- Open `NetIO-GUI.exe`;
- Select `Client-Mode` mode, `TCP` protocol;
- Fill in the IP address of the NetIO server. You can use the ifconfig command to view under MSH of RT-Thread;
- Click `Start measure` to start the test (before the test, make sure that the server can be pinged by the PC);
- Wait for the test to end. After the end, the receiving and sending test results corresponding to different data packets will be displayed in the result area.

![netio_tested](../images/netio_tested.png)
