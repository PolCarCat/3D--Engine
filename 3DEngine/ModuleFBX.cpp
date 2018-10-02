#include "ModuleFBX.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "SDL/include/SDL.h"
//#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleFBX::~ModuleFBX()
{
}

bool ModuleFBX::Start()
{

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	return true;
}

update_status ModuleFBX::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleFBX::Update(float dt)
{

	CheckDropEvent();

	return UPDATE_CONTINUE;
}

update_status ModuleFBX::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleFBX::CleanUp()
{

	SDL_free(droppedFile);

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleFBX::CheckDropEvent()
{
	SDL_Event dropEvent;
	while (SDL_PollEvent(&dropEvent))
	{
		if (dropEvent.type == SDL_DROPFILE)
		{
			droppedFile = dropEvent.drop.file;
			// Shows directory of dropped file
		}
	}
}
