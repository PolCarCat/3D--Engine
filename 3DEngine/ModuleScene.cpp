#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Primitive.h"
#include "ModuleLoader.h"


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
	
	App->camera->Move(float3(0.0f, 5.0f, 5.0f));
	App->camera->LookAt(float3(0, 0, 0));

	//currentTex = App->loader->LoadTex("Assets/Baker_house.png");
	App->loader->LoadScene("Assets/BakerHouse.fbx");



	if (App->renderer3D->meshes.size() != 0)
	{
		App->renderer3D->SetMeshesTex(currentTex);
		App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());
		App->camera->FocusMeshes();
	}

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//LOG("Unloading scene");
	root.CleanUp();

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{

	root.Update();
	return UPDATE_CONTINUE;
}


void ModuleScene::AddGameObject(GameObject* obj)
{
	obj->SetParent(&root);
	
}

void ModuleScene::MoveCurCam()
{

}