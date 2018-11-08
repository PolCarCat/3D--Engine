#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__


class JsonDoc;
class GameObject;

namespace SceneLoader
{


	void LoadScene(const char* path, GameObject* root);
	void LoadScene(JsonDoc doc);
	void SaveScene(const char* path, GameObject* root);
	JsonDoc SerializeScene(GameObject* root);
	JsonDoc SerializeScene();
	void CleanScene();

}


#endif // __SCENELOADER_H__
