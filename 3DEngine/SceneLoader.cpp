#include "Application.h"
#include "SceneLoader.h"
#include "JsonDoc.h"
#include "GameObject.h"
#include "ComponentCamera.h"


JsonDoc SceneLoader::LoadScene(const char* path, GameObject* root)
{
	JsonDoc doc;
	if (!doc.Init(path))
	{
		VSLOG("Cannot load scene");
		return doc;
	}
	else
	{

		JSON_Object* mainobj = doc.GetObj("Properties");
		App->scene->SetName(json_object_dotget_string(mainobj, "Name"));

		JSON_Object* ghostcamobj = doc.GetObj("Ghost Cam");
		App->scene->GetGhostCam()->Save(ghostcamobj, &doc);

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
	}

	root->CalcGlobalTransform();
	return doc;
}

JsonDoc* SceneLoader::SaveScene(const char* path, GameObject* root)
{
	JsonDoc* doc = new JsonDoc;
	
	if (!doc->Init(path))
	{
		VSLOG("Cannot save scene");
		return nullptr;
	}

	JSON_Object* mainobj = doc->SetObj(doc->GetRootObj(), "Properties");
	json_object_dotset_string(mainobj, "Name", App->scene->GetName());

	JSON_Object* ghostcamobj = doc->SetObj(mainobj, "Ghost Cam");
	App->scene->GetGhostCam()->Save(ghostcamobj, doc);


	JSON_Array* obj = doc->SetArray(doc->GetRootObj(), "Objects");
	
	App->renderer3D->SaveMeshes();
	root->Save(obj, doc);


	doc->Save();
	return doc;
}