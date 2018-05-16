#include "octree.hpp"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>


Octree::Octree(Box region, std::vector<Model> objects, int id, int depth) {
	this->region = region;
	this->id = id;
	this->depth = depth;
	buildTree();
}

Octree::Octree(Box region, std::vector<Mesh::Triangle> triangles, int id, int depth) {
	this->region = region;
	//this->triangles = triangles;
	for (auto tri : triangles) {
		this->triangles.push_back(tri);
		this->triangleIDs.push_back(tri.id.x);
	}
	this->id = id;
	this->depth = depth;
	buildTree();
}

Octree::Octree() {
	region = Box(glm::vec3(-1, 0, -1) * 0.5f, glm::vec3(1, 1, 1) * 0.5f);
}

Octree::~Octree() {
	
}

void Octree::buildTree() {
	// are there any more objects to contain?
	if (triangles.size() < 1) {
		printf("No triangles\n", triangles.size());
		return;
	}
	
	if (depth == MAX_DEPTH) {
		isLeaf = true;
		return;
	}

	glm::vec3 dimensions = region.max - region.min;
	glm::vec3 half = dimensions * 0.5f;
	glm::vec3 center = glm::vec3(region.min) + half;

	Box* octant = new Box[8];
	octant[0] = Box(region.min, center);
	//octant[0].color = {1,1,1,0};
	octant[1] = Box(glm::vec3(center.x, region.min.y, region.min.z), glm::vec3(region.max.x, center.y, center.z));
	//octant[1].color = {0,1,1,0};
	octant[2] = Box(glm::vec3(center.x, region.min.y, center.z), glm::vec3(region.max.x, center.y, region.max.z));
	//octant[2].color = { 0,0,1,0 };
	octant[3] = Box(glm::vec3(region.min.x, region.min.y, center.z), glm::vec3(center.x, center.y, region.max.z));
	//octant[3].color = {0.5, 0.1, 0.1, 0};
	octant[4] = Box(glm::vec3(region.min.x, center.y, region.min.z), glm::vec3(center.x, region.max.y, center.z));
	//octant[4].color = {0.1, 0.5, 0.1, 0};
	octant[5] = Box(glm::vec3(center.x, center.y, region.min.z), glm::vec3(region.max.x, region.max.y, center.z));
	//octant[5].color = { 0.1, 0.1, 0.5, 0 };
	octant[6] = Box(center, region.max);
	//octant[6].color = { 0.1, 0.5, 0.5, 0 };
	octant[7] = Box(glm::vec3(region.min.x, center.y, center.z), glm::vec3(center.x, region.max.y, region.max.z));
	//octant[7].color = { 0.9, 0.5, 0.5, 0 };

	std::vector<std::vector<Mesh::Triangle>> octLists;
	octLists.resize(8);
	std::vector<Mesh::Triangle> delist;
	for (int i = 0; i < 8; i++) {
	int count = 0;
		for (auto tris : triangles) {
			if (aabbVsTriangle(octant[i], tris)) {
				octLists[i].push_back(tris);
				delist.push_back(tris);
			}
			count++;
		}
	}

	for (auto& tris : delist) {
		triangles.erase(std::remove_if(
			triangles.begin(), triangles.end(),
			[tris](const Mesh::Triangle& t) {
			return t.id == tris.id;
		}), triangles.end());

		int id = tris.id.x;

		triangleIDs.erase(std::remove_if(
			triangleIDs.begin(), triangleIDs.end(),
			[id](const int& i) {
			return i == id;	
		}), triangleIDs.end());
	}

	for (int i = 0; i < 8; i++) {
		if (octLists[i].size()) {
			int temp = octLists[i].size() * i;
			children[i] = new Octree(octant[i], octLists[i], id, depth + 1);
			trisIndices.push_back(i);
		}
	}
	treeBuilt = true;
}

void Octree::renderOctree(ShaderProgram* shader, Octree* current, Mesh* box) {
	glm::vec4 color = {0.5, 0.7, 0.1, 0.f};
	if (current->treeBuilt) {
		shader->setValue(1, glm::vec3((current->region.max + current->region.min) * 0.5f));
		shader->setValue(2, glm::scale(glm::vec3(current->region.max - current->region.min)));
		shader->setValue(9, glm::vec4(1, 0, 0.5, 1));
		glLineWidth(2);
		glDrawElements(GL_LINES, box->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
		for (int i = 0; i < current->trisIndices.size(); i++) {
			renderOctree(shader, current->children[current->trisIndices[i]], box);
		}
	}
}

void Octree::getNrOfNodes(Octree* curr, int& count) {
	for (int i = 0; i < curr->trisIndices.size(); i++) {
		getNrOfNodes(curr->children[curr->trisIndices[i]], count);
	}
	if (curr->treeBuilt) {
		count++;
	}
	if (curr->isLeaf) {
		count++;
		printf("Number of triangles at depth %i for leaf node[%i]: %i\n", curr->depth, curr->id, curr->triangles.size());
		return;
	}
}

void Octree::getNodes(Octree* curr, std::vector<Node>& nodes) {
	Node temp;
	temp.info.x = curr->triangleIDs.size();
	temp.info.y = curr->trisIndices.size();
	temp.info.z = curr->isLeaf;
	temp.info.w = curr->depth;
	temp.region = curr->region;
	
	for (int i = 0; i < curr->trisIndices.size(); i++) {
		temp.childrenIdx[i].x = curr->trisIndices[i];
	}
	
	for (int i = 0; i < curr->triangleIDs.size(); i++) {
		temp.triangleRefs[i].x = curr->triangleIDs[i];
	}
	nodes.push_back(temp);
	for (int i = 0; i < curr->trisIndices.size(); i++) {
		getNodes(curr->children[curr->trisIndices[i]], nodes);
	}
}