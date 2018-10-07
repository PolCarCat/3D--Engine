#include "WinConsole.h"
#include "Application.h"
#include "ImGui/imgui.h"


WinConsole::WinConsole(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
}


WinConsole::~WinConsole()
{
}


bool WinConsole::Update()
{
	ImGui::Begin("Console");
	ImGui::Button("Empty button");
	ImGui::End();
	return true;
}