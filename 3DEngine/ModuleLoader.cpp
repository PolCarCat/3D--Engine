#include "ModuleLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib/MathGeoLib.h"

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
	aiAttachLogStream(&stream);



	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);


	Lenna = LoadTex("Assets/Baker_house.png");
	//Lenna = LoadChekerTex();
	//LoadScene("Assets/BakerHouse.fbx");
	
	//App->renderer3D->SetMeshesTex(Lenna);

	return true;
}

update_status ModuleLoader::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleLoader::Update(float dt)
{

	CheckDropEvent();


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
			break;
		case PNG:
		case DDS:
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

bool ModuleLoader::CheckDropEvent()
{
	bool ret = false;
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

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


void ModuleLoader::LoadScene(const char* path)
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

			mesh->tex = 0;
			mesh->name = nullptr;
			if (m->mName.length > 0)
			{
				mesh->name = new char[m->mName.length];
				strcpy(mesh->name, m->mName.C_Str());
			}
			else
			{
				mesh->name = "No name";
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

uint ModuleLoader::LoadTex(const char* path)
{
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
		}


		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),	ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);
		VSLOG("Texture creation successful, image id %d", textureID);

		ilDeleteImages(1, &imageID);
	}
	else 
	{
		error = ilGetError();
		VSLOG("Image loading eror %d", error);
	}

	
	return textureID; 
}


Format ModuleLoader::CheckFormat(const char* path)
{
	Format ret = FNULL;
	std::string t = path;
	std::string format;


	bool point = false;

	for (std::string::iterator it = t.begin(); it != t.end(); ++it) {

		if (point)
			format.push_back(*it);

		if (*it == '.')
			point = true;	
	}

	if (format == "FBX" || format == "fbx")
		ret = FBX;
	
	else if (format == "PNG" || format == "png")
		ret = PNG;
	
	else if (format == "DDS" || format == "dds")
		ret = DDS;
	


	
	return ret;
}