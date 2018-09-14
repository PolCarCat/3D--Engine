#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleGui :
	public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};

