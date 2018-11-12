#include "ImporterMesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include <experimental/filesystem>


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
	currentPath = App->loader->GetFileDir(path);

	GameObject* newobj = nullptr;
	
	if (scene != nullptr)
	{


		GameObject* newobj = new GameObject();
		newobj->SetName(App->loader->GetFileName(path));
		App->scene->AddGameObject(newobj);

		GameObject* child = LoadNode(scene->mRootNode, scene, newobj);

		aiReleaseImport(scene);
	}
	else
	{
		VSLOG("Error loading scene %s", path);
		ret = false;

	}

	currentPath = "";
	return newobj;
}

GameObject* ImporterMesh::LoadNode(aiNode* n, const aiScene* scene, GameObject* parent)
{
	GameObject* nodeobj = nullptr;


	if (n->mMetaData != nullptr)
	{
		nodeobj = new GameObject();
		nodeobj->SetName(n->mName.C_Str());
		nodeobj->SetParent(parent);

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scaling;
		n->mTransformation.Decompose(scaling, rotation, position);

		nodeobj->transform->position.Set(position.x, position.y, position.z);
		nodeobj->transform->scale.Set(scaling.x, scaling.y, scaling.z);
		nodeobj->transform->rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);
		nodeobj->transform->CalcMatrix();

		nodeobj->transform->localMartix = nodeobj->transform->localMartix * GetMatrix(mat);
		mat = aiMatrix4x4();

		if (n->mNumMeshes != 0)
		{
			for (uint i = 0; i < n->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[n->mMeshes[i]];
				ComponentMesh* meshobj = LoadMesh(mesh);

				if (meshobj != nullptr)
				{
					if (meshobj->mesh->GetName() == "")
					{
						meshobj->mesh->SetName(n->mName.C_Str());
					}

					nodeobj->AddBox(meshobj->mesh->boundingBox);
					nodeobj->AddComponent(meshobj);

					aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
					meshobj->material = LoadMat(mat);
				}

			}
		}
	}
	else
	{
		mat = mat * n->mTransformation;
	}

	if (nodeobj == nullptr)
		nodeobj = parent;

	for (uint i = 0; i < n->mNumChildren; i++)
	{
		GameObject* child = LoadNode(n->mChildren[i], scene, nodeobj);

	}

	return nodeobj;
}

ComponentMesh* ImporterMesh::LoadMesh(aiMesh* m)
{

	bool error = false;

	
	ResMesh* mesh = new ResMesh();
	mesh->num_vertex = m->mNumVertices;
	mesh->vertex = new float[mesh->num_vertex * 3];
	memcpy(mesh->vertex, m->mVertices, sizeof(float) * mesh->num_vertex * 3);


	//Create AABB
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

	//Load indices and faces
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

	//mesh->tex.id = 0;

	if (m->mName.length > 0)
	{

		mesh->SetName(m->mName.C_Str());
	}
	else
	{
		mesh->SetName("New mesh");
	}


	ComponentMesh* newcomp = nullptr;
	if (!error)
	{
		mesh->GenerateBuffer();
		mesh->SetName(m->mName.C_Str());
		App->resourceManager->AddResource(mesh);

		newcomp = new ComponentMesh(mesh);

		VSLOG("\nAdded mesh with %d ", mesh->num_vertex);
		VSLOG(" vertices, %d", mesh->num_indice);
		VSLOG(" indices and %d", mesh->num_normals);
		VSLOG(" normals");
		
	}

	return newcomp;
}

ComponentMaterial* ImporterMesh::LoadMat(aiMaterial* m)
{
	ComponentMaterial* mat = nullptr;
	aiString path;
	aiReturn error = m->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
	std::string fullpath = currentPath +  App->loader->GetFileName(path.C_Str(), true);

	if (error == aiReturn::aiReturn_SUCCESS)
	{
		ComponentMaterial* usedMat = App->scene->CheckMaterial(path.C_Str());
		if (usedMat != nullptr)
		{
			mat = usedMat;
		}
		else
		{
			mat = new ComponentMaterial();
			mat->SetTexture(App->loader->texImporter.LoadTex(fullpath.c_str()));
			std::string name = App->loader->GetFileName(path.C_Str());
			mat->SetName(name);
			App->scene->materials.push_back(mat);
		}

	}
	else 
	{
		mat = new ComponentMaterial();
		mat->SetName(path.C_Str());
		App->scene->materials.push_back(mat);
		aiColor3D c(0.0f, 0.0f, 0.0f);
		m->Get(AI_MATKEY_COLOR_DIFFUSE, c);
		mat->color.r = c.r;
		mat->color.g = c.g;
		mat->color.b = c.b;
		mat->texEnabled = false;

	}

	return mat;
}


void ImporterMesh::SaveMeshAsMeh(ResMesh* m)
{
	uint ranges[3] = { m->num_vertex, m->num_indice, m->num_textC};

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

	bytes = sizeof(float)*m->num_vertex * 3;
	memcpy(last, m->normals, bytes);

	if (m->num_textC > 0)
	{
		last += bytes;

		bytes = sizeof(float)*m->num_textC;
		memcpy(last, m->textC, bytes);

	}

	std::string str = std::string(MESH_DIR) + m->GetName() + MESH_EXTENSION;

	App->fileSystem.SaveFile(str.c_str(), data, fileSize);

}

ResMesh* ImporterMesh::LoadMeh(const char* name, bool fullpath)
{
	ResMesh* mesh = new ResMesh();

	mesh->SetName(name);
	std::string str;

	if (fullpath)
		str = name;
	else
		str = MESH_DIR + std::string(name) + MESH_EXTENSION;


	char* data = nullptr;
	App->fileSystem.LoadFile(str.c_str(), &data);
	

	if (data == nullptr )
{
	VSLOG("Error Loading %s", str.c_str());
	return nullptr;
}

	char* last = data;
	uint ranges[3];
	uint bytes = sizeof(ranges);

	memcpy(ranges, last, bytes);

	mesh->num_vertex = ranges[0];
	mesh->num_indice = ranges[1];
	mesh->num_textC = ranges[2];

	last += bytes;
	bytes = sizeof(float)*mesh->num_vertex * 3;

	mesh->vertex = new float[mesh->num_vertex * 3];
	memcpy(mesh->vertex, last, bytes);

	last += bytes;
	bytes = sizeof(uint)*mesh->num_indice;

	mesh->indice = new uint[mesh->num_indice];
	memcpy(mesh->indice, last, bytes);

	last += bytes;
	bytes = sizeof(float)*mesh->num_vertex * 3;

	mesh->normals = new float[mesh->num_vertex * 3];
	memcpy(mesh->normals, last, bytes);

	if (mesh->num_textC > 0)
	{
		last += bytes;
		bytes = sizeof(float)*mesh->num_textC;

		mesh->textC = new float[mesh->num_textC];
		memcpy(mesh->textC, last, bytes);
	}
	mesh->GenerateBuffer();

	delete[] data;
	data = nullptr;

	return mesh;
}

float4x4 ImporterMesh::GetMatrix(aiMatrix4x4 m)
{
	float values[16] =
	{
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4
	};

	float4x4 newmat;
	newmat.Set(values);
	return newmat;
}

void ImporterMesh::SetMatrix(float4x4 m)
{

}

void LogAssimp(const char* c1, char* c2)
{
	VSLOG("%s", c1);
}

