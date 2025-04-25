
# File System Driver

CreateFile => Irp请求 => 卷设备 => 硬盘设备

* 过滤硬盘设备驱动 将驱动对象放在`\FileSystem\`目录下就是文件过滤驱动
* `\FileSystem\Filters\`

* `ExInitializePagedLookasideList` 分配堆列表
* `ExDeletePagedLookasideList` 释放堆列表
* `ExAllocateFromPagedLookasideList` 分配堆中的内存，减少内存碎片
* `ExFreeToPagedLookasideList` 释放堆中的内存

* `IoRegisterFsRegistrationChange` 例程注册文件系统筛选器驱动程序的通知例程，如果有新设备插入时会调用这个回调

* `sFilter` 文件系统过滤驱动
* `MiniFilter` 文件微过滤驱动

> https://learn.microsoft.com/zh-cn/windows-hardware/drivers/ifs/filter-manager-concepts

* 在WDK里面有示例
* `FilterGetMessage` 获取过滤驱动消息

**暴力调用底层驱动**
* `IoCallDriver` 调用底层驱动手动创建IRP
* `IoAllocateIrp` 分配IRP

* `mklink b.txt a.txt` 创建硬链接，修改b.txt会修改a.txt
* 通过这种方式可以突破检测

> 暴力删除参考 36000safe

**解除占用**
> 参考 OpenArk

``` C++
#include <Ntifs.h>
#include <ntddk.h>

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,             // obsolete...delete
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation,
    SystemPerformanceTraceInformation,
    SystemObsolete0,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemVerifierAddDriverInformation,
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformation,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation,
    SystemFileCacheInformationEx,
    MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

NTSTATUS
NTAPI
ZwQuerySystemInformation(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
);

VOID Unload(_In_ struct _DRIVER_OBJECT* DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);

    DbgPrint("[51asm] Unload\n");
}

NTSTATUS UnlockFile() {
    //1.遍历所有句柄
    NTSTATUS Status;
    PSYSTEM_HANDLE_INFORMATION      Handles;
    PSYSTEM_HANDLE_TABLE_ENTRY_INFO HandleInfo;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;
    ULONG i;
    ULONG FileNumbers = 0;
    KAPC_STATE ApcState;
    PEPROCESS Process = NULL;
    POBJECT_NAME_INFORMATION ObjNameInfo = ExAllocatePoolWithTag(NonPagedPool, 0x2000, '1234');
    if (!ObjNameInfo) {
        return STATUS_NO_MEMORY;
    }

retry:
    Buffer = ExAllocatePoolWithTag(NonPagedPool, BufferSize, '1234');

    if (!Buffer) {
        ExFreePool(ObjNameInfo);
        return STATUS_NO_MEMORY;
    }
    Status = ZwQuerySystemInformation(SystemHandleInformation,
        Buffer,
        BufferSize,
        &ReturnLength
    );

    if (Status == STATUS_INFO_LENGTH_MISMATCH) {
        ExFreePool(Buffer);
        BufferSize = ReturnLength;
        goto retry;
    }

    if (NT_SUCCESS(Status)) {
        Handles = (PSYSTEM_HANDLE_INFORMATION)Buffer;
        for (i = 0;i < Handles->NumberOfHandles; i++) {
            HandleInfo = &Handles->Handles[i];

            Status = ObReferenceObjectByPointer(HandleInfo->Object,
                0,
                *IoFileObjectType,
                KernelMode);
            if (NT_SUCCESS(Status)) {
                Status = ObQueryNameString(HandleInfo->Object, ObjNameInfo, 0X1000, &ReturnLength);
                if (NT_SUCCESS(Status)) {
                    if (wcsstr(ObjNameInfo->Name.Buffer, L"51asm.txt") != NULL) {
                        DbgPrint("[51asm] UniqueProcessId:%hu HandleValue:%hu ObjNameInfo:%wZ\n", 
                            HandleInfo->UniqueProcessId,
                            HandleInfo->HandleValue, 
                            &ObjNameInfo->Name);
                        Status = PsLookupProcessByProcessId((HANDLE)HandleInfo->UniqueProcessId, &Process);
                        if (NT_SUCCESS(Status)) {
                            KeStackAttachProcess(Process, &ApcState);
                            ZwClose((HANDLE)HandleInfo->HandleValue);
                            KeUnstackDetachProcess(&ApcState);
                            ObDereferenceObject(Process);
                        }
                        FileNumbers++;
                    }
                  
                }
                ObDereferenceObject(HandleInfo->Object);
            }
        }

        DbgPrint("[51asm] NumberOfHandles:%d FileNumbers:%d\n", 
            Handles->NumberOfHandles, 
            FileNumbers);
    }

    ExFreePool(ObjNameInfo);
    ExFreePool(Buffer);
    return Status;
}

NTSTATUS DriverEntry(
    __in struct _DRIVER_OBJECT* DriverObject,
    __in PUNICODE_STRING  RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DbgPrint("[51asm] DriverEntry\n");

    DriverObject->DriverUnload = Unload;

    UnlockFile();

    return STATUS_SUCCESS;
}
```