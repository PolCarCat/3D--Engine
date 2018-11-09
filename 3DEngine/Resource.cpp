#include "Resource.h"


Resource::Resource(uint32_t UUID, ResType _type)
{
	uuid = UUID;
	type = _type;
}

Resource::~Resource()
{
}

uint32_t Resource::GetUUID() const
{
	return uint32_t();
}

const char * Resource::GetName() const
{
	return name.c_str();
}

void Resource::SetName(const char * n)
{
	name = n;
}

void Resource::SetName(std::string n)
{
	name = n;
}

ResType Resource::GetType() const
{
	return type;
}

const char * Resource::GetFile() const
{
	return file.c_str();
}

const char * Resource::GetExportedFile() const
{
	return exportedFile.c_str();
}

uint Resource::InMemory() const
{
	return inMemory;
}
