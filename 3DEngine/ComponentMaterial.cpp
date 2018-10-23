#include "ComponentMaterial.h"
#include "ImGui/imgui.h"

ComponentMaterial::ComponentMaterial()
{
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}


bool ComponentMaterial::Start()
{
	return true;
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::UpdateUI()
{

	if (ImGui::CollapsingHeader("Texture"))
	{
		if (tex.id != 0)
		{
			ImGui::Columns(2);
			{

				ImGui::Text("Texture Id:");
				ImGui::Text("Width");
				ImGui::Text("Heigth");
				ImGui::NextColumn();
				ImGui::Text("%d", tex.id);
				ImGui::Text("%d", tex.width);
				ImGui::Text("%d", tex.heigth);


				ImGui::Columns(1);
				ImGui::NewLine();
				ImGui::Text("Transformation");
				ImGui::Separator();
				ImGui::SliderFloat("Position X", &tex.position.x, 0, 1);
				ImGui::SliderFloat("Position Y", &tex.position.y, 0, 1);
				ImGui::SliderFloat("Scale X", &tex.scale.x, 0, 10);
				ImGui::SliderFloat("Scale Y", &tex.scale.y, 0, 10);
				ImGui::SliderFloat("Angle", &tex.angle, 0, 360);

				float s = ImGui::GetWindowContentRegionWidth() - 20;
				ImGui::Image((void*)tex.id, ImVec2(s, s), ImVec2(0, 1), ImVec2(1, 0));

			}

		}
	}
}

bool ComponentMaterial::CleanUp()
{
	return true;
}