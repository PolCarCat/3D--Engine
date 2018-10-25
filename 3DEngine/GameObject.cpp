#include "GameObject.h"
#include "Component.h"
#include "Globals.h"
#include "ResMesh.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Application.h"

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

void GameObject::UpdateUI()
{
	bool s = staticobj;
	ImGui::Checkbox("Set static", &staticobj);
	if (s != staticobj)
		SetStatic(staticobj);

	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
		(*item)->UpdateUI();
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
			(*item)->UpdateUI();
	}

}

bool GameObject::CleanUp()
{
	for (std::vector<GameObject*>::iterator item = objChilds.begin(); item != objChilds.end(); item++)
	{
			(*item)->CleanUp();
	}

	for (std::vector<Component*>::iterator item = compChilds.begin(); item != compChilds.end(); item++)
	{
			(*item)->CleanUp();
	}
	return true;
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

	if (parent != nullptr)
	{
		Utils::RemoveFromVector(this, parent->objChilds);
	}

	parent = p;
	parent->objChilds.push_back(this);


}

void GameObject::AddCompMesh(ResMesh mesh)
{
	Component* newcomp = new ComponentMesh(mesh);
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