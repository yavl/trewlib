#include "Hud.hpp"
#include <trew/Logger.hpp>
#include <trew/app/SdlWindow.hpp>
#include <algorithm>
#include <thread>
#include <SDL3/SDL_events.h>

using namespace trew;

Hud::Hud(SdlWindow* window) {
	// IMGUI begin
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImGui_ImplSDL3_InitForOpenGL(window->getRawSdlWindow(), nullptr);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsClassic();

	show_window = true;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// IMGUI end

	float font_size = 24.0f;
	font = io.Fonts->AddFontFromFileTTF("assets/Ubuntu-Regular.ttf", font_size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

Hud::~Hud() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void Hud::update(SDL_Event event) {
	ImGui_ImplSDL3_ProcessEvent(&event);
}

void Hud::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	if (show_window)
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(1600, 1200));
		ImGui::Begin("Connect", &show_window,
			ImGuiWindowFlags_NoResize		|
			ImGuiWindowFlags_NoScrollbar	|
			ImGuiWindowFlags_NoSavedSettings
		);
		ImGui::PushFont(font);
		ImGui::PopFont();

		static char ip[256] = "127.0.0.1";
		static int port = 13370;
		ImGui::InputText("ip", ip, IM_ARRAYSIZE(ip));
		ImGui::InputInt("port", &port);
		if (ImGui::Button("Connect")) {
			show_window = false;
			log("Hud", fmt::format("ip: {}\nport: {}\n", ip, port));
			// connection code was here
		}
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}