#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

#include "Importer.h"
#include "Globals.h"

#define INDICES_CAP 200000

class GameObject;

class ImporterMesh :
	public Importer
{
public:
	ImporterMesh();
	~ImporterMesh();

	//bool Load();
	//bool Save();
	bool Start();
	bool CleanUp();

	GameObject* LoadScene(const char* path);


};


void LogAssimp(const char* c1, char* c2);

#endif // !__IMPORTERMESH_H__