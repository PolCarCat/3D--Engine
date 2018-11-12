#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "Application.h"

#include "mmgr/mmgr.h"

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


ResTexture* ComponentMaterial::GetTexture()
{
	return tex;
}
void ComponentMaterial::SetTexture(ResTexture* _tex)
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

	if (tex != nullptr && tex->id != 0)
	{
		uint margin = -100;
		if (texEnabled)
		{
			ImGui::Text("Texture Id:");
			ImGui::Text("Width");
			ImGui::Text("Heigth");
			ImGui::NextColumn();
			ImGui::Text("%d", tex->id);
			ImGui::Text("%d", tex->width);
			ImGui::Text("%d", tex->heigth);


			ImGui::Columns(1);
			ImGui::NewLine();
			ImGui::Text("Transformation");
			ImGui::Separator();
			ImGui::SliderFloat("Position X", &tex->position.x, 0, 1);
			ImGui::SliderFloat("Position Y", &tex->position.y, 0, 1);
			ImGui::SliderFloat("Scale X", &tex->scale.x, 0, 10);
			ImGui::SliderFloat("Scale Y", &tex->scale.y, 0, 10);
			ImGui::SliderFloat("Angle", &tex->angle, 0, 360);
			uint margin = -20;
		}
	
		float s = ImGui::GetWindowContentRegionWidth() - 20;
		ImGui::Image((void*)tex->id, ImVec2(s, s), ImVec2(0, 1), ImVec2(1, 0));
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
	parent = nullptr;
	name.clear();
	return true;
}

bool ComponentMaterial::Save(JSON_Object* json, JsonDoc* doc)
{

	json_object_dotset_number(json, "Type", type);
	json_object_dotset_boolean(json, "Active", active);
	json_object_dotset_boolean(json, "Texture Enabled", texEnabled);
	json_object_dotset_string(json, "Name", name.c_str());


	JSON_Array* col = doc->SetArray(json, "Color");
	json_array_append_number(col, color.r);
	json_array_append_number(col, color.g);
	json_array_append_number(col, color.b);
	json_array_append_number(col, color.a);

	
	return true;
}
bool ComponentMaterial::Load(JSON_Object* json, JsonDoc* doc)
{

	active = json_object_dotget_boolean(json, "Active");
	texEnabled = json_object_dotget_boolean(json, "Texture Enabled");

	std::string matName = json_object_dotget_string(json, "Name");

	JSON_Array* pos = doc->GetObjAr(json, "Color");
	color.r = json_array_get_number(pos, 0);
	color.g = json_array_get_number(pos, 1);
	color.b = json_array_get_number(pos, 2);
	color.a = json_array_get_number(pos, 3);

	name = matName;
	tex = App->loader->texImporter.LoadTex(matName.c_str(), false);
	
	App->scene->materials.push_back(this);
	return true;
}