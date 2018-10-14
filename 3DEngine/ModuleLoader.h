#pragma once
#include "Module.h"

#define CHECKERS_SIZE 20


struct Texture;

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

	
	bool LoadScene(const char* path);
	void SetDropFile(char* f);


	uint LoadChekerTex();
	Texture LoadTex(const char* path);
	Format CheckFormat(const char* path);

private:
	char* droppedFile = nullptr;
	Format dFformat = FNULL;

	bool test = false;


};


void LogAssimp(const char* c1, char* c2);
