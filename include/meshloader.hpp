#pragma once
#include <glad/glad.h>
#include <string>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.hpp"

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene, Model& models);
	Model loadMesh(const char* path, bool hasModelMX);
	Mesh* getQuad();
	Mesh* getCube();
private:

};