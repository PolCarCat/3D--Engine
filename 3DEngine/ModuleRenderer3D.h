#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "glmath.h"
#include "Primitive.h"

#include <list>
#include <vector>


#define MAX_LIGHTS 8



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

	uint tex = 0;

	void GenerateBuffer();
	void Draw();
	void DrawNormals();
	void SetText(const uint i) { tex = i; };

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
	bool drawAxis = true;

private:
	void VertexArraysCube(float origin, float size);
	void IndicesCube(float origin, float size);
	void Ray(float ox, float oy, float oz, float ex, float ey, float ez);
	void Arrow(float ox, float oy, float oz, float ex, float ey, float ez);
	void Sphere(float radius, int stacks, int sectors, vector<float> origin = { 0, 0, 0 });

private:
	bool vsync = false;
	bool depthTest = true;
	bool cullface = true;
	bool lightning = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;
	float vertices[107];
	float vertices2[23];
	bool VBufferInit = false;
	bool IBufferInit = false;
	bool SBufferInit = false;
	uint my_id = 0;
	uint my_Sid = 0;
	uint my_indices = 0;
	vector<float> Svertices;
	vector<int> Sindices;
	vector<int> Cindices = { 0,1,2,	1,3,2,
		3,4,2,	4,5,2,
		2,5,6,	0,2,6,
		6,5,4,	4,7,6,
		7,4,3,	3,1,7,
		6,7,1,	1,0,6 };
	PCube	cube;
	PPlane	plane;
	PAxis	axis;
	PLine	line;
};