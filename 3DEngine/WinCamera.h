#pragma once
#include "WinBase.h"
#include "ResTexture.h"

class WinCamera :
	public WinBase
{
public:
	WinCamera(Application* parent, bool start_enabled = true);
	~WinCamera();

	bool Update();
	void BindImage();
	void GenerateBuffer();
	ResTexture image;
	
};

