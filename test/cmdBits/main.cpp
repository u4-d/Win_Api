#include "myBits.h"
void test1() {
    int n = 0;
    for (size_t i = 0; i < 6; i++) {
        int b = generateRandomBit();
        modifyInt(n, b);
        // std::wcout << "i: " << i << "n: " << n << "b: " << b << std::endl;
    }
    makeGuaXiang(n, xiaGua, shangGua);

    std::wcout << L"上挂: " << shangGua << L"下挂: " << xiaGua
               << std::endl;                            // 不会乱码
    std::wcout << L"卦象: " << guaString << std::endl;//乱码
}
int main() {
    // 设置标准输出流为 UTF-16 编码
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) {
        std::wcerr << L"Failed to set mode for stdout!" << std::endl;
        return 1;  // 错误处理
    }
    
    test1();
    //std::wcout << L"按任意键继续...\n";
    // std::cin.get();  // 等待用户输入
    delete[] guaString;
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

void makeGuaXiang(int x, int& low, int& hight) {
    // 提取低 3 位
    low = x & 7;  // 0b111 (7) 用于保留最低 3 位
    // 提取 4~6 位（右移 3 位后，取接下来的 3 位）
    hight = (x >> 3) & 7;      // 右移 3 位后取最低 3 位
    std::bitset<3> bLow(low);  // 转换为32位二进制表示
    std::bitset<3> bHight(hight);
    std::wcout << L"x: " << x << L" hight: " << bHight << L" low: " << bLow
               << std::endl;
    //std::wcout.flush();  // 手动刷新缓冲区
    getBaGua_C(hight, shangWords, shangSyb);
    getBaGua_C(low, xiaWords, xiaSyb);

    // 使用 std::wstring 来拼接
    //guaString = shangWords + shangSyb + xiaWords + xiaSyb;
    // 确保目标缓冲区有足够的空间
    size_t len1 = wcslen(shangWords);
    size_t len2 = wcslen(shangSyb);
    size_t len3 = wcslen(xiaWords);
    size_t len4 = wcslen(xiaSyb);
  // +1 是为终止符'\0'分配空间
    guaString = new wchar_t[len1 + len2 + len3 + len4 + 10];

    // 使用swprintf格式化拼接
    swprintf(guaString, len1 + len2 + len3 + len4 + 10, L"上挂:%s %s 下挂:%s %s", shangWords,
             shangSyb, xiaWords, xiaSyb);
    //std::wcout << L"卦象: " << guaString << std::endl;
}
// 函数：根据参数 bits 查找对应的 words 和 syb 
void getBaGua_C(int bits, const wchar_t*& words, const wchar_t*& syb) {
    for (const auto& item : baGua) {
        if (item.bits == bits) {
            words = item.words;  // words 现在是 const wchar_t* 类型
            syb = item.syb;      // syb 也是 const wchar_t* 类型
            return;
        }
    }
    words = nullptr;
    syb = nullptr;
}
// 函数：根据参数 bits 查找对应的 words 和 syb
void getBaGua_S(int bits, std::wstring& words, std::wstring& syb) {
    for (const auto& item : baGua) {
        /*
        std::wcout << L"检查bits: " << item.bits << L", 卦象: " << item.words
                   << L", 符号: " << item.syb << std::endl;
        */
        if (item.bits == bits) {
            words = item.words;
            //syb = item.syb;
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