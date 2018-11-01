#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__


class JsonDoc;
class GameObject;

namespace SceneLoader
{


	void LoadScene(const char* path, GameObject* root, JsonDoc* doc);
	void SaveScene(const char* path, GameObject* root, JsonDoc* doc);
	void SaveGameObj(GameObject* obj, JSON_Object* doc);
}


#endif // __SCENELOADER_H__
