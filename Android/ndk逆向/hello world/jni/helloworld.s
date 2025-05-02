
.arch  armv8-a //处理器架构
.global main
.data 
    g_sz: .string "hello world\n"
.text
main:
    mov x0, 0    // 标准输出
    adr x1, g_sz // 字符串
    mov x2, 12   // 字符串长度
    mov x8, 64   // 系统调用号
    svc 0        // 系统调用
    ret