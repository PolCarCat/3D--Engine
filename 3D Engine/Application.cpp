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
	list<Module*>::iterator item;

	for (item = list_modules.end(); item != list_modules.begin(); --item)
		delete *item;
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list<Module*>::iterator item;

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
		ret = (*item)->Init();

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
		ret = (*item)->Init();
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
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
	
	list<Module*>::iterator item;
	
	for (item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
		ret = (*item)->PreUpdate(dt);

	for (item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
		ret = (*item)->Update(dt);

	for (item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
		ret = (*item)->PostUpdate(dt);

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::iterator item;

	for (item = list_modules.end(); item != list_modules.begin() && ret; --item)
		ret = (*item)->CleanUp();
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}