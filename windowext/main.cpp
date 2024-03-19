#include "App.h"
#include <iostream>
#define WM_HIDE_TRAYICON (WM_USER + 8)
int main(int argc, char *argv)
{
    auto wins = GetWindowsInfo();
    HWND parent = NULL;
    HWND main = NULL;
    for (auto &win : wins)
    {
        if (win.processId == 13856)
        {
            parent = win.handle;
        }
    }

    for (auto &win : wins)
    {
        if (win.processId == 18368)
        {
            std::cout << win.title << "\n";
            if (win.rect.width > 0)
            {
                // SetWindowPos(win.handle, NULL, -2000, -2000, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
                main = win.handle;
                SendMessageA(win.handle, WM_HIDE_TRAYICON, 0, 0);
            }
            else
            {
                SendMessageA(win.handle, WM_HIDE_TRAYICON, 0, 0);
            }
            // ShowWindow(win.handle, SW_HIDE);
            auto nw = SetParent(win.handle, parent);
        }
    }
    std::cout << wins.size();
    while (true)
    {
        UpdateWindow(main);
        Sleep(30);
    }
}