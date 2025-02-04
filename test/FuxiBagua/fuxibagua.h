#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <wincrypt.h>
#include <sstream>
//#include <string>

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
using namespace std;

// 全局变量:
#define BUTTON_RESET 1002
#define BUTTON_ANIMATE 1003
#define WM_ANIMATION_DONE (WM_USER + 1)  // 自定义消息
#define WM_MAKEGUAXIANG (WM_USER + 2)    // 6次以后制作卦象

// 窗口代码 主窗口 按键窗口 文字窗口 动画窗口
HWND hMainWnd, hLeftTop, hLeftBottom, hRight;
HWND btHWNDAnime, btHWNDReset;             // 按钮1,2代码
std::wstring imageYin = L"./img/yin.jpg";  // 替换为你的图片路径
std::wstring imageYang = L"./img/yang.jpg";
std::wstring imageanime = L"";
// 图片文件名
wstring imgFileName[] = {
    L"./img/s0.png", L"./img/s1.png", L"./img/s2.png", L"./img/s3.png",
    L"./img/s4.png", L"./img/s5.png", L"./img/s6.png", L"./img/s7.png",
};
RECT clientRect, animeRect, wordsRect;
// 动画状态变量
bool showGuaXiang = false;
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;
wstring yao[] = {
    L"老阴",
    L"少阳",
    L"少阴",
    L"老阳",
};
wstring sixYao[6];
int sixYaoIdx = 5;
struct xy {
    int x;
    int y;
};
xy animaCenter;  // 动画显示区域中心xy
xy yaoCenter;    // 文字显示区域中心
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
float wordsX, wordsY;
BOOL bt1Vis = TRUE;  // 按钮1是否可以点击 按下6次后不可再点击
int btCnt = 0;       // 按钮按下次数
int guaXiang = 0;    // 制作的卦象


VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID,
                        DWORD dwTime);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createSubWindow(HWND hwnd);
void resetVar();
void modifyInt(int& num, int bit);
int generateRandomBit();

void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect);
void clearWindow(HWND hWnd);
void makeGua(HWND hwnd);

void setRectCenter(HWND hwnd, xy& animaCenter);
void drawYao(int i, const std::wstring& imagePath);
void DisplayImage(HWND hwnd, const std::wstring& imagePath, guaPosSize ps);
int reverseBits(int n);
void showGuaImage(HWND hwnd);


