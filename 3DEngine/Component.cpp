#include "Component.h"
#include "GameObject.h"

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
	parent = p;

	//UNFINISHED Set this to parent childs
}