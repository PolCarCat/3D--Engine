#include "Application.h"
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
	

	App->renderer3D->SetUseGhostCam(false);
	serializedScene = SceneLoader::SerializeScene();
}

void ModuleTimeManager::Reset()
{
	gameTime = 0;
	playing = false;
	realTime.SetZero();


	App->renderer3D->SetUseGhostCam(true);

	SceneLoader::CleanScene();
	SceneLoader::LoadScene(serializedScene);
	serializedScene.CleanUp();
}

float ModuleTimeManager::GetRealTime() const
{
	float rt = realTime.ReadSec();

	if (realTime.IsZero())
		rt = 0;

	return rt;
}

float ModuleTimeManager::GetGameTime() const
{
	return gameTime;
}
float ModuleTimeManager::GetTimeScale() const
{
	return timeScale;
}

float ModuleTimeManager::GetDeltaTime() const
{
	return deltaTime;
}

float ModuleTimeManager::GetRealDeltaTime() const
{
	return realDeltaTime;
}

bool ModuleTimeManager::GetPlaying() const
{
	return playing;
}

void ModuleTimeManager::SetTimeScale(float ts)
{
	timeScale = ts;

	//Not the best stuff
	if (timeScale < 0)
		timeScale = 0;
}

void ModuleTimeManager::AddTimeScale(float ts)
{
	timeScale += ts;
}

void ModuleTimeManager::SetPlaying(bool p)
{

	if (gameTime == 0)
	{
		playing = true;
		StartGame();
	}

	if (p)
	{
		timeScale = 1.0f;
	}
	else
		timeScale = 0.0f;


}