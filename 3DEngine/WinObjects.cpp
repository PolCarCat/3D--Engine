#include "WinObjects.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "WinElem.h"

WinObjects::WinObjects(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
};


WinObjects::~WinObjects()
{
}



bool WinObjects::Update()
{

	ImGui::Begin("Objects");

	/*	for (std::list<Mesh*>::iterator item = App->renderer3D->meshes.begin(); item != App->renderer3D->meshes.end(); item++)
		{
			
			ImGui::TreeNode((*item)->name);

			ImGui::TreePop();
	
		}*/

	if (ImGui::TreeNode("Meshes"))
	{
	
		
		int node_clicked = -1;      
		int i = 1;
		for (std::list<Mesh*>::iterator item = App->renderer3D->meshes.begin(); item != App->renderer3D->meshes.end(); item++)
		{
			
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; 
			ImGui::TreeNodeEx((*item)->name.c_str(), node_flags);
			if (ImGui::IsItemClicked())
			{
				App->imgui->element->curMesh = (*item);
				node_clicked = i;
			}
			i++;
		}

	

		ImGui::TreePop();
	}




	ImGui::End();
	return true;
}