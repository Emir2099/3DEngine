#define UNICODE
#define _UNICODE

#include "Window.h" // Definition of the Window class
#include "ExceptionHandler.h" // For logging exceptions (though not directly used in this file after refactor, good to keep if Window methods might throw/log)
#include <sstream>      // For std::wostringstream (used in HandleMessage for LBUTTONDOWN)

// --- Window Class Implementation ---

// Constructor: Registers class, creates window
Window::Window(HINSTANCE hInstance, const std::wstring& windowTitle, const wchar_t* windowClassName, int width, int height)
    : hInstance_(hInstance), className_(windowClassName), hWnd_(nullptr) {
    WNDCLASSEXW wc = {0}; // Use WNDCLASSEXW for Unicode
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = StaticWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance_;
    wc.hIcon = LoadIconW(NULL, IDI_APPLICATION); // Explicitly use MAKEINTRESOURCEW
    wc.hIconSm = LoadIconW(NULL, IDI_APPLICATION); // Explicitly use MAKEINTRESOURCEW
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW); // Explicitly use MAKEINTRESOURCEW
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = className_.c_str(); // className_ is std::wstring, c_str() gives const wchar_t*

    if (!RegisterClassExW(&wc)) { // Use RegisterClassExW
        throw std::runtime_error("Failed to register window class.");
    }

    hWnd_ = CreateWindowExW(
        0,                              // Optional window styles.
        className_.c_str(),             // Window class
        windowTitle.c_str(),            // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance_, // Instance handle
        this        // Additional application data
    );

    if (hWnd_ == NULL) {
        throw std::runtime_error("Failed to create window.");
    }

    SetWindowTextW(hWnd_, windowTitle.c_str()); // Ensure Unicode version is called
    ShowWindow(hWnd_, SW_SHOWDEFAULT);
    UpdateWindow(hWnd_);
}

// Destructor: Unregister class
Window::~Window() {
    if (hWnd_) {
        // DestroyWindow(hWnd_); // Window is usually destroyed by the OS when the process exits.
                               // Explicitly calling DestroyWindow can be done here if needed for specific cleanup
                               // before the object is fully destructed, but it's not strictly necessary for this setup.
    }
    // Unregister the window class to clean up resources.
    // This should be done when the last window of this class is destroyed.
    // Explicitly call UnregisterClassW
    UnregisterClassW(className_.c_str(), hInstance_);
}

HWND Window::GetHwnd() const {
    return hWnd_;
}

// Static WndProc: Delegates to the instance's HandleMessage
LRESULT CALLBACK Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* pWnd = nullptr;
    if (msg == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam); // Use CREATESTRUCTW
        pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); // Use SetWindowLongPtrW
        pWnd->hWnd_ = hWnd;
    } else {
        pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)); // Use GetWindowLongPtrW
    }

    if (pWnd) {
        return pWnd->HandleMessage(hWnd, msg, wParam, lParam); // Corrected to HandleMessage
    } else {
        return DefWindowProcW(hWnd, msg, wParam, lParam); // Use DefWindowProcW
    }
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { // Corrected to HandleMessage
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        // Test: Change window title on F key press and release
        case WM_KEYDOWN:
            // Pass key events to the Keyboard class
            if (wParam < 256) { // Ensure wParam is a valid key code
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            // Test: Change window title on F key press
            if (wParam == 'F') { // Check if the F key was pressed
                SetWindowTextW(hWnd, L"F Key Pressed"); // Set new window title (Unicode)
            }
            break; 

        case WM_KEYUP:
            // Pass key events to the Keyboard class
            if (wParam < 256) { // Ensure wParam is a valid key code
                kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            }
            // Test: Change window title on F key release
            if (wParam == 'F') { // Check if the F key was released
                SetWindowTextW(hWnd, L"F Key Released"); // Set new window title (Unicode)
            }
            break; 

        case WM_CHAR:
            // Pass char events to the Keyboard class
            kbd.OnChar(static_cast<unsigned char>(wParam));
            break; 

        // Test: Display mouse coordinates in window title on left button down
        case WM_LBUTTONDOWN:
            { // New scope for variables
                const POINTS pt = MAKEPOINTS(lParam); // Extract x and y coordinates from lParam
                std::wostringstream oss; // Use std::wostringstream for Unicode strings
                oss << L"Mouse Clicked at (" << pt.x << L", " << pt.y << L")"; // Format the string
                SetWindowTextW(hWnd, oss.str().c_str()); // Set the window title with the coordinates
            }
            break; 
        
        case WM_KILLFOCUS: // Moved this case down to avoid interfering with key/mouse messages if it lacks a break
            kbd.ClearState(); // Clear keyboard state when the window loses focus
            break; 

        default:
            return DefWindowProcW(hWnd, msg, wParam, lParam); // Use DefWindowProcW
    }
    return 0; // Default return for handled messages
}
