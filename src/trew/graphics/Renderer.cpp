//
//  Renderer.cpp
//  Trewlib
//
//  Created by Vladislav Nikolaev on 22.07.2025.
//

#include "Renderer.hpp"
#include <SDL3/SDL.h>
#include <trew/Shader.hpp>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>
#include <trew/Logger.hpp>
#include <trew/Hud.hpp>
#include <trew/Camera.hpp>
#include <trew/AssetManager.hpp>
#include <trew/drawables/ImageSurface.hpp>
#include <trew/Globals.hpp>
#include <trew/graphics/TextRenderer.hpp>

using namespace trew;

constexpr auto LOGTAG = "Renderer";

const char* assetsDirectory = "assets";

typedef struct PositionTextureVertex
{
	float x, y, z;
	float u, v;
} PositionTextureVertex;

typedef struct FragMultiplyUniform
{
	float r, g, b, a;
} FragMultiplyUniform;

Renderer::Renderer(SDL_GPUDevice* device, SDL_Window* window, Camera* cam, AssetManager* assets) :
	context(Context{ device, window }),
	cam(cam),
	assets(assets),
	textRenderer(std::make_unique<TextRenderer>(device, window, cam))
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	font = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu-Regular.ttf", font_size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

Renderer::~Renderer() {
	SDL_ReleaseGPUGraphicsPipeline(context.device, trianglePipeline);
	SDL_ReleaseGPUBuffer(context.device, vertexBuffer);
	SDL_ReleaseGPUBuffer(context.device, indexBuffer);
	//SDL_ReleaseGPUTexture(device, texture);
	SDL_ReleaseGPUSampler(context.device, linearSampler);
}

void Renderer::init() {
	trianglePipeline = createTrianglePipeline();
	rectanglePipeline = createRectanglePipeline();
	texturePipeline = createTexturePipeline();

	// Create a single, static quad for all textured rendering
	auto vertexBufferCreateInfo = SDL_GPUBufferCreateInfo{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = sizeof(PositionTextureVertex) * 4
	};
	vertexBuffer = SDL_CreateGPUBuffer(context.device, &vertexBufferCreateInfo);

	auto indexBufferCreateInfo = SDL_GPUBufferCreateInfo{
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = sizeof(Uint16) * 6
	};
	indexBuffer = SDL_CreateGPUBuffer(context.device, &indexBufferCreateInfo);

	// Create a temporary transfer buffer for one-time quad setup
	auto bufferTransferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = (sizeof(PositionTextureVertex) * 4) + (sizeof(Uint16) * 6)
	};
	SDL_GPUTransferBuffer* bufferTransferBuffer = SDL_CreateGPUTransferBuffer(context.device, &bufferTransferBufferCreateInfo);

	PositionTextureVertex* transferData = static_cast<PositionTextureVertex*>(SDL_MapGPUTransferBuffer(context.device, bufferTransferBuffer, false));
	transferData[0] = PositionTextureVertex{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f };
	transferData[1] = PositionTextureVertex{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	transferData[2] = PositionTextureVertex{ 1.0f,  1.0f, 0.0f, 1.0f, 1.0f };
	transferData[3] = PositionTextureVertex{ -1.0f,  1.0f, 0.0f, 0.0f, 1.0f };

	Uint16* indexData = (Uint16*)&transferData[4];
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 0;
	indexData[4] = 2;
	indexData[5] = 3;

	SDL_UnmapGPUTransferBuffer(context.device, bufferTransferBuffer);

	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(context.device);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

	auto transferBufferLocationOne = SDL_GPUTransferBufferLocation{
		.transfer_buffer = bufferTransferBuffer,
		.offset = 0
	};
	auto bufferRegionOne = SDL_GPUBufferRegion{
		.buffer = vertexBuffer,
		.offset = 0,
		.size = sizeof(PositionTextureVertex) * 4
	};
	SDL_UploadToGPUBuffer(copyPass, &transferBufferLocationOne, &bufferRegionOne, false);

	auto transferBufferLocationTwo = SDL_GPUTransferBufferLocation{
		.transfer_buffer = bufferTransferBuffer,
		.offset = sizeof(PositionTextureVertex) * 4
	};
	auto bufferRegionTwo = SDL_GPUBufferRegion{
		.buffer = indexBuffer,
		.offset = 0,
		.size = sizeof(Uint16) * 6
	};
	SDL_UploadToGPUBuffer(copyPass, &transferBufferLocationTwo, &bufferRegionTwo, false);

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	SDL_ReleaseGPUTransferBuffer(context.device, bufferTransferBuffer);

	auto nearestSamplerCreateInfo = SDL_GPUSamplerCreateInfo{
		.min_filter = SDL_GPU_FILTER_NEAREST,
		.mag_filter = SDL_GPU_FILTER_NEAREST,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
	};
	nearestSampler = SDL_CreateGPUSampler(context.device, &nearestSamplerCreateInfo);

	auto linearSamplerCreateInfo = SDL_GPUSamplerCreateInfo{
		.min_filter = SDL_GPU_FILTER_LINEAR,
		.mag_filter = SDL_GPU_FILTER_LINEAR,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
	};
	linearSampler = SDL_CreateGPUSampler(context.device, &linearSamplerCreateInfo);
}

