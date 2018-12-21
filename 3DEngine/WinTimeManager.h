#ifndef __WINTIMEMANAGER_H__
#define __WINTIMEMANAGER_H__

#include "WinBase.h"
class WinTimeManager :
	public WinBase
{
public:
	WinTimeManager(Application* parent, bool start_enabled = true);
	~WinTimeManager();

	bool Update();

private:
	bool paused = false;

};

#endif //__WINTIMEMANAGER_H__