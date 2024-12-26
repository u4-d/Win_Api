/*--------------------------------------------------------------
   HelloMsg.c -- Displays "Hello, Windows 98!" in a message box
                 (c) Charles Petzold, 1998
  --------------------------------------------------------------*/

#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,    _In_opt_ HINSTANCE hPrevInstance,    _In_ LPWSTR    lpCmdLine,    _In_ int       nCmdShow)
{
    MessageBox(NULL, TEXT("Hello, ���� Windows 98!"), TEXT("HelloMsg"), 0);

    return 0;
}
