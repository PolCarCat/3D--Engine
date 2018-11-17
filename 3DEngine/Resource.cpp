#include "Resource.h"
#include "pcg-c-basic-0.9/pcg_basic.h"

Resource::Resource(uint32_t UUID, ResType _type)
{
	if (UUID == 0)
		uuid = pcg32_random();
	else
		uuid = UUID;

	type = _type;

	//It's initializes as 1 because it's avoided doing Init() just in the creation
	inMemory = 1;
}

Resource::~Resource()
{
}

void Resource::CleanUp()
{
}

void Resource::Init()
{
}

uint32_t Resource::GetUUID() const
{
	return uuid;
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

void Resource::SetUuid(uint32_t d)
{
	uuid = d;
}

void Resource::SetFile(const char* f)
{
	file = f;
}

void Resource::SetFile(std::string f)
{
	file = f;
}

void Resource::SetExportedFile(const char * f)
{
	exportedFile = file;
}

void Resource::SetExportedFile(std::string f)
{
	exportedFile = f;
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

void Resource::AddInMemory()
{
	inMemory++;

	//If is the first element uisng it we reload the resource
	if (inMemory == 1)
		Init();
}

void Resource::SubstractInMemory()
{
	inMemory--;

	//If there isn't a element that uses this resource it's deleted form memory
	if (inMemory == 0)
		CleanUp();
}
