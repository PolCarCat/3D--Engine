#include "WinTest.h"
#include "Application.h"
#include "ImGui/imgui.h"


WinTest::WinTest(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
}


WinTest::~WinTest()
{
}

bool WinTest::Update()
{
	ImGui::Begin("Test Window");
	
	if (ImGui::CollapsingHeader("Random Number Generator"))
	{
		ImGui::Text("%.2f", random_f);

		if (ImGui::Button("Generate random float between 0 and 1"))
		{
			random_f = ldexp(pcg32_random_r(&rng), -32);
		}

		ImGui::Separator();

		ImGui::SliderInt("Min", &min, 0, 100);
		ImGui::SliderInt("Max", &max, 0, 100);

		if (max < min)
			max = min;

		ImGui::Text("%d", random_bounded);

		if (ImGui::Button("Generate random integer between two numbers"))
		{
			if (max - min > 0)
				random_bounded = (int)pcg32_boundedrand_r(&rng, max - min + 1) + min;
			else
				random_bounded = 0;
		}
	}

	if (ImGui::CollapsingHeader("Primitives"))
	{
		ImGui::Checkbox("Direct Cube", &App->renderer3D->directCube);
		ImGui::Checkbox("Vertex arrays Cube", &App->renderer3D->varrCube);
		ImGui::Checkbox("Indices Cube", &App->renderer3D->indCube);
		ImGui::Checkbox("Ray", &App->renderer3D->ray);
		ImGui::Checkbox("Arrow", &App->renderer3D->arrow);
		ImGui::Checkbox("Plane", &App->renderer3D->plane);
		ImGui::Checkbox("Sphere", &App->renderer3D->sphere);
	}


	ImGui::End();
	return true;
}