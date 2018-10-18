#include "ComponentMaterial.h"
#include "ModuleRenderer3D.h"


ComponentMaterial::ComponentMaterial()
{
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}


bool ComponentMaterial::Start()
{
	return true;
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::UpdateUI()
{
}

bool ComponentMaterial::CleanUp()
{
	return true;
}