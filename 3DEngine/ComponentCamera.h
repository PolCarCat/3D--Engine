#pragma once
#include "Component.h"
#include "ComponentTransform.h"
#include "ResMesh.h"
#include "Primitive.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(float _near = 0.5f, float _far = FPD, float fov = 60.0f);
	~ComponentCamera();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	bool Save(JSON_Object* json, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);


	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	float* GetViewMatrix();
	float* GetPerspMatrix();
	float4x4 GetViewMatrixF();
	float4x4 GetPerspMatrixF();
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

	GameObject* Pick(float3* hit_point);

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	void CalculateViewMatrix();
	float nearDistance = 0.5f;
	float farDistance = FPD;
	math::Frustum frustum;
	float aspectRatio = 0;
	float fovy = 60.0f;
	bool lookingAt = false;
};

