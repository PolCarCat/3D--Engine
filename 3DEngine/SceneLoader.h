#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__


class JsonDoc;
class GameObject;

namespace SceneLoader
{


	void LoadScene(const char* path, GameObject* root, JsonDoc* doc);
	JsonDoc* SaveScene(const char* path, GameObject* root);

}


#endif // __SCENELOADER_H__
