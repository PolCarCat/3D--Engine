#include "Application.h"
#include "WinAssets.h"
#include "ImGui/imgui.h"
#include <filesystem>
#include <windows.h>


WinAssets::WinAssets(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
	path = App->fileSystem.GetDirectory() + "Assets\\";
}


WinAssets::~WinAssets()
{
}

bool WinAssets::Update()
{
	ImGui::Begin("Assets");
	ImGui::PushID("assets");

	char n[100];
	strcpy_s(n, 100, path.c_str());
	ImGui::InputText("", n, 100);
	path = n;

	ImGui::SameLine();
	if (ImGui::Button("Assets"))
		path = App->fileSystem.GetDirectory() + "Assets\\";

	ObjectsInDir(path);

	ImGui::PopID();
	ImGui::End();

	return false;
}

void WinAssets::ObjectsInDir(std::string dir)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	hFind = FindFirstFile((dir + "*").c_str(), &data);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {

				ImGui::TreeNodeEx(data.cFileName, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

				if (ImGui::IsItemClicked())
				{

					if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					{
						path += data.cFileName + std::string("\\");
					}
				}			

		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}
