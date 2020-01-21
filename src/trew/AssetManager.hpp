#pragma once

#include <stb_image.h>
#include <string>
#include <unordered_map>
#include <optional>
#include <memory>

namespace trew {
	class Texture;
	class Shader;
	class Asset;
	enum class AssetType {
		SHADER,
		TEXTURE
	};

	class AssetManager {
	public:
		AssetManager(std::string jsonFilePath);
		virtual ~AssetManager() = default;
		virtual void load(std::string path, AssetType type);
		virtual std::optional<Shader*> getShader(std::string path);
		virtual std::optional<Texture*> getTexture(std::string path);
	private:
		virtual std::optional<Asset*> getAsset(std::string path);

		std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
		std::string assetsJsonFilePath;
	};
}