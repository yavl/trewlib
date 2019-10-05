#include "AssetManager.hpp"
#include <cassert>
#include "FileManager.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <fmt/core.h>

using namespace pm;

AssetManager::AssetManager(std::string path) {
}

AssetManager::~AssetManager() {
	for (auto& [key, value] : assets) {
		if (getAsset(key).has_value()) {
			delete getAsset(key).value();
		}
	}
}

void AssetManager::load(std::string path, AssetType type) {
	const char* message = fmt::format("resource {} already loaded", path).c_str();
	assert(!getAsset(path).has_value() && message);
	switch (type) {
	case AssetType::SHADER: {
		auto vertPathStr = fmt::format("{}.vert", path);
		auto fragPathStr = fmt::format("{}.frag", path);
		const char* vertPath = vertPathStr.c_str();
		const char* fragPath = fragPathStr.c_str();
		FileManager vert(vertPath);
		FileManager frag(fragPath);
		auto ownerVert = vert.asString();
		auto ownerFrag = frag.asString();
		auto vertStr = ownerVert.c_str();
		auto fragStr = ownerFrag.c_str();
		auto shader = new Shader(vertStr, fragStr);
		shader->setPathInfos(vertPathStr, fragPathStr);
		shader->compile();
		assets[path] = shader;
		break;
	}
	case AssetType::TEXTURE: {
		auto texture = new Texture(path);
		assets[path] = texture;
		break;
	}
	default:
		break;
	}
}

std::optional<Shader*> AssetManager::getShader(std::string path) {
	if (auto asset = getAsset(path); asset.has_value()) {
		auto shader = static_cast<Shader*>(asset.value());
		return shader;
	}
	return std::nullopt;
}

std::optional<Texture*> AssetManager::getTexture(std::string path) {
	if (auto asset = getAsset(path); asset.has_value()) {
		auto texture = static_cast<Texture*>(asset.value());
		return texture;
	}
	return std::nullopt;
}

std::optional<Asset*> AssetManager::getAsset(std::string path) {
	if (auto asset = assets.find(path); asset != assets.end())
		return asset->second;
	return std::nullopt;
}
