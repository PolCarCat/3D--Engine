#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Color.h"
#include "ComponentTransform.h"
class ComponentBillboard;


//All values are a random number between MIN and MAX
template <class T>
struct range 
{
	T min = 0;
	T max = 0;
};

class ParticleInfo
{
	ComponentBillboard * billboard = nullptr;

	range<float> speed;

	range<int> lifetime;

	range<float> startSize;
	range<float> endSize;

	range<float> startSpin;
	range<float> endSpin;

	range<float3> position;

	Color startColor;
	Color endColor;
};


class Particle
{
public:
	Particle();
	~Particle();

	void Start();
	void Update();
	void CleanUp();

private:

	ParticleInfo info;

	float speed = 0;
	int lifetime = 0;
	float size = 0;
	float spin;

	float3 position;
	Color color;
	
};

#endif // !__PARTICLE_H__