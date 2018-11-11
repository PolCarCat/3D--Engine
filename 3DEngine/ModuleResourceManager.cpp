#include "ModuleResourceManager.h"
#include "ResMesh.h"
#include "ResTexture.h"
#include "pcg-c-basic-0.9/pcg_basic.h"



ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Resource Manager";

}


ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{

	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	std::map<uint32_t, Resource*>::iterator it = resources.begin();
	for (it; it != resources.end(); it++)
	{

	}
	
	return true;
}

uint32_t ModuleResourceManager::FindFile(const char * file) const
{
	//std::map<uint32_t, Resource*>::iterator it = resources.begin();
	//for (it; it != resources.end(); it++)
	//{

	//}

	return uint32_t();
}

uint32_t ModuleResourceManager::ImportFile(const char * file)
{

	
	return uint32_t();
}

Resource * ModuleResourceManager::GetResource(uint32_t uid) const
{
	//Should check if exists
	//std::map<uint32_t, Resource*>::iterator it = resources.find(uid);
	return resources.find(uid)->second;
}

Resource * ModuleResourceManager::CreateNewResource(ResType t, uint32_t id)
{
	Resource* res = nullptr;
	uint32_t id = pcg32_random();

	switch (t)
	{
	case RESMESH:
		res = new ResMesh(id);
		break;
	case RESTEXTURE:
		res = new ResTexture(id);
		break;
	}

	return res;
}
