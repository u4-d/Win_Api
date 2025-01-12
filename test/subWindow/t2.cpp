#include "sWindow.h"

// 程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindow";
    RegisterClass(&wc);

    WNDCLASS subWc = {0};
    subWc.lpfnWndProc = SubWndProc;
    subWc.hInstance = hInstance;
    subWc.lpszClassName = L"SubWindow";
    RegisterClass(&subWc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"Window Layout Example",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

// 主窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            int leftWidth = width * 40 / 100;
            int rightWidth = width - leftWidth;
            int topHeight = height * 3 / 10;
            int bottomHeight = height - topHeight;

            hLeftTop = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0,
                leftWidth, topHeight, hwnd, (HMENU)1,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hLeftBottom = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                topHeight, leftWidth, bottomHeight, hwnd, (HMENU)2,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hRight = CreateWindow(
                L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
                leftWidth, 0, rightWidth, height, hwnd, (HMENU)3,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            // 创建按钮
            CreateWindow(
                L"BUTTON", L"Animate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 10, 100, 30, hLeftTop, (HMENU)BUTTON_ANIMATE,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            CreateWindow(
                L"BUTTON", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10,
                50, 100, 30, hLeftTop, (HMENU)BUTTON_RESET,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
            return 0;
        }

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            int leftWidth = width * 40 / 100;
            int rightWidth = width - leftWidth;
            int topHeight = height * 3 / 10;
            int bottomHeight = height - topHeight;

            MoveWindow(hLeftTop, 0, 0, leftWidth, topHeight, TRUE);
            MoveWindow(hLeftBottom, 0, topHeight, leftWidth, bottomHeight,
                       TRUE);
            MoveWindow(hRight, leftWidth, 0, rightWidth, height, TRUE);

            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 子窗口过程
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == BUTTON_ANIMATE) {  // Animate 按钮
                isAnimating = true;
                InvalidateRect(hRight, NULL, TRUE);
                InvalidateRect(hLeftBottom, NULL, TRUE);
            } else if (LOWORD(wParam) == BUTTON_RESET) {  // Reset 按钮
                isAnimating = false;
                InvalidateRect(hRight, NULL, TRUE);
                InvalidateRect(hLeftBottom, NULL, TRUE);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (hwnd == hLeftBottom) {
                if (isAnimating) {
                    drawYao(1, L"./img/yin.jpg");
                }
            } else if (hwnd == hRight) {
                if (isAnimating) {
                    AnimateImage(hdc, L"example.png");  // 替换为实际图片路径
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
    } else if (LOWORD(wParam) == BUTTON_RESET) {
        resetBtnVis();
        EnableWindow(bt1Hwnd, bt1Vis);
    }
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

void drawYao(int i, const std::wstring& imagePath) {
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

    // 循环输出每行文字
    graphics.DrawString(yao[i].c_str(), -1, &font, PointF(textX, textY),
                        &brush);
    Image image(imagePath.c_str());
    int x = textX + 100;
    int y = textY - 5;
    graphics.DrawImage(&image, x, y, 100, 40);
    textY -= 40.0f;

    // 清理资源
    ReleaseDC(hLeftBottom, hdc);
    GdiplusShutdown(gdiplusToken);
}
