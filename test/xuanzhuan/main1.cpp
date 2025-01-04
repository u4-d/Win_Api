#define UNICODE //必须定义在<windows.h>前
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
    switch (uMsg) {
        case WM_PAINT:
        {
            // 获取设备上下文
            HDC hdc = GetDC(hwnd);
            RECT rect;
            GetClientRect(hwnd, &rect);
            int cx = (rect.left + rect.right) / 2;  // 圆心 x
            int cy = (rect.top + rect.bottom) / 2;  // 圆心 y
            int radius = 300;  // 圆的半径

            // 要绘制的文字
            //std::string text = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
            std::wstring text = L"将这段文字打印成一个圆圈,无法合拢也没关系";
            
            int textLength = text.length();

            // 设置文字属性
            SetTextColor(hdc, RGB(0, 0, 255)); // 文字颜色
            SetBkMode(hdc, TRANSPARENT);        // 背景透明

            for (int i = 0; i < textLength; ++i) {
                // 计算每个字符的位置
                double angle = 2 * PI * i / textLength; // 计算角度
                int x = cx + static_cast<int>(radius * cos(angle)); // 计算 x 坐标
                int y = cy + static_cast<int>(radius * sin(angle)); // 计算 y 坐标

                // 设置字符旋转的角度，使字符垂直于圆心
                TEXTMETRIC tm;
                GetTextMetrics(hdc, &tm);
                double rotateAngle = angle * 180 / PI;

                // 创建变换矩阵
                XFORM xForm;
                xForm.eM11 = (FLOAT)cos(rotateAngle * PI / 180);
                xForm.eM12 = (FLOAT)sin(rotateAngle * PI / 180);
                xForm.eM21 = (FLOAT)-sin(rotateAngle * PI / 180);
                xForm.eM22 = (FLOAT)cos(rotateAngle * PI / 180);
                xForm.eDx = 0;
                xForm.eDy = 0;

                // 应用变换
                SetWorldTransform(hdc, &xForm);

                // 绘制字符
                TextOut(hdc, x, y, &text[i], 1);
            }

            // 恢复默认变换
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

