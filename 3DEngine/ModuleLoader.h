#pragma once
#include "Module.h"

#define CHECKERS_SIZE 20



enum Format {
	FBX,
	PNG,
	DDS,
	FNULL
};

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
	Format CheckFormat(const char* path);

	uint Lenna;
private:
	char* droppedFile = nullptr;
	Format dFformat = FNULL;

	bool test = false;


};

