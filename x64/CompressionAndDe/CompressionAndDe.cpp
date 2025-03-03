//#include <iostream>
//#include <stdio.h>
//#include <Windows.h>
//
//int8_t s_negLengthTab[16] =
//{
//    -1,    // 0
//    -2,    // 1
//    -1,    // 2
//    -3,    // 3
//
//    -1,    // 4
//    -2,    // 5
//    -1,    // 6
//    -4,    // 7
//
//    -1,    // 8
//    -2,    // 9
//    -1,    // 10
//    -3,    // 11
//
//    -1,    // 12
//    -2,    // 13
//    -1,    // 14
//    -5,    // 15
//};
//
//uint8_t s_shiftTab[16] =
//{
//    32 - 7 * 1,    // 0
//    32 - 7 * 2,    // 1
//    32 - 7 * 1,    // 2
//    32 - 7 * 3,    // 3
//
//    32 - 7 * 1,    // 4
//    32 - 7 * 2,    // 5
//    32 - 7 * 1,    // 6
//    32 - 7 * 4,    // 7
//
//    32 - 7 * 1,    // 8
//    32 - 7 * 2,    // 9
//    32 - 7 * 1,    // 10
//    32 - 7 * 3,    // 11
//
//    32 - 7 * 1,    // 12
//    32 - 7 * 2,    // 13
//    32 - 7 * 1,    // 14
//    0,             // 15
//};
//
// 
//inline uint32_t ReadUnsigned(uint8_t** pbEncoding)
//{
//    uint32_t lengthBits = **pbEncoding & 0x0F;
//    size_t negLength = s_negLengthTab[lengthBits];
//    uint32_t shift = s_shiftTab[lengthBits];
//    uint32_t result = *(reinterpret_cast<uint32_t*>(*pbEncoding - negLength - 4));
//
//    result >>= shift;
//    *pbEncoding -= negLength;
//    return result;
//}// Constants for decompression.
//
//
//inline uint32_t getNETencoded(uint8_t* buffer, uint32_t value)
//{
//    int i = 0;
//    if (value < 128)
//    {
//        buffer[i++] = (BYTE)((value << 1) + 0);
//    }
//    else if (value < 128 * 128)
//    {
//        buffer[i++] = (BYTE)((value << 2) + 1);
//        buffer[i++] = (BYTE)(value >> 6);
//    }
//    else if (value < 128 * 128 * 128)
//    {
//        buffer[i++] = (BYTE)((value << 3) + 3);
//        buffer[i++] = (BYTE)(value >> 5);
//        buffer[i++] = (BYTE)(value >> 13);
//    }
//    else if (value < 128 * 128 * 128 * 128)
//    {
//        buffer[i++] = (BYTE)((value << 4) + 7);
//        buffer[i++] = (BYTE)(value >> 4);
//        buffer[i++] = (BYTE)(value >> 12);
//        buffer[i++] = (BYTE)(value >> 20);
//    }
//    else
//    {
//        buffer[i++] = 15;
//        buffer[i++] = (BYTE)(value);
//        buffer[i++] = (BYTE)(value >> 8);
//        buffer[i++] = (BYTE)(value >> 16);
//        buffer[i++] = (BYTE)(value >> 24);
//    }
//
//    return i;
//}
//
//
//// 打印字节流
//void printBuffer(uint8_t* buffer, uint32_t length)
//{
//    for (uint32_t i = 0; i < length; i++)
//    {
//        printf("%02X ", buffer[i]);
//    }
//    printf("\n");
//}
//
//// 测试函数
//void testCompression(uint32_t value)
//{
//    uint8_t buffer[5];
//    uint32_t length = getNETencoded(buffer, value);
//
//    printf("Original value: 0x%X\n", value);
//    printf("Compressed data: ");
//    printBuffer(buffer, length);
//
//    uint8_t* pbEncoding = buffer + length - 1;
//    uint32_t decodedValue = ReadUnsigned(&pbEncoding);
//
//    printf("Decoded value: 0x%X\n", decodedValue);
//    printf("----------------------------------------\n");
//}
//
//int main()
//{
//    // 测试1字节压缩
//    testCompression(0x12);  // 1字节模式
//
//    // 测试2字节压缩
//    testCompression(0x123); // 2字节模式
//
//    // 测试3字节压缩
//    testCompression(0x1234); // 3字节模式
//
//    // 测试4字节压缩
//    testCompression(0x123456); // 4字节模式
//
//    return 0;
//}
//
////inline uint32_t ReadUnsigned(uint8_t* pbEncoding)
////{
////    uint32_t lengthBits = *pbEncoding & 0x0F;
////    size_t negLength = s_negLengthTab[lengthBits];
////    uint32_t shift = s_shiftTab[lengthBits];
////    uint32_t result = *(reinterpret_cast<uint32_t*>(pbEncoding - negLength - 4));
////    result >>= shift;
////    pbEncoding -= negLength;
////    return result;
////}
////
////int main() {
////    uint8_t encodedData[] = { 0x03, 0x12, 0x34, 0x56, 0x78 }; // 假设的编码数据
////    uint8_t* pbEncoding = encodedData + 4; // 指向最后一个字节
////    uint32_t value = ReadUnsigned(pbEncoding);
////    printf("%p", value);
////}
//
////#include <iostream>
////#include <vector>
////#include <cstdint>
////
////// 压缩函数
////std::vector<uint8_t> CompressUnsigned(uint32_t value) {
////    std::vector<uint8_t> compressedData;
////
////    // 确定 lengthBits
////    uint8_t lengthBits;
////    if (value <= 0xFF) {
////        lengthBits = 3; // 1字节模式
////    }
////    else if (value <= 0xFFFF) {
////        lengthBits = 2; // 2字节模式
////    }
////    else if (value <= 0xFFFFFF) {
////        lengthBits = 1; // 3字节模式
////    }
////    else {
////        lengthBits = 0; // 4字节模式
////    }
////
////    // 填充数据
////    switch (lengthBits) {
////    case 0: // 4字节
////        compressedData.push_back((value >> 24) & 0xFF);
////        compressedData.push_back((value >> 16) & 0xFF);
////        compressedData.push_back((value >> 8) & 0xFF);
////        compressedData.push_back(value & 0xFF);
////        break;
////    case 1: // 3字节
////        compressedData.push_back((value >> 16) & 0xFF);
////        compressedData.push_back((value >> 8) & 0xFF);
////        compressedData.push_back(value & 0xFF);
////        break;
////    case 2: // 2字节
////        compressedData.push_back((value >> 8) & 0xFF);
////        compressedData.push_back(value & 0xFF);
////        break;
////    case 3: // 1字节
////        compressedData.push_back(value & 0xFF);
////        break;
////    default:
////        break;
////    }
////
////    // 设置 lengthBits
////    compressedData.back() |= (lengthBits << 4);
////
////    return compressedData;
////}
////
////int main() {
////    // 测试压缩
////    uint32_t value = 0x12; // 测试数值
////    std::vector<uint8_t> compressed = CompressUnsigned(value);
////
////    // 打印压缩结果
////    std::cout << "Compressed data: ";
////    for (uint8_t byte : compressed) {
////        printf("%02X ", byte);
////    }
////    std::cout << std::endl;
////
////    return 0;
////}
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <windows.h>


