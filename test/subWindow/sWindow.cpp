#include "sWindow.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;  // 主窗口过程
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        return 0;
    }

    // 注册子窗口的窗口类
    wc.lpfnWndProc = SubWndProc;
    wc.lpszClassName = CHILD_CLASS_NAME;
    if (!RegisterClass(&wc)) {
        return 0;
    }

    // 在屏幕中心创建主窗口
    hMainWnd = CreateWindowEx(
        0, CLASS_NAME, L"伏羲八卦", WS_OVERLAPPEDWINDOW,
        (GetSystemMetrics(SM_CXSCREEN) - 1000) / 2,  // X position (centered)
        (GetSystemMetrics(SM_CYSCREEN) - 800) / 2,   // Y position (centered)
        1000, 800,                                   // Width and Height
        nullptr, nullptr, wc.hInstance, nullptr);

    if (!hMainWnd) {
        return 0;
    }

    ShowWindow(hMainWnd, SW_SHOW);
    UpdateWindow(hMainWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE: {
            // 获取主窗口的客户区大小
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // 计算宽度和高度
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;

            // 左侧区域宽度（40%），右侧区域宽度（60%）
            int leftWidth = width * 40 / 100;
            int rightWidth = width - leftWidth;  // 右侧占 60%

            // 上下区域的高度比例（上3下7）
            int topHeight = height * 3 / 10;
            int bottomHeight = height - topHeight;

            // 调整子窗口的位置和大小
            // 左上窗口
            MoveWindow(hLeftTop, 0, 0, leftWidth, topHeight, TRUE);

            // 左下窗口
            MoveWindow(hLeftBottom, 0, topHeight, leftWidth, bottomHeight,
                       TRUE);

            // 右侧窗口
            MoveWindow(hRight, leftWidth, 0, rightWidth, height, TRUE);

            return 0;
        }

        case WM_CREATE: {
            // 创建子窗口：左上
            hLeftTop = CreateWindowEx(0, CHILD_CLASS_NAME, L"Left Top",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0,
                                      0, 0, hwnd, nullptr,
                                      GetModuleHandle(nullptr), nullptr);

            // 创建子窗口：左下
            hLeftBottom = CreateWindowEx(0, CHILD_CLASS_NAME, L"Left Bottom",
                                         WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                                         0, 0, 0, hwnd, nullptr,
                                         GetModuleHandle(nullptr), nullptr);

            // 创建子窗口：右侧
            hRight = CreateWindowEx(0, CHILD_CLASS_NAME, L"Right Pane",
                                    WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0,
                                    0, hwnd, nullptr, GetModuleHandle(nullptr),
                                    nullptr);
            createButton(hLeftTop);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 子窗口过程（这里只是为了演示，可以在这里自定义每个子窗口的功能）
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    // 根据窗口句柄判断当前是哪个子窗口
    if (hwnd == hLeftTop) {
        // 处理左上子窗口的消息
        switch (uMsg) {
            case WM_COMMAND:
                onCliecked(hRight, wParam);
                return 0;
            case WM_PAINT:
                // 绘制左上窗口内容
                break;
                // 其他左上窗口的消息处理
        }
    } else if (hwnd == hLeftBottom) {
        // 处理左下子窗口的消息
        switch (uMsg) {
            case WM_PAINT:
                // 绘制左下窗口内容
                break;
                // 其他左下窗口的消息处理
        }
    } else if (hwnd == hRight) {
        // 处理右侧子窗口的消息
        switch (uMsg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hRight, &ps);
                // 绘制图像动画
                if (isAnimating) {
                    AnimateImage(hdc, imageanime);  // 假设这是一个动画函数
                }
                EndPaint(hRight, &ps);
            }
                return 0;
            // 其他右侧窗口的消息处理
            case WM_COMMAND:
                onCliecked(hRight, wParam);
                return 0;
            case WM_TIMER:
                onTimer(hRight);
                break;
            case WM_MAKEGUAXIANG:
                showGuaImage(hRight);
                DrawTextLines();
                KillTimer(hRight, 1);
                break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 函数：清除窗口内容并更新
void clearWindow(HWND hwnd) {
    // 触发窗口重绘 参数 NULL 表示整个窗口区域，TRUE 表示要清除区域
    // InvalidateRect(hwnd, &animaRect, TRUE);  //
    // 获取窗口的客户端区域
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);  // 获取绘图设备上下文
    // 使用背景颜色清除窗口
    RECT rc;
    GetClientRect(hwnd, &rc);  // 获取窗口的客户区矩形
    // 使用窗口背景色填充窗口
    FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
    // 结束绘制
    EndPaint(hwnd, &ps);
}

void createButton(HWND hwnd) {
    bt1Hwnd =
        CreateWindow(L"BUTTON", L"摇挂",
                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                     10, 100, 50, hwnd, (HMENU)BUTTON_ANIMATE,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    bt2Hwnd = CreateWindow(
        L"BUTTON", L"重新来过",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 60, 100, 50,
        hwnd, (HMENU)BUTTON2,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // 实例句柄
        NULL);  // 不需要额外数据
}

void onCliecked(HWND hwnd, WPARAM wParam) {
    if (LOWORD(wParam) == BUTTON_ANIMATE) {
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

void AnimateImage(HDC hdc, const std::wstring& imagePath) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();

    // 根据当前帧计算缩放比例
    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    if (scale < 0.0f) scale = 0.0f;  // 确保比例不为负值

    int scaledWidth = static_cast<int>(imgWidth * scale);
    int scaledHeight = static_cast<int>(imgHeight * scale);
    RECT rc;
    GetClientRect(hRight, &rc);
    int centerX = (rc.right - rc.left) / 2;
    int centerY = (rc.bottom - rc.top) / 2;

    int x = centerX - scaledWidth / 2;
    int y = centerY - scaledHeight / 2;

    // 绘制缩放后的图片
    if (scaledWidth > 0 && scaledHeight > 0) {
        graphics.DrawImage(&image, x, y, scaledWidth, scaledHeight);
    }

    ++frame;  // 增加帧计数
}

void resetBtnVis() {
    bt1Vis = TRUE;
    btCnt = 0;
    guaXiang = -1;
    sixYaoIdx = 5;
    clearWindow(hRight);
    clearWindow(hLeftBottom);
}

// 模拟3枚硬币摇挂
void makeGua(HWND hwnd) {
    int a, b, c;
    a = generateRandomBit();
    b = generateRandomBit();
    c = generateRandomBit();
    int yinYang = a + b + c;
    sixYao[sixYaoIdx--] = yao[yinYang];
    modifyInt(guaXiang, yinYang);
    if (yinYang == 1 || yinYang == 3) {
        imageanime = imageYang;
    } else {
        imageanime = imageYin;
    }
    // 按下按钮后开始动画
    isAnimating = true;
    frame = 0;  // 重置动画帧
    // InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
    SetTimer(hwnd, 1, 30, nullptr);  // 设置定时器
    // PostMessage(hwnd, WM_TIMER, 0, 0);
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

void onTimer(HWND hwnd) {
    if (isAnimating) {
        HDC hdc = GetDC(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
        AnimateImage(hdc, imageanime);
        ReleaseDC(hwnd, hdc);

        if (frame >= maxFrames) {
            isAnimating = false;  // 动画结束
            KillTimer(hwnd, 1);   // 停止定时器
            clearWindow(hwnd);
            if (btCnt == 6) {
                //  发送自定义消息触发后续操作
                PostMessage(hwnd, WM_MAKEGUAXIANG, 0, 0);
            }
        }
    }
}

void showGuaImage(HWND hwnd) {
    RECT rc;
    if (GetWindowRect(hRight, &rc)) {
        // 计算窗口中心点的坐标
        shangImgPos.p.x = (rc.right - rc.left) / 2 - imgGuaSize.width - 10;
        shangImgPos.p.y = (rc.bottom - rc.top) / 2 - imgGuaSize.height * 0.75;
        shangImgPos.s = imgGuaSize;
        xiaImgPos.p.x = shangImgPos.p.x + imgGuaSize.width + 5;
        xiaImgPos.p.y = shangImgPos.p.y;
        xiaImgPos.s = imgGuaSize;
        int newGx = reverseBits(guaXiang);
        // 提取 4~6 位（右移 3 位后，取接下来的 3 位）
        int hight = (newGx >> 3) & 7;  // 右移 3 位后取最低 3 位
                                       // 提取低 3 位
        int low = newGx & 7;           // 0b111 (7) 用于保留最低 3 位

        // 在窗口绘制时显示图片
        DisplayImage(hwnd, imgFileName[hight],
                     shangImgPos);  // 替换为实际的图片路径
        DisplayImage(hwnd, imgFileName[low],
                     xiaImgPos);  // 替换为实际的图片路径
    } else {
        // 如果获取失败，打印错误
        MessageBox(hwnd, L"GetWindowRect失败!", L"提示", MB_OK);
    }
}

// 按2进制反转
int reverseBits(int n) {
    int reversed = 0;

    // 遍历每一位，最多6位
    for (int i = 0; i < 6; i++) {
        // 取n的最低位
        int bit = n & 1;

        // 将bit移到reversed的相应位置
        reversed = (reversed << 1) | bit;

        // 将n右移一位，处理下一位
        n >>= 1;
    }

    return reversed;
}

// 初始化 GDI+ 和显示图片
void DisplayImage(HWND hwnd, const std::wstring& imagePath, guaPosSize ps) {
    InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建图形对象
    HDC hdc = GetDC(hwnd);
    Graphics graphics(hdc);

    // 加载图片
    Image image(imagePath.c_str());

    // 绘制图像到指定位置和大小
    graphics.DrawImage(&image, ps.p.x, ps.p.y, ps.s.width, ps.s.height);

    // 清理资源
    ReleaseDC(hwnd, hdc);
    GdiplusShutdown(gdiplusToken);
}

void DrawTextLines() {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // 创建图形对象
    HDC hdc = GetDC(hLeftBottom);
    Graphics graphics(hdc);
    // 创建一个字体对象，选择大小和字体
    Font font(L"Arial", 18);                  // 选择字体和字号
    SolidBrush brush(Color(255, 0, 0, 255));  // 蓝色画刷

    // 文本输出的起始位置
    float x = 200.0f;  // X 坐标
    float y = 100.0f;  // Y 坐标

    // 循环输出每行文字
    for (int i = 0; i < 6; ++i) {
        graphics.DrawString(sixYao[i].c_str(), -1, &font, PointF(x, y), &brush);
        y += 30.0f;  // 每行之间的间距
    }
    // 清理资源
    ReleaseDC(hLeftBottom, hdc);
    GdiplusShutdown(gdiplusToken);
}
