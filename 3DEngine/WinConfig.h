#ifndef __WINCONFIG_H__
#define __WINCONFIG_H__
#include "WinBase.h"


class WinConfig :
	public WinBase
{
public:
	WinConfig(Application* parent, bool start_enabled = true);
	~WinConfig();


	bool Update();

};

#endif //__WINCONFIG_H__