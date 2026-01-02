#include "BaseUI.hpp"
#include <trew/app/Window.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <trew/graphics/Renderer.hpp>
#include <trew/Logger.hpp>

using namespace trew;

BaseUI::BaseUI(Window* window) {
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

	ImGui::StyleColorsClassic();

	showWindow = true;
	clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float font_size = 24.0f;
	font = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu-Regular.ttf", font_size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

BaseUI::~BaseUI() {
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();
}

void BaseUI::update(SDL_Event event) {
	ImGui_ImplSDL3_ProcessEvent(&event);
}

void BaseUI::render(Renderer* renderer) {
	ImGui_ImplSDLGPU3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	if (showWindow) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(1600, 1200));
		ImGui::Begin("Connect", &showWindow,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings
		);
		ImGui::PushFont(font);
		ImGui::PopFont();

		static char ip[256] = "127.0.0.1";
		static int port = 13370;
		ImGui::InputText("ip", ip, IM_ARRAYSIZE(ip));
		ImGui::InputInt("port", &port);
		if (ImGui::Button("Connect")) {
			showWindow = false;
			logDebug("BaseUI", fmt::format("ip: {}\nport: {}", ip, port));
		}
		ImGui::End();
	}

	// Rendering
	ImGui::Render();

	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

	auto swapchainTexture = renderer->getSwapchainTexture();
	auto commandBuffer = renderer->getCommandBuffer();
	if (swapchainTexture != nullptr && !is_minimized) {
		// This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
		ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, commandBuffer);

		// Setup and start a render pass
		SDL_GPUColorTargetInfo target_info = {};
		target_info.texture = swapchainTexture;
		SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(commandBuffer, &target_info, 1, nullptr);

		// Render ImGui
		ImGui_ImplSDLGPU3_RenderDrawData(draw_data, commandBuffer, render_pass);

		SDL_EndGPURenderPass(render_pass);
	}
}
