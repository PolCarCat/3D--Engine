#include "WinConfig.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "mmgr/mmgr.h"

WinConfig::WinConfig(Application* parent, bool start_enabled) : WinBase(parent, start_enabled) 
{
};

WinConfig::~WinConfig()
{
}

bool WinConfig::Update()
{

	ImGui::Begin("Configuration");

	if (ImGui::CollapsingHeader("Memory Management"))
	{
		sMStats stats = m_getMemoryStatistics();
		static int speed = 0;
		static vector<float> memory(100);
		if (++speed > 20)
		{
			speed = 0;
			if (memory.size() == 100)
			{
				for (uint i = 0; i < 100 - 1; ++i)
					memory[i] = memory[i + 1];

				memory[100 - 1] = (float)stats.totalReportedMemory;
			}
			else
				memory.push_back((float)stats.totalReportedMemory);
		}

		ImGui::PlotHistogram("Memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

		ImGui::Text("Total Reported: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
	}
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
		ImGui::InputInt("Width", &App->window->w, 150);
		ImGui::InputInt("Height", &App->window->h, 150);
		if (App->window->w < 500)
			App->window->w = 500;
		if (App->window->h < 350)
			App->window->h = 350;
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

		bool vsync = App->renderer3D->GetVsync();
		ImGui::Checkbox("Vsync", &vsync);
		if (vsync != App->renderer3D->GetVsync()) App->renderer3D->EnableVsync(vsync);

		bool depthT = App->renderer3D->GetDepthTest();
		ImGui::Checkbox("Depth Test", &depthT);
		if (depthT != App->renderer3D->GetDepthTest()) App->renderer3D->EnableDepthTest(depthT);

		bool cull = App->renderer3D->GetCullFace();
		ImGui::Checkbox("Cull Face", &cull);
		if (cull != App->renderer3D->GetCullFace()) App->renderer3D->EnableCullFace(cull);

		bool light = App->renderer3D->GetLighting();
		ImGui::Checkbox("Lighting", &light);
		if (light != App->renderer3D->GetLighting()) App->renderer3D->EnableLighting(light);

		bool color = App->renderer3D->GetColorMat();
		ImGui::Checkbox("Color Material", &color);
		if (color != App->renderer3D->GetColorMat()) App->renderer3D->EnableColorMaterial(color);
		
		bool tex2 = App->renderer3D->GetTexture2D();
		ImGui::Checkbox("Texture 2D", &tex2);
		if (tex2 != App->renderer3D->GetTexture2D()) App->renderer3D->EnableTexture2D(tex2);

		bool wire = App->renderer3D->GetWireFrame();
		ImGui::Checkbox("Wireframe", &wire);
		if (wire != App->renderer3D->GetWireFrame()) App->renderer3D->EnableWireframe(wire);




	}

	ImGui::End();

	return true;
}

