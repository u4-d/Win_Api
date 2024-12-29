/*----------------------------------------------------
   SYSMETS1.C -- System Metrics Display Program No. 1
                 (c) Charles Petzold, 1998
  ----------------------------------------------------*/
constexpr int WINVER = 0x0500;
// #define WINVER 0x0500
#include <windows.h>

#include "sysmets.h"

// 全局变量:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  // 避免提示未使用变量
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // 使用 wcscpy_s 来复制字符串
  wcscpy_s(szWindowClass, MAX_LOADSTRING, L"SysMets2");
  wcscpy_s(szTitle, MAX_LOADSTRING, L"SysMets2");

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
  HDC hdc;
  int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
  PAINTSTRUCT ps;
  SCROLLINFO si;
  TCHAR szBuffer[10] = {0};
  TEXTMETRIC tm;

  switch (message) {
    case WM_CREATE:
      hdc = GetDC(hwnd);

      GetTextMetrics(hdc, &tm);
      //字符的平均宽度。
      cxChar = tm.tmAveCharWidth;
      //tm.tmPitchAndFamily 的最低位是 1 则 cxCaps 的值为 (3 * cxChar) / 2
      //tm.tmPitchAndFamily 的最低位是 0 则 cxCaps 的值为 (2 * cxChar) / 2
      cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
      //tmHeight：字体的总高度。包括字符的上升部分和下降部分。
      //tmExternalLeading：字体外部行距，即文本行之间的间距。
      cyChar = tm.tmHeight + tm.tmExternalLeading;

      ReleaseDC(hwnd, hdc);

      // Save the width of the three columns

      iMaxWidth = 40 * cxChar + 22 * cxCaps;
      return 0;

    case WM_SIZE:
      cxClient = LOWORD(lParam);
      cyClient = HIWORD(lParam);

      // Set vertical scroll bar range and page size

      si.cbSize = sizeof(si);
      si.fMask = SIF_RANGE | SIF_PAGE;
      si.nMin = 0;
      si.nMax = NUMLINES - 1;
      si.nPage = cyClient / cyChar;
      SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

      // Set horizontal scroll bar range and page size

      si.cbSize = sizeof(si);
      si.fMask = SIF_RANGE | SIF_PAGE;
      si.nMin = 0;
      si.nMax = 2 + iMaxWidth / cxChar;
      si.nPage = cxClient / cxChar;
      SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
      return 0;

    case WM_VSCROLL:
      // Get all the vertial scroll bar information

      si.cbSize = sizeof(si);
      si.fMask = SIF_ALL;
      GetScrollInfo(hwnd, SB_VERT, &si);

      // Save the position for comparison later on

      iVertPos = si.nPos;

      switch (LOWORD(wParam)) {
        case SB_TOP:
          si.nPos = si.nMin;
          break;

        case SB_BOTTOM:
          si.nPos = si.nMax;
          break;

        case SB_LINEUP:
          si.nPos -= 1;
          break;

        case SB_LINEDOWN:
          si.nPos += 1;
          break;

        case SB_PAGEUP:
          si.nPos -= si.nPage;
          break;

        case SB_PAGEDOWN:
          si.nPos += si.nPage;
          break;

        case SB_THUMBTRACK:
          si.nPos = si.nTrackPos;
          break;

        default:
          break;
      }
      // Set the position and then retrieve it.  Due to adjustments
      //   by Windows it may not be the same as the value set.

      si.fMask = SIF_POS;
      SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
      GetScrollInfo(hwnd, SB_VERT, &si);

      // If the position has changed, scroll the window and update it

      if (si.nPos != iVertPos) {
        ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
        UpdateWindow(hwnd);
      }
      return 0;

    case WM_HSCROLL:
      // Get all the vertial scroll bar information

      si.cbSize = sizeof(si);
      si.fMask = SIF_ALL;

      // Save the position for comparison later on

      GetScrollInfo(hwnd, SB_HORZ, &si);
      iHorzPos = si.nPos;

      switch (LOWORD(wParam)) {
        case SB_LINELEFT:
          si.nPos -= 1;
          break;

        case SB_LINERIGHT:
          si.nPos += 1;
          break;

        case SB_PAGELEFT:
          si.nPos -= si.nPage;
          break;

        case SB_PAGERIGHT:
          si.nPos += si.nPage;
          break;

        case SB_THUMBPOSITION:
          si.nPos = si.nTrackPos;
          break;

        default:
          break;
      }
      // Set the position and then retrieve it.  Due to adjustments
      //   by Windows it may not be the same as the value set.

      si.fMask = SIF_POS;
      SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
      GetScrollInfo(hwnd, SB_HORZ, &si);

      // If the position has changed, scroll the window

      if (si.nPos != iHorzPos) {
        ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
      }
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);

      // Get vertical scroll bar position

      si.cbSize = sizeof(si);
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_VERT, &si);
      iVertPos = si.nPos;

      // Get horizontal scroll bar position

      GetScrollInfo(hwnd, SB_HORZ, &si);
      iHorzPos = si.nPos;

      // Find painting limits

      iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
      iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

      for (i = iPaintBeg; i <= iPaintEnd; i++) {
        x = cxChar * (1 - iHorzPos);
        y = cyChar * (i - iVertPos);

        TextOut(hdc, x, y, sysmetrics[i].szLabel,
                lstrlen(sysmetrics[i].szLabel));

        TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc,
                lstrlen(sysmetrics[i].szDesc));

        SetTextAlign(hdc, TA_RIGHT | TA_TOP);

        TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer,
                wsprintf(szBuffer, TEXT("%5d"),
                         GetSystemMetrics(sysmetrics[i].iIndex)));

        SetTextAlign(hdc, TA_LEFT | TA_TOP);
      }

      EndPaint(hwnd, &ps);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
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