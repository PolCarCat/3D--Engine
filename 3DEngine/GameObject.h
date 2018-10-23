#pragma once
#include <vector>
#include "ResMesh.h"
class Component;
class ComponentTransform;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	void AddComponent(Component* comp);
	void AddCompMesh(ResMesh mesh);
	void AddGameObject(GameObject* obj);
	bool GetActive();
	void SetActive(bool b);
	const char* GetName();
	void SetName(const char* n);
	void SetName(std::string n);

	GameObject* GetParent();
	void SetParent(GameObject* p);

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;
private:

	ComponentTransform * transform = nullptr;
	GameObject* parent = nullptr;
	bool active = true;
	std::string name;
};