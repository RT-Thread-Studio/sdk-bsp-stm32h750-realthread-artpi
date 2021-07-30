# ART-Pi 开源项目

ART-Pi 开源项目如下表所示：

| **项目文件夹名称**                             | **项目实现功能**        |
| :---------------------------------------------------- | :------------------------------ |
| [art_pi_blink_led](./art_pi_blink_led)    | 实现 LED 闪烁功能,可作为二次开发的基础工程 |
| [art_pi_bootloader](./art_pi_bootloader) | 实现程序从 0x08000000 跳转到 0x90000000 |
| [art_pi_factory](./art_pi_factory) | 实现 webnet. ART-Pi 的出厂DEMO |
| [art_pi_gc0328c_camera](./art_pi_gc0328c_camera) | 实现 gc0328c 摄像头拍照，并显示在多媒体扩展板 |
| [art_pi_sensor485_app](./art_pi_sensor485_app) | 实现 串口模拟发送传感器数据，通过 MQTT 上传到手机上的 APP |
| [art_pi_wifi](./art_pi_wifi) | 实现 wifi 联网 |
| [industry_io_gateway](./industry_io_gateway) | 实现 modbustcp2rtu 及 ftp |
| [art_pi_net_player](./art_pi_net_player) | 实现 MP3 音乐播放，支持本地音乐和网易云音乐 |
| [lrs007_lora_radio](./lrs007_lora_radio) | 实现 LoRa Radio (SX126x\SX127x...) 单向/双向通信、抓包及shell示例 |
| [lrs007_lorawan_end_device](./lrs007_lorawan_end_device) | 实现 LoRaWAN End Device 协议栈Class A\C通信及shell示例 |
| [art_pi_qboot](./art_pi_qboot) | 实现程序从 `0x08000000` 跳转到 `0x90000000`,并且支持从 `download` 分区更新固件到 `app` 分区。 |
| [art_pi_ble_mesh_gateway](./art_pi_ble_mesh_gateway) | 实现蓝牙 mesh web 网关功能。 |
| [art_pi_nes](./art_pi_nes) | 实现 NES 模拟器及游戏试玩 |
| [uf2_boot]((./uf2_boot)) | 实现 USB 的 bootloader 的功能，使用的文件格式为 uf2 |
| [art_pi_kboot](./art_pi_kboot) | 实现烧录kernel/uboot/rootfs到qspi/spi flash，并跳转到qspi `0x90000000` 地址，运行uboot |
| [art_pi_fibocom_l610](./art_pi_fibocom_l610) | 实现 fibocom L610open 开机初始化 |
