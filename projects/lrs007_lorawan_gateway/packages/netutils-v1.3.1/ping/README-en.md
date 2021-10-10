# Ping

## 1 Introduction

[ping](https://baike.baidu.com/item/ping/6235) is a network tool used to test whether data packets can reach a specific host through the IP protocol. Estimate the packet loss rate (packet loss rate) and round-trip time (network delay, Round-trip delay time) between the host and the host.

## 2. Use

Ping supports access to `IP address` or `domain name`, use Finsh/MSH command to test, the general effect is as follows:

### 2.1 Ping domain name

```
msh />ping rt-thread.org
60 bytes from 116.62.244.242 icmp_seq=0 ttl=49 time=11 ms
60 bytes from 116.62.244.242 icmp_seq=1 ttl=49 time=10 ms
60 bytes from 116.62.244.242 icmp_seq=2 ttl=49 time=12 ms
60 bytes from 116.62.244.242 icmp_seq=3 ttl=49 time=10 ms
msh />
```

### 2.2 Ping IP

```
msh />ping 192.168.10.12
60 bytes from 192.168.10.12 icmp_seq=0 ttl=64 time=5 ms
60 bytes from 192.168.10.12 icmp_seq=1 ttl=64 time=1 ms
60 bytes from 192.168.10.12 icmp_seq=2 ttl=64 time=2 ms
60 bytes from 192.168.10.12 icmp_seq=3 ttl=64 time=3 ms
msh />

```

## 3. Common problems

-The ping function support is exported by default in the netdev component. If you use the netdev component included and enabled in the system, this configuration option will not be displayed in the ENV.