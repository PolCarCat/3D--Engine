#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	bool Start();

	bool Update();

	void UpdateUI();

	bool CleanUp();

	float3 position = { 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
	Quat rotation = { 0, 0, 0, 0 };

};

