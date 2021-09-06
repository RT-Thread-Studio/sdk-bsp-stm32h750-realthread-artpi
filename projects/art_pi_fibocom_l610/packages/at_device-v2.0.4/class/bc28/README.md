# BC28

## 注意事项

### 申请 socket

- BC28 最多支持 7 个 socket，编号 0-6；
- 如果 BC28 使用支持 MQTT 或 CoAP 协议的固件，则相关服务会占用部分 socket，因此最小可用的 socket 并不一定是 0；
- 可以创建 1 个 UDP socket，多个 TCP socket；
- 另外，执行 DNS 域名解析时也会占用 socket；

### 连接 socket

- R02 的基线版本固件才支持 `AT+QTCPIND` 查询 TCP 连接情况，因此目前采用保守的 30 秒延时来确保连接状态；

