#pragma once
#include <vector>

class Component;

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
	bool GetActive();
	void SetActive(bool b);
	const char* GetName();
	void SetName(const char*);
private:

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;
	GameObject* parent = nullptr;
	bool active = true;
	std::string name;
};