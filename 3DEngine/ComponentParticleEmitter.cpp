#include "ComponentParticleEmitter.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "Application.h"

ComponentParticleEmitter::ComponentParticleEmitter()
{
	type = PARTICLE_EMITTER;


	//ONLY FOR TESTING
	speed.max = 5;
	speed.min = 0;

	lifetime.max = 200;
	lifetime.min = 100;

	startSize.max = 5;
	startSize.min = 2;

	endSize.max = 2;
	endSize.min = 1;

	startSpin.max = 2;
	startSpin.min = 1;

	endSpin.max = 0.5;
	endSpin.min = 0;

	direction.max = { 0,1,0 };
	startColor.max = { 1.0f, 0.0f, 0.0f, 1.0f };
	endColor.max = { 1.0f, 0.0f, 0.0f, 1.0f };


	period = 2;
	frequency = 5;

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
	return false;
}

void ComponentParticleEmitter::CreateParticle()
{
	//Direction and color should be random

	baseParticle.Set(GetRandom(startSize), GetRandom(endSize), GetRandom(startSpin), GetRandom(endSpin), direction.max, startColor.max, endColor.max);
	baseParticle.speed = 1;
	baseParticle.lifetime = lifetime.max;
	

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
	//RANDOM PLS
	return (ldexp(pcg32_random(), -32) * (r.max - r.min)) + r.min;
}

uint ComponentParticleEmitter::GetRandom(range<uint> r)
{
	return (pcg32_boundedrand(r.max - r.min)) + r.min;
}