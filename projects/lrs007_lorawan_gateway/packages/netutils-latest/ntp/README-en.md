# NTP: Network Time Protocol

## 1 Introduction

[NTP](https://baike.baidu.com/item/NTP) is the Network Time Protocol, which is a protocol used to synchronize the time of each computer in the network.

An NTP client is implemented on RT-Thread. After connecting to the network, the current UTC time can be obtained and updated to RTC.

## 2. Use

First open meunconfig, configure and start the netutils package. In the configuration options, 3 NTP servers are provided by default to ensure the reliability of the NTP function.

### 2.1 Get UTC time

[UTC time](https://baike.baidu.com/item/%E5%8D%8F%E8%B0%83%E4%B8%96%E7%95%8C%E6%97%B6/787659?fromtitle=UTC&fromid=5899996) Also known as Universal Time, Universal Standard Time, International Coordinated Time. Beijing time is UTC+8 time, which is 8 hours longer than UTC time, or understood as 8 hours earlier.

API: `time_t ntp_get_time(void)`

|Parameter |Description|
|:----- |:----|
|return |`>0`: current UTC time, `=0`: failed to obtain time|


Sample code:

```C
#include <ntp.h>

void main(void)
{
    time_t cur_time;

    cur_time = ntp_get_time();
    
    if (cur_time)
    {
        rt_kprintf("NTP Server Time: %s", ctime((const time_t*) &cur_time));
    }
}
```

### 2.2 Get local time

Local time is more than UTC time with the concept of time zone. For example, Beijing time is Dongba District, which is 8 hours longer than UTC time.

The current time zone can be set in `menuconfig`, the default is `8`

API: `time_t ntp_get_local_time(void)`

|Parameter |Description|
|:----- |:----|
|return |`>0`: current local time, `=0`: failed to get time|

The API usage method is similar to `ntp_get_time()`

### 2.3 Sync local time to RTC

If the RTC device is turned on, you can also use the following command and API to synchronize the local time of NTP to the RTC device.

The effects of Finsh/MSH commands are as follows:

```
msh />ntp_sync
Get local time from NTP server: Sat Feb 10 15:22:33 2018
The system time is updated. Timezone is 8.
msh />
```

API: `time_t ntp_sync_to_rtc(void)`

|Parameter |Description|
|:----- |:----|
|return |`>0`: current local time, `=0`: time synchronization failed|

## 3. Matters needing attention

- 1. When the NTP API method is executed, it will take up a lot of thread stacks. Make sure that the stack space is sufficient (≥1.5K) when using it;
- 2. NTP API method **Does not support reentrancy**, please pay attention to lock when used concurrently.