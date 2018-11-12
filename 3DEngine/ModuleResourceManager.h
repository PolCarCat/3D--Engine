#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__
#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <map>

class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	uint32_t FindFile(const char* file) const;
	uint32_t ImportFile(const char* file);
	Resource* GetResource(uint32_t uid) const;
	Resource* GetResourceByName(const char* name) const;


	Resource* CreateNewResource(ResType t, uint32_t id = 0);
	void AddResource(Resource* res);

private:

	std::map<uint32_t, Resource*> resources;

};


#endif //__MODULERESOURCEMANAGER_H__
