#include "ModuleFBX.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib/MathGeoLib.h"

#include "SDL/include/SDL.h"
#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleFBX::~ModuleFBX()
{
}

bool ModuleFBX::Start()
{

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	
	LoadScene("Assets/BakerHouse.fbx");

	return true;
}

update_status ModuleFBX::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleFBX::Update(float dt)
{



	return UPDATE_CONTINUE;
}

update_status ModuleFBX::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleFBX::CleanUp()
{
	if (droppedFile != nullptr)
	SDL_free(droppedFile);

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

bool ModuleFBX::CheckDropEvent()
{
	bool ret = false;

	SDL_Event dropEvent;
	while (SDL_PollEvent(&dropEvent))
	{
		if (dropEvent.type == SDL_DROPFILE)
		{
			droppedFile = dropEvent.drop.file;
			ret = true;

			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_INFORMATION,
				"File dropped on window",
				droppedFile,
				App->window->window);
		}
	}

	return ret;
}


void ModuleFBX::LoadScene(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{				
		for (int nm = 0; nm < scene->mNumMeshes; nm++)
		{
			Mesh* mesh = new Mesh;
			aiMesh* m = scene->mMeshes[nm];
			mesh->num_vertex = m->mNumVertices;
			mesh->vertex = new float[mesh->num_vertex * 3];
			memcpy(mesh->vertex, m->mVertices, sizeof(float) * mesh->num_vertex * 3);
			//VSLOG("New mesh with %d vertices", m.num_vertices);

			if (m->HasFaces())
			{
				mesh->num_indice = m->mNumFaces * 3;
				mesh->indice = new uint[mesh->num_indice]; // assume each face is a triangle
				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices != 3)
					{
						VSLOG("WARNING, geometry face with != 3 indices!");
					}						
					else
						memcpy(&mesh->indice[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				
				}
			}

			if (m->HasNormals())
			{
				mesh->num_normals = m->mNumVertices * 3;
				mesh->normals = new float[mesh->num_normals * 3];
				memcpy(mesh->normals, m->mNormals, sizeof(float) * 3 * mesh->num_normals);
			}

			if (m->GetNumColorChannels() != 0)
			{
				mesh->num_colors = m->mNumVertices;
				mesh->colors = new float[mesh->num_colors];
				memcpy(mesh->colors, m->mColors, sizeof(float) * mesh->num_colors);
			}
			
			if (m->HasTextureCoords(AI_MAX_NUMBER_OF_TEXTURECOORDS))
			{
				mesh->num_textC = m->mNumVertices * 2;
				mesh->textC = new float[mesh->num_textC];
				memcpy(mesh->textC, m->mTextureCoords, sizeof(float) * mesh->num_textC);
			}

			mesh->GenerateBuffer();
			App->renderer3D->meshes.push_back(mesh);
			
		}	

		aiReleaseImport(scene);
		
	}
	else
	{
		VSLOG("Error loading scene %s", path);
	}
		
		
}