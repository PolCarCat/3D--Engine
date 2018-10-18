#pragma once

class GameObject;

enum Type {
	MESH,
	TRANSFORM,
	MATERIAL
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

protected:
	Type type;
	GameObject* parent = nullptr;
	bool active = true;

};

