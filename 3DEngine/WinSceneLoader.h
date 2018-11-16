#ifndef __WINSCENELOADER_H__
#define __WINSCENELOADER_H__
#include "WinBase.h"
#include <string>

#define FOLDER "Libraries\\Scenes\\"

class WinSceneLoader :
	public WinBase
{
public:
	WinSceneLoader(Application* parent, bool start_enabled = true);
	~WinSceneLoader();

	bool Update();
	void UpdateSave();
	void UpdateLoad();

	void ObjectsInDir(bool s);
	std::string path;
	std::string selectedElem;
	bool save = false;
};

#endif //__WINSCENELOADER_H__