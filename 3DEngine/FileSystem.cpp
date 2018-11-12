#include "FileSystem.h"
#include "SDL/include/SDL.h"

#include <fstream>
#include <direct.h>

#include "mmgr/mmgr.h"

FileSystem::FileSystem()
{

	CreateDir("Assets/");
	CreateDir("Libraries/");

	CreateDir("Libraries/Meshes/");
	CreateDir("Libraries/Scenes/");
	CreateDir("Libraries/Textures/");

}


FileSystem::~FileSystem()
{
}


uint FileSystem::LoadFile(const char* file, char** buffer)
{
	uint ret = 0;

		//VSLOG("Error opening %s, %s", file, PHYSFS_getLastError());
		std::ifstream dataFile(file);

		if (dataFile.good())
		{
			dataFile.seekg(0, dataFile.end);
			std::streamsize length = dataFile.tellg();
			dataFile.seekg(0, dataFile.beg);
			*buffer = new char[length];

			dataFile.read(*buffer, length);
			dataFile.close();

			ret = length;
		}
		else
			VSLOG("\nError loading file");

	return ret;
}

uint FileSystem::SaveFile(const char* file, char* buffer, unsigned int size) const
{

		std::ofstream dataFile(file, std::fstream::out | std::fstream::binary);
		dataFile.write(buffer, size);
		dataFile.close();

	return 0;
}

bool FileSystem::fileExists(const char* file) const {


	std::ifstream infile(file);

	return infile.good();

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

void FileSystem::Copy(const char* oldPath, const char* newPath)
{
	char *data;
	uint size = LoadFile(oldPath, &data);

	if (size > 0)
		SaveFile(newPath, data, size);
	else
		VSLOG("Cannot copy file %s to %s", oldPath, newPath);
}

void FileSystem::CreateDir(const char * path)
{
		mkdir(path);

}

std::string FileSystem::GetDirectory()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}


void FileSystem::GetDirDir(std::string dir)
{

	bool point = false;


	for (std::string::reverse_iterator rit = dir.rbegin(); rit != dir.rend(); ++rit)
	{
		if (*rit == '/' || *rit == '\\')
			break;


	}
}
