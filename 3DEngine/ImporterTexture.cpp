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


ImporterTexture::ImporterTexture()
{
}


ImporterTexture::~ImporterTexture()
{
}

bool ImporterTexture::Start()
{
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);



	return true;
}

bool ImporterTexture::CleanUp()
{
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

ResTexture ImporterTexture::LoadTex(const char* path, bool isfullpath)
{
	//If its not full path it's asumed that is a texture from the library

	std::string oldPath = path;
	std::string name;
	std::string newPath;

	if (isfullpath)
	{
		name = App->loader->GetFileName(path);
		
		if (App->loader->CheckFormat(path) != DDS)
		{
			newPath = std::string(TEXT_DIR) + name + TEXT_EXTENSION;
			SaveTex(path);
		}

	}
	else
	{
		SaveTex(oldPath.c_str());
	}

	newPath = std::string(TEXT_DIR) + name + TEXT_EXTENSION;

	ResTexture ret;
	ILuint imageID = 0;
	ILboolean success = false;
	ILenum error;

	glGenTextures(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, imageID);

	success = iluLoadImage((ILconst_string)oldPath.c_str());


	if (success)
	{

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		glBindTexture(GL_TEXTURE_2D, imageID);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			success = iluFlipImage();
		}

		success = ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE);


		if (!success)
		{
			error = ilGetError();
			VSLOG("error %d", error);
		}


		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		ret.width = ilGetInteger(IL_IMAGE_WIDTH);
		ret.heigth = ilGetInteger(IL_IMAGE_HEIGHT);
		ret.id = imageID;
		ret.name = name;
		ret.path = path;


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret.width, ret.heigth, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);
		VSLOG("Texture creation successful, image id %d", imageID);

		ilDeleteImages(1, &imageID);

	}
	else
	{
		error = ilGetError();
		VSLOG("\nImage loading eror %d", error);
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

	char* buffer = nullptr;
	int	  lenght = App->fileSystem.LoadFile(path, &buffer);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, lenght))
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
					std::ofstream dataFile(newPath.c_str(), std::fstream::out | std::fstream::binary);
					dataFile.write((const char*)data, size);
					dataFile.close();
				}
				delete[]data;
				ilDeleteImages(1, &image_name);
			}


		}
	}
	else
	{
		VSLOG("\nImage saving eror %d", ilGetError());
	}
}

void ImporterTexture::SaveTex(ResTexture tex)
{
	std::string path = std::string(TEXT_DIR) + tex.name + TEXT_EXTENSION;
	std::string path2 = path;
	App->fileSystem.InvertBars(path2);

	if (App->fileSystem.fileExists(path2.c_str()))
		return;

	ILuint image_name;
	ilGenImages(1, &image_name);
	ilBindImage(image_name);


	if (ilLoadImage(tex.path.c_str()))
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