#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

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

	//Mono behaviour
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
	Component* AddCompCam(float _near = 0.5f, float _far = FPD, float fov = 60.0f);
	void AddGameObject(GameObject* obj);
	void DeleteComp(Component* comp);
	void DeleteGameObj(GameObject* obj);
	bool CheckIfContained(GameObject* obj);

	//Self Properties
	bool GetActive() const;
	void SetActive(bool b);
	const char* GetName() const;
	void SetName(const char* n);
	void SetName(std::string n);
	void AddBox(AABB b);
	bool GetStatic() const;
	void SetStatic(bool b);
	AABB GetLocalABB() const;
	AABB GetGlobalABB() const;
	OBB GetOBB() const;
	void Delete();
	bool CheckDelete() const;
	bool GetDrawBB() const;

	uint32_t GetUUID() const;
	GameObject* GetObjByUUID(uint32_t id);
	void IsPickedABB(LineSegment picking, std::vector<GameObject*> &vec);

	void Save(JSON_Array* objects, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);

	GameObject* GetParent() const;
	void SetParent(GameObject* p, bool changeGlobal = false);
	void CalcGlobalTransform();
	void Select();
	void Deselect();
	bool IsSelected() const;
	bool IsInQT() const;
	void SetInQT(bool b);

	GameObject* Copy();

	std::vector<Component*> compChilds;
	std::vector<GameObject*> objChilds;
	ComponentTransform * transform = nullptr;


private:

	bool selected = false;
	GameObject* parent = nullptr;
	AABB localABB;
	AABB globalABB;
	OBB oBB;
	bool active = true;
	bool staticobj = false;
	bool toDelete = false;
	bool drawBB = false;
	bool inQT = false;

	std::string name;
	uint32_t uuid = 0;

	void UpdateNewComWindow();
	void SetChildsActive(bool a);

	uint numCopies = 1;
};
#endif //__GAMEOBJECT_H__