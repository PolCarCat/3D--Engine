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

void ComponentMaterial::SetName(const char* n)
{
	name = n;
}

void ComponentMaterial::SetName(std::string n)
{
	name = n;
}

const char* ComponentMaterial::GetName()
{
	return name.c_str();
}


ResTexture ComponentMaterial::GetTexture()
{
	return tex;
}
void ComponentMaterial::SetTexture(ResTexture _tex)
{
	tex = _tex;
}



void ComponentMaterial::UpdateUI()
{
	ImGui::Text("Material");
	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Text(name.c_str());
	ImGui::NewLine();

	ImGui::Checkbox("Use Texture", &texEnabled);

	if (tex.id != 0)
	{
		uint margin = -100;
		if (texEnabled)
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
			uint margin = -20;
		}
	
		float s = ImGui::GetWindowContentRegionWidth() - 20;
		ImGui::Image((void*)tex.id, ImVec2(s, s), ImVec2(0, 1), ImVec2(1, 0));			
	}

	if (!texEnabled)
	{
		ImVec4 colorImGui = ImVec4(color.r, color.g, color.b, color.a);
		ImGui::ColorPicker4("Color##4", (float*)&color);
	}
	else
	{
		ImGui::ColorButton("Color##3c", *(ImVec4*)&color, 0, ImVec2(80, 80));
		ImGui::SameLine();
		ImGui::Text("Color");
	}
	

}

bool ComponentMaterial::CleanUp()
{
	return true;
}