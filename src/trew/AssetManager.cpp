#include "AssetManager.hpp"
#include <trew/Shader.hpp>
#include <trew/Logger.hpp>
#include <SDL3_image/SDL_image.h>

using namespace trew;

constexpr auto LOGTAG = "AssetManager";

AssetManager::AssetManager(std::string rootPath) : rootPath(rootPath) {}

AssetManager::~AssetManager() {
	log(LOGTAG, "AssetManager destructed");
}

SDL_Surface* AssetManager::loadImage(const char* imageFilename, int desiredChannels, int& width, int& height) {
	SDL_Surface* result;
	SDL_PixelFormat format;

	result = IMG_Load(imageFilename);
	if (result == nullptr) {
		logError(LOGTAG, fmt::format("Failed to load image: {}", SDL_GetError()));
		return nullptr;
	}
	if (!SDL_FlipSurface(result, SDL_FLIP_VERTICAL)) {
		logError(LOGTAG, fmt::format("Failed to flip image: {}", SDL_GetError()));
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

std::string AssetManager::loadMap(std::string name) {
	std::string path = fmt::format("maps/{}", name);
	std::string terrainImagePath = fmt::format("{}/terrain.png", path);
	load(terrainImagePath, AssetType::IMAGE);
	return terrainImagePath;
}

ImageSurface* AssetManager::getImage(const char* path) {
	if (auto asset = getAsset(path); asset) {
		auto surface = static_cast<ImageSurface*>(asset);
		return surface;
	}
	return nullptr;
}

ImageSurface* AssetManager::getTerrain(const char* mapName) {
	std::string terrainImagePath = fmt::format("maps/{}/terrain.png", mapName);
	return getImage(terrainImagePath.c_str());
}

Asset* AssetManager::getAsset(const char* path) {
	auto fullPath = fmt::format("{}/{}", rootPath, path);
	if (auto asset = assets.find(fullPath); asset != assets.end())
		return asset->second.get();
	return nullptr;
}
