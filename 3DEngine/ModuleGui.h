#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "ModuleInput.h"
#include "Globals.h"
#include "WinBase.h"
#include "WinConfig.h"
#include "WinConsole.h"
#include "WinElem.h"
#include "WinTest.h"
#include "WinObjects.h"
#include "WinCamera.h"
#include "WinSceneLoader.h"
#include "WinTimeManager.h"
#include "WinAssets.h"


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

#include "ImGui/ImGuizmo.h"

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
	void ReadInput(SDL_Event * e) const;
	void DrawGuizmo(GameObject* obj);
	void UpdateGuizmoInput();

	bool InWindow() const;

public:
	WinConfig * config = nullptr;
	WinConsole* console = nullptr;
	WinElem*	element = nullptr;
	WinTest*	test = nullptr;
	WinObjects* objects = nullptr;
	WinCamera* cam = nullptr;
	WinSceneLoader* scene = nullptr;
	WinTimeManager* time = nullptr;
	WinAssets* assets = nullptr;

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

	ImGuizmo::MODE guizmoMode = ImGuizmo::WORLD;
	ImGuizmo::OPERATION guizmoOperation = ImGuizmo::TRANSLATE;
	float guizmoSnapSize[3];
	bool guizmoSnap = false;
	bool inWindow = false;

};

#endif //__MODULEGUI_H__