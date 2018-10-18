#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	type = TRANSFORM;
}


ComponentTransform::~ComponentTransform()
{
}


bool ComponentTransform::Start()
{
	return true;
}

bool ComponentTransform::Update()
{
	return true;
}

void ComponentTransform::UpdateUI()
{
}

bool ComponentTransform::CleanUp()
{
	return true;
}