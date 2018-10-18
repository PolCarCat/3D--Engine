#pragma once
#include "Importer.h"
#include "Globals.h"

#define CHECKERS_SIZE 20

struct Texture;

class ImporterTexture :
	public Importer
{
public:
	ImporterTexture();
	~ImporterTexture();

	bool Load();
	bool Save();
	bool Start();
	bool CleanUp();

	uint LoadChekerTex();
	Texture LoadTex(const char* path);
};

