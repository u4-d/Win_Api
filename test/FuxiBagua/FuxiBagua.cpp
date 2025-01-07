// FuxiBagua.cpp : 定义应用程序的入口点。
//

#include "FuxiBagua.h"

#include <cmath>

#include "framework.h"
#define M_PI 3.14159265358979323846  // 手动定义 M_PI
#define MAX_LOADSTRING 100
// 定义每个 Unicode 字符的常量
const wchar_t SYMBOLS[] = {
    L'\u2630',  // ☰
    L'\u2631',  // ☱
    L'\u2632',  // ☲
    L'\u2633',  // ☳
    L'\u2634',  // ☴
    L'\u2635',  // ☵
    L'\u2636',  // ☶
    L'\u2637'   // ☷
};
// 全局变量:
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void DrawCrossLines(HDC hdc, int centerX, int centerY, int distance,
                    int lineLength);
void DrawGuaxiang(HDC hdc, double angle);
int generateRandomBit();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FUXIBAGUA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable =
        LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FUXIBAGUA));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FUXIBAGUA));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FUXIBAGUA);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
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

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    switch (message) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd,
                              About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // 获取窗口的客户区中心点
            RECT rect;
            GetClientRect(hWnd, &rect);
            int centerX = (rect.right - rect.left) / 2;
            int centerY = (rect.bottom - rect.top) / 2;

            // 调用绘图函数，参数为中心点、距离和线长
            DrawCrossLines(hdc, centerX, centerY, 200, 50);
            DrawGuaxiang(hdc, 45.0);
            EndPaint(hWnd, &ps);
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

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

// 画线
void DrawCrossLines(HDC hdc, int centerX, int centerY, int distance,
                    int lineLength) {
    // 实线画笔
    HPEN solidPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    // 虚线画笔
    HPEN dashedPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));

    // 保存原始画笔
    HGDIOBJ originalPen = SelectObject(hdc, solidPen);

    // 上下左右实线
    MoveToEx(hdc, centerX - lineLength / 2, centerY - distance, nullptr);  // 上
    LineTo(hdc, centerX + lineLength / 2, centerY - distance);

    MoveToEx(hdc, centerX - lineLength / 2, centerY + distance, nullptr);  // 下
    LineTo(hdc, centerX + lineLength / 2, centerY + distance);

    MoveToEx(hdc, centerX - distance, centerY - lineLength / 2, nullptr);  // 左
    LineTo(hdc, centerX - distance, centerY + lineLength / 2);

    MoveToEx(hdc, centerX + distance, centerY - lineLength / 2, nullptr);  // 右
    LineTo(hdc, centerX + distance, centerY + lineLength / 2);

    // 切换到虚线画笔
    SelectObject(hdc, dashedPen);

    // 对角线 (右上、右下、左下、左上) 虚线，确保与中心点成 90 度夹角
    double diagonalDistance = distance / 1.414;  // 对角线距离中心点 2 倍
    int halfLength = lineLength / 2;

    // 右上
    MoveToEx(hdc, centerX + diagonalDistance - halfLength,
             centerY - diagonalDistance - halfLength, nullptr);
    LineTo(hdc, centerX + diagonalDistance + halfLength,
           centerY - diagonalDistance + halfLength);

    // 右下
    MoveToEx(hdc, centerX + diagonalDistance - halfLength,
             centerY + diagonalDistance + halfLength, nullptr);
    LineTo(hdc, centerX + diagonalDistance + halfLength,
           centerY + diagonalDistance - halfLength);

    // 左下

    MoveToEx(hdc, centerX - diagonalDistance - halfLength,
             centerY + diagonalDistance - halfLength, nullptr);
    LineTo(hdc, centerX - diagonalDistance + halfLength,
           centerY + diagonalDistance + halfLength);
    // 左上
    MoveToEx(hdc, centerX - diagonalDistance - halfLength,
             centerY - diagonalDistance + halfLength, nullptr);
    LineTo(hdc, centerX - diagonalDistance + halfLength,
           centerY - diagonalDistance - halfLength);

    // 恢复原始画笔并清理资源
    SelectObject(hdc, originalPen);
    DeleteObject(solidPen);
    DeleteObject(dashedPen);
}

void DrawGuaxiang(HDC hdc, double angle) {
    // 创建字体
    HFONT hFont =
        CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH, L"Arial");
    SelectObject(hdc, hFont);

    // 设置旋转角度，单位是度
    // double angle = 45.0;

    // 创建矩阵变换，旋转中心为 (x, y)
    POINT pt = {200, 100};  // 设置旋转中心点为窗口中的某个位置
    XFORM xForm;
    xForm.eM11 = (FLOAT)cos(angle * M_PI / 180.0);  // 旋转矩阵
    xForm.eM12 = (FLOAT)sin(angle * M_PI / 180.0);
    xForm.eM21 = (FLOAT)-sin(angle * M_PI / 180.0);
    xForm.eM22 = (FLOAT)cos(angle * M_PI / 180.0);
    xForm.eDx = (FLOAT)(pt.x - pt.x * xForm.eM11 - pt.y * xForm.eM21);
    xForm.eDy = (FLOAT)(pt.y - pt.x * xForm.eM12 - pt.y * xForm.eM22);

    // 应用变换
    SetWorldTransform(hdc, &xForm);

    // 设置绘制区域和文本
    wchar_t up = SYMBOLS[generateRandomBit()];
    wchar_t down = SYMBOLS[generateRandomBit()];
    // 创建一个包含这两个字符的字符数组
    wchar_t text[3];  // 包含 2 个字符 + 1 个结束符 '\0'
    text[0] = up;
    text[1] = down;
    text[2] = L'\0';  // 终止符
    // LPCWSTR text = L"Rotated Text 45 Degrees";
    RECT rect = {50, 50, 500, 100};

    // 使用 TextOut 绘制旋转后的文本
    TextOutW(hdc, 100, 100, text, lstrlenW(text));
}

// 使用 CryptGenRandom 生成 0 到 7 的随机数
int generateRandomBit() {
    // 申请一个随机数生成句柄
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        // std::cerr << "CryptAcquireContext failed." << std::endl;
        return -1;  // 错误
    }

    // 生成一个随机字节
    BYTE randomByte = 0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        // std::cerr << "CryptGenRandom failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 8;  // 2
}