#define UNICODE //必须定义在<windows.h>前
#include <windows.h>
#include <cmath>
#include <string>

#define PI 3.14159265
#define M_PI 3.14159265
// 全局变量:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void xuanzhuan(HWND hwnd);
// Helper function to convert degrees to radians
constexpr double DegreesToRadians(double degrees);
void DrawTextInCircle(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize);

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
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hwnd, &ps);
            //xuanzhuan(hwnd);
            // Call the function to draw text in a circle
            DrawTextInCircle(hdc, L"☰    ☱    ☲    ☳    ☴    ☵    ☶    ☷    ", 400, 400, 300, 36);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void xuanzhuan_old(HWND hwnd){
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
}

void xuanzhuan(HWND hwnd) {
    // 获取设备上下文
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    int cx = (rect.left + rect.right) / 2;  // 圆心 x
    int cy = (rect.top + rect.bottom) / 2;  // 圆心 y
    int radius = 300;  // 圆的半径

    // 要绘制的文字
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

        // 计算每个字符的旋转角度，使字符垂直于圆心
        double rotateAngle = angle * 180 / PI;  // 将角度转为度

        // 创建变换矩阵
        XFORM xForm;
        xForm.eM11 = (FLOAT)cos(rotateAngle * PI / 180);  // 旋转矩阵
        xForm.eM12 = (FLOAT)sin(rotateAngle * PI / 180);
        xForm.eM21 = (FLOAT)-sin(rotateAngle * PI / 180);
        xForm.eM22 = (FLOAT)cos(rotateAngle * PI / 180);
        xForm.eDx = 0;  // 偏移量
        xForm.eDy = 0;

        // 应用变换
        SetWorldTransform(hdc, &xForm);

        // 设置字符的基线对齐
        SIZE textSize;
        GetTextExtentPoint32(hdc, &text[i], 1, &textSize);
        
        // 绘制字符
        TextOut(hdc, x - textSize.cx / 2, y - textSize.cy / 2, &text[i], 1); // 使字符居中对齐

        // 恢复默认变换
        ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
    }

    // 释放设备上下文
    ReleaseDC(hwnd, hdc);
}

// Helper function to convert degrees to radians
constexpr double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

void DrawTextInCircle_old(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize) {
    int textLength = text.length();
    if (textLength == 0) return;

    // Calculate angle step (in degrees) for each character
    double angleStep = 360.0 / textLength;

    // Create and select the font
    HFONT hFont = CreateFont(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Draw each character
    for (int i = 0; i < textLength; ++i) {
        // Calculate the angle in radians
        double angle = DegreesToRadians(i * angleStep);

        // Calculate the position of the character
        int x = static_cast<int>(centerX + radius * cos(angle));
        int y = static_cast<int>(centerY - radius * sin(angle));

        // Calculate the rotation angle for the character
        double rotationAngle = -angleStep * i;

        // Create a rotated font
        HFONT hRotatedFont = CreateFont(
            fontSize, 0, static_cast<int>(rotationAngle * 10),
            0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
            L"Arial"
        );

        SelectObject(hdc, hRotatedFont);

        // Draw the character
        TextOut(hdc, x, y, &text[i], 1);

        // Cleanup
        DeleteObject(hRotatedFont);
    }

    // Restore original font
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

void DrawTextInCircle_old1(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize) {
    int textLength = text.length();
    if (textLength == 0) return;

    // Calculate angle step (in degrees) for each character
    double angleStep = 360.0 / textLength;

    // Get screen resolution to ensure perfect circle
    int dpiX = GetDeviceCaps(hdc, HORZRES);
    int dpiY = GetDeviceCaps(hdc, VERTRES);

    // Create and select the font
    HFONT hFont = CreateFont(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Draw each character
    for (int i = 0; i < textLength; ++i) {
        // Calculate the angle in radians
        double angle = DegreesToRadians(i * angleStep);

        // Calculate the position of the character
        int x = static_cast<int>(centerX + radius * cos(angle) * dpiY / dpiX);
        int y = static_cast<int>(centerY - radius * sin(angle));

        // Calculate the rotation angle for the character
        int rotationAngle = static_cast<int>(-angle * 180.0 / M_PI + 90);

        // Create a rotated font
        HFONT hRotatedFont = CreateFont(
            fontSize, 0, rotationAngle * 10, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
            L"Arial"
        );

        SelectObject(hdc, hRotatedFont);

        // Draw the character
        TextOut(hdc, x, y, &text[i], 1);

        // Cleanup
        DeleteObject(hRotatedFont);
    }

    // Restore original font
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}
//左转 文字下侧朝外
void DrawTextInCircle(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize) {
    int textLength = text.length();
    if (textLength == 0) return;

    // 每个字符的角度间隔（弧度制）
    double angleStep = 2 * M_PI / textLength;

    // 创建基础字体
    HFONT hFont = CreateFont(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    for (int i = 0; i < textLength; ++i) {
        // 当前字符的角度（弧度）
        double angle = i * angleStep;

        // 计算字符绘制的位置
        int x = static_cast<int>(centerX + radius * cos(angle));
        int y = static_cast<int>(centerY - radius * sin(angle));

        // 计算字符旋转角度（度数），确保垂直于圆心
        int rotationAngle = static_cast<int>(angle * 180.0 / M_PI + 90);

        // 创建旋转字体
        HFONT hRotatedFont = CreateFont(
            fontSize, 0, rotationAngle * 10, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
            L"Arial"
        );
        SelectObject(hdc, hRotatedFont);

        // 绘制字符
        WCHAR character = text[i];
        TextOut(hdc, x, y, &character, 1);

        // 删除旋转字体
        DeleteObject(hRotatedFont);
    }

    // 恢复原始字体
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

