#include "Firework.h"
#include "Application.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "ComponentCamera.h"

Firework::Firework()
{
}

Firework::~Firework()
{
	CleanUp();
}

void Firework::Set()
{
	for (int i = 0; i != 30; ++i)
	{
		index = -1;

		if (!fireworks[i].active)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		FW firework;

		firework.minLife = 1.0f;
		firework.minLifeExp = 1.0f;
		firework.body = new GameObject();
		firework.body->SetName("Firework");
		if (App->scene->quadTree.GetRoot() != nullptr)
			firework.body->SetParent(&App->scene->root);

		float rand = ldexp(pcg32_random_r(&rng), -32);

		firework.body->transform->position = float3::zero;
		firework.life = rand + firework.minLife;
		firework.lifeExp = rand * 2.0f + firework.minLifeExp;
		firework.speed = 0.1f + rand * 0.3f;
		firework.direction = float3(rand, rand, rand) * 2.0f * firework.speed;
		firework.direction.x = 0.0f;
		firework.direction.z = 0.0f;

		firework.body->AddComponent(PARTICLE_EMITTER);

		for (std::vector<Component*>::iterator item = firework.body->compChilds.begin(); item != firework.body->compChilds.end(); item++)
		{
			if ((*item)->GetType() == PARTICLE_EMITTER)
			{
				((ComponentParticleEmitter*)*item)->Set(0.1f, 0.1f, 1.0f, 2.0f, 0.6f, 0.8, 0.1f, 0.3f, 0.1f, 0.1f, 0.1f, 0.1f, White, Black, White, Black, 0.2f);
				((ComponentParticleEmitter*)*item)->SetArea(NONE);
				((ComponentParticleEmitter*)*item)->GetBaseParticle().billboard->SetTexture(App->scene->CheckMaterial("fire"));
			}
		}

		firework.active = true;
		fireworks[index] = firework;
	}
}

bool Firework::Update()
{
	for (int i = 0; i != 30; ++i)
	{
		if (fireworks[i].active)
		{
			if (fireworks[i].sec.IsZero())
				fireworks[i].sec.Start();

			if (fireworks[i].sec.ReadSec() < fireworks[i].life)
			{
				fireworks[i].body->transform->position.x = 0.0f;
				fireworks[i].body->transform->position.z = 0.0f;
				if (fireworks[i].direction.y < 0)
					fireworks[i].direction.y = -fireworks[i].direction.y;
				fireworks[i].body->transform->position += fireworks[i].direction;
			}
			else if (fireworks[i].sec.ReadSec() < (fireworks[i].life + fireworks[i].lifeExp))
			{
				if (!fireworks[i].exploded)
				{
					for (std::vector<Component*>::iterator item = fireworks[index].body->compChilds.begin(); item != fireworks[index].body->compChilds.end(); item++)
					{
						if ((*item)->GetType() == PARTICLE_EMITTER)
						{
							((ComponentParticleEmitter*)*item)->Set(10.0f, 20.0f, 0.5f, 1.0f, 0.4f, 0.8f, 0.1f, 0.3f, 0.8f, 2.0f, 2.0f, 3.0f, White, Black, White, Black, 20.0f, float3(30.0f, -15.0f, 30.0f), float3(0.0f, 8.0f, 0.0f));
						}
					}
					fireworks[i].exploded = true;
				}
			}
			else
				Explode(i);
		}
	}

	return true;
}

bool Firework::CleanUp(int index)
{
	fireworks[index].body->Delete();
	return true;
}

void Firework::Explode(int index)
{
	fireworks[index].sec.SetZero();
	fireworks[index].active = false;
	CleanUp(index);
}