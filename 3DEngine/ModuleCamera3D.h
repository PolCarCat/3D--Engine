#pragma once
#include "Module.h"
#include "Globals.h"
#include "ComponentTransform.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	void FocusMeshes();
	float4x4 ResizePerspMatrix(int width, int heigth);
	void RecalculateFrustrum(int width, int height);

private:

	void CalculateViewMatrix();
	float nearDistance = 0.5f;
	float farDistance = 512.0f;
	float fovy = 60.0f;

public:
	
	float3 X, Y, Z, Position, Reference;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	math::Frustum frustum;
	float aspectRatio = 0;

};