#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

#include "Importer.h"
#include "Globals.h"

#define INDICES_CAP 200000

class GameObject;
class ResMesh;
class aiMesh;
class aiNode;


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
	GameObject* LoadMesh(aiMesh* m, aiNode* n);

};


void LogAssimp(const char* c1, char* c2);

#endif // !__IMPORTERMESH_H__