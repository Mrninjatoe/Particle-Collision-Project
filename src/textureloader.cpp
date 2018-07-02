#include "textureloader.hpp"

TextureLoader::TextureLoader() {
	
}

TextureLoader::~TextureLoader() {
	_loadedTextures.clear();
}

std::shared_ptr<Texture> TextureLoader::loadTexture(const std::string& path) {
	return _loadedTextures[path] = std::make_shared<Texture>(path);
}