void Renderer::render() {
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(context.device);
	this->commandBuffer = commandBuffer;
	if (commandBuffer == nullptr) {
		logError(LOGTAG, fmt::format("AcquireGPUCommandBuffer failed: {}", SDL_GetError()));
	} else {
		textRenderer->context.commandBuffer = commandBuffer;
	}

	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, context.window, &swapchainTexture, nullptr, nullptr)) {
		logError(LOGTAG, fmt::format("WaitAndAcquireGPUSwapchainTexture failed: {}", SDL_GetError()));
	}
	this->swapchainTexture = swapchainTexture;
	textRenderer->context.swapchainTexture = swapchainTexture;

	clearScreen();

	drawTriangle(0, 0, 100, 100);

	{
		Vector2 begin = Globals::selectionQuad[0];
		Vector2 end = Globals::selectionQuad[1];
		auto vec1 = begin;
		auto vec2 = end;
		//logDebug(LOGTAG, fmt::format("wpos: {}, {}; {}, {}", vec1.x, vec1.y, vec2.x, vec2.y));

		auto width = abs(vec2.x - vec1.x);
		auto height = abs(vec2.y - vec1.y);
		drawRectangle(vec1.x + width / 2, vec1.y + height / 2, width, height);
	}
}

void Renderer::submit() {
	SDL_SubmitGPUCommandBuffer(commandBuffer);
	this->commandBuffer = nullptr;
	this->swapchainTexture = nullptr;
	textRenderer->context.commandBuffer = nullptr;
	textRenderer->context.swapchainTexture = nullptr;
}

SDL_GPUGraphicsPipeline* Renderer::createTrianglePipeline() {
	SDL_GPUShader* vertexShader = loadShader(context.device, assetsDirectory, "RawTriangle.vert", 0, 0, 0, 1);
	if (vertexShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create vertex shader!"));
	}

	SDL_GPUShader* fragmentShader = loadShader(context.device, assetsDirectory, "SolidColor.frag", 0, 0, 0, 0);
	if (fragmentShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create fragment shader!"));
	}

	SDL_GPUColorTargetDescription colorTargetDescription{};
	colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(context.device, context.window);
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

	auto pipeline = SDL_CreateGPUGraphicsPipeline(context.device, &pipelineCreateInfo);
	if (pipeline == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create pipeline!"));
	}

	SDL_ReleaseGPUShader(context.device, vertexShader);
	SDL_ReleaseGPUShader(context.device, fragmentShader);
	return pipeline;
}

SDL_GPUGraphicsPipeline* Renderer::createRectanglePipeline() {
	SDL_GPUShader* vertexShader = loadShader(context.device, assetsDirectory, "RawRectangle.vert", 0, 0, 0, 1);
	if (vertexShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create vertex shader!"));
	}

	SDL_GPUShader* fragmentShader = loadShader(context.device, assetsDirectory, "SolidColor.frag", 0, 0, 0, 0);
	if (fragmentShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create fragment shader!"));
	}

	SDL_GPUColorTargetDescription colorTargetDescription{};
	colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(context.device, context.window);
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

	auto pipeline = SDL_CreateGPUGraphicsPipeline(context.device, &pipelineCreateInfo);
	if (pipeline == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create pipeline!"));
	}

	SDL_ReleaseGPUShader(context.device, vertexShader);
	SDL_ReleaseGPUShader(context.device, fragmentShader);
	return pipeline;
}

