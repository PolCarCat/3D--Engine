#include "WinConsole.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleInput.h"

WinConsole::WinConsole(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
}


WinConsole::~WinConsole()
{
}


bool WinConsole::Update()
{
	ImGui::Begin("Console");
	Draw();
	if (ImGui::Button("Clear"))
		Clear();
	ImGui::End();
	return true;
}

void WinConsole::Clear()
{
	Buffer.clear();
}

void WinConsole::AddLog(const char* entry)
{
		Buffer.appendf(entry);
		ScrollToBottom = true;
}

void WinConsole::AddNumLog(int number)
{
	string s = std::to_string(number);
	const char* pchar = s.c_str();
	App->imgui->console->AddLog(pchar);
}

void WinConsole::Draw()
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(Buffer.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}