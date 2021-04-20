# ld3320

## 1 、介绍

​	这是一个 `RT-Thread` 的软件包，该软件包提供了 `LD3320` 模块的驱动。

​	`LD3320` 是由 `ICRoute`公司生产的语音识别芯片。

> 更多关于 ld3320 的信息请参阅`数据手册`

### 1.1 目录结构

| 名称 | 说明                     |
| :--- | :----------------------- |
| docs | 文档                     |
| demo | 有关使用该驱动的样例代码 |
| src  | 源代码目录               |
| inc  | 头文件目录               |

### 1.2 许可证

ld3320 package 遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread PIN 设备
- RT-Thread SPI 设备
- RT-Thread DFS （当使用`MP3`时需要）

## 2、获取软件包

使用 ld3320 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    peripheral libraries and drivers --->
        
```

选中后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 LD3320



## 4、注意事项

无

## 5、联系方式

- 维护： [xqyjlj@126.com](xqyjlj@126.com)
- 主页：[https://github.com/xqyjlj/ld3320](https://github.com/xqyjlj/ld3320)