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
	float			acceleration;
	float			minLife;
	float3			direction;
	pcg32_random_t	rng;
};

class Firework
{
public:
	Firework();
	~Firework();

	void Set();
	bool Update();
	bool CleanUp();
	void Explode();

private:
	FW		firework;
	Timer	sec;
};

#endif //__FIREWORK_H__