#pragma once
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <list>
#include "shaderprogram.hpp"
#include "mesh.hpp"
#define MAX_DEPTH 5
struct Octree {
	Octree(Box region, std::vector<Model> meshes, int id, int depth);
	Octree(Box region, std::vector<Mesh::Triangle> triangles, int id, int depth);
	Octree();
	virtual ~Octree();
	void buildTree();
	void renderOctree(ShaderProgram* shader, Octree* current, Mesh* box);
	void getNrOfNodes(Octree* curr, int& count);
	// Due to std430 buffers have 16 byte alignment, adding padding to match it.
	struct Node {
		Box region; // 48 bytes
		glm::ivec4 info; // Node information: x = nrOfTriangles, y = number of active children, z = is node a leaf node? 
		glm::ivec4 triangleRefs[2048]; // 
	};

	void getNodes(Octree* curr, std::vector<Node>& nodes, std::list<Octree*> toProcess);
	

	Box region;
	std::vector<Mesh::Triangle> triangles;
	std::vector<int> trisIndices;
	std::vector<int> triangleIDs;
	Octree* children[8]; // Max 8 children.
	//BYTE activeNodes = 0;
	int depth = 0;
	int isLeaf = 0;

	const float minSize = 0.01; // 1x1x1 cube.
	bool treeBuilt = false;
	int id = -1;

	inline double _fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	inline bool aabbVsTriangle(Box& aabb, Mesh::Triangle& tris) {
		bool inside = false;
		for (int i = 0; i < 3; i++) {
			if (aabb.min.x <= tris.verts[i].x && aabb.max.x >= tris.verts[i].x &&
				aabb.min.y <= tris.verts[i].y && aabb.max.y >= tris.verts[i].y &&
				aabb.min.z <= tris.verts[i].z && aabb.max.z >= tris.verts[i].z) {
				inside = true;
			}
		}

		return inside;
	}
};