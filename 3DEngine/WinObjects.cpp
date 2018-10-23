#include "WinObjects.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ResMesh.h"
#include "WinElem.h"
#include "GameObject.h"

WinObjects::WinObjects(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
};


WinObjects::~WinObjects()
{
}



bool WinObjects::Update()
{

	ImGui::Begin("Objects");


	if (ImGui::TreeNode("Game Objects"))
	{

		
		int node_clicked = -1;      
		int i = 1;
		for (std::vector<GameObject*>::iterator item = App->scene->root.objChilds.begin(); item != App->scene->root.objChilds.end(); item++)
		{
			
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; 
			ImGui::TreeNodeEx((*item)->GetName(), node_flags);
			if (ImGui::IsItemClicked())
			{
				App->scene->selectedObj = (*item);
				node_clicked = i;
			}
			i++;
		}

	

		ImGui::TreePop();
	}




	ImGui::End();
	return true;
}