#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imgui = new ModuleGui(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(imgui);
	
	// Scenes
	AddModule(scene);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (list<Module*>::reverse_iterator it = list_modules.rbegin(); !list_modules.empty() && it != list_modules.rend(); ++it)
		delete (*it);

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret; ++it)
		ret = (*it)->Init();

	// After all Init calls we call Start() in all modules

	VSLOG("Application Start --------------");
	
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret; ++it)
		ret = (*it)->Start();

	sec_timer.Start();
	ms_timer.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	ReadFps();
	ReadMs();

	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate(dt);

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update(dt);

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate(dt);

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = list_modules.rbegin(); !list_modules.empty() && it != list_modules.rend(); ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::ReadFps()
{
	if (sec_timer.Read() < 1000)
		last_sec_frames++;
	else
	{
		fps[fps_counter] = last_sec_frames;
		last_sec_frames = 0;
		fps_counter++;

		if (fps_counter >= 100)
			fps_counter = 0;

		sec_timer.Start();
	}
}

void Application::ReadMs()
{
	ms[ms_counter] = (float)ms_timer.Read();

	ms_counter++;

	if (ms_counter >= 100)
		ms_counter = 0;
}