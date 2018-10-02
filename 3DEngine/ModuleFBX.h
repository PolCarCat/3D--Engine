#pragma once
#include "Module.h"
class ModuleFBX : public Module
{
public:
	ModuleFBX(Application* app, bool start_enabled = true);

	~ModuleFBX();

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	bool CleanUp();

	void CheckDropEvent();

private:
	char* droppedFile;



};

