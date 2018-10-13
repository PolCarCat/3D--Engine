#pragma once


class Application;

class WinBase
{
protected:

	char* name = nullptr;

public:
	bool enabled = true;
	Application * App;

	WinBase(Application* parent, bool start_enabled = true) : App(parent)
	{}

	~WinBase()
	{}

	void SetName(const char* _name) { *name = *_name; };
	const char* GetName() { return name; };
	void SetEnable(bool b) { enabled = b; };
	bool GetEnable() { return enabled; };

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}


};

