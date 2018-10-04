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

	release_link = "https://github.com/PolCarCat/3D--Engine/releases";
	wiki_link = "https://github.com/PolCarCat/3D--Engine/wiki";
	issues_link = "https://github.com/PolCarCat/3D--Engine/issues";

	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
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
				config = !config;
			if (ImGui::MenuItem("Random Number Generator"))
				rngwindow = !rngwindow;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (about)
		AboutWindow();
	if (showdemo)
		ImGui::ShowDemoWindow();
	if (config)
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
	ImGui::Begin("Configuration");

	if (ImGui::CollapsingHeader("Frame management"))
	{
		ImGui::SliderInt("FPS Cap", &App->framerate_cap, 1, 120);

		char plot[50];
		sprintf_s(plot, 50, "Framerate %.1f", App->fps[App->fps_counter - 1]);
		ImGui::PlotHistogram("Framerate", &App->fps[0], 50, 0, plot, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::Spacing();
		sprintf_s(plot, 50, "Milliseconds %.1f", App->ms[App->ms_counter - 1]);
		ImGui::PlotHistogram("Milliseconds", &App->ms[0], 50, 0, plot, 0.0f, 100.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::SliderFloat("Brightness", &App->window->brightness, 0, 1);
		ImGui::SliderInt("Width", &App->window->w, 100, 4000);
		ImGui::SliderInt("Height", &App->window->h, 100, 4000);
		ImGui::Text("Refresh rate: %d", App->window->refresh_rate);

		ImGui::Checkbox("FullScreen", &App->window->FS);
		ImGui::Checkbox("Full Desktop", &App->window->FSD);
		ImGui::Checkbox("Resizable", &App->window->res);
		ImGui::Checkbox("Bordered", &App->window->bord);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("%d CPU's (%dkb Cache)\nSystem RAM: %.0fGb\nCaps:", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize(), (float)SDL_GetSystemRAM() / 1000);

		if (SDL_Has3DNow())
			ImGui::Text("3DNow");
		if (SDL_HasAVX())
			ImGui::Text("AVX");
		if (SDL_HasAVX2())
			ImGui::Text("AVX2");
		if (SDL_HasAltiVec())
			ImGui::Text("AltiVec");
		if (SDL_HasMMX())
			ImGui::Text("MMX");
		if (SDL_HasRDTSC())
			ImGui::Text("RDTSC");
		if (SDL_HasSSE())
			ImGui::Text("SSE");
		if (SDL_HasSSE2())
			ImGui::Text("SSE2");
		if (SDL_HasSSE3())
			ImGui::Text("SSE3");
		if (SDL_HasSSE41())
			ImGui::Text("SSE41");
		if (SDL_HasSSE42())
			ImGui::Text("SSE42");
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{

		if (ImGui::Button((App->renderer3D->GetVsync() != true ? "Disable Vsync" : "Enable Vsync")))
			App->renderer3D->EnableVsync();

		if (ImGui::Button((App->renderer3D->GetDepthTest() != true ? "Disable Depth Test" : "Enable Depth Test")))
			App->renderer3D->EnableDepthTest();

		if (ImGui::Button((App->renderer3D->GetCullFace() != true ? "Disable CullFace" : "Enable CullFace")))
			App->renderer3D->EnableCullFace();

		if (ImGui::Button((App->renderer3D->GetLighting() != true ? "Disable Lighting" : "Enable Lighting")))
			App->renderer3D->EnableLighting();

		if (ImGui::Button((App->renderer3D->GetColorMat() != true ? "Disable Color Material" : "Enable Color Material")))
			App->renderer3D->EnableColorMaterial();

		if (ImGui::Button((App->renderer3D->GetTexture2D() != true ? "Disable Texture 2D" : "Enable Texture 2D")))
			App->renderer3D->EnableTexture2D();

		if (ImGui::Button((App->renderer3D->GetWireFrame() != true ? "Enable WireFrame" : "Disable WireFrame")))
			App->renderer3D->EnableWireframe();
		


	}
	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (ImGui::Checkbox("Draw Normals", &App->renderer3D->drawNormals));
	}
	ImGui::End();
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