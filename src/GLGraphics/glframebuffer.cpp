#include "GLGraphics/glframebuffer.hpp"
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

GLFrameBuffer::GLFrameBuffer() {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

GLFrameBuffer::~GLFrameBuffer() {
	glDeleteFramebuffers(1, &_fbo);
}

GLFrameBuffer& GLFrameBuffer::addTexture(unsigned int key, std::shared_ptr<Texture> texture) {
	_textures[key] = texture;
	glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + key), GL_TEXTURE_2D, texture->getLocation(), 0);

	return *this;
}

GLFrameBuffer& GLFrameBuffer::addTexture(unsigned int key, Texture::TextureFormat type, unsigned int x, unsigned int y) {
	_textures[key] = std::make_shared<Texture>(type, glm::vec2(x, y));
	glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + key), GL_TEXTURE_2D, _textures[key]->getLocation(), 0);

	return *this;
}

GLFrameBuffer & GLFrameBuffer::addDepth(unsigned int key, unsigned int x, unsigned int y){
	_depth = std::make_shared<Texture>(Texture::TextureFormat::Depth32f, glm::vec2(x, y));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->getLocation(), 0);
	glDrawBuffer(GL_NONE);
	return *this;
}

GLFrameBuffer& GLFrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	return *this;
}

GLFrameBuffer& GLFrameBuffer::bindDepth(unsigned int key){
	_depth->bind(key);
	return *this;
}

void GLFrameBuffer::finalize(){
	std::vector<GLenum> attachments;
	for (auto text : _textures) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + text.first);
	}
	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer properly setup!\n");
}
