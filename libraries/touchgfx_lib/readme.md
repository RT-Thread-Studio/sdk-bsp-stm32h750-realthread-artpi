# 如何在ART-PI上创建TouchGFX工程

## 实验平台：
**硬件：**   RT-Thread官方ART-PI H750开发版，正点原子4.3寸RGBLCD屏（800*480）
**软件：**   TouchGFXDesigner v4.15和 STM32CubeMX V6.0.1，开发环境 MDK 或 RT-Thread Studio 1.1.5，env工具

![](https://img-blog.csdnimg.cn/20200915170735381.jpg)

## 代码下载：
[https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi)

## 移植TouchGFX到其他操作系统的原理
1. TouchGFX可以运行在带操作系统和不带操作系统的应用中，默认支持的操作系统为FreeRTOS，用户想要更换操作系统，只需要重新实现OSWrappers类，便可以切换不同的RTOS。
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200917142954604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxMDM5MDYx,size_16,color_FFFFFF,t_70#pic_center)

  

2. 在rtthread中添加touchgfx需要的驱动

​       根据touchgfx需要的组件构成，只需要在rtthread中实现相应的驱动即可。
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200918152442511.png)

## 在ART-PI中使用TouchGFX
为了方便广大ART-PI的用户使用TouchGFX，免去移植的烦恼，官方已经在ART-PI的SDK中集成了TouchGFX软件库，只需要简单的配置，便可以轻松使用TouchGFX。
### 使用RT-Thread Studio 开发
1. 打开 RT-Thread Studio 的包管理器,安装 ART-PI SDK 资源包

  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023172827389.png)

2. 安装完成后 选择基于 开发板 创建工程

     ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023173348688.png)

  

3. 打开TouchGFX Library
    
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023174030228.png)

  

4. 打开DMA2D和CRC外设
    
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023180140276.png)

  

5. 配置C++编译规则
    
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024113355777.png)

  

6. 添加GT9174触摸软件包
    
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023185917918.png)

      ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201023190054938.png)
    
7. 编译下载


### 使用 MDK 开发
1. 在 [ART-Pi SDK仓库](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi) 下载 SDK
2. 进入工程目录。如：sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_blink_led
3. 以管理员权限打开 windows 命令行 ，执行 mklink 命令，分别为 `rt-thread` 及 `libraries` 文件创建符号链接
```c
E:\project\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_blink_led>mklink /D rt-thread ..\..\rt-thread
为 rt-thread <<===>> ..\..\rt-thread 创建的符号链接

E:\project\sdk-bsp-stm32h750-realthread-artpi\projects>mklink /D libraries ..\..\libraries
为 libraries <<===>> ..\..\libraries 创建的符号链接

E:\project\sdk-bsp-stm32h750-realthread-artpi\projects>
```
4. 打开TouchGFX Library
    
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024120035558.png)
    
5. 添加GT9174触摸软件包

     ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024120433818.png)

6. 使用 [ENV](https://club.rt-thread.org/ask/question/5699.html) 工具执行 scons --target=mdk5 

7. 打开DMA2D和CRC外设

  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024120132856.png)

8. 配置MDK

  将SDK中ART-Pi_W25Q64.FLM复制到MDK安装目录的Keil_v5\ARM\Flash下。
  复制后会自动识别出下载算法，

  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024115922895.png)

  

  不使用Micro LIB库。

  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024115628218.png)

9. 编译下载
### UI开发及仿真
**使用TouchGFX 4.15.0 Designer 仿真**

在libraries文件夹下找到TouchGFX，打开art_pi.touchgfx

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024121400784.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024121801586.gif#pic_center)



**使用Visual Studio 仿真**

在TouchGFX文件夹下找到simulator，打开Application.sln
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024122143622.png)

在TouchGFX Designer设计完UI，刷新工程，即可将UI更新到项目中。

### 注意事项
**1. 如何更换屏幕**

ART-PI的LCD驱动默认支持RGB接口，更换屏幕后，只需要修改对应的参数即可。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024122931995.png)

用文本文档打开ApplicationTemplate.touchgfx.part，调整分辨率

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024123546325.png)

修改好，双击打开，重新生成UI工程。

**2. 如何更改色深**

默认支持RGB888 24位色深和RGB565 16位色深，只需要修改LCD_BITS_PER_PIXEL和LCD_PIXEL_FORMAT这两个宏，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024123108553.png)

用文本文档打开ApplicationTemplate.touchgfx.part，调整色深

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024123500952.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201024123658354.png)



修改好，双击打开，重新生成UI工程。

**3. C语言和C++如何互相调用**

 c方式编译和c++方式编译，其差异就在于符号表标识符。同一个函数名，在c方式编译的其函数名跟编译前的函数一致，c++方式编译的则是以函数名结合参数作为编译后的函数名。cpp编译器是兼容c语言的编译方式的，所以在编译cpp文件的时候，调用到.c文件的函数的地方时，需要用extern “C”指定用c语言的方式去编译它，extern “C”是c++方式编译才认识的关键字，在c++编译的方式会定义 \__cplusplus 宏，c语言编译器没有 __cplusplus宏和extern “C”关键字。

**C++语言调用C**:

在编译cpp文件的时候，调用到.c文件的函数的地方时，需要用extern “C”指定用c语言的方式去编译它

```c
extern "C"
{
	uint32_t LCD_GetXSize();
	uint32_t LCD_GetYSize();
}
```
C的头文件要使用宏定义包含函数接口。

```c
#ifdef __cplusplus
 extern "C" {
#endif
.
.
.
#ifdef __cplusplus
}
#endif
```
**C语言调用C++文件中的函数:**

对于C++文件中的普通函数，可以直接用extern "C" 对函数进行修饰，对于c++的重载函数或成员函数，需要进行二次封装后，再使用extern "C"对函数修饰，函数内部的代码依然是使用c++的编译器编译。
例如在xx.cpp中对函数进行声明，在xx.c中进行调用：
xx.cpp

```c
extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();
void touchgfx_init()
{

}
void touchgfx_taskEntry()
{

}
```
xx.c

```c
void touchgfx_init(void);
void touchgfx_taskEntry(void);

void MX_TouchGFX_Init(void)
{

  touchgfx_init();
}

void MX_TouchGFX_Process(void)
{
  touchgfx_taskEntry();
}
```
**4. 建议的开发方式**

建议使用touchGFX Designer配合Visual Stdudio来开发UI，与硬件解耦，采用前后端分离的开发方式，可以提高开发效率。
代码中可以使用宏来规避开发环境不同导致的错误：

```c
例如通过宏：
#ifdef SIMULATOR
/*使用VS仿真的代码*/
#else
/*使用HAL库的代码*/
#endif
```

详细教程：[在ART-Pi H750上移植TouchGFX（三）——如何在ART-PI上创建TouchGFX工程](https://blog.csdn.net/sinat_31039061/article/details/108638097)



关注公众号，后续有精彩内容会第一时间发送给您！

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200506150500150.jpg)