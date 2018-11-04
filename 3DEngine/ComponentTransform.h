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
	bool Save(JSON_Object* json, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);
	void CalcMatrix();


	float3 position = { 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
	Quat rotation = { 0, 0, 0, 0 };

	float4x4 localMartix;
	float4x4 globalMartix;

};

