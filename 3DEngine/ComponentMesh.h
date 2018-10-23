#pragma once
#include "Component.h"
#include "ResMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(ResMesh _mesh);
	~ComponentMesh();
	bool Start();
	bool Update();
	void UpdateUI();

	bool draw = true;
	ResMesh mesh;
	ComponentTransform transform;
	ComponentMaterial material;
};

