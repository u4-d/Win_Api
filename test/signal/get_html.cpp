/*
抓取网页
*/
#include <windows.h>
#include <wininet.h>
#include <iostream>

#pragma comment(lib, "wininet.lib")

int main() {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char buffer[4096];
    DWORD bufferLength = sizeof(buffer);

    // 打开Internet连接
    hInternet = InternetOpenW(L"WebDataFetcher", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        std::cerr << "InternetOpen failed!" << std::endl;
        return 1;
    }

    // 连接到Web服务器
    hConnect = InternetOpenUrlW(hInternet, L"http://example.com", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        std::cerr << "InternetOpenUrl failed!" << std::endl;
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 读取网页内容
    while (InternetReadFile(hConnect, buffer, bufferLength, &bytesRead) && bytesRead > 0) {
        std::cout.write(buffer, bytesRead);
    }

    // 清理资源
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}
