#include <idc.idc>

/*
 * 往指定文件追加或覆盖写入二进制数据
 * @param start_addr 起始地址
 * @param end_addr 结束地址
 * @param filename 输出文件路径
 * @param append 是否追加模式，1=追加，0=覆盖
 * @return 成功返回1，失败返回0
 */
static dump_memory(start_addr, end_addr, filename, append) {
    auto size, mem;
    
    if (start_addr >= end_addr) {
        Warning("起始地址必须小于结束地址");
        return 0;
    }
    
    size = end_addr - start_addr;
    mem = GetManyBytes(start_addr, size, 0);
    
    if (mem == 0) {
        Warning("无法读取内存 0x%x - 0x%x", start_addr, end_addr);
        return 0;
    }
    
    auto file = fopen(filename, append ? "ab" : "wb"); // "ab"模式表示追加二进制写入
    if (file == 0) {
        Warning("无法打开文件: %s", filename);
        return 0;
    }
    
    auto bytes_written = 0;
    auto current_ea = start_addr;
    while (current_ea < end_addr) 
    {
        auto byte_val = Byte(current_ea);
        if (byte_val == -1) break;  // 读取失败
        
        fputc(byte_val, file);
        bytes_written++;
        current_ea++;
        
        // 每1MB打印进度
        if (bytes_written % 0x100000 == 0) {
            Message("已写入 0x%X bytes @ 0x%08X...\n", bytes_written, current_ea);
        }
    }

    fclose(file);
    return 1;
}

static main() {
    auto outfile = "C:\\combined_dump"; // 最终的文件路径
    
    // 第一次写入，如果文件存在则先覆盖（append=0）
    dump_memory(0x73ea9c3000, 0x73ea9fe000, outfile, 0);
    
    // 后续写入，追加模式（append=1）
    dump_memory(0x73ea9fe000, 0x73eaa02000, outfile, 1);  // 追加第2个区域
    dump_memory(0x73eaa02000, 0x73eaa04000, outfile, 1);  // 追加第3个区域
    
    Message("成功将所有块写入 %s\n", outfile);
}