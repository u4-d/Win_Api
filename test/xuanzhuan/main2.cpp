//摩天轮 旋转
#define UNICODE //必须定义在<windows.h>前
#include <windows.h>
#include <cmath>
#include <string>

#define PI 3.14159265
#define M_PI 3.14159265
// 全局变量:
const wchar_t SYMBOLS[] = {
    L'\u2635',  // ☵ 坎
    L'\u2634',  // ☴ 巽
    L'\u2630',  // ☰ 乾
    L'\u2631',  // ☱ 兑
    L'\u2632',  // ☲ 离
    L'\u2633',  // ☳ 震
    L'\u2637',   // ☷ 坤    
    L'\u2636',  // ☶ 艮    
};
const wchar_t* BAGUAWORDS=  L"坎    巽    乾    兑    离    震    坤    艮    ";
const wchar_t* BAGUASYMBOLS1=L"☵    ☴    ☰    ☱    ☲    ☳    ☷    ☶    ";
const wchar_t* BAGUASYMBOLS=L"\u2635    \u2634    \u2630    \u2631    \u2632    \u2633    \u2637    \u2636    ";
// 全局变量，分别存储两个文字的当前旋转角度
double rotationAngle=0.0;
double angleWords = 0.0;
double angleSymbols = 0.0;
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
void DrawRotatingTextInCircle(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize, bool isClockwise);

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
    static bool isClockwise = true; // 默认顺时针旋转
    bool sysTrun=false;
    switch (uMsg) {
        case WM_CREATE:
            // 设置定时器，每 30ms 触发一次
            SetTimer(hwnd, 1, 30, NULL);
            break;

        case WM_TIMER:
            // 相同旋转
            //rotationAngle += (isClockwise ? 0.05 : -0.05); // 控制旋转速度
            
            // 顺时针更新角度
            angleWords += 0.05;  // 调整旋转速度
            if (angleWords >= 2 * M_PI) angleWords -= 2 * M_PI;

            // 逆时针更新角度
            angleSymbols -= 0.05; // 调整旋转速度
            if (angleSymbols <= -2 * M_PI) angleSymbols += 2 * M_PI;

            InvalidateRect(hwnd, NULL, TRUE); // 触发重绘
            break;
        case WM_PAINT:
        {
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hwnd, &ps);
            // Call the function to draw text in a circle  ☰    ☱    ☲    ☳    ☴    ☵    ☶    ☷    
            //DrawTextInCircle(hdc, BAGUAWORDS, 400, 350, 300, 42);
            //DrawTextInCircle(hdc, BAGUASYMBOLS, 400, 350, 250, 42);

            // 绘制八卦文字和符号
            DrawRotatingTextInCircle(hdc, BAGUAWORDS, 400, 350, 300, 42, isClockwise);
            DrawRotatingTextInCircle(hdc, BAGUASYMBOLS, 400, 350, 250, 42, sysTrun);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN:
            // 按键切换旋转方向（左箭头逆时针，右箭头顺时针）
            if (wParam == VK_LEFT) {
                isClockwise = false;
                sysTrun=true;
            } else if (wParam == VK_RIGHT) {
                isClockwise = true;
                sysTrun=false;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// Helper function to convert degrees to radians
constexpr double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
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
        //字体上侧朝向圆心
        //int rotationAngle = static_cast<int>(angle * 180.0 / M_PI + 90);
        //字体下侧朝向圆心
        int rotationAngle = static_cast<int>(angle  * 180.0 / M_PI +270);
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

// 函数：绘制旋转文字
void DrawRotatingTextInCircle_old(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize, bool isClockwise) {
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
        // 当前字符的角度（弧度），根据旋转方向调整
        double angle = rotationAngle + (isClockwise ? 1 : -1) * i * angleStep;

        // 计算字符绘制的位置
        int x = static_cast<int>(centerX + radius * cos(angle));
        int y = static_cast<int>(centerY - radius * sin(angle));

        // 计算字符旋转角度（度数），确保垂直于圆心
        int rotationDeg = static_cast<int>(angle * 180.0 / M_PI + 270);

        // 创建旋转字体
        HFONT hRotatedFont = CreateFont(
            fontSize, 0, rotationDeg * 10, 0, FW_NORMAL, FALSE, FALSE, FALSE,
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

void DrawRotatingTextInCircle(HDC hdc, const std::wstring& text, int centerX, int centerY, int radius, int fontSize, bool isClockwise) {
    int textLength = text.length();
    if (textLength == 0) return;

    // 每个字符的角度间隔（弧度制）
    double angleStep = 2 * M_PI / textLength;

    // 根据旋转方向更新角度
    double& currentAngle = isClockwise ? angleWords : angleSymbols;

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
        double angle = currentAngle + i * angleStep;

        // 计算字符绘制的位置
        int x = static_cast<int>(centerX + radius * cos(angle));
        int y = static_cast<int>(centerY - radius * sin(angle));

        // 计算字符旋转角度（度数），确保垂直于圆心
        int rotationAngle = static_cast<int>(angle * 180.0 / M_PI + 270);

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

