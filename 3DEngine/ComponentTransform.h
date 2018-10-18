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

	float3 position;
	float3 scale;
	Quat rotation;

};

