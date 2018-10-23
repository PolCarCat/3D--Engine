#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

ComponentMesh::ComponentMesh(ResMesh _mesh)
{
	mesh = _mesh;
	transform.position = mesh.boundingBox.CenterPoint();
	type = MESH;
	App->renderer3D->meshes.push_back(this);
}


ComponentMesh::~ComponentMesh()
{
}


bool ComponentMesh::Start()
{
	return true;
}

bool ComponentMesh::Update()
{



	return true;
}

void ComponentMesh::UpdateUI()
{ 

	if (ImGui::CollapsingHeader(mesh.name.c_str()));
	{
		ImGui::NewLine();
		ImGui::Separator();
		if (mesh.name.c_str() != nullptr)
			ImGui::Text("%s", mesh.name.c_str());
		else
			ImGui::Text("This Mesh hasn't name");

		ImGui::Separator();
		ImGui::NewLine();

		if (ImGui::Checkbox("Draw Normals", &drawNormals));
		if (ImGui::Checkbox("Draw Bounding Box", &drawBB));

		ImGui::NewLine();

		ImGui::Columns(2);
		{

			ImGui::Text("Num Vertices:");
			ImGui::Text("Num Indices:");
			ImGui::Text("Num Normals:");
			ImGui::NextColumn();
			ImGui::Text("%d", mesh.num_vertex);
			ImGui::Text("%d", mesh.num_indice);
			ImGui::Text("%d", mesh.num_normals);

		}
		ImGui::Columns(1);

		transform.UpdateUI();
	}
}
