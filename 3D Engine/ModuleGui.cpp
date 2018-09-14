#include "ModuleGui.h"
#include "Application.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"


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



	// Setup ImGui binding
	//ImGui_ImplGlfwGL3_Init(window, true);


	return true;
}

update_status ModuleGui::Update(float dt)
{

	//bool b = true;
	//ImGui::ShowDemoWindow(&b);
	//ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	//ImGui_ImplSdl_Shutdown();
	return true;
}