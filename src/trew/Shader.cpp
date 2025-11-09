#include "Shader.hpp"
#include <trew/FileHandle.hpp>
#include <trew/Logger.hpp>
#include <trew/Color.hpp>
#include <SDL3/SDL.h>

using namespace trew;

constexpr auto LOGTAG = "Shader";

SDL_GPUShader* trew::loadShader(
	SDL_GPUDevice* device,
    const char* assetsDirectory,
	const char* shaderFilename,
	Uint32 samplerCount,
	Uint32 storageTextureCount,
	Uint32 storageBufferCount,
	Uint32 uniformBufferCount
) {
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert")) {
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	} else if (SDL_strstr(shaderFilename, ".frag")) {
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	} else {
		logError(LOGTAG, fmt::format("Invalid shader stage!"));
		return NULL;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/shaders/compiled/%s.spv", assetsDirectory, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/shaders/compiled/%s.msl", assetsDirectory, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main_0";
	} else {
		logError(LOGTAG, fmt::format("{}", "Unrecognized backend shader format!"));
		return NULL;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == nullptr) {
		logError(LOGTAG, fmt::format("Failed to load shader from disk! {}", fullPath));
		return NULL;
	}

	SDL_GPUShaderCreateInfo shaderInfo = {
		codeSize,
		static_cast<Uint8*>(code),
		entrypoint,
		format,
		stage,
		samplerCount,
		storageTextureCount,
		storageBufferCount,
		uniformBufferCount,
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == NULL)
	{
		logError(LOGTAG, fmt::format("Failed to create shader!"));
		SDL_free(code);
		return NULL;
	}

	SDL_free(code);
	return shader;
}
