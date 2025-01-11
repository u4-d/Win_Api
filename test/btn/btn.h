// #define UNICODE
#include <Windows.h>
#include <gdiplus.h>
#include <wincrypt.h>

#include <string>
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
// 全局变量:
#define BUTTON1 1001
#define BUTTON2 1002
#define BUTTON_ANIMATE 1003
#define WM_ANIMATION_DONE (WM_USER + 1)  // 自定义消息
#define WM_MAKEGUAXIANG (WM_USER + 2)    // 6次以后制作卦象
BOOL bt1Vis = TRUE;  // 按钮1是否可以点击 按下6次后不可再点击
int btCnt = 0;       // 按钮按下次数
int guaXiang = 0;    // 制作的卦象
int shangGua = 0;
int xiaGua = 0;  // 上挂和下挂分别有guaxiang的高3位和低3位
wchar_t* shangWords = nullptr;
wchar_t* shangSyb = nullptr;  // 上挂
wchar_t* xiaWords = nullptr;
wchar_t* xiaSyb = nullptr;  // 下挂
std::wstring guaString = L"";
HWND bt1Hwnd, bt2Hwnd;  // 按钮1,2代码
constexpr int MAX_LOADSTRING = 100;
HINSTANCE hInst;                           // 当前实例
WCHAR szTitle[MAX_LOADSTRING];             // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];       // 主窗口类名
std::wstring imageYin = L"./img/yin.jpg";  // 替换为你的图片路径
std::wstring imageYang = L"./img/yang.jpg";
std::wstring imageanime = L"";
// 动画状态变量
bool isAnimating = false;
int frame = 0;
const int maxFrames = 100;
const wchar_t* BAGUAWORDS =
    L"坎    巽    乾    兑    离    震    坤    艮    ";  // 汉字卦象
const wchar_t* BAGUASYMBOLS =
    L"☵    ☴    ☰    ☱    ☲    ☳    ☷    ☶    ";  // 符号卦象
struct {
    int bits;
    wchar_t* words;  // std::wstring
    wchar_t* syb;    // wchar_t*
} baGua[]{
    {0b000, L"坤", L"☷"}, {0b100, L"艮", L"☶"},

    {0b010, L"坎", L"☵"}, {0b110, L"巽", L"☴"},

    {0b001, L"震", L"☳"}, {0b101, L"离", L"☲"},

    {0b011, L"兑", L"☱"}, {0b111, L"乾", L"☰"},
};

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void sayhello();
void resetBtnVis();
void modifyInt(int& num, int bit);
int generateRandomBit();
void IntToBinaryString(int value, wchar_t* buf, size_t bufSize);
void makeGuaXiang(int x, int& low, int& hight);
void getBaGua(int bits, wchar_t*& words, wchar_t*& syb);
void AnimateImage(HDC hdc, const std::wstring& imagePath, RECT clientRect);
void ClearWindow(HWND hWnd);
void makeGua(HWND hwnd);
void clieckedAnimate(HWND hwnd, WPARAM wParam);
void onTimer(HWND hwnd, RECT clientRect);
void createButton(HWND hwnd, RECT clientRect);
