#include "rect.h"
// http://zhangwenli.com/biangua/#010101
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    hInst = hInstance;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    createMainWindow(hInstance, nCmdShow);
    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // 获取窗口矩形
            setRect();
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // 绘制图像动画
            if (isAnimating) {
                AnimateImage(hdc, imageanime);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_COMMAND:
            clieckedAnimate(hwnd, wParam);
            return 0;
        case WM_SIZE:
            // 窗口大小改变时强制重绘
            // ClearWindow(hwnd);
            setRect();
            // InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        case WM_TIMER:
            onTimer(hwnd);
            return 0;
        case WM_MAKEGUAXIANG:
            showGuaImage(hwnd);
            DrawTextLines();
            return 0;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AnimateImage(HDC hdc, const std::wstring& imagePath) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());
    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();
    //  根据当前帧计算缩放比例
    float scale = 1.0f - static_cast<float>(frame) / maxFrames;
    if (scale < 0.0f) scale = 0.0f;  // 确保比例不为负值

    int scaledWidth = static_cast<int>(imgWidth * scale);
    int scaledHeight = static_cast<int>(imgHeight * scale);

    int centerX = (animaRect.right - animaRect.left) / 2;
    int centerY = (animaRect.bottom - animaRect.top) / 2;

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
    // 触发窗口重绘 参数 NULL 表示整个窗口区域，TRUE 表示要清除区域
    // InvalidateRect(hwnd, &animaRect, TRUE);  //

    // 获取窗口的客户端区域
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);  // 获取绘图设备上下文

    // 使用背景颜色清除窗口
    // RECT rc;
    // GetClientRect(hwnd, &animaRect);  // 获取窗口的客户区矩形
    FillRect(hdc, &animaRect,
             (HBRUSH)(COLOR_WINDOW + 1));  // 使用窗口背景色填充窗口

    // 结束绘制
    EndPaint(hwnd, &ps);
}

void createMainWindow(HINSTANCE hInstance, int nCmdShow) {
    // Create the window
    mainHwnd = CreateWindowEx(0, CLASS_NAME, L"伏羲八卦", WS_OVERLAPPEDWINDOW,
                              clientRect.left, clientRect.top,
                              clientRect.right - clientRect.left,
                              clientRect.bottom - clientRect.top, nullptr,
                              nullptr, hInstance, nullptr);
    if (!mainHwnd) {
        return;
    }
    createButton(mainHwnd, clientRect);
    ShowWindow(mainHwnd, nCmdShow);
    UpdateWindow(mainHwnd);
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

void showGuaImage(HWND hwnd) {
    if (GetWindowRect(hwnd, &clientRect)) {
        // 计算窗口中心点的坐标
        shangImgPos.p.x =
            (clientRect.right - clientRect.left) / 2 - imgGuaSize.width - 10;
        shangImgPos.p.y =
            (clientRect.bottom - clientRect.top) / 2 - imgGuaSize.height * 0.75;
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

void onTimer(HWND hwnd) {
    if (isAnimating) {
        HDC hdc = GetDC(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
        AnimateImage(hdc, imageanime);
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

void resetBtnVis() {
    bt1Vis = TRUE;
    btCnt = 0;
    guaXiang = -1;
    sixYaoIdx = 5;
    ClearWindow(mainHwnd);
}

void DrawTextLines() {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // 创建图形对象
    HDC hdc = GetDC(mainHwnd);
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
    ReleaseDC(mainHwnd, hdc);
    GdiplusShutdown(gdiplusToken);
}

void setRect() {
    GetClientRect(mainHwnd, &clientRect);
    // 计算 clientRect 的宽度和高度
    int totalWidth = clientRect.right - clientRect.left;
    int totalHeight = clientRect.bottom - clientRect.top;

    // 按照 4:6 的比例分配宽度
    int wordsWidth = totalWidth * 4 / 10;  // 左侧区域宽度
    int animaWidth = totalWidth - wordsWidth;  // 右侧区域宽度（剩余部分）

    // 设置左侧区域 wordsRect（占 4 部分宽度）
    wordsRect.left = clientRect.left;
    wordsRect.top = clientRect.top;
    wordsRect.right = clientRect.left + wordsWidth;
    wordsRect.bottom = clientRect.bottom;

    // 设置右侧区域 animaRect（占 6 部分宽度）
    animaRect.left = wordsRect.right;  // 右侧区域从 wordsRect 的右边开始
    animaRect.top = clientRect.top;
    animaRect.right = clientRect.right;  // 右侧区域宽度到 clientRect 的右边
    animaRect.bottom = clientRect.bottom;
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