//1			1      0            2
//2			10     0            4
//3			11     0            6

////  高位如果是1，则后续是
//1000 0000       1000 0000       1000 0000       0   000 0000
//
//01 2个字节
//0  1个字节
//
//0000 000        0
//
//0000 00         01
//0000 0000
//
//00 01           1       2字节         
//01 01           5
//10 01           9
//11 01           13      4字节     32位  + 4bit 1111
//                1111


 // 原始数据位1              1字节   2 
 // 压缩数据  1 < 128 
 // 0010        压缩后的数据  1 
 //  

//0010        // 0   1
            // 2   10
            // 3   11
            // 4   111
            // 5   1111


// 压缩表和解压表
int8_t s_negLengthTab[16] =
{
    -1,    // 0         
    -2,    // 1
    -1,    // 2
    -3,    // 3

    -1,    // 4
    -2,    // 5
    -1,    // 6
    -4,    // 7

    -1,    // 8
    -2,    // 9
    -1,    // 10
    -3,    // 11

    -1,    // 12
    -2,    // 13
    -1,    // 14
    -5,    // 15            000 0               00 01     00 11       0 111   1111
};

uint8_t s_shiftTab[16] =
{
    32 - 7 * 1,    // 0
    32 - 7 * 2,    // 1
    32 - 7 * 1,    // 2
    32 - 7 * 3,    // 3

    32 - 7 * 1,    // 4
    32 - 7 * 2,    // 5
    32 - 7 * 1,    // 6
    32 - 7 * 4,    // 7

    32 - 7 * 1,    // 8
    32 - 7 * 2,    // 9
    32 - 7 * 1,    // 10
    32 - 7 * 3,    // 11

    32 - 7 * 1,    // 12
    32 - 7 * 2,    // 13
    32 - 7 * 1,    // 14
    0,             // 15
};

