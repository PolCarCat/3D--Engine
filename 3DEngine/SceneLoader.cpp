#include "Application.h"
#include "SceneLoader.h"
#include "JsonDoc.h"
#include "GameObject.h"


void SceneLoader::LoadScene(const char* path, GameObject* root, JsonDoc* doc)
{
	if (!doc->Init(path))
	{
		VSLOG("Cannot load scene");
		return;
	}


}

JsonDoc* SceneLoader::SaveScene(const char* path, GameObject* root)
{
	JsonDoc* doc = new JsonDoc;
	
	if (!doc->Init(path))
	{
		VSLOG("Cannot save scene");
		return nullptr;
	}

	JSON_Array* obj = doc->SetArray(doc->GetRootObj(), "Objects");
	
	root->Save(obj, doc);


	doc->Save();
	return doc;
}