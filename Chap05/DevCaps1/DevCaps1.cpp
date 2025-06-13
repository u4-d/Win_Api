/*---------------------------------------------------------
   DEVCAPS1.C -- Device Capabilities Display Program No. 1
                 (c) Charles Petzold, 1998
  ---------------------------------------------------------*/
// 中文备注
#define UNICODE
#include <windows.h>

#define NUMLINES ((int)(sizeof devcaps / sizeof devcaps[0]))

struct {
    int iIndex;
    const TCHAR* szLabel;
    const TCHAR* szDesc;
} devcaps[] = {
    HORZSIZE,    TEXT("HORZSIZE"),    TEXT("Width in millimeters:"),
    VERTSIZE,    TEXT("VERTSIZE"),    TEXT("Height in millimeters:"),
    HORZRES,     TEXT("HORZRES"),     TEXT("Width in pixels:"),
    VERTRES,     TEXT("VERTRES"),     TEXT("Height in raster lines:"),
    BITSPIXEL,   TEXT("BITSPIXEL"),   TEXT("Color bits per pixel:"),
    PLANES,      TEXT("PLANES"),      TEXT("Number of color planes:"),
    NUMBRUSHES,  TEXT("NUMBRUSHES"),  TEXT("Number of device brushes:"),
    NUMPENS,     TEXT("NUMPENS"),     TEXT("Number of device pens:"),
    NUMMARKERS,  TEXT("NUMMARKERS"),  TEXT("Number of device markers:"),
    NUMFONTS,    TEXT("NUMFONTS"),    TEXT("Number of device fonts:"),
    NUMCOLORS,   TEXT("NUMCOLORS"),   TEXT("Number of device colors:"),
    PDEVICESIZE, TEXT("PDEVICESIZE"), TEXT("Size of device structure:"),
    ASPECTX,     TEXT("ASPECTX"),     TEXT("Relative width of pixel:"),
    ASPECTY,     TEXT("ASPECTY"),     TEXT("Relative height of pixel:"),
    ASPECTXY,    TEXT("ASPECTXY"),    TEXT("Relative diagonal of pixel:"),
    LOGPIXELSX,  TEXT("LOGPIXELSX"),  TEXT("Horizontal dots per inch:"),
    LOGPIXELSY,  TEXT("LOGPIXELSY"),  TEXT("Vertical dots per inch:"),
    SIZEPALETTE, TEXT("SIZEPALETTE"), TEXT("Number of palette entries:"),
    NUMRESERVED, TEXT("NUMRESERVED"), TEXT("Reserved palette entries:"),
    COLORRES,    TEXT("COLORRES"),    TEXT("Actual color resolution:")};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HFONT CreateCustomFont(int fontSize);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    static TCHAR szAppName[] = TEXT("DevCaps1");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass = {0};

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName,
                   MB_ICONERROR);
        return 0;
    }

    hwnd =
        CreateWindow(szAppName, TEXT("Device Capabilities"),
                     WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                     CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    static int cxChar, cxCaps, cyChar;
    TCHAR szBuffer[10] = {10};
    HDC hdc;
    int i = 0;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    HFONT hFont;  // 用于字体
    const int LEFT = 20;
    const int RIGHT = 40;

    switch (message) {
        case WM_CREATE:
            hdc = GetDC(hwnd);
            // 创建一个字体大小为16的字体
            hFont = CreateCustomFont(22);  // 调用函数设置字体大小为16
            // 选择并应用字体
            SelectObject(hdc, hFont);
            // 获取文本度量信息
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hwnd, hdc);
            // 保存字体句柄
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)hFont);

            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            // 获取自定义字体句柄
            hFont = (HFONT)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            // 选择字体
            SelectObject(hdc, hFont);

            for (i = 0; i < NUMLINES; i++) {
                TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel,
                        lstrlen(devcaps[i].szLabel));

                TextOut(hdc, LEFT * cxCaps, cyChar * i, devcaps[i].szDesc,
                        lstrlen(devcaps[i].szDesc));

                SetTextAlign(hdc, TA_RIGHT | TA_TOP);

                TextOut(hdc, LEFT * cxCaps + RIGHT * cxChar, cyChar * i,
                        szBuffer,
                        wsprintf(szBuffer, TEXT("%5d"),
                                 GetDeviceCaps(hdc, devcaps[i].iIndex)));

                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            TextOut(hdc, 0, cyChar * i, L"中文", lstrlen(L"中文"));
            EndPaint(hwnd, &ps);
            return 0;

        case WM_DESTROY:
            // 删除字体
            hFont = (HFONT)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            DeleteObject(hFont);  // 释放字体资源
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// 设置字体大小的函数
HFONT CreateCustomFont(int fontSize) {
    LOGFONT lf = {0};  // 设置字体信息

    // 设置字体大小
    lf.lfHeight = fontSize;   // 根据传入的参数设置字体大小
    lf.lfWeight = FW_NORMAL;  // 设置字体粗细
    lstrcpy(lf.lfFaceName, TEXT("simsun"));  // Arial使用系统默认字体Arial

    // 创建字体并返回句柄
    return CreateFontIndirect(&lf);
}