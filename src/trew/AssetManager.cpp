#include "AssetManager.hpp"
#include <trew/FileHandle.hpp>
#include <trew/Shader.hpp>
#include <trew/drawables/impl_opengl/GLTexture.hpp>
#include <trew/Logger.hpp>
#include <cassert>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <SDL3_image/SDL_image.h>
#include <trew/drawables/ImageSurface.hpp>

using namespace trew;

#define LOGTAG "AssetManager"

AssetManager::AssetManager(std::string rootPath) : rootPath(rootPath) {}

AssetManager::~AssetManager() {
	log(LOGTAG, "AssetManager destructed");
}

SDL_Surface* AssetManager::loadImage(const char* imageFilename, int desiredChannels, int& width, int& height) {
	SDL_Surface* result;
	SDL_PixelFormat format;

	result = IMG_Load(imageFilename);
	if (result == nullptr) {
		SDL_Log("Failed to load image: %s", SDL_GetError());
		return nullptr;
	}
	if (!SDL_FlipSurface(result, SDL_FLIP_VERTICAL)) {
		SDL_Log("Failed to flip image: %s", SDL_GetError());
		return nullptr;
	}

	if (desiredChannels == 4) {
		format = SDL_PIXELFORMAT_ABGR8888;
	}
	else {
		SDL_assert(!"Unexpected desiredChannels");
		SDL_DestroySurface(result);
		return nullptr;
	}
	if (result->format != format) {
		SDL_Surface* next = SDL_ConvertSurface(result, format);
		SDL_DestroySurface(result);
		result = next;
	}

	width = result->w;
	height = result->h;

	return result;
}

void AssetManager::load(std::string path, AssetType type) {
	path = fmt::format("{}/{}", rootPath, path);
	if (assets.count(path) > 0) {
		log(LOGTAG, fmt::format("resource <{}> was already loaded", path).c_str());
		return;
	}
	switch (type) {
	case AssetType::SHADER: {
		auto vertPathStr = fmt::format("{}.vert", path);
		auto fragPathStr = fmt::format("{}.frag", path);
		const char* vertPath = vertPathStr.c_str();
		const char* fragPath = fragPathStr.c_str();
		FileHandle vert(vertPath);
		FileHandle frag(fragPath);
		auto ownerVert = vert.asString();
		auto ownerFrag = frag.asString();
		auto vertStr = ownerVert.c_str();
		auto fragStr = ownerFrag.c_str();
		auto shader = std::make_unique<Shader>(vertStr, fragStr);
		shader->setPathInfos(vertPathStr, fragPathStr);
		shader->compile();
		assets[path] = std::move(shader);
		break;
	}
	case AssetType::TEXTURE: {
		auto texture = std::make_unique<Texture>(path);
		assets[path] = std::move(texture);
		break;
	}
	case AssetType::IMAGE: {
		int width = 0;
		int height = 0;
		auto sdlSurface = loadImage(path.c_str(), 4, width, height);
		auto surface = std::make_unique<ImageSurface>(sdlSurface, width, height);
		assets[path] = std::move(surface);
		break;
	}
	default:
		break;
	}
}

Shader* AssetManager::getShader(const char* path) {
	if (auto asset = getAsset(path); asset) {
		auto shader = static_cast<Shader*>(asset);
		return shader;
	}
	return nullptr;
}

Texture* AssetManager::getTexture(const char* path) {
	if (auto asset = getAsset(path); asset) {
		auto texture = static_cast<Texture*>(asset);
		return texture;
	}
	return nullptr;
}

ImageSurface* AssetManager::getImage(const char* path) {
	if (auto asset = getAsset(path); asset) {
		auto surface = static_cast<ImageSurface*>(asset);
		return surface;
	}
	return nullptr;
}

Asset* AssetManager::getAsset(const char* path) {
	auto fullPath = fmt::format("{}/{}", rootPath, path);
	if (auto asset = assets.find(fullPath); asset != assets.end())
		return asset->second.get();
	return nullptr;
}
