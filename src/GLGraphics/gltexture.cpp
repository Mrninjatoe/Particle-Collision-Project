#include "GLGraphics/gltexture.hpp"
// Private functions are in the header.

Texture::Texture(const TextureFormat& format, glm::vec2 size) {
	glCreateTextures(GL_TEXTURE_2D, 1, &_texture);
	glTextureStorage2D(_texture, 1, static_cast<GLenum>(format), size.x, size.y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_format = format;
	_size = size;
}

Texture::Texture(const std::string& path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface)
		printf("Texture %s failed to load! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
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

Texture::~Texture() {
	
}