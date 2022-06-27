# NTP：网络时间协议

## 1 介绍

[NTP](https://baike.baidu.com/item/NTP) 是网络时间协议(Network Time Protocol)，它是用来同步网络中各个计算机时间的协议。

在 RT-Thread 上实现了 NTP 客户端，连接上网络后，可以获取当前 GMT 时间，并更新至 RTC 中。

## 2 使用

首先打开 meunconfig ，配置并开启 netutils 软件包。

### 2.1 启用 NTP 时间自动同步

如果 RT-Thread 已接入互联网，可启用 NTP 时间自动同步功能，定期同步本地时间。在 menuconfig 中启用 `NTP_USING_AUTO_SYNC` 配置。启用该功能后，NTP 会自动同步时间到 RTC。同时务必确保 RT-Thread 网络访问正常以及存在 RTC 设备。

启用该配置后，还有三个配置是用户可选配置：

- 在配置选项中，默认提供了 3 个 NTP 服务器，保证了 NTP 功能的可靠性。
- `NTP_AUTO_SYNC_FIRST_DELAY`: 首次执行 NTP 时间同步的延时。延时的目的在于，给网络连接预留一定的时间，尽量提高第一次执行 NTP 时间同步时的成功率。默认时间为 30S；
- `NTP_AUTO_SYNC_PERIOD`: NTP 自动同步周期，单位为秒，默认一小时（即 3600S）同步一次。

### 2.2 获取 GMT 时间

GMT格林威治时间，即UTC+0时间。

|函数                                    |返回值                                    |描述|
|:-----                                  |:----|:----|
|time_t ntp_get_time(void)                                  |`>0`: 当前 GMT 时间，`=0`：获取时间失败                                  ||


示例代码：

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

### 2.3 同步本地时间至 RTC

如果开启 RTC 设备，还可以使用下面的命令及 API 同步 NTP 的本地时间至 RTC 设备。

Finsh/MSH 命令效果如下：

```
msh />ntp_sync
Get local time from NTP server: Sat Feb 10 15:22:33 2018
The system time is updated. Timezone is 8.
msh />
```

|函数                                    |描述|
|:-----                                  |:----|
|time_t ntp_sync_to_rtc(void)                                  |`>0`: 当前本地时间，`=0`：同步时间失败|

## 3 注意事项

- 1、NTP API 方法执行时会占用较多的线程堆栈，使用时保证堆栈空间充足（≥1.5K）；
- 2、NTP API 方法 **不支持可重入** ，并发使用时，请注意加锁。