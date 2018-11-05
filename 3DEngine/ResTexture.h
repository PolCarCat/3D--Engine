#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

class ResTexture
{
public:
	ResTexture();
	~ResTexture();

	uint id = 0;
	uint width = 0;
	uint heigth = 0;

	float2 position = { 0,0 };
	float2 scale = { 1,1 };
	float angle = 0;

	std::string name;
	std::string path;
	void CleanUp();
};

