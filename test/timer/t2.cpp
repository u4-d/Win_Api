// 测试 左移补1
#include "myTimer.h"

void test() {
    HWND hwnd = 0;
    for (size_t i = 0; i < 6; i++) {
        makeGua(hwnd);
    }
    int a = 0b111111;
    wchar_t gx[7];
    IntToBinaryString(a, gx, 7);
    wcout << L"guaXiang: " << gx << endl;
}

int main() {
    // 设置控制台输出为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    // 设置 wcout 为宽字符模式
    _setmode(_fileno(stdout), _O_U16TEXT);
    test();

    return 0;
}
void makeGua(HWND hwnd) {
    int a, b, c;
    a = generateRandomBit();
    b = generateRandomBit();
    c = generateRandomBit();
    int yinYang = a + b + c;

    sixYao[sixYaoIdx--] = yao[yinYang];
    modifyInt(guaXiang, yinYang);
    wchar_t gx[7];
    IntToBinaryString(guaXiang, gx, 7);
    wcout << L"guaXiang: " << gx << L"yinyang: " << yinYang << endl;
    // showInt(hwnd, guaXiang, L"guaXiang: ");
    if (yinYang == 1 || yinYang == 3) {
        imageanime = imageYang;
    } else {
        imageanime = imageYin;
    }
    btCnt++;
}
// 按2进制反转
int reverseBits(int n) {
    int reversed = 0;

    // 保证 n 为 6 位二进制数（填充前导零）
    n = n & 0x3F;  // 0x3F = 111111（二进制），确保n只取最低6位

    // 遍历6位
    for (int i = 0; i < 6; i++) {
        // 取n的最低位
        int bit = n & 1;

        // 将bit移到reversed的相应位置
        reversed = (reversed << 1) | bit;

        // 将n右移一位，处理下一位
        n >>= 1;
    }

    return reversed;
}

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
    switch (bit) {
        case 0:
        case 2:
            num <<= 1;
            num |= 0;
            break;
        case 1:
        case 3:
            num <<= 1;
            num |= 1;
            break;
        default:
            break;
    }
}

// 将整数转换为二进制字符串
void IntToBinaryString(int value, wchar_t* buf, size_t bufSize) {
    buf[0] = L'\0';  // 初始化为空字符串
    size_t index = 0;
    for (int i = 5; i >= 0; --i) {
        // 使用位运算获取每一位的值
        wchar_t bit = (value & (1 << i)) ? L'1' : L'0';
        buf[index++] = bit;
        if (index == bufSize - 1) break;  // 防止溢出
    }
    buf[index] = L'\0';  // 添加字符串结束符
}
