#pragma once
#include "Component.h"
#include "ModuleRenderer3D.h"


class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Start();


	bool Update();

	void UpdateUI();

	bool CleanUp();

	Texture tex;
};

