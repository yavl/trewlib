#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <memory>
#include <trew/drawables/ImageSurface.hpp>

struct SDL_Surface;

struct SDL_Surface;

namespace trew {
	class Shader;
	class Asset;
	class ImageSurface;
	enum class AssetType {
		IMAGE
	};

	class AssetManager {
	public:
		AssetManager(std::string rootPath);
		~AssetManager();
		void load(std::string path, AssetType type);
		ImageSurface* getImage(const char* path);
	private:
		Asset* getAsset(const char* path);
		SDL_Surface* loadImage(const char* imageFilename, int desiredChannels, int& width, int& height);

		std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
		std::string rootPath;
	};
}
