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

	if (ImGui::Button(play ? "Pause" : "Play"))
	{
		App->timeManager->SetPlaying(!play);
	}
	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		App->timeManager->Reset();
	}
	ImGui::SameLine();


	float ts = App->timeManager->GetTimeScale();
	//ImGuiInputTextFlags_

	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() / 7 );
	ImGui::InputFloat("Time Scale", &ts, 0.1f, 0.1f, "%.3f");
	App->timeManager->SetTimeScale(ts);
	ImGui::PopItemWidth();


	ImGui::Separator();
	ImGui::Text("Game Time: %.2f", App->timeManager->GetGameTime());
	ImGui::Text("Real Time: %.2f", App->timeManager->GetRealTime());
	ImGui::End();
	
	return true;
}
