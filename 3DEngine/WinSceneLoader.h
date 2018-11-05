#pragma once
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
	std::string GetDirectory();
	std::string path;
	std::string selectedElem;
	bool save = true;
};

