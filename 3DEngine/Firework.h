#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include "GameObject.h"
#include "ComponentParticleEmitter.h"
#include "ModuleScene.h"

class Firework
{
public:
	Firework();
	~Firework();

	bool Start();
	bool Update();
	bool CleanUp();

	float3 GetRandom(range<float3> f);

private:
	GameObject *	body;
	float3			direction;
	float3			speed;
	float3			acceleration;
	range<float3>	dirRange;
	range<float3>	speedRange;
	range<float3>	accRange;

};

#endif //__FIREWORK_H__