#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Reset();

	Uint32 Read();
	float ReadSec() const;
	void SetZero();
	bool IsZero()const;
	void SubstractTime(Timer timer_to_substract_from);
	void AddTime(Timer timer_to_add_from);
	bool Count(float num);

private:

	bool	running = false;
	bool	reset = false;
	Uint32	started_at = 0;
	Uint32	stopped_at;
};

#endif //__TIMER_H__