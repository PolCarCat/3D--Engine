#pragma once
#include "Module.h"

#define CHECKERS_SIZE 20



class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);

	~ModuleLoader();

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	bool CleanUp();

	bool CheckDropEvent();
	
	void LoadScene(const char* path);

	uint LoadChekerTex();
	uint LoadTex(const char* path);

	uint Lenna;
private:
	char* droppedFile = nullptr;


	bool test = false;


};

