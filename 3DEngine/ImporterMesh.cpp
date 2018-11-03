#include "ImporterMesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include <experimental/filesystem>
#include <fstream>

#include "SDL/include/SDL.h"
#include "Assimp/include/postprocess.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


ImporterMesh::ImporterMesh()
{
}


ImporterMesh::~ImporterMesh()
{
}

bool ImporterMesh::Start()
{
	//Init Assimp

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogAssimp;
	aiAttachLogStream(&stream);


	return true;
}

bool ImporterMesh::CleanUp()
{


	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


GameObject* ImporterMesh::LoadScene(const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	GameObject* newobj = nullptr;
	
	if (scene != nullptr && scene->HasMeshes())
	{
		uint i = 0;
		aiNode* root = scene->mRootNode;

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scaling;
		root->mTransformation.Decompose(scaling, rotation, position);

		newobj = new GameObject();
		App->scene->AddGameObject(newobj);

		newobj->SetName(App->loader->GetFileName(path));
		newobj->transform->position.Set(position.x, position.y, position.z);
		newobj->transform->scale.Set(scaling.x, scaling.y, scaling.z);
		newobj->transform->rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);


		for (uint nm = 0; nm < scene->mNumMeshes; nm++)
		{		
			aiMesh* m = scene->mMeshes[nm];
			aiNode* n = root->mChildren[nm];
			GameObject* obj = LoadMesh(m);
			if (obj != nullptr)
				obj->SetParent(newobj);
		}

		aiReleaseImport(scene);
	}
	else
	{
		VSLOG("Error loading scene %s", path);
		ret = false;

	}

	return newobj;
}


GameObject* ImporterMesh::LoadMesh(aiMesh* m)
{
	GameObject* newobj = new GameObject();
	bool error = false;

	
	ResMesh* mesh = new ResMesh;
	mesh->num_vertex = m->mNumVertices;
	mesh->vertex = new float[mesh->num_vertex * 3];
	memcpy(mesh->vertex, m->mVertices, sizeof(float) * mesh->num_vertex * 3);

	float minx = mesh->vertex[0];
	float maxx = mesh->vertex[0];
	float miny = mesh->vertex[1];
	float maxy = mesh->vertex[1];
	float minz = mesh->vertex[2];
	float maxz = mesh->vertex[2];

	for (uint i = 0; i < mesh->num_vertex * 3; i += 3)
	{
		if (mesh->vertex[i] < minx)
			minx = mesh->vertex[i];
		else if (mesh->vertex[i] > maxx)
			maxx = mesh->vertex[i];

		if (mesh->vertex[i + 1] < miny)
			miny = mesh->vertex[i + 1];
		else if (mesh->vertex[i + 1] > maxy)
			maxy = mesh->vertex[i + 1];

		if (mesh->vertex[i + 2] < minz)
			minz = mesh->vertex[i + 2];
		else if (mesh->vertex[i + 2] > maxz)
			maxz = mesh->vertex[i + 2];
	}

	mesh->boundingBox.maxPoint = { maxx, maxy, maxz };
	mesh->boundingBox.minPoint = { minx, miny, minz };

	newobj->AddBox(mesh->boundingBox);
	if (m->HasFaces())
	{
		mesh->num_indice = m->mNumFaces * 3;
		mesh->indice = new uint[mesh->num_indice]; // assume each face is a triangle

		if (mesh->num_indice > INDICES_CAP)
		{
			VSLOG("\nWARNING can not load a mesh with %d indices", mesh->num_indice);
			error = true;
		}

		for (uint i = 0; i < m->mNumFaces; ++i)
		{
			if (m->mFaces[i].mNumIndices != 3)
			{
				VSLOG("\nWARNING, geometry face with != 3 indices!");
				error = true;
			}
			else
				memcpy(&mesh->indice[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));


		}
	}

	if (m->HasNormals())
	{
		mesh->num_normals = m->mNumVertices * 3;
		mesh->normals = new float[mesh->num_normals * 3];
		memcpy(mesh->normals, m->mNormals, sizeof(float) * mesh->num_normals);
	}

	if (m->GetNumColorChannels() != 0)
	{
		mesh->num_colors = m->mNumVertices;
		mesh->colors = new float[mesh->num_colors];
		memcpy(mesh->colors, m->mColors, sizeof(float) * mesh->num_colors);
	}

	if (m->GetNumUVChannels() > 0)
	{
		mesh->num_textC = m->mNumVertices * 2;
		mesh->textC = new float[mesh->num_textC];
		
		uint j = 0;
		for (uint i = 0; i < mesh->num_textC; i += 2)
		{
			mesh->textC[i] = m->mTextureCoords[0][j].x;
			mesh->textC[i + 1] = m->mTextureCoords[0][j].y;
			j++;
		}
	}

	mesh->tex.id = 0;

	if (m->mName.length > 0)
	{

		mesh->name = m->mName.C_Str();
	}
	else
	{
		mesh->name = "New mesh";
	}



	if (!error)
	{
		mesh->GenerateBuffer();
		newobj->AddCompMesh(mesh);
		newobj->SetName(m->mName.C_Str());
		
		App->scene->AddGameObject(newobj);

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scaling;
		/*n->mTransformation.Decompose(scaling, rotation, position);

		newobj->transform->position.Set(position.x, position.y, position.z);
		newobj->transform->scale.Set(scaling.x, scaling.y, scaling.z);
		newobj->transform->rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);*/

		App->scene->AddGameObject(newobj);
	}
	else
	{
		newobj->CleanUp();
		delete newobj;
		newobj = nullptr;

		//delete mesh;
		//mesh = nullptr;
	}



	VSLOG("\nAdded mesh with %d ", mesh->num_vertex);
	VSLOG(" vertices, %d", mesh->num_indice);
	VSLOG(" indices and %d", mesh->num_normals);
	VSLOG(" normals");

	return newobj;
}


