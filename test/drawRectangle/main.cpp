#define UNICODE
#include <Windows.h>

// VCR101	宏可以转换为 constexpr
//  改为constexpr 定义变量类型为 int
// #define MAX_LOADSTRING 100

// 全局变量:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPolygon(HDC hdc);
void DrawFilledPolygon(HDC hdc, int width, int height);
void DrawPolygonSlowly(HDC hdc, POINT* apt, int nCount);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    // 避免提示未使用变量
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    // 使用 wcscpy_s 来复制字符串
    wcscpy_s(szWindowClass, MAX_LOADSTRING, L"mainWindow");
    wcscpy_s(szTitle, MAX_LOADSTRING, L"画矩形");

    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {0};             // 初始化为零，清空所有成员
    wcex.cbSize = sizeof(WNDCLASSEXW);  // 设置结构体的大小

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_INFORMATION);

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
    TCHAR szMessage[256];  // 用来存储转换后的消息字符串
    RECT rect;
    POINT apt[10];
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // 获取窗口客户区的大小
            GetClientRect(hWnd, &rect);
            int cxClient = rect.right - rect.left;
            int cyClient = rect.bottom - rect.top;

            // 定义多边形的顶点 (aptFigure)
            static POINT aptFigure[10] = {
                {10, 70}, {50, 70}, {50, 10}, {90, 10}, {90, 50},
                {30, 50}, {30, 90}, {70, 90}, {70, 30}, {10, 30}};

            // 计算顶点的实际坐标
            for (int i = 0; i < 10; i++) {
                apt[i].x = cxClient * aptFigure[i].x / 200;
                apt[i].y = cyClient * aptFigure[i].y / 100;
            }

            // 逐点绘制并填充多边形
            DrawPolygonSlowly(hdc, apt, 10);
            EndPaint(hWnd, &ps);
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawPolygon(HDC hdc) {
    SetPolyFillMode(hdc, WINDING);  // WINDING ALTERNATE
    POINT points[] = {{50, 50}, {100, 50}, {100, 100}, {50, 100}};
    Polygon(hdc, points, 4);  // 绘制一个矩形
}

void DrawFilledPolygon(HDC hdc, int width, int height) {
    // 设置填充模式
    SetPolyFillMode(hdc, WINDING);  // 可以选择 ALTERNATE 或 WINDING

    // 定义矩形的宽高
    int rectWidth = 600;
    int rectHeight = 40;

    // 计算矩形的左上角坐标，使其居中
    int left = (width - rectWidth) / 2;
    int top = (height - rectHeight) / 2;

    // 定义矩形的四个顶点
    POINT points[] = {{left, top},
                      {left + rectWidth, top},
                      {left + rectWidth, top + rectHeight},
                      {left, top + rectHeight}};

    // 创建一个红色画刷
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));  // 红色填充

    // 选择画刷到设备上下文中
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // 绘制并填充矩形（多边形）
    Polygon(hdc, points, 4);  // 填充多边形

    // 恢复旧的画刷
    SelectObject(hdc, hOldBrush);

    // 删除画刷资源
    DeleteObject(hBrush);
}

void DrawPolygonSlowly(HDC hdc, POINT* apt, int nCount) {
    // 设置画刷
    SelectObject(hdc, GetStockObject(GRAY_BRUSH));

    // 设置填充模式
    SetPolyFillMode(hdc, ALTERNATE);

    // 逐点绘制多边形
    for (int i = 0; i < nCount; i++) {
        // 连接每对相邻的顶点
        MoveToEx(hdc, apt[i].x, apt[i].y, NULL);
        LineTo(hdc, apt[(i + 1) % nCount].x, apt[(i + 1) % nCount].y);

        // 延迟一段时间（以毫秒为单位），这里是 100 毫秒
        Sleep(1000);
    }

    // 填充多边形（完成所有边的绘制后）
    Polygon(hdc, apt, nCount);
}
