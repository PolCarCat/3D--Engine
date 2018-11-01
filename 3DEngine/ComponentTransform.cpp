#include "ComponentTransform.h"
#include "ImGui/imgui.h"


ComponentTransform::ComponentTransform()
{
	type = TRANSFORM;
}


ComponentTransform::~ComponentTransform()
{
}


bool ComponentTransform::Start()
{
	return true;
}

bool ComponentTransform::Update()
{
	return true;
}

void ComponentTransform::UpdateUI()
{
	ImGui::NewLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "Transformation");

	ImGui::Columns(2);
	{

		ImGui::Text("Position:");
		ImGui::Text("Scale");
		ImGui::Text("Rotation:");
		float x = 0;
		float y = 0;
		float z = 0;
		rotation.FromEulerXYZ(x, y, z);
		ImGui::NextColumn();
		ImGui::Text("%.2f %.2f  %.2f", position.x, position.y, position.z);
		ImGui::Text("%d  %d  %d", scale.x, scale.y, scale.z);
		ImGui::Text("%d  %d  %d", x, y, z);

	}
	ImGui::Columns(1);
	
}

bool ComponentTransform::CleanUp()
{
	return true;
}

bool ComponentTransform::Save(JSON_Object* json, JsonDoc* doc)
{
	json_object_dotset_number(json, "Type", type);
	json_object_dotset_boolean(json, "Active", active);

	JSON_Array* pos = doc->SetArray(json, "Position");
	json_array_append_number(pos, position.x);
	json_array_append_number(pos, position.y);
	json_array_append_number(pos, position.z);

	JSON_Array* sc = doc->SetArray(json, "Scale");
	json_array_append_number(sc, scale.x);
	json_array_append_number(sc, scale.y);
	json_array_append_number(sc, scale.z);

	JSON_Array* rt = doc->SetArray(json, "Rotation");
	json_array_append_number(rt, rotation.x);
	json_array_append_number(rt, rotation.y);
	json_array_append_number(rt, rotation.z);
	json_array_append_number(rt, rotation.w);
	return true;
}

bool ComponentTransform::Load(JSON_Object* json, JsonDoc* doc)
{

	return true;
}