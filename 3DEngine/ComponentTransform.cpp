#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform()
{
	type = TRANSFORM;
	localMartix = float4x4::identity;
	globalMartix = float4x4::identity;
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

	if (ImGui::DragFloat3("Positon", (float*)&position, 0.25f))
		changed = true;

	if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25f, -100, 100))
		changed = true;


	float3 rot = rotation.ToEulerXYZ() * RADTODEG;
	if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.25f))
	{
		rot *= DEGTORAD;
		rotation = rotation.FromEulerXYZ(rot.x , rot.y , rot.z);
		changed = true;
	}
	

	if (changed)
	{
		CalcMatrix();
		parent->CalcGlobalTransform();
	}


}

bool ComponentTransform::CleanUp()
{
	parent = nullptr;
	return true;
}


void ComponentTransform::CalcMatrix()
{
	localMartix.Set(float4x4::FromTRS(position, rotation, scale));
}

void ComponentTransform::CalcVectors()
{
	localMartix.Decompose(position, rotation, scale);
}

bool ComponentTransform::Save(JSON_Object* json, JsonDoc* doc)
{
	json_object_dotset_number(json, "Type", type);
	json_object_dotset_boolean(json, "Active", active);

	JSON_Array* local = doc->SetArray(json, "Local Matrix");

	for (uint columns = 0; columns < 4; columns++)
	{
		for (uint rows = 0; rows < 4; rows++)
		{
			json_array_append_number(local, localMartix.At(rows, columns));
		}

	}

	return true;
}

bool ComponentTransform::Load(JSON_Object* json, JsonDoc* doc)
{

	active = json_object_dotget_boolean(json, "Active");

	JSON_Array* local = doc->GetObjAr(json, "Local Matrix");
	uint i = 0;
	for (uint columns = 0; columns < 4; columns++)
	{
		for (uint rows = 0; rows < 4; rows++)
		{
			localMartix.At(rows, columns) = json_array_get_number(local, i++);

		}

	}

	localMartix.Decompose(position, rotation, scale);
	return true;

}