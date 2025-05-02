
# 静态过PG

* 反汇编`ntoskrnl.exe`最大的函数，就是PG的初始化代码。
* 对PG的初始化代码和`winload.exe`代码打补丁
* 添加启动项，重载到我们打过补丁的WinLoad和Ntoskrnl启动

# 动态过PG

* PG运行时代码超过0x8000，并且代码在堆里面运行，并且堆具有执行权限
1. 遍历内存池，找到内存超过0x8000的内存块，修改执行位为不可执行
2. PG代码运行时会产生异常，我们通过异常处理函数，找到异常地址，该地址就是PG的入口

# VT（Virtualization Technology）虚拟化技术

> 让硬件模拟各种系统功能，让机器可以运行多个系统

* 由于系统在ring0运行，无法被打断，所以硬件提供了ring-1级别的中断，用来打断ring0级别的指令。
* 每一核只能运行一台虚拟机。

![Interaction of a Virtual-Machine Monitor and Guests](pic/过PG/image.png)

* VMX指令
* VMCS（Virtual Machine Control Structure） 虚拟机控制结构
* Guests和host都有自己的VMCS
* EPT（Extended Page Table） 内存虚拟化

* https://github.com/tandasat/HyperPlatform