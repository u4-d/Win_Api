// #define UNICODE
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <wincrypt.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Advapi32.lib")

using namespace Gdiplus;
using namespace std;
#define BUTTON1 1001
#define BUTTON2 1002
#define BUTTON_ANIMATE 1003
#define WM_ANIMATION_DONE (WM_USER + 1)  // 自定义消息
#define WM_MAKEGUAXIANG (WM_USER + 2)    // 6次以后制作卦象
HINSTANCE hInst;                         // 当前实例
HWND bt1Hwnd, bt2Hwnd;                   // 按钮1,2代码
BOOL bt1Vis = TRUE;  // 按钮1是否可以点击 按下6次后不可再点击
int btCnt = 0;       // 按钮按下次数
int guaXiang = 0;    // 制作的卦象
wstring yao[] = {
    L"老阴",
    L"少阳",
    L"少阴",
    L"老阳",
};
int sixYaoIdx = 5;
wstring sixYao[6];
std::wstring imageYin = L"./img/yin.jpg";  // 替换为你的图片路径
std::wstring imageYang = L"./img/yang.jpg";
std::wstring imageanime = L"";
// 图片文件名
wstring imgFileName[] = {
    L"./img/s0.png", L"./img/s1.png", L"./img/s2.png", L"./img/s3.png",
    L"./img/s4.png", L"./img/s5.png", L"./img/s6.png", L"./img/s7.png",
};
// 设置显示的固定位置和大小

struct imgPos {
    int x;  // 图片左上角 X 坐标
    int y;  // 图片左上角 Y 坐标
} iPos;
struct imgSize {
    int width;          // 图片的宽度
    int height;         // 图片的高度
} imgGuaSize{100, 70};  // 卦象图片大小
struct guaPosSize {
    struct imgPos p;
    struct imgSize s;
};
guaPosSize shangImgPos;
guaPosSize xiaImgPos;
// 动画状态变量
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;

// 主窗口
RECT clientRect = {500, 200, 1500, 900};
RECT wordsRect;  // 显示爻的区域
RECT animaRect;  // 动画区域
HWND mainHwnd;
// Register the window class
const wchar_t CLASS_NAME[] = L"主窗口";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AnimateImage(HDC hdc, const std::wstring& imagePath);
void ClearWindow(HWND hwnd);
void createMainWindow(HINSTANCE hInstance, int nCmdShow);
void DisplayImage(HWND hwnd, const std::wstring& imagePath, guaPosSize ps);
void GetWindowCenter(HWND hwnd, int& centerX, int& centerY);
void showGuaImage(HWND hwnd);
void createButton(HWND hwnd, RECT clientRect);
int generateRandomBit();
void modifyInt(int& num, int bit);
void makeGua(HWND hwnd);
void clieckedAnimate(HWND hwnd, WPARAM wParam);
void onTimer(HWND hwnd);
void resetBtnVis();
void DrawTextLines();
void setRect();
int reverseBits(int n);
