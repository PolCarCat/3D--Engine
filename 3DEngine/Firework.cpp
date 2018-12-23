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
			dir = float3(mouseDir.a.x, mouseDir.a.y, mouseDir.a.z);
		}

		float rand = ldexp(pcg32_random_r(&rng), -32) * 2 + firework.minLife;

		firework.body->transform->position = dir;
		firework.life = rand;
		firework.speed = rand * 1.5f;
		firework.direction = dir * 5.0f * firework.speed;

		if (dir.y <= 0.0f)
			dir.y = 3.0f;

		firework.body->AddComponent(PARTICLE_EMITTER);

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
				fireworks[i].body->transform->position += (fireworks[i].direction - fireworks[i].direction);
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
	fireworks[index].active = false;
	CleanUp(index);
}