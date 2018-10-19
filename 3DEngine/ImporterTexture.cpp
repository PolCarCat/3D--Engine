#include "ImporterTexture.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

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

ResTexture ImporterTexture::LoadTex(const char* path)
{
	ResTexture ret;
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


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ret.width, ret.heigth, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

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