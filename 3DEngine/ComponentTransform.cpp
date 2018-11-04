#include "ComponentTransform.h"
#include "ImGui/imgui.h"


ComponentTransform::ComponentTransform()
{
	type = TRANSFORM;
	matrix = float4x4::identity;
	position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
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

	bool changed = false;

	if (ImGui::DragFloat3("Positon", (float*)&position, 0.25f, -100, 100))
		changed = true;

	if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25f, -100, 100))
		changed = true;


	float3 rot = rotation.ToEulerXYZ();
	if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.25f, -100, 100))
	{
		rotation = rotation.FromEulerXYZ(rot.x, rot.y, rot.z);
		changed = true;
	}
	

	ImGui::Text("%d %d %d %d", matrix.v[0][0], matrix.v[1][0], matrix.v[2][0], matrix.v[3][0]);
	ImGui::Text("%d %d %d %d", matrix.v[0][1], matrix.v[1][1], matrix.v[2][1], matrix.v[3][1]);
	ImGui::Text("%d %d %d %d", matrix.v[0][2], matrix.v[1][2], matrix.v[2][2], matrix.v[3][2]);
	ImGui::Text("%d %d %d %d", matrix.v[0][3], matrix.v[1][3], matrix.v[2][3], matrix.v[3][3]);

	if (changed)
		CalcMatrix();

}

bool ComponentTransform::CleanUp()
{
	return true;
}


void ComponentTransform::CalcMatrix()
{
	matrix.Set(float4x4::FromTRS(position, rotation, scale));
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

	active = json_object_dotget_boolean(json, "Active");

	JSON_Array* pos = doc->GetObjAr(json, "Position");
	position.x = json_array_get_number(pos, 0);
	position.y = json_array_get_number(pos, 1);
	position.z = json_array_get_number(pos, 2);

	JSON_Array* sc = doc->GetObjAr(json, "Scale");
	scale.x = json_array_get_number(sc, 0);
	scale.y = json_array_get_number(sc, 1);
	scale.z = json_array_get_number(sc, 2);

	JSON_Array* rt = doc->GetObjAr(json, "Rotation");
	rotation.x = json_array_get_number(rt, 0);
	rotation.y = json_array_get_number(rt, 1);
	rotation.z = json_array_get_number(rt, 2);
	rotation.w = json_array_get_number(rt, 3);


	return true;
}