#pragma once
#include "WinBase.h"

#include "pcg-c-basic-0.9/pcg_basic.h"
class WinTest :
	public WinBase
{
public:
	WinTest(Application* parent, bool start_enabled = true);
	~WinTest();

	bool Update();

private:
	pcg32_random_t rng;
	int random_bounded = 0;
	int max = 0;
	int min = 0;
	float random_f = 0;
};

