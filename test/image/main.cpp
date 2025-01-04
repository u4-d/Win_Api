#define UNICODE
#include <windows.h>
#include <gdiplus.h>
#include <string>

#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Register the window class
    const wchar_t CLASS_NAME[] = L"ImageShrinkWindow";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Image Shrink Animation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::wstring imagePath = L"yin.bmp"; // Replace with your image path
    static RECT clientRect;

    switch (uMsg) {
    case WM_CREATE:
        GetClientRect(hwnd, &clientRect);
        SetTimer(hwnd, 1, 30, nullptr); // Set a timer for animation
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        AnimateImage(hdc, imagePath, clientRect);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_TIMER:
        InvalidateRect(hwnd, nullptr, TRUE); // Force a repaint
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect) {
    static int frame = 0;
    static const int maxFrames = 100;

    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    scale = max(scale, 0.1f); // Prevent scale from becoming too small

    int scaledWidth = static_cast<int>(imgWidth * scale);
    int scaledHeight = static_cast<int>(imgHeight * scale);

    int centerX = (clientRect.right - clientRect.left) / 2;
    int centerY = (clientRect.bottom - clientRect.top) / 2;

    int x = centerX - scaledWidth / 2;
    int y = centerY - scaledHeight / 2;

    graphics.DrawImage(&image, x, y, scaledWidth, scaledHeight);

    if (frame < maxFrames) {
        ++frame;
    }
}
