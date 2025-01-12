#include <windows.h>
#include <gdiplus.h>
#include <string>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// 全局变量
HWND hLeftTop, hLeftBottom, hRight;
bool isAnimating = false;
ULONG_PTR gdiplusToken;

// 函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubWndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTextLines(HDC hdc);
void AnimateImage(HDC hdc, const std::wstring& imagePath);

// 程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindow";
    RegisterClass(&wc);

    WNDCLASS subWc = {0};
    subWc.lpfnWndProc = SubWndProc;
    subWc.hInstance = hInstance;
    subWc.lpszClassName = L"SubWindow";
    RegisterClass(&subWc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"Window Layout Example",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

// 主窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            int leftWidth = width * 40 / 100;
            int rightWidth = width - leftWidth;
            int topHeight = height * 3 / 10;
            int bottomHeight = height - topHeight;

            hLeftTop = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0,
                leftWidth, topHeight, hwnd, (HMENU)1,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hLeftBottom = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                topHeight, leftWidth, bottomHeight, hwnd, (HMENU)2,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hRight = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
                leftWidth, 0, rightWidth, height, hwnd, (HMENU)3,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            // 创建按钮
            CreateWindow(
                L"BUTTON", L"Animate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 10, 100, 30, hLeftTop, (HMENU)101,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            CreateWindow(
                L"BUTTON", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10,
                50, 100, 30, hLeftTop, (HMENU)102,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
            return 0;
        }

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            int leftWidth = width * 40 / 100;
            int rightWidth = width - leftWidth;
            int topHeight = height * 3 / 10;
            int bottomHeight = height - topHeight;

            MoveWindow(hLeftTop, 0, 0, leftWidth, topHeight, TRUE);
            MoveWindow(hLeftBottom, 0, topHeight, leftWidth, bottomHeight,
                       TRUE);
            MoveWindow(hRight, leftWidth, 0, rightWidth, height, TRUE);

            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 子窗口过程
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 101) {  // Animate 按钮
                isAnimating = true;
                InvalidateRect(hRight, NULL, TRUE);
                InvalidateRect(hLeftBottom, NULL, TRUE);
            } else if (LOWORD(wParam) == 102) {  // Reset 按钮
                isAnimating = false;
                InvalidateRect(hRight, NULL, TRUE);
                InvalidateRect(hLeftBottom, NULL, TRUE);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (hwnd == hLeftBottom) {
                if (isAnimating) {
                    DrawTextLines(hdc);
                }
            } else if (hwnd == hRight) {
                if (isAnimating) {
                    AnimateImage(hdc, L"example.png");  // 替换为实际图片路径
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 绘制文字
void DrawTextLines(HDC hdc) {
    const wchar_t* text = L"Animation is running!";
    RECT rect;
    GetClientRect(hLeftBottom, &rect);
    DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 动画绘制
void AnimateImage(HDC hdc, const std::wstring& imagePath) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    RECT rect;
    GetClientRect(hRight, &rect);

    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    int x = centerX - imgWidth / 2;
    int y = centerY - imgHeight / 2;

    graphics.DrawImage(&image, x, y, imgWidth, imgHeight);
}
