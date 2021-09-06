# 简要说明

1. 本示例提供的功能是一个 boot 程序, 它可以虚拟一个 U 盘 (**按住板子上的 USER 按键再上电**) ,

   用户可以将打包成 uf2 格式的应用程序直接复制到虚拟出来的 U 盘中, 从而达到刷固件的目的.

2. 虚拟出来的 U 盘根目录包含四个文件: **CODE.BIN** 固件映射, **DATA.BIN** 数据映射(功能代码已添加), **INFO_UF2TXT** 记录了板子的基本信息, **INDEX.HTM** 打开此文件会直接跳转到 ART-Pi 相关网页.

3. 本示例时钟配置请参考 main.c 相关部分 (**以此示例作为 boot 时编写的 app 应用时钟要和本示例匹配**), 务必注意, 以免无法正确运行.

4. tools 目录下的 uf2tool 用于将纯二进制程序打包成 uf2 格式.

5. uf2 格式参考 https://github.com/microsoft/uf2

   源代码中的 uf2.c uf2.h 参考了 https://github.com/adafruit/tinyuf2



维护: lizimu2020