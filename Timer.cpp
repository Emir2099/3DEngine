#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept
{
	last = steady_clock::now();
}

float Timer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}