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
	firework.minLife = 0.5;
	firework.body = new GameObject();
	firework.body->SetName("Firework");
	if (App->scene->quadTree.GetRoot() != nullptr)
		firework.body->SetParent(&App->scene->root);

	float norm_x = -(1.0f - (float(App->input->Mx) * 2.0f) / (float)App->window->w);
	float norm_y = 1.0f - (float(App->input->My) * 2.0f) / (float)App->window->h;
	if (App->scene->quadTree.GetRoot() != nullptr)
		LineSegment mouseDir = App->scene->GetCurCam()->GetFrustum().UnProjectLineSegment(norm_x, norm_y);

	float3 dir = float3(norm_x, norm_y, 0);
	float rand = ldexp(pcg32_random_r(&firework.rng), -32) + firework.minLife;

	firework.body->transform->position = dir;
	firework.life = rand;
	firework.speed = rand * 2.0f;
	firework.direction = dir * rand * 5.0f * firework.speed;
	
	if (dir.y <= 0.0f)
		dir.y = 1.0f;

	if (rand >= 1.0f)
		firework.acceleration = rand / 2.0f;
	else
		firework.acceleration = rand / -2.0f;

	firework.body->AddComponent(PARTICLE_EMITTER);
	sec.Start();
}

bool Firework::Update()
{
	if (sec.ReadSec() < firework.life)
	{
		firework.body->transform->position += (firework.direction - firework.direction * firework.acceleration);
		firework.acceleration *= 1.2;
	}
	else
		Explode();

	return true;
}

bool Firework::CleanUp()
{
	firework.body->Delete();
	return true;
}

void Firework::Explode()
{
	CleanUp();
}