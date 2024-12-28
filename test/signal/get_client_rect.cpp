/*
获取窗口 矩形大小
*/
#include <windows.h>
#include <iostream>

int main() {
    // 创建一个简单的窗口
    HWND hWnd = GetConsoleWindow(); // 获取控制台窗口的句柄

    RECT rect;
    // 获取客户区域的大小
    if (GetClientRect(hWnd, &rect)) {
        std::cout << "Client area: "
                  << "Left = " << rect.left << ", "
                  << "Top = " << rect.top << ", "
                  << "Right = " << rect.right << ", "
                  << "Bottom = " << rect.bottom << std::endl;
    } else {
        std::cerr << "Failed to get client area." << std::endl;
    }

    return 0;
}
