#include <windows.h>
#include <gdiplus.h>
#include <cwchar>  // 或者 #include <wchar.h>

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
Rect ConvertRECTToRect(const RECT& winRect) {
    // 使用 RECT 的 left, top, right, bottom 转换为 Gdiplus::Rect
    return Rect(winRect.left, winRect.top, winRect.right - winRect.left,
                winRect.bottom - winRect.top);
}

int main() {
    RECT winRect = {10, 20, 110, 220};  // 示例的 RECT
    Rect gdiPlusRect = ConvertRECTToRect(winRect);

    // 输出转换后的 GDI+ Rect 的信息
    wprintf(L"GDI+ Rect: X = %d, Y = %d, Width = %d, Height = %d\n",
            gdiPlusRect.X, gdiPlusRect.Y, gdiPlusRect.Width,
            gdiPlusRect.Height);
}