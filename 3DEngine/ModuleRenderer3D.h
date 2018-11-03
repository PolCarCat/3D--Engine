#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"
#include "SDL/include/SDL.h"
#include "ResTexture.h"


#include <list>
#include <vector>


#define MAX_LIGHTS 8


class ComponentMesh;


class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	bool Load(json_object_t* doc);
	bool Save(json_object_t* doc);

	void DrawMeshes();
	void SetUpMat(ComponentMaterial* mat);
	void CleanUpMeshes();
	void SaveMeshes();
	float3 GetMeshesCenter();
	AABB GetMeshesAABB();
	void DrawFrustum(math::Frustum f);
	void DrawAABB(math::AABB a, Color c = { 0.2f, 0.2f, 1.0f, 1.0f });

	void OnResize(int width, int height);
	void EnableVsync(bool enable);
	void EnableDepthTest(bool enable);
	void EnableCullFace(bool enable);
	void EnableLighting(bool enable);
	void EnableColorMaterial(bool enable);
	void EnableTexture2D(bool enable);
	void EnableWireframe(bool enable);
	bool GetVsync() { return vsync; };
	bool GetDepthTest() { return depthTest; };
	bool GetCullFace() { return cullface; };
	bool GetLighting() { return lightning; };
	bool GetColorMat() { return colorMaterial; };
	bool GetTexture2D() { return texture2D; };
	bool GetWireFrame() { return wireframe; };


public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;


	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	std::list<ResMesh*> meshes;

public:
	bool drawCube = false;
	bool drawLine = false;
	bool drawArrow = false;
	bool drawPlane = true;
	bool drawSphere = false;
	bool drawNormals = false;
	bool drawBBox = false;
	bool drawAxis = true;
	bool drawCylinder = false;
	bool drawCapsule = false;
	float xx = 0;
	float yy = 0;
	float zz = 0;
private:
	bool vsync = false;
	bool depthTest = true;
	bool cullface = true;
	bool lightning = false;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;
	bool useGhostCam = true;

	PLine		line;
	PAxis		axis;
	PCube		cube;
	PPlane		plane;
	PArrow		arrow;
	PSphere		sphere;
	PCapsule	capsule;
	PCylinder	cylinder;
};