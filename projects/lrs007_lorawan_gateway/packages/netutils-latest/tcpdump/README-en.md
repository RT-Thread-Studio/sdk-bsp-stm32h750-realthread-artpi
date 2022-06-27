# tcpdump

## 1 Introduction
This is a small tool for capturing IP packets based on RT-Thread. The captured data can be saved through the file system, or imported into the PC through the rdb tool, and analyzed by the wireshark software.



### 1.1, dependency

- Rely on [optparse](https://github.com/liu2guang/optparse) package
- Rely on [dfs](https://www.rt-thread.org/document/site/rtthread-development-guide/rtthread-manual-doc/zh/1chapters/12-chapter_filesystem/) file system
- Rely on [env](https://www.rt-thread.org/document/site/rtthread-development-guide/rtthread-tool-manual/env/env-user-manual/) tool
- RT-Thread 3.0+, no dependency on bsp

### 1.2、How to get
- Use menuconfig to enable tcpdump, the details are as follows:

```
  RT-Thread online packages --->
      IOT internet of things --->
          [*] netutils: Networking utilities for RT-Thread --->
          [*] Enable tcpdump tool
          [] Enable tcpdump data to print on the console
          [*] Enable tcpdump debug log output
```
After saving the menuconfig configuration, use the `pkgs --update` command to download the package

> Note: The debugging information is not recommended



## 2. Use

### 2.1, the meaning of tcpdump command

```
-i: Specify the listening network interface
-m: select the save mode (file system or rdb)
-w: user-specified file name xx.pcap
-p: stop capturing packets
-h: help information
```

### 2.2、Command details

```
msh />tcpdump -h

|>------------------------- help -------------------------<|
| tcpdump [-p] [-h] [-i interface] [-m mode] [-w file]     |
|                                                          |
| -h: help                                                 |
| -i: specify the network interface for listening          |
| -m: choose what mode(file-system or rdb) to save the file|
| -w: write the captured packets into an xx.pcap file      |
| -p: stop capturing packets                               |
|                                                          |
| e.g.:                                                    |
| specify network interface and select save mode \         |
| and specify filename                                     |
| tcpdump -ie0 -mfile -wtext.pcap                          |
| tcpdump -ie0 -mrdb -wtext.pcap                           |
|                                                          |
| -m: file-system mode                                     |
| tcpdump -mfile                                           |
|                                                          |
| -m: rdb mode                                             |
| tcpdump -mrdb                                            |
|                                                          |
| -w: file                                                 |
| tcpdump -wtext.pcap                                      |
|                                                          |
| -p: stop                                                 |
| tcpdump -p                                               |
|                                                          |
| -h: help                                                 |
| tcpdump -h                                               |
|                                                          |
| write commands but no arguments are illegal!!            |
| e.g.: tcpdump -i / -i -mfile  / -i -mfile -wtext.pcap    |
|>------------------------- help -------------------------<|

msh />
```


## 3. Use the file system to save the captured data

> We are here to mount the sd-card to the file system

### 3.1, prepare before capturing

Before powering on the development board, insert the sd-card

- If the mount is successful, it will prompt:

```
SD card capacity 31023104 KB
probe mmcsd block device!
found part[0], begin: 10485760, size: 29.580GB
File System initialized!
```

- If the mount fails, it will prompt:

```
sdcard init fail or timeout: -2!
```

- The mount is successful, enter `list_device` to see the `sd0` device, the details are as follows:

```
msh />list_device
device         type         ref count
------ -------------------- ---------
sd0    Block Device         1       
e0     Network Interface    0             
usbd   USB Slave Device     0                   
rtc    RTC                  1       
spi4   SPI Bus              0       
pin    Miscellaneous Device 0       
uart1  Character Device     3       
msh />
```

### 3.2, check before capturing packets

> Please confirm the IP address of the board before capturing packets


- In msh />, enter `ifconfig` to view, the details are as follows:

```
msh />
network interface: e0 (Default)
MTU: 1500
MAC: 00 04 9f 05 44 e5
FLAGS: UP LINK_UP ETHARP BROADCAST
ip address: 192.168.1.137
gw address: 192.168.1.1
net mask: 255.255.255.0
dns server #0: 192.168.1.1
dns server #1: 0.0.0.0
msh />
```

### 3.3, start capturing packets

- In msh />, enter `tcpdump -ie0 -mfile -wtext.pcap`, the details are as follows:

```
msh />tcpdump -ie0 -msd -wtext.pcap
[TCPDUMP]select [e0] network card device
[TCPDUMP]select [file-system] mode
[TCPDUMP]save in [text.pcap]
[TCPDUMP]tcpdump start!
msh />
```

- Use the capture command to create a thread, the thread priority is 12.
- Enter the `list_thread` command to view the running threads, the thread name is `tdth`, details are as follows:

```
thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
tdth      12  suspend 0x000000ac 0x00000800    08%   0x0000000a 000
tshell    20  ready   0x00000070 0x00001000    22%   0x00000003 000
rp80       8  suspend 0x0000009c 0x00000400    15%   0x0000000a 000
phy       30  suspend 0x00000070 0x00000200    28%   0x00000001 000
usbd       8  suspend 0x00000098 0x00001000    03%   0x00000014 000
tcpip     10  suspend 0x000000b4 0x00000400    39%   0x00000014 000
etx       12  suspend 0x00000084 0x00000400    12%   0x00000010 000
erx       12  suspend 0x00000084 0x00000400    34%   0x00000010 000
mmcsd_de  22  suspend 0x0000008c 0x00000400    49%   0x00000013 000
tidle     31  ready   0x00000054 0x00000100    32%   0x0000001a 000
main      10  suspend 0x00000064 0x00000800    35%   0x00000010 000
msh />
```

### 3.4, packet capture test

> Use [ping](https://github.com/RT-Thread-packages/netutils/blob/master/ping/README.md) command to perform packet capture test, `ping` command needs to be enabled in menuconfig configuration, Details are as follows:

```
  RT-Thread online packages --->
      IOT internet of things --->
          [*] Enable Ping utility
```
After saving the menuconfig configuration, use the `pkgs --update` command to download the package

#### 3.4.1, ping domain name

- Enter `ping rt-thread.org` in msh />, the details are as follows:

```
msh />ping rt-thread.org
60 bytes from 116.62.244.242 icmp_seq=0 ttl=49 time=11 ticks
60 bytes from 116.62.244.242 icmp_seq=1 ttl=49 time=10 ticks
60 bytes from 116.62.244.242 icmp_seq=2 ttl=49 time=12 ticks
60 bytes from 116.62.244.242 icmp_seq=3 ttl=49 time=10 ticks
msh />
```

#### 3.4.2, ping IP

- Enter `ping 192.168.1.121` in msh />, the details are as follows:

```
msh />ping 192.168.1.121
60 bytes from 192.168.10.121 icmp_seq=0 ttl=64 time=5 ticks
60 bytes from 192.168.10.121 icmp_seq=1 ttl=64 time=1 ticks
60 bytes from 192.168.10.121 icmp_seq=2 ttl=64 time=2 ticks
60 bytes from 192.168.10.121 icmp_seq=3 ttl=64 time=3 ticks
msh />
```

### 3.5, stop capturing packets

- In msh />, enter `tcpdump -p`, the details are as follows:

```
msh />tcpdump -p
[TCPDUMP]tcpdump stop and tcpdump thread exit!
msh />
```

### 3.6, view the results

- In msh />, enter `ls` to view the saved results, the details are as follows:

```
msh />ls
Directory /:
System Volume Information<DIR>
text.pcap 1012
msh />
```

### 3.7, post-capture processing

Use a card reader to copy the xx.pcap file saved in the sd-card to the PC, and use the packet capture software wireshark to directly analyze the network flow

## 4. Import the captured file into the PC through the rdb tool

### 4.1, open packet capture

- In msh />, enter `tcpdump -ie0 -mrdb -wtext.pcap`, the details are as follows:

```
msh />tcpdump -ie0 -mrdb -wtext.pcap
[TCPDUMP]select [e0] network card device
[TCPDUMP]select [rdb] mode
[TCPDUMP]save in [text.pcap]
[TCPDUMP]tcpdump start!
msh />
```

### 4.2, packet capture test

- Please refer to 3.4 operation

### 4.3, stop capturing packets

- In msh />, enter `tcpdump -p`, the details are as follows:

```
msh />tcpdump -p
[TCPDUMP]tcpdump stop and tcpdump thread exit!
msh />
```

### 4.4, view results

- In msh />, enter `ls` to view the saved results, the details are as follows:

```
msh />ls
Directory /:
System Volume Information<DIR>
text.pcap 1012
msh />
```

### 4.5, post-capture processing

Use the rdb tool to import the xx.pcap file to the PC, and use the packet capture software wireshark to directly analyze the network flow

## 5. Matters needing attention

- The tcpdump tool needs to open the sending and receiving threads of lwip
- The packet capture is over or you don’t want to capture the packet anymore, please enter `tcpdump -p` to end the packet capture

## 6. Contact & Thanks

* Thanks: [liu2guang](https://github.com/liu2guang) made the optprase package
* Thanks: [uestczyh222](https://github.com/uestczyh222) for making rdb tool & rdb host computer
* Maintenance: [never](https://github.com/neverxie)
* Homepage: https://github.com/RT-Thread-packages/netutils
