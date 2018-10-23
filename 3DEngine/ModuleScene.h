#pragma once
#include "Module.h"
#include "ResTexture.h"
#include "GameObject.h"
#include <list>

class ComponentCamera;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void AddGameObject(GameObject* obj);
	void MoveCurCam();
	void SetCurCam(ComponentCamera* cam);
	ComponentCamera* GetCurCam();
	ComponentCamera* GetGhostCam();

	GameObject* selectedObj = nullptr;
	GameObject root;
private:

	ResTexture currentTex;

	ComponentCamera* ghostcam;
	ComponentCamera* currentCam = nullptr;

};
