#pragma once
#include "Module.h"
#include "Globals.h"

#include "pcg-c-basic-0.9/pcg_basic.h"


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
	void ShowMenuBar();
	void AddLogWindow();

private:
	bool quit = false;
	pcg32_random_t rng;
	int random_bounded = 0;
	int max = 0;
	int min = 0;
	float random_f = 0;
	char* release_link;
	char* issues_link;
	char* wiki_link;
	bool showdemo = false;
	bool about = false;
};