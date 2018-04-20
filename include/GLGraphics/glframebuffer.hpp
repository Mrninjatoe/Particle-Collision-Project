#pragma once
#include <map>
#include <memory>
#include "GLGraphics/gltexture.hpp"

class GLFrameBuffer {
public:
	GLFrameBuffer();
	virtual ~GLFrameBuffer();
	GLFrameBuffer& addTexture(unsigned int key, std::shared_ptr<Texture> texture);
	GLFrameBuffer& addTexture(unsigned int key, Texture::TextureFormat type, unsigned int x, unsigned int y);
	GLFrameBuffer& addDepth(unsigned int key, unsigned int x, unsigned int y);
	GLFrameBuffer& bind();
	GLFrameBuffer& bindDepth(unsigned int key);
	inline std::shared_ptr<Texture>& operator[](size_t key) {
		return _textures[key];
	}
	void finalize();
private:
	unsigned int _fbo;
	std::map<unsigned int, std::shared_ptr<Texture>> _textures;
	std::shared_ptr<Texture> _depth;
};