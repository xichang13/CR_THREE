
# 内核API

**内核API分类**
* 对象管理器(ObXxx)
* 内存管理器(MmXxx)
* 进程线程管理器(PsXxx)
* I/O管理器(IoXxx)
* 电源管理器(PoXxx)
* 配置管理器(CmXxx)
* 核心层API(KeXxx)
* 执行层API(ExXxx)
* 运行库API(RtlXxx)
* RING3API的内核API(ZwXxx)

**操作系统启动流程**
1. 通电
2. BIOS自检
3. 引导扇区(硬盘第一个扇区)
4. 加载器(winload.exe、winload.efi) 验证系统是否合法
5. 内核加载(ntoskrnl.exe)
6. 创建系统进程/服务
7. 创建桌面

**虚拟地址检测**
``` C++
BOOLEAN MmIsAddressValid(PVOID VirtualAddress); // 检测虚拟地址是否有效
```

**异常检测保护**
``` C++
// 主要检测Ring3的地址访问异常
__try {
     // 代码
} __except (EXCEPTION_EXECUTE_HANDLER) {
     // 异常处理代码
}
```

**RtlInitUnicodeString**
> 初始化Unicode字符串，该API不会分配内存，而是将字符串指针直接赋值给成员变量，所以在使用时要注意字符串能否修改或拼接
> 由于内核中的字符串可能不会以'\0'结尾，所以`strcpy`不可靠
> 需要时刻注意溢出检测

## 文件API

> https://learn.microsoft.com/zh-cn/windows-hardware/drivers/kernel/using-files-in-a-driver

* 应用层: C:\\1.txt
* 内核层: \\??\\C:\\1.txt    \\SystemRoot\\1.txt
* ZwCreateFile 创建文件
* ZwWriteFile 写入文件
* ZwReadFile 读取文件
* ZwDeleteFile 删除文件
* ZwQueryDirectoryFile 查询文件
* ZwQueryInformationFile 查询文件信息
* ZwQueryFullAttributesFile 查询文件属性
* ZwSetInformationFile 设置文件信息
* ZwClose 关闭文件

``` C++
#include <ntifs.h>
void File(){
    OBJECT_ATTRIBUTES oa = { 0 };
    UNICODE_STRING ustrName = { 0 };
    RtlInitUnicodeString(&ustrName, L"\\??\\C:\\1.txt");
    InitializeObjectAttributes(&oa, &ustrName, OBJ_KERNEL_HANDLE /*内核句柄*/ | OBJ_CASE_INSENSITIVE /*不区分大小写*/, NULL, NULL);
    NTSTATUS Status = ZwDeleteFile(&oa); // 删除文件
}
```

## 注册表

> https://learn.microsoft.com/zh-cn/windows-hardware/drivers/kernel/registry-key-object-routines

* `C:\\Windows\\System32\\drivers` 开机自启动的驱动目录
* `C:\\Windows\\System32\\config` 注册表配置文件目录

在内核模式中，所有注册表项的根目录都是`\\Registry`对象，全局句柄对应于 \Registry 对象的后代，如下表所示。

| 用户模式句柄 | 对应的对象名称 |
| --- | --- |
| HKEY_LOCAL_MACHINE | \Registry\Machine |
| HKEY_CURRENT_USERS | \Registry\User |

* ZwCreateKey 创建注册表项
* ZwOpenKey 打开注册表项
* ZwEnumerateKey 枚举注册表项
* ZwQueryKey 查询注册表项
* ZwDeleteKey 删除注册表项
* ZwEnumerateValueKey 枚举注册表项的值
* ZwQueryValueKey 查询注册表项的值
* ZwSetValueKey 设置注册表项的值
* ZwDeleteValueKey 删除注册表项的值

## 线程中断请求级别(IRQL)

* 无中断:
    * PASSIVE_LEVEL(0)      可以读取分页内存、操作文件I/O
* 软中断:
    * APC_LEVEL(1)          可以读取分页内存
    * DISPATCH_LEVEL(2)     不可访问分页内存、不可操作文件I/O
* 硬中断:
    * DIRQL 设备中断请求级别处理程序执行
    * PROFILE_LEVEL 配置文件定时器
    * CLOCK2_LEVEL 时钟中断
    * SYNCH_LEVEL 同步中断
    * IPI_LEVEL 处理器之间中断
    * POWER_LEVEL 电源管理

> 级别越高，优先级越高，越不会被其他线程打断，可以通过提高级别来保护代码不会被打断，相当于同步对象的上锁

* KeGetCurrentIrql 获取当前IRQL
* KeRaiseIrql 提高IRQL
* KeLowerIrql 降低IRQL
* PAGED_CODE() 断言宏，超过APC_LEVEL会报错，防止代码出现异常情况

## 线程创建

``` C++
NTSTATUS PsCreateSystemThread(
  [out]           PHANDLE            ThreadHandle,     // 线程句柄
  [in]            ULONG              DesiredAccess,    // 访问权限, GENERIC_ALL
  [in, optional]  POBJECT_ATTRIBUTES ObjectAttributes, // 对象属性, NULL
  [in, optional]  HANDLE             ProcessHandle,    // 进程句柄, NULL
  [out, optional] PCLIENT_ID         ClientId,         // 客户端ID, NULL
  [in]            PKSTART_ROUTINE    StartRoutine,     // 线程入口函数
  [in, optional]  PVOID              StartContext      // 线程入口参数
);
```

* KeDelayExecutionThread 延迟线程执行
* ZwTerminateProcess 终止进程
* ZwTerminateThread 终止线程
* KeWaitForSingleObject 等待对象
* ZwClose 关闭句柄

## 事件对象

``` C++
KEVENT obj;
KeInitializeEvent(&obj, NotificationEvent, FALSE); // 初始化事件对象
KeSetEvent(&obj, 0, FALSE); // 设置事件对象
KeWaitForSingleObject(&obj, Executive, KernelMode, FALSE, NULL); // 等待事件对象
KeClearEvent(&obj); // 清除事件对象
KeResetEvent(&obj); // 重置事件对象
```

**自旋锁对象(SPIN_LOCK)**
``` C++
KeAcquireSpinLock(&obj); // 获取自旋锁
// ...
KeReleaseSpinLock(&obj); // 释放自旋锁
```

**通知的同步对象**
> RING0主动通知RING3
``` C++
// ring0
HANDLE obj;
IoCreateNotificationEvent(L"\\BaseNamedObjects\\MyEvent", &obj); // 创建通知的同步对象

// ring3
OpenEvent(L"\\Global\\MyEvent", EVENT_ALL_ACCESS); // 打开通知的同步对象
```

## 内核数据结构

* InitializeListHead // 链表
* RtlInitializeGenericTableAvl // AVL树