SDL_GPUGraphicsPipeline* Renderer::createTexturePipeline() {
	SDL_GPUShader* vertexShader = loadShader(context.device, assetsDirectory, "TexturedQuad.vert", 0, 0, 0, 1);
	if (vertexShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create vertex shader!"));
	}

	SDL_GPUShader* fragmentShader = loadShader(context.device, assetsDirectory, "TexturedQuad.frag", 1, 0, 0, 1);
	if (fragmentShader == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create fragment shader!"));
	}

	auto vertexBufferDescription = SDL_GPUVertexBufferDescription{
		.slot = 0,
		.pitch = sizeof(PositionTextureVertex),
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};

	auto vertexAttributeOne = SDL_GPUVertexAttribute{
		.location = 0,
		.buffer_slot = 0,
		.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
		.offset = 0
	};

	auto vertexAttributeTwo = SDL_GPUVertexAttribute{
		.location = 1,
		.buffer_slot = 0,
		.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
		.offset = sizeof(float) * 3
	};

	SDL_GPUVertexAttribute vertexAttributes[2] = { vertexAttributeOne, vertexAttributeTwo };

	auto vertexInputState = SDL_GPUVertexInputState{
		.vertex_buffer_descriptions = &vertexBufferDescription,
		.num_vertex_buffers = 1,
		.vertex_attributes = vertexAttributes,
		.num_vertex_attributes = 2
	};
	auto blendState = SDL_GPUColorTargetBlendState{
		.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
		.color_blend_op = SDL_GPU_BLENDOP_ADD,
		.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
		.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
		.enable_blend = true
	};
	auto colorTargetDescription = SDL_GPUColorTargetDescription{
		.format = SDL_GetGPUSwapchainTextureFormat(context.device, context.window),
		.blend_state = blendState
	};
	auto targetInfo = SDL_GPUGraphicsPipelineTargetInfo{
		.color_target_descriptions = &colorTargetDescription,
		.num_color_targets = 1
	};
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		.vertex_input_state = vertexInputState,
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.target_info = targetInfo
	};

	auto pipeline = SDL_CreateGPUGraphicsPipeline(context.device, &pipelineCreateInfo);
	if (pipeline == nullptr) {
		logError(LOGTAG, fmt::format("Failed to create pipeline!"));
	}

	SDL_ReleaseGPUShader(context.device, vertexShader);
	SDL_ReleaseGPUShader(context.device, fragmentShader);

	return pipeline;
}

SDL_GPUTexture* Renderer::createTexture(ImageSurface* image) {
	unsigned int w;
	unsigned int h;
	void* pixels = nullptr;
	auto imageData = image->getSdlSurface();
	if (imageData) {
		w = static_cast<unsigned int>(imageData->w);
		h = static_cast<unsigned int>(imageData->h);
		pixels = imageData->pixels;
	}
	auto textureTransferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = static_cast<unsigned int>(w * h * 4)
	};
	SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(context.device, &textureTransferBufferCreateInfo);

	Uint8* textureTransferPtr = static_cast<Uint8*>(SDL_MapGPUTransferBuffer(context.device, textureTransferBuffer, false));
	if (textureTransferPtr && pixels) {
		SDL_memcpy(textureTransferPtr, pixels, w * h * 4);
	}
	else if (textureTransferPtr == nullptr) {
		logError(LOGTAG, fmt::format("Failed to map transfer buffer!"));
	}
	SDL_UnmapGPUTransferBuffer(context.device, textureTransferBuffer);

	unsigned int maxDim = (w > h) ? w : h;
	unsigned int numLevels = 0;
	while (maxDim > 1) {
		maxDim >>= 1;
		numLevels++;
	}
	numLevels += 1;

	logDebug(LOGTAG, fmt::format("Generating {}x{} texture with num levels: {}", w, h, numLevels));

	auto textureCreateInfo = SDL_GPUTextureCreateInfo{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		.width = w,
		.height = h,
		.layer_count_or_depth = 1,
		.num_levels = numLevels
	};
	auto texture = SDL_CreateGPUTexture(context.device, &textureCreateInfo);
	auto textureTransferInfo = SDL_GPUTextureTransferInfo{
		.transfer_buffer = textureTransferBuffer,
		.offset = 0 /* Zeroes out the rest */
	};
	auto textureRegion = SDL_GPUTextureRegion{
		.texture = texture,
		.w = w,
		.h = h,
		.d = 1
	};
	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(context.device);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);
	SDL_UploadToGPUTexture(copyPass, &textureTransferInfo, &textureRegion, false);

	SDL_EndGPUCopyPass(copyPass);
	SDL_GenerateMipmapsForGPUTexture(uploadCmdBuf, texture);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

	SDL_ReleaseGPUTransferBuffer(context.device, textureTransferBuffer);
	return texture;
}

SDL_GPUTexture* Renderer::getTexture(ImageSurface* image) {
	if (textures.count(image) == 0) {
		logError(LOGTAG, "No such texture, please call Renderer::prepareTexture first");
	}
	return textures[image];
}

void Renderer::prepareTexture(const char* name) {
	if (auto image = assets->getImage(name)) {
		if (textures[image] == nullptr) {
			textures[image] = createTexture(image);
		}
		else {
			logError(LOGTAG, fmt::format("Texture {} was already loaded", name));
		}
	} else {
		logError(LOGTAG, fmt::format("Asset {} not found, please load image with AssetManager::load first", name));
	}
}

