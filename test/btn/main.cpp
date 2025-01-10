// 按钮是否可以按下

#include "btn.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    // 避免提示未使用变量
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    // 使用 wcscpy_s 来复制字符串
    wcscpy_s(szWindowClass, MAX_LOADSTRING, L"mainWindow");
    wcscpy_s(szTitle, MAX_LOADSTRING, L"按钮控制");

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

    HWND hWnd =
        CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                     0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }
    // 创建按钮
    bt1Hwnd = CreateWindow(
        L"BUTTON",                                              // 按钮类名
        L"Say Hello",                                           // 按钮文本
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
        10, 10, 100, 50,  // 按钮位置和大小
        hWnd,             // 父窗口句柄
        (HMENU)BUTTON1,   // 按钮ID
        hInst,            // 实例句柄
        NULL);            // 不需要额外数据
    /**/
    bt2Hwnd = CreateWindow(
        L"BUTTON",                                              // 按钮类名
        L"重新来过",                                            // 按钮文本
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
        10, 60, 100, 50,  // 按钮位置和大小
        hWnd,             // 父窗口句柄
        (HMENU)BUTTON2,   // 按钮ID
        hInst,            // 实例句柄
        NULL);            // 不需要额外数据

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
        case WM_CREATE: {
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // 获取窗口客户区的大小

            EndPaint(hWnd, &ps);
        } break;
        case WM_COMMAND:                      // 处理按钮点击事件
            if (LOWORD(wParam) == BUTTON1) {  // 按钮的ID是1
                btCnt++;
                // 按下6次后解卦
                if (btCnt >= 6) {
                    bt1Vis = FALSE;
                    EnableWindow(bt1Hwnd, bt1Vis);
                    makeGuaXiang(guaXiang, shangGua, xiaGua);
                    MessageBox(NULL, guaString.c_str(), L"卦象", MB_OK);
                } else {
                    modifyInt(guaXiang, generateRandomBit());
                    sayhello();
                }

            } else if (LOWORD(wParam) == BUTTON2) {
                resetBtnVis();
                EnableWindow(bt1Hwnd, bt1Vis);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 定义函数 sayhello()
void sayhello() {
    // 创建一个足够大的缓冲区来存储二进制字符串
    wchar_t buf[7];  // 32 位整数 + 结束符

    // 将整数转换为二进制字符串
    IntToBinaryString(guaXiang, buf, sizeof(buf));
    MessageBox(NULL, buf, L"卦象", MB_OK);
}

void resetBtnVis() {
    bt1Vis = TRUE;
    btCnt = 0;
    guaXiang = 0;
}

// 使用 CryptGenRandom 生成 0 或 1 的随机数
int generateRandomBit() {
    // 申请一个随机数生成句柄
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return -1;  // 错误
    }

    // 生成一个随机字节
    BYTE randomByte = 0;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // 错误
    }

    // 释放加密上下文
    CryptReleaseContext(hCryptProv, 0);

    // 将随机字节转换为 0 或 1
    return randomByte % 2;
}

// 左移1位后bit填补末位
void modifyInt(int& num, int bit) {
    if (btCnt <= 6) {
        // 将 num 左移1位
        num <<= 1;

        // 将 bit 填充到最低位
        num |= bit;  // 使用按位或操作设置最低位为 bit
    }
}

// 将整数转换为二进制字符串
void IntToBinaryString(int value, wchar_t* buf, size_t bufSize) {
    buf[0] = L'\0';  // 初始化为空字符串
    size_t index = 0;
    for (int i = 5; i >= 0; --i) {
        // 使用位运算获取每一位的值
        wchar_t bit = (value & (1 << i)) ? L'1' : L'0';
        buf[index++] = bit;
        if (index == bufSize - 1) break;  // 防止溢出
    }
    buf[index] = L'\0';  // 添加字符串结束符
}

void makeGuaXiang(int x, int& low, int& hight) {
    // 提取低 3 位
    low = x & 7;  // 0b111 (7) 用于保留最低 3 位
    // 提取 4~6 位（右移 3 位后，取接下来的 3 位）
    hight = (x >> 3) & 7;  // 右移 3 位后取最低 3 位
    getBaGua(hight, shangWords, shangSyb);
    getBaGua(low, xiaWords, xiaSyb);
    // 使用 std::wstring 来拼接
    guaString = std::wstring(shangWords) + shangSyb + xiaWords + xiaSyb;
}
// 函数：根据参数 bits 查找对应的 words 和 syb
void getBaGua(int bits, wchar_t*& words, wchar_t*& syb) {
    for (const auto& item : baGua) {
        if (item.bits == bits) {
            words = item.words;
            syb = item.syb;
            return;
        }
    }
    words = nullptr;
    syb = nullptr;
}
// 函数：清除窗口内容并更新
void ClearWindow(HWND hwnd) {
    // 触发窗口重绘
    InvalidateRect(hwnd, NULL,
                   TRUE);  // 参数 NULL 表示整个窗口区域，TRUE 表示要清除区域

    // 获取窗口的客户端区域
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);  // 获取绘图设备上下文

    // 使用背景颜色清除窗口
    RECT rc;
    GetClientRect(hwnd, &rc);  // 获取窗口的客户区矩形
    FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));  // 使用窗口背景色填充窗口

    // 结束绘制
    EndPaint(hwnd, &ps);
}

void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    // 根据当前帧计算缩放比例
    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    if (scale < 0.0f) scale = 0.0f;  // 确保比例不为负值

    int scaledWidth = static_cast<int>(imgWidth * scale);
    int scaledHeight = static_cast<int>(imgHeight * scale);

    int centerX = (clientRect.right - clientRect.left) / 2;
    int centerY = (clientRect.bottom - clientRect.top) / 2;

    int x = centerX - scaledWidth / 2;
    int y = centerY - scaledHeight / 2;

    // 绘制缩放后的图片
    if (scaledWidth > 0 && scaledHeight > 0) {
        graphics.DrawImage(&image, x, y, scaledWidth, scaledHeight);
    }

    ++frame;  // 增加帧计数
}