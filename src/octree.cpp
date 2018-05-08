#include "octree.hpp"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
Octree::Octree(Box* region, std::vector<Model> objects, int id, int depth) {
	this->region = region;
	for (auto model : objects) {
		this->objects.push_back(model);
	}
	this->id = id;
	this->depth = depth;
	buildTree();
}

Octree::Octree() {
	region = new Box(glm::vec3(-1, 0, -1) * 0.5f, glm::vec3(1, 1, 1) * 0.5f);
}

Octree::~Octree() {
	
}

void Octree::buildTree() {
	// are there any more objects to contain?
	
	if (objects.size() < 0) {
		printf("Can't place any here.\n", objects.size());
		return;
	}
	
	if (depth == 4)
		return;

	glm::vec3 dimensions = region->max - region->min;
	if (dimensions == glm::vec3(0)) {
		dimensions = region->max - region->min;
	}
	// check if the dimensions are smaller than the pre-defined minimum size of the box
	if (dimensions.x <= minSize && dimensions.y <= minSize && dimensions.z <= minSize) {
		printf("Dimensions too small.\n");
		return;
	}
	glm::vec3 half = dimensions * 0.5f;
	glm::vec3 center = region->min + half;

	Box* octant = new Box[8];
	octant[0] = Box(region->min, center);
	octant[1] = Box(glm::vec3(center.x, region->min.y, region->min.z), glm::vec3(region->max.x, center.y, center.z)); 
	octant[2] = Box(glm::vec3(center.x, region->min.y, center.z), glm::vec3(region->max.x, center.y, region->max.z));
	octant[3] = Box(glm::vec3(region->min.x, region->min.y, center.z), glm::vec3(center.x, center.y, region->max.z));
	octant[4] = Box(glm::vec3(region->min.x, center.y, region->min.z), glm::vec3(center.x, region->max.y, center.z));
	octant[5] = Box(glm::vec3(center.x, center.y, region->min.z), glm::vec3(region->max.x, region->max.y, center.z));
	octant[6] = Box(center, region->max);
	octant[7] = Box(glm::vec3(region->min.x, center.y, center.z), glm::vec3(center.x, region->max.y, region->max.z));

	std::vector<std::vector<Model>> octLists;
	octLists.resize(8);
	std::vector<Model> delist;

	for (auto model : objects) {
		for (auto mesh : model.meshes) {
			glm::vec3 meshMin = mesh->getMin();
			glm::vec3 meshMax = mesh->getMax();
			if (meshMin != meshMax); {
				for (int i = 0; i < 8; i++) {
					if (octant[i].min.x <= meshMax.x && octant[i].max.x >= meshMin.x &&
						octant[i].min.y <= meshMax.y && octant[i].max.y >= meshMin.y &&
						octant[i].min.z <= meshMax.z && octant[i].max.z >= meshMin.z) {
						Model newmod;
						newmod.boundingBox = model.boundingBox;
						newmod.model = model.model;
						newmod.meshes.push_back(mesh);

						octLists[i].push_back(newmod);
						delist.push_back(newmod);
						//break;
					}
				}
			}
		}
	}
	//printf("Done adding to children\n\n");

	//printf("Before: %zu\n", objects.size());
	for (auto& model : delist) {
		objects.erase(std::remove_if(
			objects.begin(), objects.end(),
			[model](const Model& m) {
			return m.boundingBox.max == model.boundingBox.max;
		}), objects.end());
	}
	//printf("After: %zu\n\n", objects.size());

	for (int i = 0; i < 8; i++) {
		if (octLists[i].size() != 0) {
			children[i] = new Octree(&octant[i], octLists[i], id + 1, depth + 1);
		}
		else
			isLeaf = 1;
	}
	treeBuilt = true;
	//printf("Tree id: %i\n", id);
	printf("Depth: %i\n", depth);
}

void Octree::renderOctree(ShaderProgram* shader, Octree* current, Mesh* box) {
	glm::vec4 color;
	switch (current->depth) {
	case 0:
		color = {1,1,1,0};
		break;
	case 1:
		color = {1,0,0,0 };
		break;
	case 2:
		color = { 0,1,0,0 };
		break;
	case 3:
		color = { 0,0,1,0 };
		break;
	case 4:
		color = { 0,1,1,0 };
		break;
		
	}
	if (current->treeBuilt) {
		if (current->isLeaf) {
			shader->setValue(1, (current->region->max + current->region->min) * 0.5f);
			shader->setValue(2, glm::scale(current->region->max - current->region->min));
			shader->setValue(9, color);
			glLineWidth(4 - current->depth);
			glDrawElements(GL_TRIANGLES, box->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
		}
		for (int i = 0; i < 8; i++)
			if(!current->isLeaf)
				renderOctree(shader, current->children[i], box);
	}
}

void Octree::getNrOfNodes(Octree* curr, int& count) {
	if (curr->treeBuilt) {
		for (int i = 0; i < 8; i++) {
			if (curr->isLeaf) {
				count++;
				return;
			}
			else
				getNrOfNodes(curr->children[i], count);
		}
	}
}