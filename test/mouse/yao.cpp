
#include "gua.h"
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
        case WM_CREATE:
            CreateTooltipWindow(hwnd);  // 创建悬浮窗口
            guaDetail = &jieGua[3];
            break;
        case WM_SIZE: {
            InvalidateRect(hwnd, NULL, TRUE);
            // PostMessage(hwnd, WM_PAINT, wParam, lParam);
        }

        break;
        case WM_MOUSEMOVE: {
            // 获取鼠标位置
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);

            // 判断鼠标是否在目标矩形区域内
            bool showTooltip = false;
            for (int i = 0; i < 6; i++) {
                if (PtInRect(&yaoWinRect[i], pt)) {
                    showTooltip = true;
                    if (hTooltipWnd) {
                        // 使用 SetProp 存储当前的索引 i
                        SetProp(hTooltipWnd, L"TooltipIndex",
                                (HANDLE)(intptr_t)i);
                        // 设置悬浮窗口的位置并显示
                        SetWindowPos(hTooltipWnd, HWND_TOPMOST, pt.x + 20,
                                     pt.y + 10, 300, 80, SWP_SHOWWINDOW);
                        InvalidateRect(hTooltipWnd, NULL,
                                       TRUE);  // 重绘悬浮窗口
                    }
                    break;
                }
            }

            // 如果鼠标不在矩形区域内，隐藏悬浮窗口
            if (!showTooltip) {
                if (hTooltipWnd) ShowWindow(hTooltipWnd, SW_HIDE);
            }
            break;
        }
        case WM_PAINT: {
            // 创建一个设备上下文
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 创建 GDI+ Graphics 对象
            Gdiplus::Graphics graphics(hdc);
            // 擦除背景
            graphics.Clear(Gdiplus::Color(255, 255, 255));  // 清空为白色背景
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
void DrawBinaryImages(Gdiplus::Graphics* graphics, int binaryInput) {
    // 加载图像
    Gdiplus::Image yangImage(yang);
    Gdiplus::Image yinImage(yin);
    // Gdiplus::Rect imgRect(yaoXY.x, yaoXY.y, imgWidth, imgHeight);
    for (int i = 0; i < 6; i++) {
        // 取出当前二进制位的值 (从高位到低位)
        bool isYang = (binaryInput >> (5 - i)) & 1;
        // 根据二进制位选择要显示的图像
        Gdiplus::Image* imgToDraw = isYang ? &yangImage : &yinImage;
        // 绘制图像
        // graphics->DrawImage(imgToDraw, imgRect);
        // imgRect.Y -= imgHeight;
        graphics->DrawImage(imgToDraw, yaoGdiRect[i]);
    }
}

void setYaoXY(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    yaoXY.x = rc.right * 0.5;
    yaoXY.y = rc.bottom * 0.7;
}

Gdiplus::Rect ConvertRECTToRect(const RECT& winRect) {
    // 使用 RECT 的 left, top, right, bottom 转换为 Gdiplus::Rect
    return Gdiplus::Rect(winRect.left, winRect.top,
                         winRect.right - winRect.left,
                         winRect.bottom - winRect.top);
}

void setYaoRect(HWND hwnd) {
    setYaoXY(hwnd);
    int x = yaoXY.x;
    int y = yaoXY.y;
    for (size_t i = 0; i < 6; i++) {
        yaoWinRect[i] = RECT{x, y + 10, x + imgWidth, y + imgHeight - 10};
        yaoGdiRect[i] = Gdiplus::Rect(x, y, imgWidth, imgHeight);
        y -= imgHeight;
    }
}

// 创建悬浮窗口
void CreateTooltipWindow(HWND hwnd) {
    if (hTooltipWnd) return;  // 防止重复创建

    // 注册悬浮窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = TooltipWndProc;  // 悬浮窗口的过程
    wc.hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    wc.lpszClassName = L"TooltipWindowClass";
    wc.hbrBackground = CreateSolidBrush(RGB(0, 200, 255));  // 设置背景颜色
    RegisterClass(&wc);

    // 创建悬浮窗口
    hTooltipWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        TEXT("TooltipWindowClass"), NULL, WS_POPUP, 0, 0, 300, 80, NULL, NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
}

// 悬浮窗口的窗口过程函数
LRESULT CALLBACK TooltipWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            // 获取存储的索引 i
            int i = (int)(intptr_t)GetProp(hWnd, L"TooltipIndex");  //

            // 使用 i 选择不同的文本 tooltipText[i].c_str()
            if (i >= 0 && i < 6) {
                DrawText(hdc, guaDetail->yaoDetail[i].c_str(), -1, &ps.rcPaint,
                         DT_CENTER | DT_CENTER | DT_WORDBREAK);
            }
            EndPaint(hWnd, &ps);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

void mouseMove(HWND hwnd) {
    // 获取鼠标位置
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hwnd, &pt);

    // 判断鼠标是否在目标矩形区域内
    bool showTooltip = false;
    for (int i = 0; i < 6; i++) {
        if (PtInRect(&yaoWinRect[i], pt)) {
            showTooltip = true;
            if (hTooltipWnd) {
                // 使用 SetProp 存储当前的索引 i
                SetProp(hTooltipWnd, L"TooltipIndex", (HANDLE)(intptr_t)i);
                // 设置悬浮窗口的位置并显示
                SetWindowPos(hTooltipWnd, HWND_TOPMOST, pt.x + 20, pt.y + 10,
                             300, 80, SWP_SHOWWINDOW);
                InvalidateRect(hTooltipWnd, NULL,
                               TRUE);  // 重绘悬浮窗口
            }
            break;
        }
    }

    // 如果鼠标不在矩形区域内，隐藏悬浮窗口
    if (!showTooltip) {
        if (hTooltipWnd) ShowWindow(hTooltipWnd, SW_HIDE);
    }
}

gua64* findGuaByXiang(int n) {
    // 遍历jieGua数组，查找匹配的guaXiang
    for (int i = 0; i < guaLenth; ++i) {
        if (jieGua[i].guaXiang == n) {
            return &jieGua[i];  // 返回指向匹配结构体的指针
        }
    }
    return nullptr;  // 如果没有找到，返回nullptr
}
