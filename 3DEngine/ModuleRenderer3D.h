#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "glmath.h"
#include "Primitive.h"
#include "SDL/include/SDL.h"
#include "ResTexture.h"


#include <list>
#include <vector>


#define MAX_LIGHTS 8

class ResMesh;


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
	void CleanUpMeshes();
	void SetMeshesTex(ResTexture i);
	float3 GetMeshesCenter();
	AABB GetMeshesAABB();

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


	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
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
private:
	bool vsync = false;
	bool depthTest = true;
	bool cullface = true;
	bool lightning = false;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;

	PLine		line;
	PAxis		axis;
	PCube		cube;
	PPlane		plane;
	PArrow		arrow;
	PSphere		sphere;
	PCapsule	capsule;
	PCylinder	cylinder;
};