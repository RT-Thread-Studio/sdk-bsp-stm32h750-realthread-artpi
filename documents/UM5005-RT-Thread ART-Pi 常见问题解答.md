# ART-Pi 常见问题解答

### 在您遇到问题时，请先确认，RTT Studio  版本在 V1.1.5 以上版本，ART-Pi-SDK 在 V1.0.1 以上版本，ST-LINK Debugger 已安装 V1.4.0 以上

### Q1:  设备管理显示 ST-LINK 带感叹号，且 ST-LINK 红灯常亮 

![stlink1](../documents/figures/stlink1.png)

### A: 需要更新 ST-LINK 驱动，[下载地址](https://www.st.com/zh/development-tools/stsw-link009.html)

### Q2: 设备管理器能看到 ST-LINK 虚拟串口，但是通用串行设备看不到 ST-LINK
![stlink2](../documents/figures/stlink2.png)

### A: 查看我的电脑是否有一个 ART-Pi 的U盘图标，如果有请参考上一条重新安装驱动，如果还不行，请卸载掉原来的 ST-LINK 驱动后再重新安装一次

### Q3：RTT Studio 提示找不到 ST-LINK.exe 的路径

![img](../documents/figures/rttstudio1.png)

### A: 先确保已经通过 RTT Studio 资源管理器 下载了1.4版本的 ST-LINK，然后修改工程配置中ST-LINK版本为 V1.4

![img](../documents/figures/rttstudio2.png)

### Q4：为什么无法 debug 程序/为什么程序下载进去了不会运行

程序编译成功，烧录成功，就是不运行，好气啊

### A：重新烧录 bootloader

`STM32H750` 执行片外 `QSPI FLASH` 上的程序需要有一个 `bootloader` 来跳转过去，出厂前已经默认烧录了 `bootloader`，所以可以运行，如果不小心擦除了，请重新烧录 `bootloader`。`bootloader` 烧录成功之后会打印 logo

![bootlogo](../documents/figures/bootlogo.png)

> 在使用Keil之前先了 `stm32Programmer` 烧录了一次，将里面的`bootloader` 直接擦除了，需要重新烧录一遍

解决：

1. 进入到 `sdk-bsp-stm32h750-realthread-artpi-master\projects\art_pi_bootloader` 路径下

2. 使用 [ENV](https://club.rt-thread.org/ask/question/5699.html) 工具执行 mklink 命令，分别为 `rt-thread` 及 `libraries` 文件创建符号链接

   ```shell
   E:\project\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_blink_led>mklink /D rt-thread ..\..\rt-thread
   symbolic link created for rt-thread <<===>> ..\..\rt-thread
   
   E:\project\sdk-bsp-stm32h750-realthread-artpi\projects>mklink /D libraries ..\..\libraries
   symbolic link created for libraries <<===>> ..\..\libraries
   ```

   ---

   如若上述失败，

   备选方案（**不建议**）：

   拷贝 `../../rt-thread` 和 `../../libraries` 到当前目录

3. 使用 [ENV](https://club.rt-thread.org/ask/question/5699.html) 工具执行 `scons --target=mdk5`

4. 打开 mdk5 工程，

5. 修改

   ![](https://gitee.com/lebhoryi/PicGoPictureBed/raw/master/img/20201204141257.png)

6. 编译 烧录

   ![](https://gitee.com/lebhoryi/PicGoPictureBed/raw/master/img/20201204104643.png)

### Q5: RTT Studio 报错

![image](../documents/figures/rttstudio3.png)

### A： 请使用 RTT Studio V1.1.5 以上版本，ART-Pi-SDK 请使用 V1.0.1以上版本

### Q6: 提示请不要同时勾选多个下载

![image](../documents/figures/rttstudio4.png)

### A： STM32H7 目录下有2个版本的资源包，请打开下拉框取消其中一个勾选

### Q7：使用 MDK 编译成功，但是下载程序失败

错误界面：

![](https://gitee.com/lebhoryi/PicGoPictureBed/raw/master/img/20201204102416.png)

![img](https://git.rt-thread.com/research/edge-ai/uploads/708b22002df98027e6ff586ecf005059/image.png)

### A: 未添加下载算法导致

拷贝 `sdk-bsp-stm32h750-realthread-artpi\debug\flm\ART-Pi_W25Q64.FLM` 到 `<MDK安装目录>\ARM\Flash` 文件夹下

同时，`RAM for Algorithm` 需要调整成 `0x4000`

![image-20201204141924972](https://gitee.com/lebhoryi/PicGoPictureBed/raw/master/img/20201204141925.png)

即可烧录成功

### Q8：使用 RTT Studio 编译示例工程失败
![rttstudio_error1](../documents/figures/rttstudio_error1.png)

### A:  请使用 RTT Studio 安装离线资源包，然后基于开发板创建方式创建示例工程。如何使用请参考  **UM5002-RT-Thread ART-Pi 开发手册.md**

