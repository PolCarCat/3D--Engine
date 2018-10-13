#pragma once
#include "Module.h"
#include "Globals.h"
#include "WinBase.h"
#include "WinConfig.h"
#include "WinConsole.h"
#include "WinElem.h"
#include "WinTest.h"
#include "WinObjects.h"
#include <list>




class ModuleGui :
	public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	bool CleanUp();
	void AddWindow(WinBase* win);

public:
	WinConfig * config = nullptr;
	WinConsole* console = nullptr;
	WinElem*	element = nullptr;
	WinTest*	test = nullptr;
	WinObjects* objects = nullptr;

private:
	void AboutWindow();
	void UpdateDockSpace();

private:
	bool quit = false;
	std::list<WinBase*> windows;

	bool showAll = false;
	char* release_link = nullptr;
	char* issues_link = nullptr;
	char* wiki_link = nullptr;
	bool showdemo = false;
	bool about = false;

};