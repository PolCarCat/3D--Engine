#include "Application.h"
#include "SceneLoader.h"
#include "JsonDoc.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"

#include "mmgr/mmgr.h"


void SceneLoader::LoadScene(const char* path, GameObject* root)
{
	//Inits a new document
	JsonDoc doc;
	if (!doc.Init(path))
	{
		VSLOG("Cannot load scene");
	}
	else
	{
		//Saves the items that aren't in the game object hierachy
		JSON_Object* mainobj = doc.GetObj("Properties");
		App->scene->SetName(json_object_dotget_string(mainobj, "Name"));

		JSON_Object* ghostcamobj = doc.GetObj("Ghost Cam");
		App->scene->GetGhostCam()->Load(ghostcamobj, &doc);

		JSON_Array* objects = doc.GetAr("Objects");
		uint i = 0;
		JSON_Object* item = doc.GetArObj(objects, i);
		while (item != nullptr)
		{
		
			GameObject* obj = new GameObject();
			if (obj->Load(item, &doc))
			{
				delete obj;
				obj = nullptr;
			}

			item = doc.GetArObj(objects, ++i);
		}
		doc.CleanUp();
	}

	root->CalcGlobalTransform();

}

void SceneLoader::LoadScene(JsonDoc doc)
{

	if (doc.GetRootObj() == nullptr)
	{
		VSLOG("Cannot load scene");
	}
	else
	{

		JSON_Object* mainobj = doc.GetObj("Properties");
		App->scene->SetName(json_object_dotget_string(mainobj, "Name"));

		JSON_Object* ghostcamobj = doc.GetObj("Ghost Cam");
		App->scene->GetGhostCam()->Load(ghostcamobj, &doc);

		JSON_Array* objects = doc.GetAr("Objects");
		uint i = 0;
		JSON_Object* item = doc.GetArObj(objects, i);
		while (item != nullptr)
		{

			GameObject* obj = new GameObject();
			if (obj->Load(item, &doc))
			{
				delete obj;
				obj = nullptr;
			}

			item = doc.GetArObj(objects, ++i);
		}
		App->scene->root.CalcGlobalTransform();
		//doc.CleanUp();
	}

	//root->CalcGlobalTransform();
}

void SceneLoader::SaveScene(const char* path, GameObject* root)
{
	JsonDoc doc;
	
	if (!doc.Init(path))
	{
		VSLOG("Cannot save scene");
	}
	else
	{
		JSON_Object* mainobj = doc.SetObj(doc.GetRootObj(), "Properties");
		json_object_dotset_string(mainobj, "Name", App->scene->GetName());

		JSON_Object* ghostcamobj = doc.SetObj(mainobj, "Ghost Cam");
		App->scene->GetGhostCam()->Save(ghostcamobj, &doc);


		JSON_Array* obj = doc.SetArray(doc.GetRootObj(), "Objects");

		root->Save(obj, &doc);


		doc.Save();
		doc.CleanUp();
	}


}

JsonDoc SceneLoader::SerializeScene(GameObject * root)
{
	//Does the same than the save but this dont actually save a file, it keeps it in memory
	JsonDoc doc;

	if (!doc.Init())
	{
		VSLOG("Cannot init scene");
	}
	else
	{
		JSON_Object* mainobj = doc.SetObj(doc.GetRootObj(), "Properties");
		json_object_dotset_string(mainobj, "Name", App->scene->GetName());

		JSON_Object* ghostcamobj = doc.SetObj(mainobj, "Ghost Cam");
		App->scene->GetGhostCam()->Save(ghostcamobj, &doc);


		JSON_Array* obj = doc.SetArray(doc.GetRootObj(), "Objects");

		root->Save(obj, &doc);

	}

	return doc;
}


JsonDoc SceneLoader::SerializeScene()
{
	//Without specifying saves the root obj 

	return SerializeScene(&App->scene->root);
}
void SceneLoader::CleanScene()
{
	App->scene->selectedObj = nullptr;
	App->scene->selectedMat = nullptr;
	App->scene->SetCurCam(App->scene->GetGhostCam());
	App->scene->root.CleanUp();

	//Cleaning materials
	for (std::list<ComponentMaterial*>::iterator item = App->scene->materials.begin(); item != App->scene->materials.end(); item++)
	{
		(*item)->CleanUp();
		if (*item != nullptr)
		{
			delete *item;
			*item = nullptr;
		}
	}
	App->scene->materials.clear();

}