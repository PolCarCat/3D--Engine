#ifndef __WINOBJECTS_H__
#define __WINOBJECTS_H__
#include "WinBase.h"

class GameObject;

class WinObjects :
	public WinBase
{
public:
	WinObjects(Application* parent, bool start_enabled = true);
	~WinObjects();


	bool Update();
	bool PostUpdate();
	void UpdateMatWin();
	bool UpdateObj(GameObject* obj);

private:
	GameObject* target = nullptr;
	GameObject* source = nullptr;
};

#endif //__WINOBJECTS_H__