#include "WinElem.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "ResMesh.h"
#include "ComponentMaterial.h"
#include <list>

WinElem::WinElem(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{

};

WinElem::~WinElem()
{
}

bool WinElem::Start()
{

	return true;
}
bool WinElem::Update()
{
	ImGui::Begin("Element");
	if (App->scene->selectedObj != nullptr)
	{	
		App->scene->selectedObj->UpdateUI();
	}
	ImGui::End();

	ImGui::Begin("Material");
	if (App->scene->selectedMat != nullptr)
	{
		App->scene->selectedMat->UpdateUI();
	}
	ImGui::End();

	return true;
}

