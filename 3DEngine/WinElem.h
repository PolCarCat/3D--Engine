#pragma once
#include "WinBase.h"
class WinElem :
	public WinBase
{
public:
	WinElem(Application* parent, bool start_enabled = true);
	~WinElem();

	bool Update();
};

