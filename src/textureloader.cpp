#include "textureloader.hpp"

TextureLoader::TextureLoader() {
	
}

TextureLoader::~TextureLoader() {
	_loadedTextures.clear();
}

std::shared_ptr<Texture> TextureLoader::loadTexture(const std::string& path) {
	return _loadedTextures[path] = std::make_shared<Texture>(path);
}

GLuint TextureLoader::loadCubeMap(std::vector<std::string> filePaths) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	SDL_Surface* surface;
	for (int i = 0; i < filePaths.size(); i++) {
		std::string path = "assets/textures/" + filePaths[i];
		surface = IMG_Load(path.c_str());
		if(surface) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			//SDL_FreeSurface(surface);
		}
		else {
			printf("FAILED TO LOAD CUBEMAP!");
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
