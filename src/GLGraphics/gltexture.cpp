#include "GLGraphics/gltexture.hpp"
// Private functions are in the header.

Texture::Texture(const TextureFormat format, glm::vec2 size) {
	_format = format;
	_size = size;
	_setData(nullptr);
}

Texture::Texture(const std::string& path) {
	printf("%s\n\n", path.c_str());
	std::string temp = "assets/textures/"+ path;
	SDL_Surface* surface = IMG_Load(temp.c_str());
	if (!surface)
		printf("Texture %s failed to load! SDL_image Error: %s\n", temp.c_str(), SDL_GetError());
	else {
		SDL_Surface* newSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		if (!newSurf)
			printf("Unknown pixelformat. SDL_image Error: %s\n", SDL_GetError());
		else {
			SDL_FreeSurface(surface);
			surface = newSurf;
			_format = TextureFormat::RGBA8;
			_size = glm::vec2(surface->w, surface->h);
			_setData(surface->pixels);
			SDL_FreeSurface(surface);
		}
	}
}

Texture::Texture(std::string files[6]) {
	glGenTextures(6, &_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	
	for (int i = 0; i < files->length(); i++) {
		std::string path = files[i] + ".png";
		SDL_Surface* surface = IMG_Load(path.c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

Texture::~Texture() {
	
}