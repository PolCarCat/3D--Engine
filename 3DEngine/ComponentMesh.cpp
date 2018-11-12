#include "ComponentMesh.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "WinObjects.h"
#include "ImporterMesh.h"
#include "GameObject.h"
#include "ResMesh.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

ComponentMesh::ComponentMesh(ResMesh* _mesh)
{
	mesh = _mesh;
	type = MESH;
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
	if (material != nullptr)
	App->renderer3D->SetUpMat(material);

	glPushMatrix();
	glMultMatrixf(parent->transform->globalMartix.Transposed().ptr());
	
	mesh->Draw();

	if (drawNormals)
		mesh->DrawNormals();
	
	glPopMatrix();

	if (drawBB)
	{
		App->renderer3D->DrawAABB(parent->GetGlobalABB());
		App->renderer3D->DrawOBB(parent->GetOBB(), { 1.0f, 0.0f, 1.0f, 1.0f });
	}


	return true;
}

void ComponentMesh::UpdateUI()
{ 
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::NewLine();
		ImGui::Separator();
		if (mesh->GetName() != nullptr)
			ImGui::Text("%s", mesh->GetName());
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
			ImGui::Text("%d", mesh->id_vertex);
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
	
	//transform.Save(json, doc);
	json_object_dotset_string(json, "Mesh Name", mesh->GetName());
	json_object_dotset_number(json, "Mesh UUID", mesh->GetUUID());

	if (material != nullptr)
	{
		JSON_Object* mat = doc->SetObj(json, "Material");

		material->Save(mat, doc);
	}


	return true;
}

bool ComponentMesh::Load(JSON_Object* json, JsonDoc* doc)
{

	uint32_t meshuuid = json_object_dotget_number(json, "Mesh UUID");
	std::string name = json_object_dotget_string(json, "Mesh Name");



	JSON_Object* mat = doc->GetObjObj(json, "Material");
	
	if (mat != nullptr)
	{
		std::string matName = json_object_dotget_string(mat, "Name");
		ComponentMaterial* usedMat = App->scene->CheckMaterial(matName.c_str());

		if (usedMat == nullptr)
		{
			material = new ComponentMaterial();
			material->Load(mat, doc);
		}
		else
			material = usedMat;
	}


	Resource* m  = App->resourceManager->GetResource(meshuuid);

	if (m == nullptr)
	{
		ResMesh* m = App->loader->meshImporter.LoadMeh(name.c_str());
	
		
		if (m != nullptr)
		{
			mesh = m;
			App->resourceManager->AddResource(m);
		}
		else
		{
			VSLOG("Cannot load mesh %s", name.c_str());
		}


	}
	else if (m->GetType() == RESMESH)
	{
		VSLOG("\nUsing repeated mesh with id %d", (int)m->GetUUID())
		mesh = (ResMesh*)m;
	}
	else
		VSLOG("Found resource that should be mesh but it isn't");
	

	parent->transform->CalcMatrix();
	return true;
}