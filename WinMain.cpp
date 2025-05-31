#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <sstream>
#include "ExceptionHandler.h" // Include the new exception handler

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
        case WM_CHAR:
            {
                static std::wstring title;
                title.push_back((wchar_t)wParam);  // append the character to the title
                SetWindowText( hWnd, title.c_str() );  // set the window title to the current string
            }
            break;
        case WM_LBUTTONDOWN:
            {
                const POINTS pt = MAKEPOINTS(lParam);  // get the mouse position
                std::wostringstream oss;  // use wostringstream to format the output beacuse ostringstream is not wide character aware
                oss << L"(" << pt.x << L"," << pt.y << L")";
                SetWindowText(hWnd, oss.str().c_str());  // set the window title to the cursor position
            }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
    HINSTANCE hInstance,  // handle to the current instance of the application
    HINSTANCE hPrevInstance,  // always NULL for Win32 applications
    LPSTR lpCmdLine,  // commmand line (helps to receive command line arguments)
    int nShowCmd
) {
    try { // Start of try block
        const auto pClassName = L"hw3butts";
        const wchar_t* windowTitle = L"Happy Window"; // Defined title
        const int windowWidth = 640;
        const int windowHeight = 480;

        // Validate window title (Example of a custom check)
        if (windowTitle == nullptr || wcslen(windowTitle) == 0) {
            // Log a validation error and then throw to stop execution if it's critical,
            // or just log and continue if it's a non-critical warning.
            LOG_VALIDATION_ERROR(L"Window title cannot be empty.");
            throw std::runtime_error("Window title configuration error."); // Or handle differently
        }

        // Validate window dimensions (Example of a custom check)
        if (windowWidth <= 0 || windowHeight <= 0) {
            LOG_VALIDATION_ERROR(L"Window dimensions must be positive.");
            throw std::runtime_error("Window dimensions configuration error."); // Or handle differently
        }

        // register window class
        WNDCLASSEX wc = {0};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WndProc;  // default window procedure
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        // Load the standard application icon
        wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
        wc.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
        // wc.hIcon = nullptr;  // if want no icon
        wc.hCursor = nullptr;  // no cursor
        wc.hbrBackground = nullptr;  // no background
        wc.lpszMenuName = nullptr;  // no menu
        wc.lpszClassName = pClassName;  // class name
        // wc.hIconSm = nullptr;  // if want no small icon
        if (!RegisterClassEx( &wc )) // register the window class
        {
            throw std::runtime_error("Failed to register window class");
        }
        // create a window
        HWND hWnd = CreateWindowEx(
            0,  // no extended styles
            pClassName,  // class name
            windowTitle,  // Use the validated window title
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            200, 200, windowWidth, windowHeight, // Use validated dimensions
            nullptr,  // no parent window
            nullptr,  // no menu
            hInstance,  // instance handle
            nullptr  // no additional data
        );
        if (!hWnd)
        {
            throw std::runtime_error("Failed to create window");
        }
        //show the window
        ShowWindow(hWnd, SW_SHOW);

        // TEST STANDARD EXCEPTION
        //throw std::runtime_error("This is a test runtime error!"); 
        
        // TEST UNKNOWN EXCEPTION
        // throw 123; // Throw a non-std::exception type
        
        // TEST UNHANDLED EXCEPTION
        // throw; // Uncommenting this will cause an unhandled exception
    
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
            // Instead of just returning -1, log the error
            LOG_ERROR_MESSAGE(L"GetMessage failed with -1");
            return -1;  // error occurred
        }
        else
        {
            return msg.wParam;  
        }
    } // End of try block
    catch (const std::exception& ex) {
        CATCH_AND_LOG_STD_EXCEPTION(ex);
        return -1; // Indicate an error
    }
    catch (...) { // Catch all other types of exceptions
        CATCH_AND_LOG_UNKNOWN_EXCEPTION();
        return -1; // Indicate an error
    }
}
