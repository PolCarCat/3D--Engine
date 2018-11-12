#ifndef __WINASSETS_H__
#define __WINASSETS_H__

#include "WinBase.h"
class WinAssets :
	public WinBase
{
public:
	WinAssets(Application* parent, bool start_enabled = true);
	~WinAssets();

	bool Update();
	void ObjectsInDir(std::string dir);

private:
	std::string path = "Assets/";
};

#endif // !__WINASSETS_H__