#include "App.h"
#include <sstream>
#include <iomanip>
#include "ExceptionHandler.h" // Include ExceptionHandler for error handling

App::App()
	:
	wnd(GetModuleHandle(nullptr), L"MyWindowClass", L"The Donkey Fart Box", 800, 600) // Correctly initialize Window with required parameters
{}

int App::Go()
{
	// MSG msg;
	// BOOL gResult;
	// while( (gResult = GetMessage( &msg,nullptr,0,0 )) > 0 )

    while( true )
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		// TranslateMessage( &msg );
		// DispatchMessage( &msg );
        // process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}

	// check if GetMessage call itself borked
	// if( gResult == -1 )
	// {
	// 	// Replace with the appropriate exception handling from ExceptionHandler.h
	// 	LOG_ERROR_MESSAGE(L"GetMessage failed in message pump.");
	// 	throw std::runtime_error("GetMessage failed");
	// }

	// wParam here is the value passed to PostQuitMessage
	// return msg.wParam;
}

void App::DoFrame()
{
    // Test App LOOP
	// const float t = timer.Peek();
	// std::wostringstream woss; // Use wide string stream for Unicode
	// woss << L"Time elapsed: " << std::setprecision(1) << std::fixed << t << L"s";
	// SetWindowTextW(wnd.GetHwnd(), woss.str().c_str()); // Explicitly use SetWindowTextW for wide strings
}