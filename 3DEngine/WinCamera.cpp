#include "WinCamera.h"
#include "ImGui/imgui.h"
#include "Application.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


WinCamera::WinCamera(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
	//GenerateBuffer();
};


WinCamera::~WinCamera()
{
}


bool WinCamera::Update()
{
	//BindImage();
	//ImGui::Begin("Camera");
	//ImVec2 size;
	//size.x = ImGui::GetWindowContentRegionWidth() - 20;
	//size.y = ImGui::GetWindowHeight() - 20;
	////ImGui::Image((void*)image.id, size , ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::End();

	//Provisional window to see the resources 

	ImGui::Begin("Resources");
	for (auto it = App->resourceManager->resources.begin(); it != App->resourceManager->resources.end(); it++)
	{
		ImGui::Text(it->second->GetName());
		ImGui::SameLine();
		ImGui::Text("Used %d times", it->second->InMemory());

	}


	ImGui::End();

	return true;
}

void WinCamera::BindImage()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, image.id);
	//glViewport(0, 0, 100, 100);
}

void WinCamera::GenerateBuffer()
{
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glGenTextures(1, &image.id);
	//glBindTexture(GL_TEXTURE_2D, image.id);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	////ret.width = ilGetInteger(IL_IMAGE_WIDTH);
	////ret.heigth = ilGetInteger(IL_IMAGE_HEIGHT);
	////ret.id = textureID;

	//float w = 1000;
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL());

	//glBindTexture(GL_TEXTURE_2D, 0);
}