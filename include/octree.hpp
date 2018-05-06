#pragma once
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "shaderprogram.hpp"
#include "mesh.hpp"

struct Octree {
	Octree(Box* region, std::vector<Mesh*>& meshes, int id);
	Octree();
	virtual ~Octree();
	void buildTree();
	void renderOctree(ShaderProgram* shader, Octree* current, Mesh* box);
	

	Box* region;
	std::vector<Mesh*> objects;
	Octree* children[8]; // Max 8 children.
	//BYTE activeNodes = 0;
	const float minSize = 0.3; // 1x1x1 cube.
	int id = -1;
	Octree* parent;
	bool treeReady = false;
	bool treeBuilt = false;
};