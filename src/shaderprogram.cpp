#include <fstream>
#include "shaderprogram.hpp"

ShaderProgram::ShaderProgram(const std::string& label) {
	_program = glCreateProgram();
	glObjectLabel(GL_PROGRAM, _program, -1, label.c_str());
}

ShaderProgram::~ShaderProgram() {
	
}

ShaderProgram& ShaderProgram::attachShader(GLenum type, const std::string& path) {
	std::string shaderCode;
	std::ifstream shaderStream(path.c_str(), std::ios::in);
	if (shaderStream.is_open()) {
		std::string line = "";
		while (std::getline(shaderStream, line))
			shaderCode += "\n" + line;
		shaderStream.close();
	}
	else
		printf("Could not open file with path: %s\n", path.c_str());

	char const* srcPointer = shaderCode.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &srcPointer, NULL);
	glCompileShader(shader);
	glAttachShader(_program, shader);
	glDeleteShader(shader);

	return *this;
}

void ShaderProgram::useProgram() {
	glUseProgram(_program);
}

void ShaderProgram::finalize() {
	glLinkProgram(_program);
	GLint result;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		char msg[10240];
		glGetProgramInfoLog(_program, 10240, NULL, msg);
		fprintf(stderr, "Linking program failed:\n%s\n", msg);
		abort();
	}
}