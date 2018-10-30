#include "GameObject.h"
#include "Component.h"
#include "Globals.h"
#include "ResMesh.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "QuadTree.h"

GameObject::GameObject()
{
	transform = new ComponentTransform();
	transform->SetParent(this);
	bBox.minPoint = { 0, 0, 0 };
	bBox.maxPoint = { 0, 0, 0 };
}


GameObject::~GameObject()
{
}


bool GameObject::Start()
{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
			(*item)->Start();
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
			(*item)->Start();
	}
	return true;
}

bool GameObject::Update()
{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
		if ((*item)->GetActive())
			(*item)->Update();
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
		if ((*item)->GetActive())
		(*item)->Update();
	}
	//App->renderer3D->DrawAABB(bBox);
	
	return true;
}

bool GameObject::PostUpdate()


{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end();)
	{
		
		if ((*item)->CheckDelete())
		{
			(*item)->CleanUp();
			item = objChilds.erase(item);
			//delete *item;
		}
		else
		{
			(*item)->PostUpdate();
			item++;
		}

	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end();)
	{
		if ((*item)->toDelete)
		{
			(*item)->CleanUp();
			item = compChilds.erase(item);
			//delete *item;
		}
		else
			item++;
		
	}

	return true;
}

void GameObject::UpdateUI()
{

	char n[50];
	strcpy_s(n, 50, name.c_str());
	ImGui::InputText("", n, 50);
		name = n;


	bool a = active;
	ImGui::Checkbox("Active", &active);
	if (a != active)
		SetChildsActive(active);

	ImGui::SameLine();

	bool s = staticobj;
	ImGui::Checkbox("Static", &staticobj);
	if (s != staticobj)
		SetStatic(staticobj);


	ImGui::NewLine();
	ImGui::Text("Objects");
	ImGui::Separator();
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
		if (ImGui::CollapsingHeader((*item)->GetName()))
			(*item)->UpdateUI();
	}

	ImGui::NewLine();
	ImGui::Text("Components");
	ImGui::Separator();
	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
			(*item)->UpdateUI();

	}

	if (ImGui::BeginMenu("Add Component"))
	{
		UpdateNewComWindow();
		ImGui::EndMenu();
	}

	ImGui::NewLine();
	ImGui::Separator();
	if (ImGui::Button("Delete"))
		Delete();
}

bool GameObject::CleanUp()
{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
			(*item)->CleanUp();
			if (*item != nullptr)
			{
				delete *item;
				*item = nullptr;
			}
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
			(*item)->CleanUp();
	}
	return true;
}

void GameObject::Delete()
{
	toDelete = true;
}

bool GameObject::CheckDelete()
{
	return toDelete;
}

bool GameObject::GetActive()
{
	return active;
}

void GameObject::SetActive(bool b)
{
	active = b;
}

const char* GameObject::GetName()
{
	return name.c_str();
}

void GameObject::SetName(const char* n)
{
	name = n;
}

void GameObject::SetName(std::string n)
{
	name = n;
}


void GameObject::AddComponent(Component* comp)
{
	GameObject* prevparent = comp->GetParent();
	if (prevparent != this)
	{
		if (prevparent != nullptr)
		{
			Utils::RemoveFromVector(comp, prevparent->compChilds);		
		}

		compChilds.push_back(comp);
		comp->SetParent(this);
	}

}

void GameObject::AddGameObject(GameObject* obj)
{
	GameObject* prevparent = obj->GetParent();
	if (prevparent != this)
	{
		if (prevparent != nullptr)
		{
			Utils::RemoveFromVector(obj, prevparent->objChilds);
		}
		objChilds.push_back(obj);
		obj->SetParent(this);
	}
}

GameObject* GameObject::GetParent()
{
	return parent;
}


void GameObject::SetParent(GameObject* p)
{
	if (!CheckIfContained(p))
	{
		if (parent != nullptr)
		{
			Utils::RemoveFromVector(this, parent->objChilds);
		}

		parent = p;
		parent->objChilds.push_back(this);
	}
	else
	{
		VSLOG("Cannot set a child as parent\n");
	}

}

bool GameObject::CheckIfContained(GameObject* obj)
{
	//Check if the element is in the list or the list of the children

	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
		if ((*item) == obj || (*item)->CheckIfContained(obj))
			return true;
	}
	return false;
}

void GameObject::SetChildsActive(bool a)
{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
		(*item)->SetActive(a);
		(*item)->SetChildsActive(a);
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
		(*item)->SetActive(a);
	}
}

void GameObject::DeleteComp(Component* comp)
{
	Utils::RemoveFromVector(comp, compChilds);
	comp->CleanUp();
	delete comp;
}

void GameObject::DeleteGameObj(GameObject* obj)
{

}

void GameObject::AddCompMesh()
{
	Component* newcomp = new ComponentMesh();
	newcomp->SetParent(this);
}

void GameObject::AddCompMesh(ResMesh* m)
{
	Component* newcomp = new ComponentMesh(*m);
	newcomp->SetParent(this);
}

void GameObject::AddCompCam(float _near , float _far, float fov)
{
	Component* newcomp = new ComponentCamera(_near, _far, fov);
	newcomp->SetParent(this);
}

void GameObject::AddBox(AABB b)
{
	if (bBox.minPoint.Equals(0, 0, 0) && bBox.maxPoint.Equals(0, 0, 0))
		bBox = b;
	else
		bBox.Enclose(b);
}

bool GameObject::GetStatic()
{
	return staticobj;
}
void GameObject::SetStatic(bool b)
{
	staticobj = b;

	if (staticobj)
		App->scene->quadTree.AddObject(this);

	else
		App->scene->quadTree.RemoveObject(this);
}

AABB GameObject::GetBB()
{
	return bBox;
}


void GameObject::UpdateNewComWindow()
{
	ImGui::Separator();

	ImGuiTreeNodeFlags node_flags =  ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

	Type t = Type::NULLCOMP;
	bool add = false;
	ImGui::TreeNodeEx("Camera", node_flags);

	if (ImGui::IsItemClicked())
	{
		t = CAMERA;
		add = true;
	}
	
	ImGui::TreeNodeEx("Mesh", node_flags);
	if (ImGui::IsItemClicked())
	{
		t = MESH;
		add = true;
	}

	if (add)
		AddComponent(t);
}

void GameObject::AddComponent(Type t)
{
	switch (t)
	{
	case MESH:
		AddCompMesh();
		break;
	case CAMERA:
		AddCompCam();
		break;
	default:
		break;
	}
}