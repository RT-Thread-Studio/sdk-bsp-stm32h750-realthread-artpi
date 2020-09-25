## RT-Thread Studio 工程使用说明

> 本项目为了测试，使用 windows 的**符号链接模式**来关联项目目录（BSP_ROOT）外的文件夹，可能在 git 管理或跨 PC 访问时存在问题，建议使用下面的步骤修复工程

### STEP1 新建符号链接

进入当前文件夹下，以管理员权限打开 windows 命令行 ，执行 mklink 命令，分别为 `rt-thread` 及 `libraries` 文件创建符号链接，成功后有如下的日志提示

```
E:\program\RTT\art-pi-sdk\projects\eclipse>mklink /D rt-thread ..\..\rt-thread
为 rt-thread <<===>> ..\..\rt-thread 创建的符号链接

E:\program\RTT\art-pi-sdk\projects\eclipse>mklink /D libraries ..\..\libraries
为 libraries <<===>> ..\..\libraries 创建的符号链接

E:\program\RTT\art-pi-sdk\projects\eclipse>
```

### STEP2 更新 Eclipse 工程配置

对于符号链接的项目，需要在使用前执行如下命令，重新更新 eclipse 工程配置（主要是一些绝对路径地址）

```
scons --target=eclipse
```

### STEP3 导入 RT-Studio

选择导入->Studio 项目->项目工程路径即可

