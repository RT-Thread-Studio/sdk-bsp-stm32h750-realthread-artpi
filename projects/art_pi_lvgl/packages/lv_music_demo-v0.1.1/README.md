# Music Player Demo for RT-Thread

# LVGL音乐播放器演示示例（RT-Thread定制版）

## Overview
The music player demo shows what kind of modern, smartphone-like user interfaces can be created on LVGL. It works the best with display with 480x272 or 272x480 resolution. 


![Music player demo with LVGL embedded GUI library](screenshot1.gif)

## 如何使用这个Demo

- 在`lv_conf.h`中增加宏定义`#define LV_USE_DEMO_RTT_MUSIC 1 `

- 如果想要自动播放的话，可以增加宏定义`#define LV_DEMO_RTT_MUSIC_AUTO_PLAY 1`

- 如果想要一直自动播放下去（默认是自动播放40秒，显示平均FPS结果），需要增加宏定义`#define LV_DEMO_RTT_MUSIC_AUTO_PLAY_FOREVER 1`

- `lv_conf.h`文件涉及到本demo的宏定义配置示例：

  ```c
    /* music player demo */
    #include <rtconfig.h>
    #define LV_HOR_RES_MAX          BSP_LCD_WIDTH
    #define LV_VER_RES_MAX          BSP_LCD_HEIGHT
    #define LV_USE_DEMO_RTT_MUSIC       1
    #define LV_DEMO_RTT_MUSIC_AUTO_PLAY 1
    #define LV_FONT_MONTSERRAT_12       1
    #define LV_FONT_MONTSERRAT_16       1
  ```
  
- 调用`lv_demo_music()`函数：

  ```c
  static void lvgl_thread(void *parameter)
  {
      extern void lv_demo_music(void); //对外声明lv_demo_music()函数
      lv_demo_music(); //调用demo函数运行音乐播放器
  
      while(1)
      {
          lv_task_handler();
          rt_thread_mdelay(1);
      }
  }
  ```

## 参考工程

https://github.com/RT-Thread/rt-thread/tree/master/bsp/qemu-vexpress-a9/drivers/lvgl

注意：这是个模拟器BSP，因此没有`board`文件夹，LVGL相关配置源文件放在了`drivers`文件夹中。实际移植时，应当放在BSP目录下的`board/ports/lvgl`文件夹下。

硬件BSP实际位置参考：https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/board/ports/lvgl

## How the spectrum animation works

- `assets/spectrum.py` creates an array of spectrum values from a music. 4 band are created with 33 samples/sec: bass, bass-mid, mid, mid-treble.
- The spectrum meter UI does the followings:
	- Zoom the album cover proportionality to the current bass value
	- Display the 4 bands on the left side of a circle by default at 0°, 45°, 90°, 135° 
	- Add extra bars next to the "main bars" with a cosine shape. Add more bars for the lower bands.
	- If the there is a large enough bass add a random offset to the position of the bars. E.g. start from 63° istead of 0°. (bars greater than 180° start again from 0°)
	- If there no bass add 1 to the offset of the bars (it creates a "walking" effect)
	- Mirror the bars to the right side of the circle
	
## Using spectrum.py
- install `librosa` with `pip3 install librosa`	
- run `python sectrum.py my_file.mp3`
- see the result in `spectrum.h`
