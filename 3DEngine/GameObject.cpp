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
	return true;
}

bool GameObject::Update()
{
	
	return true;
}

void GameObject::UpdateUI()
{

}

bool GameObject::CleanUp()
{
	return true;
}


void GameObject::AddComponent(Compontent* comp)
{
	compChilds.push_back(comp);
}