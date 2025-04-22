
# trace

* `dex_pc` - 指令地址
* `Instruction*` - 指令反编译结果
* `ArtMethod*` - java源码方法，通过该字段可以拿到执行的java方法
* 输出日志
``` C++
LOG(ERROR) << "日志内容";
```

``` C++
if(method->PrettyMethod().find("com.cr49.test.MainActivity.OnCreate") != std::string::npos)
{
    std::string strDecode = inst->DumpString(method->GetDexFile());
    LOG(ERROR) << "[trace]: " << dex_pc << " " << strDecode.c_str();
}
```

* 文件夹
    * `/data/local/tmp/` - 临时目录
    * `/sdcard/` - 外部存储目录