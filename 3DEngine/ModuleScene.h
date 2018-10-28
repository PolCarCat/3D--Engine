#pragma once
#include "Module.h"
#include "GameObject.h"

#include <list>

class ComponentCamera;
class ComponentMaterial;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void AddGameObject(GameObject* obj);
	void CreateEmptyObj();
	void DeleteGameObject(GameObject* obj);
	void SetCurCam(ComponentCamera* cam);
	void CreateMaterial();
	ComponentCamera* GetCurCam();
	ComponentCamera* GetGhostCam();

	GameObject* selectedObj = nullptr;
	ComponentMaterial* selectedMat = nullptr;
	GameObject root;
	std::list<ComponentMaterial*> materials;

	Quadtree quadTree;
private:

	ComponentCamera* ghostcam;
	ComponentCamera* currentCam = nullptr;

};
