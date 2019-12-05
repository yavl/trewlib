#include "Hud.hpp"
#include <algorithm>
#include <thread>
#include "Logger.hpp"

using namespace pm;

Hud::Hud(GLFWwindow* window) {
	// IMGUI begin
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsClassic();

	show_window = true;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// IMGUI end

	float font_size = 24.0f;
	font = io.Fonts->AddFontFromFileTTF("assets/Ubuntu-Regular.ttf", font_size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

Hud::~Hud() {}

void Hud::display() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (show_window)
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(1600, 1200));
		ImGui::Begin("Connect", &show_window, ImVec2(300, 150),
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