#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

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
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AddGameObject(GameObject* obj);
	void CreateEmptyObj();
	void DeleteGameObject(GameObject* obj);
	void SetCurCam(ComponentCamera* cam);
	void CreateMaterial();
	ComponentCamera* GetCurCam() const;
	ComponentCamera* GetGhostCam() const;
	ComponentMaterial* CheckMaterial(const char* name);

	const char* GetName() const;
	void SetName(const char* n);
	void UpdateShorcuts();

	GameObject* selectedObj = nullptr;
	ComponentMaterial* selectedMat = nullptr;
	GameObject root;

	std::list<ComponentMaterial*> materials;

	Quadtree quadTree;

private:

	ComponentCamera* ghostcam = nullptr;
	ComponentCamera* currentCam = nullptr;
	std::string name;

};

#endif // !__MODULESCENE_H__