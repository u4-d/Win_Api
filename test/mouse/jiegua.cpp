#include "gua.h"


gua64* findGuaByXiang(int guaXiang){
    // 遍历jieGua数组，查找匹配的guaXiang
    for (int i = 0; i < guaLenth; ++i) {
        if (jieGua[i].guaXiang == guaXiang) {
            return &jieGua[i];  // 返回指向匹配结构体的指针
        }
    }
    return nullptr;  // 如果没有找到，返回nullptr
}

int main() {
    // 设置控制台输出为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
    // 设置 wcout 为宽字符模式
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    int guaXiangInput = 0b000010;  // 输入你想查找的guaXiang
    gua64* result = findGuaByXiang(guaXiangInput);
    
    if (result != nullptr) {
        wcout << L"卦名: " << result->guaName << endl;
        wcout << L"卦象: " << result->guaXiang << endl;
        wcout << L"卦辞: " << result->guaDetail << endl;
        wcout << L"爻辞: " << endl;
        for (int i = 0; i < 6; ++i) {
            wcout << result->yaoDetail[i] << endl;
        }
    } else {
        wcout << L"未找到对应的卦象！" << endl;
    }
    
    return 0;
}