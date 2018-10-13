#include "WinElem.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
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
	if (curMesh != nullptr)
	{
	
		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::NewLine();
			ImGui::Separator();
			if (curMesh->name != nullptr)
				ImGui::Text("%s", curMesh->name);
			else
				ImGui::Text("This Mesh hasn't name");

			ImGui::Separator();
			ImGui::NewLine();

			if (ImGui::Checkbox("Draw Normals", &App->renderer3D->drawNormals));
			if (ImGui::Checkbox("Draw Bounding Box", &App->renderer3D->drawBBox));

			ImGui::NewLine();

			ImGui::Columns(2);
			{

				ImGui::Text("Num Vertices:");
				ImGui::Text("Num Indices:");
				ImGui::Text("Num Normals:");
				ImGui::NextColumn();
				ImGui::Text("%d", curMesh->num_vertex);
				ImGui::Text("%d", curMesh->num_indice);
				ImGui::Text("%d", curMesh->num_normals);
				
			}
			ImGui::Columns(1);
		}

		if (ImGui::CollapsingHeader("Texture"))
		{
			if (curMesh->tex.id != 0)
			{
				ImGui::Columns(2);
				{

					ImGui::Text("Texture Id:");
					ImGui::Text("Width");
					ImGui::Text("Heigth");
					ImGui::NextColumn();
					ImGui::Text("%d", curMesh->tex.id);
					ImGui::Text("%d", curMesh->tex.width);
					ImGui::Text("%d", curMesh->tex.heigth);


					ImGui::Columns(1);
					float s = ImGui::GetWindowContentRegionWidth();
					ImGui::Image((void*)curMesh->tex.id, ImVec2(s , s),ImVec2(0,1),ImVec2(1,0));

				}

			}
		}
	}
	ImGui::End();
	return true;
}

