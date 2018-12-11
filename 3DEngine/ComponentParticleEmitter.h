#ifndef __COMPONENTEMITTER_H__
#define __COMPONENTEMITTER_H__

#include "Component.h"
#include "Particle.h"
#include "Timer.h"
#include <list>

template <class T>
struct range
{
	T min; 
	T max; 
};

class ComponentParticleEmitter :
	public Component
{
public:
	ComponentParticleEmitter();
	~ComponentParticleEmitter();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	void CreateParticle();
	void SpawnParticles(float dt);
	void UpdateParticles(float dt);

	float GetRandom(range<float> r);
	uint GetRandom(range<uint> r);
	float3 GetRandom(range<float3> r);
	Color GetRandom(range<Color> r);

private:

	ParticleInfo baseParticle;
	Timer emisionTimer;
	float emisionTime = 0;
	float period = 0;
	float frequency = 0;

	int max_emissions = 0;
	int current_emissions = 0;

	
	range <float> speed;
	range <uint> particleLifetime;
	range <float> startSize;
	range <float> endSize;
	range <float> startSpin;
	range <float> endSpin;

	range <Color> startColor;
	range <Color> endColor;
	
	float3 gravity;
	float3 direction;
	float dirVartiation = 0;

	std::list<Particle*> particles;

	float emitterLifetime = 0;
	float time = 0;
};

#endif // !__COMPONENTEMITTER_H__