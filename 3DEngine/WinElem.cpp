#include "WinElem.h"
#include "Application.h"
#include "ImGui/imgui.h"


WinElem::WinElem(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{

};

bool WinElem::Update()
{
	ImGui::Begin("Element");
	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (ImGui::Checkbox("Draw Normals", &App->renderer3D->drawNormals));
	}
	ImGui::End();
	return true;
}

WinElem::~WinElem()
{
}
