#pragma once
#include <glad/glad.h>
#include <string>
#include "mesh.hpp"

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();
	//Mesh& loadMesh(const std::string& path);
	Model loadMesh(const char* path);
	Mesh* getQuad();
private:

};