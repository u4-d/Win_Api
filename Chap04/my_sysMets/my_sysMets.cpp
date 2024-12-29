// 改写sysmets3
//
//  把WndProc中语句块过多的地方写出函数
//
//  case WM_CREATE:
//  case WM_SIZE
//  case WM_VSCROLL: wparam
//  case WM_HSCROLL wparam
//  case WM_PAINT:
//
//  my_sysMets.cpp : 定义应用程序的入口点。
//

#include "my_sysMets.h"
#include "framework.h"

//#define MAX_LOADSTRING 100
constexpr int MAX_LOADSTRING = 100;
// 全局变量:
HINSTANCE hInst;                     // 当前实例
WCHAR szTitle[MAX_LOADSTRING];       // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL my_WM_Size(LPARAM, SCROLLINFO);
VOID my_wm_create(_In_ HWND hWnd, _Out_ TEXTMETRIC &tm, _Out_ my_create &crt);
VOID my_wm_size(_In_ LPARAM lParam, _In_ HWND hWnd, _In_ my_create crt,
                _In_ SCROLLINFO si);
VOID my_wm_vscroll(_In_ WPARAM wParam, _In_ HWND hWnd, _In_ my_create crt);
VOID my_wm_hscroll(_In_ WPARAM wParam, _In_ HWND hWnd, _In_ my_create crt);
VOID my_wm_paint(_In_ HWND hWnd, _In_ my_create crt);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: 在此处放置代码。

  // 初始化全局字符串
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_MYSYSMETS, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // 执行应用程序初始化:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYSYSMETS));

  MSG msg;

  // 主消息循环:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex = {0};

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYSYSMETS));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_MYSYSMETS)
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
  hInst = hInstance; // 将实例句柄存储在全局变量中

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
  static my_create crt;
  TEXTMETRIC tm;
  // HDC hdc;
  SCROLLINFO si;
  switch (message) {
  case WM_CREATE:
    my_wm_create(hWnd, tm, crt);
    break;
  case WM_SIZE:
    ZeroMemory(&si, sizeof(si)); // 初始化结构体
    my_wm_size(lParam, hWnd, crt, si);
    break;
  case WM_VSCROLL:
    my_wm_vscroll(wParam, hWnd, crt);
    break;
  case WM_HSCROLL:
    my_wm_hscroll(wParam, hWnd, crt);
    break;
  case WM_PAINT:
    my_wm_paint(hWnd, crt);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

VOID my_wm_create(_In_ HWND hWnd, _Out_ TEXTMETRIC &tm, _Out_ my_create &crt) {
  HDC hdc;
  hdc = GetDC(hWnd);

  GetTextMetrics(hdc, &tm);
  // 字符的平均宽度。
  crt.cxChar = tm.tmAveCharWidth;
  // tm.tmPitchAndFamily 的最低位是 1 则 cxCaps 的值为 (3 * cxChar) / 2
  // tm.tmPitchAndFamily 的最低位是 0 则 cxCaps 的值为 (2 * cxChar) / 2
  crt.cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * crt.cxChar / 2;
  // tmHeight：字体的总高度。包括字符的上升部分和下降部分。
  // tmExternalLeading：字体外部行距，即文本行之间的间距。
  crt.cyChar = tm.tmHeight + tm.tmExternalLeading;

  ReleaseDC(hWnd, hdc);

  // Save the width of the three columns

  crt.iMaxWidth = 40 * crt.cxChar + 22 * crt.cxCaps;
}

VOID my_wm_size(_In_ LPARAM lParam, _In_ HWND hWnd, _In_ my_create crt,
                _In_ SCROLLINFO si) {
  int cxClient = LOWORD(lParam);
  int cyClient = HIWORD(lParam);

  // Set vertical scroll bar range and page size

  si.cbSize = sizeof(si);
  si.fMask = SIF_RANGE | SIF_PAGE;
  si.nMin = 0;
  si.nMax = NUMLINES - 1;
  si.nPage = cyClient / crt.cyChar;
  SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

  // Set horizontal scroll bar range and page size

  si.cbSize = sizeof(si);
  si.fMask = SIF_RANGE | SIF_PAGE;
  si.nMin = 0;
  si.nMax = 2 + crt.iMaxWidth / crt.cxChar;
  si.nPage = cxClient / crt.cxChar;
  SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
}

VOID my_wm_vscroll(_In_ WPARAM wParam, _In_ HWND hWnd, _In_ my_create crt) {
  SCROLLINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;
  GetScrollInfo(hWnd, SB_VERT, &si);

  // Save the position for comparison later on

  int iVertPos = si.nPos;

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
  SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
  GetScrollInfo(hWnd, SB_VERT, &si);

  // If the position has changed, scroll the window and update it

  if (si.nPos != iVertPos) {
    ScrollWindow(hWnd, 0, crt.cyChar * (iVertPos - si.nPos), NULL, NULL);
    UpdateWindow(hWnd);
  }
}

VOID my_wm_hscroll(_In_ WPARAM wParam, _In_ HWND hWnd, _In_ my_create crt) {
  SCROLLINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;

  // Save the position for comparison later on

  GetScrollInfo(hWnd, SB_HORZ, &si);
  int iHorzPos = si.nPos;

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
  SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
  GetScrollInfo(hWnd, SB_HORZ, &si);

  // If the position has changed, scroll the window

  if (si.nPos != iHorzPos) {
    ScrollWindow(hWnd, crt.cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
  }
}

VOID my_wm_paint(_In_ HWND hWnd, _In_ my_create crt) {
  HDC hdc;
  PAINTSTRUCT ps;
  SCROLLINFO si;
  TCHAR szBuffer[10] = {0};
  hdc = BeginPaint(hWnd, &ps);

  // Get vertical scroll bar position
  ZeroMemory(&si, sizeof(si));
  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  GetScrollInfo(hWnd, SB_VERT, &si);
  int iVertPos = si.nPos;

  // Get horizontal scroll bar position

  GetScrollInfo(hWnd, SB_HORZ, &si);
  int iHorzPos = si.nPos;

  // Find painting limits

  int iPaintBeg = max(0, iVertPos + ps.rcPaint.top / crt.cyChar);
  int iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / crt.cyChar);
  int x=0;
  int y=0;
  for (int i = iPaintBeg; i <= iPaintEnd; i++) {
    x = crt.cxChar * (1 - iHorzPos);
    y = crt.cyChar * (i - iVertPos);

    TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));

    TextOut(hdc, x + 22 * crt.cxCaps, y, sysmetrics[i].szDesc,
            lstrlen(sysmetrics[i].szDesc));

    SetTextAlign(hdc, TA_RIGHT | TA_TOP);

    TextOut(hdc, x + 22 * crt.cxCaps + 40 * crt.cxChar, y, szBuffer,
            wsprintf(szBuffer, TEXT("%5d"),
                     GetSystemMetrics(sysmetrics[i].iIndex)));

    SetTextAlign(hdc, TA_LEFT | TA_TOP);
  }
  
  EndPaint(hWnd, &ps);
}
