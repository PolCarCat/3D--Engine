#pragma once
#include "Component.h"
#include "ResMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

class ComponentMesh :
	public Component
{
public:
	ComponentMesh();
	~ComponentMesh();
	bool Start();
	bool Update();

	ResMesh mesh;
	ComponentTransform transform;
	ComponentMaterial material;
};

