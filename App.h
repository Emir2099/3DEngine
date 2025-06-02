#pragma once
#include "Window.h"
#include "Timer.h"
class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
    // Timer timer; // Timer to track elapsed time
private:
	Window wnd;
	Timer timer;
};