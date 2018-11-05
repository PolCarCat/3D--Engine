#include "WinSceneLoader.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "SceneLoader.h"
#include <filesystem>
#include <windows.h>


WinSceneLoader::WinSceneLoader(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{

	path = GetDirectory() + FOLDER;
}


WinSceneLoader::~WinSceneLoader()
{
}


bool WinSceneLoader::Update()
{

	ImGui::Begin(save ? "Save" : "Load", &enabled, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	uint w = (App->window->w / 3);
	uint h = (App->window->h / 3);
	ImGui::SetWindowPos(ImVec2((App->window->w / 2) - (w / 2), (App->window->h / 2) - (h / 2)));
	ImGui::SetWindowSize(ImVec2(w, h));

	ImGui::Text("Path");
	ImGui::Separator();

	ImGui::PushID("path");
	char p[100];
	strcpy_s(p, 100, path.c_str());
	if (ImGui::InputText("", p, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		path = p;

	ImGui::PopID();

	ImGui::Text("Scenes");
	ImGui::Separator();
	ImGui::NewLine();

	ObjectsInDir(save);

	if (save)
		UpdateSave();


	ImGui::End();
	return true;
}

void WinSceneLoader::UpdateSave()
{

	ImGui::PushID("save");
	char n[50];
	strcpy_s(n, 50, App->scene->GetName());
	ImGui::InputText("", n, 50);
	App->scene->SetName(n);

	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		SceneLoader::SaveScene( std::string(path + App->scene->GetName() + ".json").c_str(), &App->scene->root);
		enabled = false;
	}
	ImGui::PopID();
}

void WinSceneLoader::UpdateLoad()
{

}

void WinSceneLoader::ObjectsInDir(bool s)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	hFind = FindFirstFile((path + "*").c_str(), &data);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (App->loader->CheckFormat(data.cFileName) == JSON)
			{
				ImGui::TreeNodeEx(data.cFileName, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
				
				if (ImGui::IsItemClicked())
				{
					if (s)
					{
						SceneLoader::SaveScene((std::string(FOLDER) + data.cFileName).c_str(), &App->scene->root);
					}
					else
					{
						SceneLoader::LoadScene((std::string(FOLDER) + data.cFileName).c_str(), &App->scene->root);
					}
					enabled = false;
				}
			}

		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

std::string WinSceneLoader::GetDirectory()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}