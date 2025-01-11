#pragma once
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <wincrypt.h>
#include <bitset>
#include <cwchar>
#include <iostream>

#pragma comment(lib, "Advapi32.lib")

int guaXiang = 0;  // 制作的卦象
int shangGua = 0;
int xiaGua = 0;  // 上挂和下挂分别有guaxiang的高3位和低3位
const wchar_t* shangWords;
const wchar_t* shangSyb;  // 上挂
// std::wstring shangWords;
// std::wstring shangSyb;  // 上挂
const wchar_t* xiaWords;
const wchar_t* xiaSyb;  // 下挂
wchar_t* guaString;
const wchar_t* BAGUAWORDS =
    L"坎    巽    乾    兑    离    震    坤    艮    ";  // 汉字卦象
const wchar_t* BAGUASYMBOLS =
    L"☵    ☴    ☰    ☱    ☲    ☳    ☷    ☶    ";  // 符号卦象
struct Gua {
    int bits;
    const wchar_t* words;  // std::wstring
    const wchar_t* syb;    // wchar_t*
    std::wstring path;
};
Gua baGua[]{
    {0b000, L"坤", L"☷", L"./img/0.png"}, {0b001, L"震", L"☳", L"./img/1.png"},
    {0b010, L"坎", L"☵", L"./img/2.png"}, {0b011, L"兑", L"☱", L"./img/3.png"},
    {0b100, L"艮", L"☶", L"./img/4.png"}, {0b101, L"离", L"☲", L"./img/5.png"},
    {0b110, L"巽", L"☴", L"./img/6.png"}, {0b111, L"乾", L"☰", L"./img/7.png"},
};
struct {
    int idx;
    std::wstring path;
} imgPath[]{
    {0, L"./img/0.png"}, {1, L"./img/1.png"}, {2, L"./img/2.png"},
    {3, L"./img/3.png"}, {4, L"./img/4.png"}, {5, L"./img/5.png"},
    {6, L"./img/6.png"}, {7, L"./img/7.png"},
};
int generateRandomBit();
void makeBits();
void makeFuxiBagua();
void modifyInt(int& num, int bit);
void makeGuaXiang(int x, int& low, int& hight);
void makeGuaXiang_old(int x, int& low, int& hight);
void getBaGua_C(int bits, const wchar_t*& words, const wchar_t*& syb);
void getBaGua_S(int bits, std::wstring& words, std::wstring& syb);
void test1();