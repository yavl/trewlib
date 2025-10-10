#pragma once

#include <stb_image.h>
#include <string>
#include <unordered_map>
#include <optional>
#include <memory>

struct SDL_Surface;

namespace trew {
	class Texture;
	class Shader;
	class Asset;
	class ImageSurface;
	enum class AssetType {
		SHADER,
		TEXTURE,
		IMAGE
	};

	class AssetManager {
	public:
		AssetManager(std::string rootPath);
		~AssetManager();
		void load(std::string path, AssetType type);
		Shader* getShader(const char* path);
		Texture* getTexture(const char* path);
		ImageSurface* getImage(const char* path);
	private:
		Asset* getAsset(const char* path);
		SDL_Surface* loadImage(const char* imageFilename, int desiredChannels, int& width, int& height);

		std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
		std::string rootPath;
	};
}