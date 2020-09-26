# agile_telnet

## 1、介绍

- 从2.0开始`agile_telnet`作为[agile_console](https://github.com/loogg/agile_console)的插件使用。

- 如果只想使用telnet作为console设备，请访问[v1.0.0](https://github.com/loogg/agile_telnet/tree/v1.0.0)。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

`agile_telnet` package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 3.0+
- SAL
- POSIX
- DFS
- agile_console

## 2、如何打开 agile_telnet

使用 `agile_telnet` package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    IoT - internet of things  --->
        [*] agile_telnet: TCP debug for Ethernet  --->
            (2048) Set agile_telnet thread stack size
            (26)  Set agile_telnet thread priority
            (23)  Set agile_telnet listen port
            (256) Set agile_telnet rx buffer size
            (2048) Set agile_telnet tx buffer size
            (3)   Set agile_telnet client timeout(min)
                  Version (latest)  --->
```

- **Set agile_telnet thread stack size**：`agile_telnet`线程堆栈大小
- **Set agile_telnet thread priority**：`agile_telnet`线程优先级
- **Set agile_telnet listen port**：`agile_telnet`监听端口号
- **Set agile_telnet rx buffer size**：`agile_telnet`接收环形缓冲区大小
- **Set agile_telnet tx buffer size**：`agile_telnet`发送环形缓冲区大小
- **Set agile_telnet client timeout(min)**：`agile_telnet`客户端默认超时时间(3分钟)
- **Version**：大于`v1.0.0`版本会强制选中`agile_console`软件包，并作为其插件使用

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 agile_telnet

在打开 `agile_telnet` package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

该软件包作为[agile_console](https://github.com/loogg/agile_console)的插件使用。

## 4、注意事项

- 使用以太网设备请使能以太网设备和`lwip`，这些`agile_telnet`不会强制打开，只强制选中到`SAL`层。

## 5、联系方式 & 感谢

- 维护：malongwei
- 主页：<https://github.com/loogg/agile_telnet>
- 邮箱：<2544047213@qq.com>
