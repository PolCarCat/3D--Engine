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
		ImGui::Checkbox("Grid", &App->renderer3D->drawPlane);
		ImGui::Checkbox("Axis", &App->renderer3D->drawAxis);
		ImGui::Checkbox("Cube", &App->renderer3D->drawCube);
		ImGui::Checkbox("Line", &App->renderer3D->drawLine);
		ImGui::Checkbox("Arrow", &App->renderer3D->drawArrow);
		ImGui::Checkbox("Sphere", &App->renderer3D->drawSphere);
		ImGui::Checkbox("Cylinder", &App->renderer3D->drawCylinder);
		ImGui::Checkbox("Capsule", &App->renderer3D->drawCapsule);
	}


	ImGui::End();
	return true;
}