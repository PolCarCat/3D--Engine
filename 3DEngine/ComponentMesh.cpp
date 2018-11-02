#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "WinObjects.h"
#include "ImporterMesh.h"

ComponentMesh::ComponentMesh(ResMesh* _mesh)
{
	mesh = _mesh;
	transform.position = mesh->boundingBox.CenterPoint();
	type = MESH;
	App->renderer3D->meshes.push_back(this->mesh);
}

ComponentMesh::ComponentMesh()
{
	type = MESH;
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
	mesh->Draw();


	return true;
}

void ComponentMesh::UpdateUI()
{ 
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::NewLine();
		ImGui::Separator();
		if (mesh->name.c_str() != nullptr)
			ImGui::Text("%s", mesh->name.c_str());
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
			ImGui::Text("%d", mesh->num_vertex);
			ImGui::Text("%d", mesh->num_indice);
			ImGui::Text("%d", mesh->num_normals);

		}
		ImGui::Columns(1);

		ImGui::NewLine();

		if (material == nullptr)
		{
			if (ImGui::Button("Add Material"))
				ImGui::OpenPopup("select");

			if (ImGui::BeginPopup("select"))
			{
				UpdateMatWin();
				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::NewLine();
			material->UpdateUI();
		}
		transform.UpdateUI();

		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Delete"))
			Delete();
	}
}

void ComponentMesh::UpdateMatWin()
{
	for (std::list<ComponentMaterial*>::iterator item = App->scene->materials.begin(); item != App->scene->materials.end(); item++)
	{

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((*item)->GetName(), node_flags);
		if (ImGui::IsItemClicked())
		{
			material = (*item);

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::ColorButton("Color##3c", *(ImVec4*)&(*item)->color, 0, ImVec2(80, 80));
			ImGui::EndTooltip();
		}
	}

	if (ImGui::Button("Add Material"))
	{
		App->scene->CreateMaterial();
	}
}


bool ComponentMesh::Save(JSON_Object* json, JsonDoc* doc)
{
	App->loader->meshImporter.SaveMeshAsMeh(mesh);

	return true;
}

bool ComponentMesh::Load(JSON_Object* json, JsonDoc* doc)
{


	return true;
}