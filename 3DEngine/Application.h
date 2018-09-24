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

	JsonDoc jsondoc;

	int fps_counter = 0;
	int ms_counter = 0;
	float last_sec_frames = 0;
	float fps[100];
	float ms[100];


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void LoadGame();
	void SaveGame() const;

private:

	Timer	sec_timer;
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
	bool SaveNow() const;

};