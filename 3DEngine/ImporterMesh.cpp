#include "ImporterMesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"


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
		for (int nm = 0; nm < scene->mNumMeshes; nm++)
		{
			GameObject* newobj = new GameObject();
			bool error = false;
			ResMesh* mesh = new ResMesh;
			aiMesh* m = scene->mMeshes[nm];
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
				//memcpy(mesh->textC, m->mTextureCoords, sizeof(float) * mesh->num_textC);
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
				mesh->name = "Untitlied mesh";
			}



			if (!error)
			{
				mesh->GenerateBuffer();
				newobj->AddCompMesh(mesh);
				newobj->SetName(App->loader->GetFileName(path));
				App->scene->AddGameObject(newobj);
				//App->renderer3D->meshes.push_back(mesh);

			}



			VSLOG("\nAdded mesh with %d ", mesh->num_vertex);
			VSLOG(" vertices, %d", mesh->num_indice);
			VSLOG(" indices and &d", mesh->num_normals);
			VSLOG(" normals");

			delete mesh;
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



void LogAssimp(const char* c1, char* c2)
{
	VSLOG("%s", c1);
}