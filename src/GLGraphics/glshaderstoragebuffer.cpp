#include "GLGraphics/glshaderstoragebuffer.hpp"


ShaderStorageBuffer::ShaderStorageBuffer(size_t size, GLenum dataMode) {
	glGenBuffers(1, &_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, dataMode);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
	glDeleteBuffers(1, &_ssbo);
}

void ShaderStorageBuffer::bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
}

void ShaderStorageBuffer::bindBase(GLuint loc) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, _ssbo);
}