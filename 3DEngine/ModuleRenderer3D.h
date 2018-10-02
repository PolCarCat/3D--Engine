#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "glmath.h"

#define MAX_LIGHTS 8

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

public:
	bool directCube = false;
	bool varrCube = false;
	bool indCube = false;
	bool ray = false;
	bool arrow = false;
	bool plane = false;

private:
	void DirectCube(float origin, float size);
	void VertexArraysCube(float origin, float size);
	void IndicesCube(float origin, float size);
	void Ray(float ox, float oy, float oz, float ex, float ey, float ez);
	void Arrow(float ox, float oy, float oz, float ex, float ey, float ez);
	void Plane(float size);

private:
	bool vsync = false;
	bool depthTest = true;
	bool cullface = true;
	bool lightning = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;
	float vertices[108];
	float vertices2[24];
	bool VBufferInit = false;
	bool IBufferInit = false;
	uint my_id = 0;
	uint my_indices = 0;
	float indices[36] = { 0,1,2,	1,3,2,
		3,4,2,	4,5,2,
		2,5,6,	0,2,6,
		6,5,4,	4,7,6,
		7,4,3,	3,1,7,
		6,7,1,	1,0,6 };
};