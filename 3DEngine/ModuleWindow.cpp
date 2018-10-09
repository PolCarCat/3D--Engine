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
		Load(App->config.GetObj(name));
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

		SDL_GetDisplayMode(0, 0, &DM);
		refresh_rate = DM.refresh_rate;
	}

	return ret;
}

update_status ModuleWindow::Update(float dt)
{
	SetBools();

	SDL_SetWindowBrightness(window, brightness);
	SDL_SetWindowResizable(window, resizable);
	SDL_SetWindowBordered(window, bordered);

	if (!fullscreen && !full_desktop)
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, w * SCREEN_SIZE, h * SCREEN_SIZE);
		resized = false;
	}

	if (!resized && fullscreen)
	{
		App->renderer3D->OnResize(DM.w, DM.h);
		SDL_SetWindowSize(window, DM.w * SCREEN_SIZE, DM.h * SCREEN_SIZE);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		resized = true;
	}

	if (!resized && full_desktop)
	{
		App->renderer3D->OnResize(DM.w, DM.h);
		SDL_SetWindowSize(window, DM.w * SCREEN_SIZE, DM.h * SCREEN_SIZE);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		resized = true;
	}

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

	w = App->config.GetObjValueInt(doc, "Width");
	h = App->config.GetObjValueInt(doc, "Height");
	brightness = App->config.GetObjValueFloat(doc, "Brightness");
	res = App->config.GetObjValueBool(doc, "Resizable");
	bord = App->config.GetObjValueBool(doc, "Borderless");
	FS = App->config.GetObjValueBool(doc, "Fullscreen");
	FSWin = App->config.GetObjValueBool(doc, "Fullscreen Window");

	return true;
}

bool ModuleWindow::Save(json_object_t* doc)
{
	JSON_Status error; 

	error = json_object_dotset_number(doc, "Window.Width", w);
	error = json_object_dotset_number(doc, "Window.Height", h);
	error = json_object_dotset_number(doc, "Window.Brightness", brightness);
	error = json_object_dotset_boolean(doc, "Window.Resizable", res);
	error = json_object_dotset_boolean(doc, "Window.Borderless", bord);
	error = json_object_dotset_boolean(doc, "Window.Fullscreen", FS);
	error = json_object_dotset_boolean(doc, "Window.Fullscreen Window", FSWin);

	return !error;
}

void ModuleWindow::SetBools()
{
	if (FS && FSD)
	{
		if (full_desktop = SDL_TRUE)
			FSD = false;
		if (fullscreen = SDL_TRUE)
			FS = false;
	}

	if (res)
		resizable = SDL_TRUE;
	else
		resizable = SDL_FALSE;
	if (FS)
		fullscreen = SDL_TRUE;
	else
		fullscreen = SDL_FALSE;
	if (FSD)
		full_desktop = SDL_TRUE;
	else
		full_desktop = SDL_FALSE;
	if (bord)
		bordered = SDL_TRUE;
	else
		bordered = SDL_FALSE;

}