#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include "GameObject.h"
#include "ComponentParticleEmitter.h"
#include "ModuleScene.h"
#include "pcg-c-basic-0.9/pcg_basic.h"

class FW
{
public:

	GameObject *	body;
	float			life;
	float			speed;
	float			minLife;
	float3			direction;
	bool			active = false;
	Timer			sec;
};

class Firework
{
public:
	Firework();
	~Firework();

	void Set();
	bool Update();
	bool CleanUp(int index = 0);
	void Explode(int index = 0);

private:
	FW					fireworks[30];
	int					index = 0;
	pcg32_random_t		rng;
};

#endif //__FIREWORK_H__