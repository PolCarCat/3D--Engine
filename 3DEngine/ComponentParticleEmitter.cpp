#include "ComponentParticleEmitter.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "Application.h"
#include "MathGeoLib/MathGeoLib.h"

#include "mmgr/mmgr.h"

ComponentParticleEmitter::ComponentParticleEmitter()
{
	type = PARTICLE_EMITTER;


	//ONLY FOR TESTING
	speed.max = 5;
	speed.min = 0;

	lifetime.max = 100;
	lifetime.min = 50;

	startSize.max = 10;
	startSize.min = 9;

	endSize.max = 5;
	endSize.min = 4;

	startSpin.max = 2;
	startSpin.min = 1;

	endSpin.max = 0.5;
	endSpin.min = 0;

	direction = { 0,1,0 };
	dirVartiation = 20;

	startColor.max = { 1.0f, 0.0f, 0.0f, 1.0f };
	endColor.max = { 0.0f, 0.0f, 1.0f, 1.0f };


	period = 0.1f;
	frequency = 20;

	max_emissions = 100;

}


ComponentParticleEmitter::~ComponentParticleEmitter()
{
}

bool ComponentParticleEmitter::Start()
{


	//baseParticle.billboard = App->resourceManager->GetBillboard();
	return true;
}

bool ComponentParticleEmitter::Update()
{
	UpdateParticles(App->timeManager->GetRealDeltaTime());

	return true;
}

void ComponentParticleEmitter::UpdateUI()
{

}

bool ComponentParticleEmitter::CleanUp()
{
	for (std::list<Particle*>::iterator item = particles.begin(); item != particles.end();)
	{
		(*item)->CleanUp();
		if (*item != nullptr)
		{
			delete *item;
			*item = nullptr;
		}

		item = particles.erase(item);
		
	}
	return false;
}

void ComponentParticleEmitter::CreateParticle()
{
	//Direction and color should be random


	float radius = direction.Length() * Atan(dirVartiation * DEGTORAD);

	float random = ldexp(pcg32_random(), -32);
	double a = random * 2 * pi;

	random = ldexp(pcg32_random(), -32);
	float r = radius * sqrt(random);

	float x = r * cos(a);
	float y = r * sin(a);

	float3 dir = { x + direction.x, direction.y, y + direction.z };

	baseParticle.Set(GetRandom(startSize), GetRandom(endSize), GetRandom(startSpin), GetRandom(endSpin), GetRandom(speed), GetRandom(lifetime) ,parent->transform->position, dir, startColor.max, endColor.max);


	Particle* newParticle = new Particle(baseParticle);
	particles.push_back(newParticle);


	emisionTimer.Start();
}

void ComponentParticleEmitter::UpdateParticles(float dt)
{
	if (emisionTimer.Read() * dt >= period) 
	{
		CreateParticle();
		current_emissions++;
	}

	for (std::list<Particle*>::iterator item = particles.begin(); item != particles.end();)
	{
		if (!(*item)->Delete())
		{
			(*item)->Update(dt);
			item++;
		}
		else
		{
			(*item)->CleanUp();
			if (*item != nullptr)
			{
				delete *item;
				*item = nullptr;
			}

			item = particles.erase(item);
		}
	}
}

float ComponentParticleEmitter::GetRandom(range<float> r)
{
	return (ldexp(pcg32_random(), -32) * (r.max - r.min)) + r.min;
}

uint ComponentParticleEmitter::GetRandom(range<uint> r)
{
	return (ldexp(pcg32_random(), -32) * (r.max - r.min)) + r.min;
}