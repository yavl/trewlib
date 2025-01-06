#include "AssetManager.hpp"
#include <trew/FileHandle.hpp>
#include <trew/Shader.hpp>
#include <trew/drawables/impl_glfw/Texture.hpp>
#include <trew/Logger.hpp>
#include <cassert>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

using namespace trew;

#define LOGTAG "AssetManager"

AssetManager::AssetManager(std::string rootPath) : rootPath(rootPath) {}

AssetManager::~AssetManager() {
	log(LOGTAG, "AssetManager destructed");
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
	auto fullPath = fmt::format("{}/{}", rootPath, path);
	if (auto asset = assets.find(fullPath); asset != assets.end())
		return asset->second.get();
	return std::nullopt;
}
