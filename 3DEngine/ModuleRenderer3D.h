#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "glmath.h"
#include "Primitive.h"

#include <list>
#include <vector>


#define MAX_LIGHTS 8


struct Texture
{
	uint id = 0;
	uint width = 0;
	uint heigth = 0;

	void CleanUp();
};

struct Mesh
{

	uint id_indice = 0; // index in VRAM
	uint num_indice = 0;
	uint* indice = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint num_normals = 0;
	float* normals = nullptr;

	uint id_colors = 0;
	uint num_colors = 0;
	float* colors = nullptr;

	uint id_textC = 0;
	uint num_textC = 0;
	float* textC = nullptr;

	Texture tex;
	char* name;

	AABB boundingBox;

	float3 position = { 0,0,0 };
	float3 scale = { 0,0,0 };
	Quat rotation = { 0,0,0,0 };
	
	

	void GenerateBuffer();
	void Draw();
	void DrawNormals();
	void DrawBoundingBox();
	void CleanUp();

};



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
	void SetMeshesTex(Texture i);
	float3 GetMeshesCenter();

	void OnResize(int width, int height);
	void EnableVsync();
	void EnableDepthTest();
	void EnableCullFace();
	void EnableLighting();
	void EnableColorMaterial();
	void EnableTexture2D();
	void EnableWireframe();
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
	std::list<Mesh*> meshes;

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