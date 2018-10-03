#pragma once
#include "Module.h"

class aiScene;
class Mesh;

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

	bool CheckDropEvent();
	
	void LoadScene(const char* path);
private:
	char* droppedFile = nullptr;

	Mesh* testScene = nullptr;
	bool test = false;


};

