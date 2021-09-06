# QBoot命令详述
---
#### QBoot的shell命令列表：

|命令|功能说明|
|---|---|
|`qboot probe`|探测和显示download分区和factory分区固件包，并显示固件包信息
|`qboot resume src_part`|从参数src_part指定的分区恢复固件到app分区，与release不同的是只有固件包的目标分区是app分区时才会被释放
|`qboot clone src_part dst_part`|从参数src_part指定的分区克隆固件包到参数dst_part指定的分区，如：克隆download到factory
|`qboot release part`|释放参数part指定分区中的固件包，与resume命令不同的是固件包释放的目标分区不受限制
|`qboot verify part`|校验参数part指定分区的代码完整性，分区必须是固件包释放的目标分区，如app分区
|`qboot jump`|跳到应用程序执行

