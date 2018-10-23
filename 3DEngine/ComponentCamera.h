#pragma once
#include "Component.h"
#include "ComponentTransform.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ResMesh.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(float _near = 0.5f, float _far = 512.0f, float fov = 60.0f);
	~ComponentCamera();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	void FocusMeshes();
	float4x4 ResizePerspMatrix(int width, int heigth);
	void RecalculateFrustrum(int width = 0, int height = 0);
	void CheckInput(float dt);
	bool CheckInside(const ResMesh m);


	float3  Reference;
	ComponentTransform transform;
	bool locked = false;
	bool drawFrustum = true;
	bool isCurCam = false;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	void CalculateViewMatrix();
	float nearDistance = 0.5f;
	float farDistance = 512.0f;
	math::Frustum frustum;
	float aspectRatio = 0;
	float fovy = 60.0f;
	bool lookingAt = false;
};

