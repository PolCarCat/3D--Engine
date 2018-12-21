#include "Firework.h"
#include "Application.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "ComponentCamera.h"

Firework::Firework()
{
	body = new GameObject();
	body->SetName("Firework");
	body->SetParent(&App->scene->root);

	float norm_x = -(1.0f - (float(App->input->Mx) * 2.0f) / (float)App->window->w);
	float norm_y = 1.0f - (float(App->input->My) * 2.0f) / (float)App->window->h;
	LineSegment mouseDir = App->scene->GetCurCam()->GetFrustum().UnProjectLineSegment(norm_x, norm_y);

	float3 dir = float3(norm_x, norm_y, 0);
	float rand = ldexp(pcg32_random_r(&rng), -32);

	life = rand * 2.0f;
	speed = rand * 5.0f;
	direction = dir * rand * 10.0f * speed;
	
	if (rand != 0.5)
		acceleration = rand * 2.0f;
	else
		acceleration = rand * -2.0f;
}


Firework::~Firework()
{

}

bool Firework::Start()
{
	return true;
}

bool Firework::Update()
{
	return true;
}

bool Firework::CleanUp()
{
	return true;
}