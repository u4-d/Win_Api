// 不使用lamda表达式
// 窗口中画4个矩形,像人行横道那样,横着的,当鼠标移动到这4个矩形里面时显示悬浮窗口

#include <windows.h>
#include <string>

// 定义全局变量
HINSTANCE hInst;   // 当前实例句柄
HWND hTooltipWnd;  // 悬浮窗口句柄
RECT targetRects[4] = {
    // 定义四个矩形区域（像人行横道）
    {100, 100, 300, 130},  // 第一个矩形
    {100, 160, 300, 190},  // 第二个矩形
    {100, 220, 300, 250},  // 第三个矩形
    {100, 280, 300, 310}   // 第四个矩形
};

// 悬浮窗口的文字内容
std::wstring tooltipText =
    L"这是一个提示窗口\n包含多行文字内容\n鼠标悬浮时显示";

// 窗口过程函数声明
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TooltipWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam);  // 悬浮窗口的窗口过程
void CreateTooltipWindow(HINSTANCE hInstance);
void DrawRectangles(HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // 注册主窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;  // 主窗口过程函数
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 背景颜色
    RegisterClass(&wc);

    // 创建主窗口
    HWND hWnd = CreateWindow(TEXT("MainWindowClass"), L"鼠标悬浮提示窗口示例",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// 创建悬浮窗口
void CreateTooltipWindow(HINSTANCE hInstance) {
    if (hTooltipWnd) return;  // 防止重复创建

    // 注册悬浮窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = TooltipWndProc;  // 悬浮窗口的过程
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TooltipWindowClass";
    wc.hbrBackground = CreateSolidBrush(RGB(0, 200, 255));  // 设置背景颜色
    RegisterClass(&wc);

    // 创建悬浮窗口
    hTooltipWnd =
        CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
                       TEXT("TooltipWindowClass"), NULL, WS_POPUP, 0, 0, 200,
                       100, NULL, NULL, hInstance, NULL);
}

// 悬浮窗口的窗口过程函数
LRESULT CALLBACK TooltipWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, tooltipText.c_str(), -1, &ps.rcPaint,
                     DT_CENTER | DT_VCENTER | DT_WORDBREAK);
            EndPaint(hWnd, &ps);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

// 主窗口的窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateTooltipWindow(hInst);  // 创建悬浮窗口
            break;

        case WM_MOUSEMOVE: {
            // 获取鼠标位置
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);

            // 判断鼠标是否在目标矩形区域内
            bool showTooltip = false;
            for (int i = 0; i < 4; ++i) {
                if (PtInRect(&targetRects[i], pt)) {
                    showTooltip = true;
                    if (hTooltipWnd) {
                        // 设置悬浮窗口的位置并显示
                        SetWindowPos(hTooltipWnd, HWND_TOPMOST, pt.x + 10,
                                     pt.y + 10, 200, 100, SWP_SHOWWINDOW);
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
            // 绘制四个矩形
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawRectangles(hdc);
            EndPaint(hWnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);  // 销毁窗口时退出消息循环
            break;

        default:
            return DefWindowProc(hWnd, message, wParam,
                                 lParam);  // 默认处理消息
    }
    return 0;
}

// 绘制四个矩形的函数
void DrawRectangles(HDC hdc) {
    for (int i = 0; i < 4; ++i) {
        // 绘制矩形
        Rectangle(hdc, targetRects[i].left, targetRects[i].top,
                  targetRects[i].right, targetRects[i].bottom);
    }
}
