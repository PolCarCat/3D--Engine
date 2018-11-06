#include "WinTimeManager.h"
#include "ImGui/imgui.h"
#include "Application.h"


WinTimeManager::WinTimeManager(Application* parent, bool start_enabled) : WinBase(parent, start_enabled)
{
}


WinTimeManager::~WinTimeManager()
{
}

bool WinTimeManager::Update()
{
	ImGui::Begin("Scene");
	bool play = App->timeManager->GetPlaying();
	float tsc = App->timeManager->GetTimeScale();

	if (ImGui::Button(play ? "Stop" : "Play"))
	{
		App->timeManager->SetPlaying(!play);
	}
	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		App->timeManager->Reset();
	}
	ImGui::SameLine();

	if (ImGui::Button("Slow"))
	{
		App->timeManager->SetTimeScale(0.5f);
	}
	ImGui::SameLine();

	if (ImGui::Button("Normal Speed"))
	{
		App->timeManager->SetTimeScale(1.0f);
	}
	ImGui::SameLine();

	if (ImGui::Button("Accelerate"))
	{
		App->timeManager->SetTimeScale(1.5f);
	}

	ImGui::Separator();
	ImGui::Text("Game Time: %.2f", App->timeManager->GetGameTime());
	ImGui::Text("Real Time: %.2f", App->timeManager->GetRealTime());
	ImGui::End();
	
	return true;
}
