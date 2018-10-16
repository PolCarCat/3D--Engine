#pragma once
class Component
{
public:
	Component()
	{}

	~Component()
	{}

	enum Type {
		MESH,
		TRANSFORM,
		MATERIAL
	};

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

public:
	Type type;


};

