#include "HelloSDL3GPU.hpp"
#include <trew/Globals.hpp>
#include <trew/InputManager.hpp>
#include <trew/Camera.hpp>
#include <trew/scripting/ASManager.hpp>
#include <trew/AssetManager.hpp>
#include <trew/app/SdlWindow.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>

HelloSDL3GPU::HelloSDL3GPU(std::weak_ptr<Window> window) {
	this->window = window;
	Globals::camera = std::make_shared<Camera>(window);
	cam = Globals::camera;
	input = std::make_unique<InputManager>(window);
}

HelloSDL3GPU::~HelloSDL3GPU() {}

void HelloSDL3GPU::create() {
	Globals::assets = std::make_shared<AssetManager>("assets");;
	assets = Globals::assets;

	ASManager as(assets);
	//as.registerScript("assets/scripts/main.as");
	//as.runScript("assets/scripts/main.as");
}

void HelloSDL3GPU::update(float dt) {
	input->update();
	cam->update(dt);

	SDL_Event evt;
	//fmt::println("fps: {}", 1 / dt);
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_EVENT_QUIT) {
			SDL_Quit();
			exit(0);
		}
	}
}

void HelloSDL3GPU::render() {
	auto sdlWindow = static_cast<SdlWindow*>(window.lock().get());
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(sdlWindow->getSdlGpuDevice());
	if (!commandBuffer) {
		SDL_Log("SDL_AcquireGPUCommandBuffer failed: %s", SDL_GetError());
		SDL_Quit();
	}

	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, sdlWindow->getRawSdlWindow(), &swapchainTexture, nullptr, nullptr)) {
		SDL_Log("SDL_WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
		SDL_Quit();
	}

	if (swapchainTexture != NULL)
	{
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = SDL_FColor{ 0.3f, 0.4f, 0.5f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(commandBuffer);
}

void HelloSDL3GPU::resize(int width, int height) {
	render();
}
