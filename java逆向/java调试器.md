
# java调试器

> [调试器结构](https://docs.oracle.com/javase/8/docs/technotes/guides/jpda/architecture.html)

* JVM TI - Java VM Tool Interface
    * Java虚拟机工具接口，Java虚拟机提供的接口。
* JDWP - Java Debug Wire Protocol
    * Java调试协议，我们基于该协议实现调试器，该层使用TCP协议实现调试器。
* JDI - Java Debug Interface
    * Java调试接口，Java开发时使用。

> [JDWP](https://docs.oracle.com/javase/8/docs/technotes/guides/jpda/jdwp-spec.html)
> [协议ID](https://docs.oracle.com/javase/8/docs/platform/jpda/jdwp/jdwp-protocol.html)

**应用开始调试**
* `adb shell am start -D -n com.android.test/.MainActivity` - 以调试模式启动应用
* `adb shell ps | grep com.android.test` 查看应用的进程id
* `adb jdwp` 列出当前可以被调试的进程
* `adb forward tcp:8000 jdwp:com.android.test:5678` 转发端口，将应用端口转发到本地端口8000，用于连接虚拟机