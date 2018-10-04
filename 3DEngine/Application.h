#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGui.h"
#include "ModuleFBX.h"
#include "JsonDoc.h"
#include <list>

using namespace std;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGui* imgui;
	ModuleFBX* fbx;

	JsonDoc config;

	int fps_counter = 0;
	int ms_counter = 0;
	float fps[100];
	float ms[100];
	float expected_delay;
	int framerate_cap = 60;

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