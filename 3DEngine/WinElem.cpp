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
			if (curMesh->name.c_str() != nullptr)
				ImGui::Text("%s", curMesh->name.c_str());
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
		if (ImGui::CollapsingHeader("Transform"))
		{

			ImGui::Columns(2);
			{

				ImGui::Text("Position:");
				ImGui::Text("Scale");
				ImGui::Text("Rotation:");
				float x = 0;
				float y = 0;
				float z = 0;
				curMesh->rotation.FromEulerXYZ(x, y, z);
				ImGui::NextColumn();
				ImGui::Text("%.2f %.2f  %.2f", curMesh->position.x, curMesh->position.y, curMesh->position.z);
				ImGui::Text("%d  %d  %d", curMesh->scale.x, curMesh->scale.y, curMesh->scale.z);
				ImGui::Text("%d  %d  %d", x, y, z);

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
					ImGui::NewLine();
					ImGui::Text("Transformation");
					ImGui::Separator();
					ImGui::SliderFloat("Position X", &curMesh->tex.position.x, 0, 1);
					ImGui::SliderFloat("Position Y", &curMesh->tex.position.y, 0, 1);
					ImGui::SliderFloat("Scale X", &curMesh->tex.scale.x, 0, 10);
					ImGui::SliderFloat("Scale Y", &curMesh->tex.scale.y, 0, 10);
					ImGui::SliderFloat("Angle", &curMesh->tex.angle, 0, 360);

					float s = ImGui::GetWindowContentRegionWidth() - 20;
					ImGui::Image((void*)curMesh->tex.id, ImVec2(s , s),ImVec2(0,1),ImVec2(1,0));

				}

			}
		}
	}
	ImGui::End();
	return true;
}

