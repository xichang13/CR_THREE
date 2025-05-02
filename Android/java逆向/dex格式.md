
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Dalvik 可执行文件格式 dex格式](#dalvik-可执行文件格式-dex格式)
- [准备 （编译dex+单独执行dex）](#准备-编译dex单独执行dex)

<!-- /code_chunk_output -->


# Dalvik 可执行文件格式 dex格式

> [Dalvik 可执行文件格式](https://source.android.com/docs/core/runtime/dex-format?hl=zh-cn)

**在Android中执行dex文件**
* `adb push classes.dex /data/local/tmp` 将dex文件推送到设备上
* `adb shell dalvikvm64 -cp /data/local/tmp/classes.dex com.cr49.example.MyClass` 执行dex文件中的MyClass类

* 在修改dex文件后需要修正dex文件的`checksum`，否则无法执行
* > [adler32校验算法](http://androidxref.com/8.1.0_r33/xref/external/syslinux/com32/lib/zlib/adler32.c)

* 使用`d8 [file] ...` 将Java文件编译为dex文件，`d8`是Android SDK中的工具，用于将Java文件编译为dex文件。

# 准备 （编译dex+单独执行dex）
* 源码
``` Java
public class TestDex {
    public static void main(String[] args) {
        System.out.println("hello dex");
    }
}
```
* 使用`android studio`编译，生成的`class`文件在`\build\intermediates\javac\debug\classes\com\kr\xxx\`下
* 编译成dex，`d8`工具位于`Sdk\build-tools\34.0.0-rc3`中
``` bash
dx xxx.class
```
* 执行
``` bash
dalvikvm -cp classes.dex com.cr49.test.TestDex
```
``` bash
export CLASSPATH=/data/local/tmp/classes.dex
exec app_process /data/local/tmp com.cr49.test.TestDex
```

* 修正校验值
``` C++
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#define uLong unsigned long
#define Bytef unsigned char
#define uInt unsigned int

#define BASE 65521UL
#define NMAX 5552
#define Z_NULL NULL

#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

/* use NO_DIVIDE if your processor does not do division in hardware */
#ifdef NO_DIVIDE
#  define MOD(a) \
    do { \
        if (a >= (BASE << 16)) a -= (BASE << 16); \
        if (a >= (BASE << 15)) a -= (BASE << 15); \
        if (a >= (BASE << 14)) a -= (BASE << 14); \
        if (a >= (BASE << 13)) a -= (BASE << 13); \
        if (a >= (BASE << 12)) a -= (BASE << 12); \
        if (a >= (BASE << 11)) a -= (BASE << 11); \
        if (a >= (BASE << 10)) a -= (BASE << 10); \
        if (a >= (BASE << 9)) a -= (BASE << 9); \
        if (a >= (BASE << 8)) a -= (BASE << 8); \
        if (a >= (BASE << 7)) a -= (BASE << 7); \
        if (a >= (BASE << 6)) a -= (BASE << 6); \
        if (a >= (BASE << 5)) a -= (BASE << 5); \
        if (a >= (BASE << 4)) a -= (BASE << 4); \
        if (a >= (BASE << 3)) a -= (BASE << 3); \
        if (a >= (BASE << 2)) a -= (BASE << 2); \
        if (a >= (BASE << 1)) a -= (BASE << 1); \
        if (a >= BASE) a -= BASE; \
    } while (0)
#  define MOD4(a) \
    do { \
        if (a >= (BASE << 4)) a -= (BASE << 4); \
        if (a >= (BASE << 3)) a -= (BASE << 3); \
        if (a >= (BASE << 2)) a -= (BASE << 2); \
        if (a >= (BASE << 1)) a -= (BASE << 1); \
        if (a >= BASE) a -= BASE; \
    } while (0)
#else
#  define MOD(a) a %= BASE
#  define MOD4(a) a %= BASE
#endif

uLong adler32(uLong adler, const Bytef* buf, uInt len)
{
    unsigned long sum2;
    unsigned n;

    /* split Adler-32 into component sums */
    sum2 = (adler >> 16) & 0xffff;
    adler &= 0xffff;

    /* in case user likes doing a byte at a time, keep it fast */
    if (len == 1) {
        adler += buf[0];
        if (adler >= BASE)
            adler -= BASE;
        sum2 += adler;
        if (sum2 >= BASE)
            sum2 -= BASE;
        return adler | (sum2 << 16);
    }

    /* initial Adler-32 value (deferred check for len == 1 speed) */
    if (buf == Z_NULL)
        return 1L;

    /* in case short lengths are provided, keep it somewhat fast */
    if (len < 16) {
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        if (adler >= BASE)
            adler -= BASE;
        MOD4(sum2);             /* only added so many BASE's */
        return adler | (sum2 << 16);
    }

    /* do length NMAX blocks -- requires just one modulo operation */
    while (len >= NMAX) {
        len -= NMAX;
        n = NMAX / 16;          /* NMAX is divisible by 16 */
        do {
            DO16(buf);          /* 16 sums unrolled */
            buf += 16;
        } while (--n);
        MOD(adler);
        MOD(sum2);
    }

    /* do remaining bytes (less than NMAX, still just one modulo) */
    if (len) {                  /* avoid modulos if none remaining */
        while (len >= 16) {
            len -= 16;
            DO16(buf);
            buf += 16;
        }
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        MOD(adler);
        MOD(sum2);
    }

    /* return recombined sums */
    return adler | (sum2 << 16);
}

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        std::cout << "缺少文件路径: adler32.exe pathname" << std::endl;
        system("pause");
        return 0;
    }
    FILE* pFile = fopen(argv[1], "rb+");
    if (pFile == NULL) {
        std::cout << "fopen failed " << strerror(errno) << std::endl;
        system("pause");
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    long lFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    Bytef* pBuf = new Bytef[lFileSize];
    fread(pBuf, 1, lFileSize, pFile);

    uLong adler = adler32(0L, Z_NULL, 0);
    const int nonSum = 12;

    uLong nCheckSum = adler32(adler, pBuf + nonSum, lFileSize - nonSum);
    *(uLong*)(pBuf + 8) = nCheckSum;

    std::cout << "checksum:" << std::hex << nCheckSum << std::endl;

    fseek(pFile, 0, SEEK_SET);
    fwrite(pBuf, 1, lFileSize, pFile);

    delete[]pBuf;
    fclose(pFile);
    system("pause");
	return 0;
}
```