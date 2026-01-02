#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>

namespace trew {
	class Window;
	class Renderer;

	class BaseUI {
	public:
		BaseUI(Window* window);
		virtual ~BaseUI();
		virtual void update(SDL_Event event);
		virtual void render(Renderer* renderer);
	protected:
        Window* window;
		bool showWindow;
		ImVec4 clearColor;
		ImFont* font;
	};
}
