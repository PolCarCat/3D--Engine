#pragma once
#include <vector>
#include "ResMesh.h"
#include "QuadTree.h"

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
	void AddCompCam(float _near = 0.5f, float _far = 512.0f, float fov = 60.0f);
	void AddGameObject(GameObject* obj);
	bool GetActive();
	void SetActive(bool b);
	const char* GetName();
	void SetName(const char* n);
	void SetName(std::string n);
	void AddBox(AABB b);
	bool GetStatic();
	void SetStatic(bool b);
	AABB GetBB();

	GameObject* GetParent();
	void SetParent(GameObject* p);

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;

private:

	ComponentTransform * transform = nullptr;
	GameObject* parent = nullptr;
	AABB bBox;
	bool active = true;
	bool staticobj = false;
	std::string name;
};