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
	
	App->camera->Move(vec3(0.0f, 5.0f, 5.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	currentTex = App->loader->LoadTex("Assets/Baker_house.png");
	App->loader->LoadScene("Assets/BakerHouse.fbx");

	App->renderer3D->SetMeshesTex(currentTex);

	App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//LOG("Unloading scene");


	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

