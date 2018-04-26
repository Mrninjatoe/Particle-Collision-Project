#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class ShaderStorageBuffer {
private:
	GLuint _ssbo;

public:
	ShaderStorageBuffer(size_t size, GLenum dataMode = GL_DYNAMIC_COPY);
	template <typename T>
	ShaderStorageBuffer(GLenum dataMode, std::vector<T>& data) {
		glGenBuffers(1, &_ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * data.size(), &data[0], dataMode);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	virtual ~ShaderStorageBuffer();
	void bind();
	void bindBase(GLuint loc);
	template <typename T>
	void setData(const std::vector<T>& data) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T) * data.size(), &data[0]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
};