#include "meshloader.hpp"
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


MeshLoader::MeshLoader() {

	
}

MeshLoader::~MeshLoader() {

}

//Mesh& MeshLoader::loadMesh(const std::string& path) {
//
//}

std::vector<Mesh> MeshLoader::loadMesh(const char* path) {
	const aiScene* test = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	//aiMesh* mesh = test->mMeshes[1];
	//printf("%d", mesh);

	aiMesh* mesh;
	std::vector<Mesh> meshes;
	std::vector <Mesh::Vertex> vertices;
	std::vector<unsigned short> indices;
	for (int i = 0; i < test->mNumMeshes; i++) {
		mesh = test->mMeshes[i];
		for (int j = 0; j < mesh->mNumVertices; j++) {
			Mesh::Vertex vertex;
			glm::vec3 tmpVec;

			tmpVec.x = mesh->mVertices[i].x;
			tmpVec.y = mesh->mVertices[i].y;
			tmpVec.z = mesh->mVertices[i].z;
			vertex.pos = tmpVec;

			tmpVec.x = mesh->mNormals[i].x;
			tmpVec.y = mesh->mNormals[i].y;
			tmpVec.z = mesh->mNormals[i].z;
			vertex.normal = tmpVec;




			vertices.push_back(vertex);
		}

		for (int j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++) {
				indices.push_back(face.mIndices[k]);
			}
		}
		meshes.push_back(Mesh(vertices, indices));
	}
	return meshes;
}

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