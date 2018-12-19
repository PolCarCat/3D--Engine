#include "Application.h"
#include "ModuleResourceManager.h"
#include "ResMesh.h"
#include "ResTexture.h"
#include "pcg-c-basic-0.9/pcg_basic.h"

#include "mmgr/mmgr.h"

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

bool ModuleResourceManager::Start()
{
	//LoadDirectory("Assets/");
	GenerateBillboard();

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

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		it->second->CleanUp();
		delete it->second;
		it->second = nullptr;
	}
	
	resources.clear();

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

uint32_t ModuleResourceManager::ImportFile(std::string file)
{

	Format f = App->loader->CheckFormat(file.c_str());

	switch (f)
	{
	case FBX:
		App->loader->meshImporter.LoadScene(file.c_str());
		break;
	case PNG:
	case DDS:
		App->loader->texImporter.LoadTex(file.c_str());
		break;
	case MEH:
		App->loader->meshImporter.LoadMeh(file.c_str(), true);
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

ResMesh * ModuleResourceManager::GetBillboard() const
{
	return billboard;
}

void ModuleResourceManager::GenerateBillboard()
{


	static const float vertex[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	billboard = new ResMesh();

	//Vertex
	billboard->num_vertex = 4;
	billboard->vertex = new float[billboard->num_vertex * 3];
	memcpy(billboard->vertex, vertex, sizeof(vertex));


	//Indices
	static const uint indices[] = {
		0,1,2,1,3,2
	};
	billboard->num_indice = 6;
	billboard->indice = new uint[billboard->num_indice];
	memcpy(billboard->indice, indices, sizeof(indices));

	//Normals
	//billboard->normals = new float[mesh->num_vertex * 3];
	//memcpy(billboard->normals, last, bytes);


	//Texture Coordinates
	static const float tex[] = {
	1,1,0,1,1,0,0,0
	};

	billboard->num_textC = billboard->num_vertex * 2;
	billboard->textC = new float[billboard->num_textC];
	memcpy(billboard->textC, tex, sizeof(tex));
	
	billboard->GenerateBuffer();
}

void ModuleResourceManager::LoadAssets()
{
	LoadDirectory("Assets/");
}

void ModuleResourceManager::LoadDirectory(std::string dir)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	hFind = FindFirstFile((dir + "*").c_str(), &data);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && data.cFileName != std::string(".") && data.cFileName != std::string(".."))
			{
				//LoadDirectory(dir + data.cFileName + std::string("\\"));
			}
			else if (data.cFileName != std::string(".") && data.cFileName != std::string(".."))
			{
				ImportFile(dir + data.cFileName);
			}
			

		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}
