#ifndef __WINELEM_H__
#define __WINELEM_H__
#include "WinBase.h"

class ResMesh;

class WinElem :
	public WinBase
{
public:
	WinElem(Application* parent, bool start_enabled = true);
	~WinElem();

	bool Start();
	bool Update();
	ResMesh* curMesh = nullptr;
	

};

#endif //__WINELEM_H_