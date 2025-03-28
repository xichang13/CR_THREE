
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [SSDT Hook 方法](#ssdt-hook-方法)

<!-- /code_chunk_output -->

# SSDT Hook 方法

1. 替换SSDT表中的函数地址
2. Hook MSR
3. inline hook nt*函数
4. inline hook KiSystemService 或者 KiFastCallEntry

* 找到原始函数地址，和当前函数地址对比可以判断是否被hook
* hook `NtOpenProcess` 保护自己的进程不被打开
* 查看当前函数地址是否在 ntokernel.exe 模块范围

* 拷贝 ETHREAD.ServiceTable 修改指定进程的 SSDT 表

* ring3可以通过ntdll的导出表直接调用内核函数

**ring3 读取物理内存条**
1. 动态调用 `ZwOpenSection` 打开物理内存条
2. 动态调用 `ZwMapViewOfSection` 将物理内存映射到进程地址空间
3. 动态调用 `ZwUnmapViewOfSection` 解除映射
4. 动态调用 `ZwClose` 关闭物理内存条

**Hook NtOpenProcess**

``` C++
#include <Ntifs.h>
#include <ntddk.h>
#include <intrin.h>

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

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
    HANDLE Section;                 // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

extern "C" {
    UCHAR* PsGetProcessImageFileName(__in PEPROCESS Process);
    NTSTATUS
        ZwQuerySystemInformation(
            __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
            __out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
            __in ULONG SystemInformationLength,
            __out_opt PULONG ReturnLength
        );
}

typedef struct _KSERVICE_TABLE_DESCRIPTOR {
    PULONG_PTR Base;
    PULONG Count;
    ULONG Limit;
    PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR, * PKSERVICE_TABLE_DESCRIPTOR;

void* GetServiceDescriptorTable() {
    UNICODE_STRING ustrName;
    RtlInitUnicodeString(&ustrName, L"KeAddSystemServiceTable");
    UCHAR *pCode = (UCHAR*)MmGetSystemRoutineAddress(&ustrName);
    if (pCode == NULL) {
        DbgPrint("[51asm] GetServiceDescriptorTable pCode == NULL\n");
        return NULL;
    }
    DbgPrint("[51asm] GetServiceDescriptorTable pCode:%p\n", pCode);

    for (int i = 0; i < 50; i++) {
        if (pCode[0] == 0x83 && pCode[1] == 0xb8) {
            DbgPrint("[51asm] GetServiceDescriptorTable KeServiceDescriptorTable:%p\n", *(void**)(pCode + 2));
            return *(void**)(pCode + 2);
        }
        pCode++;
    }

    return NULL;
}

ULONG_PTR HookSSDT(ULONG nIndex, ULONG_PTR NewAddress) {
    __try {
        DbgPrint("[51asm] HookSSDT nIndex:%d NewAddress:%08x\n", nIndex, NewAddress);
        PKSERVICE_TABLE_DESCRIPTOR pTable = (PKSERVICE_TABLE_DESCRIPTOR)GetServiceDescriptorTable();
        if (pTable == NULL) {
            DbgPrint("[51asm] HookSSDT pTable == NULL\n");
            return NULL;
        }
        DbgPrint("[51asm] HookSSDT pTable:%p\n", pTable);

        if (nIndex >= pTable->Limit) {
            DbgPrint("[51asm] HookSSDT Out Index\n");
            return NULL;
        }

        ULONG_PTR OldAddress = pTable->Base[nIndex];
        pTable->Base[nIndex] = NewAddress;
        DbgPrint("[51asm] HookSSDT OldAddress:%p => NewAddress:%p\n", 
            OldAddress, NewAddress);
        return OldAddress;
    }
    __except (1) {
        DbgPrint("[51asm] HookSSDT __except\n");
    }
    return NULL;
}

//extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
typedef NTSTATUS (__stdcall* NT_OPEN_PROCESS)(
    __out PHANDLE  ProcessHandle,
    __in ACCESS_MASK  DesiredAccess,
    __in POBJECT_ATTRIBUTES  ObjectAttributes,
    __in_opt PCLIENT_ID  ClientId);

LONG g_nRefCount = 0;

ULONG_PTR g_OldNtOpenProcess = NULL;
NTSTATUS FakeNtOpenProcess(
    __out PHANDLE  ProcessHandle,
    __in ACCESS_MASK  DesiredAccess,
    __in POBJECT_ATTRIBUTES  ObjectAttributes,
    __in_opt PCLIENT_ID  ClientId) {
    UNREFERENCED_PARAMETER(ProcessHandle);
    UNREFERENCED_PARAMETER(ObjectAttributes);
    InterlockedIncrement(&g_nRefCount);

    //DbgPrint("[51asm] FakeNtOpenProcess g_nRefCount:%d\n", g_nRefCount);

    __try {
       /* DbgPrint("[51asm] FakeNtOpenProcess UniqueProcess:%d DesiredAccess:%08x\n",
            ClientId->UniqueProcess, DesiredAccess);*/
        PEPROCESS CurProcess = PsGetCurrentProcess();
        PEPROCESS DstProcess = NULL;
        if (CurProcess == DstProcess) {
           
        }
        NTSTATUS Status = PsLookupProcessByProcessId(ClientId->UniqueProcess, &DstProcess);
        if (NT_SUCCESS(Status)) {
            UCHAR* ImageFileName = PsGetProcessImageFileName(DstProcess);
            if (strcmp((char*)ImageFileName, "calc.exe") == 0) {
                DbgPrint("[51asm] FakeNtOpenProcess CurProcess:%08X DstProcess:%08x\n",
                    CurProcess, DstProcess);
                *ProcessHandle = (HANDLE)0x1234;
                InterlockedDecrement(&g_nRefCount);
                return STATUS_SUCCESS;
            }
            ObDereferenceObject(DstProcess);
        }
    }
    __except (1) {
    }
   
    NTSTATUS Status = ((NT_OPEN_PROCESS)g_OldNtOpenProcess)(ProcessHandle, DesiredAccess, 
        ObjectAttributes, ClientId);
 
    InterlockedDecrement(&g_nRefCount);
    //DbgPrint("[51asm] FakeNtOpenProcess g_nRefCount:%d\n", g_nRefCount);

    return Status;
}

VOID Unload(_In_ struct _DRIVER_OBJECT* DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("[51asm] Unload\n");
    HookSSDT(0X7A, g_OldNtOpenProcess);
    
    while (g_nRefCount != 0) {
        //DbgPrint("[51asm] Unload g_nRefCount:%d\n", g_nRefCount);

        LARGE_INTEGER Timer = { 0 };
        Timer.QuadPart = 1000;
        KeDelayExecutionThread(KernelMode, FALSE, &Timer);
    }
}

NTSTATUS EnumSystemModule() {
    NTSTATUS Status;
    PRTL_PROCESS_MODULES            Modules;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;
    ULONG i;

retry:
    Buffer = ExAllocatePoolWithTag(NonPagedPool, BufferSize, '1234');

    if (!Buffer) {
        return STATUS_NO_MEMORY;
    }
    Status = ZwQuerySystemInformation(SystemModuleInformation,
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
        Modules = (PRTL_PROCESS_MODULES)Buffer;
        for (i = 0; i < Modules->NumberOfModules; i++) {
            DbgPrint("[51asm] EnumSystemModule ImageBase:%p ImageSize:%p FullPathName:%s\n",
                Modules->Modules[i].ImageBase,
                Modules->Modules[i].ImageSize,
                Modules->Modules[i].FullPathName);
        }
    }

    ExFreePool(Buffer);
    return Status;
}

extern "C" NTSTATUS DriverEntry(
    __in struct _DRIVER_OBJECT* DriverObject,
    __in PUNICODE_STRING  RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DbgPrint("[51asm] DriverEntry\n");

    g_OldNtOpenProcess = HookSSDT(0X7A, (ULONG_PTR)FakeNtOpenProcess);

    //EnumSystemModule();
    DriverObject->DriverUnload = Unload;

    return STATUS_SUCCESS;
}
```