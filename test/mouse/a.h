#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <bitset>

using namespace Gdiplus;
using namespace std;

#pragma comment(lib, "gdiplus.lib")

// 初始化 GDI+
ULONG_PTR gdiplusToken;

HINSTANCE hInst;   // 当前实例句柄
HWND hTooltipWnd;  // 悬浮窗口句柄
//
// 图像大小
int imgWidth = 200;
int imgHeight = 50;
// 加载图像
PTCHAR yang = L"./img/yang.jpg";
PTCHAR yin = L"./img/yin.jpg";  // wchar_t*
//
// 解卦 结构体
struct gua64 {
    wstring guaName;
    int guaXiang;
    wstring guaDetail;
    wstring yaoDetail[6];
};
gua64* guaDetailStrings;
// 悬浮窗口的文字内容
std::wstring tooltipText[6] = {L"潜龙，勿用。",
                               L"见龙在田，利见大人。",
                               L"君子终日乾乾，夕惕若，厉，无咎。",
                               L"或跃在渊，无咎。",
                               L"飞龙在天，利见大人。",
                               L"亢龙有悔。"};
wstring yao[] = {
    L"老阴",
    L"少阳",
    L"少阴",
    L"老阳",
};
// 显示爻的坐标
POINT yaoXY;
// 悬浮窗矩形
RECT yaoWinRect[6];
// 显示图片 GDI 矩形
Rect yaoGdiRect[6];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TooltipWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                LPARAM lParam);  // 悬浮窗口的窗口过程
void CreateTooltipWindow(HINSTANCE hInstance);

void InitializeGDIPlus();
void ShutdownGDIPlus();
void DrawBinaryImages(Graphics* graphics, int binaryInput);
void setYaoXY(HWND hwnd);
Gdiplus::Rect ConvertRECTToRect(const RECT& winRect);
void setYaoRect(HWND hwnd);
void mouseMove(HWND hwnd);
gua64* findGuaByXiang(int guaXiang);
