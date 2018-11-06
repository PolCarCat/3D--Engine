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

	//Components
	void AddComponent(Component* comp);
	Component* AddComponent(Type t);
	Component* AddCompMesh();
	Component* AddCompMesh(ResMesh* m);
	Component* AddCompCam(float _near = 0.5f, float _far = 512.0f, float fov = 60.0f);
	void AddGameObject(GameObject* obj);
	void DeleteComp(Component* comp);
	void DeleteGameObj(GameObject* obj);
	bool CheckIfContained(GameObject* obj);

	bool GetActive();
	void SetActive(bool b);
	const char* GetName();
	void SetName(const char* n);
	void SetName(std::string n);
	void AddBox(AABB b);
	bool GetStatic();
	void SetStatic(bool b);
	AABB GetLocalABB();
	AABB GetGlobalABB();
	OBB GetOBB();
	void Delete();
	bool CheckDelete();
	uint32_t GetUUID();
	GameObject* GetObjByUUID(uint32_t id);
	void IsPickedABB(LineSegment picking, std::vector<GameObject*> vec);

	void Save(JSON_Array* objects, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);

	GameObject* GetParent();
	void SetParent(GameObject* p);
	void CalcGlobalTransform();

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;
	ComponentTransform * transform = nullptr;


private:


	GameObject* parent = nullptr;
	AABB localABB;
	AABB globalABB;
	OBB oBB;
	bool active = true;
	bool staticobj = false;
	bool toDelete = false;
	std::string name;
	void UpdateNewComWindow();
	void SetChildsActive(bool a);
	uint32_t uuid = 0;
};