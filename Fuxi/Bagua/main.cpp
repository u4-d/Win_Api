/*------------------------------------------------------------
   伏羲八卦
  ------------------------------------------------------------*/
// #define UNICODE

#include "bagua.h"

// 全局变量:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    // 避免提示未使用变量
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 使用 wcscpy_s 来复制字符串
    wcscpy_s(szWindowClass, MAX_LOADSTRING, L"mainWindow");
    wcscpy_s(szTitle, MAX_LOADSTRING, L"szTitle - 窗口标题");

    MyRegisterClass(hInstance);
    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }
    MSG msg;
    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (message) {
        case WM_CREATE:
            // PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC)
            // ;
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            GetClientRect(hWnd, &rect);

            DrawText(hdc, TEXT("Hello, 中文Windows 98!"), -1, &rect,
                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            // 调用绘制☵卦的函数
            DrawKanGua(hdc);
            EndPaint(hWnd, &ps);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {0};             // 初始化为零，清空所有成员
    wcex.cbSize = sizeof(WNDCLASSEXW);  // 设置结构体的大小

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    // lpszClassName 未设置会注册失败
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // 加载大图标 IDI_APPLICATION
    wcex.hIconSm =
        LoadIcon(hInstance, IDI_INFORMATION);  // 加载小图标 IDI_INFORMATION

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;  // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
                              nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


// 绘制 ☵ 卦象的函数
void DrawKanGua(HDC hdc) {
    // 创建画笔
    HPEN hPenSolid = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));  // 实线画笔（阴线）
    HPEN hPenDashed = CreatePen(PS_DASH, 2, RGB(0, 0, 0));  // 虚线画笔（阳线）

    // 保存当前画笔
    HPEN hOldPen =
        (HPEN)SelectObject(hdc, hPenDashed);  // 默认选择虚线画笔（阳线）

    // 绘制第一条断线（阳线）-- 断线使用虚线
    MoveToEx(hdc, 50, 50, NULL);
    LineTo(hdc, 150, 50);

    // 使用实线画笔绘制第二条实线（阴线）-- 实线使用实线画笔
    SelectObject(hdc, hPenSolid);
    MoveToEx(hdc, 50, 100, NULL);
    LineTo(hdc, 150, 100);

    // 使用虚线画笔绘制第三条断线（阳线）-- 断线使用虚线
    SelectObject(hdc, hPenDashed);
    MoveToEx(hdc, 50, 150, NULL);
    LineTo(hdc, 150, 150);

    // 恢复旧画笔
    SelectObject(hdc, hOldPen);

    // 清理
    DeleteObject(hPenSolid);
    DeleteObject(hPenDashed);
}

