#ifndef __WINCAMERA_H__
#define __WINCAMERA_H__

#include "WinBase.h"
#include "ResTexture.h"

class WinCamera :
	public WinBase
{
public:
	WinCamera(Application* parent, bool start_enabled = true);
	~WinCamera();

	bool Update();

};

#endif //__WINCAMERA_H__