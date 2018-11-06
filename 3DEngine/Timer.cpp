// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	reset = false;
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Reset()
{
	reset = true;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if (!reset)
	{
		if (running == true)
			return SDL_GetTicks() - started_at;
		else
			return 0;
	}
	else
		return 0;
}

float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}

void Timer::SetZero()
{
	started_at = 0;
}

bool Timer::IsZero() const {

	return(started_at == 0);
}

void Timer::SubstractTime(Timer nwtime) {

	started_at += nwtime.Read();

}

void Timer::AddTime(Timer nwtime) {

	started_at -= nwtime.Read();

}


bool Timer::Count(float num)
{
	bool ret = false;
	if (num <= ReadSec())
	{
		ret = true;
	}

	return ret;
}