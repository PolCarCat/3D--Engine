#pragma once
#include "WinBase.h"
class WinObjects :
	public WinBase
{
public:
	WinObjects(Application* parent, bool start_enabled = true);
	~WinObjects();


	bool Update();


};

