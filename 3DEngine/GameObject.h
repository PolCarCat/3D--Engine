#pragma once
#include <list>

class Compontent;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	void AddComponent(Compontent* comp);

	std::list<Compontent*> compChilds;
	std::list<GameObject*> objChilds;
	GameObject* parent = nullptr;

};