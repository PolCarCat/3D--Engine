#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include "GameObject.h"
#include "ComponentParticleEmitter.h"
#include "ModuleScene.h"
#include "pcg-c-basic-0.9/pcg_basic.h"

class Firework
{
public:
	Firework();
	~Firework();

	bool Start();
	bool Update();
	bool CleanUp();

private:
	GameObject *	body;
	float			life;
	float			speed;
	float			acceleration;
	float3			direction;
	pcg32_random_t rng;

};

#endif //__FIREWORK_H__