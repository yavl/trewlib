#pragma once

#include <trew/trew.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>

namespace trew {
	class Window;

	class Hud {
	public:
		Hud(Window* window);
		~Hud();
		void update(SDL_Event event);
		bool& getShowWindow();
	private:
        Window* window;
		bool show_window;
		ImVec4 clear_color;
		ImFont* font;
	};
}
