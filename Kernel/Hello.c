// Hello.c
#include <ntddk.h>

VOID Unload(__in struct _DRIVER_OBJECT  *DriverObject){
    // 声明未使用变量，不然会报错
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Unload Hello World!");
}

NTSTATUS DriverEntry( 
    __in struct _DRIVER_OBJECT  *DriverObject,
    __in PUNICODE_STRING  RegistryPath 
    )
{
    UNREFERENCED_PARAMETER(RegistryPath);
    DbgPrint("Hello World!");
    // 注册卸载函数
    DriverObject->DriverUnload = Unload;
    return STATUS_SUCCESS;
} 