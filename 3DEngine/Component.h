#pragma once

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

	Type GetType();
	GameObject* GetParent();
	void SetParent(GameObject* p);
	void SetActive(bool a);
	bool GetActive();
	void Delete();
	bool CheckDelete();
	bool toDelete = false;

protected:
	Type type;
	GameObject* parent = nullptr;
	bool active = true;

};

