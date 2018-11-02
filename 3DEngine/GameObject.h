#pragma once
#include <vector>
#include "ResMesh.h"
#include "QuadTree.h"
#include "Component.h"
#include "JsonDoc.h"

class ComponentTransform;


class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Start();
	bool Update();
	bool PostUpdate();
	void UpdateUI();
	bool CleanUp();

	void AddComponent(Component* comp);
	void AddCompMesh();
	void AddCompMesh(ResMesh* m);
	void AddCompCam(float _near = 0.5f, float _far = 512.0f, float fov = 60.0f);
	void AddGameObject(GameObject* obj);
	void DeleteComp(Component* comp);
	void DeleteGameObj(GameObject* obj);
	void AddComponent(Type t);
	bool CheckIfContained(GameObject* obj);

	bool GetActive();
	void SetActive(bool b);
	const char* GetName();
	void SetName(const char* n);
	void SetName(std::string n);
	void AddBox(AABB b);
	bool GetStatic();
	void SetStatic(bool b);
	AABB GetBB();
	void Delete();
	bool CheckDelete();
	uint32_t GetUUID();

	void Save(JSON_Array* objects, JsonDoc* doc);
	void Load(JSON_Object* json, JsonDoc* doc);

	GameObject* GetParent();
	void SetParent(GameObject* p);

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;
	ComponentTransform * transform = nullptr;

private:


	GameObject* parent = nullptr;
	AABB bBox;
	bool active = true;
	bool staticobj = false;
	bool toDelete = false;
	std::string name;
	void UpdateNewComWindow();
	void SetChildsActive(bool a);
	uint32_t uuid = 0;
};