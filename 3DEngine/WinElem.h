#pragma once
#include "WinBase.h"

class Mesh;

class WinElem :
	public WinBase
{
public:
	WinElem(Application* parent, bool start_enabled = true);
	~WinElem();

	bool Start();
	bool Update();
	Mesh* curMesh = nullptr;
	

};

