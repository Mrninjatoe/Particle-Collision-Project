#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, bool hasMX) {
	_vertices = vertices;
	_indices = indices;
	_hasModelMX = hasMX;
	_setupBuffers();
	_setupAttributes();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 min, glm::vec3 max, bool hasMX, std::vector<Triangle>& triangles) {
	_vertices = vertices;
	_indices = indices;
	_min = min;
	_max = max;
	_triangles = triangles;
	_hasModelMX = hasMX;
	_setupBuffers();
	_setupAttributes();
}


Mesh::~Mesh() {
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_vao);
}

void Mesh::_setupBuffers() {
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);
}

void Mesh::_setupAttributes() {
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(BindingLocation::position);
	glVertexAttribPointer(BindingLocation::position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(BindingLocation::normal);
	glVertexAttribPointer(BindingLocation::normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::normal));

	glEnableVertexAttribArray(BindingLocation::color);
	glVertexAttribPointer(BindingLocation::color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::color));

	glEnableVertexAttribArray(BindingLocation::uv);
	glVertexAttribPointer(BindingLocation::uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::uv));

	glBindVertexArray(0);
}