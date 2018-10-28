#include "WinObjects.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ResMesh.h"
#include "WinElem.h"
#include "GameObject.h"
#include "ComponentMaterial.h"

WinObjects::WinObjects(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
};


WinObjects::~WinObjects()
{
}



bool WinObjects::Update()
{

	ImGui::Begin("Objects");

	for (std::vector<GameObject*>::iterator item = App->scene->root.objChilds.begin(); item != App->scene->root.objChilds.end(); item++)
	{
			
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
					
		node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((*item)->GetName(), node_flags);
		if (ImGui::IsItemClicked())
		{
			App->scene->selectedObj = (*item);

		}

	}
		
	if (ImGui::Button("Add Empty Object"))
	{
		App->scene->CreateEmptyObj();
	}
	ImGui::End();


	ImGui::Begin("Materials");
	UpdateMatWin();
	ImGui::End();

	return true;
}


void WinObjects::UpdateMatWin()
{
	for (std::list<ComponentMaterial*>::iterator item = App->scene->materials.begin(); item != App->scene->materials.end(); item++)
	{

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((*item)->GetName(), node_flags);
		if (ImGui::IsItemClicked())
		{
			App->scene->selectedMat = (*item);

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::ColorButton("Color##3c", *(ImVec4*)&(*item)->color, 0, ImVec2(80, 80));
			ImGui::EndTooltip();
		}
	}

	if (ImGui::Button("Add Material"))
	{
		App->scene->CreateMaterial();
	}
}