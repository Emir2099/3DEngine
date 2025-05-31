#pragma once // Prevents multiple inclusions of this header

#include <windows.h> // Ensure this is first
#include <string>    // Required for std::wstring
#include "Keyboard.h" // The Window class uses a Keyboard object
#include "Mouse.h"    // The Window class uses a Mouse object

// --- Window Class Definition ---
// For better organization in larger projects, this class could be in Window.h and Window.cpp
class Window {
public:
    Keyboard kbd; // Public keyboard object 
    Mouse mouse; // Public mouse object 
    // Constructor: Registers class, creates window
    Window(HINSTANCE hInstance, const std::wstring& className, const wchar_t* windowTitle, int width, int height);
    
    // Destructor: Unregister class
    ~Window();

    HWND GetHwnd() const;

private:
    // Static WndProc: Delegates to the instance's HandleMessage
    // This function is called by the Windows operating system to process messages for windows of this class.
    // It's static because it's a C-style callback and doesn't have an implicit 'this' pointer.
    // It retrieves the 'this' pointer of the specific Window instance using GetWindowLongPtr/SetWindowLongPtr.
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    // Instance-specific message handler
    // This function is called by StaticWndProc to handle messages for a specific Window instance.
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hWnd_; // Handle to this window
    HINSTANCE hInstance_; // Handle to the application instance
    std::wstring className_; // The name of the window class, using std::wstring for easier management
    int width;
	int height;
};
// --- End Window Class Definition ---
