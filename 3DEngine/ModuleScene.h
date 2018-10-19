#pragma once
#include "Module.h"
#include "ResTexture.h"
#include "GameObject.h"
#include <list>

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void AddGameObject(GameObject* obj);

private:
	GameObject root;
	ResTexture currentTex;

};
