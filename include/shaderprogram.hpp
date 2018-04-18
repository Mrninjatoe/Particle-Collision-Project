#pragma once
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ShaderProgram {
public:
	const static enum ShaderType : const int {
		VertexShader = GL_VERTEX_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		ComputeShader = GL_COMPUTE_SHADER
	};
	ShaderProgram(const std::string& label);
	~ShaderProgram();
	ShaderProgram& attachShader(GLenum type, const std::string& path);
	void useProgram();
	void finalize();

	void setValue(int32_t id, int32_t value) { glProgramUniform1i(_program, id, value); }
	void setValue(int32_t id, uint32_t value) { glProgramUniform1ui(_program, id, value); }
	void setValue(int32_t id, float value) { glProgramUniform1f(_program, id, value); }
	void setValue(int32_t id, const glm::ivec2& value) { glProgramUniform2iv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::ivec3& value) { glProgramUniform3iv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::ivec4& value) { glProgramUniform4iv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::vec2& value) { glProgramUniform2fv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::vec3& value) { glProgramUniform3fv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::vec4& value) { glProgramUniform4fv(_program, id, 1, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::mat2& value) { glProgramUniformMatrix2fv(_program, id, 1, false, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::mat3& value) { glProgramUniformMatrix3fv(_program, id, 1, false, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::mat4& value) { glProgramUniformMatrix4fv(_program, id, 1, false, glm::value_ptr(value)); }
	void setValue(int32_t id, const glm::mat4& value, int i, std::string path) { glProgramUniformMatrix4fv(glGetUniformLocation(_program, (path + "[" + std::to_string(i) + "]").c_str()), id, 1, false, glm::value_ptr(value)); }
private:
	GLuint _program;
};