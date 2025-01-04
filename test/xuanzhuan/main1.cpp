#define UNICODE
#include <windows.h>
#include <cmath>
#include <string>

#define PI 3.14159265
// 全局变量:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  // 避免提示未使用变量
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // 使用 wcscpy_s 来复制字符串
  wcscpy_s(szWindowClass, MAX_LOADSTRING, L"mainWindow");
  wcscpy_s(szTitle, MAX_LOADSTRING, L"旋转字体");

  MyRegisterClass(hInstance);
  // 执行应用程序初始化:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }
  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int)msg.wParam;
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
  wcex.hIcon =
      LoadIcon(hInstance, IDI_APPLICATION);  // 加载大图标 IDI_APPLICATION
  wcex.hIconSm =
      LoadIcon(hInstance, IDI_INFORMATION);  // 加载小图标 IDI_INFORMATION

  return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // 将实例句柄存储在全局变量中

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HDC hdcMem;
    static HBITMAP hbmMem;
    
    switch (uMsg) {
        case WM_PAINT:
        {
            // 获取设备上下文
            HDC hdc = GetDC(hwnd);
            RECT rect;
            GetClientRect(hwnd, &rect);

            // 设置旋转角度（以角度为单位，转为弧度）
            XFORM xForm;
            xForm.eM11 = (FLOAT)cos(30 * 3.14159 / 180);  // 旋转矩阵的值
            xForm.eM12 = (FLOAT)sin(30 * 3.14159 / 180);
            xForm.eM21 = (FLOAT)-sin(30 * 3.14159 / 180);
            xForm.eM22 = (FLOAT)cos(30 * 3.14159 / 180);
            xForm.eDx = 0;  // 不平移
            xForm.eDy = 0;  // 不平移

            // 设置旋转
            SetWorldTransform(hdc, &xForm);

            // 设置文本
            const WCHAR* text = L"旋转的文字";
            SetTextColor(hdc, RGB(255, 0, 0));  // 设置文字颜色为红色
            SetBkMode(hdc, TRANSPARENT);        // 设置背景透明
            int len=(int) wcslen(text);
            // 绘制旋转后的文字
            TextOut(hdc, 100, 100, text, len);

            // 恢复旋转
            ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
            
            // 释放设备上下文
            ReleaseDC(hwnd, hdc);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}