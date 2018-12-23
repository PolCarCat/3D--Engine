#include "ModuleGui.h"
#include "Application.h"
#include <gl/GL.h>
#include "ImGui/imgui_internal.h"
#include "SceneLoader.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"


#include "mmgr/mmgr.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	console = new WinConsole(App, true);
	App->logAvaliable = true;
}


ModuleGui::~ModuleGui()
{
}


bool ModuleGui::Start()
{
	//Init ImGui
	ImGuiContext* context = ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//Basic Links
	release_link = "https://github.com/PolCarCat/3D--Engine/releases";
	wiki_link = "https://github.com/PolCarCat/3D--Engine/wiki";
	issues_link = "https://github.com/PolCarCat/3D--Engine/issues";

	//Init Windows 
	config = new WinConfig(App, true);
	element = new WinElem(App, true);
	test = new WinTest(App, true);
	objects = new WinObjects(App, true);
	cam = new WinCamera(App, true);
	scene = new WinSceneLoader(App, false);
	time = new 	WinTimeManager(App, true);
	assets = new WinAssets(App, false);


	AddWindow(config);
	AddWindow(element);
	AddWindow(console);
	AddWindow(test);
	AddWindow(objects);
	AddWindow(cam);
	AddWindow(scene);
	AddWindow(time);
	AddWindow(assets);

	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
		(*item)->Start();
	}


	//Set Guizmo Snap size
	guizmoSnapSize[0] = 1.0f;
	guizmoSnapSize[1] = 1.0f;
	guizmoSnapSize[2] = 1.0f;


	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();


	UpdateDockSpace();

	if (ImGui::IsMouseHoveringAnyWindow)
		inWindow = true;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				//App->SaveGame();
				scene->enabled = true;
				scene->save = true;
			}


			if (ImGui::MenuItem("Load"))
			{
				//App->LoadGame();
				scene->enabled = true;
				scene->save = false;
			}

			if (ImGui::MenuItem("Clear"))
			{
				SceneLoader::CleanScene();
			}

			if (ImGui::MenuItem("Quit"))
				quit = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Config", NULL, &config->enabled);
			ImGui::MenuItem("Console", NULL, &console->enabled);
			ImGui::MenuItem("Element", NULL, &element->enabled);
			ImGui::MenuItem("Objects", NULL, &objects->enabled);
			ImGui::MenuItem("Test", NULL, &test->enabled);
			if (ImGui::MenuItem("Show/Hide All"))
			{
				config->enabled = showAll;
				console->enabled = showAll;
				element->enabled = showAll;
				objects->enabled = showAll;
				test->enabled = showAll;
				showAll = !showAll;
			}

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


		ImGui::EndMainMenuBar();
	}

	if (showdemo)
		ImGui::ShowDemoWindow();
	if (about)
		AboutWindow();


	if (!App->timeManager->GetPlaying() && App->renderer3D->IsUsingGhostCam())
		DrawGuizmo(App->scene->selectedObj);

	return UPDATE_CONTINUE;
}



update_status ModuleGui::Update(float dt)
{
	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
		if ((*item)->GetEnable())
			(*item)->Update();
	}
	return UPDATE_CONTINUE;
}


update_status ModuleGui::PostUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;


	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
			(*item)->PostUpdate();
	}



	if (!ImGui::IsMouseHoveringAnyWindow)
		inWindow = false;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());



	if (quit)
		status = UPDATE_STOP;

	return status;
}

bool ModuleGui::CleanUp()
{

	VSLOG("Cleaning UP IMGUI Module")

	App->logAvaliable = false;
	
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();


	for (std::list<WinBase*>::iterator item = windows.begin(); item != windows.end(); item++) {
		delete *item;
		*item = nullptr;
	}

	windows.clear();

	return true;
}

void ModuleGui::AddWindow(WinBase* win)
{
	windows.push_back(win);
}



void ModuleGui::AboutWindow()
{
	SDL_version version;
	SDL_GetVersion(&version);

	ImGui::Begin("About");

	ImGui::Text("Mehgine 0.1\n\n\nEngine made for 3rd year University subject.\n\nBy Pol Carrera and Genis Bayo.\n\nUsing:\nSDL %d.%d.%d\nImGUI %s\npcg-c-basic-0.9\nDevIL 1.8\nParson\nMathGeoLib 1.5\nmmgr\nOpenGL %s\nAssimp 3.1.1\nMIT License (See 'LICENSE' for more information).", version.major, version.minor, version.patch, ImGui::GetVersion(), glGetString(GL_VERSION));

	ImGui::End();
}


void ModuleGui::UpdateDockSpace()
{
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;



	if (opt_flags & ImGuiDockNodeFlags_RenderWindowBg)
		ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	bool b = true;
	ImGui::Begin("DockSpace Demo", &b, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);


	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	opt_flags |= ImGuiDockNodeFlags_PassthruInEmptyNodes;
	opt_flags |= ImGuiDockNodeFlags_RenderWindowBg;



	ImGui::End();
}



void ModuleGui::ReadInput(SDL_Event * e) const
{
	ImGui_ImplSDL2_ProcessEvent(e);
}

void ModuleGui::DrawGuizmo(GameObject * obj)
{
	if (obj == nullptr) return;


	//Set up Guizmo
	ImGuizmo::BeginFrame();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	//Check Input
	UpdateGuizmoInput();

	//Calculate Matrices
	float4x4 cameraView, cameraProjection, objectMat;

	cameraView = App->scene->GetGhostCam()->GetViewMatrixF();
	cameraProjection = App->scene->GetGhostCam()->GetPerspMatrixF();

	objectMat = obj->transform->globalMartix.Transposed();

	ImGuizmo::Manipulate((float*)cameraView.v, (float*)cameraProjection.v, guizmoOperation, guizmoMode, (float*)objectMat.v, NULL, guizmoSnap ? guizmoSnapSize : NULL);


	if (ImGuizmo::IsUsing())
	{

	//Calculate the transformation in local space
	float4x4 transform = obj->transform->globalMartix.Inverted() * objectMat.Transposed();

	//Check if the guizmo cap didn't worked
	float4x4 checkMat = obj->transform->localMartix * transform;
	if (checkMat.GetScale().Equals(float3::zero))
	{
		checkMat.RemoveScale();
		VSLOG("\nError scaling with guizmo, the scale has been reset");
	}


	//Apply tranfromation
	obj->transform->localMartix = checkMat;
	obj->transform->CalcVectors();
	obj->CalcGlobalTransform();
	
	


	}


}


void ModuleGui::UpdateGuizmoInput()
{
	guizmoSnap = false;

	//Scan Inputs
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) guizmoOperation = ImGuizmo::TRANSLATE;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) guizmoOperation = ImGuizmo::ROTATE;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) guizmoOperation = ImGuizmo::SCALE;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) guizmoMode == ImGuizmo::WORLD ? guizmoMode = ImGuizmo::LOCAL : guizmoMode = ImGuizmo::WORLD;

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) 
		guizmoSnap = true;
}

bool ModuleGui::InWindow() const
{
	return inWindow;
}
