#include "showImage.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    createMainWindow(hInstance, nCmdShow);
    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            GetClientRect(hwnd, &clientRect);
            SetTimer(hwnd, 1, 30, nullptr);  // Set a timer for animation
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            AnimateImage(hdc, imageYin, clientRect);
            // AnimateImage(hdc, imageYang, clientRect);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_TIMER:
            InvalidateRect(hwnd, nullptr, TRUE);  // Force a repaint
            return 0;

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AnimateImage(HDC hdc, const std::wstring &imagePath, RECT clientRect) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    // 根据当前帧计算缩放比例
    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    if (scale < 0.0f) scale = 0.0f;  // 确保比例不为负值

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

    ++frame;  // 增加帧计数
}

// 函数：清除窗口内容并更新
void ClearWindow(HWND hwnd) {
    // 触发窗口重绘
    InvalidateRect(hwnd, NULL,
                   TRUE);  // 参数 NULL 表示整个窗口区域，TRUE 表示要清除区域

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

void createMainWindow(HINSTANCE hInstance, int nCmdShow) {
    // Create the window
    mainHwnd = CreateWindowEx(0, CLASS_NAME, L"Image Shrink Animation",
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                              hInstance, nullptr);

    if (!mainHwnd) {
        return;
    }
    ShowWindow(mainHwnd, nCmdShow);
}