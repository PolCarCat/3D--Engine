#include "ModuleLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib/MathGeoLib.h"
#include "WinConsole.h"
#include "ResMesh.h"
#include "ResTexture.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "SDL/include/SDL.h"
#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"




#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Start()
{



	texImporter.Start();
	meshImporter.Start();

	return true;
}

update_status ModuleLoader::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleLoader::Update(float dt)
{

	//CheckDropEvent();


	return UPDATE_CONTINUE;
}

update_status ModuleLoader::PostUpdate(float dt)
{

	if (droppedFile != nullptr)
	{
		Format f = CheckFormat(droppedFile);
		switch (f)
		{
		case FBX:
			App->renderer3D->CleanUpMeshes();
			meshImporter.LoadScene(droppedFile);
			break;
		case PNG:
		case DDS:
			App->imgui->element->curMesh->tex.CleanUp();
			break;
		case FNULL:
			break;
		default:
			break;
		}
		
		droppedFile = nullptr;
	}
	return UPDATE_CONTINUE;
}

bool ModuleLoader::CleanUp()
{
	if (droppedFile != nullptr)
	SDL_free(droppedFile);



	texImporter.CleanUp();
	return true;
}


void ModuleLoader::SetDropFile(char* f)
{
	droppedFile = f;
}


Format ModuleLoader::CheckFormat(const char* path)
{
	Format ret = FNULL;
	std::string t = path;
	std::string format;

	bool point = false;

	for (std::string::reverse_iterator rit = t.rbegin(); rit != t.rend(); ++rit)
	{
		if (*rit == '.')
			break;
		format.insert(0, 1, *rit);
	}
	

	if (format == "FBX" || format == "fbx")
		ret = FBX;
	
	else if (format == "PNG" || format == "png")
		ret = PNG;
	
	else if (format == "DDS" || format == "dds")
		ret = DDS;

	else if (format == "json" || format == "JSON")
		ret = JSON;
	
	return ret;
}

std::string ModuleLoader::GetFileName(const char* path, bool extension)
{
	std::string t = path;
	std::string name;

	bool point = false;

	if (extension) point = true;

	for (std::string::reverse_iterator rit = t.rbegin(); rit != t.rend(); ++rit)
	{
		if (*rit == '/' || *rit == '\\')
			break;

		if (point)
		name.insert(0, 1, *rit);

		if (*rit == '.')
			point = true;
	}

	return name;
}


std::string ModuleLoader::GetFileDir(const char* path)
{
	std::string t = path;

	for (std::string::reverse_iterator rit = t.rbegin(); rit != t.rend(); ++rit)
	{
		if (*rit == '/' || *rit == '\\')
			break;
		else
			t.pop_back();
	}
	return t;
}

void ModuleLoader::LoadFBX(const char* path)
{

	meshImporter.LoadScene(path);
}