#define UNICODE
#include <windows.h>
#include <wincrypt.h>
#include <gdiplus.h>
#include <string>

#pragma comment (lib, "gdiplus.lib") // Crypt32.lib
#pragma comment (lib, "Advapi32.lib")

using namespace Gdiplus;

// 定义按钮的 ID
#define BUTTON_ANIMATE 1001

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect);
int generateRandomBit();

// 动画状态变量
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 注册窗口类
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Animate Image Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 关闭 GDI+
    GdiplusShutdown(gdiplusToken);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::wstring imageYin = L"./img/yin.jpg"; // 替换为你的图片路径
    static std::wstring imageYang = L"./img/yang.jpg";
    static std::wstring imageanime=L"";
    static RECT clientRect;

    switch (uMsg) {
    case WM_CREATE:
        GetClientRect(hwnd, &clientRect);
        CreateWindow(L"BUTTON", L"Animate Image", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                     10, 10, 150, 30, hwnd, (HMENU)BUTTON_ANIMATE, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == BUTTON_ANIMATE) {
            int yinYang=generateRandomBit();
            if (yinYang==1){
                imageanime=imageYang;
            }else{
                imageanime=imageYin;
            }
            
            // 按下按钮后开始动画
            isAnimating = true;
            frame = 0; // 重置动画帧
            SetTimer(hwnd, 1, 30, nullptr); // 设置定时器
        }
        return 0;

    case WM_TIMER:
        if (isAnimating) {
            HDC hdc = GetDC(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE); // 强制重绘
            AnimateImage(hdc, imageanime, clientRect);
            ReleaseDC(hwnd, hdc);

            if (frame >= maxFrames) {
                isAnimating = false; // 动画结束
                KillTimer(hwnd, 1); // 停止定时器
            }
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        //std::cerr << "CryptAcquireContext failed." << std::endl;
        return -1;  // 错误
    }

    // 生成一个随机字节
    BYTE randomByte=0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        //std::cerr << "CryptGenRandom failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 2;
}

