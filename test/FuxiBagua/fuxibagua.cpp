#include "fuxibagua.h"

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
    const wchar_t CLASS_NAME[] = L"mainWindow";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    WNDCLASS subWc = {0};
    subWc.lpfnWndProc = SubWndProc;
    subWc.hInstance = hInstance;
    subWc.lpszClassName = L"SubWindow";
    RegisterClass(&subWc);
    // 创建窗口
    hMainWnd = CreateWindowEx(
        0, CLASS_NAME, L"伏羲八卦", WS_OVERLAPPEDWINDOW,
        (GetSystemMetrics(SM_CXSCREEN) - 800) / 2,  // X position (centered)
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,  // Y position (centered)
        800, 600,                                   // Width and Height
        nullptr, nullptr, wc.hInstance, nullptr);
    if (hMainWnd == NULL) {
        return 0;
    }

    ShowWindow(hMainWnd, nCmdShow);

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
    switch (uMsg) {
        case WM_CREATE:
            createSubWindow(hwnd);
            setRectCenter(hRight, animaCenter);
            // setRectCenter(hLeftBottom, yaoCenter);
            GetClientRect(hLeftBottom, &wordsRect);
            wordsX = wordsRect.right * 0.2f;   // 30% 宽度
            wordsY = wordsRect.bottom * 0.7f;  // 70% 高度
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND:
            return 0;
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
            setRectCenter(hRight, animaCenter);
            GetClientRect(hLeftBottom, &wordsRect);
            wordsX = wordsRect.right * 0.2f;   // 30% 宽度
            wordsY = wordsRect.bottom * 0.7f;  // 70% 高度
            clearWindow(hwnd);
        }
            return 0;

        case WM_DESTROY:
            // KillTimer(hwnd, 1);
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
            if (LOWORD(wParam) == BUTTON_ANIMATE) {
                makeGua(hRight);

            } else if (LOWORD(wParam) == BUTTON_RESET) {
                resetVar();
                EnableWindow(btHWNDAnime, bt1Vis);
            }
            return 0;
        case WM_MAKEGUAXIANG:
            // MessageBox(hwnd, L"Received WM_MAKEGUAXIANG!", L"Message",
            // MB_OK);
            showGuaImage(hRight);
            // DrawTextLines();
            return 0;
        case WM_ANIMATION_DONE:  // 自定义消息，动画完成后执行的操作
            // 在这里添加你希望在动画完成后执行的代码
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID,
                        DWORD dwTime) {
    if (isAnimating) {
        HDC hdc = GetDC(hwnd);
        AnimateImage(hdc, imageanime, clientRect);
        ReleaseDC(hwnd, hdc);

        if (frame >= maxFrames) {
            isAnimating = false;  // 动画结束
            KillTimer(hwnd, 1);   // 停止定时器
            clearWindow(hwnd);
            if (showGuaXiang) {
                PostMessage(hwnd, WM_MAKEGUAXIANG, 0, 0);
            }
        }
    }
}

void createSubWindow(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int leftWidth = width * 40 / 100;
    int rightWidth = width - leftWidth;
    int topHeight = height * 3 / 10;
    int bottomHeight = height - topHeight;

    hLeftTop =
        CreateWindow(L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                     0, leftWidth, topHeight, hwnd, (HMENU)1,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    hLeftBottom =
        CreateWindow(L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                     topHeight, leftWidth, bottomHeight, hwnd, (HMENU)2,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    hRight =
        CreateWindow(L"SubWindow", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
                     leftWidth, 0, rightWidth, height, hwnd, (HMENU)3,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    // 创建按钮
    btHWNDAnime =
        CreateWindow(L"BUTTON", L"摇挂", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                     10, 10, 100, 30, hLeftTop, (HMENU)BUTTON_ANIMATE,
                     (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    btHWNDReset = CreateWindow(
        L"BUTTON", L"重新来过", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 50,
        100, 30, hLeftTop, (HMENU)BUTTON_RESET,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
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

    // int centerX = (clientRect.right - clientRect.left) / 2;
    // int centerY = (clientRect.bottom - clientRect.top) / 2;

    int x = animaCenter.x - scaledWidth / 2;
    int y = animaCenter.y - scaledHeight / 2;

    // 绘制缩放后的图片
    if (scaledWidth > 0 && scaledHeight > 0) {
        graphics.DrawImage(&image, x, y, scaledWidth, scaledHeight);
    }

    ++frame;  // 增加帧计数
}

// 函数：清除窗口内容并更新
void clearWindow(HWND hwnd) {
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

void resetVar() {
    bt1Vis = TRUE;
    btCnt = 0;
    guaXiang = 0;
    sixYaoIdx = 5;
    GetClientRect(hLeftBottom, &wordsRect);
    wordsX = wordsRect.right * 0.2f;   // 30% 宽度
    wordsY = wordsRect.bottom * 0.7f;  // 70% 高度
    clearWindow(hLeftBottom);
    clearWindow(hRight);
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
    switch (bit) {
        case 0:
        case 2:
            num <<= 1;
            num |= 0;
            break;
        case 1:
        case 3:
            num <<= 1;
            num |= 1;
            break;
        default:
            break;
    }
}

void setRectCenter(HWND hwnd, xy& xy) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    xy.x = (rc.right - rc.left) / 2;
    xy.y = (rc.bottom - rc.top) / 2;
}
// 模拟3枚硬币摇挂
void makeGua(HWND hwnd) {
    int a, b, c;
    a = generateRandomBit();
    b = generateRandomBit();
    c = generateRandomBit();
    int yinYang = a + b + c;
    if (sixYaoIdx < 0) return;
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
    drawYao(yinYang, imageanime);
    // InvalidateRect(hwnd, nullptr, TRUE);  // 强制重绘
    SetTimer(hwnd, 1, 30, TimerProc);  // 设置定时器
    btCnt++;
    if (btCnt >= 6) {
        bt1Vis = FALSE;
        showGuaXiang = true;
        EnableWindow(btHWNDAnime, bt1Vis);
    }
}
// 按2进制反转
int reverseBits(int n) {
    int reversed = 0;

    // 保证 n 为 6 位二进制数（填充前导零）
    n = n & 0x3F;  // 0x3F = 111111（二进制），确保n只取最低6位

    // 遍历6位
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
    graphics.DrawString(yao[i].c_str(), -1, &font, PointF(wordsX, wordsY),
                        &brush);
    Image image(imagePath.c_str());
    int x = wordsX + 100;
    int y = wordsY - 5;
    graphics.DrawImage(&image, x, y, 100, 40);
    wordsY -= 40.0f;

    // 清理资源
    ReleaseDC(hLeftBottom, hdc);
    GdiplusShutdown(gdiplusToken);
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
