#define UNICODE
#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <bitset>

#pragma comment (lib, "Advapi32.lib")

int generateRandomBit();
void makeBits();

int main() {
    // 设置控制台输出为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    makeBits();    

    return 0;
}

// 使用 CryptGenRandom 生成 0 或 1 的随机数
int generateRandomBit() {
    // 申请一个随机数生成句柄
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return -1;  // 错误
    }

    // 生成一个随机字节
    BYTE randomByte=0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 2;
}

//左移1位后bit填补末位
void modifyInt(int& num, int bit) {
    // 将 num 左移1位
    num <<= 1;

    // 将 bit 填充到最低位
    num |= bit;  // 使用按位或操作设置最低位为 bit
}

void makeBits(){
    int num=0;
    for(int i=0;i<3;i++){
        int bit=generateRandomBit();
        std::cout << "第 " << i <<" 次 bit = " << bit << std::endl;
        modifyInt(num,bit);
    }
    std::bitset<4> binary(num);  // 将数字转换为32位的二进制
    std::cout << "10进制: " << num << " 2进制: " << binary << std::endl;
}