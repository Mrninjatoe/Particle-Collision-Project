#pragma once
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "shaderprogram.hpp"
#include "mesh.hpp"

struct Octree {
	Octree(Box* region, std::vector<Model> meshes, int id, int depth);
	Octree();
	virtual ~Octree();
	void buildTree();
	void renderOctree(ShaderProgram* shader, Octree* current, Mesh* box);
	
	void getNrOfNodes(Octree* curr, int& count);

	Box* region;
	std::vector<Model> objects;
	Octree* children[8]; // Max 8 children.
	//BYTE activeNodes = 0;
	int depth = 0;
	int isLeaf = 0;


	const float minSize = 0.1f; // 1x1x1 cube.
	bool treeBuilt = false;
	int id = -1;

	inline double _fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}
};