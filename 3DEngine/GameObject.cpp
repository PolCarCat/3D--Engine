#include "GameObject.h"
#include "Component.h"


GameObject::GameObject()
{
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

void GameObject::AddComponent(Component* comp)
{
	if (comp->GetParent() != this)
	{
		compChilds.push_back(comp);
		comp->SetParent(this);
	}

}