// #define UNICODE
#include <windows.h>
#include <gdiplus.h>
#include <string>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

std::wstring imageYin = L"./img/yin.jpg";  // 替换为你的图片路径
std::wstring imageYang = L"./img/yang.jpg";
std::wstring imageanime = L"";

// 动画状态变量
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;

// 主窗口
RECT clientRect;
HWND mainHwnd;
// Register the window class
const wchar_t CLASS_NAME[] = L"主窗口";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AnimateImage(HDC hdc, const std::wstring &imagePath, RECT clientRect);
void ClearWindow(HWND hwnd);
void createMainWindow(HINSTANCE hInstance, int nCmdShow);
