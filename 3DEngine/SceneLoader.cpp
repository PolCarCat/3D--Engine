#include "Application.h"
#include "SceneLoader.h"
#include "JsonDoc.h"
#include "GameObject.h"


void SceneLoader::LoadScene(const char* path, GameObject* root, JsonDoc* doc)
{
	

}

void SceneLoader::SaveScene(const char* path, GameObject* root, JsonDoc* doc)
{
	if (!doc->Init(path))
	{
		VSLOG("Cannot load scene");
		return;
	}


}

void SceneLoader::SaveGameObj(GameObject* obj, JSON_Object* json)
{
	

}