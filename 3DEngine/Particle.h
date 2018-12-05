#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Color.h"
#include "ComponentTransform.h"
class ComponentBillboard;



class ParticleInfo
{
public:
	ComponentBillboard * billboard = nullptr;

	float speed;

	uint lifetime;

	float startSize;
	float endSize;

	float startSpin;
	float endSpin;

	float3 direction;

	Color startColor;
	Color endColor;

	void Set(float sSize = 0, float eSize = 0, float sSpin = 0, float eSpin = 0, float3 dir = float3::zero, Color sColor = White, Color eColor = White);
};


class Particle
{
public:
	Particle(ParticleInfo i);
	~Particle();

	void Start();
	void Update(float dt);
	void Draw();
	void CleanUp();
	bool Delete();

private:

	float Ratio(float max, float  min);
	Color Ratio(Color max, Color  min);

	ParticleInfo info;

	float speed = 0;
	int lifeTime = 0;
	float size = 0;
	float spin = 0;

	float3 position;
	Color color;
	
	uint framesLeft = 0;
	float lifetimeRatio = 1;

	bool toDelete = false;
};

#endif // !__PARTICLE_H__