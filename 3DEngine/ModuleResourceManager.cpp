#include "Application.h"
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

	for (auto it = resources.begin(); it != resources.end(); it++) 
	{
		if (std::string(it->second->GetName()) == file)
			return it->first;
	}

	return 0;
}

uint32_t ModuleResourceManager::ImportFile(const char * file)
{
	
	Format f = App->loader->CheckFormat(file);

	switch (f)
	{
	case FBX:
		App->loader->meshImporter.LoadScene(file);
		break;
	case PNG:
	case DDS:
		App->loader->texImporter.LoadTex(file);
		break;
	case MEH:
		App->loader->meshImporter.LoadMeh(file, true);
		break;
	case JSON:
		break;
	case FNULL:
		break;
	}

	return 0;
}

Resource * ModuleResourceManager::GetResource(uint32_t uid) const
{
	//Check if exists
	Resource* ret = nullptr;
	if (resources.find(uid) != resources.end())
		ret = resources.find(uid)->second;
	else
		VSLOG("\nCannot find resource with uuid %d", uid);


	return ret;
}

Resource * ModuleResourceManager::GetResourceByName(const char * name) const
{

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if (std::string(it->second->GetName()) == name)
			return it->second;
	}

	return nullptr;
}

Resource * ModuleResourceManager::GetResourceByName(std::string name) const
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if (it->second->GetName() == name)
			return it->second;
	}

	return nullptr;
}

Resource * ModuleResourceManager::CreateNewResource(ResType t, uint32_t id)
{
	Resource* res = nullptr;
	uint32_t newid = id;

	if (id == 0)
		newid = pcg32_random();

	switch (t)
	{
	case RESMESH:
		res = new ResMesh(newid);
		break;
	case RESTEXTURE:
		res = new ResTexture(newid);
		break;
	}

	AddResource(res);
	return res;
}

void ModuleResourceManager::AddResource(Resource * res)
{
	if (res == nullptr)
		return;

	if (resources[res->GetUUID()] != nullptr)
		VSLOG("\nMemory overriden in resource map");

	resources[res->GetUUID()] = res;
}
