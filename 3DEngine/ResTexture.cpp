#include "ResTexture.h"

#include "MathGeoLib/MathGeoLib.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ResTexture::ResTexture()
{
}


ResTexture::~ResTexture()
{
}

void ResTexture::CleanUp()
{
	glDeleteBuffers(1, (GLuint*) &(id));

	width = 0;
	heigth = 0;
}