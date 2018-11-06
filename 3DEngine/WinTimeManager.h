#pragma once
#include "WinBase.h"
class WinTimeManager :
	public WinBase
{
public:
	WinTimeManager(Application* parent, bool start_enabled = true);
	~WinTimeManager();

	bool Update();

};

