#pragma once
#include "WinBase.h"


class WinConfig :
	public WinBase
{
public:
	WinConfig(Application* parent, bool start_enabled = true);
	~WinConfig();


	bool Update();


private:



};

