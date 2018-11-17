#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JsonDoc.h"

class GameObject;


enum Type {
	MESH,
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

	Type GetType() const;
	GameObject* GetParent() const;
	void SetParent(GameObject* p);
	void SetActive(bool a);
	bool GetActive() const;
	void Delete();
	bool CheckDelete() const;
	bool toDelete = false;

protected:
	Type type = NULLCOMP;
	GameObject* parent = nullptr;
	bool active = true;

};

#endif // !__COMPONENT_H__