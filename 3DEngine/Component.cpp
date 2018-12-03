#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component()
{
}


Component::~Component()
{
}


Type Component::GetType() const
{ 
	return type;
}

GameObject* Component::GetParent() const
{ 
	return parent; 
}

void Component::SetActive(bool a)
{ 
	active = a; 
}

bool Component::GetActive() const
{ 
	return active; 
}

void Component::SetParent(GameObject* p)
{
	if (parent != nullptr)
	{
		Utils::RemoveFromVector(this, parent->compChilds);
	}

	parent = p;
	parent->compChilds.push_back(this);


}

void Component::ForceParent(GameObject * p)
{
	parent = p;
}

void Component::Delete()
{
	toDelete = true;
}

bool Component::CheckDelete() const
{
	return toDelete;
}