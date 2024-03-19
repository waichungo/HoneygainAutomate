#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

typedef BOOL(__stdcall *ProcQueryFullProcessImageNameA)(HANDLE hProcess, DWORD dwFlags, LPSTR lpExeName, PDWORD lpdwSize);

typedef struct WINRECT
{
    int x;
    int y;
    int width;
    int height;

} WINRECT;
typedef struct WININFO
{
    bool minimized;
    bool isTopmost;
    bool isDialog;
    int processId;
    std::string path;
    std::string title;
    std::string className;
    HWND handle;
    WINRECT rect;

} WININFO;
std::vector<WININFO> GetWindowsInfo();