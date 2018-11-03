#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__


class JsonDoc;
class GameObject;

namespace SceneLoader
{


	JsonDoc LoadScene(const char* path, GameObject* root);
	JsonDoc* SaveScene(const char* path, GameObject* root);

}


#endif // __SCENELOADER_H__
