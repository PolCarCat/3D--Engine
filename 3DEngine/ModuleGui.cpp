#include "ModuleGui.h"
#include "Application.h"
#include <gl/GL.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleGui::~ModuleGui()
{
}


bool ModuleGui::Start()
{
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();


	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	

	release_link = "https://github.com/PolCarCat/3D--Engine/releases";
	wiki_link = "https://github.com/PolCarCat/3D--Engine/wiki";
	issues_link = "https://github.com/PolCarCat/3D--Engine/issues";


	config = new WinConfig(App, true);
	AddWindow(config);


	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
		(*item)->Start();
	}

	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();



	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

	ImGui::End;

	ImGui::Separator();

	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
		if ((*item)->GetEnable())
		(*item)->Update();
	}

	/*if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
				App->SaveGame();

			if (ImGui::MenuItem("Load"))
				App->LoadGame();

			if (ImGui::MenuItem("Quit"))
				quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show/Hide Gui Demo"))
				showdemo = !showdemo;
			if (ImGui::MenuItem("Go to our Wiki"))
				ShellExecute(NULL, "open", wiki_link, NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("Download our latest release"))
				ShellExecute(NULL, "open", release_link, NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("Report a bug"))
				ShellExecute(NULL, "open", issues_link, NULL, NULL, SW_SHOWNORMAL);
			if (ImGui::MenuItem("About"))
				about = !about;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Configuration"))
				config_b = !config_b;
			if (ImGui::MenuItem("Random Number Generator"))
				rngwindow = !rngwindow;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}*/

	if (about)
		AboutWindow();

		ImGui::ShowDemoWindow();
	if (config_b)
		ConfigWindow();
	if (rngwindow)
		RngWindow();
	PrimitivesWindow();

	return UPDATE_CONTINUE;
}



update_status ModuleGui::Update(float dt)
{
	return UPDATE_CONTINUE;
}


update_status ModuleGui::PostUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if (quit)
		status = UPDATE_STOP;

	return status;
}

bool ModuleGui::CleanUp()
{
	//ImGui_ImplSdl_Shutdown();

	VSLOG("Cleaning UP IMGUI Module")
		bool ret = true;

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGui::AddWindow(WinBase* win)
{
	windows.push_back(win);
}

void ModuleGui::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Text("Hello");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void ModuleGui::RngWindow()
{
	ImGui::Begin("Random Number Generator");

	ImGui::Text("%.2f", random_f);

	if (ImGui::Button("Generate random float between 0 and 1"))
	{
		random_f = ldexp(pcg32_random_r(&rng), -32);
	}

	ImGui::Separator();

	ImGui::SliderInt("Min", &min, 0, 100);
	ImGui::SliderInt("Max", &max, 0, 100);

	if (max < min)
		max = min;

	ImGui::Text("%d", random_bounded);

	if (ImGui::Button("Generate random integer between two numbers"))
	{
		if (max - min > 0)
			random_bounded = (int)pcg32_boundedrand_r(&rng, max - min + 1) + min;
		else
			random_bounded = 0;
	}

	ImGui::End();
}
void ModuleGui::AboutWindow()
{
	SDL_version version;
	SDL_GetVersion(&version);

	ImGui::Begin("About");

	ImGui::Text("-NAME OF ENGINE- 0.1\n\n\nEngine made for 3rd year University subject.\n\nBy Pol Carrera and Genis Bayo.\n\nUsing:\nSDL %d.%d.%d\nImGUI %s\n\nMIT License (See 'LICENSE' for more information).", version.major, version.minor, version.patch, ImGui::GetVersion());

	ImGui::End();
}

void ModuleGui::ConfigWindow()
{
	
}

void ModuleGui::PrimitivesWindow()
{
	ImGui::Begin("Primitives");

	ImGui::Checkbox("Direct Cube", &App->renderer3D->directCube);
	ImGui::Checkbox("Vertex arrays Cube", &App->renderer3D->varrCube);
	ImGui::Checkbox("Indices Cube", &App->renderer3D->indCube);
	ImGui::Checkbox("Ray", &App->renderer3D->ray);
	ImGui::Checkbox("Arrow", &App->renderer3D->arrow);
	ImGui::Checkbox("Plane", &App->renderer3D->plane);
	ImGui::Checkbox("Sphere", &App->renderer3D->sphere);

	ImGui::End();
}