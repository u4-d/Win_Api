/*----------------------------------------------------
   SYSMETS2.C -- System Metrics Display Program No. 2
                 (c) Charles Petzold, 1998
  ----------------------------------------------------*/

constexpr int WINVER = 0x0500;
// #define WINVER 0x0500
#include <windows.h>
#include "sysmets.h"

// ȫ�ֱ���:
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                     // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];       // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING]; // ����������

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    // ������ʾδʹ�ñ���
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ʹ�� wcscpy_s �������ַ���
    wcscpy_s(szWindowClass, MAX_LOADSTRING, L"SysMets2");
    wcscpy_s(szTitle, MAX_LOADSTRING, L"SysMets2");

    MyRegisterClass(hInstance);
    // ִ��Ӧ�ó����ʼ��:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    MSG msg;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return (int)msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static int  cxChar, cxCaps, cyChar, cyClient, iVscrollPos ;
     HDC         hdc ;
     int         i, y ;
     PAINTSTRUCT ps ;
     TCHAR       szBuffer[10] ;
     TEXTMETRIC  tm ;

     switch (message)
     {
     case WM_CREATE:
          hdc = GetDC (hwnd) ;

          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
          cyChar = tm.tmHeight + tm.tmExternalLeading ;

          ReleaseDC (hwnd, hdc) ;

          SetScrollRange (hwnd, SB_VERT, 0, NUMLINES - 1, FALSE) ;
          SetScrollPos   (hwnd, SB_VERT, iVscrollPos, TRUE) ;
          return 0 ;

     case WM_SIZE:
          cyClient = HIWORD (lParam) ;
          return 0 ;

     case WM_VSCROLL:
          switch (LOWORD (wParam))
          {
          case SB_LINEUP:
               iVscrollPos -= 1 ;
               break ;
     
          case SB_LINEDOWN:
               iVscrollPos += 1 ;
               break ;
     
          case SB_PAGEUP:
               iVscrollPos -= cyClient / cyChar ;
               break ;
     
          case SB_PAGEDOWN:
               iVscrollPos += cyClient / cyChar ;
               break ;
     
          case SB_THUMBPOSITION:
               iVscrollPos = HIWORD (wParam) ;
               break ;
     
          default :
               break ;
          }

          iVscrollPos = max (0, min (iVscrollPos, NUMLINES - 1)) ;

          if (iVscrollPos != GetScrollPos (hwnd, SB_VERT))
          {
               SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE) ;
               InvalidateRect (hwnd, NULL, TRUE) ;
          }
          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
     
          for (i = 0 ; i < NUMLINES ; i++)
          {
               y = cyChar * (i - iVscrollPos) ;
     
               TextOut (hdc, 0, y,
                        sysmetrics[i].szLabel,
                        lstrlen (sysmetrics[i].szLabel)) ;
     
               TextOut (hdc, 22 * cxCaps, y,
                        sysmetrics[i].szDesc,
                        lstrlen (sysmetrics[i].szDesc)) ;
     
               SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;
     
               TextOut (hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer,
                        wsprintf (szBuffer, TEXT ("%5d"),
                             GetSystemMetrics (sysmetrics[i].iIndex))) ;
     
               SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
          }
          EndPaint (hwnd, &ps) ;
          return 0 ;

     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};            // ��ʼ��Ϊ�㣬������г�Ա
    wcex.cbSize = sizeof(WNDCLASSEXW); // ���ýṹ��Ĵ�С

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    // lpszClassName δ���û�ע��ʧ��
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);   // ���ش�ͼ�� IDI_APPLICATION
    wcex.hIconSm = LoadIcon(hInstance, IDI_INFORMATION); // ����Сͼ�� IDI_INFORMATION

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
                              nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}