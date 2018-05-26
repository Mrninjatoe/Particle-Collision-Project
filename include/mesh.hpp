#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtx/transform.hpp>

class Mesh {
public:
	const static enum BindingLocation : const int {
		position = 0,
		normal = 1,
		color = 2,
		uv = 3,
	};

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;
	};

	struct Triangle {
		Triangle& mul(glm::mat4& model) {
			for (int i = 0; i < 3; i++) {
				verts[i] = model * verts[i];
			}
			return *this;
		}

		glm::vec4 verts[3];
		glm::ivec4 id;
	};

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, bool hasMX);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, glm::vec3 min, glm::vec3 max, bool hasMX, std::vector<Triangle>& triangles);
	~Mesh();
	std::vector<Vertex> getVertices() { return _vertices; }
	std::vector<unsigned short> getIndices() { return _indices; }
	GLuint getVAO() { return _vao; }
	GLuint getIBO() { return _ibo; }
	glm::vec3 getMin() { return _min; }
	glm::vec3 getMax() { return _max; }
	std::vector<Triangle>& getTriangles() { return _triangles; }
	void setMin(glm::vec4 min) {
		this->_min = glm::vec3(min.x, min.y, min.z);
	}
	void setMax(glm::vec4 max) {
		this->_max = glm::vec3(max.x, max.y, max.z);;
	}
private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned short> _indices;
	glm::vec3 _min, _max;
	std::vector<Triangle> _triangles;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ibo;
	bool _hasModelMX;

	void _setupBuffers();
	void _setupAttributes();
};

struct Box {
	Box(glm::vec3 min, glm::vec3 max) {
		this->min = glm::vec4(min, 1);
		this->max = glm::vec4(max, 1);
	}
	Box() {
		this->min = glm::vec4(0, 0, 0, 1);
		this->max = glm::vec4(0, 0, 0, 1);
	}

	glm::vec4 color = { 1,1,1,0 };
	glm::vec4 min;
	glm::vec4 max;
};

struct Model {
	std::vector<Mesh*> meshes;
	glm::mat4 model;
	Box boundingBox;

	void updateModelMatrix(glm::vec3 trans, glm::vec3 scale) {
		model = glm::translate(trans) * glm::scale(scale);
	}
};