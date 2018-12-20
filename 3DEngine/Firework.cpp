#include "Firework.h"
#include "Application.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "ComponentCamera.h"

Firework::Firework()
{
	body = new GameObject();
	body->SetName("Firework");
	body->SetParent(&App->scene->root);

	//float norm_x = -(1.0f - (float(App->input->Mx) * 2.0f) / (float)App->window->w);
	//float norm_y = 1.0f - (float(App->input->My) * 2.0f) / (float)App->window->h;
	//LineSegment picking = frustum.UnProjectLineSegment(norm_x, norm_y);

	dirRange.min = float3(0, 0, 0);
}


Firework::~Firework()
{

}

bool Firework::Start()
{

}

bool Firework::Update()
{

}

bool Firework::CleanUp()
{

}

float3 Firework::GetRandom(range<float3> r)
{
	float3 f(0, 0, 0);
	f.x = (pcg32_random(), -32) * (r.max.x - r.min.x) + r.min.x;
	f.y = (pcg32_random(), -32) * (r.max.y - r.min.y) + r.min.y;
	f.z = (pcg32_random(), -32) * (r.max.z - r.min.z) + r.min.z;
	return f;
}