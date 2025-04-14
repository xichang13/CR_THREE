
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [签名](#签名)
  - [手动签名](#手动签名)
  - [使用Android Studio签名](#使用android-studio签名)
- [重打包](#重打包)
- [调试](#调试)
- [smali 汇编](#smali-汇编)

<!-- /code_chunk_output -->


# 签名

## 手动签名

* 生成密钥库
    * `keytool` - java自带
``` shell
# 生成密钥库
keytool -genkeypair -alias 别名 -keyalg RSA -keystore 库名

# 查看密钥库中的密钥
keytool -list -v -keystore 库名
```

* 签名
    * `apksigner` - android自带，位于`SDK目录\build-tools\版本号\`
    * https://developer.android.com/tools/apksigner?hl=zh#examples-sign
    * v1 - 7.0以下
    * v2 - 7.0及以上
``` shell
# 查看apk是否签过名
apksigner verify -v xxx.apk

# 签名
apksigner sign --ks yyyy xxx.apk
```

## 使用Android Studio签名

* `Build -> Generate Signed Bundle/APK` 生成密钥库
* `File -> Project Structure -> Modules -> Signing Configs` 指定签名库
* `Project -> Modules -> Default Config -> Signing Config` 选择设置
* 编译

# 重打包

* `apktool` - 反编译
    * https://apktool.org/
    * https://apktool.org/docs/install/
* 使用 `apktool` 反编译apk
    * `apktool d xxx.apk`
* 重打包
    * `apktool b 输入.apk -o 输出.apk`
* `zipalign` - zip压缩对齐工具
``` shell
# 判断是否对齐
zipalign -c -v 4 xxx.apk

# 对齐
zipalign -v 4 xxx.apk
```
* 签名
* 安装

# 调试

* 在手机上调试启动app
    * `adb shell am start -D -n 包名/活动名`

* `jda`  反编译工具
* `jadx` 反编译工具
* `jeb`  调试和反编译工具
* 使用`jeb`打开apk，要在调试的位置设置断点
* 调试前APK需要设置调试标志
    * 在清单文件中`application`标签添加`android:debuggable="true"`

# smali 汇编

* [指令格式](https://source.android.google.cn/docs/core/runtime/instruction-formats?hl=zh-cn)
* [字节码格式](https://source.android.google.cn/docs/core/runtime/dalvik-bytecode?hl=zh-cn)

```
54 41 F1 47

B|A|op CCCC	22t	op vA, vB, +CCCC

B A op CCCC 
4 1 54 47f1
```