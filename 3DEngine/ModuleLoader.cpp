#include "ModuleLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib/MathGeoLib.h"
#include "WinConsole.h"

#include "SDL/include/SDL.h"
#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"



#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"


#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")
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



	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);


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
			LoadScene(droppedFile);
			App->imgui->element->curMesh = (*App->renderer3D->meshes.begin());
			App->camera->FocusMeshes();
			break;
		case PNG:
		case DDS:
			App->imgui->element->curMesh->tex.CleanUp();
			App->renderer3D->SetMeshesTex(LoadTex(droppedFile));
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

	return true;
}


void ModuleLoader::SetDropFile(char* f)
{
	droppedFile = f;
}


void ModuleLoader::LoadScene(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{				
		uint i = 0;
		for (int nm = 0; nm < scene->mNumMeshes; nm++)
		{
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
				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices != 3)
					{
						VSLOG("WARNING, geometry face with != 3 indices!");
						App->imgui->console->AddLog("\nWARNING, geometry face with != 3 indices!");
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
			mesh->name = nullptr;
			if (m->mName.length > 0)
			{
				mesh->name = new char[m->mName.length];
				strcpy(mesh->name, m->mName.C_Str());
			}
			else
			{				
				mesh->name = "Untitlied mesh";
			}


			mesh->GenerateBuffer();
			App->renderer3D->meshes.push_back(mesh);

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
	}
		
		
}

uint ModuleLoader::LoadChekerTex()
{

	GLubyte checkImage[CHECKERS_SIZE][CHECKERS_SIZE][4];
	for (int i = 0; i < CHECKERS_SIZE; i++) {
		for (int j = 0; j < CHECKERS_SIZE; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	uint id;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_SIZE, CHECKERS_SIZE,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	return id;
}

Texture ModuleLoader::LoadTex(const char* path)
{
	Texture ret;
	ILuint imageID;				
	GLuint textureID;	
	ILboolean success = false;	
	ILenum error;		

	ilGenImages(1, &imageID); 
	ilBindImage(imageID);
	success = ilLoadImage((ILconst_string)path);

										
	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		//success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);


		if (!success)
		{
			error = ilGetError();
			VSLOG("Image fliping error %d", error);
			App->imgui->console->AddLog("\Image fliping error");
		}


		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		ret.width = ilGetInteger(IL_IMAGE_WIDTH);
		ret.heigth = ilGetInteger(IL_IMAGE_HEIGHT);
		ret.id = textureID;


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret.width,	ret.heigth, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);
		VSLOG("Texture creation successful, image id %d", textureID);
		App->imgui->console->AddLog("\nTexture creation successful, image id ");
		App->imgui->console->AddNumLog((int)textureID);

		ilDeleteImages(1, &imageID);
	}
	else 
	{
		error = ilGetError();
		VSLOG("Image loading eror %d", error);
		App->imgui->console->AddLog("\nImage loading error");
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