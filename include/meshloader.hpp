#pragma once
#include <glad/glad.h>
#include <string>
#include "mesh.hpp"

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();
	//Mesh& loadMesh(const std::string& path);
	Mesh* getQuad();
private:

};