#include "ModuleLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib/MathGeoLib.h"
#include "WinConsole.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "SDL/include/SDL.h"
#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"




#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Start()
{

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogAssimp;
	aiAttachLogStream(&stream);

	texImporter.Start();

	return true;
}

update_status ModuleLoader::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleLoader::Update(float dt)
{

	//CheckDropEvent();


	return UPDATE_CONTINUE;
}

update_status ModuleLoader::PostUpdate(float dt)
{

	if (droppedFile != nullptr)
	{
		Format f = CheckFormat(droppedFile);
		switch (f)
		{
		case FBX:
			App->renderer3D->CleanUpMeshes();
			if (LoadScene(droppedFile))
			{
				App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());
				//App->camera->FocusMeshes();
			}
			break;
		case PNG:
		case DDS:
			App->imgui->element->curMesh->tex.CleanUp();
			//App->renderer3D->SetMeshesTex(LoadTex(droppedFile));
			break;
		case FNULL:
			break;
		default:
			break;
		}
		
		droppedFile = nullptr;
	}
	return UPDATE_CONTINUE;
}

bool ModuleLoader::CleanUp()
{
	if (droppedFile != nullptr)
	SDL_free(droppedFile);

	// detach log stream
	aiDetachAllLogStreams();

	texImporter.CleanUp();
	return true;
}


void ModuleLoader::SetDropFile(char* f)
{
	droppedFile = f;
}


bool ModuleLoader::LoadScene(const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr)
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);

	if (scene != nullptr && scene->HasMeshes())
	{				
		uint i = 0;
		for (int nm = 0; nm < scene->mNumMeshes; nm++)
		{	
			bool error = false;
			Mesh* mesh = new Mesh;
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

			mesh->position = mesh->boundingBox.CenterPoint();

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
				
				mesh->name =  m->mName.C_Str();
			}
			else
			{				
				mesh->name = "Untitlied mesh";
			}


		
			if (!error)
			{
				mesh->GenerateBuffer();
				App->renderer3D->meshes.push_back(mesh);
			}


			App->imgui->console->AddLog("\nAdded mesh with ");
			App->imgui->console->AddNumLog(mesh->num_vertex);
			App->imgui->console->AddLog(" vertices, ");
			App->imgui->console->AddNumLog(mesh->num_indice);
			App->imgui->console->AddLog(" indices and ");
			App->imgui->console->AddNumLog(mesh->num_normals);
			App->imgui->console->AddLog(" normals");
			
		}	

		aiReleaseImport(scene);
	}
	else
	{
		VSLOG("Error loading scene %s", path);
		ret = false;
	}
		
	return ret;
}




Format ModuleLoader::CheckFormat(const char* path)
{
	Format ret = FNULL;
	std::string t = path;
	std::string format;

	bool point = false;

	for (std::string::reverse_iterator rit = t.rbegin(); rit != t.rend(); ++rit)
	{
		if (*rit == '.')
			break;
		format.insert(0, 1, *rit);
	}
	

	if (format == "FBX" || format == "fbx")
		ret = FBX;
	
	else if (format == "PNG" || format == "png")
		ret = PNG;
	
	else if (format == "DDS" || format == "dds")
		ret = DDS;
	else
		VSLOG("Unkown format file\n");
	
	return ret;
}

void LogAssimp(const char* c1, char* c2)
{
	VSLOG("%s", c1);
}