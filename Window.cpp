#define UNICODE
#define _UNICODE

#include "Window.h" // Definition of the Window class
#include "ExceptionHandler.h" // For logging exceptions (though not directly used in this file after refactor, good to keep if Window methods might throw/log)
#include "Graphics.h" // Include Graphics class definition
#include <sstream>      // For std::wostringstream (used in HandleMessage for LBUTTONDOWN)

// --- Window Class Implementation ---

// Constructor: Registers class, creates window
Window::Window(HINSTANCE hInstance, const std::wstring& windowTitle, const wchar_t* windowClassName, int width_param, int height_param)
    : hInstance_(hInstance), className_(windowClassName), hWnd_(nullptr), width(width_param), height(height_param), pGfx(nullptr) { // Initialize all member variables including pGfx
    WNDCLASSEXW wc = {0}; 
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
        CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, // Use member variables for CreateWindowExW

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
    
    // create Graphics object
    pGfx = std::make_unique<Graphics>(hWnd_); // Create Graphics object with the window handle

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


std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		// check for quit because peekmessage does not signal this via return val
		if( msg.message == WM_QUIT )
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	// return empty optional when not quitting app
	return {};
}


// Static WndProc: Delegates to the instance's HandleMessage
LRESULT CALLBACK Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* pWnd = nullptr;
    if (msg == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam); // Use CREATESTRUCTW
        pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); 
        pWnd->hWnd_ = hWnd; // Ensure hWnd_ is set early
    } else {
        pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)); // Use GetWindowLongPtrW
    }

    if (pWnd) {
        return pWnd->HandleMessage(hWnd, msg, wParam, lParam); // Corrected to HandleMessage
    } else {
        return DefWindowProcW(hWnd, msg, wParam, lParam); // Use DefWindowProcW
    }
}

Graphics& Window::Gfx()
{
    return *pGfx; // Return the Graphics object
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { // Corrected to HandleMessage
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        // KEYBOARD MESSAGES
        case WM_KEYDOWN:
            if( !(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())   // filter autorepeat
            {
            // Pass key events to the Keyboard class
            if (wParam < 256) { // Ensure wParam is a valid key code
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            }
            // Test: Change window title on F key press
            // if (wParam == 'F') { // Check if the F key was pressed
            //     SetWindowTextW(hWnd, L"F Key Pressed"); // Set new window title (Unicode)
            // }
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

        case WM_SYSKEYDOWN:
            // Handle system key down events (e.g., Alt key)
            if (wParam < 256) { // Ensure wParam is a valid key code
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;

        case WM_SYSKEYUP:
            // Handle system key up events (e.g., Alt key)
            if (wParam < 256) { // Ensure wParam is a valid key code
                kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            }
            break;

        case WM_CHAR:
            // Pass char events to the Keyboard class
            kbd.OnChar(static_cast<unsigned char>(wParam));
            break; 

        // Test: Display mouse coordinates in window title on left button down
        // case WM_LBUTTONDOWN:
        //     { // New scope for variables
        //         const POINTS pt = MAKEPOINTS(lParam); // Extract x and y coordinates from lParam
        //         std::wostringstream oss; // Use std::wostringstream for Unicode strings
        //         oss << L"Mouse Clicked at (" << pt.x << L", " << pt.y << L")"; // Format the string
        //         SetWindowTextW(hWnd, oss.str().c_str()); // Set the window title with the coordinates
        //     }
        //     break; 
        
        case WM_KILLFOCUS: // Moved this case down ro avoid interference with key/mouse messages if it lacks a break
            kbd.ClearState(); // Clear keyboard state when the window loses focus
            if (GetCapture() == hWnd) { // If this window has mouse capture
                ReleaseCapture();       // Release it
            }
            break; 

        /************* MOUSE MESSAGES ****************/
	    case WM_MOUSEMOVE:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
            // Check if mouse is within the client area
            if( pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height )
            {
                mouse.OnMouseMove( pt.x,pt.y );
                if( !mouse.IsInWindow() )
                {
                    mouse.OnMouseEnter();
                    // Request WM_MOUSELEAVE when mouse leaves client area
                    TRACKMOUSEEVENT tme = {}; // Initialize to zero
                    tme.cbSize = sizeof(TRACKMOUSEEVENT);
                    tme.dwFlags = TME_LEAVE;
                    tme.hwndTrack = hWnd;
                    if (!TrackMouseEvent(&tme)) {
                        // Log error if TrackMouseEvent fails
                        LOG_ERROR_MESSAGE(L"TrackMouseEvent failed in WM_MOUSEMOVE");
                    }
                }
            }
            // Mouse is outside client area
            else
            {
                // If dragging (LEFT button is pressed AND we have capture)
                if( (wParam & MK_LBUTTON) && GetCapture() == hWnd )
                {
                    mouse.OnMouseMove( pt.x,pt.y );
                }
                // If not dragging but mouse is outside, and we thought it was in
                // This is handled by WM_MOUSELEAVE.
            }
            break; 
	    }
        case WM_MOUSELEAVE: // Handle this message
        {
            if (mouse.IsInWindow()) { // Check before calling OnMouseLeave
                 mouse.OnMouseLeave();
            }
            break;
        }
	    case WM_LBUTTONDOWN:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
		    mouse.OnLeftPressed( pt.x,pt.y );
            SetCapture(hWnd); // Capture the mouse for left button drag
		    break;
	    }
	    case WM_RBUTTONDOWN:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
		    mouse.OnRightPressed( pt.x,pt.y );
            SetCapture(hWnd); // Capture the mouse (if you want right-drag)
		    break;
	    }
	    case WM_LBUTTONUP:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
		    mouse.OnLeftReleased( pt.x,pt.y );
            if (GetCapture() == hWnd) { // Only release if we have capture
                ReleaseCapture();
            }
		    break;
	    }
	    case WM_RBUTTONUP:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
		    mouse.OnRightReleased( pt.x,pt.y );
            if (GetCapture() == hWnd) { // Only release if we have capture
                ReleaseCapture();
            }
		    break;
	    }
	    case WM_MOUSEWHEEL:
	    {
		    const POINTS pt = MAKEPOINTS( lParam );
		    const int delta = GET_WHEEL_DELTA_WPARAM( wParam );
		    mouse.OnWheelDelta( pt.x,pt.y,delta );
		    break;
	    }
	    /************** END MOUSE MESSAGES **************/
        default:
            return DefWindowProcW(hWnd, msg, wParam, lParam); 
    }
    return 0; 
}
