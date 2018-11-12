#ifndef __RESOURCE_H__
#define __RESOURCE_H__
#include "Globals.h"


enum ResType 
{
	RESMESH,
	RESTEXTURE,
	RESNULL,
};


class Resource
{
public:
	Resource(uint32_t UUID = 0, ResType _type = RESNULL);
	virtual ~Resource();
	uint32_t GetUUID() const;
	const char* GetName() const;
	void SetName(const char* n);
	void SetName(std::string n);
	ResType GetType() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	uint InMemory() const;
	void AddInMemory();
	void SubstractInMemory();

protected:

	uint32_t uuid = 0;
	std::string name;
	std::string exportedFile;
	std::string file;
	ResType type = RESNULL;

	uint inMemory = 0;
};

#endif // !__RESOURCE_H__