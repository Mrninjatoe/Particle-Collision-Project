#pragma once
#include <string>
#include <memory>
#include <map>
#include "GLGraphics/gltexture.hpp"

class TextureLoader {
public:
	TextureLoader();
	~TextureLoader();
	std::shared_ptr<Texture> loadTexture(const std::string& path);

private:
	std::map<std::string, std::shared_ptr<Texture>> _loadedTextures;
};