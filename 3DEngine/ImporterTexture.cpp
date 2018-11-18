#include "Application.h"
#include "ImporterTexture.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include <fstream>


#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"


#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#include "mmgr/mmgr.h"

ImporterTexture::ImporterTexture()
{

}


ImporterTexture::~ImporterTexture()
{
}

bool ImporterTexture::Start()
{

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool ImporterTexture::CleanUp()
{
	ilShutDown();
	return true;
}

bool ImporterTexture::Load()
{
	return true;
}

bool ImporterTexture::Save()
{
	return true;
}

uint ImporterTexture::LoadChekerTex()
{
	//Generate checker texture

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

ResTexture* ImporterTexture::LoadTex(const char* path, bool isfullpath)
{

	//return ret;
	std::string oldPath = path;
	std::string name = path;

	if (isfullpath)
		name = App->loader->GetFileName(path);


	std::string newPath = std::string(TEXT_DIR) + name + TEXT_EXTENSION;

	//If it's not fullpath it's assumed that is from the library or we couldn't load it
	if (isfullpath)
	{
		if (App->loader->CheckFormat(path) != DDS)
			SaveTex(oldPath.c_str());

		else
			App->fileSystem.Copy(oldPath.c_str(), newPath.c_str());
	}

	
	Resource* resource = App->resourceManager->GetResourceByName(name);
	ResTexture* ret = nullptr;



	ILuint imageID;
	GLuint textureID;
	ILboolean success = false;
	ILenum error;

	if (resource != nullptr && resource->GetType() == RESTEXTURE)
	{
		//Check if the image already exisit
		ret = (ResTexture*)resource;
		ret->AddInMemory();
	}
	else if (resource == nullptr)
	{
		//Load image
		ret = new ResTexture();

		ilGenImages(1, &imageID);
		ilBindImage(imageID);
		success = ilLoadImage((ILconst_string)newPath.c_str());


		if (success)
		{
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			if (!success)
			{
				error = ilGetError();
				VSLOG("\nImage fliping error %d", error);
			}


			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			ret->width = ilGetInteger(IL_IMAGE_WIDTH);
			ret->heigth = ilGetInteger(IL_IMAGE_HEIGHT);
			ret->id = textureID;
			ret->SetName(name);


			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret->width, ret->heigth, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

			glBindTexture(GL_TEXTURE_2D, 0);
			VSLOG("\nTexture creation successful, image id %d", textureID);

			ilDeleteImages(1, &imageID);

			//Add to resources
			App->resourceManager->AddResource(ret);
		}
	}
	else if (resource == nullptr)
	{
		VSLOG("Found resource with same id but different type");
	}
	else
	{
		error = ilGetError();
		VSLOG("\nImage loading eror %d", error);
	}


	return ret;
}

ResTexture ImporterTexture::ReloadTex(const char * path)
{

	//Load image
	ResTexture ret;

	ILuint imageID;
	GLuint textureID;
	ILboolean success = false;
	ILenum error;

	//Generate Buffer
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

		if (!success)
		{
			error = ilGetError();
			VSLOG("\nImage fliping error %d", error);
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


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret.width, ret.heigth, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);
		VSLOG("\nTexture creation successful, image id %d", textureID);

		ilDeleteImages(1, &imageID);

	}
	
	return ret;
}

void ImporterTexture::SaveTex(const char* path, bool isfullpath)
{
	std::string oldPath = path;
	std::string newPath;

	if (isfullpath)
		newPath = std::string(TEXT_DIR) + App->loader->GetFileName(path) + TEXT_EXTENSION;	
	else
		newPath = std::string(TEXT_DIR) + path + TEXT_EXTENSION;


	if (App->fileSystem.fileExists(newPath.c_str()))
		return;

	ILuint image_name;
	ilGenImages(1, &image_name);
	ilBindImage(image_name);


	if (ilLoadImage(path))
	{
		VSLOG("\nImage saving eror %d", ilGetError());
		ILuint size = 0;
		ILubyte* data = nullptr;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size = ilSaveL(IL_DDS, NULL, 0);
		if (size != 0) {
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				char* stuff = (char*)(data);

				App->fileSystem.SaveFile(newPath.c_str(), stuff, size);
			}
			delete[] data;
			ilDeleteImages(1, &image_name);
		}
	}



}

void ImporterTexture::SaveTex(ResTexture tex)
{
	std::string path = std::string(TEXT_DIR) + tex.GetName() + TEXT_EXTENSION;

	//Check if the files exist
	if (App->fileSystem.fileExists(path.c_str()))
		return;

	ILuint image_name;
	ilGenImages(1, &image_name);
	ilBindImage(image_name);


	if (ilLoadImage(tex.GetFile()))
	{
		ILinfo info;
		iluGetImageInfo(&info);

		if (ilGetError() == IL_NO_ERROR)
		{
	
			ILuint size;
			ILubyte* data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

			size = ilSaveL(IL_DDS, NULL, 0);
			if (size > 0)
			{
				data = new ILubyte[size];
				if (ilSaveL(IL_DDS, data, size) > 0)
				{
					std::ofstream dataFile(path.c_str(), std::fstream::out | std::fstream::binary);
					dataFile.write((const char*)data, size);
					dataFile.close();
				}
				delete[]data;
				ilDeleteImages(1, &image_name);
			}

		
		}
	}
	
}