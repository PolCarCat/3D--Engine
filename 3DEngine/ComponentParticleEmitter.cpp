#include "ComponentParticleEmitter.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "Application.h"
#include "MathGeoLib/MathGeoLib.h"

#include "mmgr/mmgr.h"

ComponentParticleEmitter::ComponentParticleEmitter()
{
	type = PARTICLE_EMITTER;

	emitterLifetime = 10;


	//ONLY FOR TESTING
	speed.max = 5;
	speed.min = 0;

	particleLifetime.max = 100;
	particleLifetime.min = 50;

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
	float dt = App->timeManager->GetRealDeltaTime();

	if (time <= emitterLifetime || emitterLifetime < 0)
	{
		SpawnParticles(dt);
		time += dt;

	}
		
	UpdateParticles(dt);

	return true;
}

void ComponentParticleEmitter::UpdateUI()
{
	if (ImGui::CollapsingHeader("Particle Emitter"))
	{

		ImGui::Text("Emitter");
		ImGui::Separator();

		if (ImGui::SliderFloat("LifeTime", &emitterLifetime, -1, 100))
		{
			time = 0;
		}

		ImGui::Text("LifeTime: %.2f", emitterLifetime - time);


		ImGui::Text("Paticle");
		ImGui::Separator();

		ImGui::DragFloat3("Direction", (float*)&direction, 0.25f);
		ImGui::SliderFloat("Direction Variation", &dirVartiation, 0, 180);

		int minlife = particleLifetime.min;
		int maxlife = particleLifetime.max;

		//LifeTime
		ImGui::PushID("LT");

		ImGui::Text("Particle Life Time");
		if (ImGui::SliderInt("Min", &minlife, 0, maxlife))
			particleLifetime.min = minlife;

		if (ImGui::SliderInt("Max", &maxlife, minlife, 100))
			particleLifetime.max = maxlife;

		ImGui::PopID();

		//Speed
		ImGui::PushID("Speed");

		ImGui::Text("Speed");
		ImGui::SliderFloat("Min", &speed.min, 0, speed.max);
		ImGui::SliderFloat("Max", &speed.max, speed.min, 100);

		ImGui::PopID();

		//Start Size
		ImGui::PushID("SSize");

		ImGui::Text("Start Size");
		ImGui::SliderFloat("Min", &startSize.min, 0, startSize.max);
		ImGui::SliderFloat("Max", &startSize.max, startSize.min, 100);

		ImGui::PopID();

		//End Size
		ImGui::PushID("ESize");

		ImGui::Text("End Size");
		ImGui::SliderFloat("Min", &endSize.min, 0, endSize.max);
		ImGui::SliderFloat("Max", &endSize.max, endSize.min, 100);

		ImGui::PopID();

		//Start Spin
		ImGui::PushID("SSpin");

		ImGui::Text("Start Spin");
		ImGui::SliderFloat("Min", &startSpin.min, 0, startSpin.max);
		ImGui::SliderFloat("Max", &startSpin.max, startSpin.min, 100);

		ImGui::PopID();

		//End Spin
		ImGui::PushID("ESpin");

		ImGui::Text("End Spin");
		ImGui::SliderFloat("Min", &endSpin.min, 0, endSpin.max);
		ImGui::SliderFloat("Max", &endSpin.max, endSpin.min, 100);

		ImGui::PopID();


		ImGui::PushID("SColor");

		ImGui::Text("StartColor");

		ImGui::ColorEdit4("Min", (float*)&startColor.min);
		ImGui::ColorEdit4("Max", (float*)&startColor.max);

		ImGui::PopID();

		ImGui::PushID("EColor");

		ImGui::Text("EndColor");

		ImGui::ColorEdit4("Min", (float*)&endColor.min);
		ImGui::ColorEdit4("Max", (float*)&endColor.max);

		ImGui::PopID();


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

	baseParticle.Set(GetRandom(startSize), GetRandom(endSize), GetRandom(startSpin), GetRandom(endSpin), GetRandom(speed), GetRandom(particleLifetime) ,parent->transform->position, dir.Normalized(), GetRandom(startColor), GetRandom(endColor));


	//Create New Particle
	Particle* newParticle = new Particle(baseParticle);
	particles.push_back(newParticle);


	emisionTimer.Start();
}

void ComponentParticleEmitter::SpawnParticles(float dt)
{
	if (emisionTimer.Read() * dt >= period)
	{
		CreateParticle();
		current_emissions++;
	}
}

void ComponentParticleEmitter::UpdateParticles(float dt)
{

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

Color ComponentParticleEmitter::GetRandom(range<Color> r)
{
	Color c;
	c.r = (ldexp(pcg32_random(), -32) * (r.max.r - r.min.r)) + r.min.r;
	c.g = (ldexp(pcg32_random(), -32) * (r.max.g - r.min.g)) + r.min.g;
	c.b = (ldexp(pcg32_random(), -32) * (r.max.b - r.min.b)) + r.min.b;
	c.a = (ldexp(pcg32_random(), -32) * (r.max.a - r.min.a)) + r.min.a;

	return c;
}