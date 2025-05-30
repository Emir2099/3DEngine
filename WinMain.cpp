#define UNICODE
#define _UNICODE
#include <Windows.h>

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            PostQuitMessage(22);  // post a quit message to the message queue
            break; 
        case WM_KEYDOWN:
            if ( wParam == 'F' )
            {
                SetWindowText( hWnd, L"Respects" );
            }   
            break;
        case WM_KEYUP:
            if ( wParam == 'F' )
            {
                SetWindowText( hWnd, L"No Respect");
            }
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
    wc.lpfnWndProc = WndProc;  // default window procedure
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

    // message pump
    MSG msg;
    BOOL gResult;
    // process messages as long as we are not quitting and get unfiltered messages
    while((gResult = GetMessage(&msg,nullptr,0,0)) > 0)
    {
        TranslateMessage(&msg);  // translate virtual key messages
        DispatchMessage(&msg);  // dispatch the message to the window procedure
    }

    if(gResult == -1)
    {
        return -1;  // error occurred
    }
    else
    {
        return msg.wParam;  
    }
}
