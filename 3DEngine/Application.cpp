#include "Application.h"
#include "Parson/parson.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleGui(this);
	loader = new ModuleLoader(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(imgui);
	AddModule(loader);
	
	// Scenes
	AddModule(scene);

	// Renderer last!
	AddModule(renderer3D);

	//JsonDoc
	config.Init("config.json");
	LoadGame();

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

	ms_timer.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	static int speed = 0;
	if (++speed > 20)
	{
		speed = 0;
		ReadFps();
		ReadMs();
	}

	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (want_to_save == true)
		SaveNow();

	if (want_to_load == true)
		LoadNow();

	if (framerate_cap > 0)
		expected_delay = 1000 / (float)framerate_cap - (float)ms_timer.Read() / 1000.0f;
	else
		expected_delay = 0;


	if (expected_delay > 0)
		SDL_Delay(expected_delay);
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
		fps[fps_counter] = 1 / ((float)ms_timer.Read() / 1000);

		fps_counter++;

		if (fps_counter >= 100)
			fps_counter = 0;
}

void Application::ReadMs()
{
	ms[ms_counter] = (float)ms_timer.Read();

	ms_counter++;

	if (ms_counter >= 100)
		ms_counter = 0;
}

// Load / Save
void Application::LoadGame()
{
	want_to_load = true;
}

// ---------------------------------------
void Application::SaveGame() const
{
	want_to_save = true;
}

// ---------------------------------------


bool Application::LoadNow()
{
	bool ret = false;


	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
		(*it)->Load(config.GetObj((*it)->GetName()));

	want_to_load = false;
	return ret;
}

bool Application::SaveNow()
{
	bool ret = true;

	for (list<Module*>::const_iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		(*it)->Save(config.GetRootObj());
	}


	config.Save();

	want_to_save = false;
	return ret;
}