void ImporterMesh::SaveMeshAsMeh(ResMesh* m)
{
	uint ranges[2] = { m->num_vertex, m->num_indice };

	uint fileSize = sizeof(ranges) + sizeof(float)*m->num_vertex * 3 + sizeof(uint)*m->num_indice + sizeof(float)*m->num_vertex * 2 + sizeof(float)*m->num_vertex * 3;
	
	char* data = new char[fileSize];
	char* last = data;


	uint bytes = sizeof(ranges);
	memcpy(last, ranges, bytes);

	last += bytes;

	bytes = sizeof(float)*m->num_vertex * 3;
	memcpy(last, m->vertex, bytes);

	last += bytes;

	bytes = sizeof(uint)*m->num_indice;
	memcpy(last, m->indice, bytes);

	last += bytes;

	bytes = sizeof(float)*m->num_vertex * 2;
	memcpy(last, m->textC, bytes);

	last += bytes;

	bytes = sizeof(float)*m->num_vertex * 3;
	memcpy(last, m->normals, bytes);

	std::string str = MESH_DIR + m->name + MESH_EXTENSION;

	std::ofstream dataFile(str.c_str(), std::fstream::out | std::fstream::binary);
	dataFile.write(data, fileSize);
	dataFile.close();

	//App->fileSystem.SaveFile(str.c_str(), data, fileSize);

}

ResMesh* ImporterMesh::LoadMeh(const char* name)
{
	ResMesh* mesh = new ResMesh();

	std::string str = MESH_DIR + std::string(name) + MESH_EXTENSION;
	App->fileSystem.InvertBars(str);
	//std::ifstream dataFile(str.c_str(), std::fstream::binary);

	//// get length of file:
	//int length = dataFile.tellg();


	//if (length <= 0)
	//{
	//	VSLOG("Error Loading %s", str.c_str());
	//	dataFile.close();
	//	return nullptr;
	//}
	char* data;
	App->fileSystem.LoadFile(str.c_str(), &data);
	

	char* last = data;
	uint ranges[2];
	uint bytes = sizeof(ranges);

	memcpy(ranges, last, bytes);

	mesh->num_vertex = ranges[0];
	mesh->num_indice = ranges[1];

	last += bytes;
	bytes = sizeof(float)*mesh->num_vertex * 3;

	mesh->vertex = new float[mesh->num_vertex * 3];
	memcpy(mesh->vertex, last, bytes);

	last += bytes;
	bytes = sizeof(uint)*mesh->num_indice;

	mesh->indice = new uint[mesh->num_indice];
	memcpy(mesh->indice, last, bytes);

	last += bytes;
	bytes = sizeof(float)*mesh->num_vertex * 2;

	mesh->textC = new float[mesh->num_vertex * 2];
	memcpy(mesh->textC, last, bytes);

	last += bytes;
	bytes = sizeof(float)*mesh->num_vertex * 3;

	mesh->normals = new float[mesh->num_vertex * 3];
	memcpy(mesh->normals, last, bytes);

	mesh->GenerateBuffer();
	return mesh;
}

void LogAssimp(const char* c1, char* c2)
{
	VSLOG("%s", c1);
}