#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

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

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	bool vsync = false;
	//mat3x3 NormalMatrix;
	//mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};