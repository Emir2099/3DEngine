#define UNICODE
#define _UNICODE
#include <Windows.h> // Standard Windows header
#include "ExceptionHandler.h" // For CATCH_AND_LOG macros
#include "Window.h"           // Definition of our Window class

// Main application entry point
int CALLBACK WinMain(
    HINSTANCE hInstance,          // Handle to the current instance of the application
    HINSTANCE hPrevInstance,      // Always NULL for modern Windows applications (historical artifact)
    LPSTR lpCmdLine,              // Command line arguments (as an ANSI string)
    int nShowCmd                  // How the window is to be shown (e.g., SW_SHOW, SW_MINIMIZE)
) {
    try { // Start of try block for catching exceptions from Window creation or message loop
        
        // Define properties for our window
        const std::wstring pClassName = L"hw3butts"; // Unique class name for our window, std::wstring for safety
        const wchar_t* windowTitle = L"3D Happy Window";  // Defined title
        const int windowWidth = 800;
        const int windowHeight = 600;

        // Validate window title (Example of a custom check before attempting to create the window)
        if (windowTitle == nullptr || wcslen(windowTitle) == 0) {
            // Log a validation error and then throw to stop execution if it's critical,
            // or just log and continue if it's a non-critical warning.
            LOG_VALIDATION_ERROR(L"Window title cannot be empty.");
            throw std::runtime_error("Window title configuration error."); // Throw to be caught by outer try-catch
        }

        // Validate window dimensions (Example of a custom check)
        if (windowWidth <= 0 || windowHeight <= 0) {
            LOG_VALIDATION_ERROR(L"Window dimensions must be positive.");
            throw std::runtime_error("Window dimensions configuration error."); // Throw to be caught by outer try-catch
        }

        // Create the main window object.
        // The Window class constructor now handles WNDCLASSEX registration, CreateWindowEx, ShowWindow, and UpdateWindow.
        Window wnd(hInstance, pClassName, windowTitle, windowWidth, windowHeight);

        // Message pump (the heart of a Windows application)
        MSG msg = {}; // Initialize msg structure to all zeros
        BOOL bRet;
        // Loop until WM_QUIT is received (GetMessage returns 0)
        while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0) { 
            if (bRet == -1) {
                // Handle error from GetMessage (-1 indicates an error, e.g., invalid window handle if msg.hwnd was used)
                // Since we use nullptr for hWnd in GetMessage, this is less likely for typical app errors
                // but could indicate a more severe system issue.
                LOG_ERROR_MESSAGE(L"GetMessage failed in message pump with error code -1.");
                return -1; // Exit application on GetMessage error
            } else {
                TranslateMessage(&msg); // Translates virtual-key messages (WM_KEYDOWN) into character messages (WM_CHAR)
                DispatchMessage(&msg);  // Dispatches a message to a window procedure (Window::StaticWndProc in our case)

                // Check for Space key press after messages are dispatched.
                // This allows kbd state to be updated by WM_KEYDOWN/WM_KEYUP via DispatchMessage first.
                if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
                    // Using wnd.GetHwnd() to get the window handle for MessageBox parent, making it modal to our app.
                    MessageBox(wnd.GetHwnd(), L"Something Happened!", L"Space Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
                    // Note: Depending on Keyboard::KeyIsPressed behavior (is it a continuous state check or a one-shot event?),
                    // this MessageBox might appear repeatedly if the space key is held down.
                    // The Keyboard class might need a "get key just pressed"/"consume key press" mechanism
                    // or you might clear the specific key state in the Keyboard object here if KeyIsPressed is a continuous state check
                    // and you only want the message box once per press-hold cycle.
                }

                // Test case for alt key handling 
                // if( wnd.kbd.KeyIsPressed( VK_MENU ) )
			    // {
				//     MessageBox( nullptr,L"Something Happon!",L"The alt key was pressed",MB_OK | MB_ICONEXCLAMATION );
			    // }
            }
        }
        
        // Return the exit code specified in PostQuitMessage (usually in msg.wParam)
        // This value is returned to the operating system.
        return static_cast<int>(msg.wParam);

    } catch (const std::exception& ex) {
        // Catch standard C++ exceptions (e.g., std::runtime_error thrown by Window constructor)
        CATCH_AND_LOG_STD_EXCEPTION(ex); // Log standard exceptions using our handler
        return -1; // Indicate an error to the OS
    } catch (...) {
        // Catch all other types of exceptions (non-standard C++ exceptions)
        CATCH_AND_LOG_UNKNOWN_EXCEPTION(); // Log unknown exceptions using our handler
        return -1; // Indicate an error to the OS
    }
}
