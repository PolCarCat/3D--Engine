#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Primitive.h"
#include "ModuleLoader.h"
#include "ComponentCamera.h"

//TEST
#include "ImGui/imgui.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{


}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	//VLOG("Loading scene assets");
	bool ret = true;
	


	//currentTex = App->loader->LoadTex("Assets/Baker_house.png");
	App->loader->LoadScene("Assets/BakerHouse.fbx");

	ghostcam = new ComponentCamera(0.5f, 512.0f, 50.0f);
	ghostcam->Start();
	currentCam = ghostcam;

	App->renderer3D->OnResize(App->window->w, App->window->h);

	if (App->renderer3D->meshes.size() != 0)
	{
		App->renderer3D->SetMeshesTex(currentTex);
		App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());
		App->camera->FocusMeshes();
	}

	root.Start();
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//LOG("Unloading scene");
	root.CleanUp();

	if (currentCam != ghostcam)
		delete currentCam;

	delete ghostcam;



	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	ghostcam->Update();
	root.Update();

	//TEST
	ImGui::Begin("Test camera");

	ImGui::SliderFloat("FOV", &currentCam->fovy, 0, 100);

	ImGui::End();
	App->renderer3D->OnResize(App->window->w, App->window->h);
	return UPDATE_CONTINUE;
}


void ModuleScene::AddGameObject(GameObject* obj)
{

	obj->SetParent(&root);
	
}

void ModuleScene::MoveCurCam()
{

}

ComponentCamera* ModuleScene::GetCurCam()
{
	return currentCam;
}