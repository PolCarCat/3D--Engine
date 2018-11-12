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
	void Copy(const char* oldPath, const char* newPath);
	void CreateDir(const char* path);
	void GetDirDir(std::string dir);
	std::string GetDirectory();
	
	void InvertBars(char* path);
	void InvertBars(std::string& str);

private:
	char* basePath = NULL;
	char* write_path = NULL;
};


#endif // !__FILESYSTEM_H__