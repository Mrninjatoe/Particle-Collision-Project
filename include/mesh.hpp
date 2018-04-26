#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
public:
	const static enum BindingLocation : const int {
		position = 0,
		normal = 1,
		color = 2,
		uv = 3
	};

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;
	};

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices);
	~Mesh();
	std::vector<Vertex> getVertices() { return _vertices; }
	std::vector<unsigned short> getIndices() { return _indices; }
	GLuint getVAO() { return _vao; }
	GLuint getIBO() { return _ibo; }
private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned short> _indices;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ibo;

	void _setupBuffers();
	void _setupAttributes();
};

struct Model {
	std::vector<Mesh*> meshes;
	glm::mat4 model;
};