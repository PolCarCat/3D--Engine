#include "Resource.h"



Resource::Resource()
{
}


Resource::~Resource()
{
}

uint32_t Resource::GetUUID()
{
	return uint32_t();
}

const char * Resource::GetName()
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