void Renderer::render(Hud* hud) {
	ImGui_ImplSDLGPU3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	auto& show_window = hud->getShowWindow();
	if (show_window) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(1600, 1200));
		ImGui::Begin("Connect", &show_window,
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
			show_window = false;
			logDebug("Hud", fmt::format("ip: {}\nport: {}", ip, port));
			// connection code was here
		}
		ImGui::End();
	}

	// Rendering
	ImGui::Render();

	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

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

void Renderer::clearScreen() {
	SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
	colorTargetInfo.texture = swapchainTexture;
	colorTargetInfo.clear_color = SDL_FColor{ 0.f, 0.5f, 0.7f, 1.0f };
	colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
	SDL_EndGPURenderPass(renderPass);
}

void Renderer::drawTriangle(float x, float y, float width, float height) {
	if (swapchainTexture) {
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
		SDL_BindGPUGraphicsPipeline(renderPass, trianglePipeline);

		auto texWidth = 1;
		auto texHeight = 1;
		auto rotation = 0.f;
		auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0));
		matrix = glm::scale(matrix, glm::vec3(1.f / texWidth, 1.f / texHeight, 1.f));
		matrix = glm::scale(matrix, glm::vec3(width, height, 1.f));
		matrix = glm::scale(matrix, glm::vec3(0.5f));
		matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, -1.f));
		glm::mat4 multiplied = cam->projection * cam->view * matrix;
		SDL_PushGPUVertexUniformData(commandBuffer, 0, glm::value_ptr(multiplied), sizeof(multiplied));

		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}
}

void Renderer::drawRectangle(float x, float y, float width, float height) {
	if (swapchainTexture) {
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
		SDL_BindGPUGraphicsPipeline(renderPass, rectanglePipeline);

		auto texWidth = 1;
		auto texHeight = 1;
		auto rotation = 0.f;
		auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0));
		matrix = glm::scale(matrix, glm::vec3(1.f / texWidth, 1.f / texHeight, 1.f));
		matrix = glm::scale(matrix, glm::vec3(width, height, 1.f));
		matrix = glm::scale(matrix, glm::vec3(0.5f));
		matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, -1.f));
		glm::mat4 multiplied = cam->projection * cam->view * matrix;
		SDL_PushGPUVertexUniformData(commandBuffer, 0, glm::value_ptr(multiplied), sizeof(multiplied));

		SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}
}

void Renderer::drawTexture(float x, float y, float width, float height, SDL_GPUTexture* texture, float rotation, float scale, std::optional<Color> color, std::optional<glm::mat4> parentModelMatrix, FilterMode filterMode) {
	if (swapchainTexture) {
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
		SDL_BindGPUGraphicsPipeline(renderPass, texturePipeline);
		auto vertexBufferBinding = SDL_GPUBufferBinding{
			.buffer = vertexBuffer,
			.offset = 0
		};
		SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBufferBinding, 1);
		auto indexBufferBinding = SDL_GPUBufferBinding{
			.buffer = indexBuffer,
			.offset = 0
		};
		SDL_BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

		auto textureSamplerBinding = SDL_GPUTextureSamplerBinding{
			.texture = texture,
			.sampler = (filterMode == FilterMode::NEAREST) ? nearestSampler : linearSampler
		};
		SDL_BindGPUFragmentSamplers(renderPass, 0, &textureSamplerBinding, 1);

		auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0));
		matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, -1.f));
		matrix = glm::scale(matrix, glm::vec3(width, height, 1.f));
		matrix = glm::scale(matrix, glm::vec3(scale, scale, 1.f));
		matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 1.f));
		if (auto resultOpt = parentModelMatrix) {
			glm::mat4 parentMatrix = *resultOpt;
			matrix = parentMatrix * matrix;
		}
		glm::mat4 multiplied = cam->projection * cam->view * matrix;
		SDL_PushGPUVertexUniformData(commandBuffer, 0, glm::value_ptr(multiplied), sizeof(multiplied));

		FragMultiplyUniform multiplyUniform;
		if (auto resultOpt = color) {
			auto value = *color;
			multiplyUniform = FragMultiplyUniform{ value.r, value.g, value.b, value.a };
		} else {
			multiplyUniform = FragMultiplyUniform{ 1.f, 1.f, 1.f, 1.f };
		}
		SDL_PushGPUFragmentUniformData(commandBuffer, 0, &multiplyUniform, sizeof(multiplyUniform));
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}
}

void Renderer::drawText(const char* text, float x, float y, FontSize fontSize, float rotation) {
	textRenderer->drawText(text, x, y, fontSize, 0.f);
}