#include "octree.hpp"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
Octree::Octree(Box* region, std::vector<Mesh*>& objects, int id) {
	this->region = region;
	for (auto mesh : objects) {
		this->objects.push_back(mesh);
	}
	this->id = id;
	buildTree();
}

Octree::Octree() {
	region = new Box(glm::vec3(-1, 0, -1) * 0.5f, glm::vec3(1, 1, 1) * 0.5f);
}

Octree::~Octree() {
	
}

void Octree::buildTree() {
	// are there any more objects to contain?
	if (objects.size() < 1) {
		printf("Can't place any here\n", objects.size());
		return;
	}

	glm::vec3 dimensions = region->max - region->min;
	if (dimensions == glm::vec3(0)) {
		dimensions = region->max - region->min;
	}
	// check if the dimensions are smaller than the pre-defined minimum size of the box
	if (dimensions.x <= minSize && dimensions.y <= minSize && dimensions.z <= minSize) {
		printf("Dimensions too small.");
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

	std::vector<std::vector<Mesh*>> octLists;
	octLists.resize(8);
	std::vector<Mesh*> delist;

	//printf("NrOfMeshes:%zu\n", objects.size());
	//for (auto mesh : objects) {
	//	printf("Min: (%f, %f, %f)\nMax: (%f, %f, %f)\n", mesh->getMin().x, mesh->getMin().y, mesh->getMin().z,
	//		mesh->getMax().x, mesh->getMax().y, mesh->getMax().z);
	//	printf("\n");
	//}
	//printf("\n\n");


	for (auto mesh : objects) {
		glm::vec3 meshMin = mesh->getMin();
		glm::vec3 meshMax = mesh->getMax();
		printf("%f,%f,%f\n", meshMin.x, meshMin.y, meshMin.z);
		printf("%f,%f,%f\n\n", meshMax.x, meshMax.y, meshMax.z);
		if (meshMin != meshMax); {
			for (int i = 0; i < 8; i++) {
				if (octant[i].min.x <= meshMax.x && octant[i].max.x >= meshMin.x &&
					octant[i].min.y <= meshMax.y && octant[i].max.y >= meshMin.y &&
					octant[i].min.z <= meshMax.z && octant[i].max.z >= meshMin.z) {
					octLists[i].push_back(mesh);
					delist.push_back(mesh);
					break;
				}
			}
		}
	}

	for (auto mesh : delist) {
		objects.erase(std::remove_if(
			objects.begin(), objects.end(),
			[mesh](Mesh* m) {
			return m == mesh;
		}), objects.end());
	}


	for (int i = 0; i < 8; i++) {
		if (octLists[i].size() != 0) {
			//printf("%u\n", i);
			children[i] = new Octree(&octant[i], octLists[i], id + 1);
		}
		else
			children[i] = nullptr;
	}
	treeBuilt = true;
	printf("%i\n", id);
}

void Octree::renderOctree(ShaderProgram* shader, Octree* current, Mesh* box) {
	if (current->treeBuilt) {
		for (int i = 0; i < 8; i++) {
				shader->setValue(1, (current->region->max + current->region->min) * 0.5f);
				shader->setValue(2, glm::scale(current->region->max - current->region->min));
				shader->setValue(9, glm::vec4(current->region->max / (float)i, 0));
				glDrawElements(GL_TRIANGLES, box->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
				if(current->children[i] != nullptr)
					renderOctree(shader, current->children[i], box);
		}
	}

	/*shader->setValue(1, (octree->region->max + octree->region->min) * 0.5f);
	shader->setValue(2, glm::scale(octree->region->max - octree->region->min));
	shader->setValue(9, glm::vec4(octree->region->max, 0));
	glDrawElements(GL_TRIANGLES, _octreeBox->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
	for (auto octan : octree->children) {
		shader->setValue(1, (octan->region->max + octan->region->min) * 0.5f);
		shader->setValue(2, glm::scale(octan->region->max - octan->region->min));
		shader->setValue(9, glm::vec4(octan->region->max, 0));
		glDrawElements(GL_TRIANGLES, _octreeBox->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
	}*/
}