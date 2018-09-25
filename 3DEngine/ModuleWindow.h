#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"


class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	update_status Update(float dt);

	bool Init();
	bool CleanUp();
	bool Load(json_object_t* doc);
	bool Save(json_object_t* doc) const;

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int w;
	int h;
	int brightness = 50;
	bool res = true;
	bool bord = true;
	bool FS = false;
	SDL_bool resizable = SDL_TRUE;
	//SDL_bool full_desktop = SDL_FALSE;
	SDL_bool bordered = SDL_TRUE;
	SDL_bool fullscreen = SDL_FALSE;
};

#endif // __ModuleWindow_H__