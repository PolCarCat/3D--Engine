#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Start()
{
	return true;
}

update_status ModuleTimeManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}