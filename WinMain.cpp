#define UNICODE
#define _UNICODE
#include <Windows.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,  // handle to the current instance of the application
    HINSTANCE hPrevInstance,  // always NULL for Win32 applications
    LPSTR lpCmdLine,  // commmand line (helps to receive command line arguments)
    int nShowCmd
) {
    const auto pClassName = L"hw3butts";
    // register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;  // default window procedure
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;  // no icon
    wc.hCursor = nullptr;  // no cursor
    wc.hbrBackground = nullptr;  // no background
    wc.lpszMenuName = nullptr;  // no menu
    wc.lpszClassName = pClassName;  // class name
    wc.hIconSm = nullptr;  // no small icon
    RegisterClassEx( &wc );  // register the window class
    // create a window
    HWND hWnd = CreateWindowEx(
        0,  // no extended styles
        pClassName,  // class name
        L"Happy Window",  // window title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200,200,640,480,
        nullptr,  // no parent window
        nullptr,  // no menu
        hInstance,  // instance handle
        nullptr  // no additional data
    );
    //show the window
    ShowWindow(hWnd, SW_SHOW);
    while(true);
    return 0;
}