// 解压函数
inline uint32_t ReadUnsigned(uint8_t** pbEncoding)
{ // 1111    1110           8 4 2 
    uint32_t lengthBits = **pbEncoding & 0x0F;      // 1 2 3 4      &F 
    size_t negLength = s_negLengthTab[lengthBits];  // 

    if (negLength == 1)
    {
        // 读取一个字节
    }
    if (negLength == 2)
    {
        // 读取二个字节 >> 2
    }
    if (negLength == 3)
    {
        // 读取二个字节 >> 2
    }

    uint32_t shift = s_shiftTab[lengthBits];
    uint32_t result = *(reinterpret_cast<uint32_t*>(*pbEncoding - negLength - 4));
    // 04 8D CC CC   2字节的数
    //  32 16 2
    // 010010001101         01        1100 1100 1100 1100
    result >>= shift;
    *pbEncoding -= negLength;

    return result;
}

// 压缩函数
inline uint32_t getNETencoded(uint8_t* buffer, uint32_t value)
{
    // 1.高1表示后续是否有数据     
    // 2.
    int i = 0;
    if (value < 128)
    {
        //7F =   01111111           127    11111110
        buffer[i++] = (BYTE)((value << 1) + 0); //  0   0000 000        0                     1.数据位低3位+ 0        1字节大小          8   7    1  
    }
    else if (value < 128 * 128)
    {
        // 000100      10 00 11                                       4                          2       
        //                                                                                       0000 0   00  01      2位标志2位数据       2字节大小               2字节   2bit
        // [i]   000100 10 00 11 01                                                              F7
        // [i+1]
        buffer[i++] = (BYTE)((value << 2) + 1); //  01                                     
        buffer[i++] = (BYTE)(value >> 6);       //  0000 00             01
    }
    else if (value < 128 * 128 * 128)           //      011
    {
        // 0000 000  011
        buffer[i++] = (BYTE)((value << 3) + 3); //                      11 
        buffer[i++] = (BYTE)(value >> 5);
        buffer[i++] = (BYTE)(value >> 13);
    }
    else if (value < 128 * 128 * 128 * 128)
    {
        buffer[i++] = (BYTE)((value << 4) + 7); // 0111
        buffer[i++] = (BYTE)(value >> 4);
        buffer[i++] = (BYTE)(value >> 12);
        buffer[i++] = (BYTE)(value >> 20); // 4字节                0111      4字节      2^28
    }
    else
    {
        buffer[i++] = 15;                   //   XXXXXXXXX           1111            2^32               << 1111
        // db 1
        // db 2
        buffer[i++] = (BYTE)(value);
        buffer[i++] = (BYTE)(value >> 8);
        buffer[i++] = (BYTE)(value >> 16);
        buffer[i++] = (BYTE)(value >> 24);
    }

    return i;
}

// 打印字节流            1             1
void printBuffer(uint8_t* buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

// 测试函数
void testCompression(uint32_t value)
{
    uint8_t buffer[5] = { 0 };
    uint32_t length = getNETencoded(buffer, value);

    printf("Original value: 0x%X\n", value);
    printf("Compressed data: ");
    printBuffer(buffer, length);

    uint8_t* pbEncoding = buffer;
    uint32_t decodedValue = ReadUnsigned(&pbEncoding);

    printf("Decoded value: 0x%X\n", decodedValue);
    printf("----------------------------------------\n");
}

int main()
{
    // 测试1字节压缩
   // testCompression(0x12);  // 1字节模式

    //7F =   01111111           127

    //// 测试2字节压缩
    // testCompression(0x7F); // 1字节模式
   //testCompression(0xFFFFFFFF); // 2字节模式

    //// 测试3字节压缩
    testCompression(0x12345); // 3字节模式

    //// 测试4字节压缩
    // testCompression(0x1234567); // 4字节模式

    return 0;
}
// 0        1字节 
// 01       2字节   
// 11
// 111
// 1111

//0-128     1
//0-        2 
// 
// 
//1			1      0                    10               => 1                2
//2			10     0                    100              => 1                4
//3			11     0                    110              => 1                6
//4			100    0                    1000             => 1                8
//5			101    0                    1010                                 10
//6			110    0
//7			111    0
//   11111110                   低3数据位  + 0                  
//8			1000   0
//9			1001   0
//
//10			1010   0
//11          1011   0
//12          1100   0                  =>  1
//13          1101   0
//14          1110   0
//15          1111   0
//
//
//
//   << 2   +1
//0001001000      00 01
//                01 01                     =>   2
//                10 01
//                11 01
//                 
//
//                00  11                    =>   3  
//                10  11

//
//                0111                      =>    4
// 
// 
//                1111                      =>    5
//
//

         //00  01           => 2
