//#include "Application.h"
#include "ModuleTimeManager.h"
#include "SceneLoader.h"

#include "mmgr/mmgr.h"

ModuleTimeManager::ModuleTimeManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Start()
{
	timeScale = 1;
	realTime.SetZero();

	return true;
}

update_status ModuleTimeManager::PreUpdate(float dt)
{

	realDeltaTime = dt;


	if (playing)
	{
		deltaTime = dt * timeScale;
		gameTime += deltaTime;
	}

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

void ModuleTimeManager::StartGame()
{
	realTime.Start();
	gameTime = 0;
	playing = true;

	serializedScene = SceneLoader::SerializeScene();
}

void ModuleTimeManager::Reset()
{
	gameTime = 0;
	playing = false;
	realTime.SetZero();

	SceneLoader::LoadScene(serializedScene);
}

float ModuleTimeManager::GetRealTime()
{
	float rt = realTime.ReadSec();

	if (realTime.IsZero())
		rt = 0;

	return rt;
}

float ModuleTimeManager::GetGameTime()
{
	return gameTime;
}
float ModuleTimeManager::GetTimeScale()
{
	return timeScale;
}

float ModuleTimeManager::GetDeltaTime()
{
	return deltaTime;
}

float ModuleTimeManager::GetRealDeltaTime()
{
	return realDeltaTime;
}

bool ModuleTimeManager::GetPlaying()
{
	return playing;
}

void ModuleTimeManager::SetTimeScale(float ts)
{
	timeScale = ts;
}

void ModuleTimeManager::SetPlaying(bool p)
{
	playing = p;

	if (playing && gameTime == 0)
	{
		StartGame();
	}

}