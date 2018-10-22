#pragma once
#include "Component.h"
#include "ComponentTransform.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(float _near = 0.5f, float _far = 512.0f, float fov = 60.0f);
	~ComponentCamera();

	bool Start();
	bool Update();
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
	ComponentTransform transform;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	math::Frustum frustum;
	float aspectRatio = 0;
};

