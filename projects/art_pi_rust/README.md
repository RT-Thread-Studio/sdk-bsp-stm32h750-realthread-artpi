# RUST驱动与应用支持

## 简介

本例程提供了rust应用程序、驱动程序的支持。
应用程序需要额外进行编译。
代码仓库位于：[driver_for_rtt](https://github.com/vito-chl/driver_for_rtt.git)

## 软件说明

rust编译出来的程序为静态库，链接到本工程中进行编译。
静态库存放地址为board目录，名称为librust_rtt.a。

## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

正常运行后，驱动框架会注册驱动并打印log。
rust入口应用程序会自动运行。

## 注意事项

更多内容请参考仓库：[rtt_rs](https://github.com/vito-chl/rtt_rs.git)

