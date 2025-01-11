#include "bits.h"

int main() {
    // 设置控制台输出为UTF-8编码
    _setmode(_fileno(stdout), _O_U16TEXT);  // 设置标准输出为宽字符模式
    SetConsoleOutputCP(CP_UTF8);

    test1();
    // std::wcout << L"按任意键继续...\n";
    // std::cin.get();  // 等待用户输入
    return 0;
}
void test1() {
    int n = 0;
    for (size_t i = 0; i < 6; i++) {
        int b = generateRandomBit();
        modifyInt(n, b);
        // std::wcout << "i: " << i << "n: " << n << "b: " << b << std::endl;
    }
    makeGuaXiang(n, xiaGua, shangGua);
    std::wcout << L"上挂: " << shangGua << L"下挂: " << xiaGua << std::endl;
    std::wcout << L"卦象: " << guaString << std::endl;
    std::wcout.flush();  // 手动刷新缓冲区
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
    hight = (x >> 3) & 7;      // 右移 3 位后取最低 3 位
    std::bitset<3> bLow(low);  // 转换为32位二进制表示
    std::bitset<3> bHight(hight);
    std::wcout << L"x: " << x << L" hight: " << bHight << L" low: " << bLow
               << std::endl;
    std::wcout.flush();  // 手动刷新缓冲区
    getBaGua(hight, shangWords, shangSyb);
    getBaGua(low, xiaWords, xiaSyb);
    /*
    if (!shangWords.empty() && !shangSyb.empty() && !xiaWords.empty() &&
        !xiaSyb.empty()) {
        std::wcout << "shangWords: " << shangWords[0]
                   << "shangSyb: " << shangSyb << std::endl;
        std::wcout << "xiaWords: " << xiaWords << "xiaSyb: " << xiaSyb
                   << std::endl;
    } else {
        std::wcout << "未找到对应的卦象" << std::endl;
    }
    */
    // 使用 std::wstring 来拼接
    guaString = shangWords + shangSyb + xiaWords + xiaSyb;
}
// 函数：根据参数 bits 查找对应的 words 和 syb
void getBaGua_old(int bits, wchar_t*& words, wchar_t*& syb) {
    for (const auto& item : baGua) {
        if (item.bits == bits) {
            // words = item.words;
            // syb = item.syb;
            return;
        }
    }
    words = nullptr;
    syb = nullptr;
}
// 函数：根据参数 bits 查找对应的 words 和 syb
void getBaGua(int bits, std::wstring& words, std::wstring& syb) {
    for (const auto& item : baGua) {
        /*
        std::wcout << L"检查bits: " << item.bits << L", 卦象: " << item.words
                   << L", 符号: " << item.syb << std::endl;
        */
        if (item.bits == bits) {
            words = item.words;
            syb = item.syb;
            /**/
            std::wcout << L"匹配到bits: " << bits << L", 卦象: " << words
                       << L", 符号: " << syb << std::endl;

            return;
        }
    }
    std::wcout << L"未找到匹配的bits: " << bits << std::endl;
    words.clear();  // 清空字符串
    syb.clear();    // 清空字符串
}
