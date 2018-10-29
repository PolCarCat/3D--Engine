#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component()
{
}


Component::~Component()
{
}


Type Component::GetType()
{ 
	return type;
}

GameObject* Component::GetParent() 
{ 
	return parent; 
}

void Component::SetActive(bool a)
{ 
	active = a; 
}

bool Component::GetActive() 
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


	//UNFINISHED Set this to parent childs
}

void Component::Delete()
{
	toDelete = true;
}

bool Component::CheckDelete()
{
	return toDelete;
}