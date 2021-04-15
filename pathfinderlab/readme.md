# RT-Thread ART-Pi 探路者实验室

## 背景

RT-Thread ART-Pi 探路者实验室，即一部分程序无法立即投入到RT-Thread master分支中，现在ART-Pi工程中做预先研究、先导验证和技术孵化，待时机成熟，再将成果并入上游RT-Thread master分支中。



## 项目1 - 新版RTC驱动框架

负责：[Meco Man](https://github.com/mysterywolf)

由于历史原因，目前RT-Thread使用的RTC驱动框架，并没有实际起到驱动框架的作用 - 即隔离底层驱动，抽象设备的功能。导致用户在调用RTC设备相关操作时，直接越过了RTC驱动框架，直接调用的底层驱动的程序。此外，在底层驱动中，框架功能和驱动功能相互交织在一起，没有剥离。因此应当予以重新实现该框架。

但是，由于涉及到大量BSP的改动，无法立即投入到master中，因此先放在实验室中孵化。