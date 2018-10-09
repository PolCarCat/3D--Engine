#pragma once
#include "WinBase.h"
class WinConsole :
	public WinBase
{
public:


	WinConsole(Application* parent, bool start_enabled = true);
	~WinConsole();


	bool Update();

};

