#include "ResTexture.h"
#include "Application.h"

#include "MathGeoLib/MathGeoLib.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ResTexture::ResTexture(uint32_t UUID) : Resource(UUID, RESTEXTURE)
{
}


ResTexture::~ResTexture()
{
}

void ResTexture::Init()
{
	ResTexture tex = App->loader->texImporter.ReloadTex(exportedFile.c_str());
	id = tex.id;
	width = tex.width;
	heigth = tex.heigth;

}

void ResTexture::CleanUp()
{
	glDeleteBuffers(1, (GLuint*) &(id));

	width = 0;
	heigth = 0;
}