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

	bool ret = true;
	

	//Generate Ghost Cam
	ghostcam = new ComponentCamera(0.5f, FPD, 50.0f);
	ghostcam->Start();
	ghostcam->drawFrustum = false;
	currentCam = ghostcam;

	GameObject* camobj = new GameObject();
	camobj->SetName("Camera");
	

	camobj->AddCompCam();
	AddGameObject(camobj);

	App->renderer3D->OnResize(App->window->w, App->window->h);

	//App->loader->meshImporter.LoadScene("Assets/TransparentScene/TransparentScene.fbx");

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


	root.Update();
	quadTree.Draw();

	App->renderer3D->OnResize(App->window->w, App->window->h);

	UpdateShorcuts();

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

	quadTree.CleanUp();

	for (std::list<ComponentMaterial*>::iterator item = materials.begin(); item != materials.end(); item++)
	{
	(*item)->CleanUp();
	if (*item != nullptr)
	{
		delete *item;
		*item = nullptr;
	}

	item = materials.erase(item);
	}

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


void ModuleScene::CreateMaterial(ResTexture * tex)
{
	ComponentMaterial* mat = new ComponentMaterial();
	if (tex != nullptr)
	{
		mat->SetName(tex->GetName());
		mat->SetTexture(tex);
		mat->texEnabled = true;
	}
	else
	{
		mat->SetName("New Material");
	}

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

void ModuleScene::UpdateShorcuts()
{
	//Scene Main Shortcuts

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		//Save
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			App->imgui->scene->enabled = true;
			App->imgui->scene->save = true;
		}

		//Load
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			App->imgui->scene->enabled = true;
			App->imgui->scene->save = false;
		}

		//Clean
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
			SceneLoader::CleanScene();

		//Copy
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			selectedObj->Copy();
	}

	//Delete
	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		selectedObj->Delete();
}
