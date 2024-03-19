#include "App.h"
ProcQueryFullProcessImageNameA QueryFullProcessImageNameAFunc;
BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam)
{
    char strBuffer[4096] = {0};

    // if (strcmp((const char *)classN, TOOLTIPS_CLASSA) == 0)
    // {
    //     return TRUE;
    // }
    // if (!(IsWindow(hWnd)))
    // {
    //     return TRUE;
    // }
    std::vector<WININFO> *bag = (std::vector<WININFO> *)lParam;

    WININFO info;
    memset((void *)&info, 0, sizeof(info));
    info.handle = hWnd;
    RECT rect;
    memset((void *)&rect, 0, sizeof(rect));
    GetClientRect(hWnd, &rect);
    WINRECT wrect;
    wrect.width = rect.right - rect.left;
    wrect.height = rect.bottom - rect.top;
    wrect.x = rect.left;
    wrect.y = rect.top;

    DWORD processId = 0;
    GetWindowThreadProcessId(hWnd, &processId);
    info.processId = (int)processId;

    info.rect = wrect;
    info.isDialog = (WC_DIALOG == MAKEINTATOM(GetClassLong(hWnd, GCW_ATOM))) != 0;
    info.minimized = IsIconic(hWnd) != 0;
    LONG style = GetWindowLongA(hWnd, GWL_STYLE);

    info.isTopmost = (style & WS_EX_TOPMOST) != 0;

    DWORD sz = sizeof(strBuffer);
    memset(strBuffer, 0, sizeof(strBuffer));
    GetWindowTextA(hWnd, (char *)strBuffer, sz);
    info.title = strBuffer;

    memset(strBuffer, 0, sizeof(strBuffer));
    GetClassNameA(hWnd, (char *)strBuffer, sizeof(strBuffer));
    info.className = strBuffer;

    if (QueryFullProcessImageNameAFunc)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                          PROCESS_VM_READ,
                                      FALSE, processId);
        if (NULL != hProcess)
        {
            memset(strBuffer, 0, sizeof(strBuffer));
            QueryFullProcessImageNameAFunc(hProcess, 0, (char *)strBuffer, &sz);
            info.path = strBuffer;
        }
    }

    bag->push_back(info);

    return TRUE;
}

std::vector<WININFO> GetWindowsInfo()
{
    std::vector<WININFO> result;
    if (QueryFullProcessImageNameAFunc == NULL)
    {
        QueryFullProcessImageNameAFunc = (ProcQueryFullProcessImageNameA)GetProcAddress(GetModuleHandle("Kernel32.dll"), "QueryFullProcessImageNameA");
    }

    HWND window = GetTopWindow(GetDesktopWindow());
    do
    {
        if (!IsWindow(window))
        {
            continue;
        }
        enumWindowsProc(window, (LPARAM)&result);
    } while (window = GetWindow(window, GW_HWNDNEXT));

    return result;
}