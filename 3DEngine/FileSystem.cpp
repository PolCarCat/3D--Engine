#include "FileSystem.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"
#include <fstream>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")


FileSystem::FileSystem()
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	if (PHYSFS_setWriteDir(".") == 0)
		VSLOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	AddPath(".");


	if (!PHYSFS_exists("Libraries/"))
		PHYSFS_mkdir("Libraries/");

	AddPath("Libraries/");


	if (!PHYSFS_exists("Libraries/Meshes/"))
		PHYSFS_mkdir("Libraries/Meshes/");

	AddPath("Libraries/Meshes/");


	if (!PHYSFS_exists("Libraries/Textures/"))
		PHYSFS_mkdir("Libraries/Textures/");


	AddPath("Libaries/Textures/");

	if (!PHYSFS_exists("Libraries/Scenes/"))
		PHYSFS_mkdir("Libraries/Scenes/");

	AddPath("Libraries/Scenes/");
	
}


FileSystem::~FileSystem()
{
	PHYSFS_deinit();
}


uint FileSystem::LoadFile(const char* file, char** buffer)
{
	uint ret = 0;
	PHYSFS_file* filefs = PHYSFS_openRead(file);

	if (filefs != nullptr) 
	{
		int size = PHYSFS_fileLength(filefs);
		if (size > 0) 
		{
			*buffer = new char[size];
			if (size != PHYSFS_readBytes(filefs, *buffer, size))
			{
				VSLOG("Error reading %s, &s", file, PHYSFS_getLastError());
				delete buffer;
				buffer = nullptr;
			}
			else
				ret = size;

		}

		PHYSFS_close(filefs);
	}
	else
		VSLOG("Error opening %s, %s", file, PHYSFS_getLastError());

	return ret;
}

uint FileSystem::SaveFile(const char* file, char* buffer, unsigned int size) const
{
	uint ret = 0;
	PHYSFS_file* filefs = PHYSFS_openWrite(file);

	if (filefs == nullptr)
	{
		std::ofstream dataFile(file, std::fstream::out | std::fstream::binary);
		dataFile.write(buffer, size);
		dataFile.close();

		//VSLOG("\nCreated new file %s", file);
	}
	else
	{
		if (size == PHYSFS_writeBytes(filefs, (const void*)buffer, size))
			ret = size;


		PHYSFS_close(filefs);
	}

	

	return ret;
}

bool FileSystem::fileExists(const char* file) const {
	//Excuse me wtf
	//std::string path = file;
	//InvertBars(path);

	return PHYSFS_exists(file);
}

void FileSystem::AddPath(const char* path, const char* mount_point)
{
	if (PHYSFS_mount(path, mount_point, 1) == 0)
	{

		VSLOG("Error adding %s, %s", path, PHYSFS_getLastError());
		
	}
		
}

void FileSystem::InvertBars(char* path)
{
	uint size = strlen(path);
	for (int i = 0; i < size; ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
	}

}

void FileSystem::InvertBars(std::string& str)
{
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] == '\\')
			str[i] = '/';
	}
}

