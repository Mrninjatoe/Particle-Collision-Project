#include "meshloader.hpp"
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


MeshLoader::MeshLoader() {
	const aiScene* asd = aiImportFile("bb8.FBX", aiProcessPreset_TargetRealtime_MaxQuality);
	printf("%d", asd->mNumMeshes);
}

MeshLoader::~MeshLoader() {
	
}

//Mesh& MeshLoader::loadMesh(const std::string& path) {
//
//}

Mesh* MeshLoader::getQuad() {
	std::vector<Mesh::Vertex> vertices{
		Mesh::Vertex{ { -1, 1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 0, 1 }},
		Mesh::Vertex{ { 1, 1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 1, 1 }},
		Mesh::Vertex{ { 1, -1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 1, 0 }},
		Mesh::Vertex{ { -1, -1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 0, 0 }}
	};
	std::vector<unsigned short> indices{ 0, 2, 1, 2, 0, 3 };
	return new Mesh(vertices, indices);
};