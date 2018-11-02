#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

#include "Importer.h"
#include "Globals.h"

#define DIR "Libraries\\Meshes\\"
#define MESH_EXTENSION ".meh"

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
	GameObject* LoadMesh(aiMesh* m);
	void SaveMeshAsMeh(ResMesh* m);
	ResMesh* LoadMeh(const char* name);
};


void LogAssimp(const char* c1, char* c2);

#endif // !__IMPORTERMESH_H__