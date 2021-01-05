#  ART-Pi BT_WiFi 模块固件下载

ART-Pi 板卡在出厂时已经烧录过 BT_WiFi 固件，存储在外部 Flash。如果固件被不慎擦除，会导致 BT_WiFi 模组的功能无法正常使用，出现异常情况，需要重新烧录 BT_WiFi 固件。RT-Thread 提供的解决方式：先烧录出厂例程固件 `factory`，然后选择使用 Ymodem 下载固件。

## 烧录出厂例程

[TODO]

## 下载 BT_WIFI 固件

wifi 和蓝牙固件的下载方式相同，可以通过终端进行 ymodem 传输。固件路径位于 `/tools/firmware/` 目录下。其中 `ap6212-bt-image-1.0.rbl` 为蓝牙固件，`ap6212-wifi-image-1.0.rbl` 为 WIFI 固件。以 WIFI 固件上传为例，需要执行两个步骤：

1. 在终端命令行中依次执行如下命令去格式化名为 `wifi_image` 的 fal 分区：
    ```
    fal probe wifi_image
    fal erase 0 0x80000
    ```
    ![FAL_ERASE_CMD](./figures/fal_erase_cmd.png)
2. 在终端命令行中执行 `update` 命令。
3. 使用 ymodem 进行文件传输；需要注意，终端需要支持 ymodem 协议，推荐 Xshell。

完成后系统会自动识别传入的固件，若固件有效则会自动搬运到对应的固件存储区。

![WIFI-OTA](./figures/wifi_ota.gif)