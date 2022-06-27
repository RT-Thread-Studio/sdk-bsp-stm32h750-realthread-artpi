# RT-Thread Network Gadgets Collection

[Chinese](README_ZH.md) | English

## 1 Introduction

When RT-Thread is connected to the network, the playability is greatly enhanced. Here is a collection of all the web widgets available for RT-Thread, and all the widgets you need can be found here.

## 2. How to obtain

Please use ENV tool to assist download:

The path of the package is: `RT-Thread online package` -> `IoT-internet of things` -> `netutils`

## 3. Instructions for use

Each gadget can be enabled/disabled independently using menuconfig and provides commands for using Finsh/MSH. There is a detailed usage document in its catalog. If you need to use it, please check separately. The following is a summary of currently supported gadgets:

| Name | Classification | Function Introduction | Use Document |
| :--------------------------- | :------: | :--------------------------------------------------------- | :---------------------------- |
| [Ping](ping/README-en.md) | Debugging test | Use the "ping" command to check whether the network is connected, which can help us analyze and determine network failures | [click to view](ping/README-en.md) |
| [TFTP](tftp/README-en.md) | File transfer | TFTP is a simple protocol for transferring files, which is lighter than FTP | [click to view](tftp/README-en.md) |
| [iperf](iperf/README-en.md) | Performance Test | Test maximum TCP and UDP bandwidth performance, report bandwidth, delay jitter and packet loss | [Click to view](iperf/README-en.md) |
| [NetIO](netio/README-en.md) | Performance Test | Tools for testing network throughput | [Click to view](netio/README-en.md) |
| [NTP](ntp/README-en.md) | Time synchronization | Network time protocol, support 3 alternative servers | [Click to view](ntp/README-en.md) |
| [Telnet](telnet/README-en.md) | Remote access | Can remotely log in to RT-Thread's Finsh/MSH Shell | [Click to view](telnet/README-en.md) |
| [tcpdump](tcpdump/README-en.md) | Network debugging | tcpdump is RT-Thread's lwip-based network packet capture tool | [Click to view](tcpdump/README-en.md) |