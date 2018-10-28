#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Primitive.h"
#include "ModuleLoader.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"

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
	ghostcam->drawFrustum = false;
	currentCam = ghostcam;

	GameObject* camobj = new GameObject();
	camobj->SetName("Camera");

	camobj->AddCompCam();
	//AddGameObject(camobj);

	App->renderer3D->OnResize(App->window->w, App->window->h);

	if (App->renderer3D->meshes.size() != 0)
	{
		//App->renderer3D->SetMeshesTex(currentTex);
		//App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());
		App->camera->FocusMeshes();
	}

	root.Start();
	//quadTree.TEST();
	return ret;
}



// Update
update_status ModuleScene::Update(float dt)
{

	if (!currentCam->locked)
		currentCam->CheckInput(dt);

	ghostcam->Update();
	root.Update();
	quadTree.Draw();

	ImGui::Begin("TEST");
	if (ImGui::Button("Distribute tree"))
	{
		quadTree.DistributeTree();
		//quadTree.GenerateTestChildren();
	}
	ImGui::End();

	App->renderer3D->OnResize(App->window->w, App->window->h);
	return UPDATE_CONTINUE;
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

void ModuleScene::AddGameObject(GameObject* obj)
{

	obj->SetParent(&root);
	
}

void ModuleScene::DeleteGameObject(GameObject* obj)
{
	Utils::RemoveFromVector(obj, root.objChilds);
}

void ModuleScene::CreateEmptyObj()
{
	GameObject* obj = new GameObject();
	obj->SetName("Untitiled");
	obj->SetParent(&root);
}

void ModuleScene::CreateMaterial()
{
	ComponentMaterial* mat = new ComponentMaterial();
	mat->SetName("Untitiled");
	materials.push_back(mat);
}

ComponentCamera* ModuleScene::GetCurCam()
{
	return currentCam;
}

ComponentCamera* ModuleScene::GetGhostCam()
{
	return ghostcam;
}

void ModuleScene::SetCurCam(ComponentCamera* cam)
{
	if (currentCam != nullptr)
		currentCam->isCurCam = false;

	currentCam = cam;
	currentCam->isCurCam = true;
}