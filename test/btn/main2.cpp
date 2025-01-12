#include "btn.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    // 避免提示未使用变量
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    /**/
    // 注册窗口类
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Animate Image Example", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 关闭 GDI+
    GdiplusShutdown(gdiplusToken);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static RECT clientRect;
    int yinYang;
    switch (uMsg) {
        case WM_CREATE:
            GetClientRect(hwnd, &clientRect);
            createButton(hwnd, clientRect);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // 绘制图像动画
            if (isAnimating) {
                AnimateImage(hdc, imageanime, clientRect);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND:
            clieckedAnimate(hwnd, wParam);
            return 0;

        case WM_TIMER:
            onTimer(hwnd, clientRect);
            return 0;
        case WM_ANIMATION_DONE:  // 自定义消息，动画完成后执行的操作
            // 在这里添加你希望在动画完成后执行的代码
            MessageBox(hwnd, L"动画完成！", L"提示", MB_OK);
            return 0;
        case WM_MAKEGUAXIANG:
            makeGuaXiang(guaXiang, shangGua, xiaGua);
            MessageBox(NULL, guaString.c_str(), L"卦象", MB_OK);
            return 0;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        10, 10, 100, 50,        // 按钮位置和大小
        hWnd,                   // 父窗口句柄
        (HMENU)BUTTON_ANIMATE,  // 按钮ID
        hInst,                  // 实例句柄
        NULL);                  // 不需要额外数据
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
// 动画摇挂
void makeGua(HWND hwnd) {
    int yinYang = generateRandomBit();
    modifyInt(guaXiang, yinYang);
    if (yinYang == 1) {
        imageanime = imageYang;
    } else {
        imageanime = imageYin;
    }
    // 按下按钮后开始动画
    isAnimating = true;
    frame = 0;                            // 重置动画帧
    InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
    SetTimer(hwnd, 1, 30, nullptr);       // 设置定时器
    // PostMessage(hwnd, WM_TIMER, 0, 0);
}

void clieckedAnimate(HWND hwnd, WPARAM wParam) {
    if (LOWORD(wParam) == BUTTON_ANIMATE) {
        // btCnt++;
        switch (++btCnt) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                makeGua(hwnd);
                break;
            case 6:
                makeGua(hwnd);
                bt1Vis = FALSE;
                EnableWindow(bt1Hwnd, bt1Vis);
                break;
            default:

                break;
        }
    } else if (LOWORD(wParam) == BUTTON2) {
        resetBtnVis();
        EnableWindow(bt1Hwnd, bt1Vis);
    }
}

void onTimer(HWND hwnd, RECT clientRect) {
    if (isAnimating) {
        HDC hdc = GetDC(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
        AnimateImage(hdc, imageanime, clientRect);
        ReleaseDC(hwnd, hdc);

        if (frame >= maxFrames) {
            isAnimating = false;  // 动画结束
            KillTimer(hwnd, 1);   // 停止定时器
            ClearWindow(hwnd);
            if (btCnt == 6) {
                //  发送自定义消息触发后续操作
                PostMessage(hwnd, WM_MAKEGUAXIANG, 0, 0);
            }
        }
    }
}

void createButton(HWND hwnd, RECT clientRect) {
    bt1Hwnd =
        CreateWindow(L"BUTTON", L"摇挂",
                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                     10, 100, 50, hwnd, (HMENU)BUTTON_ANIMATE,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    bt2Hwnd = CreateWindow(
        L"BUTTON",                                              // 按钮类名
        L"重新来过",                                            // 按钮文本
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // 按钮样式
        10, 60, 100, 50,  // 按钮位置和大小
        hwnd,             // 父窗口句柄
        (HMENU)BUTTON2,   // 按钮ID
        hInst,            // 实例句柄
        NULL);            // 不需要额外数据
}
