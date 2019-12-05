#pragma once

#include <stb_image.h>
#include <string>
#include <unordered_map>
#include <optional>

namespace pm {
	class Texture;
	class Shader;
	class Asset;
	enum class AssetType {
		SHADER,
		TEXTURE
	};

	class AssetManager {
	public:
		AssetManager(std::string path);
		virtual ~AssetManager();
		virtual void load(std::string path, AssetType type);
		virtual std::optional<Shader*> getShader(std::string path);
		virtual std::optional<Texture*> getTexture(std::string path);
	private:
		virtual std::optional<Asset*> getAsset(std::string path);

		std::unordered_map<std::string, Asset*> assets;
		std::string assetsPath;
		std::string shadersPath;
	};
}