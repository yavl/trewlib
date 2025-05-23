#include "HelloSDL3GPU.hpp"
#include <trew/Globals.hpp>
#include <trew/InputManager.hpp>
#include <trew/Camera.hpp>
#include <trew/scripting/ASManager.hpp>
#include <trew/AssetManager.hpp>
#include <trew/app/SdlWindow.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>
#include <trew/Shader.hpp>

static SDL_GPUGraphicsPipeline* Pipeline;
static Sint32 ResolutionIndex;

HelloSDL3GPU::HelloSDL3GPU(std::weak_ptr<Window> window) {
	this->window = window;
	Globals::camera = std::make_shared<Camera>(window);
	cam = Globals::camera;
	input = std::make_unique<InputManager>(window);
}

HelloSDL3GPU::~HelloSDL3GPU() {
	auto sdlWindow = static_cast<SdlWindow*>(window.lock().get());
	SDL_ReleaseGPUGraphicsPipeline(sdlWindow->getSdlGpuDevice(), Pipeline);
}

void HelloSDL3GPU::create() {
	Globals::assets = std::make_shared<AssetManager>("assets");;
	assets = Globals::assets;

	ASManager as(assets);
	//as.registerScript("assets/scripts/main.as");
	//as.runScript("assets/scripts/main.as");

	ResolutionIndex = 0;

	auto sdlWindow = static_cast<SdlWindow*>(window.lock().get());
	auto device = sdlWindow->getSdlGpuDevice();

	SDL_GPUShader* vertexShader = LoadShader(device, "RawTriangle.vert", 0, 0, 0, 0);
	if (vertexShader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
	}

	SDL_GPUShader* fragmentShader = LoadShader(device, "SolidColor.frag", 0, 0, 0, 0);
	if (fragmentShader == NULL)
	{
		SDL_Log("Failed to create fragment shader!");
	}

	SDL_GPUColorTargetDescription colorTargetDescription{};
	colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(device, sdlWindow->getRawSdlWindow());
	std::vector colorTargetDescriptions{ colorTargetDescription };

	SDL_GPUGraphicsPipelineTargetInfo targetInfo{};
	targetInfo.color_target_descriptions = colorTargetDescriptions.data();
	targetInfo.num_color_targets = colorTargetDescriptions.size();

	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.vertex_shader = vertexShader;
	pipelineCreateInfo.fragment_shader = fragmentShader;
	pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	pipelineCreateInfo.target_info = targetInfo;

	Pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineCreateInfo);
	if (Pipeline == NULL)
	{
		SDL_Log("Failed to create pipeline!");
	}

	SDL_ReleaseGPUShader(device, vertexShader);
	SDL_ReleaseGPUShader(device, fragmentShader);
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
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(sdlWindow->getSdlGpuDevice());
	if (cmdbuf == NULL)
	{
		SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
	}

	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, sdlWindow->getRawSdlWindow(), &swapchainTexture, NULL, NULL)) {
		SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
	}

	if (swapchainTexture != NULL)
	{
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = SDL_FColor { 0.0f, 0.0f, 0.0f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
		SDL_BindGPUGraphicsPipeline(renderPass, Pipeline);
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(cmdbuf);
}

void HelloSDL3GPU::resize(int width, int height) {
	render();
}
