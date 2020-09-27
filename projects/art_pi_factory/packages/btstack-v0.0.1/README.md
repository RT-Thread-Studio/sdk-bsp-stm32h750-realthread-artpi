# Welcome to RTT_BTstack

1. 基于原版btstack适配RTT

2. 该软件包不能**商用**，如果需要**商用**需要联系BTSTACK获取license，本软件包作者未被授权商用，不负责任商用后的责任。

3. 需要把文件BCM43430A1.hcd放到文件系统中去。

4. 程序烧入之后，输入命令bt_stack_port_main可以启动蓝牙

5. 看到如下的log，说明蓝牙已经起来了

   ![image-20200829060523161](doc\figure\readme_1.jpg)

   6.此时用手机nrf connect软件可以搜索到该设备。

   7.连上该设备之后，打开notification，可以看到gatt_counter计数器在增加。

   ![image-20200829060523161](doc\figure\readme_2.png)

