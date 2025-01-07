#define UNICODE
#include <fcntl.h>
#include <io.h>
#include <wincrypt.h>
#include <windows.h>

#include <bitset>
#include <iostream>

#pragma comment(lib, "Advapi32.lib")
const wchar_t* BAGUAWORDS =
    L"坎    巽    乾    兑    离    震    坤    艮    ";  // 汉字卦象
const wchar_t* BAGUASYMBOLS =
    L"☵    ☴    ☰    ☱    ☲    ☳    ☷    ☶    ";  // 符号卦象
struct Gua {
    int bits;
    wchar_t* words;  // std::wstring
    wchar_t* syb;    // wchar_t*
};
Gua baGua[]{
    {0b000, L"坤", L"☷"}, {0b100, L"艮", L"☶"},

    {0b010, L"坎", L"☵"}, {0b110, L"巽", L"☴"},

    {0b001, L"震", L"☳"}, {0b101, L"离", L"☲"},

    {0b011, L"兑", L"☱"}, {0b111, L"乾", L"☰"},
};

int generateRandomBit();
void makeBits();
void makeFuxiBagua();
void modifyInt(int& num, int bit);

int main() {
    // 设置控制台输出为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    // 设置 wcout 为宽字符模式
    _setmode(_fileno(stdout), _O_U16TEXT);
    // makeBits();
    makeFuxiBagua();

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
    BYTE randomByte = 0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 2;
}

// 左移1位后bit填补末位
void modifyInt(int& num, int bit) {
    // 将 num 左移1位
    num <<= 1;

    // 将 bit 填充到最低位
    num |= bit;  // 使用按位或操作设置最低位为 bit
}

void makeBits() {
    int num = 0;
    for (int i = 0; i < 3; i++) {
        int bit = generateRandomBit();
        std::cout << "第 " << i << " 次 bit = " << bit << std::endl;
        modifyInt(num, bit);
    }
    std::bitset<4> binary(num);  // 将数字转换为32位的二进制
    std::cout << "10进制: " << num << " 2进制: " << binary << std::endl;
}

void makeFuxiBagua() {
    int cnt = 8;
    for (int i = 0; i < cnt; i++) {
        for (int j = 0; j < cnt; j++) {
            std::wcout << baGua[j].words << baGua[i].words << " "
                       << baGua[j].syb << " " << baGua[i].syb << L"  ";
        }
        // 换行
        std::wcout << std::endl;
    }
}
