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
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "Transform");

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