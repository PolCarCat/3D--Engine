#include "WinConfig.h"
#include "Application.h"
#include "ImGui/imgui.h"

WinConfig::WinConfig(Application* parent, bool start_enabled) : WinBase(parent, start_enabled) 
{
};

WinConfig::~WinConfig()
{
}

bool WinConfig::Update()
{

	ImGui::Begin("Configuration");

	if (ImGui::CollapsingHeader("Frame management"))
	{
		ImGui::SliderInt("FPS Cap", &App->framerate_cap, 1, 120);

		char plot[50];
		sprintf_s(plot, 50, "Framerate %.1f", App->fps[App->fps_counter - 1]);
		ImGui::PlotHistogram("Framerate", &App->fps[0], 50, 0, plot, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::Spacing();
		sprintf_s(plot, 50, "Milliseconds %.1f", App->ms[App->ms_counter - 1]);
		ImGui::PlotHistogram("Milliseconds", &App->ms[0], 50, 0, plot, 0.0f, 100.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::SliderFloat("Brightness", &App->window->brightness, 0, 1);
		ImGui::SliderInt("Width", &App->window->w, 100, 4000);
		ImGui::SliderInt("Height", &App->window->h, 100, 4000);
		ImGui::Text("Refresh rate: %d", App->window->refresh_rate);

		ImGui::Checkbox("FullScreen", &App->window->FS);
		ImGui::Checkbox("Full Desktop", &App->window->FSD);
		ImGui::Checkbox("Resizable", &App->window->res);
		ImGui::Checkbox("Bordered", &App->window->bord);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("%d CPU's (%dkb Cache)\nSystem RAM: %.0fGb\nCaps:", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize(), (float)SDL_GetSystemRAM() / 1000);

		if (SDL_Has3DNow())
			ImGui::Text("3DNow");
		if (SDL_HasAVX())
			ImGui::Text("AVX");
		if (SDL_HasAVX2())
			ImGui::Text("AVX2");
		if (SDL_HasAltiVec())
			ImGui::Text("AltiVec");
		if (SDL_HasMMX())
			ImGui::Text("MMX");
		if (SDL_HasRDTSC())
			ImGui::Text("RDTSC");
		if (SDL_HasSSE())
			ImGui::Text("SSE");
		if (SDL_HasSSE2())
			ImGui::Text("SSE2");
		if (SDL_HasSSE3())
			ImGui::Text("SSE3");
		if (SDL_HasSSE41())
			ImGui::Text("SSE41");
		if (SDL_HasSSE42())
			ImGui::Text("SSE42");
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{

		if (ImGui::Button((App->renderer3D->GetVsync() != true ? "Disable Vsync" : "Enable Vsync")))
			App->renderer3D->EnableVsync();

		if (ImGui::Button((App->renderer3D->GetDepthTest() != true ? "Disable Depth Test" : "Enable Depth Test")))
			App->renderer3D->EnableDepthTest();

		if (ImGui::Button((App->renderer3D->GetCullFace() != true ? "Disable CullFace" : "Enable CullFace")))
			App->renderer3D->EnableCullFace();

		if (ImGui::Button((App->renderer3D->GetLighting() != true ? "Disable Lighting" : "Enable Lighting")))
			App->renderer3D->EnableLighting();

		if (ImGui::Button((App->renderer3D->GetColorMat() != true ? "Disable Color Material" : "Enable Color Material")))
			App->renderer3D->EnableColorMaterial();

		if (ImGui::Button((App->renderer3D->GetTexture2D() != true ? "Disable Texture 2D" : "Enable Texture 2D")))
			App->renderer3D->EnableTexture2D();

		if (ImGui::Button((App->renderer3D->GetWireFrame() != true ? "Enable WireFrame" : "Disable WireFrame")))
			App->renderer3D->EnableWireframe();



	}

	ImGui::End();

	return true;
}

