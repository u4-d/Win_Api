/*-----------------------------------------------------
   SCRNSIZE.C -- Displays screen size in a message box
                 (c) Charles Petzold, 1998
  -----------------------------------------------------*/

#include <windows.h>
#include <tchar.h>     
#include <stdio.h>     

int CDECL MessageBoxPrintf (const TCHAR * szCaption, const TCHAR * szFormat, ...)
{//���� const�ؼ��� const TCHAR * szCaption,��Ȼ�ᱨ��
     TCHAR   szBuffer [1024] ;
     va_list pArgList ;

          // The va_start macro (defined in STDARG.H) is usually equivalent to:
          // pArgList = (char *) &szFormat + sizeof (szFormat) ;

     va_start (pArgList, szFormat) ;

          // The last argument to wvsprintf points to the arguments

     //_vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), szFormat, pArgList) ;
     _vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _TRUNCATE, szFormat, pArgList);


          // The va_end macro just zeroes out pArgList for no good reason

     va_end (pArgList) ;

     return MessageBox (NULL, szBuffer, szCaption, 0) ;
}

int WINAPI wWinMain (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR szCmdLine, _In_ int iCmdShow)
{
     int cxScreen, cyScreen ;

     cxScreen = GetSystemMetrics (SM_CXSCREEN) ;
     cyScreen = GetSystemMetrics (SM_CYSCREEN) ;

     MessageBoxPrintf (
         TEXT ("ScrnSize"),TEXT ("The screen is %i pixels ��wide by %i pixels ��high."),
         cxScreen, cyScreen) ;
     return 0 ;
}