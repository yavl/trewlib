#include "Hud.hpp"
#include <trew/app/Window.hpp>
#include <algorithm>
#include <thread>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>

using namespace trew;

Hud::Hud(Window* window) {
	// IMGUI begin
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

    this->window = window;
    
    ImGui_ImplSDL3_InitForSDLGPU(window->getRawSdlWindow());
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    auto device = window->getSdlGpuDevice();
    init_info.Device = window->getSdlGpuDevice();
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window->getRawSdlWindow());
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);
    //ImGui_ImplSDL3_InitForOpenGL(window->getRawSdlWindow(), nullptr);
	//ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsClassic();

	show_window = true;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// IMGUI end

	float font_size = 24.0f;
	font = io.Fonts->AddFontFromFileTTF("assets/Ubuntu-Regular.ttf", font_size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

Hud::~Hud() {
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();
}

void Hud::update(SDL_Event event) {
	ImGui_ImplSDL3_ProcessEvent(&event);
}

bool& Hud::getShowWindow() {
	return show_window;
}
