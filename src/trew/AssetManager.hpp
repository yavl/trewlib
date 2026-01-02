#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <trew/drawables/ImageSurface.hpp>

struct SDL_Surface;

namespace trew {
	class Shader;
	class Asset;
	enum class AssetType {
		IMAGE
	};

	class AssetManager {
	public:
		AssetManager(std::string rootPath);
		~AssetManager();
		void load(std::string path, AssetType type);
		std::string loadMap(std::string name);
		ImageSurface* getImage(const char* path);
		ImageSurface* getTerrain(const char* mapName);
	private:
		Asset* getAsset(const char* path);
		SDL_Surface* loadImage(const char* imageFilename, int desiredChannels, int& width, int& height);

		std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
		std::string rootPath;
	};
}
