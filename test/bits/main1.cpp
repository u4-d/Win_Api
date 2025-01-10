#include "bits.h"

int main() {
    // 设置控制台输出为UTF-8编码
    // SetConsoleOutputCP(CP_UTF8);
    // 设置 wcout 为宽字符模式
    //_setmode(_fileno(stdout), _O_U16TEXT);
    // makeBits();
    // makeFuxiBagua();
    test1();
    return 0;
}
void test1() {
    int n = 0;
    for (size_t i = 0; i < 6; i++) {
        int b = generateRandomBit();
        modifyInt(n, b);
        std::cout << "i: " << i << "n: " << n << "b: " << b << std::endl;
    }
    makeGuaXiang(n, xiaGua, shangGua);
    std::wcout << "上挂: " << shangGua << "下挂: " << xiaGua << std::endl;
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

void makeGuaXiang(int x, int& low, int& hight) {
    // 提取低 3 位
    low = x & 7;  // 0b111 (7) 用于保留最低 3 位
    // 提取 4~6 位（右移 3 位后，取接下来的 3 位）
    hight = (x >> 3) & 7;  // 右移 3 位后取最低 3 位
    std::cout << "x: " << x << "low: " << low << "hight: " << hight
              << std::endl;
    getBaGua(hight, shangWords, shangSyb);
    getBaGua(low, xiaWords, xiaSyb);
    // 使用 std::wstring 来拼接
    guaString = std::wstring(shangWords) + shangSyb + xiaWords + xiaSyb;
}
// 函数：根据参数 bits 查找对应的 words 和 syb
void getBaGua(int bits, wchar_t*& words, wchar_t*& syb) {
    for (const auto& item : baGua) {
        if (item.bits == bits) {
            words = item.words;
            syb = item.syb;
            return;
        }
    }
    words = nullptr;
    syb = nullptr;
}
