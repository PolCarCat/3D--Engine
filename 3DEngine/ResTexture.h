#ifndef __RESTEXTURE_H__
#define __RESTEXTURE_H__
#include "Resource.h"
#include "MathGeoLib/MathGeoLib.h"

class ResTexture : public Resource
{
public:
	ResTexture(uint32_t UUID = 0);
	~ResTexture();

	uint id = 0;
	uint width = 0;
	uint heigth = 0;

	float2 position = { 0,0 };
	float2 scale = { 1,1 };
	float angle = 0;

	void Init();
	void CleanUp();
};

#endif // !__RESTEXTURE_H__