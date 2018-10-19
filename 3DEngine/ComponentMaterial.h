#pragma once
#include "Component.h"
#include "ResTexture.h"


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

	ResTexture tex;
};

