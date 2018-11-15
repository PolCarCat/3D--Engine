#include "WinCamera.h"
#include "ImGui/imgui.h"
#include "Application.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


WinCamera::WinCamera(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{

};


WinCamera::~WinCamera()
{
}


bool WinCamera::Update()
{


	//Provisional window to see the resources 

	ImGui::Begin("Resources");
	ImGui::Columns(2);
	for (auto it = App->resourceManager->resources.begin(); it != App->resourceManager->resources.end(); it++)
	{
		
		ImGui::Text(it->second->GetName());

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Uuidd: %d Used: %d times", it->second->GetUUID(), it->second->InMemory());
			ImGui::EndTooltip();
		}

		ImGui::NextColumn();
		
		ImVec4 color = { 1.0f, 0.2f, 0.2f, 1.0f };
		char* name = "Texture";
		if (it->second->GetType() == RESMESH)
		{
			color = { 0, 1.0f, 1.0f, 1.0f };
			name = "Mesh";
		}
		ImGui::TextColored(color, name);
		ImGui::NextColumn();
		

	}

	ImGui::Columns(1);
	ImGui::End();

	return true;
}

