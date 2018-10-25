#pragma once
#include "Module.h"
#include "ImporterTexture.h"

#define INDICES_CAP 200000

class GameObject;

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

	
	GameObject* LoadScene(const char* path);
	void SetDropFile(char* f);



	Format CheckFormat(const char* path);
	std::string GetFileName(const char* path);

private:
	char* droppedFile = nullptr;
	Format dFformat = FNULL;

	ImporterTexture texImporter;


};


void LogAssimp(const char* c1, char* c2);
