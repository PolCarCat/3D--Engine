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

		firework.minLife = 2.0f;
		firework.body = new GameObject();
		firework.body->SetName("Firework");
		if (App->scene->quadTree.GetRoot() != nullptr)
			firework.body->SetParent(&App->scene->root);

		float norm_x = -(1.0f - (float(App->input->Mx) * 2.0f) / (float)App->window->w);
		float norm_y = 1.0f - (float(App->input->My) * 2.0f) / (float)App->window->h;
		float3 dir = float3::zero;
		if (App->scene->quadTree.GetRoot() != nullptr)
		{
			LineSegment mouseDir = App->scene->GetGhostCam()->GetFrustum().UnProjectLineSegment(norm_x, norm_y);
			dir = float3(mouseDir.a.x, mouseDir.a.y, mouseDir.a.z + 10.0f);
		}

		float rand = ldexp(pcg32_random_r(&rng), -32) * 2;

		firework.body->transform->position = dir;
		firework.life = rand + firework.minLife;
		firework.speed = rand * 2.0f;
		firework.direction = dir * 0.2f * firework.speed;
		firework.direction.x = 0.0f;
		firework.direction.z = 0.0f;

		firework.body->AddComponent(PARTICLE_EMITTER);

		for (std::vector<Component*>::iterator item = firework.body->compChilds.begin(); item != firework.body->compChilds.end(); item++)
		{
			if ((*item)->GetType() == PARTICLE_EMITTER)
			{
				((ComponentParticleEmitter*)*item)->Set(0.1f, 0.1f, 1.0f, 2.0f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, Red, Red, Green, Green, 0.2f);
			}
			((ComponentParticleEmitter*)*item)->SetArea(NONE);
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
				fireworks[i].body->transform->position += fireworks[i].direction;
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