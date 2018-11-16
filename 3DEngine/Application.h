#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleGui.h"
#include "ModuleLoader.h"
#include "ModuleTimeManager.h"
#include "ModuleResourceManager.h"
#include "JsonDoc.h"
#include "FileSystem.h"
#include <list>

using namespace std;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleScene* scene = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleGui* imgui = nullptr;
	ModuleLoader* loader = nullptr;
	ModuleTimeManager* timeManager = nullptr;
	ModuleResourceManager* resourceManager = nullptr;
	JsonDoc config;
	FileSystem fileSystem;

	int fps_counter = 0;
	int ms_counter = 0;
	float fps[100];
	float ms[100];
	float expected_delay;
	int framerate_cap = 60;
	bool logAvaliable = false;
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void LoadGame();
	void SaveGame() const;


private:

	Timer	ms_timer;
	float	dt;
	pcg32_random_t rng;
	list<Module*> list_modules;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	bool				want_to_reload = false;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void ReadFps();
	void ReadMs();
	bool LoadNow();
	bool SaveNow();

};

extern Application* App;

#endif //__APLICATION_H__