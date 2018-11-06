#pragma once
#include "JsonDoc.h"

class GameObject;


enum Type {
	MESH,
	RESMESH,
	TRANSFORM,
	CAMERA,
	MATERIAL,
	NULLCOMP
};

class Component
{
public:
	Component();


	~Component();
	


	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual void UpdateUI()
	{
	}

	virtual bool CleanUp()
	{
		return true;
	}


	virtual bool Save(JSON_Object* json, JsonDoc* doc)
	{
		return true;
	}

	virtual bool Load(JSON_Object* json, JsonDoc* doc)
	{
		return true;
	}

	Type GetType();
	GameObject* GetParent();
	void SetParent(GameObject* p);
	void SetActive(bool a);
	bool GetActive();
	void Delete();
	bool CheckDelete();
	bool toDelete = false;

protected:
	Type type = NULLCOMP;
	GameObject* parent = nullptr;
	bool active = true;

};

