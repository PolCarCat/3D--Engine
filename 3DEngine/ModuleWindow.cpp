#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "JsonDoc.h"
#include "Parson\parson.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	VSLOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		VSLOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		w = width;
		h = height;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			VSLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

update_status ModuleWindow::Update(float dt)
{
	if (res)
		resizable = SDL_TRUE;
	else
		resizable = SDL_FALSE;
	if (FS)
		fullscreen = SDL_TRUE;
	else
		fullscreen = SDL_FALSE;
	if (bord)
		bordered = SDL_TRUE;
	else
		bordered = SDL_FALSE;

	SDL_SetWindowBrightness(window, brightness);
	SDL_SetWindowSize(window, w * SCREEN_SIZE, h * SCREEN_SIZE);
	SDL_SetWindowResizable(window, resizable);
	SDL_SetWindowBordered(window, bordered);
	SDL_SetWindowFullscreen(window, fullscreen);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	VSLOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}



bool ModuleWindow::Load(json_object_t* doc)
{
	
	JSON_Value* v = json_object_get_value(doc, "Width");
	int i = -1;

	if (json_value_get_type(v) == JSONNumber)
	{
		i = json_value_get_number(v);
	}

	

	return true;
}

bool ModuleWindow::Save(json_object_t* doc) const
{

	return true;
}