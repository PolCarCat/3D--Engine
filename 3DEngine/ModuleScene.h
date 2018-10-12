#pragma once
#include "Module.h"
#include "ModuleRenderer3D.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	Texture currentTex;

};
