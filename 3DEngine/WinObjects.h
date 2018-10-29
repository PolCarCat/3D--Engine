#pragma once
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
	void UpdateObj(GameObject* obj);

private:
	GameObject* target = nullptr;
	GameObject* source = nullptr;
};

