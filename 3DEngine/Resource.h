#ifndef __RESOURCE_H__
#define __RESOURCE_H__
#include "Globals.h"


enum ResType 
{
	MESH,
	TEXTURE,
	RESNULL,
};


class Resource
{
public:
	Resource();
	~Resource();
	uint32_t GetUUID();
	const char* GetName();
	void SetName(const char* n);
	void SetName(std::string n);

protected:

	uint32_t uuid = 0;
	std::string name;
	std::string path;
	ResType type = RESNULL;
};

#endif // !__RESOURCE_H__