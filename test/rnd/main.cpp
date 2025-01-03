
#include <windows.h>
#include <wincrypt.h>
#include <bitset>  // 用于方便的二进制打印
#include <iostream>

// 定义每个 Unicode 字符的常量
const wchar_t SYMBOLS[] = {
    L'\u2630',  // ☰
    L'\u2631',  // ☱
    L'\u2632',  // ☲
    L'\u2633',  // ☳
    L'\u2634',  // ☴
    L'\u2635',  // ☵
    L'\u2636',  // ☶
    L'\u2637'   // ☷
};

// 使用 CryptGenRandom 生成 0 或 1 的随机数
int generateRandomBit() {
    // 申请一个随机数生成句柄
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        std::cerr << "CryptAcquireContext failed." << std::endl;
        return -1;  // 错误
    }

    // 生成一个随机字节
    BYTE randomByte=0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        std::cerr << "CryptGenRandom failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 8;//2
}

// 返回一个字节类型，最低的 3 位由 generateRandomBit 设置
unsigned char generateByteWithRandomBits() {
    unsigned char result = 0;  // 初始化为 0

    // 获取并设置最低的 3 位
    for (int i = 0; i < 3; ++i) {
        int bit = generateRandomBit();  // 生成 0 或 1
        result |= (bit << i);  // 将生成的 bit 放置在字节的对应位置
    }

    return result;
}

// 打印字节的二进制表示
void printBinary(unsigned char byte) {
    std::bitset<8> binary(byte);  // 将字节转换为 8 位二进制
    std::cout << binary << std::endl;
}

int main() {
    // 测试生成 10 个随机 0 或 1
    for (int i = 0; i < 10; ++i) {
        // 测试生成带有随机最低 3 位的字节
        //unsigned char byteWithRandomBits = generateByteWithRandomBits();

        // 打印结果
        //printBinary(byteWithRandomBits);
        std::cout << SYMBOLS[generateRandomBit()] << std::endl;

    }
    std::cout << std::endl;

    return 0;
}
