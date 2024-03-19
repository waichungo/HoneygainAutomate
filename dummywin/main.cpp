#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

#include <Windows.h>

// Step 1: Define a Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Step 2: Register the Window Class
    const char CLASS_NAME[] = "DummyWinApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;    // Pointer to window procedure function
    wc.hInstance = hInstance;       // Handle to the application instance
    wc.lpszClassName = CLASS_NAME;  // Name of the window class
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor for the window

    RegisterClass(&wc);

    // Step 3: Create the Window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles
        CLASS_NAME,                 // Window class
        "DApp",          // Window title
        WS_OVERLAPPEDWINDOW,       // Window style

        // Position and size
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Application instance
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Step 4: Show and update the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 5: Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // Translate keystroke messages into the right format
        DispatchMessage(&msg);  // Dispatch message to the WindowProc
    }

    return 0;
}
