# agile_console

## 1、介绍

- `agile_console` 是基于RT-Thread实现的灵活的console设备，支持多个通道输入和输出。

- 支持device接口和POSIX接口。

- 增加一个新的通道非常简单，只需要实现两个回调函数即可。

- 需要使用telnet的可以看我的另一个软件包 [agile_telnet](https://github.com/loogg/agile_telnet)，其最新版本作为该软件包的插件实现。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| example | 示例 |
| figures | 文档图片 |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

`agile_console` package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 3.0+

## 2、如何打开 agile_console

使用 `agile_console` package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    peripheral libraries and drivers  --->
         [*] agile_console: A agile console pachage.  --->
            (256) Set agile_console rx buffer size
            (acon) Set agile_console device name
            (19)  Set agile_console thread priority
            (2048) Set agile_console thread stack size
            [*]   Enable agile_console serial example
            (uart1) Set serial device name
                  Version (latest)  --->
```

- **Set agile_console rx buffer size**：`agile_console`接受环形缓冲区大小
- **Set agile_console device name**：`agile_console`设备名
- **Set agile_console thread priority**：`agile_console`线程优先级
- **Set agile_console thread stack size**：`agile_console`线程堆栈大小
- **Enable agile_console serial example**：使能串行接口例程
- **Set serial device name**：串行接口设备名

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 agile_console

在打开 `agile_console` package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

![agile_console_test](./figures/agile_console_test.gif)

如上面动图所示，我注册了串行接口和telnet两个通道，可以看到两个终端都能使用shell。

## 4、注意事项

- agile_console使能后是会自动注册为调试设备，是在`INIT_BOARD_EXPORT`层注册设备并更改为调试设备的。
- 配置`RT_CONSOLE_DEVICE_NAME`为一个不存在的设备名或在`rt_hw_board_init`中将`rt_console_set_device(RT_CONSOLE_DEVICE_NAME);`删除。


## 5、联系方式 & 感谢

- 维护：malongwei
- 主页：<https://github.com/loogg/agile_console>
- 邮箱：<2544047213@qq.com>
