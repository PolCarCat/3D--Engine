#pragma once
#include "Globals.h"
#include "Parson/parson.h"
#include "JsonDoc.h"

class Application;

class Module
{
private :
	bool enabled;

protected:
	char* name = nullptr;
public:
	Application* App;
	void SetName(const char* _name) { *name = *_name; };
	const char* GetName() { return name; };

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Load(json_object_t* doc)
	{
		return true;
	}

	virtual bool Save(json_object_t* doc)
	{
		return true;
	}
};