#pragma once

#include <trew/trew.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>

namespace trew {
	class SdlWindow;

	class Hud {
	public:
		Hud(SdlWindow* window);
		~Hud();
		void update(SDL_Event event);
		void render();
	private:
		bool show_window;
		ImVec4 clear_color;
		ImFont* font;
	};
}