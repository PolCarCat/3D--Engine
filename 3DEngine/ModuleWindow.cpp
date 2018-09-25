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
		Load(App->jsondoc.GetObj(name.c_str()));
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(FS == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(res == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(bord == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(FSWin == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

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
	SDL_SetWindowResizable(window, resizable);
	SDL_SetWindowBordered(window, bordered);

	if (!fullscreen)
	{
		SDL_SetWindowSize(window, w * SCREEN_SIZE, h * SCREEN_SIZE);
		resized = false;
	}

	if (fullscreen && !resized)
	{
		SDL_GetDisplayMode(0, 0, &DM);
		SDL_SetWindowSize(window, DM.w * SCREEN_SIZE, DM.h * SCREEN_SIZE);

		resized = true;
	}

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

	w = App->jsondoc.GetObjValueInt(doc, "Width");
	h = App->jsondoc.GetObjValueInt(doc, "Height");
	brightness = App->jsondoc.GetObjValueFloat(doc, "Brightness");
	res = App->jsondoc.GetObjValueBool(doc, "Resizable");
	bord = App->jsondoc.GetObjValueBool(doc, "Borderless");
	FS = App->jsondoc.GetObjValueBool(doc, "Fullscreen");
	FSWin = App->jsondoc.GetObjValueBool(doc, "Fullscreen Window");

	return true;
}

bool ModuleWindow::Save(json_object_t* doc)
{
	json_object_set_number(doc, "Window.Width", w);
	json_object_set_number(doc, "Window.Height", h);
	json_object_set_number(doc, "Window.Brightness", brightness);
	json_object_set_boolean(doc, "Window.Resizable", res);
	json_object_set_boolean(doc, "Window.Borderless", bord);
	json_object_set_boolean(doc, "Window.Fullscreen", FS);
	json_object_set_boolean(doc, "Window.Fullscreen Window", FSWin);
	return true;
}