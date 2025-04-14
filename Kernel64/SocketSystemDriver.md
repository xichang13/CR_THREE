
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [网络系统驱动](#网络系统驱动)

<!-- /code_chunk_output -->


# 网络系统驱动

* Ws2_32.sendto => WSP => TDI(.sys) => NDIS(.sys) => 网卡驱动 => 网卡

* 在 `TDI` 层可以拦截到网络包和进程信息，但是可能会被绕过，直接和 NDIS 通信
* 在 `NDIS` 层可以拦截到网络包，但是无法获取进程信息

```
TCP/IP
应用层
传输层 TDI tcp tcp6 udp udp6
网络层 NDIS tcpip.sys ip ip6
物理层
```

* NDIS 一般指网络驱动程序接口规范。 网络驱动程序接口规范 （Network Driver Interface Specification(NDIS)）

如何实现网络监控：
1. TDI过滤驱动
2. NDIS中间驱动
3. WFP微过滤驱动 微软提供的网络监控框架 Win7以上
4. WSK Windows Sockets Kernel 内核级套接字

实现产品：
1. 防火墙
2. 代理
3. 抓包工具
4. 流量控制工具