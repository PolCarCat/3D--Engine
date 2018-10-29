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

	UpdateObj(&App->scene->root);

		
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

bool WinObjects::PostUpdate()
{
	if (target != nullptr)
	{
		target->AddGameObject(source);
		target = nullptr;
		source = nullptr;
	}

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

void WinObjects::UpdateObj(GameObject* obj)
{
	int n = 0;
	for (std::vector<GameObject*>::iterator item = obj->objChilds.begin(); item != obj->objChilds.end();item++)
	{
		ImGui::PushID(n);

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if ((*item)->objChilds.size() == 0)
			node_flags |= ImGuiTreeNodeFlags_Leaf;

		ImGui::TreeNodeEx((*item)->GetName(), node_flags);

		ImGui::Indent();
		UpdateObj(*item);
		ImGui::Unindent();

		if (ImGui::IsItemClicked())
		{
			App->scene->selectedObj = (*item);
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			source = *item;
			ImGui::SetDragDropPayload("Payload", &n, sizeof(int));
			ImGui::Text("%s", (*item)->GetName());
			ImGui::EndDragDropSource();

		}

		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Payload"))
			{
				IM_ASSERT(payload->DataSize == sizeof(int));
				int payload_n = *(const int*)payload->Data;
				target = App->scene->root.objChilds[payload_n];

			}

			ImGui::EndDragDropTarget();

		}

		ImGui::PopID();
		n++;
	}
}