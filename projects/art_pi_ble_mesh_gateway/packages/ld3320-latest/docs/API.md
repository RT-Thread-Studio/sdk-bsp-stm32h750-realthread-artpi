# API 手册

## 重要结构体说明

### ld3320_port

```c
struct ld3320_port
{
    int wr_pin;
    int irq_pin;
    int rst_pin;
};
```

作用：配置`LD3320`的端口。

详细：

- `wr_pin`：`LD3320`的读写控制端口，如果不需要的话，可以设置为`LD3320_PIN_NONE`，但是在硬件上该引脚需要接地。
- `irq_pin`：`LD3320`的中断触发端口，用来接收芯片的中断信号，该端口一定需要配置。
- `rst_pin`：`LD3320`的复位端口，用来硬件复位单片机，建议配置该端口，以防该芯片因为电压波纹或者其他问题死机而无法响应。如果不需要的话，也可以设置为`LD3320_PIN_NONE`。

### asr_over_callback_t

```c
typedef void (*asr_over_callback_t)(uint8_t num);
```

作用：`LD3320`语音识别成功后的回调函数

详细：
- `num`：语音对应的编号。

### ld3320_obj

  ```c
  struct ld3320_obj
  {
      struct ld3320_port port;
      struct rt_spi_device *dev;
      void (*asr_over_callback_t)(uint8_t num); /*callback */
  #ifdef LD3320_USING_MP3
      char mp3_path[30];
      uint32_t mp3_size;
      uint32_t mp3_pos;
  #endif
  };
  ```

作用：`LD3320`设备的对象

详细：

- `port`：`LD3320`的端口。
- `*dev`：`LD3320`对应的SPI设备。
- `asr_over_callback_t`：`LD3320`语音识别成功后的回调函数。
- `mp3_path[30]`：`mp3`文件的路径。
- `mp3_size`：`mp3`文件的大小。
- `mp3_pos`：正在播放`mp3`文件的偏移量。

### ld3320

```c
struct ld3320
{
    struct ld3320_obj obj;
    rt_list_t node;
};
typedef struct ld3320 *ld3320_t;
```

作用：`LD3320`设备（实质是链表结构的表头）

详细：

- `obj`：`LD3320`设备对象
- `node`：`LD3320`链表节点（链表头）

### ld3320_command

```c
struct ld3320_command
{
    char name[LD3320_MAX_COMMAND_LEN];
    uint8_t num;
    rt_list_t list;
};
typedef struct ld3320_command *ld3320_command_t;
```

作用：`LD3320`命令链表

详细：

- `name`：`LD3320`语音命令
- `num`：`LD3320`语音对应的节点
- `list`：`LD3320`链表节点（子节点）

## 重要函数说明

### ld3320_create

```c
ld3320_t ld3320_create(char *spi_dev_name, int wr_pin, int rst_pin, int irq_pin, uint8_t mode);
```

功能：创建并初始化`LD3320`对象

参数：

- `*spi_dev_name`： `spi`设备名。

- `wr_pin`：`LD3320`的读写控制端口，如果不需要的话，可以设置为`LD3320_PIN_NONE`，但是在硬件上该引脚需要接地。

- `rst_pin`：`LD3320`的复位端口，用来硬件复位单片机，建议配置该端口，以防该芯片因为电压波纹或者其他问题死机而无法响应。如果不需要的话，也可以设置为`LD3320_PIN_NONE`。

- `irq_pin`：`LD3320`的中断触发端口，用来接收芯片的中断信号，该端口一定需要配置。

- `mode`：`LD3320`启动模式，有`LD3320_MODE_ASR`和`LD3320_MODE_MP3`两种选项可供选择。

返回：`ld3320`句柄。

### ld3320_run

```c
void ld3320_run(ld3320_t ld3320, uint8_t mode);
```

功能：`LD3320`运行函数，需要将其放置到循环中反复运行。

参数：

- `ld3320`：句柄。
- `mode`：`LD3320`运行模式，有`LD3320_MODE_ASR`和`LD3320_MODE_MP3`两种选项可供选择。

###  ld3320_hw_rst

```c
void ld3320_hw_rst(ld3320_t ops);
```

功能：`LD3320`硬件复位。

参数：

- `ops`：句柄。

###  ld3320_asr_start

```c
uint8_t ld3320_asr_start(ld3320_t ops);
```

功能：`LD3320`启动ASR模式。

参数：

- `ops`：句柄。

### ld3320_set_asr_over_callback

```c
void ld3320_set_asr_over_callback(ld3320_t ops, asr_over_callback_t callback);
```

功能：`LD3320`设置ASR识别成功后的回调函数。

参数：

- `ops`：句柄。
- `callback`：回调函数。

### ld3320_addcommand_tolist

```c
void ld3320_addcommand_tolist(ld3320_t ops, char *pass, int num);
```

功能：`LD3320`添加命令语句进链表。

参数：

- `ops`：句柄。
- `pass`：命令语句。
- `num`：命令语句对应的编号。

### ld3320_addcommand_fromlist

```c
void ld3320_addcommand_fromlist(ld3320_t ops);
```

功能：`LD3320`将链表中的命令语句添加进`LD3320`。

参数：

- `ops`：句柄。

### ld3320_mp3_start

```c
void ld3320_mp3_start(ld3320_t ops);
```

功能：`LD3320`启动mp3模式

参数：

- `ops`：句柄。

### ld3320_set_mp3_file_path

```c
void ld3320_set_mp3_file_path(ld3320_t ops, const char * mp3);
```

功能：`LD3320`设置mp3文件路径

参数：

- `ops`：句柄。
- `mp3`：`mp3`文件的路径。