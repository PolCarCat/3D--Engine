#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"
#include "SDL/include/SDL.h"
#include "ResTexture.h"
#include "ComponentMesh.h"

#include <queue>


#include <list>
#include <vector>


#define MAX_LIGHTS 8


//class ComponentMesh;



struct MeshPriority
{
	bool operator()(const ComponentMesh* mesh1, const ComponentMesh* mesh2)const
	{
		bool ret = false;
		if (mesh1->material->GetTexture() != nullptr && mesh1->material->GetTexture()->transparent)
		{
			ret = mesh1->DistanceToCamera() < mesh2->DistanceToCamera();
		}
		else
		{
			ret = mesh1->DistanceToCamera() > mesh2->DistanceToCamera();
		}

		return ret;
	}
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


	void SetUpMat(ComponentMaterial* mat);
	void DrawFrustum(math::Frustum f);
	void DrawAABB(math::AABB a, Color c = { 0.2f, 0.2f, 1.0f, 1.0f });
	void DrawOBB(math::OBB a, Color c = { 0.2f, 0.2f, 1.0f, 1.0f });
	void DrawSphere(math::Sphere sphere, Color c = { 0.2f, 0.2f, 1.0f, 1.0f });

	void SetUseGhostCam(bool s);
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
	bool IsUsingGhostCam() const;

	//Draw Meshes
	void ToDraw(ComponentMesh* entity);
	void DrawQueue(std::priority_queue<ComponentMesh*, std::vector<ComponentMesh*>, MeshPriority>& queue);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;


	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

public:

	bool drawPlane = true;

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

	PPlane		plane;

	std::priority_queue<ComponentMesh*, std::vector<ComponentMesh*>, MeshPriority> transparentMeshes;
	std::priority_queue<ComponentMesh*, std::vector<ComponentMesh*>, MeshPriority> opaqueMeshes;

};

#endif //__MODULERENDERER3D_H__