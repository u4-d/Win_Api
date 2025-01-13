#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <wincrypt.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Advapi32.lib")

using namespace Gdiplus;
using namespace std;

#define BUTTON1 1001
#define BUTTON_RESET 1002
#define BUTTON_ANIMATE 1003
#define WM_ANIMATION_DONE (WM_USER + 1)  // 自定义消息
#define WM_MAKEGUAXIANG (WM_USER + 2)    // 6次以后制作卦象

// ULONG_PTR gdiplusToken;
const wchar_t CLASS_NAME[] = L"Main Window Class";
const wchar_t CHILD_CLASS_NAME[] = L"Child Window Class";
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
// 动画状态变量
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;
// 文本输出的起始位置
float textX = 50.0f;   // X 坐标
float textY = 270.0f;  // Y 坐标

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

// 窗口代码 主窗口 按键窗口 文字窗口 动画窗口
HWND hMainWnd, hLeftTop, hLeftBottom, hRight;
HWND bt1Hwnd, bt2Hwnd;  // 按钮1,2代码

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void clearWindow(HWND hwnd);
int generateRandomBit();
void AdjustChildWindows(HWND hwnd);

// 按键窗口函数
void createButton(HWND hwnd);
void resetBtnVis();
void makeGua(HWND hwnd);
void modifyInt(int& num, int bit);

// 文字窗口函数
int reverseBits(int n);
void showGuaImage(HWND hwnd);
void DrawTextLines();
void drawYao(int i, const std::wstring& imagePath);

// 动画窗口函数
void onCliecked(HWND hwnd, WPARAM wParam);
void AnimateImage(HDC hdc, const std::wstring& imagePath);
void onTimer(HWND hwnd);
void DisplayImage(HWND hwnd, const std::wstring& imagePath, guaPosSize ps);
