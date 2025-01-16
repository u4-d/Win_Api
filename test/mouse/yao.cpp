
#include "a.h"
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    // 初始化 GDI+
    InitializeGDIPlus();

    // 创建窗口类
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd =
        CreateWindowEx(0, CLASS_NAME, L"GDI+ DrawImage Example",
                       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800,
                       600, nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 关闭 GDI+
    ShutdownGDIPlus();

    return (int)msg.wParam;
}

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            // 创建一个设备上下文
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 创建 GDI+ Graphics 对象
            Gdiplus::Graphics graphics(hdc);

            // 假设传入的二进制值为 101010 (26)
            int binaryInput = 26;
            setYaoRect(hwnd);
            //  绘制二进制图像
            DrawBinaryImages(&graphics, binaryInput);

            // 结束绘制
            EndPaint(hwnd, &ps);
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);  // 销毁窗口时退出消息循环
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// 关闭 GDI+
void ShutdownGDIPlus() { GdiplusShutdown(gdiplusToken); }

void InitializeGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

// 绘制图像的函数
void DrawBinaryImages_old(Gdiplus::Graphics* graphics, int binaryInput) {
    // 加载图像
    Image yangImage(yang);
    Image yinImage(yin);

    // 计算每个图像的绘制位置，从下往上
    for (int i = 0; i < 6; i++) {
        int x = yaoXY.x;  // 固定的 X 坐标，绘制在窗口的中间
        int y = yaoXY.y - (i * imgHeight);  // 每个图像的 Y 坐标，从下往上

        // 取出当前二进制位的值 (从高位到低位)
        bool isYang = (binaryInput >> (5 - i)) & 1;

        // 根据二进制位选择要显示的图像
        Image* imgToDraw = isYang ? &yangImage : &yinImage;

        // 绘制图像
        graphics->DrawImage(imgToDraw, x, y, imgWidth, imgHeight);
    }
}

void DrawBinaryImages(Gdiplus::Graphics* graphics, int binaryInput) {
    // 加载图像
    Image yangImage(yang);
    Image yinImage(yin);
    Gdiplus::Rect imgRect(yaoXY.x, yaoXY.y, imgWidth, imgHeight);
    for (int i = 0; i < 6; i++) {
        // 取出当前二进制位的值 (从高位到低位)
        bool isYang = (binaryInput >> (5 - i)) & 1;
        // 根据二进制位选择要显示的图像
        Image* imgToDraw = isYang ? &yangImage : &yinImage;
        // 绘制图像
        graphics->DrawImage(imgToDraw, imgRect);
        imgRect.Y -= imgHeight;
    }
}

void setYaoXY(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    yaoXY.x = rc.right * 0.5;
    yaoXY.y = rc.bottom * 0.7;
}

Rect ConvertRECTToRect(const RECT& winRect) {
    // 使用 RECT 的 left, top, right, bottom 转换为 Gdiplus::Rect
    return Rect(winRect.left, winRect.top, winRect.right - winRect.left,
                winRect.bottom - winRect.top);
}
void setYaoRect(HWND hwnd) {
    setYaoXY(hwnd);
    int x = yaoXY.x;
    int y = yaoXY.y;
    for (size_t i = 0; i < 6; i++) {
        yaoWinRect[i].left = x;
        yaoWinRect[i].right = x + imgWidth;
        yaoWinRect[i].bottom = y;
        yaoWinRect[i].top = y - imgHeight;
        yaoGdiRect[i] = ConvertRECTToRect(yaoWinRect[i]);
        y -= imgHeight;
    }
}