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
	dirVartiation = 180;

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
	if (ImGui::CollapsingHeader("Particle Emitter"))
	{

		ImGui::DragFloat3("Direction", (float*)&direction, 0.25f);
		ImGui::SliderFloat("Direction Variation", &dirVartiation, 0, 180);

	}
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

	//Pick a random direction from the base direction with a the angle vartiation
	LCG lcg;
	float3 randomInSphere = float3::RandomSphere(lcg, { 0,0,0 }, 1);
	float3 vartiation = randomInSphere.Normalized();
	vartiation.x = vartiation.x * dirVartiation * DEGTORAD; 
	vartiation.y = vartiation.y * dirVartiation * DEGTORAD;
	vartiation.z = vartiation.z * dirVartiation * DEGTORAD;


	float3 dir = direction.Normalized() + vartiation;

	baseParticle.Set(GetRandom(startSize), GetRandom(endSize), GetRandom(startSpin), GetRandom(endSpin), GetRandom(speed), GetRandom(lifetime) ,parent->transform->position, dir.Normalized(), startColor.max, endColor.max);


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