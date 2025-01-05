#define UNICODE
#include <windows.h>
#include <wincrypt.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <sstream>

#pragma comment (lib, "gdiplus.lib") // Crypt32.lib
#pragma comment (lib, "Advapi32.lib")

using namespace Gdiplus;

// 定义按钮的 ID
#define BUTTON_ANIMATE 1001
std::vector<int> randomNumbers;
std::vector<int> randomBits;
int frame = 0;
const int maxFrames = 100;
const wchar_t* BAGUAWORDS=  L"坎    巽    乾    兑    离    震    坤    艮    ";//汉字卦象
const wchar_t* BAGUASYMBOLS=L"☵    ☴    ☰    ☱    ☲    ☳    ☷    ☶    ";//符号卦象

struct Hexagram {
    int upperTrigram;
    int lowerTrigram;
    const wchar_t* name;
    const wchar_t* image;
};

// 八卦卦名表
const Hexagram HEXAGRAM_TABLE[] = {
    {2, 2, L"乾为天", L"天上天"},
    {6, 6, L"坤为地", L"地下地"},
    {0, 5, L"水雷屈", L"云雷之象"},
    // 其他卦象省略，需完整填写
};

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect);
int generateRandomBit();
void ClearWindow(HWND hWnd);
int getTrigram(const std::vector<int>& bits);
void updateDisplay(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Hexagram Window";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Hexagram Generator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            CreateWindow(L"BUTTON", L"Generate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                         10, 10, 150, 30, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                if (randomBits.size() < 6) {
                    randomBits.push_back(generateRandomBit());
                }
                if (randomBits.size() == 6) {
                    // 若随机数已达 6 个，则禁用按钮
                    EnableWindow((HWND)lParam, FALSE);
                }
                updateDisplay(hwnd);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void updateDisplay(HWND hwnd) {
    std::wstringstream displayText;

    if (randomBits.size() >= 3) {
        std::vector<int> upperBits(randomBits.begin(), randomBits.begin() + 3);
        int upperTrigram = getTrigram(upperBits);
        displayText << L"上卦: " << BAGUAWORDS[upperTrigram] << L" " << BAGUASYMBOLS[upperTrigram] << L"\n";
    }
    if (randomBits.size() == 6) {
        std::vector<int> lowerBits(randomBits.begin() + 3, randomBits.end());
        int lowerTrigram = getTrigram(lowerBits);
        displayText << L"下卦: " << BAGUAWORDS[lowerTrigram] << L" " << BAGUASYMBOLS[lowerTrigram] << L"\n";

        // 计算卦名和卦象
        for (const auto& hexagram : HEXAGRAM_TABLE) {
            if (hexagram.upperTrigram == getTrigram({randomBits[0], randomBits[1], randomBits[2]}) &&
                hexagram.lowerTrigram == getTrigram({randomBits[3], randomBits[4], randomBits[5]})) {
                displayText << L"卦名: " << hexagram.name << L"\n";
                displayText << L"卦象: " << hexagram.image;
                break;
            }
        }
    }

    SetWindowText(hwnd, displayText.str().c_str());
}

void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    // 根据当前帧计算缩放比例
    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    if (scale < 0.0f) scale = 0.0f; // 确保比例不为负值

    int scaledWidth = static_cast<int>(imgWidth * scale);
    int scaledHeight = static_cast<int>(imgHeight * scale);

    int centerX = (clientRect.right - clientRect.left) / 2;
    int centerY = (clientRect.bottom - clientRect.top) / 2;

    int x = centerX - scaledWidth / 2;
    int y = centerY - scaledHeight / 2;

    // 绘制缩放后的图片
    if (scaledWidth > 0 && scaledHeight > 0) {
        graphics.DrawImage(&image, x, y, scaledWidth, scaledHeight);
    }

    ++frame; // 增加帧计数
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

// 函数：清除窗口内容并更新
void ClearWindow(HWND hwnd)
{
    // 触发窗口重绘
    InvalidateRect(hwnd, NULL, TRUE);  // 参数 NULL 表示整个窗口区域，TRUE 表示要清除区域

    // 获取窗口的客户端区域
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);  // 获取绘图设备上下文

    // 使用背景颜色清除窗口
    RECT rc;
    GetClientRect(hwnd, &rc);  // 获取窗口的客户区矩形
    FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));  // 使用窗口背景色填充窗口

    // 结束绘制
    EndPaint(hwnd, &ps);
}

int getTrigram(const std::vector<int>& bits) {
    return bits[0] * 4 + bits[1] * 2 + bits[2];
}

