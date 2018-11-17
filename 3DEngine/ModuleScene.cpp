#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Primitive.h"
#include "ModuleLoader.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "SceneLoader.h"
#include "mmgr/mmgr.h"

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
	//App->loader->LoadFBX("Assets/BakerHouse.fbx");

	//GameObject* bk1 = new GameObject();
	//bk1->AddCompMesh(App->loader->meshImporter.LoadMeh("Untitled"));
	//AddGameObject(bk1);

	ghostcam = new ComponentCamera(0.5f, FPD, 50.0f);
	ghostcam->Start();
	ghostcam->drawFrustum = false;
	currentCam = ghostcam;

	GameObject* camobj = new GameObject();
	camobj->SetName("Camera");

	camobj->AddCompCam();
	AddGameObject(camobj);

	App->renderer3D->OnResize(App->window->w, App->window->h);

	//App->loader->meshImporter.LoadScene("Assets/street/Street environment_V01.fbx");

	root.Start();
	root.SetName("root");
	//quadTree.TEST();
	return ret;
}



// Update
update_status ModuleScene::Update(float dt)
{


	ghostcam->CheckInput(dt);
	ghostcam->Update();
	//root.CalcGlobalTransform();
	root.Update();
	quadTree.Draw();

	App->renderer3D->OnResize(App->window->w, App->window->h);
	return UPDATE_CONTINUE;
}


update_status ModuleScene::PostUpdate(float dt)
{

	root.PostUpdate();

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleScene::CleanUp()
{
	VSLOG("Unloading scene");
	SceneLoader::CleanScene();

	ghostcam->CleanUp();
	if (currentCam != ghostcam)
		delete currentCam;

	delete ghostcam;
	ghostcam = nullptr;

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
	obj->SetName("New Object");
	obj->SetParent(&root);
}

void ModuleScene::CreateMaterial()
{
	ComponentMaterial* mat = new ComponentMaterial();
	mat->SetName("New Material");
	materials.push_back(mat);
}

ComponentMaterial* ModuleScene::CheckMaterial(const char* name)
{
	if (std::string(name) == "")
		return nullptr;

	for (std::list<ComponentMaterial*>::iterator item = materials.begin(); item != materials.end(); item++)
	{
		if (std::string((*item)->GetName()) == name)
		{
			return *item;
		}
	}

	return nullptr;
}

ComponentCamera* ModuleScene::GetCurCam() const
{
	return currentCam;
}

ComponentCamera* ModuleScene::GetGhostCam() const
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


const char*  ModuleScene::GetName() const
{
	return name.c_str();
}

void  ModuleScene::SetName(const char* n)
{
	name = n;
}