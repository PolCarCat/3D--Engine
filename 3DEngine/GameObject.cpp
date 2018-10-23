#include "GameObject.h"
#include "Component.h"
#include "Globals.h"
#include "ResMesh.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"

GameObject::GameObject()
{
	transform = new ComponentTransform();
	transform->SetParent(this);
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

	return true;
}

void GameObject::UpdateUI()
{

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