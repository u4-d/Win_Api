#include <windows.h>
#include <string>

// 定义全局变量
HINSTANCE hInst; // 当前实例句柄
HWND hTooltipWnd; // 悬浮窗口句柄
RECT targetRect = {100, 100, 300, 200}; // 指定的矩形区域

// 悬浮窗口的文字内容
std::wstring tooltipText = L"这是一个提示窗口\n包含多行文字内容\n鼠标悬浮时显示";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateTooltipWindow(HINSTANCE hInstance);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // 注册窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindowClass"; // 或使用 TEXT 宏
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    // 创建主窗口
    HWND hWnd = CreateWindow(
        TEXT("MainWindowClass"), L"鼠标悬浮提示窗口示例",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

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
    if (hTooltipWnd) return; // 防止重复创建

    // 注册悬浮窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (msg == WM_PAINT) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, tooltipText.c_str(), -1, &ps.rcPaint, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
            EndPaint(hWnd, &ps);
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    };
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TooltipWindowClass";
    wc.hbrBackground = CreateSolidBrush(RGB(0, 200, 255));//(HBRUSH)(COLOR_INFOBK + 1)
    RegisterClass(&wc);

    // 创建悬浮窗口
    hTooltipWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        TEXT("TooltipWindowClass"), NULL,
        WS_POPUP, 0, 0, 200, 100,
        NULL, NULL, hInstance, NULL);
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateTooltipWindow(hInst);
            break;

        case WM_MOUSEMOVE: {
            // 获取鼠标位置
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);

            // 判断鼠标是否在矩形区域内
            if (PtInRect(&targetRect, pt)) {
                if (hTooltipWnd) {
                    // 设置悬浮窗口的位置并显示
                    SetWindowPos(hTooltipWnd, HWND_TOPMOST, pt.x + 10, pt.y + 10, 200, 100, SWP_SHOWWINDOW);
                    InvalidateRect(hTooltipWnd, NULL, TRUE);
                }
            } else {
                // 隐藏悬浮窗口
                if (hTooltipWnd) ShowWindow(hTooltipWnd, SW_HIDE);
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
