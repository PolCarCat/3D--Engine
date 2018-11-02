#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__
#include "Globals.h"



class FileSystem
{
public:
	FileSystem();
	~FileSystem();	
	bool fileExists(const char* file) const;
	uint LoadFile(const char* file, char** buffer);
	uint SaveFile(const char* file, char* buffer, unsigned int size) const;

	void AddPath(const char* path, const char* mount_point = nullptr);
	void InvertBars(const char* path);
	void InvertBars(std::string& str);

private:
	char* basePath = NULL;
	char* write_path = NULL;
};


#endif // !__FILESYSTEM_H__