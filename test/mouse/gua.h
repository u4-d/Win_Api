#include "jiegua.h"

// 全局变量:
#define BUTTON_RESET 1002
#define BUTTON_ANIMATE 1003
#define WM_ANIMATION_DONE (WM_USER + 1)  // 自定义消息
#define WM_MAKEGUAXIANG (WM_USER + 2)    // 6次以后制作卦象
// 窗口代码 主窗口 按键窗口 文字窗口 动画窗口  悬浮窗
HWND hMainWnd, hLeftTop, hLeftBottom, hRight, hTooltipWnd;
// 按钮1,2代码
HWND btHWNDAnime, btHWNDReset;
// GDI+
ULONG_PTR gdiplusToken;

// hLeftTop 窗口中使用的变量
//
//

// hLeftBottom 窗口中使用的变量
// 摇卦显示的汉字
wstring yao[] = {
    L"老阴",
    L"少阳",
    L"少阴",
    L"老阳",
};
// yao 显示区域 GDI Rect结构体
Rect yaoWordsRect[6];
// yin yang 显示区域 GDI Rect结构体
Rect yaoSybRect[6];

// hRight 窗口中使用的变量
//
// 动画显示摇卦使用的变量
//
// 加载图像
PTCHAR yang = L"./img/yang.jpg";
PTCHAR yin = L"./img/yin.jpg";
// yin yang 图像大小
int imgWidth = 200;
int imgHeight = 50;

//

// 显示结果卦象时使用的变量
//
// 显示爻的坐标
POINT yaoXY;
// 悬浮窗矩形 判断鼠标是否在其中
RECT yaoWinRect[6];
// 显示图片 GDI 矩形
Rect yaoGdiRect[6];
// 悬浮窗口的文字内容
//

// 解卦 卦象结构体
// guaName 卦象名称
// guaXiang 卦象的2进制数
// guaDetail 卦象解说
// yaoDetail[6] 爻的解说
gua64* guaDetail;
// guaDetail = &jieGua[0];

// 消息回调函数
// 主窗口
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 悬浮窗口的窗口过程
LRESULT CALLBACK TooltipWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                                LPARAM lParam);

// 通用
void InitializeGDIPlus();
void ShutdownGDIPlus();
Gdiplus::Rect ConvertRECTToRect(const RECT& winRect);

// hLeftTop 窗口中使用的函数
//
//

// hLeftBottom 窗口中使用的函数
//

// hRight 窗口中使用的函数
//
// 创建悬浮窗口
void CreateTooltipWindow(HWND hwnd);

// 画爻
void DrawBinaryImages(Graphics* graphics, int binaryInput);
// 设置爻显示xy
void setYaoXY(HWND hwnd);
// 设置爻的GDI Rect结构
void setYaoRect(HWND hwnd);
// 鼠标移动消息中的过程
void mouseMove(HWND hwnd);
// 返回一个解卦的结构体
gua64* findGuaByXiang(int n);
